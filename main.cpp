
#include <iostream>
#include "SQLite.h"

int main(int, char**) {
   
    try
    {
        Connection connection = Connection::Memory();
        Statement statement;
        statement.Prepare(connection, "select 'Hello' union all select  'World'");
        for (Row const & row : statement)
        {
            std::cout << row.GetString(0) << "\n";
        }
    }
    catch (Exception const &e)
    {
        std::cout << e.Message << " (error code): " << e.Result << "\n";
    }
}
