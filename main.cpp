
#include <iostream>
#include "SQLite.h"

static char const* TypeName(Type const type)
{
    switch (type)
    {
        case Type::Integer : return "Int";
        case Type::Float : return "Float";
        case Type::Blob : return "Blob";
        case Type::Null : return "Null";
        case Type::Text : return "Text";
    }

    ASSERT(false);
    return "Invalid";
}

int main(int, char**) {
   
    try
    {
        // Connection connection = Connection::Memory();
        Connection connection("C:\\Temp\\Count.db");

        //Execute(connection, "create table Things (Content)");
        Statement insert(connection, "insert into Things (Content) values (?1)");

        Execute(connection,"begin");
        for (int i=0; i != 1000; i++)
        {
            insert.Reset(i);
            insert.Execute();
        }

        Execute(connection,"commit");

        Statement count(connection, "select count(*) from Things");
        count.Step();
        std::cout << count.GetInt() << "\n";
        
        // for (Row row : Statement(connection, "select Name from Hens"))
        // {
        //     std::cout << row.GetString(0) << "\n";
        // }
    }
    catch (Exception const &e)
    {
        std::cout << e.Message << " (error code): " << e.Result << "\n";
    }
}
