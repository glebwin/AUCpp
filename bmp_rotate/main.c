#include "bmp.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CODE_SUCCESS 0
#define CODE_WRONG_INPUT 1

bool is_bmp(char *file_name) {
    char *suff = strrchr(file_name, '.');
    if(!suff || file_name == suff)
        return false;
    if(suff[1] == 'b' && suff[2] == 'm' && suff[3] == 'p' && suff[4] == '\0')
        return true;
    else
        return false;
}

int convert_to_int(char *str) {
    int res = 0;
    while(*str != '\0') {
        if(!(*str >= '0' && *str <= '9'))
            return -1;
        res = res * 10 + (*str - '0');
        str++;
    }
    return res;
}

bool parse_input(int argc, char *argv[], char **input, char **output, int *x0, int *y0, int *x1, int *y1) {
    if(argc != 7)
        return false;
    if(!is_bmp(argv[1]) || !is_bmp(argv[2]))
        return false;
    *input = argv[1];
    *output = argv[2];
    *x0 = convert_to_int(argv[3]);
    *y0 = convert_to_int(argv[4]);
    *x1 = convert_to_int(argv[5]);
    *y1 = convert_to_int(argv[6]);
    if(*x0 < 0 || *y0 < 0 || *x1 < 0 || *y1 < 0
       || *x0 > *x1 || *y0 > *y1)
        return false;
    return true;
}

int solve(int argc, char *argv[]) {
    char *input, *output;
    int x0, y0, x1, y1;
    if(!parse_input(argc, argv, &input, &output, &x0, &y0, &x1, &y1)) {
        printf("Error: wrong input format.\nExpected: ./main in.bmp out.bmp x0 y0 x1 y1\n");
        return CODE_WRONG_INPUT;
    }

    Bmp *source = malloc(sizeof(Bmp));
    bmp_read_file(source, input);
    Bmp *result;
    bmp_crop(&result, source, x0, y0, x1, y1);
    bmp_rotate(result);
    bmp_write_file(result, output);

    bmp_finit(source);
    bmp_finit(result);
    free(source);
    free(result);

    return CODE_SUCCESS;
}

int main(int argc, char *argv[]) {
    return solve(argc, argv);
}

