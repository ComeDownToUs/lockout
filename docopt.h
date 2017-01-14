#ifndef DOCOPT_H

#define DOCOPT_H

#ifdef __cplusplus
#include <cstdio>
#include <cstdlib>
#include <cstring>
#else
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#endif


typedef struct {
    /* commands */
    int decode;
    int d;
    int encode;
    int e;

    /* arguments */
    char *ciphertextdir;
    char *input;
    char *inputstring;
    char *strength;
    /* options without arguments */
    int commandline;
    int help;
    int version;
    int file;
    /* options with arguments */
    char *password;
    char *read;
    char *salt;
    char *write;
    /* special */
    const char *usage_pattern;
    const char *help_message;
} DocoptArgs;

typedef struct {
    const char *name;
    bool value;
} Command;

typedef struct {
    const char *name;
    char *value;
    char **array;
} Argument;

typedef struct {
    const char *oshort;
    const char *olong;
    bool argcount;
    bool value;
    char *argument;
} Option;

typedef struct {
    int n_commands;
    int n_arguments;
    int n_options;
    Command *commands;
    Argument *arguments;
    Option *options;
} Elements;


/*
 * Tokens object
 */

typedef struct Tokens {
    int argc;
    char **argv;
    int i;
    char *current;
} Tokens;

Tokens tokens_new(int argc, char **argv);
Tokens* tokens_move(Tokens *ts);
int parse_long(Tokens *ts, Elements *elements);
int parse_shorts(Tokens *ts, Elements *elements); 
int parse_argcmd(Tokens *ts, Elements *elements);
int parse_args(Tokens *ts, Elements *elements);
int elems_to_args(Elements *elements, DocoptArgs *args, bool help, const char *version);
DocoptArgs docopt(int argc, char *argv[], bool help, const char *version);

#endif