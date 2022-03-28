#include <stdio.h>
#include <ctype.h>

int main(int argc[], char *argv[]) {

    if (argv[1] == NULL) {
        fputs("Error! No params\n", stderr);
        return 1;   
    }

    FILE *file = fopen(argv[1], "w");
    if (file == NULL) {
        fputs("Error! File not found\n", stderr);
        return 1;
    }

    while (1) {
        char symbol = getc(stdin);
        if (iscntrl(symbol))
            continue;
        if (symbol == '\e')
            break;
        fputc(symbol, file);
    }

    fclose(file);

    return 0;

}