#include <stdio.h>
#include <stdlib.h>

#include "tree.h"
#include "mem.h"

// Print usage
void usage();
// Get length of file
int lengthOfFile(FILE* fp);
// Read a file into a buffer (allocates memory but does NOT free it for you)
char* readFileIntoBuffer(FILE* fp, int length);
// qsort compare function for frequency table
int compare(const void* a, const void* b);
// I dare you to guess what this does
void compress(FILE* in, FILE* out);
// Take a wild guess
void uncompress(FILE* in, FILE* out);

typedef struct {
    char original;
    unsigned char out;
} Key;

typedef struct {
    Key keys[256];
    int maxToken;
} Table;

int main(int argv, char** args){
    // Check if enough args
    if(argv < 4){ 
        usage();
        return 1;
    }

    // Open Input and validate 
    FILE* input = fopen(args[2], "rb");

    if(!input){
        printf("Fatal: Cannot open %s! Quitting.\n", args[2]);
    }

    // Open Output 
    FILE* output = fopen(args[3], "wb");

    switch(args[1][0]){
        // Compress
        case 'c':
            compress(input, output);
            break;
        // Uncompress
        case 'u':
            uncompress(input, output);
            break;
        default:
            printf("Fatal: Command %c is unrecognized!\n", args[1][0]);
            usage();
            return 1;
    }

    fclose(input);
    fclose(output);

    memQuit();

    return 0;
}

// Get length of file
int lengthOfFile(FILE* fp){
    fseek(fp, 0L, SEEK_END);
    int length = ftell(fp);
    rewind(fp);
    printf("length of file: %d\n", length);
    return length;
}

// Read a file into a buffer (allocates memory but does NOT free it for you)
char* readFileIntoBuffer(FILE* fp, int length){
    char* buffer = (char*) s_malloc(sizeof(char) * length); 
    fread(buffer, sizeof(char), length, fp);
    return buffer;
}

// qsort compare function for frequency table
int compare(const void* a, const void* b){
    if(*(int*)a < *(int*)b) return 1;
    else if(*(int*)a > *(int*)b) return -1;
    return 0;
}

// Compress a file
void compress(FILE* in, FILE* out){
    // Get length of file
    int length = lengthOfFile(in);
    // Read file
    char* inputBuffer = readFileIntoBuffer(in, length);
    // Create output
    char* output = (char*) s_malloc(length);
    // Intermedite format
    

    // Create our Table
    Table table;
    // Keep track of our current indicator
    char indicator = 0;

    // Process
    for(int i = 0; i < length; i++){
        // Do we have
        char c = inputBuffer[i];
        if(table.keys[c].original == NULL){
            table.keys[c].original = c;
            table.keys[c].out = indicator++;
        } 
        // Output
        output[i] = table.keys[c].out;
    }

    printf("Tokens generated: %d\n", indicator);
    table.maxToken = indicator;
    
    fwrite(&table, sizeof(Table), 1, out);
    fwrite(output, sizeof(char), length, out);

    // We done
    s_free(inputBuffer);
    s_free(output);
}


// Uncompress a file
void uncompress(FILE* in, FILE* out){
    // Get length of file
    int length = lengthOfFile(in);
    // Read table into memory
    Table table;
    fread(&table, sizeof(table), 1, in);
    rewind(in);
    // Read rest of file
    char* inputBuffer = readFileIntoBuffer(in, length);
    // Create output
    char* output = (char*) s_malloc((length) - sizeof(Table));

    printf("Tokens: %d\n", table.maxToken);

    for(int k = sizeof(Table); k < length; k++){
        // Find key
        char token = inputBuffer[k];
        for(int i = 0; i < 256; i++){
            if(table.keys[i].out == token){
                output[k - sizeof(Table)] = table.keys[i].original;
                break;
            }
        }
    }


    fwrite(output, sizeof(char), length - sizeof(Table), out);

    // We done
    s_free(inputBuffer);
    s_free(output);
}



void usage(){
    printf("= Compression Utilty =\n");
    printf("Usage: \n");
    printf("./compress (command) [inputfile] [outputfile]\n");
    printf("(command) options: ");
    printf("c - compress\n");
    printf("u - uncompress\n");
}

