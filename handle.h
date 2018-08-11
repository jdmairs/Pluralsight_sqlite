#pragma once

#ifdef _DEBUG
#include <crtdbg.h>
#define ASSERT _ASSERTE
#else
    #define ASSERT __noop
#endif

template <typename T>
struct HandleTraits
{
    using Type=T;

    static Type Invalid() noexcept
    {
        return nullptr;
    }
    // static void Close(Type value) noexcept

};

template <typename Traits>
class Handle
{
    using Type=decltype(Traits::Invalid());
    Type mValue;

    void Close() noexcept
    {
        if (*this)
        {
            Traits::Close(mValue);
        }
    }

    public:

    Handle(Handle const &) = delete;
    Handle & operator=(Handle const&) = delete;

    explicit Handle(Type value = Traits::Invalid()) noexcept:
        mValue(value)
        {}

    Handle(Handle && other) noexcept:
    mValue(other.Detach())
    {}
    
    Handle & operator=(Handle && other) noexcept
    {
        if (this != &other)
        {
            Reset(other.Detach());
        }

        return *this;
    }

    ~Handle() noexcept
    {
        Close();
    }

    explicit operator bool() const noexcept
    {
        return mValue != Traits::Invalid();
    }

    Type Get() const noexcept
    {
        return mValue;
    }

    Type * Set() noexcept
    {
        ASSERT(!*this);
        return &mValue;
    }

    Type Detach() noexcept
    {
        Type value = mValue;
        mValue = Traits::Invalid();
        return value;
    }

    bool Reset(Type value = Traits::Invalid()) noexcept
    {
        if (mValue != value)
        {
            Close();
            mValue = value;
        }

        return static_cast<bool>(*this);
    }

    void Swap(Handle<Traits> & other) noexcept
    {
        Type temp = mValue;
        mValue = other.mValue;
        other.mValue = temp;
    }
};

template <typename Traits>
void swap(Handle<Traits>& left, Handle<Traits>& right) noexcept
{
    left.Swap(right);
}

template <typename Traits>
bool operator==(Handle<Traits> const & left, Handle<Traits> const & right) noexcept
{
    return left.Get() == right.Get();
}

template <typename Traits>
bool operator!=(Handle<Traits> const & left, Handle<Traits> const & right) noexcept
{
    return !(left == right); 
}
