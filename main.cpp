
#include <iostream>
#include "SQLite.h"

int main(int, char**) {
    ConnectionHandle connection;

    try
    {
        if (SQLITE_OK != sqlite3_open(":badmemory:", connection.Set()))
        {
            throw Exception(connection.Get());
        }
    }
    catch (Exception const &e)
    {
        std::cout << e.Message << " (error code): " << e.Result << "\n";
    }
}
