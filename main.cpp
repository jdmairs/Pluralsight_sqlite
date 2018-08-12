#include "sqlite3.h"
#include <iostream>
#include "handle.h"

#if defined(_DEBUG) && defined(_MSC_VER)
#define VERIFY ASSERT
#define VERIFY_(result, expression) ASSERT(result == expression)
#else
#define VERIFY(expression) (expression)
#define VERIFY_(result, expression) (expression)
#endif

struct ConnectionHandleTraits : HandleTraits<sqlite3 *>
{
    static void Close(Type value) noexcept
    {
        VERIFY_(SQLITE_OK, sqlite3_close(value));
    }
};

using ConnectionHandle = Handle<ConnectionHandleTraits>;

int main(int, char**) {
    ConnectionHandle connection;

    int result = sqlite3_open(":memory:", connection.Set());

    if (SQLITE_OK != result)
    {
        std::cout << sqlite3_errmsg(connection.Get());
        return result;
    }
}
