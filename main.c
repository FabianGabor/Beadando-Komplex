#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <limits.h>


FILE *file;

int create_file(char filename[30], int rows)
{
    int id, date, amount, random;
    char type;

    char cwd[255];
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        strcat(cwd,"\\..\\");
        strcat(cwd,filename);
    }
    else return 1;

    file = fopen(cwd, "w");

    for (int i=0; i<rows; i++)
    {
        //futársorszáma dátum db fajta db fajta
        id = rand() % 9; // ID 0-9
        date = rand() % 30 + 1;

        fprintf(file, "%d %d", id, date);

        random = rand()%9+1;
        for (int j=0; j<random; j++)
        {
            amount = rand() % 9 + 1;
            type = rand() % 5 + 65;
            fprintf(file, " %d %c", amount, type);
        }

        if (i<rows-1) fprintf(file, "\n");
    }

    return 0;
}

void read_file()
{
    char c = fgetc(file);
    while (c != EOF)
    {
        printf ("%c", c);
        c = fgetc(file);
    }

    fclose(file);
}

int main()
{
    if (create_file("in.txt", 30)) return 1;
    //read_file();

    return 0;
}
