#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <limits.h>


FILE *file;


//futársorszáma dátum db fajta db fajta

struct date {
    int type[6];
};

struct id {
    int id;
    struct date date[30];
};

struct id data[1000];


int create_file(char filename[30], int rows)
{
    int id, date, amount, random;

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

        random = rand()%9+1;
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
    char cwd[255];
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        strcat(cwd,"\\..\\");
        strcat(cwd,filename);
    }
    else return 1;

    file = fopen(cwd, "r");

    //char c = fgetc(file);
    char str[255];
    int i=0;
    int date, amount,type;

    while( fgets(str, 255, file) != NULL )
    {
        data[i].id = (int)str[0]-'0';
        data[i].date[(int)str[2]-'0'].type[(int)str[4]-'0'] = (int)str[6]-'0';
        i++;
    }

    for (int row=0; row<i-1; row++)
        printf("%d\n", data[row].date[18].type[1]);


    /*
    while (c != EOF)
    {
        //printf ("%c", c);
        data[i].id = (int)c;
        c = fgetc(file); // space
        date = (int)fgetc(file); // date
        c = fgetc(file); // space
        amount = (int)fgetc(file); // amount
        c = fgetc(file); // space
        type = (int)fgetc(file); // type
        data[i].date[date].type[(int)c] = amount;
        c = fgetc(file); // maybe space

        do
        {
            c = fgetc(file); // space
            data[i].date[date].type[type] = (int)c;
            c = fgetc(file);
        }
        while (c != '\0')
        i++;
    }
    */

    fclose(file);
}

int main()
{
    //if (create_file("in.txt", 20)) return 1;
    read_file("in.txt");

    return 0;
}
