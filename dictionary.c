
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <stdint.h>

/*
	The basic implementation of a spell checker.
	Requires 1,408 bytes of memory. (slightly over 1K)
	Can check any word in O(1) efficiency.
*/

#define CHARS_IN_ALPHABET 26
#define INVALID_WORD -1

//the longest word in the dictionary is antidisestablishmentarianism. That's it's value.
#define MAX_WORD_VALUE 11281
#define MAX_WORD_LENGTH 28+1
typedef uint32_t word_key_t;

#define BUCKET_SIZE 32
#define BUCKETS ((MAX_WORD_VALUE / BUCKET_SIZE) + 1)
typedef uint32_t bucket_t;

// This function turns a word into a
// unique number.
// a=0, b=1, c=2, y=24, z=25, aa=26, ab=27 etc etc.
word_key_t get_key(char *word);

// The program has an array of 352 unsigned 32 bit ints.
// Each bit represents a word. So the word ab=27, is element
// 0, bit 26. If the bit equals 1, the word is valid, if 0 the word is
// invalid.

bool is_word_valid(char *word);
void store_word(char *word);

int main(int argc, char **argv);
void empty_dictionary();
void print_dictionary();

// definition of the actual array
bucket_t dictionary[BUCKETS];


void empty_dictionary() {
	int i;

	for (i = 0; i < BUCKETS; i++) {
		dictionary[i] = 0;
	}
}

void print_dictionary() {
	int i;

	printf("data required for entire English dictionary: ");
	for (i = 0; i < BUCKETS; i++) {
		printf("%x", dictionary[i]);
	}
	printf("\n");
	
}

bool is_word_valid(char *word) {

	word_key_t key;
	bucket_t bucket;
	bucket_t bit;
	bucket_t mask;
	
	key = get_key(word);

	//remember each array element is a 32 bit int.
	//so we have 32 words described in one array element.
	//use a mask and bitwise operations to extract the bit
	//representing our word in question.
	bucket = dictionary[key / BUCKET_SIZE];
	bit = key % BUCKET_SIZE;
	
	mask = pow(2, bit);

	return (bucket & bit);

}

void store_word(char *word) {
	
	word_key_t key;
	bucket_t bucket;
	bucket_t bit;
	bucket_t mask;

	key = get_key(word);

	bucket = dictionary[key/BUCKET_SIZE];
	bit = key % BUCKET_SIZE;
	mask = pow(2, bit);

	// switch bit to 1
	dictionary[key/BUCKET_SIZE] = (mask | bit);
}

word_key_t get_key(char *word) {

	int pos;
	word_key_t char_key;
	word_key_t key = 0;

	pos = 0;

	while (word[pos] != '\0') {

		// longest word in dictionary is 28 letters.
		// because of this break, this loop is O(29)
		// which is equivalent to O(1), fixed time.
		if (pos == MAX_WORD_LENGTH)
			return INVALID_WORD;

		// get a number between 0 and 25 for the character
		if (word[pos] >= 'A' && word[pos] <= 'Z')
			char_key = word[pos] - 'A';
		else if (word[pos] >= 'a' && word[pos] <= 'z')
			char_key = word[pos] - 'a';
		else {
			printf("dont recognise character: %c\n", word[pos]);
			return INVALID_WORD;
		}

		// multiply it by its position in the string.
		key += char_key + (pos * CHARS_IN_ALPHABET);

		pos++;
	}

	return key;

}

int main(int argc, char **argv) {

	int i;

	empty_dictionary();

	store_word("bus");
	store_word("helicopter");
	store_word("hello");

	word_key_t key;

	//take argument 1, get a unique key for it.
	if (argc > 1) {
		key = get_key(argv[1]);
		printf("key is %llu\n", key);

		if (is_word_valid(argv[1]))
			printf("Word is in the dictionary\n");
		else
			printf("Word is spelt incorrectly or not in dictionary\n");
	}
	else
		printf("specify a word\n");

	//print_dictionary();
	return 0;
}

/*
 * to see the program in action, download and compile it:
 * 
 *     gcc -lm -o dict dictionary.c
 * 
 *  and try out words. the dictionary currently only recognises 3 words.
 *  ./dict helicopter
 *  ./dict hello
 * 
 * a final note. you could read a file of valid words in, invoke
 * store_word on each of them, and then, moving forward you could
 * load and save the dictionary by writing out and reading in the tiny 352
 * sized array. There it is. An O(1) efficiency, tiny spell checker.
/
