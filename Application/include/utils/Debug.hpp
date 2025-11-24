#pragma once
#ifndef DEBUG_HPP
#define DEBUG_HPP

#include <string>
#include <cstdio>
#include <mutex>

//#define ENABLE_DEBUG
#define MAX_LOG_LEN 512

namespace Utils {
    extern std::mutex mutex;
    void write_log(const char *pszFmt, ...);
};

#endif
