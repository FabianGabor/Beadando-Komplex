#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>

FILE *file;

//futársorszáma dátum db fajta db fajta
struct data {
    int pizza[30][6];
};
struct data data[9];


void parseStr (char str[255])
{
    int i=0;
    int data_tmp[14];
    char delim[] = " ";
    char *ptr = strtok(str, delim);

    for (i=0; i<14; i++)
        data_tmp[i] = 0;

    i=0;
    while(ptr != NULL)
    {
        if (atoi(ptr) >0)
            data_tmp[i] = atoi(ptr);
        else
            data_tmp[i] = (*ptr - 'A');

        ptr = strtok(NULL, delim);
        i++;
    }

    for (i=0; i<14; i++)
        printf("%2d ", data_tmp[i]);
    printf("\n");

}

int create_file(char filename[30], int rows)
{
    int id, date, amount;

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
        id = rand() % 9; // 0-9
        date = rand() % 30 + 1; // 1-30

        fprintf(file, "%d %d", id, date);


        for (int j=0; j<6; j++) // 0-6 => A-F
        {
            amount = rand() % 9; // 0-9
            if (amount > 0)
                fprintf(file, " %d %c", amount, j+65); // 0-6 + ASCII => A-F
        }

        if (i<rows-1) fprintf(file, "\n"); // no empty last row
    }
    fclose(file);

    return 0;
}

void read_file(char filename[30])
{
    char str[255];
    int row=0;

    char cwd[255];
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        strcat(cwd,"\\..\\");
        strcat(cwd,filename);
    }
    //else return 1;

    file = fopen(cwd, "r");

    while( fgets(str, 255, file) != NULL )
    {        
        parseStr(str);
        row++;
    }

    fclose(file);
}

int main()
{
    //if (create_file("in.txt", 20)) return 1;
    read_file("in.txt");

    return 0;
}
