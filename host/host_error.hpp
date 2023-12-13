/**
 * @file host_error.hpp
 * @author simakeng (simakeng@outlook.com)
 * @brief
 * @version 0.1
 * @date 2023-12-12
 *
 * @copyright Copyright (c) 2023
 *
 */
#include <string>

#include <stdio.h>
#include <stdlib.h>

#include <Windows.h>

#ifndef __HOST_ERROR_HPP__
#define __HOST_ERROR_HPP__

#define COLOR_RESET "\033[0m"
#define COLOR_RED "\033[31m"
#define COLOR_DARKGREEN "\033[32m"
#define COLOR_YELLOW "\033[33m"
#define COLOR_BLUE "\033[34m"
#define COLOR_MAGENTA "\033[35m"
#define COLOR_CYAN "\033[36m"
#define COLOR_GRAY "\033[37m"
#define COLOR_PINK "\033[91m"
#define COLOR_GREEN "\033[92m"
#define COLOR_BEIGE "\033[93m"
#define COLOR_ROYALBLUE "\033[94m"
#define COLOR_PURPLE "\033[95m"
#define COLOR_TEAL "\033[96m"
#define COLOR_WHITE "\033[97m"

#if !defined(__PRETTY_FUNCTION__) && !defined(__GNUC__)
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif

#define EXPAND_VALUE(x) #x
#define LINE_NUMBER(x) EXPAND_VALUE(x)
#define LINE_INFO __FILE__ ":" LINE_NUMBER(__LINE__)

#define error_exit(...)                                                   \
    do                                                                    \
    {                                                                     \
        const char *func_name = __PRETTY_FUNCTION__;                      \
        fprintf(stderr,                                                   \
                "[ERROR INFO]\n"                                          \
                "\n"                                                      \
                "Message:\n" COLOR_YELLOW __VA_ARGS__);                   \
                                                                          \
        fprintf(stderr,                                                   \
                "\n"                                                      \
                "In function:\n" COLOR_BEIGE                              \
                "%s\n\n" COLOR_RESET                                      \
                "You can find it at:\n" COLOR_BEIGE LINE_INFO COLOR_RESET \
                "\n",                                                     \
                func_name);                                               \
        exit(-1);                                                         \
    } while (0u);

static inline std::string get_last_error_str()
{
    DWORD error_code = GetLastError();
    LPSTR lpMsgBuf = nullptr;
    DWORD cnt = FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
            FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        error_code,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL),
        (LPSTR)&lpMsgBuf,
        0,
        NULL);

    if (cnt == 0 || lpMsgBuf == nullptr)
        return "[SYSTEM] Unknown error, message format failed.";

    std::string error_msg = lpMsgBuf;
    LocalFree((void *)lpMsgBuf);
    return "[SYSTEM] " + error_msg;
}

#define error_system_api_exit(...)                      \
    do                                                  \
    {                                                   \
        auto message = get_last_error_str();            \
        const char *func_name = __PRETTY_FUNCTION__;    \
        fprintf(stderr,                                 \
                "[ERROR INFO]\n" COLOR_RED              \
                "%s\n" COLOR_RESET,                     \
                message.data());                        \
        fprintf(stderr,                                 \
                "Message:\n" COLOR_YELLOW __VA_ARGS__); \
        fprintf(stderr,                                 \
                "\n" COLOR_RESET                        \
                "In function:\n" COLOR_BEIGE            \
                "%s\n\n" COLOR_RESET                    \
                "You can find it at:\n" COLOR_BEIGE     \
                    LINE_INFO COLOR_RESET               \
                "\n",                                   \
                func_name);                             \
        exit(-1);                                       \
    } while (0u)

#endif // !__HOST_ERROR_HPP__
