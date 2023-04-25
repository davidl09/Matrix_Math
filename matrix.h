//
// Created by dalae on 2023-04-16.
//

#ifndef MATRIX_MATH_MATRIX_H
#define MATRIX_MATH_MATRIX_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

//Macro defs
#define MAXROWS 64
#define MAXCOLS 64

#define min(num1, num2) (num1 > num2 ? num2 : num1)
#define max(num1, num2) (num1 > num2 ? num1 : num2)

typedef struct Rational{
    int numerator;
    int denominator;
    double dec_est; //slave to numerator/denominator
}Rational;

typedef struct Matrix{
    int rows;
    int columns;
    Rational** self;
}Matrix;


Rational* set_rat_d(Rational*  rational);
Rational* set_rat_f(Rational* rational, double val);
Rational* rat_sum(Rational* num1, Rational* num2);
Rational* rat_dif(Rational* num1, Rational* num2);
Rational* rat_s_prod(Rational* num, double scalar);
Rational* rat_r_prod(Rational* num1, Rational* num2);
Rational* rat_r_quot(Rational* num1, Rational* num2);
Rational* check_norm(Rational* num);
Rational* input_rational(Rational* num);
Rational* rnorm(Rational* rational);

unsigned int is_prime(int num);
unsigned int gcf(int num1, int num2);
unsigned int prime_fact(int num);
unsigned int* list_prime_fact(int num);
unsigned int round_e2(double num);

void* alloc_mat(int rows, int columns);

#endif //MATRIX_MATH_MATRIX_H
