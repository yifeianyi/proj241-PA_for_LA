/***************************************************************************************
* Copyright (c) 2014-2022 Zihao Yu, Nanjing University
*
* NEMU is licensed under Mulan PSL v2.
* You can use this software according to the terms and conditions of the Mulan PSL v2.
* You may obtain a copy of Mulan PSL v2 at:
*          http://license.coscl.org.cn/MulanPSL2
*
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
* EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
* MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
*
* See the Mulan PSL v2 for more details.
***************************************************************************************/

#include <isa.h>

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>

enum {
  TK_NOTYPE = 256, TK_EQ, TK_NEQ,
  TK_NUMBER, TK_FLOAT, TK_IDENTIFIER,
  TK_AND, TK_OR, TK_HEX, TK_REGISTER
};

static struct rule {
  const char *regex;
  int token_type;
} rules[] = {

  {" +", TK_NOTYPE},
  {"\\+", '+'},
  {"\\-", '-'},
  {"\\*", '*'},
  {"\\(", '('},
  {"\\)", ')'},
  {"/", '/'},
  {"!", '!'},
  {"==", TK_EQ},
  {"!=", TK_NEQ},
  {"0[xX][0-9a-fA-F]+", TK_HEX},
  {"[0-9]+", TK_NUMBER},
  {"[0-9]+\\.[0-9]+", TK_FLOAT},
  {"\\$(zero|0|ra|tp|sp|a[0-7]|t[0-8]|rs|fp|s[0-8])", TK_REGISTER}, // reg
  {"[a-zA-Z_][a-zA-Z0-9_]*", TK_IDENTIFIER}, // 标识符（变量名等）
  {"&&", TK_AND},
  {"\\|\\|", TK_OR},
};

#define NR_REGEX ARRLEN(rules)

static regex_t re[NR_REGEX] = {};

void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

typedef struct token {
  int type;
  char str[32];
} Token;

static Token tokens[32] __attribute__((used)) = {};
static int nr_token __attribute__((used))  = 0;

static bool make_token(char *e) {
  int position = 0;
  int i;
  regmatch_t pmatch;
  nr_token = 0;

  while (e[position] != '\0') {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i ++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;

        position += substr_len;

        switch (rules[i].token_type) {
          case TK_NOTYPE:
            break;
          case TK_HEX:
            tokens[nr_token].type = rules[i].token_type;
            strncpy(tokens[nr_token].str, substr_start + 2, substr_len - 2); // 跳过 "0x"
            tokens[nr_token].str[substr_len - 2] = '\0';
            nr_token++;
            break;
          case TK_REGISTER:
            // 剥离 $ 符号
            if (substr_len > 1 && substr_start[0] == '$') {
              substr_start++; // 跳过 $
              substr_len--; // 长度减一
            }
            tokens[nr_token].type = rules[i].token_type;
            strncpy(tokens[nr_token].str, substr_start, substr_len);
            tokens[nr_token].str[substr_len] = '\0';
            nr_token++;
            break;
          default:
            tokens[nr_token].type = rules[i].token_type;
            strncpy(tokens[nr_token].str, substr_start, substr_len);
            tokens[nr_token].str[substr_len] = '\0';
            nr_token++;
        }

        break;
      }
    }

    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }
  
  return true;
}

static bool check_parentheses(int p, int q) {
    int i, cnt = 0;
    if (tokens[p].type != '(' || tokens[q].type != ')') 
        return false;
    for (i = p; i <= q; i++) {
        if (tokens[i].type == '(') 
            cnt++;
        else if (tokens[i].type == ')') 
            cnt--;
        if (cnt == 0 && i < q) 
            return false;
    }
    if (cnt < 0) 
        return false;
    return true;
}

static word_t evaluate_register(const char *str) {
    bool success;
    word_t reg_value = isa_reg_str2val(str, &success);
    if (success) {
        return reg_value;
    } else {
        // 如果没有找到匹配的寄存器，返回 -1
        return -1;
    }
}
int eval(int p, int q) {
    if (p > q) {
        return 0;
    } else if (check_parentheses(p, q)) {
        return eval(p + 1, q - 1);
    } else {
        int level = 0, op = -1, i;
        int op_priority[128] = {0}; // 定义运算符优先级数组
        op_priority['+'] = op_priority['-'] = 1; // 设置加减法优先级为1
        op_priority['*'] = op_priority['/'] = 2; // 设置乘除法优先级为2
        op_priority['!'] = 3; // 设置逻辑非优先级为3
        op_priority[TK_AND] = op_priority[TK_OR] = 4; // 设置逻辑与和逻辑或优先级为4
        
        int max_priority = -1; // 记录当前优先级最高的运算符
        for (i = q; i >= p; i--) {
            if (tokens[i].type == '(')
                level++;
            else if (tokens[i].type == ')')
                level--;
            else if (level == 0 && op_priority[tokens[i].type] > max_priority) {
                op = i;
                max_priority = op_priority[tokens[i].type];
            }
        }

        if (op == -1) {
            // Handle hexadecimal and register expressions
            if (tokens[p].type == TK_HEX) {
                int val;
                sscanf(tokens[p].str, "%x", &val);
                return val;
            } else if (tokens[p].type == TK_REGISTER) {
                int val = evaluate_register(tokens[p].str);
                return val;
            } else {
                int val;
                sscanf(tokens[p].str, "%d", &val);
                return val;
            }
        }

        int val1 = eval(p, op - 1);
        int val2 = eval(op + 1, q);
        switch (tokens[op].type) {
            case '+':
                return val1 + val2;
            case '-':
                return val1 - val2;
            case '*':
                return val1 * val2;
            case '/':
                if (val2 == 0) {
                    printf("error: The divisor cannot be '0'\n");
                    return -1;
                } else {
                    return val1 / val2;
                }
            case TK_AND:
                return val1 && val2;
            case TK_OR:
                return val1 || val2;
            case '!':
                return !val2;
            default:
                return 0;
        }
    }
}

word_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }

  *success = true;
  return eval(0, nr_token - 1);
}
