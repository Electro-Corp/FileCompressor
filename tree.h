/*
    Tree with multiple nodes
*/
#ifndef TREE_H
#define TREE_H

#include <stddef.h>
#include "mem.h"

typedef struct Node{
    char value;
    int childCount;
    struct Node** children;
} Node;

// Creates a sub node of parent with value. If parent is null, head is created
Node* create_sub_node(Node* parent, char value);

// Check if a node has a child with the value
int does_node_have_child(Node* node, char value);

// Checks if a node has a depth of length
char* find_nodes_of_length(Node* node, int length);


// Get head at index
Node* getHead(int i);

#endif