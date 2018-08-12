
#include <iostream>
#include "SQLite.h"

int main(int, char**) {
   
    try
    {
        Connection connection = Connection::Memory();
        Statement statement;
        statement.Prepare(connection, "select 'Hello world', 1234");
        while (statement.Step())
        {
            std::cout << statement.GetString(0) << " " << statement.GetInt(1) << "\n";
        }
    }
    catch (Exception const &e)
    {
        std::cout << e.Message << " (error code): " << e.Result << "\n";
    }
}
