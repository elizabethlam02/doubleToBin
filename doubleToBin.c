#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>

#define EXP_SZ 11
#define FRAC_SZ 52

// A program that generates a double to binary
// and does not use unsigned long int ref_bits = *(unsigned long int*) &value;

int main(int argc, char *argv[]) {

    FILE* fp = fopen(argv[1], "r");
    if (!fp) {
        perror("fopen failed");
        return 0;
    }

    // first, read the number
    double value;
    fscanf(fp, "%lf", &value);

    // the reference solution ('the easy way')
    // you are not allowed to print from this casted 'ref_bits' variable below
    // but, it is helpful for validating your solution
    // unsigned long int ref_bits = *(unsigned long int*) &value;

    // THE SIGN BIT
    bool sign = value<0.0;
    if (value == 0.0){
        double x = 1/value;
        sign = x<0.0;
    }

    printf("%d_",sign);

    double fraction = sign ? -value : value;
    signed short trial_exp=(1<<(EXP_SZ-1))-1;

    int k =0;
    while ( (fraction < 1 || fraction >= 2.0 ) && fraction != 0){
      
        if (fraction <  1 ){
            fraction =  fraction * 2;
            k--;
        } else if (fraction >= 2.0){
            fraction /= 2;
            k++;
        }
    }

    trial_exp = k;
    bool denormalized = false;
    unsigned short bias = (1<<(EXP_SZ-1))-1;
    signed short exp = trial_exp + bias;

    float fraction2 = value;
    if (fraction2 < 0){
        fraction2 = -fraction2;
    }

    if (fraction2 <= 0){ // not supposed to be 0.
        exp = 0;
        denormalized = true;
    }
    //printf("%d\n", exp);
   // printf("%f\n", fraction2);
   // printf(denormalized ? "true" : "false");

    for ( int exp_index=EXP_SZ-1; 0<=exp_index; exp_index-- ) {
        bool exp_bit = 1&exp>>exp_index;
        printf("%d",exp_bit);
       // assert (exp_bit == (1&ref_bits>>(exp_index+FRAC_SZ))); // validate your result against the reference
    }
    printf("_");


    if (fraction > 1) {
        fraction = fraction - 1;
      
    }
    if (denormalized){
        if(sign){
            fraction = -value;
        } else {
            fraction = value;
        }
        int e = 1 - bias;
        fraction = fraction / pow(2, e);
    }

   // printf("fraction: %f\n", fraction);
    int x =0;
    bool frac_array[FRAC_SZ+1]; // one extra LSB bit for rounding
    for (int frac_index=FRAC_SZ; 0<=frac_index; frac_index-- ) {
            fraction=fraction* 2;
            x = fraction;
            //printf("fraction: %f\n", fraction);
            //printf("x: %d\n", x);
            if(x==1){
                fraction=fraction-x;
               frac_array[frac_index] = true;
               // printf(" %d 1\n", frac_index);
            } else {
                frac_array[frac_index] = false;
                //printf(" %d 0\n", frac_index);
            }
    }
     // rounding
    /* ... */
   // printf("/n test: %d\n", frac_array[0]);
 
    
    bool carry = false;
    if (frac_array[0]){
        for (int i = 1; i < FRAC_SZ; i++) { // iterate from LSB to MSB

            if (!frac_array[i]){
                frac_array[i] = true;
                carry = false;
                break;
            } else if (frac_array[i] && !carry){
                frac_array[i] = 0;
                carry=true;
            } else if (frac_array[i] && carry){
                frac_array[i] = 0;
                carry=true;
            }

        }
    }
    
    for ( int frac_index=FRAC_SZ-1; 0<=frac_index; frac_index-- ) {
       bool frac_bit = frac_array[frac_index+1];
       //bool frac_bit = frac_array[frac_index];
        printf("%d", frac_bit);
        // assert (frac_bit == (1&ref_bits>>frac_index)); // validate your result against the reference
    }

    return(EXIT_SUCCESS);

}