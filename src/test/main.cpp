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

#ifndef __SSE4_2__
#define __SSE4_2__  1
#endif

#include <FastCRC32/Crc32.h>
#include "crc32/crc32c.h"
#include <folly/hash/Checksum.h>

#ifndef __COMPILER_BARRIER
#if defined(_MSC_VER) || defined(__ICL) || defined(__INTEL_COMPILER)
#if defined(_M_AMD64)
#pragma intrinsic(_ReadWriteBarrier)
#define __COMPILER_BARRIER()        _ReadWriteBarrier()
#else
#define __COMPILER_BARRIER()        (void)(0)
#endif
#else
#define __COMPILER_BARRIER()        asm volatile ("" : : : "memory")
#endif
#endif

/// one gigabyte
#ifdef NDEBUG
static const size_t kTotalBytes = 1024 * 1024 * 1024;
#else
static const size_t kTotalBytes = 128 * 1024 * 1024;
#endif

static const size_t kStepBytes = 512 * 1024;

/// 4k chunks during last test
static const size_t kDefaultChunkSize = 4 * 1024;

//////////////////////////////////////////////////////////
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

void benchmark_crc32_bitwise(char * data, size_t totalBytes)
{
    double startTime, duration, throughput;
    size_t iterations;
    uint32_t crc32, crc32_sum;

    iterations = (totalBytes + kStepBytes - 1) / kStepBytes;
    crc32_sum = 0;

    startTime = clock_seconds();
    for (size_t i = 0; i < iterations; ++i) {
        crc32 = crc32_bitwise(data + i, kStepBytes);
        crc32_sum += crc32;
    }
    duration = clock_seconds() - startTime;

    throughput = ((double)totalBytes / (1024 * 1024)) / duration;
    printf(" bitwise                  : CRC32 = 0x%08X, SUM = 0x%08X, %.3f sec(s), %.3f MB/s\n",
           crc32, crc32_sum, duration, throughput);
}

void benchmark_crc32_halfbyte(char * data, size_t totalBytes)
{
    double startTime, duration, throughput;
    size_t iterations;
    uint32_t crc32, crc32_sum;

    iterations = (totalBytes + kStepBytes - 1) / kStepBytes;
    crc32_sum = 0;

    startTime = clock_seconds();
    for (size_t i = 0; i < iterations; ++i) {
        crc32 = crc32_halfbyte(data + i, kStepBytes);
        crc32_sum += crc32;
    }
    duration = clock_seconds() - startTime;

    throughput = ((double)totalBytes / (1024 * 1024)) / duration;
    printf(" half-byte                : CRC32 = 0x%08X, SUM = 0x%08X, %.3f sec(s), %.3f MB/s\n",
           crc32, crc32_sum, duration, throughput);
}

//
// one byte at once (without lookup tables)
//
void benchmark_crc32_1byte_tableless(char * data, size_t totalBytes)
{
    double startTime, duration, throughput;
    size_t iterations;
    uint32_t crc32, crc32_sum;

    iterations = (totalBytes + kStepBytes - 1) / kStepBytes;
    crc32_sum = 0;

    startTime = clock_seconds();
    for (size_t i = 0; i < iterations; ++i) {
        crc32 = crc32_1byte_tableless(data + i, kStepBytes);
        crc32_sum += crc32;
    }
    duration = clock_seconds() - startTime;

    throughput = ((double)totalBytes / (1024 * 1024)) / duration;
    printf(" tableless (byte)         : CRC32 = 0x%08X, SUM = 0x%08X, %.3f sec(s), %.3f MB/s\n",
           crc32, crc32_sum, duration, throughput);
}

//
// one byte at once #2 (without lookup tables)
//
void benchmark_crc32_1byte_tableless2(char * data, size_t totalBytes)
{
    double startTime, duration, throughput;
    size_t iterations;
    uint32_t crc32, crc32_sum;

    iterations = (totalBytes + kStepBytes - 1) / kStepBytes;
    crc32_sum = 0;

    startTime = clock_seconds();
    for (size_t i = 0; i < iterations; ++i) {
        crc32 = crc32_1byte_tableless2(data + i, kStepBytes);
        crc32_sum += crc32;
    }
    duration = clock_seconds() - startTime;

    throughput = ((double)totalBytes / (1024 * 1024)) / duration;
    printf(" tableless2 (byte)        : CRC32 = 0x%08X, SUM = 0x%08X, %.3f sec(s), %.3f MB/s\n",
           crc32, crc32_sum, duration, throughput);
}

//
// one byte at once
//
void benchmark_crc32_1byte(char * data, size_t totalBytes)
{
#ifdef CRC32_USE_LOOKUP_TABLE_BYTE
    double startTime, duration, throughput;
    size_t iterations;
    uint32_t crc32, crc32_sum;

    iterations = (totalBytes + kStepBytes - 1) / kStepBytes;
    crc32_sum = 0;

    startTime = clock_seconds();
    for (size_t i = 0; i < iterations; ++i) {
        crc32 = crc32_1byte(data + i, kStepBytes);
        crc32_sum += crc32;
    }
    duration = clock_seconds() - startTime;

    throughput = ((double)totalBytes / (1024 * 1024)) / duration;
    printf("  1 byte  at once         : CRC32 = 0x%08X, SUM = 0x%08X, %.3f sec(s), %.3f MB/s\n",
           crc32, crc32_sum, duration, throughput);
#endif
}

//
// four bytes at once
//
void benchmark_crc32_4byte(char * data, size_t totalBytes)
{
#ifdef CRC32_USE_LOOKUP_TABLE_SLICING_BY_4
    double startTime, duration, throughput;
    size_t iterations;
    uint32_t crc32, crc32_sum;

    iterations = (totalBytes + kStepBytes - 1) / kStepBytes;
    crc32_sum = 0;

    startTime = clock_seconds();
    for (size_t i = 0; i < iterations; ++i) {
        crc32 = crc32_4bytes(data + i, kStepBytes);
        crc32_sum += crc32;
    }
    duration = clock_seconds() - startTime;

    throughput = ((double)totalBytes / (1024 * 1024)) / duration;
    printf("  4 bytes at once         : CRC32 = 0x%08X, SUM = 0x%08X, %.3f sec(s), %.3f MB/s\n",
           crc32, crc32_sum, duration, throughput);
#endif
}

//
// eight bytes at once
//
void benchmark_crc32_8byte(char * data, size_t totalBytes)
{
#ifdef CRC32_USE_LOOKUP_TABLE_SLICING_BY_8
    double startTime, duration, throughput;
    size_t iterations;
    uint32_t crc32, crc32_sum;

    iterations = (totalBytes + kStepBytes - 1) / kStepBytes;
    crc32_sum = 0;

    startTime = clock_seconds();
    for (size_t i = 0; i < iterations; ++i) {
        crc32 = crc32_8bytes(data + i, kStepBytes);
        crc32_sum += crc32;
    }
    duration = clock_seconds() - startTime;

    throughput = ((double)totalBytes / (1024 * 1024)) / duration;
    printf("  8 bytes at once         : CRC32 = 0x%08X, SUM = 0x%08X, %.3f sec(s), %.3f MB/s\n",
           crc32, crc32_sum, duration, throughput);
#endif
}

//
// eight bytes at once, unrolled 4 times (=> 32 bytes per loop)
//
void benchmark_crc32_4x8byte(char * data, size_t totalBytes)
{
#ifdef CRC32_USE_LOOKUP_TABLE_SLICING_BY_8
    double startTime, duration, throughput;
    size_t iterations;
    uint32_t crc32, crc32_sum;

    iterations = (totalBytes + kStepBytes - 1) / kStepBytes;
    crc32_sum = 0;

    startTime = clock_seconds();
    for (size_t i = 0; i < iterations; ++i) {
        crc32 = crc32_4x8bytes(data + i, kStepBytes);
        crc32_sum += crc32;
    }
    duration = clock_seconds() - startTime;

    throughput = ((double)totalBytes / (1024 * 1024)) / duration;
    printf(" 4x8 bytes at once        : CRC32 = 0x%08X, SUM = 0x%08X, %.3f sec(s), %.3f MB/s\n",
           crc32, crc32_sum, duration, throughput);
#endif
}

//
// sixteen bytes at once
//
void benchmark_crc32_16byte(char * data, size_t totalBytes)
{
#ifdef CRC32_USE_LOOKUP_TABLE_SLICING_BY_16
    double startTime, duration, throughput;
    size_t iterations;
    uint32_t crc32, crc32_sum;

    iterations = (totalBytes + kStepBytes - 1) / kStepBytes;
    crc32_sum = 0;

    startTime = clock_seconds();
    for (size_t i = 0; i < iterations; ++i) {
        crc32 = crc32_16bytes(data + i, kStepBytes);
        crc32_sum += crc32;
    }
    duration = clock_seconds() - startTime;

    throughput = ((double)totalBytes / (1024 * 1024)) / duration;
    printf(" 16 bytes at once         : CRC32 = 0x%08X, SUM = 0x%08X, %.3f sec(s), %.3f MB/s\n",
           crc32, crc32_sum, duration, throughput);
#endif
}

//
// sixteen bytes at once (prefetch)
//
void benchmark_crc32_16byte_prefetch(char * data, size_t totalBytes)
{
#ifdef CRC32_USE_LOOKUP_TABLE_SLICING_BY_16
    double startTime, duration, throughput;
    size_t iterations;
    uint32_t crc32, crc32_sum;

    iterations = (totalBytes + kStepBytes - 1) / kStepBytes;
    crc32_sum = 0;

    startTime = clock_seconds();
    for (size_t i = 0; i < iterations; ++i) {
        crc32 = crc32_16bytes_prefetch(data + i, kStepBytes, 0, 256);
        crc32_sum += crc32;
    }
    duration = clock_seconds() - startTime;

    throughput = ((double)totalBytes / (1024 * 1024)) / duration;
    printf(" 16 bytes at once         : CRC32 = 0x%08X, SUM = 0x%08X, %.3f sec(s), %.3f MB/s (including prefetching)\n",
           crc32, crc32_sum, duration, throughput);
#endif
}

//
// process in 4k chunks
//
void benchmark_crc32_fast(char * data, size_t totalBytes)
{
    double startTime, duration, throughput;
    size_t iterations;
    uint32_t crc32, crc32_sum;

    iterations = (totalBytes + kStepBytes - 1) / kStepBytes;
    crc32_sum = 0;

    startTime = clock_seconds();
    for (size_t i = 0; i < iterations; ++i) {
        crc32 = 0; // also default parameter of crc32_xx functions
        size_t bytesProcessed = 0;
        char * data_start = data + i;
        while (bytesProcessed < kStepBytes) {
            size_t bytesLeft = kStepBytes - bytesProcessed;
            size_t chunkSize = (kDefaultChunkSize < bytesLeft) ? kDefaultChunkSize : bytesLeft;

            crc32 = crc32_fast((const void *)(data_start + bytesProcessed), chunkSize, crc32);

            bytesProcessed += chunkSize;
        }
        crc32_sum += crc32;
    }
    duration = clock_seconds() - startTime;

    throughput = ((double)totalBytes / (1024 * 1024)) / duration;
    printf("    chunked               : CRC32 = 0x%08X, SUM = 0x%08X, %.3f sec(s), %.3f MB/s\n",
           crc32, crc32_sum, duration, throughput);
}

//
// folly::crc32()
//
void benchmark_folly_crc32(char * data, size_t totalBytes)
{
    double startTime, duration, throughput;
    size_t iterations;
    uint32_t crc32, crc32_sum;

    iterations = (totalBytes + kStepBytes - 1) / kStepBytes;
    crc32_sum = 0;

    startTime = clock_seconds();
    for (size_t i = 0; i < iterations; ++i) {
        crc32 = folly::crc32((const uint8_t *)(data + i), kStepBytes);
        crc32_sum += crc32;
    }
    duration = clock_seconds() - startTime;

    throughput = ((double)totalBytes / (1024 * 1024)) / duration;
    printf(" folly::crc32()           : CRC32 = 0x%08X, SUM = 0x%08X, %.3f sec(s), %.3f MB/s\n",
           crc32, crc32_sum, duration, throughput);
}

//
// jimi::crc32c_hw_u32()
//
void benchmark_jimi_crc32c_hw_u32(char * data, size_t totalBytes)
{
    double startTime, duration, throughput;
    size_t iterations;
    uint32_t crc32, crc32_sum;

    iterations = (totalBytes + kStepBytes - 1) / kStepBytes;
    crc32_sum = 0;

    startTime = clock_seconds();
    for (size_t i = 0; i < iterations; ++i) {
        crc32 = jimi::crc32c_hw_u32(data + i, kStepBytes);
        crc32_sum += crc32;
    }
    duration = clock_seconds() - startTime;

    throughput = ((double)totalBytes / (1024 * 1024)) / duration;
    printf(" crc32c_hw_u32()          : CRC32 = 0x%08X, SUM = 0x%08X, %.3f sec(s), %.3f MB/s\n",
           crc32, crc32_sum, duration, throughput);
}

//
// jimi::crc32c_hw_u64()
//
void benchmark_jimi_crc32c_hw_u64(char * data, size_t totalBytes)
{
#if CRC32C_IS_X86_64
    double startTime, duration, throughput;
    size_t iterations;
    uint32_t crc32, crc32_sum;

    iterations = (totalBytes + kStepBytes - 1) / kStepBytes;
    crc32_sum = 0;

    startTime = clock_seconds();
    for (size_t i = 0; i < iterations; ++i) {
        crc32 = jimi::crc32c_hw_u64(data + i, kStepBytes);
        crc32_sum += crc32;
    }
    duration = clock_seconds() - startTime;

    throughput = ((double)totalBytes / (1024 * 1024)) / duration;
    printf(" crc32c_hw_u64()          : CRC32 = 0x%08X, SUM = 0x%08X, %.3f sec(s), %.3f MB/s\n",
           crc32, crc32_sum, duration, throughput);
#endif // CRC32C_IS_X86_64
}

//
// jimi::crc32c_hw_triplet_loop()
//
void benchmark_jimi_crc32c_hw_triplet_loop(char * data, size_t totalBytes)
{
    double startTime, duration, throughput;
    size_t iterations;
    uint32_t crc32, crc32_sum;

    iterations = (totalBytes + kStepBytes - 1) / kStepBytes;
    crc32_sum = 0;

    startTime = clock_seconds();
    for (size_t i = 0; i < iterations; ++i) {
        crc32 = jimi::crc32c_hw_triplet_loop(data + i, kStepBytes);
        crc32_sum += crc32;
    }
    duration = clock_seconds() - startTime;

    throughput = ((double)totalBytes / (1024 * 1024)) / duration;
    printf(" crc32c_hw_triplet_loop() : CRC32 = 0x%08X, SUM = 0x%08X, %.3f sec(s), %.3f MB/s\n",
           crc32, crc32_sum, duration, throughput);
}

//
// folly::crc32c()
//
void benchmark_folly_crc32c(char * data, size_t totalBytes)
{
    double startTime, duration, throughput;
    size_t iterations;
    uint32_t crc32, crc32_sum;

    iterations = (totalBytes + kStepBytes - 1) / kStepBytes;
    crc32_sum = 0;

    startTime = clock_seconds();
    for (size_t i = 0; i < iterations; ++i) {
        crc32 = folly::crc32c((const uint8_t *)(data + i), kStepBytes);
        crc32_sum += crc32;
    }
    duration = clock_seconds() - startTime;

    throughput = ((double)totalBytes / (1024 * 1024)) / duration;
    printf(" folly::crc32c()          : CRC32 = 0x%08X, SUM = 0x%08X, %.3f sec(s), %.3f MB/s\n",
           crc32, crc32_sum, duration, throughput);
}

//
// jimi::crc32c_hw_one_loop_x86()
//
void benchmark_jimi_crc32c_hw_one_loop_x86(char * data, size_t totalBytes)
{
    double startTime, duration, throughput;
    size_t iterations;
    uint32_t crc32, crc32_sum;

    iterations = (totalBytes + kStepBytes - 1) / kStepBytes;
    crc32_sum = 0;

    startTime = clock_seconds();
    for (size_t i = 0; i < iterations; ++i) {
        crc32 = jimi::crc32c_hw_one_loop_x86(data + i, kStepBytes);
        crc32_sum += crc32;
    }
    duration = clock_seconds() - startTime;

    throughput = ((double)totalBytes / (1024 * 1024)) / duration;
    printf(" crc32c_hw_one_loop_x86() : CRC32 = 0x%08X, SUM = 0x%08X, %.3f sec(s), %.3f MB/s\n",
           crc32, crc32_sum, duration, throughput);
}

//
// jimi::crc32c_hw_one_loop_x64()
//
void benchmark_jimi_crc32c_hw_one_loop_x64(char * data, size_t totalBytes)
{
#if CRC32C_IS_X86_64
    double startTime, duration, throughput;
    size_t iterations;
    uint32_t crc32, crc32_sum;

    iterations = (totalBytes + kStepBytes - 1) / kStepBytes;
    crc32_sum = 0;

    startTime = clock_seconds();
    for (size_t i = 0; i < iterations; ++i) {
        crc32 = jimi::crc32c_hw_one_loop_x64(data + i, kStepBytes);
        crc32_sum += crc32;
    }
    duration = clock_seconds() - startTime;

    throughput = ((double)totalBytes / (1024 * 1024)) / duration;
    printf(" crc32c_hw_one_loop_x64() : CRC32 = 0x%08X, SUM = 0x%08X, %.3f sec(s), %.3f MB/s\n",
           crc32, crc32_sum, duration, throughput);
#endif // CRC32C_IS_X86_64
}

int main(int argn, char ** argv)
{
    printf("Please wait ...\n\n");

    uint32_t randomNumber = 0x27121978U;

    if (folly::detail::crc32c_hw_supported()) {
        printf("CpuId: SSE 4.2 is supported.\n\n");
    }
    else {
        printf("CpuId: SSE 4.2 is not supported.\n\n");
    }

    // initialize
    static const size_t kAdditionalBytes = (kTotalBytes / kStepBytes) + 1;
    char * data = new char[kStepBytes + kAdditionalBytes];
    if (data == nullptr) {
        printf("Error: Out of memory ...\n\n");
        return 0;
    }

    for (size_t i = 0; i < (kStepBytes + kAdditionalBytes); i++) {
        data[i] = char(randomNumber & 0xFF);
        // simple LCG, see http://en.wikipedia.org/wiki/Linear_congruential_generator
        randomNumber = 1664525 * randomNumber + 1013904223;
    }

    //
    // bitwise
    //
    benchmark_crc32_bitwise(data, kTotalBytes / 16);

    //
    // half-byte
    //
    benchmark_crc32_halfbyte(data, kTotalBytes / 8);

    //
    // one byte at once (without lookup tables)
    //
    benchmark_crc32_1byte_tableless(data, kTotalBytes / 4);

    //
    // one byte at once #2 (without lookup tables)
    //
    benchmark_crc32_1byte_tableless2(data, kTotalBytes / 4);

    //
    // one byte at once
    //
    benchmark_crc32_1byte(data, kTotalBytes / 2);

    //
    // four bytes at once
    //
    benchmark_crc32_4byte(data, kTotalBytes);

    //
    // eight bytes at once
    //
    benchmark_crc32_8byte(data, kTotalBytes);

    //
    // eight bytes at once, unrolled 4 times (=> 32 bytes per loop)
    //
    benchmark_crc32_4x8byte(data, kTotalBytes);

    //
    // sixteen bytes at once
    //
    benchmark_crc32_16byte(data, kTotalBytes);

    //
    // sixteen bytes at once (prefetch)
    //
    benchmark_crc32_16byte_prefetch(data, kTotalBytes);

    //
    // process in 4k chunks
    //
    benchmark_crc32_fast(data, kTotalBytes);

    printf("\n");

    //
    // folly::crc32()
    //
    benchmark_folly_crc32(data, kTotalBytes);

    printf("\n");

#if __SSE4_2__

    //
    // jimi::crc32c_hw_u32()
    //
    benchmark_jimi_crc32c_hw_u32(data, kTotalBytes);

    //
    // jimi::crc32c_hw_u64()
    //
    benchmark_jimi_crc32c_hw_u64(data, kTotalBytes);

    //
    // jimi::crc32c_hw_one_loop_x86()
    //
    benchmark_jimi_crc32c_hw_one_loop_x86(data, kTotalBytes);

    //
    // jimi::crc32c_hw_one_loop_x64()
    //
    benchmark_jimi_crc32c_hw_one_loop_x64(data, kTotalBytes);

    printf("\n");

    //
    // jimi::crc32c_hw_triplet_loop()
    //
    benchmark_jimi_crc32c_hw_triplet_loop(data, kTotalBytes);

#endif // __SSE4_2__

    //
    // folly::crc32c()
    //
    benchmark_folly_crc32c(data, kTotalBytes);

    printf("\n");

    if (data != nullptr)
        delete[] data;

#if (defined(_WIN32) || defined(WIN32) || defined(OS_WINDOWS) || defined(_WINDOWS_))
    ::system("pause");
#endif
    return 0;
}
