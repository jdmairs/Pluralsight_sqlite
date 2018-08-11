#pragma once

#ifdef _DEBUG
#include <crtdbg.h>
#define ASSERT ASSERTE
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

};

template <typename Traits>
struct HandleTraits
{
    using Type=decltype(Traits::Invalid());
    Type mValue;

    void close() noexcept
    {
        if (*this)
        {
            Traits::close(mValue);
        }
    }
};