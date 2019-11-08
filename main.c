#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>

FILE *file;

/*
 *  0    1   2   3    4   5
 * ID dátum db fajta db fajta
*/
struct data {
    int did_work;
    int date[30];
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

    data[ data_tmp[0] ].did_work++;
    data[ data_tmp[0] ].date[ data_tmp[1] ]++;

    for (i=2; i<14; i+=2)
    {
        data[ data_tmp[0] ].pizza[ data_tmp[1] ][ data_tmp[i+1] ] += data_tmp[i]; // same ID can have multiple identical day records with different pizza amounts -> add them together
    }
/*
    for (i=0; i<14; i++)
        printf("%d ", data_tmp[i]);
    printf("\n\n");
    */
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
        id = rand() % 9 + 1; // 1-9
        date = rand() % 29 + 1; // 1-30

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

void init_data ()
{
    for (int i=0; i<9; i++)
    {
        data[i].did_work = 0;
        for (int j=0; j<30; j++)
            data[i].date[j] = 0;
    }
}

void print_data ()
{
    for (int i=0; i<9; i++)
    {
        if (data[i].did_work>0)
        {
            printf("ID: %d\n", i);
            for (int date=0; date<30; date++)
            {
                if (data[i].date[date]>0)
                {
                    printf("\tDay: %d\n", date);
                    printf("\t\t Pizza type:\t");
                    for (int type=0; type<6; type++)
                        printf("%c ", type+'A');
                    printf("\n");
                    printf("\t\t Pizza amount:\t");
                    for (int type=0; type<6; type++)
                        printf("%d ", data[i].pizza[date][type]);
                    printf("\n");
                }
            }
            printf("\n");
        }
    }
}

int top_pizza_1 ()
{
    int pizza_count[6] = {0,0,0,0,0,0};

    for (int id=0; id<9; id++)
    {
        if (data[id].did_work)
            for (int date=0; date<30; date++)
                if (data[id].date[date])
                    for (int type=0; type<6; type++)
                        pizza_count[type] += data[id].pizza[date][type];
    }

    for (int i=0; i<6; i++)
        printf("%d ", pizza_count[i]);

    return 0;
}

int main()
{
    init_data();
    //if (create_file("in.txt", 2)) return 1;
    read_file("in.txt");

    print_data();


    // 1. Melyik típusú pizzából szállítottak ki legtöbbet elsején?
    top_pizza_1();


    return 0;
}
