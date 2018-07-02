
# crc32_test

## [English]

A test routine for fast `crc32` and `crc32c` algorithm.

## [Chinese/中文]

一个关于 `crc32` 和 `crc32c` 算法的测试程序。

## Reference (引用)

`Fast CRC32`: [http://create.stephan-brumme.com/crc32/]()

* `/FastCRC32/Crc32.h`
* `/FastCRC32/Crc32.cpp`

`Folly`: [https://github.com/facebook/folly]()

* `/folly/hash/Checksum.h`

## Dependent library (依赖库)

`Boost library`:

* Boost.crc
* Boost.preprocessor

## Benchmark (性能测试)

* `Windows 10 Professional Build 1709` - 64 bit
* `AMD Ryzen 1700X` @ `3.40GHz`
* `Vc 2015 update 3`

```shell
 bitwise           : CRC32 = 0x221F390F, 10.846 sec(s), 94.411 MB/s
 half-byte         : CRC32 = 0x221F390F, 5.145 sec(s), 199.027 MB/s
 tableless (byte)  : CRC32 = 0x221F390F, 4.861 sec(s), 210.662 MB/s
 tableless (byte2) : CRC32 = 0x221F390F, 4.177 sec(s), 245.139 MB/s
  1 byte  at once  : CRC32 = 0x221F390F, 2.481 sec(s), 412.691 MB/s
  4 bytes at once  : CRC32 = 0x221F390F, 0.918 sec(s), 1115.472 MB/s
  8 bytes at once  : CRC32 = 0x221F390F, 0.615 sec(s), 1664.642 MB/s
 4x8 bytes at once : CRC32 = 0x221F390F, 0.608 sec(s), 1685.355 MB/s
 16 bytes at once  : CRC32 = 0x221F390F, 0.457 sec(s), 2241.662 MB/s
 16 bytes at once  : CRC32 = 0x221F390F, 0.465 sec(s), 2203.015 MB/s (including prefetching)
    chunked        : CRC32 = 0x221F390F, 0.463 sec(s), 2211.929 MB/s

 folly::crc32()    : CRC32 = 0x221F390F, 0.096 sec(s), 10611.453 MB/s

 crc32c_hw_u32()   : CRC32 = 0xB85FB04A, 0.325 sec(s), 3153.989 MB/s
 crc32c_hw_u64()   : CRC32 = 0xB85FB04A, 0.175 sec(s), 5865.083 MB/s
 folly::crc32c()   : CRC32 = 0xB85FB04A, 0.198 sec(s), 5182.397 MB/s

 crc32c_hw_x86()   : CRC32 = 0x193A2B0B, 0.316 sec(s), 3238.855 MB/s
 crc32c_hw_x64()   : CRC32 = 0x24DAC648, 0.170 sec(s), 6017.945 MB/s
```

* `Ubuntu 14.04 Server` - 64 bit
* `Intel Dual E5-2680 v3` @ `2.00GHz`
*  `g++ 5.5.0`

```shell
 bitwise           : CRC32 = 0x221F390F, 15.335 sec(s), 66.776 MB/s
 half-byte         : CRC32 = 0x221F390F, 8.173 sec(s), 125.295 MB/s
 tableless (byte)  : CRC32 = 0x221F390F, 6.821 sec(s), 150.125 MB/s
 tableless (byte2) : CRC32 = 0x221F390F, 7.567 sec(s), 135.328 MB/s
  1 byte  at once  : CRC32 = 0x221F390F, 3.647 sec(s), 280.776 MB/s
  4 bytes at once  : CRC32 = 0x221F390F, 1.501 sec(s), 682.185 MB/s
  8 bytes at once  : CRC32 = 0x221F390F, 0.962 sec(s), 1064.351 MB/s
 4x8 bytes at once : CRC32 = 0x221F390F, 0.661 sec(s), 1548.828 MB/s
 16 bytes at once  : CRC32 = 0x221F390F, 0.471 sec(s), 2173.914 MB/s
 16 bytes at once  : CRC32 = 0x221F390F, 0.510 sec(s), 2006.052 MB/s (including prefetching)
    chunked        : CRC32 = 0x221F390F, 0.434 sec(s), 2358.953 MB/s

 folly::crc32()    : CRC32 = 0x221F390F, 0.196 sec(s), 5212.813 MB/s

 crc32c_hw_u32()   : CRC32 = 0xB85FB04A, 0.365 sec(s), 2809.149 MB/s
 crc32c_hw_u64()   : CRC32 = 0xB85FB04A, 0.205 sec(s), 5005.212 MB/s
 folly::crc32c()   : CRC32 = 0xB85FB04A, 0.184 sec(s), 5568.926 MB/s

 crc32c_hw_x86()   : CRC32 = 0x193A2B0B, 0.368 sec(s), 2779.222 MB/s
 crc32c_hw_x64()   : CRC32 = 0x24DAC648, 0.214 sec(s), 4781.217 MB/s
```
