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

    char input[40];
    char* ptr[20];
    short len;

    for (int i = 0; i < 20; ++i) {
        ptr[i] = malloc(40 * sizeof(char));
    }

    printf("Enter a fraction, decimal number, or combination thereof\n");
    len = (short)scanf("%s", input);

    for (int i = 0; i < 40; ++i) {
        if(input[i] == '\n' || input[i] == '\0') {
            input[i] = '\0';
            break;
        }
        if(input[i] != '/' && input[i] != '.' && input[i] != '\0' && input[i] != '-' && (input[i] < '0' || input[i] > '9')){
            printf("That was not a valid input, please try again\n");
            return input_rational(num);
        }
    }

    unsigned int i = 0;
    unsigned int dec_pts = 0;
    unsigned int gfact;
    double numer, denom;

    ptr[0] = strtok(input, "/");
    ptr[1] = strtok(NULL, "/");

    if(!ptr[0]){ //if no number included before the '/' sign
        printf("That was not a valid input, please try again\n");
        return input_rational(num);
    }

    //evaluate numerator
    numer = atof(ptr[0]);
    strtok(ptr[0], ".");
    ptr[2] = strtok(NULL, ".");
    if(ptr[2])
        dec_pts = strlen(ptr[2]);
    else dec_pts = strlen(ptr[0]);

    //evaluate denominator
    if(ptr[1]) { //if a '/' detected
        denom = atof(ptr[1]);
        strtok(ptr[1], ".");
        ptr[3] = strtok(NULL, ".");
        if (ptr[3])
            i = strlen(ptr[3]);
        else i = 1;

        dec_pts = min(7, max(dec_pts, i));
    }
    else{
        denom = 1;
    }

    num->numerator = (int)round(numer *  pow(10, dec_pts));
    num->denominator = (int)round(denom *  pow(10, dec_pts));

    gfact = gcf(num->numerator, num->denominator);


    num->numerator /= (int)gfact;
    num->denominator /= (int)gfact;
    num->dec_est = (double)num->numerator/num->denominator;

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

    unsigned int gfact = gcf(rational->numerator, rational->denominator);
    if(gfact == 0)
        return NULL;
    rational->numerator /= (int)gfact;
    rational->denominator /= (int)gfact;
    rational->dec_est = (double)rational->numerator/rational->denominator;
    return rational;
}

Rational* set_rat_f(Rational* rational, double val){
    //set a rational's fractional part from a given float (7 digit precision)
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
    //adds two rational objects and stores result in second object
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

void mat_free(Matrix* mat){
    //deallocate all memory allocated to a matrix
    for (int i = 0; i < mat->columns; ++i) {
        free(mat->self[i]);
    }
    free(mat->self);
    free(mat);
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
    Matrix* mat2 = malloc(sizeof(Matrix)); //allocate on heap
    mat_alloc(mat->rows, mat->columns);

    mat2->columns = mat->columns;
    mat2->rows = mat->rows;
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
    if(mat1->columns != mat2->rows)
        return NULL;

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

    printf("Enter the number of rows and columns in the format 'RRxCC'.\n");
    scanf("%s", input);

    for (int i = 0; i < 10; ++i) {
        if(!(input[i] == 'x' || input[i] == 'X' || input[i] == '\n' || input[i] == '\0' || (input[i] >= '0' && input[i] <= '9'))){
            printf("That was not a valid input, please try again.\n");
            return input_mat();
        }
        if(input[i] == '\n' || input[i] == '\0'){
            input[i] = '\0';
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
            printf("Row %d, column %d\n", i, j);
            input_rational(&(mat->self[i][j]));
        }
    }

    print_mat(mat);
    return mat;
}

void print_mat(Matrix* mat){
    for (int i = 0; i < mat->rows; ++i) {
        printf("[ ");
        for (int j = 0; j < mat->columns; ++j) {
            printf("  %d/%d  ", mat->self[i][j].numerator, mat->self[i][j].denominator);
        }
        printf(" ]\n\n");
    }
    printf("\n");
}

unsigned int gcf(int num1, int num2){
    int numFact[2], gcf = 1;
    int** factors = calloc(2, sizeof(int*));
    int* temp;

    factors[0] = calloc((round_e2(num1) + 1), sizeof(int));
    factors[1] = calloc((round_e2(num2) + 1), sizeof(int));

    if(factors[0] == NULL || factors[1] == NULL){
        fprintf(stderr, "Error allocating memory\n");
        return 0;
    }

    factors[0][0] = num1;
    factors[1][0] = num2;

    for(int i = 0; i<2; i++){
        numFact[i] = 1;
        for (int j = 1; j < factors[i][0]; j++) {
            if (factors[i][0] % (j+1) == 0) {
                temp = realloc(factors[i], (numFact[i] + 1) * sizeof(int));
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

    free(factors);

    return gcf;
}

unsigned int round_e2(double num){
    //returns exponent of next highest power of 2
    num = fabs(num);
    return (unsigned int)ceil(log((double)num)/log(2.0));
}