#ifndef HASH_FUNCTION_H
#define HASH_FUNCTION_H

#include <stdbool.h>

#define N 2500       // Number of buckets in the hash table
// min is 2,5k. We don't want to divide intervals by non-integer values, so 1% of N must be
// divisible by 25 pointwise
// so 1% can be 25, 50, 75... from 2.5k, 5k, 7.5k...
#define WEIGHTS_SIZE 15 // Number of weight categories
#define CHUNK_SIZE 25 // There are 26 letters in the English alphabet,
// but 25 is much more convenient for us as a number because 25*4 = 100, 4%= 1.

extern const char f_letters[];      
extern const double weights[];     
extern double f_frequencies[];      
extern int global_indexes[WEIGHTS_SIZE][26]; 


void initialize_global_indexes(int weights_size, const int *unique_weights);
void initialize_weights_and_indexes(const double *weights, int weights_size, unsigned int table_size, int *unique_weights, int *indexes);

unsigned int hash(const int *indexes, const char *word, const int *unique_weights, unsigned int table_size);

int local_index_f(char letter, char next_letter); 
int find_bucket(const int *indexes, const char *word, const int *unique_weights, unsigned int table_size); 
void calculate_unique_weights(const double *weights, int weights_size, unsigned int table_size, int *unique_weights); 
void calculate_indexes(const int *unique_weights, int weights_size, int *indexes); 

#endif // HASH_FUNCTION_H
