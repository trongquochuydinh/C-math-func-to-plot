#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "parser.h"
#include "post_script.h"

#define MAX_EXPR_LENGTH 256


/* Parse the arguments */
int parse_args(int argc, char *argv[], char **func, char **outfile, double *x_min, double *x_max, double *y_min, double *y_max, int *calc_x_range, int *calc_y_range) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <function> <output file> [x_min:x_max:y_min:y_max]\n", argv[0]);
        return 1;
    }

    /* Allocate a buffer for the cleaned function expression */ 
    char* cleaned_func = (char*)malloc(MAX_EXPR_LENGTH * sizeof(char));
    if (cleaned_func == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    /* Remove whitespace from the function expression */ 
    remove_whitespace(cleaned_func, argv[1]);
    *func = cleaned_func;

    /* Validate the function to ensure it contains only allowed elements */
    if (!validate_expression(*func)) {
        free(cleaned_func);
        return 2;
    }

    *outfile = argv[2];

    FILE *test_file = fopen(*outfile, "w");
    if (test_file == NULL) {
        fprintf(stderr, "Error: Cannot create or write to the file '%s'. Ensure it is a valid file name.\n", *outfile);
        free(cleaned_func);
        return 3;
    }
    /* Close the file after testing */
    fclose(test_file);

    /* Set flags to calculate ranges dynamically by default */
    *calc_x_range = 1;
    *calc_y_range = 1;

    /* Check for optional argument with format x_min:x_max:y_min:y_max */
    if (argc >= 4) {
        double temp_x_min, temp_x_max, temp_y_min, temp_y_max;
        if (sscanf(argv[3], "%lf:%lf:%lf:%lf", &temp_x_min, &temp_x_max, &temp_y_min, &temp_y_max) == 4) {
            *x_min = temp_x_min;
            *x_max = temp_x_max;
            *y_min = temp_y_min;
            *y_max = temp_y_max;
            *calc_x_range = 0;
            *calc_y_range = 0;
        } else {
            fprintf(stderr, "Invalid format for range. Expected x_min:x_max:y_min:y_max\n");
            free(cleaned_func);
            return 4;
        }
    }

    return 0;
}

int main(int argc, char *argv[]) {
    char *func, *outfile;
    double x_min, x_max, y_min, y_max;
    int calc_x_range, calc_y_range;
    int parse_args_status;

    /* Parse arguments */
    parse_args_status = parse_args(argc, argv, &func, &outfile, &x_min, &x_max, &y_min, &y_max, &calc_x_range, &calc_y_range);
    if ( parse_args_status != 0) {
        if (func) free(func);
        return parse_args_status;
    }

    /* Generate PostScript output with default or provided bounds */
    generate_postscript(outfile, func, x_min, x_max, y_min, y_max, calc_x_range, calc_y_range);

    free(func);

    return EXIT_SUCCESS;
}



/*
    TODO:
        1) Add comments to header files for tools like Doxygen
        2) Apply single responsibility principle
        3) Check for unified coding style
        4) Comment more often?
        5) Add param checking for program and functions if needed
        6) Make sure that the program works for Windows and Linux AT LEAST
*/