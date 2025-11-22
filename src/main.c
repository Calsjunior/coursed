#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *open_csv_file(const char *filename);
sqlite3 *db_setup();

const char *csv_ext = ".csv";

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Usage ./compare_course <curriculum.csv> <available.csv>\n");
        return 1;
    }

    FILE *csv_file1 = open_csv_file(argv[1]);
    if (csv_file1 == NULL)
    {
        return 1;
    }

    FILE *csv_file2 = open_csv_file(argv[2]);
    if (csv_file2 == NULL)
    {
        fclose(csv_file1);
        return 1;
    }

    sqlite3 *db = db_setup();
    if (db == NULL)
    {
        fclose(csv_file1);
        fclose(csv_file2);
        return 1;
    }

    sqlite3_close(db);
    fclose(csv_file1);
    fclose(csv_file2);

    return 0;
}

FILE *open_csv_file(const char *filename)
{
    char *dot = strrchr(filename, '.');
    if (dot == NULL || strcmp(dot, csv_ext) != 0)
    {
        fprintf(stderr, "Error: File '%s' must end with .csv\n", filename);
        return NULL;
    }

    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Error: Could not open file: %s\n", filename);
        return NULL;
    }
    return file;
}

sqlite3 *db_setup()
{
    sqlite3 *db;
    int rc = sqlite3_open("courses.db", &db);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return NULL;
    }
    return db;
}
