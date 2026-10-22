#include "regex.h"

#include <errno.h>
#include <math.h>
#include <stdlib.h>


RBTree* create_tree_from_value(double value)
{
    RBTree *res = calloc(1, sizeof(RBTree));
    if (!res) return NULL;

    res->node.value = value;
    res->type = REGEX_NODE_VALUE;
    return res;
}
RBTree* create_tree_from_function(double (*function)(double, double))
{
    if (!function) return NULL;

    RBTree *res = calloc(1, sizeof(RBTree));
    if (!res) return NULL;

    res->node.function = function;
    res->type = REGEX_NODE_FUNCTION;
    return res;
}

void freeRBTree(RBTree *tree)
{
    if (!tree) return;

    freeRBTree(tree->left);
    freeRBTree(tree->right);
    free(tree);
}

static char* remove_spaces_and_tabs(const char *string)
{
    if (!string) return NULL;

    size_t length = 0;
    for (const char *current = string; *current; current++)
        if (*current != ' ' && *current != '\t')
            length++;

    char *result = malloc(length + 1);
    if (!result) return NULL;

    char *destination = result;
    for (const char *current = string; *current; current++)
        if (*current != ' ' && *current != '\t')
            *destination++ = *current;
    *destination = '\0';

    return result;
}

RBTree* str2regex(char *string)
{
    if (!string) return NULL;

    char *cleaned_string = remove_spaces_and_tabs(string);
    if (!cleaned_string) return NULL;

    char *end;
    errno = 0;
    double value = strtod(cleaned_string, &end);
    if (end == cleaned_string || errno == ERANGE || !isfinite(value)) {
        free(cleaned_string);
        return NULL;
    }
    if (*end != '\0') {
        free(cleaned_string);
        return NULL;
    }

    free(cleaned_string);
    return create_tree_from_value(value);
}