#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int main(int argc, char *argv[]) {

    if (argc != 3) {
        fputs("Error! Not enough params\n", stderr);
        return 1;   
    }

    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        fputs("Error! File not found\n", stderr);
        return 1;
    }

    long N = strtol(argv[2], NULL, 10);
    if (N == 0) {
        while (!feof(file))
            putc(fgetc(file), stdout); 
    } else if (N > 0) {
        int k = 0;
        while (N != k  && !feof(file)) {
            char symbol = fgetc(file);
            if (symbol == '\n') k++;
            putc(symbol, stdout);
        }
    } else {
        fputs("Error! Wrong 2nd parameter: it has to be possitive\n", stderr);
        return 1;
    }

    fclose(file);
    return 0;

}