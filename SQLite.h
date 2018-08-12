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



class Connection
{
    struct ConnectionHandleTraits : HandleTraits<sqlite3 *>
    {
        static void Close(Type value) noexcept
        {
            VERIFY_(SQLITE_OK, sqlite3_close(value));
        }
    };

    using ConnectionHandle = Handle<ConnectionHandleTraits>;

    ConnectionHandle myHandle;

    template <typename Func, typename CharSet>
    void InternalOpen(Func open, CharSet const filename)
    {
        Connection temp;
            if (SQLITE_OK != open(filename, temp.myHandle.Set()))
            {
                temp.ThrowLastError();
            }
            swap(myHandle, temp.myHandle);
    }
    public:

        Connection() noexcept = default;

        template <typename CharSet>
        explicit Connection(CharSet const * const filename)
        {
            Open(filename);
        }

        static Connection Memory()
        {
            return Connection(":memory:");
        }

        explicit operator bool() const noexcept
        {
            return static_cast<bool>(myHandle);
        }
        sqlite3 * GetAbi() const noexcept
        {
            return myHandle.Get();
        }
        __declspec(noreturn) void ThrowLastError() const
        {
            throw Exception(GetAbi());
        }

        void Open(char const * const filename)
        {
            InternalOpen(sqlite3_open, filename);
        }

        void Open(wchar_t const * const filename)
        {
            InternalOpen(sqlite3_open16, filename);
        }


};
