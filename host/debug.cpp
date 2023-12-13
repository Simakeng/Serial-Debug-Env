/**
 * @file debug.cpp
 * @author simakeng (simakeng@outlook.com)
 * @brief
 * @version 0.1
 * @date 2023-12-13
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "host_error.hpp"

#include <stdio.h>
#include <stddef.h>
#include <stdarg.h>
#include <debug_api.h>

extern "C" void debug_print(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
}

extern "C" void debug_putc(int ch)
{
    fputc(ch, stderr);
}
