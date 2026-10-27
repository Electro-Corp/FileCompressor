#include <stdio.h>
#include <stdlib.h>

#include "config.h"
#include "tree.h"
#include "mem.h"


#define MIN(a, b) (((a) < (b)) ? (a) : (b))

// Print usage
void usage();
// Get length of file
unsigned long lengthOfFile(FILE* fp);
// Read a file into a buffer (allocates memory but does NOT free it for you)
unsigned char* readFileIntoBuffer(FILE* fp, int length);
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
        exit(-1);
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
unsigned long lengthOfFile(FILE* fp){
    fseek(fp, 0L, SEEK_END);
    unsigned long length = ftell(fp);
    rewind(fp);
    return length;
}

// Read a file into a buffer (allocates memory but does NOT free it for you)
unsigned char* readFileIntoBuffer(FILE* fp, int length){
    unsigned char* buffer = (unsigned char*) s_malloc(sizeof(char) * length); 
    fread(buffer, sizeof(unsigned char), length, fp);
    return buffer;
}

// Compress a file
void compress(FILE* in, FILE* out){
    // Get length of file
    unsigned long length = lengthOfFile(in);
    printf("[Compressor] Preparing to compress file... (%lu bytes, with a search depth of %d)\n", length, SEARCH_DEPTH);
    // Read file
    unsigned char* inputBuffer = readFileIntoBuffer(in, length);
    // Create output
    unsigned char* output = (unsigned char*) s_malloc(length);

    // Track nodes
    int headsCreated = 0, nodesCreated = 0;
    // Create tree
    for(unsigned long i = 0; i < length - 1; i++){
        printf("[Compressor] Creating tree... (%d percent complete...) [%lu KB processed of %lu KB]\r", (int)((float)((float)i / (float)length) * 100.0), i / 1000, (length - 1) / 1000);
        // Look at nodes and see if we have one for the current byte
        Node* root = getHead(inputBuffer[i]);
        if(root == NULL){
            // Head doesn't exist, create it
            root = create_sub_node(NULL, inputBuffer[i]);
            headsCreated++;
        }
        // Start going down the chain
        for(unsigned long j = i + 1; j < MIN(i + SEARCH_DEPTH, length); j++){
            int idx = does_node_have_child(root, inputBuffer[j]);
            if(idx < 0){
                root = create_sub_node(root, inputBuffer[j]);
                nodesCreated++;
            }else{
                root->freq++;
                root = root->children[idx];
            }
        }
    }
    printf("[Compressor] Creating tree... (100 percent complete...)\n");
    printf("[Compressor] Tree Created (%d heads, %d nodes)...\n", headsCreated, nodesCreated);

    unsigned long purged = 0, lastMemory = getTotalMemoryUsed();;

    // Purge nodes under MIN_FREQ
    for(int i = 0; i < 256; i++){
        printf("[Compressor] Purging nodes with frequency under %d... (%d percent complete...)\r", MIN_FREQ, (int)((float)((float)i / (float)256) * 100.0));
        // Does a head exist at this pos?
        Node* node = getHead(i);
        if(node != NULL){
            Node* node = getHead(i);
            purged += purge_nodes_under(node, MIN_FREQ);
        }
    }
    printf("[Compressor] Purging nodes with frequency under %d... (100 percent complete...)\n", MIN_FREQ);
    printf("[Compressor] Purging complete (%lu nodes purged, %lu nodes left to analyze)\n", purged, nodesCreated - purged);
    printf("[Compressor] Memory usaged dropped from %lu KB to %lu KB\n", lastMemory / 1000, getTotalMemoryUsed() / 1000);

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
    printf("[Compressor] Analyzing... (100 percent complete...)\n");
    printf("[Compressor] Pattern Analysis completed.\n");

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

