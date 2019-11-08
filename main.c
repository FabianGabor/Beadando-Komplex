/*
 * Folyamatban
 * https://github.com/FabianGabor/Beadando-Komplex
 */

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
    int date[31];
    int pizza[30][6];
};
struct data data[9];


void parseStr (char str[255])
{
    int i=0;
    int data_tmp[14] = {0};
    char delim[] = " ";
    char *ptr = strtok(str, delim);

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
        str[0] = '\0';
        row++;
    }

    fclose(file);
}

void init_data ()
{
    for (int i=1; i<=9; i++)
    {
        data[i].did_work = 0;
        for (int j=0; j<=30; j++)
            data[i].date[j] = 0;
    }
}

void print_data ()
{
    for (int id=1; id<=9; id++)
    {
        if (data[id].did_work>0)
        {
            printf("ID: %d\n", id);
            printf("Work count: %d\n", data[id].did_work);
            for (int date=0; date<=30; date++)
            {
                if (data[id].date[date]>0)
                {
                    printf("\tDay: %d\n", date);
                    printf("\t\t Pizza type:\t");
                    for (int type=0; type<6; type++)
                        printf("%c ", type+'A');
                    printf("\n");
                    printf("\t\t Pizza amount:\t");
                    for (int type=0; type<6; type++)
                        printf("%d ", data[id].pizza[date][type]);
                    printf("\n");
                }
            }
            printf("\n");
        }
    }

}

int cmpfunc (void * a, void * b) {
    return ( *(int*)b - *(int*)a );
}

int top_pizza_on_day(int day)
{
    int pizza_count[6] = {0};
    int date = day;
    int max_pizza = 0;

    for (int id=1; id<=9; id++)
    {
        if (data[id].did_work)
            //for (int date=0; date<30; date++)
                if (data[id].date[date])
                    for (int type=0; type<6; type++)
                        pizza_count[type] += data[id].pizza[date][type];
    }

    // max search
    for (int i=0; i<6; i++)
        if (pizza_count[i]>max_pizza)
            max_pizza = pizza_count[i];
    printf("Top pizza(s) on day %d\n", day);

    for (int i=0; i<6; i++)
        if (pizza_count[i] == max_pizza)
            printf("%6c ", i+'A');
    printf("\n");
    for (int i=0; i<6; i++)
        if (pizza_count[i] == max_pizza)
            printf("%6d ", pizza_count[i]);
    printf("\n\n");

    return 0;
}

int total_pizza_on_day(int day)
{
    int pizza_count[6] = {0};
    int date = day;
    int total_pizza = 0;

    for (int id=1; id<=9; id++)
    {
        if (data[id].did_work)
            //for (int date=0; date<30; date++)
                if (data[id].date[date])
                    for (int type=0; type<6; type++)
                    {
                        pizza_count[type] += data[id].pizza[date][type];
                        total_pizza += data[id].pizza[date][type];
                    }
    }

    printf("Total pizza(s) on day %d: %d\n", day, total_pizza);

    for (int i=0; i<6; i++)
        printf("%6c ", i+'A');
    printf("\n");
    for (int i=0; i<6; i++)
        printf("%6d ", pizza_count[i]);
    printf("\n\n");

    return 0;
}

int top_pizza_dude()
{
    int pizza_dude_count[9] = {0};
    int max_pizza = 0;


    for (int id=1; id<=9; id++)
    {
        if (data[id].did_work)
            for (int date=0; date<=30; date++)
                if (data[id].date[date])
                    for (int type=0; type<6; type++)
                        pizza_dude_count[id] += data[id].pizza[date][type];
    }

    printf("Pizza dudes monthly stats:\n");
    printf("ID:\t");
    for (int i=1; i<=9; i++)
        printf("%6d ", i);
    printf("\n");

    printf("Amount:\t");
    for (int i=1; i<=9; i++)
        printf("%6d ", pizza_dude_count[i]);
    printf("\n\n");

    // max search
    for (int i=0; i<6; i++)
        if (pizza_dude_count[i]>max_pizza)
            max_pizza = pizza_dude_count[i];
    printf("Best pizza dude(s) in month:\n");

    printf("ID:\t");
    for (int i=1; i<=9; i++)
        if (pizza_dude_count[i] == max_pizza)
            printf("%6d ", i);
    printf("\n");

    printf("Amount:\t");
    for (int i=1; i<=9; i++)
        if (pizza_dude_count[i] == max_pizza)
            printf("%6d ", pizza_dude_count[i]);
    printf("\n\n");

    return 0;
}

int worst_pizza_day()
{
    int day_count[31] = {0};
    int min_pizza = 0;

    for (int id=1; id<=9; id++)
    {
        if (data[id].did_work)
            for (int date=0; date<=30; date++)
                if (data[id].date[date])
                    for (int type=0; type<6; type++)
                        day_count[date] += data[id].pizza[date][type];
    }

    // min search
    min_pizza = day_count[0];
    for (int i=1; i<=30; i++)
        if (day_count[i]<min_pizza)
            min_pizza = day_count[i];

    printf("Daily pizza amounts:\n");
    printf("Day:\t");
    for (int i=1; i<=30; i++)
        //if (day_count[i] == min_pizza)
            printf("%4d ", i);
    printf("\n");
    printf("Amount:\t");
    for (int i=1; i<=30; i++)
        //if (day_count[i] == min_pizza)
            printf("%4d ", day_count[i]);
    printf("\n\n");

    printf("Worst pizza day(s):\n");
    printf("Day:\t");
    for (int i=1; i<=30; i++)
        if (day_count[i] == min_pizza)
            printf("%4d ", i);
    printf("\n");
    printf("Amount:\t");
    for (int i=1; i<=30; i++)
        if (day_count[i] == min_pizza)
            printf("%4d ", day_count[i]);
    printf("\n\n");



    return 0;
}


int main()
{
    init_data();
    if (create_file("in.txt", 100)) return 1;
    read_file("in.txt");

    print_data();
/*

    // 1. Melyik típusú pizzából szállítottak ki legtöbbet elsején?
    top_pizza_on_day(1);

    // 2. Hány pizzát szállítottak ki összesen elsején?
    total_pizza_on_day(1);

    // 3. Melyik futár szállította ki a legtöbb pizzát a hónapban?
    top_pizza_dude();
*/
    // 4. Hanyadikán szállították ki a legkevesebb pizzát a hónapban?
    worst_pizza_day();

    return 0;
}
