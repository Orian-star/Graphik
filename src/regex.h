#ifndef REGEX_H
#define REGEX_H

typedef union node {
    double (*function)(double, double);
    double value;
} Node;

typedef enum RegexNodeType {
    REGEX_NODE_VALUE,
    REGEX_NODE_FUNCTION
} RegexNodeType;

typedef struct Regex_Binary_Tree RBTree;
struct Regex_Binary_Tree {
    Node node;
    RegexNodeType type;
    RBTree *left;
    RBTree *right;
};

RBTree* create_tree_from_value(double value);
RBTree* create_tree_from_function(double (*function)(double, double));
void freeRBTree(RBTree *tree);
RBTree* str2regex(char *string);
int export(RBTree *tree, const char *name);


#endif