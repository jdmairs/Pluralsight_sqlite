#pragma once

#include "sqlite3.h"
#include "handle.h"
#include <string>

#if defined(_DEBUG) && defined(_MSC_VER)
#define VERIFY ASSERT
#define VERIFY_(result, expression) ASSERT(result == expression)
#else
#define VERIFY(expression) (expression)
#define VERIFY_(result, expression) (expression)
#endif

struct Exception
{
    int Result = 0;
    std::string Message;

    explicit Exception(sqlite3 * const connection):
        Result(sqlite3_extended_errcode(connection)),
        Message(sqlite3_errmsg(connection))
    {}
};

struct ConnectionHandleTraits : HandleTraits<sqlite3 *>
{
    static void Close(Type value) noexcept
    {
        VERIFY_(SQLITE_OK, sqlite3_close(value));
    }
};

using ConnectionHandle = Handle<ConnectionHandleTraits>;