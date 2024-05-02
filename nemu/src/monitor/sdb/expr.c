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
#include <limits.h>

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
  {"\\$(pc|0|ra|tp|sp|a[0-7]|t[0-8]|rs|fp|s[0-8])", TK_REGISTER}, // reg
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
  char str[64];
} Token;

static Token tokens[64] __attribute__((used)) = {};
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
      if(nr_token >= 64){
        printf("The expression is too long!\n");
        return false;
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

int get_precedence(char op) {
    if (op == '*' || op == '/')
        return 2;
    else if (op == '+' || op == '-')
        return 1;
    else if (op == '!' || op == TK_AND || op == TK_OR)
        return 3;
    else
        return 0;
}
int eval(int p, int q) {
    if (p > q) {
        return 0;
    } else if (check_parentheses(p, q)) {
        return eval(p + 1, q - 1);
    } else {
        int level = 0, op = -1, i;
        int min_precedence = INT_MAX; // 记录最低优先级的运算符
        for (i = q; i >= p; i--) {
            if (tokens[i].type == '(')
                level++;
            else if (tokens[i].type == ')')
                level--;
            else if (level == 0 &&
                      (tokens[i].type == '+' || tokens[i].type == '-' || tokens[i].type == '*' 
                      || tokens[i].type == '/' || tokens[i].type == '!' || tokens[i].type == TK_AND || tokens[i].type == TK_OR)) {
                // 计算运算符的优先级
                int precedence = get_precedence(tokens[i].type);
                if (precedence < min_precedence) {
                    min_precedence = precedence;
                    op = i;
                }
            }
        }

        if (op == -1) {
            if (q - p >= 64) { // Adjust the threshold as needed
              printf("Expression is too long. Returning 0. Warning: Expression is lengthy.\n");
              return 0;
            }
            // 处理数字和其他类型表达式
            if (tokens[p].type == TK_HEX) {
                int val;
                sscanf(tokens[p].str, "%x", &val);
                return val;
            } else if (tokens[p].type == TK_REGISTER) {
              int substr_len = strlen(tokens[p].str);
              char *substr_start = tokens[p].str;
              if (substr_len > 1 && substr_start[0] == '$') {
                substr_start++; // 跳过 $
                substr_len--; // 长度减一
              }
              int val = evaluate_register(tokens[p].str);
              return val;
            } else {
                int val;
                long long temp=0;
                sscanf(tokens[p].str, "%lld", &temp);
                if(temp >= INT_MAX || temp <= INT_MIN){
                  printf("error: Spill values are detected\n");
                  return -1;
                }else{
                  sscanf(tokens[p].str, "%d", &val);
                }
                return val;
            }
        }

        
        int val1 = eval(p, op - 1);
        int val2 = eval(op + 1, q);
        switch (tokens[op].type) {
            case '+':
                if ((val1 > 0 && val2 > INT_MAX - val1) || 
                    (val1 < 0 && val2 < INT_MIN - val1)) {
                    printf("error: Integer overflow detected in addition\n");
                    return -1;
                }
                if ((val2 > 0 && (val1 > INT_MAX - val2 || val1 > INT_MAX)) || 
                    (val2 < 0 && (val1 < INT_MIN - val2 || val1 < INT_MIN))) {
                    printf("error: Integer overflow detected in addition\n");
                    return -1;
                } else {
                    return val1 + val2;
                }
            case '-':
                if ((val2 > 0 && val1 < INT_MIN + val2) || (val2 < 0 && val1 > INT_MAX + val2)) {
                    printf("error: Integer overflow detected in subtraction\n");
                    return -1;
                } else {
                    return val1 - val2;
                }
            case '*':
                if (val1 > 0 && (val2 > INT_MAX / val1 || val2 < INT_MIN / val1)) {
                    printf("error: Integer overflow detected in multiplication\n");
                    return -1;
                } else if (val1 < 0 && (val2 < INT_MAX / val1 || val2 > INT_MIN / val1)) {
                    printf("error: Integer overflow detected in multiplication\n");
                    return -1;
                } else {
                    return val1 * val2;
                }
            case '/':
                if (val2 == 0) {
                    printf("error: The divisor cannot be '0'\n");
                    return -1;
                } else if (val1 == INT_MIN && val2 == -1) {
                    printf("error: Integer overflow detected in division\n");
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