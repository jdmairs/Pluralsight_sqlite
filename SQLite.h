#pragma once

#include "sqlite3.h"
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