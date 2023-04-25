#include "matrix.h"

int main(void){
    double num;
    Rational rational, rational1;
    while(1){
        /*
        printf("Enter a decimal number\n");
        scanf("%lf", &num);
        if(set_rat_f(&rational, num) == NULL)
            exit(1);
        printf("The fractional expression of %lf is %d/%d\n",
               rational.dec_est, rational.numerator, rational.denominator
        );*/
        //printf("%d", gcf(400, 1000));
        printf("Enter a fraction in this format: 'x/x', or a decimal number\n");
        input_rational(&rational);
        printf("You entered: %d/%d, approx. %lf\n", rational.numerator, rational.denominator, rational.dec_est);
        printf("Enter another fraction in this format: 'x/x', or a decimal number\n");
        input_rational(&rational1);
        printf("You entered: %d/%d, approx. %lf\n", rational1.numerator, rational1.denominator, rational1.dec_est);
        rat_r_prod(&rational, &rational1);
        printf("The answer is %d/%d, approx. %lf\n", rational.numerator, rational.denominator, rational.dec_est);
    }


}
