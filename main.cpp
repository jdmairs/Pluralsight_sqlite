
#include <iostream>
#include "SQLite.h"

int main(int, char**) {
   
    try
    {
        Connection connection = Connection::Memory();
       
        Execute(connection, "create table Users (Name)");
        Execute(connection, "insert into Users values(?)", "Joe");
        Execute(connection, "insert into Users values(?)", "Beth");

        for (Row row : Statement(connection, "select * from Users"))
        {
            std::cout << row.GetString() << "\n";
        }
    }
    catch (Exception const &e)
    {
        std::cout << e.Message << " (error code): " << e.Result << "\n";
    }
}
