#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "docopt.h"


const char help_message[] =
    "Lockout Arguments\n"
    "\n"
    "Usage:\n"
    "  ./lockout --help\n"
    "  ./lockout --version\n"
    "  ./lockout --commandline (<strength>) <input> [--write=<output>] [--salt=<salt>] [--password=<pw>]\n"
    "  ./lockout (encode|e) (<strength>) (--read=<input>|<inputstring>) [--write=<output>] [--salt=<salt>] [--password=<pw>]\n"
    "  ./lockout (decode|d) (<ciphertextdir>) [<strength>] [--write=<output>] [--salt=<salt>] [--password=<pw>]\n"
    "\n"
    "Options:\n"
    "  -h --help       Show this screen.\n"
    "  -v --version    Show version.\n"
    "  -c --command    Test whole process in one command line entry\n"
    "  -p <pw> --password=<pw>   Inline specification of password, \n"
    "                            No default, will be prompted  \n"
    "  -r <in> --read=<in>       File to be read from, might req verification\n"
    "  -w <out> --write=<out>    name for output file, \n"
    "                            default is \"<datetime>.lockout<strength>\"   \n"
    "  -s <salt> --salt=<salt>   Probably unused, appended to file as means of testing decryption attempts\n"
    "                            means to avoid knowing the password being a liability, \n"
    "                            default is <password>\n"
    "\n"
    "\n"
    "";

const char usage_pattern[] =
    "Usage:\n"
    "  ./lockout --help\n"
    "  ./lockout --version\n"
    "  ./lockout --commandline (<strength>) <input> [--write=<output>] [--salt=<salt>] [--password=<pw>]\n"
    "  ./lockout (encode|e) (<strength>) (--read=<input>|<inputstring>) [--write=<output>] [--salt=<salt>] [--password=<pw>]\n"
    "  ./lockout (decode|d) (<ciphertextdir>) [<strength>] [--write=<output>] [--salt=<salt>] [--password=<pw>]";

Tokens tokens_new(int argc, char **argv) {
    Tokens ts = {argc, argv, 0, argv[0]};
    return ts;
}

Tokens* tokens_move(Tokens *ts) {
    if (ts->i < ts->argc) {
        ts->current = ts->argv[++ts->i];
    }
    if (ts->i == ts->argc) {
        ts->current = NULL;
    }
    return ts;
}


/*
 * ARGV parsing functions
 */

int parse_long(Tokens *ts, Elements *elements) {
    int i;
    int len_prefix;
    int n_options = elements->n_options;
    char *eq = strchr(ts->current, '=');
    Option *option;
    Option *options = elements->options;

    len_prefix = (eq-(ts->current))/sizeof(char);
    for (i=0; i < n_options; i++) {
        option = &options[i];
        if (!strncmp(ts->current, option->olong, len_prefix))
            break;
    }
    if (i == n_options) {
        // TODO '%s is not a unique prefix
        fprintf(stderr, "%s is not recognized\n", ts->current);
        return 1;
    }
    tokens_move(ts);
    if (option->argcount) {
        if (eq == NULL) {
            if (ts->current == NULL) {
                fprintf(stderr, "%s requires argument\n", option->olong);
                return 1;
            }
            option->argument = ts->current;
            tokens_move(ts);
        } else {
            option->argument = eq + 1;
        }
    } else {
        if (eq != NULL) {
            fprintf(stderr, "%s must not have an argument\n", option->olong);
            return 1;
        }
        option->value = true;
    }
    return 0;
}

int parse_shorts(Tokens *ts, Elements *elements) {
    char *raw;
    int i;
    int n_options = elements->n_options;
    Option *option;
    Option *options = elements->options;

    raw = &ts->current[1];
    tokens_move(ts);
    while (raw[0] != '\0') {
        for (i=0; i < n_options; i++) {
            option = &options[i];
            if (option->oshort != NULL && option->oshort[1] == raw[0])
                break;
        }
        if (i == n_options) {
            // TODO -%s is specified ambiguously %d times
            fprintf(stderr, "-%c is not recognized\n", raw[0]);
            return 1;
        }
        raw++;
        if (!option->argcount) {
            option->value = true;
        } else {
            if (raw[0] == '\0') {
                if (ts->current == NULL) {
                    fprintf(stderr, "%s requires argument\n", option->oshort);
                    return 1;
                }
                raw = ts->current;
                tokens_move(ts);
            }
            option->argument = raw;
            break;
        }
    }
    return 0;
}

int parse_argcmd(Tokens *ts, Elements *elements) {
    int i;
    int n_commands = elements->n_commands;
    //int n_arguments = elements->n_arguments;
    Command *command;
    Command *commands = elements->commands;
    //Argument *arguments = elements->arguments;

    for (i=0; i < n_commands; i++) {
        command = &commands[i];
        if (!strcmp(command->name, ts->current)){
            command->value = true;
            tokens_move(ts);
            return 0;
        }
    }
    
    tokens_move(ts);
    return 0;
}

int parse_args(Tokens *ts, Elements *elements) {
    int ret;

    while (ts->current != NULL) {
        if (ts->current[0] == '-' && ts->current[1] == '-') {
            ret = parse_long(ts, elements);
        } else if (ts->current[0] == '-' && ts->current[1] != '\0') {
            ret = parse_shorts(ts, elements);
        } else
            ret = parse_argcmd(ts, elements);
        if (ret) return ret;
    }
    return 0;
}

int elems_to_args(Elements *elements, DocoptArgs *args, bool help,
                  const char *version){
    Command *command;
    Argument *argument;
    Option *option;
    int i;

    // fix gcc-related compiler warnings (unused)
    (void)command;
    (void)argument;

    /* options */
    for (i=0; i < elements->n_options; i++) {
        option = &elements->options[i];
        if (help && option->value && !strcmp(option->olong, "--help")) {
            printf("%s", args->help_message);
            return 1;
        } else if (version && option->value &&
                   !strcmp(option->olong, "--version")) {
            printf("%s\n", version);
            return 1;
        } else if (!strcmp(option->olong, "--commandline")) {
            args->commandline = option->value;
        } else if (!strcmp(option->olong, "--help")) {
            args->help = option->value;
        } else if (!strcmp(option->olong, "--version")) {
            args->version = option->value;
        } else if (!strcmp(option->olong, "--password")) {
            if (option->argument)
                args->password = option->argument;
        } else if (!strcmp(option->olong, "--read")) {
            if (option->argument)
                args->read = option->argument;
        } else if (!strcmp(option->olong, "--salt")) {
            if (option->argument)
                args->salt = option->argument;
        } else if (!strcmp(option->olong, "--write")) {
            if (option->argument)
                args->write = option->argument;
        }
    }
    /* commands */
    for (i=0; i < elements->n_commands; i++) {
        command = &elements->commands[i];
        if (!strcmp(command->name, "d")) {
            args->d = command->value;
        } else if (!strcmp(command->name, "decode")) {
            args->decode = command->value;
        } else if (!strcmp(command->name, "e")) {
            args->e = command->value;
        } else if (!strcmp(command->name, "encode")) {
            args->encode = command->value;
        }
    }
    // /* arguments */
    // COMMENTING OUT AS NOT RUNNING CORRECTLY
    // for (i=0; i < elements->n_arguments; i++) {
    //     argument = &elements->arguments[i];
    //     if (!strcmp(argument->name, "<ciphertextdir>")) {
    //         args->ciphertextdir = argument->value;
    //     } else if (!strcmp(argument->name, "<input>")) {
    //         args->input = argument->value;
    //     } else if (!strcmp(argument->name, "<inputstring>")) {
    //         args->inputstring = argument->value;
    //     } else if (!strcmp(argument->name, "<strength>")) {
    //         args->strength = argument->value;
    //     }
    // }
    return 0;
}


/*
 * Main docopt function
 */

DocoptArgs docopt(int argc, char *argv[], bool help, const char *version) {
    DocoptArgs args = {
        0, 0, 0, 0, NULL, NULL, NULL, NULL, 0, 0, 0, NULL, NULL, NULL, NULL,
        usage_pattern, help_message
    };
    Tokens ts;
    Command commands[] = {
        {"d", 0},
        {"decode", 0},
        {"e", 0},
        {"encode", 0}
    };
    Argument arguments[] = {
        {"<ciphertextdir>", NULL, NULL},
        {"<input>", NULL, NULL},
        {"<inputstring>", NULL, NULL},
        {"<strength>", NULL, NULL}
    };
    Option options[] = {
        {NULL, "--commandline", 0, 0, NULL},
        {"-h", "--help", 0, 0, NULL},
        {"-v", "--version", 0, 0, NULL},
        {"-p", "--password", 1, 0, NULL},
        {"-r", "--read", 1, 0, NULL},
        {"-s", "--salt", 1, 0, NULL},
        {"-w", "--write", 1, 0, NULL}
    };
    Elements elements = {4, 4, 7, commands, arguments, options};

    ts = tokens_new(argc, argv);
    if (parse_args(&ts, &elements))
        exit(EXIT_FAILURE);
    if (elems_to_args(&elements, &args, help, version))
        exit(EXIT_SUCCESS);

    //docopt didn't seem to process a few things correctly
    //so I'm patching it here
    if( args.encode==1 || args.e==1 ){
        args.encode=1;
        if(!strcmp(argv[3],"test")){
            printf("testprocess\n");
        } else{
            args.strength = atoi(argv[3]);
        }
    } else if(args.d == 1 || args.decode == 1){
        args.decode=1;
        args.ciphertextdir = argv[2];
        args.strength = atoi(argv[3]);
    }
    return args;
}
