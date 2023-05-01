//
// Created by dalae on 2023-04-16.
//

#include "matrix.h"
#include <stdio.h>

unsigned int is_prime(int num){
    num = abs(num);
    for (int i = 2; i <= num/2; ++i) {
        if(num % i == 0)
            return 0;
    }
    return 1;
}


Rational* input_rational(Rational* num){
    double numer, denom;
    char* input[3];
    int dec_pts = 0;
    input[0] = malloc(40);

    printf("Enter a fraction, decimal number, or combination thereof\n");
    scanf("%s", input[0]);

    switch(sscanf(input[0], "%lf/%lf", &numer, &denom)){ //check for invalid input
        case 0:
            printf("Not a valid input, please try again\n");
            free(input[0]);
            return input_rational(num);
        case 1: 
            denom = 1; 
            strtok(input[0], ".");
            dec_pts = ((input[1] = strtok(NULL, ".")) != NULL ? strlen(input[1]) : 0);
            break;
        case 2:
            if(denom == 0.0){
                printf("Divide by zero, please try again\n");
                free(input[0]);
                return input_rational(num);
            }

            input[0] = strtok(input[0], "/"); //ptr to numerator
            input[1] = strtok(NULL, "/"); //ptr to denominator - ptr[3] is temp buffer
            
            strtok(input[0], ".");
            input[2] = strtok(NULL, ".");
            dec_pts = (input[2] != NULL ? strlen(input[2]) : 0); //num of decimal places on numerator

            strtok(input[1], ".");
            input[2] = strtok(NULL, ".");
            dec_pts = (dec_pts >= (input[2] != NULL ? strlen(input[2]) : 0) ? dec_pts : strlen(input[2]));
            break;

        default: 
            printf("Not a valid input, please try again\n");
            free(input[0]);
            return input_rational(num);
    }

    num->numerator = (int)round(numer * pow(10, dec_pts));
    num->denominator = (int)round(denom * pow(10, dec_pts));

    if(num->numerator != 0) rnorm(num);
    
    free(input[0]);
    return num;
}

Rational* set_rat_d(Rational* rational){
    //set a rational's double precision estimate from a given fraction
    if(rational->denominator == 0)
        return NULL;
    rational->dec_est = (double)rational->numerator/(double)rational->denominator;
    return rational;
}

Rational* rnorm(Rational* rational){
    //call after manipulating rational expressions, ensures conformity with sign convention and reduces fraction if necessary
    if(rational->denominator < 0){ //make denominator always positive
        rational->denominator *= -1;
        rational->numerator *= -1;
    }
    if(rational->numerator == 0){
        rational->dec_est = 0.0;
        return rational;
    }

    unsigned int gfact = gcf(rational->numerator, rational->denominator);
    if(gfact == 0)
        return NULL;
    rational->numerator /= (int)gfact;
    rational->denominator /= (int)gfact;
    rational->dec_est = (double)rational->numerator/rational->denominator;
    return rational;
}

Rational* set_rat_f(Rational* rational, double val)
//set a rational's fractional part from a given float (7 digit precision)
{
    if(fabs(val - round(val)) < 1e-7){
        rational->dec_est = round(val);
        rational->numerator = (int)round(val);
        rational->denominator = 1;
        return rational;
    }

    unsigned int gfact = gcf(abs((int)round(val * 1e7)), (int)1e7);
    if(gfact == 0)
        return NULL;
    rational->dec_est = val;

    rational->numerator = abs((int)round(val * 1e7))/(int)gfact;
    rational->denominator = 10000000/(int)gfact;

    if(val < 0)
        rational->numerator *= -1;
    return rational;
}

Rational* rat_sum(Rational* num1, Rational* num2){
    //adds two rational objects and stores result in first object
    int temp = num1->denominator;
    num1->numerator *= num2->denominator;
    num1->denominator *= num2->denominator;

    num2->numerator *= temp;
    num2->denominator *= temp;

    if(num1->denominator == num2->denominator){
        num1->numerator += num2->numerator;
        rnorm(num1);
        rnorm(num2);
        return num1;
    }
    rnorm(num1);
    rnorm(num2);
    return NULL;
}

Rational* rat_dif(Rational* num1, Rational* num2){
//subtracts second rational from first and stores result in first object
    int temp = num1->denominator;
    num1->numerator *= num2->denominator; //cross multiply
    num1->denominator *= num2->denominator;

    num2->numerator *= temp;
    num2->denominator *= temp;

    if(num1->denominator == num2->denominator){
        num1->numerator -= num2->numerator;
        rnorm(num1);
        rnorm(num2);
        return num1;
    }
    rnorm(num1);
    rnorm(num2);
    return NULL;
}

Rational* rat_inv_s(Rational* num1){
    num1->numerator *= -1;
    rnorm(num1);
    return num1;
}

Rational* rat_s_prod(Rational* num, double scalar){
    if(set_rat_d(num)){
        set_rat_f(num, num->dec_est * scalar);
        return num;
    }
    return NULL;
}

Rational* rat_r_prod(Rational* num1, Rational* num2){
    num1->numerator *= num2->numerator;
    num1->denominator *= num2->denominator;
    rnorm(num1);
    return num1;
}

Rational rat_r_prod_c(Rational num1, Rational num2){
    num1.numerator *= num2.numerator;
    num1.denominator *= num2.denominator;
    rnorm(&num1);
    return num1;
}

Rational* rat_r_quot(Rational* num1, Rational* num2){
    if(num2->numerator == 0)
        return NULL;
    num1->numerator *= num2->denominator;
    num1->denominator *= num2->numerator;
    rnorm(num1);
    return num1;
}

void print_rat(Rational* rational){
    if(rational->numerator == 0){
        printf("      0      ");
        return;
    }
    switch(rational->denominator){
        case 1:
            printf("%6lld  ", rational->numerator);
            return;
        default:
            printf("%6lld/%lld  ", rational->numerator, rational->denominator);
            return;
    }
}

void mat_free(Matrix* mat){
    //deallocate all memory allocated to a matrix
    for (int i = 0; i < mat->columns; ++i) {
        free(mat->self[i]);
    }
    free(mat->self);
    free(mat);
}

int is_ref(Matrix* mat){
    for (int i = 0; i < mat->rows; ++i) {
        for (int j = 0; j < i; ++j) {
            if(mat->self[i][j].numerator != 0){ //if pivot column found
                for (int k = i + 1; k < mat->rows; ++k) {
                    if(mat->self[k][j].numerator != 0)
                        return 0;
                }
                break;
            }
        }
    }
    return 1;
}

int is_rref(Matrix* mat){
    for (int i = 0; i < mat->rows; ++i) {
        for (int j = 0; j < mat->columns; ++j) {
            if(mat->self[i][j].numerator != 0){ //if pivot column found
                for (int k = 0; k < mat->rows; ++k) {
                    if(k != i && mat->self[k][j].numerator !=  0)
                        return 0;
                }
                break;
            }
        }
    }
    return 1;
}

Matrix* mat_alloc(int rows, int columns){
    Matrix* matrix = malloc(sizeof(Matrix)); //allocate object on heap

    matrix->columns = columns;
    matrix->rows = rows;

    matrix->self = malloc(matrix->rows * sizeof(Rational*));
    for (int i = 0; i < rows; ++i) {
        matrix->self[i] = malloc(matrix->columns * sizeof(Rational));
        for (int j = 0; j < matrix->columns; ++j) {
            set_rat_f(&matrix->self[i][j], 0.0);
        }
    }

    return matrix;
}

Matrix* mat_alloc_r(Matrix* matrix){
//allocate memory to existing matrix

    matrix->self = malloc(matrix->rows * sizeof(Rational*));

    for (int i = 0; i < matrix->rows; ++i) {
        matrix->self[i] = malloc(matrix->columns * sizeof(Rational));
        for (int j = 0; j < matrix->columns; ++j) {
            set_rat_f(&matrix->self[i][j], 0.0);
        }
    }
    return matrix;
}

Matrix* mat_copy(Matrix* mat){
    //duplicates given matrix and returns pointer to copy
    Matrix* mat2 = mat_alloc(mat->rows, mat->columns);

    for (int i = 0; i < mat->rows; ++i) {
        for (int j = 0; j < mat->columns; ++j) {
            mat2->self[i][j] = mat->self[i][j];
        }
    }
    return mat2;
}

Matrix* mat_sum(Matrix* mat1, Matrix* mat2){
    if(mat1->rows != mat2->rows || mat1->rows != mat2->rows)
        return NULL;

    for (int i = 0; i < mat1->rows; ++i) {
        for (int j = 0; j < mat1->columns; ++j) {
            rat_sum(&mat1->self[i][j], &mat2->self[i][j]);
        }
    }

    return mat1;
}

Matrix* mat_dif(Matrix* mat1, Matrix* mat2){
    if(mat1->rows != mat2->rows || mat1->rows != mat2->rows)
        return NULL;

    for (int i = 0; i < mat1->rows; ++i) {
        for (int j = 0; j < mat1->columns; ++j) {
            rat_dif(&mat1->self[i][j], &mat2->self[i][j]);
        }
    }

    return mat1;
}

Matrix* mat_prod(Matrix* mat1, Matrix* mat2){
    if(mat1->columns != mat2->rows){
        fprintf(stderr, "Columns in mat1 must match rows in mat2");
        return NULL;
    }
    Rational temp;
    Matrix* result = mat_alloc(mat1->rows, mat2->columns);
    for (int i = 0; i < result->rows; ++i) {
        for (int j = 0; j < result->columns; ++j) { //iterate over rows and columns
            for (int k = 0; k < mat1->rows; ++k) {
                temp = rat_r_prod_c(mat1->self[i][k], mat2->self[k][j]);
                rat_sum(&result->self[i][j], &temp);
            }
        }
    }
    mat_free(mat1);
    mat_free(mat2);
    return result;
}

Matrix* mat_row_swap(Rational* row1, Rational* row2){

}

Matrix* mat_row_sm(Rational* row){

}

Matrix* mat_row_add(Rational* row1, Rational* row2, Rational coeff){
    
}

Matrix* input_mat(){
    //allocates mem and polls user to fill matrix

    char input[10];
    int x = 0;

    printf("Enter the number of rows and columns in the format 'RRxCC'.\n");
    scanf("%s", input);

    for (int i = 0; i < 10; ++i) {
        if(!(input[i] == 'x' || input[i] == 'X' || input[i] == '\n' || input[i] == '\0' || (input[i] >= '0' && input[i] <= '9'))){
            printf("That was not a valid input, please try again.\n");
            return input_mat();
        }
        if(input[i] == 'x' || input[i] == 'X') x = 1;
        if(input[i] == '\n' || input[i] == '\0'){
            input[i] = '\0';
            if(x == 0){
                printf("That was not a valid input, please try again.\n");
                return input_mat();
            }
            break;
        }
    }

    int rows = atoi(strtok(input, "xX"));
    int columns = atoi(strtok(NULL, "xX"));

    if(columns > MAXCOLS || rows > MAXROWS){
        printf("Matrix too large, please enter a smaller size\n");
        return input_mat();
    }

    Matrix* mat = mat_alloc(rows, columns);

    if(!mat){
        fprintf(stderr, "Memory fault\n");
        exit(1);
    }

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            printf("Row %d, column %d\n", i + 1, j + 1);
            input_rational(&(mat->self[i][j]));
        }
    }

    print_mat(mat);
    return mat;
}

void print_mat(Matrix* mat){
    for (int i = 0; i < mat->rows; ++i) {
        printf("[");
        for (int j = 0; j < mat->columns; ++j) {
            print_rat(&mat->self[i][j]);//printf(" %3.lld/%lld ", mat->self[i][j].numerator, mat->self[i][j].denominator);
        }
        printf(" ]\n\n");
    }
    printf("\n");
}

unsigned int lcm(int num1, int num2){
    return (num1 * num2)/gcf(num1, num2);
}

unsigned int gcf(int num1, int num2){

    if(max(num1, num2)%min(num1, num2) == 0) //simple checks to save time in some cases
        return min(num1, num2);
    if(max(num1, num2) < 10000){
        if((is_prime(num1) || is_prime(num2)))
            return 1;
    }

    int numFact[2], gcf = 1;
    long long int** factors = calloc(2, sizeof(long long int*));
    long long int* temp;

    factors[0] = calloc((round_e2(num1) + 1), sizeof(long long int));
    factors[1] = calloc((round_e2(num2) + 1), sizeof(long long int));

    if(factors[0] == NULL || factors[1] == NULL){
        fprintf(stderr, "Error allocating memory\n");
        return 0;
    }

    factors[0][0] = abs(num1);
    factors[1][0] = abs(num2);

    for(int i = 0; i<2; i++){
        numFact[i] = 1;
        for (int j = 1; j < factors[i][0]; j++) {
            if (factors[i][0] % (j+1) == 0) {
                temp = realloc(factors[i], (numFact[i] + 1) * sizeof(long long int));
                if(!temp){
                    fprintf(stderr, "Error allocating memory\n");
                    return 0;
                }
                factors[i] = temp;
                factors[i][numFact[i]] = factors[i][0]/(j+1);
                numFact[i]++;
            }
        }

    }

    for (int i = 0; i < numFact[0]; ++i) {
        for (int j = 0; j < numFact[1]; ++j) {
            if(factors[0][i] == factors[1][j] && factors[0][i] >=gcf)
                gcf = factors[0][i];
        }
    }
    
    free(factors[0]);
    free(factors[1]);
    free(factors);
    return gcf;
}

unsigned int round_e2(double num){
    //returns exponent of next highest power of 2
    num = fabs(num);
    return (unsigned int)ceil(log((double)num)/log(2.0));
}