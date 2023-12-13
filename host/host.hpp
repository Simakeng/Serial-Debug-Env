/**
 * @file host.hpp
 * @author simakeng (simakeng@outlook.com)
 * @brief 
 * @version 0.1
 * @date 2023-12-13
 * 
 * @copyright Copyright (c) 2023
 * 
 */



#ifndef __HOST_HPP__
#define __HOST_HPP__

namespace Serial
{
    /**
     * @brief prepare the host environment
     */
    void HostInit();

    /**
     * @brief clean up
     */
    void HostExit();
} // namespace Serial





#endif // ! #ifndef __HOST_HPP__