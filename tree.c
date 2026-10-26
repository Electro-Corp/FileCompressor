/*
    Impl tree with multiple child nodes per node
*/
#include "tree.h"

Node* heads[256]; // Possible heads
int foundNodes = 0;

// Creates a sub node of parent with value. If parent is null, head is created
Node* create_sub_node(Node* parent, unsigned char value){
    Node* n = (Node*)s_malloc(sizeof(*n));
    n->value = value;
    n->freq = 1;
    n->childCount = 0;
    // Parent or nah?
    if(parent == NULL){
        // Sort by value
        heads[value] = n;
        foundNodes++;
    }else{
        if(parent->childCount == 0){
            // Allocate children for this jit
            parent->children = (Node**)s_malloc(sizeof(*n));
        }else{
            // Reallocate
            parent->children = (Node**)s_realloc(parent->children, sizeof(*n) * parent->childCount);
        }
        parent->children[parent->childCount] = n;
        parent->childCount++;
    }
    return n;
}

// Check if a node has a child with the value
int does_node_have_child(Node* node, unsigned char value){
    if(node == NULL) return -1;
    for(int i = 0; i < node->childCount; i++){
        if(node->children[i]->value == value) return i;
    }
    
    return -1;
}

// Traverse a chain of nodes 
char* traverse_node(Node* node){
    // Start with one ting
    char out[SEARCH_DEPTH];
    out[0] = node->value;
    // Traverse (recurse)
    for(int i = 0; i < node->childCount; i++){

    }

    return out;
}

// Purge all nodes that have under min freq
unsigned long purge_nodes_under(Node* node, int min){
    unsigned long purgedNodes = 0;

    if(node->freq < min){
        s_free(node);
        return 1;
    }

    for(int i = 0; i < node->childCount; i++){
        purgedNodes += purge_nodes_under(node->children[i], min);
    }

    return purgedNodes;
}

Node* getHead(int i){
    return heads[i];
}