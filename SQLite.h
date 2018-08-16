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

enum class Type
{
    Integer = SQLITE_INTEGER,
    Float = SQLITE_FLOAT,
    Blob = SQLITE_BLOB,
    Null = SQLITE_NULL,
    Text = SQLITE_TEXT,
};

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

        long long RowId() const noexcept
        {
            return sqlite3_last_insert_rowid(GetAbi());
        }

        template <typename F>
        void Profile(F callback, void * const context = nullptr)
        {
            sqlite3_profile(GetAbi(), callback, context);
        }
};

template <typename T>
struct Reader
{
    int GetInt(int const column = 0) const noexcept
    {
        return sqlite3_column_int( static_cast<T const*>(this)->GetAbi(), column);
    }

    char const * GetString(int const column = 0) const noexcept
    {
        return reinterpret_cast<char const *>(sqlite3_column_text(
            static_cast<T const*>(this)->GetAbi(), column));
    }

    int GetStringLength(int const column =0)
    {
        return sqlite3_column_bytes(static_cast<T const*>(this)->GetAbi(), column);
    }

    Type GetType(int const column = 0) const noexcept
    {
        return static_cast<Type>(sqlite3_column_type(static_cast<T const *>(this)->GetAbi(), column));
    }
};

class Statement : public Reader<Statement>
{
    struct StatementHandleTraits  : HandleTraits<sqlite3_stmt *>
    {
        static void Close(Type value) noexcept
        {
            sqlite3_finalize(value);
        }
    };

    using StatementHandle = Handle<StatementHandleTraits>;
    StatementHandle myHandle;

    template <typename F, typename C, typename ... Values>
    void InternalPrepare(Connection const & connection, 
        F prepare, 
        C const * const text,
        Values&& ... values)
    {
        ASSERT(connection);

        if (SQLITE_OK != prepare(connection.GetAbi(), text, -1, myHandle.Set(), nullptr))
        {
            connection.ThrowLastError();
        }

        BindAll(std::forward<Values>(values)...);
    }

    void InternalBind(int) const noexcept
    {}

    template <typename First, typename ... Rest>
    void InternalBind(int const index, First && first, Rest && ... rest) const
    {
        Bind(index, std::forward<First>(first));
        InternalBind(index + 1, std::forward<Rest>(rest)...);
    }

public:

    Statement() noexcept = default;

    template <typename C, typename ... Values>
    Statement(Connection const & connection,
    C const * const text,
    Values && ... values)
    {
        Prepare(connection, text, std::forward<Values>(values)...);
    }
    explicit operator bool() const noexcept
    {
        return static_cast<bool>(myHandle);
    }

    sqlite3_stmt * GetAbi() const noexcept
    {
        return myHandle.Get();
    }

    __declspec(noreturn) void ThrowLastError() const
    {
        throw Exception(sqlite3_db_handle(GetAbi()));
    }

    template <typename ... Values>
    void Prepare(Connection const & connection, char const *const text, Values && ... values)
    {
        InternalPrepare(connection, sqlite3_prepare_v2, text, std::forward<Values>(values)...);
    }

    bool Step() const
    {
        int const result = sqlite3_step(GetAbi());

        if (result == SQLITE_ROW)
            return true;
        if (result == SQLITE_DONE)
            return false;

        ThrowLastError();
    }

    void Execute() const
    {
        VERIFY(!Step());
    }

    void Bind(int const index, int const value) const
    {
        if (SQLITE_OK != sqlite3_bind_int(GetAbi(), index, value))
        {
            ThrowLastError();
        }
    }

    void Bind(int const index, char const * const value, int const size = -1) const
    {
        if (SQLITE_OK != sqlite3_bind_text(GetAbi(), index, value, size, SQLITE_STATIC))
        {
            ThrowLastError();
        }
    }

    void Bind(int const index, wchar_t const * const value, int const size = -1) const
    {
        if (SQLITE_OK != sqlite3_bind_text16(GetAbi(), index, value, size, SQLITE_STATIC))
        {
            ThrowLastError();
        }
    }

    void Bind(int const index, std::string const & value) const
    {
        Bind(index, value.c_str(), value.size());
    }

    void Bind(int const index, std::wstring const & value) const
    {
        Bind(index, value.c_str(), value.size() * sizeof(wchar_t));
    }

    void Bind(int const index, std::string && value) const
    {
        if (SQLITE_OK != sqlite3_bind_text(GetAbi(), index, value.c_str(), value.size(), SQLITE_TRANSIENT))
        {
            ThrowLastError();
        }
    }

    void Bind(int const index, std::wstring && value) const
    {
        if (SQLITE_OK != sqlite3_bind_text16(GetAbi(), index, value.c_str(), value.size() * sizeof(wchar_t), SQLITE_TRANSIENT))
        {
            ThrowLastError();
        }
    }

    template <typename ... Values>
    void BindAll(Values && ...values)
    {
        InternalBind(1, std::forward<Values>(values)...);
    }

    template <typename ... Values>
    void Reset(Values && ... values) 
    {
        if (SQLITE_OK != sqlite3_reset(GetAbi()))
        {
            ThrowLastError();
        }

        BindAll(values ...);
    }
};

class Row : public Reader<Row>
{
    sqlite3_stmt * myStatement = nullptr;
    
    public:

    sqlite3_stmt * GetAbi() const noexcept
    {
        return myStatement;
    }

    Row(sqlite3_stmt * const statement) noexcept :
        myStatement(statement)
        {

        }
};

class RowIterator
{
    Statement const * myStatement = nullptr;

    public:

    RowIterator() noexcept = default;

    RowIterator(Statement const & statement) noexcept
    {
        if (statement.Step())
        {
            myStatement = &statement;
        }
    }

    RowIterator & operator++() noexcept
    {
        if (!myStatement->Step())
        {
            myStatement = nullptr;
        }

        return *this;
    }

    bool operator !=(RowIterator const & other) const noexcept
    {
        return myStatement != other.myStatement;
    }

    Row operator *() const noexcept
    {
        return Row(myStatement->GetAbi());
    }
};

inline RowIterator begin(Statement const & statement) noexcept
{
    return RowIterator(statement);
}

inline RowIterator end(Statement const &) noexcept
{
    return RowIterator();
}

template <typename C, typename ... Values>
void Execute(Connection const & connection,
C const * const text,
Values && ... values)
{
    Statement(connection, text, std::forward<Values>(values) ...).Execute();
}