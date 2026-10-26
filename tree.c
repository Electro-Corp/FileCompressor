/*
    Impl tree with multiple child nodes per node
*/
#include "tree.h"

Node* heads[256]; // Possible heads
int foundNodes = 0;

// Creates a sub node of parent with value. If parent is null, head is created
Node* create_sub_node(Node* parent, char value){
    Node* n = (Node*)s_malloc(sizeof(*n));
    n->value = value;
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
int does_node_have_child(Node* node, char value){
    if(node == NULL) return -1;
    for(int i = 0; i < node->childCount; i++){
        if(node->children[i]->value == value) return i;
    }
    
    return -1;
}

// Checks if a node has a depth of length
char* find_nodes_of_length(Node* node, int length){
    
}

Node* getHead(int i){
    return heads[i];
}