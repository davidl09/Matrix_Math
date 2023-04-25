#include "matrix.h"

int main(void){
    /*
    double num;
    Rational rational, rational1;
    while(1){
        input_rational(&rational);
        printf("You entered: %d/%d, approx. %lf\n", rational.numerator, rational.denominator, rational.dec_est);
        input_rational(&rational1);
        printf("You entered: %d/%d, approx. %lf\n", rational1.numerator, rational1.denominator, rational1.dec_est);
        rat_sum(&rational, &rational1);
        printf("The answer is %d/%d, approx. %lf\n", rational.numerator, rational.denominator, rational.dec_est);
    }
    */

    Matrix *matrix1, *matrix2;
    matrix1 = input_mat();
    matrix2 = input_mat();
    Matrix* result = mat_prod(matrix1, matrix2);
    print_mat(result);
    return 0;
}