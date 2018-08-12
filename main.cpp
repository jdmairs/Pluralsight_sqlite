
#include <iostream>
#include "SQLite.h"

int main(int, char**) {
    ConnectionHandle connection;

    int result = sqlite3_open(":memory:", connection.Set());

    if (SQLITE_OK != result)
    {
        std::cout << sqlite3_errmsg(connection.Get());
        return result;
    }
}
