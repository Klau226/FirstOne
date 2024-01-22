/* MANOFI KLAOUNTIO   AM: 2022201900119   EMAIL: dit19119@go.uop.gr 
   MAURAKHS GEORGIOS  AM: 2022201900130   EMAIL: dit19130@go.uop.gr */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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
void sortString(char* arr);                         // Sorting the string
void putIntoTable(char *c);                         // Storing strings into table
void searchString(char* c);                         // Searching a string                 
void removeWhiteSpace(char* str);                   // Removing white space      
void printHashTable();                              // Printing the table
void putNumbersFromFile(FILE* ptr,int number);      // Taking strings from a file
char* my_strdup(const char* str);                   // Like strdup (strdup didnt work as intented)
void freeHashTable();                               // Freeing the table


int main(){
    FILE* ptr;
    char* search = malloc(sizeof(char)*64);         // Variable for the search
    int number=0;                                   // How many strings should be stored

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
    }while(number < 1 || number > 183719);      // Making sure the number doesnt exceed our max/min

    putNumbersFromFile(ptr,number);             // Storing Strings with a maximum of number which the user decided
    printf("\n");

    do {
        printf("If you want to leave, type 'exit' \n");
        printf("What do you want to search? \n");
        if (fgets(search, 64, stdin)) {
            removeWhiteSpace(search);           // Making sure string has not a while line
            searchString(search);               // Now that the string is correct search it
        }
    } while (strcmp(search, "exit") != 0);     
 
	printHashTable();   // Printing all the strings and anagrams after exiting the programm
    fclose(ptr);        // Closing the file           
    freeHashTable();    // Freeing
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
    hTable* newNode = NULL;
    hTable* current = NULL;
    hTable* temp = NULL;
    // Allocating memory for the string
    char* sortedString = my_strdup(c);     

    // Sorting the string
    sortString(sortedString);    
    // Hash the sorted string and store it     
    unsigned int sortedIndex = strtohash(sortedString);

    newNode = malloc(sizeof(hTable));       // Allocating memory for our list
    newNode->string = my_strdup(c);         // Storing unsorted string
    newNode->sortedString = sortedString;   // Storing sorted string
    newNode->next = NULL;

    if (hashTable[sortedIndex] == NULL) {
        hashTable[sortedIndex] = newNode;
    } else {
        while(hashTable[sortedIndex] != NULL){
            current = hashTable[sortedIndex];
        
            // Making sure we are in the right list so we can store the anagram        
            if (strcmp(current->sortedString, sortedString) == 0) {
                // Matching sorted strings, insert at the end of the linked list
                temp = current->next;
                while (temp != NULL) {
                    current = temp;
                    temp = temp->next;
                }
                current->next = newNode;
                return;                     // Exiting the function since we inserted the node
            }

            // Variable going to the next cell
            sortedIndex++;

            // We are at the end of the table start from the beginning
            if(sortedIndex >= DICTIONARYSIZE){
                sortedIndex = 0;
            }
        }
    }
}

void searchString(char* c){
    hTable* current = NULL;
    char* sortedString = my_strdup(c);

    sortString(sortedString);
    unsigned int sortedIndex = strtohash(sortedString);
    int i = sortedIndex,count = 0;      // Using i as the loop variable starting from the hash value

    do {
        // We are at the end of the table start from the beginning
        if (i >= DICTIONARYSIZE) {
            i = 0;
        }
        
        // Sorted String Matched!!
        if (hashTable[i] != NULL && strcmp(hashTable[i]->sortedString, sortedString) == 0) {
            current = hashTable[i];
            printf("----------------------------------------------\n");
            printf("The Hash funtion returned: %d \n",sortedIndex);
            printf("The String found in %d\n", i);
            // But no anagrams
            if(current->next == NULL){
                printf("There are no anagrams for %s\n",current->sortedString);
                printf("|%s|->",current->string);
            }
            // We have anagrams
            else{
                printf("There are anagrams and the anagrams of %s are: \n",current->sortedString);
                while(current != NULL){
                    printf("|%s|->",current->string);
                    current = current->next;
                }
            }
            printf("END \n");
            printf("----------------------------------------------\n");
            return;                     // Exit the function if the key is found
        }

        i++;
    } while (i != sortedIndex);         // Continue searching until we reach the starting position
    printf("----------------------------------------------\n");
    printf("Key not found\n");
    printf("----------------------------------------------\n");
}

void removeWhiteSpace(char* str) {
	int length = strlen(str);
    
    // Traverse the string from the end and replace spaces with null characters
	int i;
    	for (i = length - 1; i >= 0 && isspace(str[i]); i--) {
        str[i] = '\0';
    }
}


int partition(char* arr, int low, int high) {
    char pivot = arr[high];
    int i = low - 1;
    int j;
    char temp;
    for (j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }

    temp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = temp;

    return i + 1;
}

char* my_strdup(const char* str) {
    size_t len = strlen(str) + 1;           // Including space for the null terminator
    char* new_str = (char*)malloc(len);
    
    if (new_str != NULL) {
        strcpy(new_str, str);
    }
    
    return new_str;
}

// Function performing quicksort on the string
void quicksort(char* arr, int low, int high) {
    int pi;
    if (low < high) {
        pi = partition(arr, low, high);

        quicksort(arr, low, pi - 1);
        quicksort(arr, pi + 1, high);
    }
}

// Function sorting a string
void sortString(char* arr) {
    int len;
    len = strlen(arr);
    quicksort(arr, 0, len - 1);
}

void printHashTable() {         // Printing only the cells that have a value
    int i;
    hTable* current = NULL;
    for (i = 0; i < DICTIONARYSIZE; i++) {
        if(hashTable[i] != NULL){
            current = hashTable[i];
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
        removeWhiteSpace(ch);       // Removing white space so we can store it and search it with ease
        putIntoTable(ch);

        // Determines how many Strings will be stored
        count ++;   
        if(count > number){
            break;
        }
    }
}

void freeHashTable() {
    hTable* current = NULL;
    hTable* temp = NULL;
    int i;
    for (i = 0; i < DICTIONARYSIZE; i++) {
        current = hashTable[i];
        while (current != NULL) {
            temp = current;
            current = current->next;
            free(temp->string);
            free(temp->sortedString);
            free(temp);
        }
        hashTable[i] = NULL;
    }
}