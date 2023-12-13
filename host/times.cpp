/**
 * @file times.cpp
 * @author simakeng (simakeng@outlook.com)
 * @brief
 * @version 0.1
 * @date 2023-12-13
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <timer_api.h>
#include <thread>
#include <chrono>

using namespace std;

extern "C"
{
    void sys_delay_ms(uint32_t ms)
    {
        this_thread::sleep_for(chrono::milliseconds(ms));
    }

    void sys_delay_us(uint32_t us)
    {
        this_thread::sleep_for(chrono::microseconds(us));
    }

    uint32_t sys_get_tick_ms()
    {
        return (uint32_t)sys_get_tick_ms_u64();
    }
    uint64_t sys_get_tick_ms_u64()
    {
        auto now = chrono::system_clock::now();
        auto from_epoch = now.time_since_epoch();
        auto from_epoch_in_ms = chrono::duration_cast<chrono::milliseconds>(from_epoch);
        auto ticks = from_epoch_in_ms.count();

        return (uint64_t)ticks;
    }

    uint64_t sys_get_tick_us()
    {
        auto now = chrono::system_clock::now();
        auto from_epoch = now.time_since_epoch();
        auto from_epoch_in_us = chrono::duration_cast<chrono::microseconds>(from_epoch);
        auto ticks = from_epoch_in_us.count();

        return (uint64_t)ticks;
    }

    uint32_t sys_get_duration_ms(uint32_t from);
    uint64_t sys_get_duration_ms_u64(uint64_t from);
    uint64_t sys_get_duration_us(uint64_t from);
}