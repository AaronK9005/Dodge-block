#include "error_util.hpp"

#include <Windows.h>

std::wstring getLastErrorMessage()
{
    DWORD error = GetLastError();

    wchar_t* buffer = nullptr;

    FormatMessageW(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr,
        error,
        0,
        reinterpret_cast<wchar_t*>(&buffer),
        0,
        nullptr
    );

    std::wstring message = buffer ? buffer : L"Unknown error";

    if (buffer)
        LocalFree(buffer);

    return message;
}