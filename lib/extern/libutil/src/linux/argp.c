/* Copyright (c) 2013-2018 GomSpace A/S. All rights reserved. */

#include <gs/util/linux/argp.h>
#include <gs/util/linux/exitcode.h>
#include <gs/util/string.h>

void gs_argp_parse(const struct argp * argp,
                   int argc, char ** argv,
                   unsigned int flags, int * return_arg_index,
                   const char * revision)
{
    if (gs_string_empty(revision) == false) {
        argp_program_version = revision;
    }

    int arg_index = 0;
    int res = argp_parse(argp, argc, argv, 0, &arg_index, 0);
    if (res) {
        printf("Failed to parse argument/option (result: %d)\n", res);
        exit(GS_EXITCODE_USAGE);
    }

    if ((return_arg_index == NULL) && (arg_index < argc)) {
        // application doesn't expect unhandled arguments
        for (int i = arg_index; i < argc; ++i) {
            printf("Unhandled/unknown argument: [%s]\n", argv[i]);
        }
        exit(GS_EXITCODE_USAGE);
    }

    if (return_arg_index) {
        *return_arg_index = arg_index;
    }
}
