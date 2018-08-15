
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
        Connection connection = Connection::Memory();
       
        Execute(connection, "create table Hens (Name)");
        Execute(connection, "insert into Hens (Name) values (?)", "Marge");
        Execute(connection, "insert into Hens (Name) values (?)",  "Jane");
        Execute(connection, "insert into Hens (Name) values (?)",  "Greta");
        std::cout << "Inserted " << connection.RowId() << "\n";

        for (Row row : Statement(connection, "select Name from Hens"))
        {
            std::cout << row.GetString(0) << "\n";
        }
    }
    catch (Exception const &e)
    {
        std::cout << e.Message << " (error code): " << e.Result << "\n";
    }
}
