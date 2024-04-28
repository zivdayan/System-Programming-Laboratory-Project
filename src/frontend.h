#define MAX_LENGTH_DATA_TYPE 80
#define SYNTAX_ERROR_LENGTH 200
#include "global_consts.h"
#include <stdlib.h>
#include <string.h>


struct string_sep_result {
    char *strings[80];
    int strings_count;
};

struct ast {
    char lineError[SYNTAX_ERROR_LENGTH];
    const char * labelName;
    /* Define all possible directives */ 
    enum {
        ast_inst,
        ast_dir,
        ast_define,
        ast_empty
    } line_type;
    union {
        struct {
            const char *label;
            int number;
        } define;
        struct {
            enum {
                ast_extern,
                ast_entry,
                ast_string,
                ast_data
            } dir_type;

            union {
                char * label;
                char * string;
                struct {
                    enum {
                        data_label,
                        data_number
                    } data_type;
                    union {
                        char *label;
                        int number;
                    } data_options;
                } data[80];
            } dir_options;

        } dir;

        /* Define all possible instructions */
        struct {
            enum {
                inst_mov,
                inst_cmp,
                inst_add,
                inst_sub,
                inst_not,
                inst_clr,
                inst_lea,
                inst_inc,
                inst_dec,
                inst_jmp,
                inst_bne,
                inst_red,
                inst_prn,
                inst_jsr,
                inst_rts,
                inst_hlt
            } inst_type;

            struct {
                enum {
                    addrs_immed,
                    addrs_labe,
                    adddrs_index_const,
                    adddrs_index_label,
                    addrs_register
                } addrs_mode;
                union {
                    int *immed;
                    char *label;
                    int reg;
                    /* Define the index addressing case - Example: mov x[2], r2 */ 
                    struct {
                        char * label;
                        union {
                            int number;
                            char * label;
                        } index_option;
                    } index;
                }operand_options;
            } operands[2];
        } inst;
    } ast_options;

};

struct ast *get_ast_from_line(char* line);

int is_keyword(char *str, char *collection[], int length);
static int is_number(char *str, int max, int min, int * result);
static int is_instruction_line(char* line, struct string_sep_result ssr);