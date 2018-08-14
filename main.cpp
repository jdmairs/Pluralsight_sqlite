
#include <iostream>
#include "SQLite.h"

int main(int, char**) {
   
    try
    {
        Connection connection = Connection::Memory();
        Statement statement;
        statement.Prepare(connection, "select ?1 union all select ?2");
        std::string lvalue{"c14 rules"};
        statement.BindAll(std::string("hello"), lvalue);

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
