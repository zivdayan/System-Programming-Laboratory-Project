
#include <stdio.h>
#include <stdlib.h>
#include "preprocessor.h"
#include "structs.h"
#include "firstPass.h"
#include "secondPass.h"
#include "backend.h"

/**
 * Main function to handle and assemble an input file.
 *
 * Input:
 * The function gets as an input arguments through the command line, and generated the required
 * output files after assembly.
 * Example: ./assembler example, will initiate assembly process to example.as file, and will
 * generate example.am, example.ob, example.ext, example.ent
 *
 * @param argc The number of input arguments.
 * @param argv An array of strings containing the input arguments.
 * @return Returns 0 on successful assembly.
 */

int main(int argc, char *argv[])
{
    char **pargv;
    struct translation_unit *program;
    /* char *as_file, *am_file */;
    if (argc < 2)
    {
        printf("ERROR: You need at least one argument.\n Example: ./assembler example");
        return 1;
    }
    for (pargv = argv + 1; *pargv != argv[argc]; pargv++)
    {
        char *amFileName = "";
        FILE *amFile;
        char *current_file = *pargv;
        struct Node *output_macro_list = NULL;

        printf("Starting preprocessor - %s \n", current_file);

        amFileName = preproc(current_file, output_macro_list);

        if (!strcmp(amFileName, ""))
            continue;

        printf("Preprocessor done, file name: %s \n", amFileName);

        amFile = fopen(amFileName, "r");

        printf("Starting first pass - %s \n", current_file);

        /* -- Execute First pass -- */

        program = malloc(sizeof(struct translation_unit));

        if (!firstPass(program, amFileName, amFile, output_macro_list))
        {
            rewind(amFile);
            printf("Starting second pass - %s \n", current_file);

            /* -- Execute Second pass -- */
            if (!secondPass(program, amFileName, amFile, output_macro_list))
            {

                print_ob_file(current_file, &program);
                if (program->entries_count >= 1)
                    print_ent_file(current_file, &program);

                if (program->extCount >= 1)
                    print_ext_file(current_file, &program);
            }
        }
    }
    printf("end\n");
    return 0;
}
