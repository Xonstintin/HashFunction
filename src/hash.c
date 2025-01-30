#include "../include/hash_function.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

// weights of lenght of words based on https://norvig.com/mayzner.html
// We want to create proportional indexes to weights (0.01 is 1% is 1 index for that words len)
const double weights[WEIGHTS_SIZE] = {0.01, 0.01, 0.05, 0.07, 0.10, 0.14, 0.15, 0.14, 0.11, 0.08, 0.06, 0.04, 0.02, 0.01, 0.01};
// https://en.wikipedia.org/wiki/Letter_frequency
// letters and it's probability of being found in word (combined not less than 4% = 1 block)
const char f_letters[] = {'T', 'A', 'O', 'I', 'S', 'W', 'C', 'B', 'P', 'H', 'F', 'M', 'D',
                          'R', 'E', 'L', 'N', 'G', 'U', 'K', 'V', 'Y', 'J', 'Q', 'Z', 'X'};
double f_frequencies[] = {16.0, 11.7, 7.6, 7.3, 6.7, 5.5, 5.2, 4.4, 4.3, 4.2, 4.0, 3.8, 3.2, 2.8, 2.8, 2.4, 2.3, 1.6, 1.2, 0.86, 0.82, 0.76, 0.51, 0.22, 0.045, 0.045};
int global_indexes[WEIGHTS_SIZE][26];

void calculate_unique_weights(const double *weights, int weights_size, unsigned int table_size, int *unique_weights)
{
    int total_intervals = table_size / CHUNK_SIZE; // 1% of table_size
    for (int i = 0; i < weights_size; i++)
    {
        unique_weights[i] = round(weights[i] * total_intervals);
    }
}

void calculate_indexes(const int *unique_weights, int weights_size, int *indexes)
{
    indexes[0] = 0;
    for (int i = 1; i <= weights_size; i++)
    {
        indexes[i] = indexes[i - 1] + unique_weights[i - 1] * CHUNK_SIZE;
    }
}

// Based on first and second letter, local because always [0 to 24] inc
int local_index_f(char letter, char next_letter)
{
    if (letter == 'E')
    {
        if (64 < next_letter && next_letter < 71)
        {
            return 0;
        }
        if (next_letter < 80)
        {
            return 1;
        }
        return 2;
    }
    if (letter == 'T')
    {
        if (next_letter < 78)
        {
            return 3;
        }
        return 4;
    }
    if (letter == 'A')
    {
        if (next_letter < 78)
        {
            return 5;
        }
        return 6;
    }
    if (letter == 'O')
    {
        return 7;
    }
    if (letter == 'D')
    {
        return 8;
    }
    if (letter == 'L')
    {
        if (next_letter < 78)
        {
            return 9;
        }
        return 10;
    }
    if (letter == 'I' || letter == 'V')
    {
        if (next_letter < 78)
        {
            return 11;
        }
        return 12;
    }
    if (letter == 'N' || letter == 'M' || letter == 'W')
    {
        if (64 < next_letter && next_letter < 71)
        {
            return 13;
        }
        if (next_letter < 80)
        {
            return 14;
        }
        return 15;
    }
    if (letter == 'S' || letter == 'C' || letter == 'U')
    {
        if (64 < next_letter && next_letter < 71)
        {
            return 16;
        }
        if (next_letter < 80)
        {
            return 17;
        }
        return 18;
    }
    if (letter == 'H' || letter == 'P')
    {
        if (next_letter < 78)
        {
            return 19;
        }
        return 20;
    }
    if (letter == 'G' || letter == 'R')
    {
        return 21;
    }
    if (letter == 'F' || letter == 'Y' || letter == 'K')
    {
        if (next_letter < 78)
        {
            return 22;
        }
        return 23;
    }
    else
    {
        return 24;
    }
    printf("Local index was not founded, stored at 24.\n");
    return 24;
}

void initialize_weights_and_indexes(const double *weights, int weights_size, unsigned int table_size, int *unique_weights, int *indexes)
{
    calculate_unique_weights(weights, weights_size, table_size, unique_weights);
    calculate_indexes(unique_weights, weights_size, indexes);
}

void initialize_buckets(double *buckets, int num_of_buckets)
{
    for (int i = 0; i < num_of_buckets; i++)
    {
        buckets[i] = 0.0;
    }
}

// Function that allows us to store letters according to their probabilities in the flattest way
// (for 3 blocks 33%/33%/33%, for 2 blocks 50%/50%, etc.).
int find_min_bucket_index(double *buckets, int num_of_buckets)
{
    int minIndex = 0;
    for (int i = 1; i < num_of_buckets; i++)
    {
        if (buckets[i] < buckets[minIndex])
        {
            minIndex = i;
        }
    }
    return minIndex;
}

// We calculate all global indices 1 time on load, so now we just have to find value in array.
int global_index_f(char letter, int len_of_word)
{
    if (letter >= 'A' && letter <= 'Z' && len_of_word >= 0 && len_of_word <= 15)
    {
        return global_indexes[len_of_word][letter - 'A'];
    }
    printf("Invalid letter or number of chunks: %c, %d\n", letter, len_of_word);
    return -1;
}

int find_bucket(const int *indexes, const char *word, const int *unique_weights, unsigned int table_size)
{
    if (!word || !unique_weights)
    {
        printf("Error: Null pointer passed to hash function.\n");
        return 0;
    }
    int word_len = strlen(word) - 1; 
    if (word_len > 15)
    {
        word_len = 15; // It is unlikely that we will ever use a word longer than 15 characters.
    }
    int start_index = indexes[word_len];

    char f_local_letter = toupper(word[0]);
    char s_local_letter = toupper(word[1]);
    char l_local_letter = toupper(word[2]);

    int global_chunk_index = global_index_f(f_local_letter, word_len);
    if (global_chunk_index == -1)
    {
        printf("Error in global_index_f\n");
        return -1;
    }

    global_chunk_index = global_chunk_index * CHUNK_SIZE + start_index;
    int local_chunk_index = local_index_f(s_local_letter, l_local_letter);

    if (global_chunk_index + local_chunk_index >= (int)table_size)
    {
        return table_size - 1;
    }

    return global_chunk_index + local_chunk_index;
}

// Hashes word to a number
unsigned int hash(const int *indexes, const char *word, const int *unique_weights, unsigned int table_size)
{
    if (!word || !unique_weights)
    {
        printf("Error: Null pointer passed to hash function.\n");
        return 0;
    }
    int bucket = find_bucket(indexes, word, unique_weights, table_size);
    if (bucket == -1 || (unsigned int)bucket >= table_size)
    {
        printf("Error: Could not hash word '%s'\n", word);
        return 0;
    }

    return bucket;
}

void initialize_global_indexes(int weights_size, const int *unique_weights)
{
    int num_of_iterations = weights_size;

    for (int iter = 0; iter < num_of_iterations; iter++)
    {
        int num_of_chunks = unique_weights[iter];

        double buckets[num_of_chunks];
        initialize_buckets(buckets, num_of_chunks);

        for (int i = 0; i < 26; i++)
        {
            char letter = f_letters[i];
            int minIndex = find_min_bucket_index(buckets, num_of_chunks);

            global_indexes[iter][letter - 'A'] = minIndex;
            buckets[minIndex] += f_frequencies[i];
        }
    }
}
