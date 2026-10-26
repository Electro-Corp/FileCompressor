#include <stdio.h>
#include <stdlib.h>

#include "tree.h"
#include "mem.h"

#define SEARCH_DEPTH 15
#define PATTERN_SIZE 4

#define MIN(a, b) (((a) < (b)) ? (a) : (b))

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
    char original[PATTERN_SIZE];
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
    printf("[Compressor] Preparing to compress file...\n", in);
    // Get length of file
    int length = lengthOfFile(in);
    // Read file
    char* inputBuffer = readFileIntoBuffer(in, length);
    // Create output
    char* output = (char*) s_malloc(length);

    // Track nodes
    int headsCreated = 0, nodesCreated = 0;
    // Create tree
    for(int i = 0; i < length - 1; i++){
        printf("[Compressor] Creating tree... (%d percent complete...)\r", (int)((float)((float)i / (float)length) * 100.0));
        // Look at nodes and see if we have one for the current byte
        Node* root = getHead(inputBuffer[i]);
        if(root == NULL){
            // Head doesn't exist, create it
            root = create_sub_node(NULL, inputBuffer[i]);
            headsCreated++;
        }
        // Start going down the chain
        for(int j = i + 1; j < MIN(i + SEARCH_DEPTH, length); j++){
            int idx = does_node_have_child(root, inputBuffer[j]);
            if(idx < 0){
                root = create_sub_node(root, inputBuffer[j]);
                nodesCreated++;
            }else{
                root = root->children[idx];
            }
        }
    }
    
    printf("\n[Compressor] Tree Created (%d heads, %d nodes)...\n", headsCreated, nodesCreated);
    printf("[Compressor] Starting pattern analysis...\n");
    // Find patterns that are PATTERN_SIZE long (most intensive part)
    for(int i = 0; i < 256; i++){
        printf("[Compressor] Analyzing... (%d percent complete...)\r", (int)((float)((float)i / (float)256) * 100.0));
        // Does a head exist at this pos?
        if(getHead(i) != NULL){
            Node* root = getHead(i);
            // Loop through the sub nodes and find a tree thats longer than PATTERN_SIZE
            
        }
    }

    printf("\n[Compressor] Pattern Analysis completed.\n");

    //printf("Tokens generated: %d\n", indicator);
    //table.maxToken = indicator;
    
    //fwrite(&table, sizeof(Table), 1, out);
    //fwrite(output, sizeof(char), length, out);

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
                //output[k - sizeof(Table)] = table.keys[i].original;
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

