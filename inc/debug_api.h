/**
 * @file debug_api.h
 * @author simakeng (simakeng@outlook.com)
 * @brief
 * @version 0.1
 * @date 2023-12-13
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __DEBUG_API_H__
#define __DEBUG_API_H__

#ifdef __cplusplus
extern "C"
{ 
#endif

    void debug_print(const char *fmt, ...);
    void debug_putc(int ch);

#ifdef __cplusplus
}
#endif

#endif // ! #ifndef __DEBUG_API_H__
