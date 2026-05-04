#ifndef GRAPH_DUMP
#define GRAPH_DUMP

#include "../../M_compile.h"


void call_terminal(const char* text);

void draw_stick(FILE *fp_out, struct leaf *leaf);

void draw_clear_leaf(FILE *fp_out, struct leaf *leaf);

const char* translate_r(size_t oper);

void picture_of_root(struct leaf *root, const char* text);


#endif