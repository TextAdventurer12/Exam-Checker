#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int between(int val, int min, int max)
{
    return val < max && val >= min;
}

char caps_lock(char letter)
{
    return letter > 'Z' ? letter - ('a' - 'A') : letter;
}

int is_letter(char val)
{
    return between(caps_lock(val), 'A', 'Z'+1);
}

int is_formatted(char *str, int len)
{
    char format[] = "9999-99-AAA-99";
    if (len != strlen(format))
        return 0;
    for (int i = 0; i < len; i++)
    {
        if (format[i] == '9' && !between(str[i] - '0', 0, 10))
            return 0;
        if (format[i] == '-' && str[i] != '-')
            return 0;
        if (format[i] == 'A' && !is_letter(str[i]))
            return 0;
    }
    return 1;
}

int unique_paper(FILE *db, char *paper_code)
{
    fseek(db, 0, SEEK_SET);
    char buf[32];
    while (!feof(db))
        if (!strncmp(paper_code, fgets(buf, 32, db), 14))
            return 0;
    return 1;
}

int add_paper(char *arg, int len)
{
    if (!is_formatted(arg, len))
    {
        printf("Please provide in the format:\n");
        printf("{Subject Code}-{Paper Number}-{Month (Shortened to 3 letters)}-{Last 2 digits of year}\n");
        return 1;
    }
    FILE *db = fopen("completed_papers.db", "r+");
    if (!db)
        return 1;
    if (!unique_paper(db, arg))
    {
        printf("PAPER IS ALREADY PRINTED\n");
        fclose(db);
        return 0;
    }
    fprintf(db, "\n%s", arg);
    fclose(db);
    return 0;
}

int valid_num(char *str)
{
    if (atoi(str))
        return 1;
    return 0;
}

char *get_month(int period)
{
    if (period == 3)
        return "OCT";
    if (period == 2)
        return "MAY";
    if (period == 1)
        return "FEB";
    return NULL;
}

int next_paper(int *period, int *year, int *paper_num)
{
    if (*paper_num < 3)
    {
        ++*paper_num;
        return 0;
    }
    *paper_num = 1;
    if (*period > 1)
    {
        --*period;
        return 0;
    }
    *period = 3;
    --*year;
    if (*year < 0)
        exit(1);
    return 0;
}

char *get_paper(int subject_code, int type)
{
    int period = 3;
    int year = 22;
    int paper_num = 1;
    FILE *db = fopen("completed_papers.db", "r");
    char *paper_code = malloc(16);
    sprintf(paper_code, "0%d-%d%d-%s-%d", subject_code, type, paper_num, get_month(period), year);
    for (; !unique_paper(db, paper_code); next_paper(&period, &year, &paper_num))
        sprintf(paper_code, "0%d-%d%d-%s-%d", subject_code, type, paper_num, get_month(period), year);
    fclose(db);
    return paper_code;
}

int get_papers(int n, int subject_code, int type)
{
    if (n==0)
        return 0;
    char *pp = get_paper(subject_code, type);
    add_paper(pp, strlen(pp));
    printf("VALID PAPER: %s\n", pp);
    free(pp);
    return get_papers(n-1, subject_code, type);
}

int main(int argc, char **argv)
{
    int ADD_PAPER = (argc == 2);
    int GET_PAPER = (argc == 4);
    if (!ADD_PAPER && !GET_PAPER)
        return 1;
    if (ADD_PAPER)
        return add_paper(argv[1], strlen(argv[1]));
    if (!valid_num(argv[1]) || !valid_num(argv[2]) || !valid_num(argv[3]))
    {
        printf("Command Usage:\n");
        printf("./build.sh {number of papers} {subject code} {paper type}\n");
        return 1;
    }
    return get_papers(atoi(argv[1]), atoi(argv[2]), atoi(argv[3]));
}