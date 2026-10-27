/*
    Tree with multiple nodes
*/
#ifndef TREE_H
#define TREE_H

#include <stddef.h>
#include <stdio.h>
#include <limits.h>
#include "mem.h"
#include "config.h"

typedef struct Node{
    unsigned char value;
    unsigned long freq;
    unsigned long childCount;
    struct Node** children;
} Node;

// Creates a sub node of parent with value. If parent is null, head is created
Node* create_sub_node(Node* parent, unsigned char value);

// Check if a node has a child with the value
int does_node_have_child(Node* node, unsigned char value);

// Traverse a chain of nodes 
char* get_patterns(Node* node);

// Print out all chains from node
void print_all_chains(Node* node);

// Purge all nodes that have under min freq
unsigned long purge_nodes_under(Node* node, unsigned long min);

// Get head at index
Node* getHead(int i);

#endif