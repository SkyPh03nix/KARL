// MemoryUsage.h
#pragma once

//file for debugging functions

#ifdef _WIN32
#include <windows.h>
#include <psapi.h>

inline float getMemoryUsageInMB() {
    PROCESS_MEMORY_COUNTERS memInfo;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &memInfo, sizeof(memInfo))) {
        return static_cast<float>(memInfo.WorkingSetSize) / (1024 * 1024);
    }
    return -1.0f; // Fehler
}
#endif