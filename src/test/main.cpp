// //////////////////////////////////////////////////////////
//
// Crc32Test.cpp
// Copyright (c) 2016 Stephan Brumme. All rights reserved.
// see http://create.stephan-brumme.com/disclaimer.html
//
//////////////////////////////////////////////////////////

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <cstdlib>
#include <cstdio>

//#if defined(_MSC_VER) || defined(__CYGWIN__)
#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#else
#include <ctime>
#endif

#include <FastCRC32/Crc32.h>
#include "crc32/crc32c.h"

#ifndef __SSE4_2__
#define __SSE4_2__      1
#endif
#include <folly/hash/Checksum.h>

#ifndef __COMPILER_BARRIER
#if defined(_MSC_VER) || defined(__ICL) || defined(__INTEL_COMPILER)
#define __COMPILER_BARRIER()        _ReadWriteBarrier()
#else
#define __COMPILER_BARRIER()        asm volatile ("" : : : "memory")
#endif
#endif

/// one gigabyte
static const size_t kNumBytes = 1024 * 1024 * 1024;
/// 4k chunks during last test
static const size_t kDefaultChunkSize = 4 * 1024;

// //////////////////////////////////////////////////////////
// test code
//////////////////////////////////////////////////////////

// timing
static double clock_seconds()
{
    __COMPILER_BARRIER();

//#if defined(_MSC_VER) || defined(__CYGWIN__)
#if defined(_WIN32) || defined(_WIN64)
    LARGE_INTEGER frequency, now;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&now);
    return (now.QuadPart / double(frequency.QuadPart));
#else
    timespec now;
    clock_gettime(CLOCK_REALTIME, &now);
    return now.tv_sec + now.tv_nsec / 1000000000.0;
#endif

    __COMPILER_BARRIER();
}

int main(int argn, char ** argv)
{
    printf("Please wait ...\n\n");

    uint32_t randomNumber = 0x27121978U;

    // initialize
    char * data = new char[kNumBytes];
    if (data == nullptr) {
        printf("Error: Out of memory ...\n\n");
        return 0;
    }

    for (size_t i = 0; i < kNumBytes; i++) {
        data[i] = char(randomNumber & 0xFF);
        // simple LCG, see http://en.wikipedia.org/wiki/Linear_congruential_generator
        randomNumber = 1664525 * randomNumber + 1013904223;
    }

    // re-use variables
    double startTime, duration;
    uint32_t crc32;

    //
    // bitwise
    //
    startTime = clock_seconds();
    crc32 = crc32_bitwise(data, kNumBytes);
    duration = clock_seconds() - startTime;
    printf(" bitwise           : CRC32 = 0x%08X, %.3f sec(s), %.3f MB/s\n",
           crc32, duration, (kNumBytes / (1024 * 1024)) / duration);

    //
    // half-byte
    //
    startTime = clock_seconds();
    crc32 = crc32_halfbyte(data, kNumBytes);
    duration = clock_seconds() - startTime;
    printf(" half-byte         : CRC32 = 0x%08X, %.3f sec(s), %.3f MB/s\n",
           crc32, duration, (kNumBytes / (1024 * 1024)) / duration);

    //
    // one byte at once (without lookup tables)
    //
    startTime = clock_seconds();
    crc32 = crc32_1byte_tableless(data, kNumBytes);
    duration = clock_seconds() - startTime;
    printf(" tableless (byte)  : CRC32 = 0x%08X, %.3f sec(s), %.3f MB/s\n",
           crc32, duration, (kNumBytes / (1024 * 1024)) / duration);

    //
    // one byte at once (without lookup tables)
    //
    startTime = clock_seconds();
    crc32 = crc32_1byte_tableless2(data, kNumBytes);
    duration = clock_seconds() - startTime;
    printf(" tableless (byte2) : CRC32 = 0x%08X, %.3f sec(s), %.3f MB/s\n",
           crc32, duration, (kNumBytes / (1024 * 1024)) / duration);

#ifdef CRC32_USE_LOOKUP_TABLE_BYTE
    //
    // one byte at once
    //
    startTime = clock_seconds();
    crc32 = crc32_1byte(data, kNumBytes);
    duration = clock_seconds() - startTime;
    printf("  1 byte  at once  : CRC32 = 0x%08X, %.3f sec(s), %.3f MB/s\n",
           crc32, duration, (kNumBytes / (1024 * 1024)) / duration);
#endif

#ifdef CRC32_USE_LOOKUP_TABLE_SLICING_BY_4
    //
    // four bytes at once
    //
    startTime = clock_seconds();
    crc32 = crc32_4bytes(data, kNumBytes);
    duration = clock_seconds() - startTime;
    printf("  4 bytes at once  : CRC32 = 0x%08X, %.3f sec(s), %.3f MB/s\n",
           crc32, duration, (kNumBytes / (1024 * 1024)) / duration);
#endif // CRC32_USE_LOOKUP_TABLE_SLICING_BY_4

#ifdef CRC32_USE_LOOKUP_TABLE_SLICING_BY_8
    //
    // eight bytes at once
    //
    startTime = clock_seconds();
    crc32 = crc32_8bytes(data, kNumBytes);
    duration = clock_seconds() - startTime;
    printf("  8 bytes at once  : CRC32 = 0x%08X, %.3f sec(s), %.3f MB/s\n",
           crc32, duration, (kNumBytes / (1024 * 1024)) / duration);

    //
    // eight bytes at once, unrolled 4 times (=> 32 bytes per loop)
    //
    startTime = clock_seconds();
    crc32 = crc32_4x8bytes(data, kNumBytes);
    duration = clock_seconds() - startTime;
    printf(" 4x8 bytes at once : CRC32 = 0x%08X, %.3f sec(s), %.3f MB/s\n",
           crc32, duration, (kNumBytes / (1024 * 1024)) / duration);
#endif // CRC32_USE_LOOKUP_TABLE_SLICING_BY_8

#ifdef CRC32_USE_LOOKUP_TABLE_SLICING_BY_16
    //
    // sixteen bytes at once
    //
    startTime = clock_seconds();
    crc32 = crc32_16bytes(data, kNumBytes);
    duration = clock_seconds() - startTime;
    printf(" 16 bytes at once  : CRC32 = 0x%08X, %.3f sec(s), %.3f MB/s\n",
           crc32, duration, (kNumBytes / (1024 * 1024)) / duration);

    //
    // sixteen bytes at once
    //
    startTime = clock_seconds();
    crc32 = crc32_16bytes_prefetch(data, kNumBytes, 0, 256);
    duration = clock_seconds() - startTime;
    printf(" 16 bytes at once  : CRC32 = 0x%08X, %.3f sec(s), %.3f MB/s (including prefetching)\n",
           crc32, duration, (kNumBytes / (1024 * 1024)) / duration);
#endif // CRC32_USE_LOOKUP_TABLE_SLICING_BY_16

    //
    // process in 4k chunks
    //
    startTime = clock_seconds();
    crc32 = 0; // also default parameter of crc32_xx functions
    size_t bytesProcessed = 0;
    while (bytesProcessed < kNumBytes) {
        size_t bytesLeft = kNumBytes - bytesProcessed;
        size_t chunkSize = (kDefaultChunkSize < bytesLeft) ? kDefaultChunkSize : bytesLeft;

        crc32 = crc32_fast(data + bytesProcessed, chunkSize, crc32);

        bytesProcessed += chunkSize;
    }
    duration = clock_seconds() - startTime;
    printf("    chunked        : CRC32 = 0x%08X, %.3f sec(s), %.3f MB/s\n",
           crc32, duration, (kNumBytes / (1024 * 1024)) / duration);

#if __SSE4_2__

    //
    // jimi::crc32c_hw_u32()
    //
    startTime = clock_seconds();
    crc32 = jimi::crc32c_hw_u32(data, kNumBytes);
    duration = clock_seconds() - startTime;
    printf(" crc32c_hw_u32()   : CRC32 = 0x%08X, %.3f sec(s), %.3f MB/s\n",
           crc32, duration, (kNumBytes / (1024 * 1024)) / duration);

#if CRC32_IS_X86_64
    //
    // jimi::crc32c_hw_u64_v2()
    //
    startTime = clock_seconds();
    crc32 = jimi::crc32c_hw_u64_v2(data, kNumBytes);
    duration = clock_seconds() - startTime;
    printf(" crc32c_hw_u64()   : CRC32 = 0x%08X, %.3f sec(s), %.3f MB/s\n",
           crc32, duration, (kNumBytes / (1024 * 1024)) / duration);
#endif // CRC32_IS_X86_64

#endif // __SSE4_2__

    //
    // folly::crc32()
    //
    startTime = clock_seconds();
    crc32 = folly::crc32((const uint8_t *)data, kNumBytes);
    duration = clock_seconds() - startTime;
    printf(" folly::crc32()    : CRC32 = 0x%08X, %.3f sec(s), %.3f MB/s\n",
           crc32, duration, (kNumBytes / (1024 * 1024)) / duration);

    //
    // folly::crc32c()
    //
    startTime = clock_seconds();
    crc32 = folly::crc32c((const uint8_t *)data, kNumBytes);
    duration = clock_seconds() - startTime;
    printf(" folly::crc32c()   : CRC32 = 0x%08X, %.3f sec(s), %.3f MB/s\n",
           crc32, duration, (kNumBytes / (1024 * 1024)) / duration);

    printf("\n");

    if (data != nullptr)
        delete[] data;

#if (defined(_WIN32) || defined(WIN32) || defined(OS_WINDOWS) || defined(_WINDOWS_))
    ::system("pause");
#endif
    return 0;
}
