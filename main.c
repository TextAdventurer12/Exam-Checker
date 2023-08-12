#include <stdio.h>
#include <string.h>

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

int main(int argc, char **argv)
{
    if (argc != 2)
        return 1;
    printf("Checking: %s\n", argv[1]);
    if (!is_formatted(argv[1], strlen(argv[1])))
    {
        printf("Please provide in the format:\n");
        printf("{Subject Code}-{Paper Number}-{Month (Shortened to 3 letters)}-{Last 2 digits of year}\n");
        return 1;
    }
    printf("FORMAT VALIDATED\n");
    FILE *db = fopen("completed_papers.pp", "r+");
    if (!db)
        return 1;
    char buf[32];
    while (!feof(db))
    {
        if (!strcmp(argv[1], fgets(buf, 32, db)))
        {
            printf("PAPER IS ALREADY PRINTED\n");
            fclose(db);
            return 0;
        }
    }
    printf("PAPER IS UNIQUE\n");
    fprintf(db, "\n%s", argv[1]);
    fclose(db);
    return 0;
}