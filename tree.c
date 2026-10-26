/*
    Impl tree with multiple child nodes per node
*/
#include "tree.h"

Node* heads[256]; // Possible heads
int foundNodes = 0;

// Creates a sub node of parent with value. If parent is null, head is created
Node* create_sub_node(Node* parent, char value){
    Node* n = (Node*)s_malloc(sizeof(Node));
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
            parent->children = (Node*)s_malloc(sizeof(Node));
        }else{
            // Reallocate
            parent->children = (Node*)s_realloc(parent->children, sizeof(Node) * parent->childCount);
        }
        parent->children[parent->childCount] = n;
        parent->childCount++;
    }
    return n;
}

