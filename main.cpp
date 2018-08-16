
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

static void SaveToDisk(Connection const & source, char const * const filename)
{
    Connection destination(filename);
    Backup backup(destination, source);
    backup.Step();
}

int main(int, char**) {
   
    try
    {
        Connection connection = Connection::Memory();
        
        Execute(connection, "create table Things (Content real)");

        Statement statement(connection, "insert into Things values (?)");

        for (int i = 0; i != 1000000; ++i)
        {
            statement.Reset(i);
            statement.Execute();
        }

        SaveToDisk(connection, "C:\\temp\\backup.db");

    }
    catch (Exception const &e)
    {
        std::cout << e.Message << " (error code): " << e.Result << "\n";
    }
}
