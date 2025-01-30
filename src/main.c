#include <stdio.h>
#include "../include/hash_function.h"

//Hashes word to a number.
//Warning, only works with English alphabet! (not symbols, numbers etc.)

int main(void) 
{

    int unique_weights[WEIGHTS_SIZE];
    int indexes[WEIGHTS_SIZE + 1];

    initialize_weights_and_indexes(weights, WEIGHTS_SIZE, N, unique_weights, indexes);
    initialize_global_indexes(WEIGHTS_SIZE, unique_weights);

    // Example word to hash
    const char *test_word = "example";
    unsigned int hash_value = hash(indexes, test_word, unique_weights, N);

    printf("Hash value for '%s': %u\n", test_word, hash_value);

    return 0;
}
