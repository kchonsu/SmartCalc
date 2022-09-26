#ifndef SRC_S21_CALC_H_
#define SRC_S21_CALC_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//        Q - sinus
//        w - cos
//        e - tan
//        Z - asin
//        X - acos
//        C - atan
//        V - sqrt
//        B - ln
//        N - log

typedef struct st{
    int priority;
    char symbol;
    struct st* next;
} st_t;

typedef struct vl{
    double value;
    struct vl* next;
} vl_l;

// основная часть
int input_data(char *input);
double s21_parser(char *input, double x_value);
double calculating(char *output);
double calculating_op(double a, double b, char operatorr);
double calculating_func(double a, char func);

// функции для стека
void push(st_t **top, int priority, char symbol);
char pop(st_t **top);
void operator_st_add(st_t **top, char operator_res);
void func_st_add(st_t **top, char *func_res);

// функции для 2 стека
void push_v(vl_l **top, double value);
double pop_v(vl_l **top);

// вспомогательные функции
int is_digit(char *input);
char is_operator(char *input);
char* is_func(char *input);
int see_priority(char operator_res);
int peek_priority(const st_t *top);
int check_operators(char *input);
//char func_type(char *func_res);

#endif // SRC_S21_CALC_H_