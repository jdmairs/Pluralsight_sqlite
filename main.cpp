
#include <iostream>
#include "SQLite.h"

int main(int, char**) {
   
    try
    {
        Connection connection = Connection::Memory();
        Statement statement;
        // did not compile with 25 as int
        statement.Prepare(connection, "select ?1 union all select ?2", "Vader", "25");

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
