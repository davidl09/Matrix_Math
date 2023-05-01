#include "matrix.h"

int main(void){
    /*
    double num;
    Rational rational, rational1;
    int i = 0;
    while(i < 1){
        input_rational(&rational);
        printf("You entered: %lld/%lld, approx. %lf\n", rational.numerator, rational.denominator, rational.dec_est);
        input_rational(&rational1);
        printf("You entered: %lld/%lld, approx. %lf\n", rational1.numerator, rational1.denominator, rational1.dec_est);
        rat_sum(&rational, &rational1);
        printf("The answer is %lld/%lld, approx. %lf\n", rational.numerator, rational.denominator, rational.dec_est);
        i++;
    }
    
*/
    Matrix *matrix1, *matrix2;
    matrix1 = input_mat();
    if(is_ref(matrix1))
        printf("Matrix is in REF");
    else printf("Matrix is not in REF");
    matrix2 = input_mat();
    Matrix* result = mat_prod(matrix1, matrix2);
    if(result)
        print_mat(result);
    else exit(1);
    mat_free(result);
    return 0;
}