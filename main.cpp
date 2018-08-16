
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
              
    }
    catch (Exception const &e)
    {
        std::cout << e.Message << " (error code): " << e.Result << "\n";
    }
}
