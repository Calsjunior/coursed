#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *open_csv_file(const char *filename);
sqlite3 *setup_db();
void create_tables(sqlite3 *db);

const char *csv_ext = ".csv";

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Usage ./coursed <curriculum.csv> <available.csv>\n");
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

    sqlite3 *db = setup_db();
    if (db == NULL)
    {
        fclose(csv_file1);
        fclose(csv_file2);
        return 1;
    }

    create_tables(db);

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
        fprintf(stderr, "Error: Cannot open file %s\n", filename);
        return NULL;
    }
    return file;
}

sqlite3 *setup_db()
{
    sqlite3 *db;
    int rc = sqlite3_open("courses.db", &db);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Error: Cannot open database %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return NULL;
    }
    return db;
}

void create_tables(sqlite3 *db)
{
    char *err_msg = 0;
    int rc;
    const char *sql_curriculum = "CREATE TABLE IF NOT EXISTS curriculum ("
                                 "id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, "
                                 "code TEXT NOT NULL, "
                                 "title TEXT NOT NULL, "
                                 "credits INTEGER NOT NULL, "
                                 "status TEXT NOT NULL);";

    const char *sql_offer = "CREATE TABLE IF NOT EXISTS offered ("
                            "id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, "
                            "code TEXT NOT NULL, "
                            "title TEXT NOT NULL, "
                            "credits INTEGER NOT NULL, "
                            "time TEXT NOT NULL);";

    rc = sqlite3_exec(db, sql_curriculum, 0, 0, &err_msg);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Table error: %s\n", err_msg);
        sqlite3_free(err_msg);
        return;
    }

    rc = sqlite3_exec(db, sql_offer, 0, 0, &err_msg);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Table error: %s\n", err_msg);
        sqlite3_free(err_msg);
        return;
    }
}
