#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_BITS 17
#define WORD_SIZE 16

// Node Definition
typedef struct word_node
{
    char *word;
    struct word_node *next;
} word_node;

// * Helper Function to read a line
// Since we have now idea how long the line is,
//   we will use a standard size of 16 passed in as a parameter
//   as most English words are under this size.
// If the line is longer than the parameter size (16),
//   we will increase the size of the buffer.
char *read_line(int size)
{
    char *str;
    int ch;
    int len = 0;
    str = malloc(size); // Alloc memory for string (using size parameter)
    if (str == NULL)
    {
        printf("Memory error\n");
        exit(1);
    }
    // Read characters from stdin
    // If str is full, resize it by doubling its size.
    while ((ch = getchar()) != EOF && ch != '\n')
    {
        str[len++] = ch;
        if (len == size)
        {
            size *= 2;
            str = realloc(str, size);
            if (str == NULL)
            {
                printf(stderr, "realloc error\n");
                exit(1);
            }
        }
    }
    str[len] = '\0'; // terminate string with null character
    return str;
}

// * Helper Function to search the hash table for a word
//   Returns 1 if found, 0 if not found
int in_hash_table(word_node *hash_table[], char *find, unsigned find_len)
{
    unsigned word_code;
    word_node *wordptr;
    word_code = oaat(find, find_len, NUM_BITS); // calculate hash code
    wordptr = hash_table[word_code];            // get pointer to the linked word list
    // Note: The hash table contains pointers to strings not the strings themselves.
    while (wordptr)
    {
        if ((strlen(*(wordptr->word)) == find_len) && (strcmp(*(wordptr->word), find) == 0))
        {
            return 1;
        }
        wordptr = wordptr->next;
    }
    return 0;
}

// * Helper Function to check all possible splits of a word
void find_compound_words(word_node *hash_table[], char *words[], int total_words) {
    int i, j;
    unsigned word_len;
    // For each word generate all possible compound words (splits)
    for (i = 0; i < total_words; i++) {
        word_len = strlen(words[i]);
        // Split the word into two parts
        for (j = 1; j < word_len; j++) {
            if (in_hash_table(hash_table, words[i], j) && 
                in_hash_table(hash_table, words[i][j], word_len - j))
            {
                printf("%s\n", words[i]);
                break; // Only print the word once, not multiple times
            }
        }
    }
}
 
int main (void) {
    static char *words[ 1 << NUM_BITS] = {NULL};
    static word_node *hash_table[ 1 << NUM_BITS] = {NULL};
    int total_words = 0;
    char *word;
    word_node *wordptr;
    unsigned length, word_code;
    word = read_line(WORD_SIZE);
    while (strcmp(word, "END") != 0) {
        length = strlen(word);
        word_code = oaat(word, length, NUM_BITS);
        wordptr->word = &words[total_words];
        wordptr->next = hash_table[word_code];
        hash_table[word_code] = wordptr;
        word = read_line(WORD_SIZE);
        total_words++;
    }
    find_compound_words(hash_table, words, total_words);
    return 0;
}
