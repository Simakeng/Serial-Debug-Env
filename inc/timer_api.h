/**
 * @file timer_api.h
 * @author simakeng (simakeng@outlook.com)
 * @brief
 * @version 0.1
 * @date 2023-12-13
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <stdint.h>

#ifndef __TIMER_API_H__
#define __TIMER_API_H__

#ifdef __cplusplus
extern "C"
{
#endif

    void sys_delay_ms(uint32_t ms);
    void sys_delay_us(uint32_t us);

    uint32_t sys_get_tick_ms();
    uint64_t sys_get_tick_ms_u64();

    uint64_t sys_get_tick_us();

    uint32_t sys_get_duration_ms(uint32_t from);
    uint64_t sys_get_duration_ms_u64(uint64_t from);
    uint64_t sys_get_duration_us(uint64_t from);

#ifdef __cplusplus
}
#endif

#endif // ! #ifndef __TIMER_API_H__