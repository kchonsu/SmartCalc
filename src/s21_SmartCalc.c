#include "s21_SmartCalc.h"

//int main() {
//    char *input = "48*6+1*(-6)+sin(sqrt(ln(34)))";
//    printf("%lf\n", s21_parser(input, 0.05));
//    return 0;
//}

int input_data(char *input) {
    int res = 0;
    int len = (int)strlen(input);
    if (len > 255) res = 1;
    if (res == 0) {
        int bracket = 0;
        int right;
        char allow_symbols[40] = {'0', '1', '2', '3', '4', '5',
                                  '6', '7', '8', '9', '+', '-',
                                  '(', ')', '*', '/', '^', '=',
                                  'a', 'c', 'd', 'o', 's', 't',
                                  'i', 'q', 'r', 'n', 'l',
                                  'm', 'x', 'g', '.', ' '};
        while(*input) {
            right = 0;
            for (size_t j = 0; j < strlen(allow_symbols); j++) {
                if (*input == allow_symbols[j]) right = 1;
            }
            if (right == 0) {
                res = 1;
                break;
            }
            if (*input == '(') bracket++;
            if (*input == ')') bracket--;
            res += check_operators(input);
            input++;
        }
        if (bracket != 0) res = 1;
    }
    return res;
}

double s21_parser(char *input, double x_value) {
    double res = 0;
    char *output = calloc(512, sizeof (char));
    if (!input_data(input)) {
        char x_str[256] = "\0";
        snprintf(x_str, 255, "%lf", x_value);
        st_t *top = NULL;
        int digit_res = 0;
        char operator_res = 0;
        char *func_res = NULL;
        char pop_res = 0;
        char prev_symbol = ' ';
        char *digit = calloc(512, sizeof (char));
        int j = 0, i = 0, dot_count = 0;
        while (*input) {
            digit_res = is_digit(input);
            operator_res = is_operator(input);
            func_res = is_func(input);
            if (top != NULL && prev_symbol == '(' &&
            (operator_res == '-' || operator_res == '+')) {
                output[i++] = '0';
                output[i++] = ' ';
            }
            if (func_res != NULL) {
                if (strncmp(func_res, "error", 5) == 0) {
                    output = NULL;
                    func_res = NULL;
                    break;
                }
                input += (strlen(func_res) - 1);
                if (strncmp(func_res, "mod", 3) == 0) {
                    operator_res = 'm';
                    func_res = NULL;
                }
                if (func_res != NULL) {
                    func_st_add(&top, func_res);
                }
            }
            if (digit_res == 1) digit = strncat(digit, input, 1);
            if (digit_res == 2) {
                if (dot_count == 0) {
                    digit = strncat(digit, input, 1);
                    dot_count++;
                }
            }
            if (digit_res == 3) {
                for (j = 0; j < (int)strlen(x_str); j++) output[i++] = x_str[j];
                output[i++] = ' ';
            }
            if (digit_res == 0 && strlen(digit) > 0) {
                for (j = 0; j < (int)strlen(digit); j++) output[i++] = digit[j];
                dot_count = 0;
                output[i++] = ' ';
                digit[0] = '\0';
            }
            if (operator_res != 0) {
                if (operator_res == ')' || operator_res != '(') {
                    while (top != NULL && see_priority(operator_res) <= peek_priority(top)) {
                        pop_res = pop(&top);
                        if (pop_res != '(') {
                            output[i++] = pop_res;
                            output[i++] = ' ';
                        }
                        if (pop_res == '(') break;
                    }
                }
                operator_st_add(&top, operator_res);
            }
            prev_symbol = operator_res;
            input++;
        }
        if (output != NULL) {
            if (digit[0] != '\0') {
                for (j = 0; j < (int)strlen(digit); j++, i++) output[i] = digit[j];
                output[i++] = ' ';
            }
            while (top != NULL) {
                pop_res = pop(&top);
                if (pop_res != '(') {
                    output[i++] = pop_res;
                    output[i++] = ' ';
                }
            }
        }
        res = calculating(output);
        free(digit);
        free(top);
        free(func_res);
    }
    free(output);
    return res;
}

double calculating(char *output) {
    double res;
    double calc_op_res = 0;
    double calc_func_res = 0;
    vl_l *val = NULL;
    while(*output) {
        if (is_digit(output) == 1) {
            push_v(&val, atof(output));
            while(*output != ' ') output++;
            continue;
        }
        if ((is_operator(output) != 0 || *output == 'm') && *output != ' ') {
            calc_op_res = calculating_op(pop_v(&val), pop_v(&val), *output);
            push_v(&val, calc_op_res);
        }
        if (*output >= 'a' && *output <= 'z' && *output != 'm' && *output != ' ') {
            calc_func_res = calculating_func(pop_v(&val), *output);
            push_v(&val, calc_func_res);
        }
        output++;
    }
    res = pop_v(&val);
    while (val != NULL) pop_v(&val);
    return res;
}

int check_operators(char *input) {
    int res = 0;
    static int oper_count = 0;
    static int digit_count = 0;
    static int bracket_count = 0;
    static int protect = 0;
    char c = is_operator(input);
    int i = is_digit(input);
    if (c != 0 && c != '(' && c != ')') {
        oper_count++;
        digit_count = 0;
    }
    if (c == '(' || c == ')') {
        digit_count = 0;
    }
    if (i > 0) {
        digit_count++;
        oper_count = 0;
    }
    if (c == '(') {
        bracket_count++;
    } else if ((c == '-' || c == '+') && bracket_count > 0 && protect == 0) {
        oper_count--;
        protect++;
    } else if (c == ')') {
        bracket_count = 0;
        protect = 0;
    }

    if (oper_count > 1 && digit_count == 0) {
        res = 1;
    }
    return res;
}

double calculating_op(double a, double b, char operatorr) {
    double res = 0;
    if (operatorr == '+') res = b+a;
    if (operatorr == '-') res = b-a;
    if (operatorr == '*') res = b*a;
    if (operatorr == '/') res = b/a;
    if (operatorr == 'm') res = fmod(b, a);
    if (operatorr == '^') res = pow(b, a);
    return res;
}

double calculating_func(double a, char func) {
    double res = 0;
    if (func == 'q') res = sin(a);
    if (func == 'w') res = cos(a);
    if (func == 'e') res = tan(a);
    if (func == 'z') res = asin(a);
    if (func == 'x') res = acos(a);
    if (func == 'c') res = atan(a);
    if (func == 'v') res = sqrt(a);
    if (func == 'b') res = log(a);
    if (func == 'n') res = log10(a);
    return res;
}

int peek_priority(const st_t *top) {
    return top->priority;
}

int see_priority(char operator_res) {
    int res = 0;
    if (operator_res == '+') res = 1;
    if (operator_res == '-') res = 1;
    if (operator_res == '*') res = 2;
    if (operator_res == '/') res = 2;
    if (operator_res == 'm') res = 2;
    if (operator_res == '^') res = 3;
    if (operator_res == '(') res = 0;
    if (operator_res == ')') res = 0;
    return res;
}

void operator_st_add(st_t **top, char operator_res) {
    if (operator_res == '+') push(top, 1, '+');
    if (operator_res == '-') push(top, 1, '-');
    if (operator_res == '*') push(top, 2, '*');
    if (operator_res == '/') push(top, 2, '/');
    if (operator_res == 'm') push(top, 2, 'm');
    if (operator_res == '^') push(top, 3, '^');
    if (operator_res == '(') push(top, 0, '(');
}

char is_operator(char *input) {
    char res = 0;
    char operators[10] = {'+', '-','(', ')', '*', '/', '^'};
    for (size_t i = 0; i < strlen(operators); i++) {
        if (*input == operators[i]) res = operators[i];
    }
    return res;
}

int is_digit(char *input) {
    int res = 0;
    char digits[15] = {'0', '1', '2', '3', '4', '5',
                     '6', '7', '8', '9'};
    for (size_t i = 0; i < strlen(digits); i++) {
        if (*input == digits[i]) res = 1;
    }
    if (*input == '.') res = 2;
    if (*input == 'x') res = 3;
    return res;
}

char* is_func(char *input) {
    char *res = NULL;
    if (*input == 's') {
        if (strncmp(input, "sin", 3) == 0) {
            res = "sin";
        } else if (strncmp(input, "sqrt", 4) == 0) {
            res = "sqrt";
        } else {
            res = "error";
        }
    }
    if (*input == 'c') {
        if (strncmp(input, "cos", 3) == 0) {
            res = "cos";
        } else {
            res = "error";
        }
    }
    if (*input == 't') {
        if (strncmp(input, "tan", 3) == 0) {
            res = "tan";
        } else {
            res = "error";
        }
    }
    if (*input == 'a') {
        if (strncmp(input, "asin", 4) == 0) {
            res = "asin";
        } else if (strncmp(input, "acos", 4) == 0) {
            res = "acos";
        } else if (strncmp(input, "atan", 4) == 0) {
            res = "atan";
        } else {
            res = "error";
        }
    }
    if (*input == 'l') {
        if (strncmp(input, "ln", 2) == 0) {
            res = "ln";
        } else if (strncmp(input, "log", 3) == 0) {
            res = "log";
        } else {
            res = "error";
        }
    }
    if (*input == 'm') {
        if (strncmp(input, "mod", 3) == 0) {
            res = "mod";
        } else {
            res = "error";
        }
    }
    return res;
}

//char func_type(char *func_res) {
//    char res = '?';
//    if (strcmp(func_res, "sin") == 0) res = 'q';
//    if (strcmp(func_res, "cos") == 0) res = 'w';
//    if (strcmp(func_res, "tan") == 0) res = 'e';
//    if (strcmp(func_res, "asin") == 0) res = 'z';
//    if (strcmp(func_res, "acos") == 0) res = 'x';
//    if (strcmp(func_res, "atan") == 0) res = 'c';
//    if (strcmp(func_res, "sqrt") == 0) res = 'v';
//    if (strcmp(func_res, "ln") == 0) res = 'b';
//    if (strcmp(func_res, "log") == 0) res = 'n';
//    return res;
//}

void func_st_add(st_t **top, char *func_res) {
    if (strcmp(func_res, "sin") == 0) push(top, 4, 'q');
    if (strcmp(func_res, "cos") == 0) push(top, 4, 'w');
    if (strcmp(func_res, "tan") == 0) push(top, 4, 'e');
    if (strcmp(func_res, "asin") == 0) push(top, 4, 'z');
    if (strcmp(func_res, "acos") == 0) push(top, 4, 'x');
    if (strcmp(func_res, "atan") == 0) push(top, 4, 'c');
    if (strcmp(func_res, "sqrt") == 0) push(top, 4, 'v');
    if (strcmp(func_res, "ln") == 0) push(top, 4, 'b');
    if (strcmp(func_res, "log") == 0) push(top, 4, 'n');
}

void push(st_t **top, int priority, char symbol) {
    st_t *q = calloc(1, sizeof(st_t));
    q->priority = priority;
    q->symbol = symbol;
    if (*top == NULL) {  // Если стек пустой
        *top = q;
    } else {  // Если стек не пустой
        q->next = *top;
        *top = q;
    }
}

void push_v(vl_l **top, double value) {
    vl_l *q = calloc(1, sizeof(vl_l));
    q->value = value;
    if (*top == NULL) {  // Если стек пустой
        *top = q;
    } else {  // Если стек не пустой
        q->next = *top;
        *top = q;
    }
}

char pop(st_t **top) {
    char type = 0;
    if (*top != NULL) {
        st_t *out = NULL;
        out = (*top);
        (*top) = (*top)->next;
        type = out->symbol;
        free(out);
    }
    return type;
}

double pop_v(vl_l **top) {
    double value = 0;
    if (*top != NULL) {
        vl_l *out = NULL;
        out = (*top);
        (*top) = (*top)->next;
        value = out->value;
        free(out);
    }
    return value;
}
