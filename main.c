#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>



FILE *file;


//futársorszáma dátum db fajta db fajta
// 1 + 1 + 2*6
/*
struct date {
    int type[6];
};

struct data {
    int id;
    struct date date[30];
};

struct data data[1000];
*/
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

        //printf("'%s'\n", ptr);
        //printf("%s\n", data_tmp[i]);
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
    char cwd[255];
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        strcat(cwd,"\\..\\");
        strcat(cwd,filename);
    }
    //else return 1;

    file = fopen(cwd, "r");

    //char c = fgetc(file);
    char str[255];
    int row=0;

    while( fgets(str, 255, file) != NULL )
    {
        /*
        data[i].id = (int)str[0]-'0';
        data[i].date[(int)str[2]-'0'].type[(int)str[4]-'0'] = (int)str[6]-'0';
        */
        parseStr(str);
        row++;
    }

    /*
    for (int row=0; row<i-1; row++)
        printf("%d\n", data[row].date[18].type[1]);
    */

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
