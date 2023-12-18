#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DICTIONARYSIZE 183719


typedef struct HashTable {
    //char* key;
    char* string;
    //struct HashTable* next;
} hTable;


hTable* hashTable[DICTIONARYSIZE];

unsigned int strtohash(char* str){
     unsigned int hash=0;
     while(*str){
            hash=*str+31*hash;
            str++;
     }
     return hash % DICTIONARYSIZE;
}	

void putIntoTable(char *c){
    unsigned int m = strtohash(c);

    if(hashTable[m] == NULL){
        hashTable[m] = malloc(sizeof(hTable));
        hashTable[m]->string = strdup(c);
    }
    else{
        while(hashTable[m] != NULL){
            m++;
            if(m >= DICTIONARYSIZE){
                m = 0; 
            }
        }
        hashTable[m] = malloc(sizeof(hTable));
        hashTable[m]->string = strdup(c);
    }
}


void searchKey(char* c){
    unsigned int m = strtohash(c);
    int i = m; // Use i as the loop variable starting from the hash value

    do {
        if (i >= DICTIONARYSIZE) {
            i = 0;
        }

        if (hashTable[i] != NULL && strcmp(hashTable[i]->string, c) == 0) {
            printf("The String found in %d\n", i);
            printf("The String is %s\n", hashTable[i]->string);
            printf("The Hash funtion returned: %d \n",m);
            return; // Exit the function if the key is found
        }

        i++;
    } while (i != m); // Continue searching until we reach the starting position

    printf("Key not found\n");
}

void removeNewline(char* str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0'; // Remove the newline character
    }
}

void putNumbersFromFile(FILE* ptr){
    char ch[256];

    while (fgets(ch,sizeof(ch), ptr))
    {
        removeNewline(ch);
        putIntoTable(ch);
    }

}

void printTable(){
    int i;
    for(i=0;i<DICTIONARYSIZE;i++){
        if(hashTable[i] != NULL)
        printf("The char board is: %s and the index is: %d \n",hashTable[i]->string,i);
    }
}

int main(){
    int i = 0;
    unsigned int m;

    FILE* ptr;
    ptr = fopen("dictionary.txt", "r");             // Opening file in reading mode
 
    if (NULL == ptr) {
        printf("file can't be opened \n");
    }
 
    putNumbersFromFile(ptr);

    searchKey("scarry");

    fclose(ptr);                                    // Closing the file

}