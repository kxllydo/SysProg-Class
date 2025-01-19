#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER_SZ 50

//prototypes
void usage(char *);
void print_buff(char *, int);
int  setup_buff(char *, char *, int);

//prototypes for functions to handle required functionality
int  count_words(char *, int, int);
//add additional prototypes heres

int setup_buff(char *buff, char *user_str, int len) {
    char *current = buff;
    char *currentChar = user_str;
    int consecSpace = 0;
    int count = 0;

    if (find_length(user_str) > len){
        return -1;
    }

    while (*currentChar != '\0' && isspace(*currentChar)) {
        currentChar++;
    }

    while (*currentChar != '\0' && (current - buff) < len) {  
        if (isspace(*currentChar)) {  
            if (!consecSpace) {  
                *current = ' ';  
                current++;       
                consecSpace = 1;
                count++;
            }
        } else {
            *current = *currentChar;
            current++;
            consecSpace = 0;
            count++;
        }
        currentChar++; 
    }

    while (current > buff && isspace(*(current - 1))) {
        current--;
    }

    while ((current - buff) < len) {
        *current = '.';
        current++;
    }

    return count;
}

int find_length(char *buff) {
    char *current = buff;
    int length = 0;
    int inWord = 0; 
    while (*current == ' ') {
        current++;
    }
    
    while (*current != '\0') {
        if (*current != ' ') {
            length++;
            inWord = 1; // We're inside a word
        } else if (inWord && *(current + 1) != ' ' && *(current + 1) != '\0') {
            length++;
            inWord = 0;
        }
        current++;
    }

    return length;
}


void print_buff(char *buff, int len){
    printf("Buffer:  [");
    for (int i=0; i<len; i++){
        putchar(*(buff+i));
    }
    printf("]");
    putchar('\n');
}

void usage(char *exename){
    printf("usage: %s [-h|c|r|w|x] \"string\" [other args]\n", exename);

}

int count_words(char *buff, int len, int str_len){
    char *current = buff;
    int inWord = 0;
    int wordCount = 0;

    for (int i = 0; i < str_len; i++) {
        if (isspace(*current)) {
            if (inWord != 0){
                wordCount++;
            }
            inWord = 0;
        } else {
            inWord = 1;
        }
        current++;
    }
    return wordCount + 1;
}

void reverse_string(char *buff, int str_len) {
    char *start = buff; 
    char *end = buff + str_len - 1;  

    while (start < end) {
        char temp = *start;
        *start = *end;
        *end = temp;

        start++;
        end--;
    }
}

int word_print(char *buff, int str_len) {
    char *current = buff;
    char *word = (char *)malloc(BUFFER_SZ * sizeof(char));
    int inWord = 0;
    int wordCount = 0;
    int length = 0;
    char *wordPtr = word;

    if (!word) {
        printf("Memory allocation failed\n");
        return -1;
    }

    printf("Word Print\n----------\n");

    for (int i = 0; i < str_len; i++) {
        if (isspace(*current) || *current == '\0') {
            if (inWord) {
                *wordPtr = '\0';
                wordCount++;
                printf("%d. %s(%d)\n", wordCount, word, length);
                wordPtr = word;
                length = 0;
                inWord = 0;
            }
        } else {
            *wordPtr = *current;
            wordPtr++;
            length++;
            inWord = 1;
        }
        current++;
    }

    if (inWord) {
        *wordPtr = '\0';
        wordCount++;
        printf("%d. %s(%d)\n", wordCount, word, length);
    }
    printf("\nNumber of words returned: %d\n", wordCount);
    free(word);
    return 0;
}
int replace_word(char *buff, int len, int str_len, char *oldWord, char *newWord) {
    int oldLength = strlen(oldWord);
    int newLength = strlen(newWord);

    if (oldLength == 0 || oldLength > str_len) {
        return -1;
    }

    int newMaxSize = len;

    char *newBuff = (char *)malloc(newMaxSize);
    if (!newBuff) {
        return -1;
    }

    int replaced = 0;

    char *buffEnd = buff + str_len;
    char *newBuffEnd = buff + len;

    while (buff < buffEnd && newBuff < newBuffEnd) {
    if (strncmp(buff, oldWord, oldLength) == 0) {
        if ((newBuff + newLength) < newBuffEnd) {
            memcpy(newBuff, newWord, newLength);
            newBuff += newLength;
            replaced = 1;
        }
        buff += oldLength;
    } else {
        *newBuff = *buff;
        newBuff++;
        buff++;
    }
}

    while (newBuff < newBuffEnd) {
        *newBuff++ = '.';
    }

    *newBuff = '\0';
    memcpy(buff, newBuff, len);
    free(newBuff);
    return replaced;
}


int main(int argc, char *argv[]){

    char *buff;             //placehoder for the internal buffer
    char *input_string;     //holds the string provided by the user on cmd line
    char opt;               //used to capture user option from cmd line
    int  rc;                //used for return codes
    int  user_str_len;      //length of user supplied string

    //TODO:  #1. WHY IS THIS SAFE, aka what if arv[1] does not exist?
    //      PLACE A COMMENT BLOCK HERE EXPLAINING
    if ((argc < 2) || (*argv[1] != '-')){
        usage(argv[0]);
        exit(1);
    }

    opt = (char)*(argv[1]+1);   //get the option flag

    if (opt == 'h'){
        usage(argv[0]);
        exit(0);
    }

    //WE NOW WILL HANDLE THE REQUIRED OPERATIONS

    //TODO:  #2 Document the purpose of the if statement below
    //      PLACE A COMMENT BLOCK HERE EXPLAINING
    if (argc < 3){
        usage(argv[0]);
        exit(1);
    }

    input_string = argv[2]; //capture the user input string

    //TODO:  #3 Allocate space for the buffer using malloc and
    //          handle error if malloc fails by exiting with a 
    //          return code of 99
    buff = (char *)malloc(BUFFER_SZ * sizeof(char));  // Allocate memory for the buffer
    if (buff == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 99;
    }

    user_str_len = setup_buff(buff, input_string, BUFFER_SZ);     //see todos

    if (user_str_len < 0){
        printf("Error setting up buffer, error = %d", user_str_len);
        exit(2);
    }

    switch (opt){
        case 'c':
            rc = count_words(buff, BUFFER_SZ, user_str_len);
            if (rc < 0){
                printf("Error counting words, rc = %d", rc);
                exit(2);
            }
            printf("Word Count: %d\n", rc);
            break;

        case 'w':
            rc = word_print(buff, user_str_len);
            if (rc < 0){
                exit(2);
            }
            break;
        
        case 'r':
            reverse_string(buff, user_str_len);
            break;

        case 'x':
            if (argc != 5) {
                exit(1);
            }

            char *oldWord = argv[3];
            char *newWord = argv[4];
            rc = replace_word(buff, BUFFER_SZ, user_str_len, oldWord, newWord);
            if (rc <= 0){
                printf("Not Implemented!");
                exit(2);
            }
            break;

        default:
            usage(argv[0]);
            exit(1);
    }

    print_buff(buff,BUFFER_SZ);
    free(buff);

    exit(0);
}

//TODO:  #7  Notice all of the helper functions provided in the 
//          starter take both the buffer as well as the length.  Why
//          do you think providing both the pointer and the length
//          is a good practice, after all we know from main() that 
//          the buff variable will have exactly 50 bytes?
//  
//          PLACE YOUR ANSWER HERE