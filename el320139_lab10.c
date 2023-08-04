#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Trie node structure
struct TrieNode {
    int isWord;
    struct TrieNode* children[26];
};

// Trie structure
struct Trie {
    struct TrieNode* root;
};

// Creates a new Trie node
struct TrieNode* createTrieNode() {
    struct TrieNode* newNode = (struct TrieNode*)malloc(sizeof(struct TrieNode));
    newNode->isWord = 0;
    for (int i = 0; i < 26; ++i) {
        newNode->children[i] = NULL;
    }
    return newNode;
}

// Initializes a trie structure
struct Trie* createTrie() {
    struct Trie* newTrie = (struct Trie*)malloc(sizeof(struct Trie));
    newTrie->root = createTrieNode();
    return newTrie;
}

// Inserts the word to the trie structure
void insert(struct TrieNode* pNode, char* word) {
    if (*word == '\0') {
        pNode->isWord++;
        return;
    }

    int index = *word - 'a';
    if (pNode->children[index] == NULL) {
        pNode->children[index] = createTrieNode();
    }
    insert(pNode->children[index], word + 1);
}

// Computes the number of occurrences of the word
int numberOfOccurrences(struct TrieNode* pTrie, char* word) {
    if (pTrie == NULL || word == NULL)
    {
        return 0;
    }

    for (int i = 0; word[i] != '\0'; ++i)
    {
        int index = word[i] - 'a';
        if (pTrie->children[index] == NULL)
        {
            return 0;
        }
        pTrie = pTrie->children[index];
    }
    return pTrie->isWord;
    
}

// Deallocates the trie structure
void deallocateTrieNode(struct TrieNode* pNode) {
    if (pNode == NULL) {
        return;
    }
    for (int i = 0; i < 26; ++i) {
        deallocateTrieNode(pNode->children[i]);
    }
    free(pNode);
}

struct Trie* deallocateTrie(struct Trie* pTrie) {
    if (pTrie == NULL) {
        return NULL;
    }
    deallocateTrieNode(pTrie->root);
    free(pTrie);
    return NULL;
}

// This function will return the number of words in the dictionary,
// and read all the words in the dictionary to the structure words
int readDictionary(char* filename, char** pInWords) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening the file: %s\n", filename);
        return 0;
    }

    int numWords = 0;
    char buffer[256];
    fscanf(file, "%d", &numWords);
    for(int i = 0; i < numWords; i++){
        fscanf(file, "%s", buffer);
        pInWords[i] = malloc(strlen(buffer) + 1);
        strcpy(pInWords[i], buffer);
    }
    return numWords;
}

int main(void) {
    char* inWords[256];

    // Read the number of words in the dictionary
    int numWords = readDictionary("dictionary.txt", inWords);
    for (int i = 0; i < numWords; ++i) {
        printf("%s\n", inWords[i]);
    }

    struct Trie* pTrie = createTrie();
    for (int i = 0; i < numWords; i++) {
        insert(pTrie->root, inWords[i]);
    }

    // Parse line by line, and insert each word into the trie data structure
    char* pWords[] = {"notaword", "ucf", "no", "note", "corg"};
    for (int i = 0; i < 5; i++) {
        printf("\t%s : %d\n", pWords[i], numberOfOccurrences(pTrie->root, pWords[i]));
    }

    pTrie = deallocateTrie(pTrie);
    if (pTrie != NULL) {
        printf("There is an error in this program\n");
    }

    // Deallocate the memory used for inWords
    for (int i = 0; i < numWords; ++i) {
        free(inWords[i]);
    }

    return 0;
}
