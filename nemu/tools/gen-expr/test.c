#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <limits.h>
#include <assert.h>
#include <stdbool.h>

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
  {"\\$(zero|0|ra|tp|sp|a[0-7]|t[0-8]|rs|fp|s[0-8])", TK_REGISTER},
  {"[a-zA-Z_][a-zA-Z0-9_]*", TK_IDENTIFIER},
  {"&&", TK_AND},
  {"\\|\\|", TK_OR},
};

#define ARRLEN(arr) (sizeof(arr) / sizeof((arr)[0]))
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
            strncpy(tokens[nr_token].str, substr_start + 2, substr_len - 2);
            tokens[nr_token].str[substr_len - 2] = '\0';
            nr_token++;
            break;
          case TK_REGISTER:
            if (substr_len > 1 && substr_start[0] == '$') {
              substr_start++;
              substr_len--;
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

static int evaluate_register(const char *str) {
    bool success;
    int reg_value = 114514;
    if (success) {
        return reg_value;
    } else {
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

long eval(int p, int q) {
    if (p > q) {
        return 0;
    } else if (check_parentheses(p, q)) {
        return eval(p + 1, q - 1);
    } else {
        int level = 0, op = -1, i;
        int min_precedence = INT_MAX;
        for (i = q; i >= p; i--) {
            if (tokens[i].type == '(')
                level++;
            else if (tokens[i].type == ')')
                level--;
            else if (level == 0 &&
                      (tokens[i].type == '+' || tokens[i].type == '-' || tokens[i].type == '*' 
                      || tokens[i].type == '/' || tokens[i].type == '!' || tokens[i].type == TK_AND || tokens[i].type == TK_OR)) {
                int precedence = get_precedence(tokens[i].type);
                if (precedence < min_precedence) {
                    min_precedence = precedence;
                    op = i;
                }
            }
        }

        if (op == -1) {
            if (tokens[p].type == TK_HEX) {
                int val;
                sscanf(tokens[p].str, "%x", &val);
                return val;
            } else if (tokens[p].type == TK_REGISTER) {
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
        long val1 = eval(p, op - 1);
        long val2 = eval(op + 1, q);
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
long expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }
  *success = true;
  return eval(0, nr_token - 1);
}
int main() {
    FILE *file = fopen("input", "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }
    char line[256];
    int f_len = 0;
    while (fgets(line, sizeof(line), file) != NULL) {
        long result;
        char expression[256];
        if (sscanf(line, "%ld %255s", &result, expression) == 2) {
            f_len++;
            bool success = true;
            init_regex();
            long res = expr(expression, &success);
            if (!success) {
                puts("invalid expression!");
            } else {
                assert(result == res);
            }
        } else {
            printf("Invalid line: %s", line);
        }
    }
    fclose(file);
    return 0;
}