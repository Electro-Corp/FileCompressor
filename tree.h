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

#endif