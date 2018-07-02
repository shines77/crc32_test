
#ifndef JIMI_CRC32_H
#define JIMI_CRC32_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include "jimi/basic/stddef.h"
#include "jimi/basic/stdint.h"
#include "jimi/basic/stdsize.h"

#include <assert.h>

#ifndef __SSE4_2__
#define __SSE4_2__      1
#endif

#ifdef __SSE4_2__
#ifdef _MSC_VER
#include <nmmintrin.h>  // For SSE 4.2
#else
#include <x86intrin.h>
//#include <nmmintrin.h>  // For SSE 4.2
#endif // _MSC_VER
#endif // __SSE4_2__

#if defined(WIN64) || defined(_WIN64) || defined(_M_X64) || defined(_M_AMD64) \
 || defined(__amd64__) || defined(__x86_64__)
#ifndef CRC32_IS_X86_64
#define CRC32_IS_X86_64     1
#endif
#endif // _WIN64 || __amd64__

namespace jimi {

#if __SSE4_2__

static uint32_t crc32c_hw_x86(const char * data, size_t length)
{
    assert(data != nullptr);

    static const uint32_t kStepLen = sizeof(uint32_t);
    static const uint32_t kOneMask = 0xFFFFFFFFUL;

    uint32_t crc32 = 0;

    uint32_t data32;
    const char * data_end = data + length;
    data32 = *(uint32_t *)(data);
    data += kStepLen;

    do {
        if (likely(data <= data_end)) {
            crc32 = _mm_crc32_u32(crc32, data32);
            data += kStepLen;
            data32 = *(uint32_t *)(data);
        }
        else {
            assert(data > data_end);
            uint32_t rest = (uint32_t)(kStepLen - (data - data_end));
            assert(rest > 0 && rest < kStepLen);
            uint32_t mask = kOneMask >> (rest * 8U);
            data32 &= mask;
            crc32 = _mm_crc32_u32(crc32, data32);
            break;
        }
    } while (1);

    return crc32;
}

#if CRC32_IS_X86_64

static uint64_t crc32c_hw_x64(const char * data, size_t length)
{
    assert(data != nullptr);

    static const size_t kStepLen = sizeof(uint64_t);
    static const uint64_t kOneMask = 0xFFFFFFFFFFFFFFFFULL;

    uint64_t crc64 = 0;

    uint64_t data64;
    const char * data_end = data + length;
    data64 = *(uint64_t *)(data);
    data += kStepLen;

    do {
        if (likely(data <= data_end)) {
            crc64 = _mm_crc32_u64(crc64, data64);
            data += kStepLen;
            data64 = *(uint64_t *)(data);
        }
        else {
            assert(data > data_end);
            size_t rest = (size_t)(kStepLen - (data - data_end));
            assert(rest > 0 && rest < kStepLen);
            uint64_t mask = kOneMask >> (rest * 8U);
            data64 &= mask;
            crc64 = _mm_crc32_u64(crc64, data64);
            break;
        }
    } while (1);

    return crc64;
}

#endif // CRC32_IS_X86_64

static uint32_t crc32c_hw_u32(const char * data, size_t length)
{
    assert(data != nullptr);
    uint32_t crc32 = 0;

    static const size_t kStepLen = sizeof(uint32_t);
    uint32_t * src = (uint32_t *)data;
    uint32_t * src_end = src + (length / kStepLen);

    while (likely(src < src_end)) {
        crc32 = _mm_crc32_u32(crc32, *src);
        ++src;
    }

    unsigned char * src8 = (unsigned char *)src;
    unsigned char * src8_end = (unsigned char *)(data + length);

    while (likely(src8 < src8_end)) {
        crc32 = _mm_crc32_u8(crc32, *src8);
        ++src8;
    }
    return crc32;
}

#if CRC32_IS_X86_64

static uint32_t crc32c_hw_u64(const char * data, size_t length)
{
    assert(data != nullptr);
    uint64_t crc64 = 0;

    static const size_t kStepLen = sizeof(uint64_t);
    uint64_t * src = (uint64_t *)data;
    uint64_t * src_end = src + (length / kStepLen);

    while (likely(src < src_end)) {
        crc64 = _mm_crc32_u64(crc64, *src);
        ++src;
    }

    uint32_t crc32 = (uint32_t)crc64;

    size_t i = length / kStepLen * kStepLen;
    while (likely(i < length)) {
        crc32 = _mm_crc32_u8(crc32, data[i]);
        ++i;
    }
    return crc32;
}

#endif // CRC32_IS_X86_64

#if CRC32_IS_X86_64

static uint32_t crc32c_hw_u64_v2(const char * data, size_t length)
{
    assert(data != nullptr);
    uint64_t crc64 = 0;

    static const size_t kStepLen = sizeof(uint64_t);
    uint64_t * src = (uint64_t *)data;
    uint64_t * src_end = src + (length / kStepLen);

    while (likely(src < src_end)) {
        crc64 = _mm_crc32_u64(crc64, *src);
        ++src;
    }

    uint32_t crc32 = (uint32_t)crc64;
    unsigned char * src8 = (unsigned char *)src;
    unsigned char * src8_end = (unsigned char *)(data + length);

    while (likely(src8 < src8_end)) {
        crc32 = _mm_crc32_u8(crc32, *src8);
        ++src8;
    }
    return crc32;
}

#endif // CRC32_IS_X86_64

#endif // __SSE4_2__

} // namespace jimi

#endif // JIMI_CRC32_H