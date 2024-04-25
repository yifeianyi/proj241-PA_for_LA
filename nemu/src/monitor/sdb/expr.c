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

};

static struct rule {
  const char *regex;
  int token_type;
} rules[] = {

  {" +", TK_NOTYPE},    // spaces
  {"\\+", '+'},
  {"\\-", '-'},
  {"\\*", '*'},
  {"\\(", '('},
  {"\\)", ')'},
  {"/", '/'},
  {"==", TK_EQ},        // equal
  {"!=", TK_NEQ},       // nequal

  {"[0-9]+", TK_NUMBER},// 整数
  {"[0-9]+\\.[0-9]+", TK_FLOAT}, // 浮点数
  {"[a-zA-Z_][a-zA-Z0-9_]*", TK_IDENTIFIER}, // 标识符（变量名等）
};

#define NR_REGEX ARRLEN(rules)

static regex_t re[NR_REGEX] = {};

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
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

        //Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
        //    i, rules[i].regex, position, substr_len, substr_len, substr_start);

        position += substr_len;

        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */

        switch (rules[i].token_type) {
          case TK_NOTYPE:
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

int eval(int p, int q) {
    if (p > q) {
        // 表达式为空，返回 0
        return 0;
    } else if (check_parentheses(p, q)) {
        // 如果整个表达式被一对括号包围，则去掉括号继续递归计算
        return eval(p + 1, q - 1);
    } else {
        // 查找最低优先级的运算符
        int level = 0, op = -1, i;
        for (i = q; i >= p; i--) {
            if (tokens[i].type == '(')
                level++;
            else if (tokens[i].type == ')')
                level--;
            else if (level == 0 &&
                     (tokens[i].type == '+' || tokens[i].type == '-')) {
                op = i;
                break;
            }
        }
        if (op == -1) {
            // 没有找到加减号，继续查找更低优先级的运算符
            for (i = q; i >= p; i--) {
                if (tokens[i].type == '(')
                    level++;
                else if (tokens[i].type == ')')
                    level--;
                else if (level == 0 &&
                         (tokens[i].type == '*' || tokens[i].type == '/')) {
                    op = i;
                    break;
                }
            }
        }

        if (op == -1) {
            // 没有找到任何运算符，说明表达式是一个数值
            int val;
            sscanf(tokens[p].str, "%d", &val);
            return val;
        }

        // 递归计算左右两边的表达式，并根据运算符进行计算
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
                return val1 / val2;
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
