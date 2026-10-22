#define _POSIX_C_SOURCE 200809L

#include "regex.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

static int write_dot_node(FILE *file, const RBTree *tree, size_t *next_id)
{
    if (!tree) return 0;

    size_t id = (*next_id)++;
    if (tree->type == REGEX_NODE_VALUE) {
        if (fprintf(file, "  n%zu [label=\"%.17g\"];\n", id, tree->node.value) < 0)
            return -1;
    } else if (tree->type == REGEX_NODE_FUNCTION) {
        if (fprintf(file, "  n%zu [label=\"function\"];\n", id) < 0)
            return -1;
    } else {
        return -1;
    }

    if (tree->left) {
        size_t child_id = *next_id;
        if (fprintf(file, "  n%zu -> n%zu;\n", id, child_id) < 0 ||
            write_dot_node(file, tree->left, next_id) != 0)
            return -1;
    }
    if (tree->right) {
        size_t child_id = *next_id;
        if (fprintf(file, "  n%zu -> n%zu;\n", id, child_id) < 0 ||
            write_dot_node(file, tree->right, next_id) != 0)
            return -1;
    }

    return 0;
}

static char* pdf_output_path(const char *name)
{
    size_t length = strlen(name);
    int has_pdf_extension = length >= 4 && strcmp(name + length - 4, ".pdf") == 0;
    if (length > (size_t)-1 - (has_pdf_extension ? 1 : 5)) return NULL;

    char *path = malloc(length + (has_pdf_extension ? 1 : 5));
    if (!path) return NULL;

    memcpy(path, name, length);
    if (has_pdf_extension) {
        path[length] = '\0';
    } else {
        memcpy(path + length, ".pdf", 5);
    }
    return path;
}

int export(RBTree *tree, const char *name)
{
    if (!tree || !name || *name == '\0') {
        fprintf(stderr, "export: arbre regex ou nom de fichier invalide\n");
        return -1;
    }

    char *output_path = pdf_output_path(name);
    if (!output_path) {
        fprintf(stderr, "export: impossible de créer le chemin du PDF\n");
        return -1;
    }

    char dot_path[] = "/tmp/graphik-regex-XXXXXX";
    int dot_fd = mkstemp(dot_path);
    if (dot_fd == -1) {
        perror("export: création du fichier temporaire");
        free(output_path);
        return -1;
    }

    FILE *dot_file = fdopen(dot_fd, "w");
    if (!dot_file) {
        perror("export: ouverture du fichier temporaire");
        close(dot_fd);
        unlink(dot_path);
        free(output_path);
        return -1;
    }

    size_t next_id = 0;
    int write_failed =
        fprintf(dot_file, "digraph Regex {\n  node [shape=circle];\n") < 0 ||
        write_dot_node(dot_file, tree, &next_id) != 0 ||
        fprintf(dot_file, "}\n") < 0;
    if (fclose(dot_file) != 0) write_failed = 1;
    if (write_failed) {
        fprintf(stderr, "export: erreur lors de l'écriture de l'arbre\n");
        unlink(dot_path);
        free(output_path);
        return -1;
    }

    pid_t child = fork();
    if (child == -1) {
        perror("export: lancement de Graphviz");
        unlink(dot_path);
        free(output_path);
        return -1;
    }
    if (child == 0) {
        execlp("dot", "dot", "-Tpdf", "-o", output_path, dot_path, (char *)NULL);
        perror("export: exécution de Graphviz (dot)");
        _exit(127);
    }

    int status;
    pid_t waited;
    do {
        waited = waitpid(child, &status, 0);
    } while (waited == -1 && errno == EINTR);

    unlink(dot_path);
    free(output_path);
    if (waited == -1) {
        perror("export: attente de Graphviz");
        return -1;
    }
    if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
        fprintf(stderr, "export: Graphviz n'a pas pu générer le PDF\n");
        return -1;
    }

    return 0;
}