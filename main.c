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
} data[9];
//struct data data[9];
int pizza_price[6] = {1000,1100,1200,1300,1400,1500};

struct daily_income {
    int day;
    int income;
} order;


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
            for (int date=1; date<=30; date++)
            {
                if (data[id].date[date]>0)
                {
                    printf("\tDay: %d\n", date);
                    printf("\t\t Pizza type:\t");
                    for (int type=0; type<6; type++)
                        printf("%3c ", type+'A');
                    printf("\n");
                    printf("\t\t Pizza amount:\t");
                    for (int type=0; type<6; type++)
                        printf("%3d ", data[id].pizza[date][type]);
                    printf("\n");
                }
            }
            printf("\n");
        }
    }

}
/* qsort struct comparision function (income) */
int compare(void *a, void *b)
{
    struct daily_income *ia = (struct daily_income *)a;
    struct daily_income *ib = (struct daily_income *)b;
    return ib->income - ia->income;
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

void best_pizza_in_month()
{
    int pizza_count[6] = {0};
    int max_pizza = 0;

    for (int id=1; id<=9; id++)
    {
        if (data[id].did_work)
            for (int date=1; date<=30; date++)
                if (data[id].date[date])
                    for (int type=0; type<6; type++)
                        pizza_count[type] += data[id].pizza[date][type];
    }

    // max search
    for (int i=0; i<6; i++)
        if (pizza_count[i]>max_pizza)
            max_pizza = pizza_count[i];

    printf("Pizza stats in month\n");
    for (int i=0; i<6; i++)
            printf("%6c ", i+'A');
    printf("\n");
    for (int i=0; i<6; i++)
            printf("%6d ", pizza_count[i]);
    printf("\n\n");

    printf("Best pizza(s) in month\n");
    for (int i=0; i<6; i++)
        if (pizza_count[i] == max_pizza)
            printf("%6c ", i+'A');
    printf("\n");
    for (int i=0; i<6; i++)
        if (pizza_count[i] == max_pizza)
            printf("%6d ", pizza_count[i]);
    printf("\n\n");
}

void calculate_income()
{
    int income_by_id[10] = {0};
    int max_income = 0;

    for (int id=1; id<=9; id++)
    {
        if (data[id].did_work)
            for (int date=1; date<=30; date++)
                if (data[id].date[date])
                    for (int type=0; type<6; type++)
                        income_by_id[id] += data[id].pizza[date][type] * pizza_price[type];
    }

    for (int id=1; id<=9; id++)
        if (data[id].did_work)
            printf("%d 's income: %d\n", id, income_by_id[id]);
    printf("\n");

    for (int id=1; id<=9; id++)
        if (income_by_id[id]>max_income)
            max_income = income_by_id[id];

    printf("Most incomes by ID's:\n");
    for (int id=1; id<=9; id++)
        if (income_by_id[id]==max_income)
            printf("%d's income: %d\n", id, income_by_id[id]);
    printf("\n");
}

void hardest_working_pizza_dude()
{
    int max_workday = 0;

    for (int id=1; id<=9; id++)
        if (data[id].did_work > max_workday)
            max_workday = data[id].did_work;

    printf("Hardest working by ID:\n");
    for (int id=1; id<=9; id++)
        if (data[id].did_work == max_workday)
            printf("%d's worked days: %d\n", id, max_workday);
    printf("\n");
}

void most_daily_incomes()
{
    struct daily_income daily_income[31] = {0};
    int max_income = 0;

    for (int date=1; date<=30; date++)
        daily_income[date].day = date;

    for (int id=1; id<=9; id++)
    {
        if (data[id].did_work)
            for (int date=1; date<=30; date++)
                if (data[id].date[date])
                    for (int type=0; type<6; type++)
                        daily_income[date].income += data[id].pizza[date][type] * pizza_price[type];
    }


    printf("Daily income:\n");

    for (int date=1; date<=30; date++)
        if (daily_income[date].income>0)
            printf("Day %2d income: %8d\n", daily_income[date].day, daily_income[date].income);
    printf("\n\n");

    qsort(daily_income, 31, sizeof(order), compare);

    printf("4 days with the most income:\n");
    for (int date=1; date<=4; date++)
        if (daily_income[date].income>0)
            printf("Day %2d income: %8d\n", daily_income[date].day, daily_income[date].income);
    printf("\n");
}

void total_pizza_in_month()
{
    int total_pizza = 0;

    for (int id=1; id<=9; id++)
    {
        if (data[id].did_work)
            for (int date=1; date<=30; date++)
                if (data[id].date[date])
                    for (int type=0; type<6; type++)
                        total_pizza += data[id].pizza[date][type];
    }

    printf("Total pizza in month: %d\n", total_pizza);
    printf("\n");
}

void pizza_type_zero_order(int day)
{
    if (day == 0)
    {
        for (int date=1; date<=30; date++)
        {
            int pizza_count[6] = {0};
            int no_pizza = 0;

            for (int id=1; id<=9; id++)
                if (data[id].did_work)
                {
                    for (int type=0; type<6; type++)
                        if (data[id].pizza[date][type] != 0)
                        {
                            pizza_count[ type ]++;
                            //printf("On day %d no orders of pizza type %c.\n", date, type+'A');
                        }
                }

            for (int type=0; type<6; type++)
                if ( pizza_count[type] == 0 )
                    no_pizza++;
            if (no_pizza == 0) printf("On day %2d there are orders of each pizza type.\n", date);
            else
            {
                printf("On day %2d no orders of pizza type: ", date);
                for (int type=0; type<6; type++)
                    if ( pizza_count[type] == 0 )
                        printf("%c ", type+'A');
                printf("\n");
            }
        }

    }
    else
    {
        int pizza_count[6] = {0};
        int no_pizza = 0;

        for (int id=1; id<=9; id++)
            if (data[id].did_work)
                for (int type=0; type<6; type++)
                    if (data[id].pizza[day][type] != 0)
                    {
                        pizza_count[ type ]++;
                        // printf("On day %d no orders of pizza type %c.\n", day, type+'A');
                    }

        for (int type=0; type<6; type++)
            if ( pizza_count[type] == 0 )
            {
                printf("On day %2d no orders of pizza type %c.\n", day, type+'A');
                no_pizza++;
            }
        if (no_pizza == 0) printf("On day %2d there are orders of each pizza type.\n", day);
    }

}

int main()
{
    init_data();
    //if (create_file("in.txt", 90)) return 1;
    create_file("in.txt", 90);
    read_file("in.txt");

    print_data();

    // 1. Melyik típusú pizzából szállítottak ki legtöbbet elsején?
    top_pizza_on_day(1);

    // 2. Hány pizzát szállítottak ki összesen elsején?
    total_pizza_on_day(1);

    // 3. Melyik futár szállította ki a legtöbb pizzát a hónapban?
    top_pizza_dude();

    // 4. Hanyadikán szállították ki a legkevesebb pizzát a hónapban?
    worst_pizza_day();

    // 5. Melyik típusú pizza volt a legkelendőbb a hónap során?
    best_pizza_in_month();

    // 6. Hányas számú futár szedte be a legtöbb pénzt a hónapban?
    calculate_income();

    // 7. Melyik futár dolgozott a legtöbb napon a hónap során?
    hardest_working_pizza_dude();

    // 8. Írassa ki csökkenő sorrendben az első 4 legeredményesebb nap bevételeit.
    most_daily_incomes();

    // 9. Hány pizzát szállítottak ki összesen a hónap során?
    total_pizza_in_month();

    // 10. Kérje be egy nap sorszámát, majd írassa ki, hogy aznap mely típusú pizzából nem rendeltek egy darabot sem!
    printf("Which day to check for no orders of some type of pizza? (Enter 0 to check each day) ");
    pizza_type_zero_order( getchar()-48 );

    return 0;
}
