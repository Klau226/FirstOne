#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DICTIONARYSIZE 183719


typedef struct HashTable {
    char* sortedString;
    char* string;
    struct HashTable* next;
} hTable;


hTable* hashTable[DICTIONARYSIZE];


unsigned int strtohash(char* str);
int partition(char* arr, int low, int high);
void quicksort(char* arr, int low, int high);
void sortString(char* arr);
void putIntoTable(char *c);
void searchString(char* c);
void removeNewline(char* str);
void printHashTable();
void putNumbersFromFile(FILE* ptr,int number);


int main(){
    FILE* ptr;
    char* search = malloc(sizeof(char)*64);
    int number=0;

    ptr = fopen("dictionary.txt", "r");             // Opening file in reading mode
    if (NULL == ptr) {
        printf("file can't be opened \n");
    }
    
    printf("Hi and welcome\n");

    do{
        printf("How many strings do you want to insert? \n");
        printf("Type the number:        (The number should be higher than 0 but lower than 183719) \n");
        scanf("%d",&number);
        while (getchar() != '\n');
    }while(number < 1 || number > 183719);
    putNumbersFromFile(ptr,number);
    printf("\n");

    do {
        printf("If you want to leave, press exit\n");
        printf("What do you want to search? \n");
        if (fgets(search, 64, stdin)) {
            removeNewline(search);
            searchString(search);
        }
    } while (strcmp(search, "exit") != 0); // Use strcmp for string comparison
 
    printHashTable();
    free(search);
    fclose(ptr);                                    // Closing the file

    return 0;
}

unsigned int strtohash(char* str){
     unsigned int hash=0;
     while(*str){
            hash=*str+31*hash;
            str++;
     }
     return hash % DICTIONARYSIZE;
}	

void putIntoTable(char *c) {

    char* sortedString = strdup(c);
    sortString(sortedString);
    unsigned int sortedIndex = strtohash(sortedString);

    hTable* newNode = malloc(sizeof(hTable));
    newNode->string = strdup(c);
    newNode->sortedString = sortedString;
    newNode->next = NULL;

    if (hashTable[sortedIndex] == NULL) {
        hashTable[sortedIndex] = newNode;
    } else {
        while(hashTable[sortedIndex] != NULL){
            hTable* current = hashTable[sortedIndex];
        
            // Check the first node separately
            if (strcmp(current->sortedString, sortedString) == 0) {
                // Matching sorted strings, insert at the end of the linked list
                hTable* temp = current->next;
                while (temp != NULL) {
                    current = temp;
                    temp = temp->next;
                }
                current->next = newNode;
                return; // Exit the function since we inserted the node
            }
            sortedIndex++;
            if(sortedIndex >= DICTIONARYSIZE){
                sortedIndex = 0;
            }
        }
    }
}

void searchString(char* c){
    char* sortedString = strdup(c);
    sortString(sortedString);
    unsigned int sortedIndex = strtohash(sortedString);
    int i = sortedIndex,count = 0; // Use i as the loop variable starting from the hash value

    do {
        if (i >= DICTIONARYSIZE) {
            i = 0;
        }

        if (hashTable[i] != NULL && strcmp(hashTable[i]->sortedString, sortedString) == 0) {
            hTable* current = hashTable[i];
            printf("----------------------------------------------\n");
            printf("The Hash funtion returned: %d \n",sortedIndex);
            printf("The String found in %d\n", i);
            if(current->next == NULL){
                printf("There are no anagrams for %s\n",current->sortedString);
                printf("|%s|->",current->string);
            }
            else{
                printf("There are anagrams and the anagrams of %s are: \n",current->sortedString);
                while(current != NULL){
                    printf("|%s|->",current->string);
                    current = current->next;
                }
            }
            printf("END \n");
            printf("----------------------------------------------\n");
            return; // Exit the function if the key is found
        }

        i++;
    } while (i != sortedIndex); // Continue searching until we reach the starting position
    printf("----------------------------------------------\n");
    printf("Key not found\n");
    printf("----------------------------------------------\n");
}

void removeNewline(char* str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0'; // Remove the newline character
    }
}


int partition(char* arr, int low, int high) {
    char pivot = arr[high];
    int i = low - 1;

    for (int j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            // Swap arr[i] and arr[j]
            char temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }

    // Swap arr[i+1] and arr[high] (pivot)
    char temp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = temp;

    return i + 1;
}

// Function to perform quicksort on the string
void quicksort(char* arr, int low, int high) {
    if (low < high) {
        // Find the partitioning index
        int pi = partition(arr, low, high);

        // Recursively sort the elements before and after the partition
        quicksort(arr, low, pi - 1);
        quicksort(arr, pi + 1, high);
    }
}

// Function to sort a string
void sortString(char* arr) {
    int len = strlen(arr);
    quicksort(arr, 0, len - 1);
}

void printHashTable() {
    for (int i = 0; i < DICTIONARYSIZE; i++) {
        if(hashTable[i] != NULL){
            hTable* current = hashTable[i];
            printf("Hash Table[%d]: ", i);
            while (current != NULL) {
                printf("(%s, %s) -> ", current->string, current->sortedString);
                current = current->next;
            }
        printf("NULL\n");
        }
    }
}

void putNumbersFromFile(FILE* ptr,int number){
    char ch[256];
    int count = 0;

    while (fgets(ch,sizeof(ch), ptr))
    {
        removeNewline(ch);
        putIntoTable(ch);

        count ++;
        if(count > number){
            break;
        }
    }
}