
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
       
        Execute(connection, "create table Hens (Id, Name)");
        Execute(connection, "insert into Hens (Id, Name) values (?,?)", 101, "Marge");
        Execute(connection, "insert into Hens (Id, Name) values (?,?)", 102, "Jane");

        for (Row row : Statement(connection, "select Id, Name from Hens"))
        {
            std::cout << row.GetInt(0) << " " << row.GetString(1) << "\n";
        }
    }
    catch (Exception const &e)
    {
        std::cout << e.Message << " (error code): " << e.Result << "\n";
    }
}
