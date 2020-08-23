
# crc32_test

## Introduction

A test routine for fast `crc32` and `crc32c` algorithm.

## 简介 [Chinese]

一个关于 `crc32` 和 `crc32c` 算法的测试程序。

## Reference (引用)

`Fast CRC32`: [http://create.stephan-brumme.com/crc32/](http://create.stephan-brumme.com/crc32/)

* `/FastCRC32/Crc32.h`
* `/FastCRC32/Crc32.cpp`

`Folly`: [https://github.com/facebook/folly](https://github.com/facebook/folly)

* `/folly/hash/Checksum.h`

## Reference Article (参考文章)

`zhihu.com`: [https://www.zhihu.com/question/280252145](https://www.zhihu.com/question/280252145)

## Dependent library (依赖库)

`Boost library`:

* boost.crc
* boost.preprocessor

## Benchmark (性能测试)

1. Windows

* `Windows 10 Professional Build 1709` - 64 bit
* `AMD Ryzen 1700X` @ `3.40GHz`, Boost on `3.8GHz` (Support `SSE 4.2`)
* `Vc 2015 update 3`

```bash
 bitwise                  : CRC32 = 0x0E225381, SUM = 0xFAB9C5C6, 0.669 sec(s), 95.711 MB/s
 half-byte                : CRC32 = 0x08F105AF, SUM = 0xCAC04360, 0.634 sec(s), 201.956 MB/s
 tableless (byte)         : CRC32 = 0x08F105AF, SUM = 0x958086C0, 1.194 sec(s), 214.338 MB/s
 tableless2 (byte)        : CRC32 = 0x08F105AF, SUM = 0x958086C0, 1.035 sec(s), 247.383 MB/s
  1 byte  at once         : CRC32 = 0x08F105AF, SUM = 0x2B010D80, 1.235 sec(s), 414.715 MB/s
  4 bytes at once         : CRC32 = 0x08F105AF, SUM = 0x56021B00, 0.908 sec(s), 1127.164 MB/s
  8 bytes at once         : CRC32 = 0x08F105AF, SUM = 0x56021B00, 0.605 sec(s), 1692.147 MB/s
 4x8 bytes at once        : CRC32 = 0x08F105AF, SUM = 0x56021B00, 0.611 sec(s), 1675.682 MB/s
 16 bytes at once         : CRC32 = 0x08F105AF, SUM = 0x56021B00, 0.459 sec(s), 2230.457 MB/s
 16 bytes at once         : CRC32 = 0x08F105AF, SUM = 0x56021B00, 0.458 sec(s), 2237.239 MB/s (including prefetching)
    chunked               : CRC32 = 0x08F105AF, SUM = 0x56021B00, 0.460 sec(s), 2224.228 MB/s

 folly::crc32()           : CRC32 = 0xDF220648, SUM = 0xDD955680, 0.083 sec(s), 12401.629 MB/s

 crc32c_hw_u32()          : CRC32 = 0x98387EBD, SUM = 0xFAE4F920, 0.308 sec(s), 3325.675 MB/s
 crc32c_hw_u64()          : CRC32 = 0x98387EBD, SUM = 0xFAE4F920, 0.154 sec(s), 6638.509 MB/s
 crc32c_hw_one_loop_x86() : CRC32 = 0x98387EBD, SUM = 0xFAE4F920, 0.311 sec(s), 3287.970 MB/s
 crc32c_hw_one_loop_x64() : CRC32 = 0x98387EBD, SUM = 0xFAE4F920, 0.154 sec(s), 6651.048 MB/s

 crc32c_hw_triplet_loop() : CRC32 = 0x98387EBD, SUM = 0xFAE4F920, 0.127 sec(s), 8036.289 MB/s
 folly::crc32c()          : CRC32 = 0x98387EBD, SUM = 0xFAE4F920, 0.132 sec(s), 7760.427 MB/s
```

```bash
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

1. Windows

* `Windows 10 Professional Build 1803` - 64 bit
* `Intel i5-4210M` @ `2.60GHz`, Boost on `3.2GHz` (Support `SSE 4.2`)
* `Vc 2015 update 3`

```bash
 bitwise           : CRC32 = 0x221F390F, 12.319 sec(s), 83.127 MB/s
 half-byte         : CRC32 = 0x221F390F, 5.766 sec(s), 177.605 MB/s
 tableless (byte)  : CRC32 = 0x221F390F, 4.574 sec(s), 223.851 MB/s
 tableless (byte2) : CRC32 = 0x221F390F, 5.891 sec(s), 173.824 MB/s
  1 byte  at once  : CRC32 = 0x221F390F, 2.523 sec(s), 405.815 MB/s
  4 bytes at once  : CRC32 = 0x221F390F, 1.071 sec(s), 955.966 MB/s
  8 bytes at once  : CRC32 = 0x221F390F, 0.766 sec(s), 1336.391 MB/s
 4x8 bytes at once : CRC32 = 0x221F390F, 0.743 sec(s), 1378.817 MB/s
 16 bytes at once  : CRC32 = 0x221F390F, 0.575 sec(s), 1780.454 MB/s
 16 bytes at once  : CRC32 = 0x221F390F, 0.585 sec(s), 1751.354 MB/s (including prefetching)
    chunked        : CRC32 = 0x221F390F, 0.584 sec(s), 1754.880 MB/s

 folly::crc32()    : CRC32 = 0x221F390F, 0.125 sec(s), 8182.784 MB/s

 crc32c_hw_u32()   : CRC32 = 0xB85FB04A, 0.283 sec(s), 3613.655 MB/s
 crc32c_hw_u64()   : CRC32 = 0xB85FB04A, 0.145 sec(s), 7057.738 MB/s
 folly::crc32c()   : CRC32 = 0xB85FB04A, 0.160 sec(s), 6391.166 MB/s

 crc32c_hw_x86()   : CRC32 = 0x193A2B0B, 0.279 sec(s), 3674.769 MB/s
 crc32c_hw_x64()   : CRC32 = 0x24DAC648, 0.146 sec(s), 7029.146 MB/s
```

3. Linux

* `Ubuntu 14.04 Server` - 64 bit
* `Dual Intel Xeon E5-2683 v3` @ `2.00GHz`, Boost on `3.0GHz` (Support `SSE 4.2`)
*  `g++ 5.5.0`

```bash
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

* `Ubuntu 16.04 Server` - 64 bit
* `Intel(R) Xeon(R) CPU E5-2690 v3` @ `2.60GHz`, (Support `SSE 4.2`)
*  `gcc 5.4.0`

```bash
 bitwise                  : CRC32 = 0x0E225381, SUM = 0xFAB9C5C6, 0.802 sec(s), 79.765 MB/s
 half-byte                : CRC32 = 0x08F105AF, SUM = 0xCAC04360, 0.734 sec(s), 174.290 MB/s
 tableless (byte)         : CRC32 = 0x08F105AF, SUM = 0x958086C0, 1.444 sec(s), 177.280 MB/s
 tableless2 (byte)        : CRC32 = 0x08F105AF, SUM = 0x958086C0, 1.438 sec(s), 178.010 MB/s
  1 byte  at once         : CRC32 = 0x08F105AF, SUM = 0x2B010D80, 1.435 sec(s), 356.725 MB/s
  4 bytes at once         : CRC32 = 0x08F105AF, SUM = 0x56021B00, 1.062 sec(s), 964.429 MB/s
  8 bytes at once         : CRC32 = 0x08F105AF, SUM = 0x56021B00, 0.676 sec(s), 1513.887 MB/s
 4x8 bytes at once        : CRC32 = 0x08F105AF, SUM = 0x56021B00, 0.505 sec(s), 2026.246 MB/s
 16 bytes at once         : CRC32 = 0x08F105AF, SUM = 0x56021B00, 0.331 sec(s), 3089.951 MB/s
 16 bytes at once         : CRC32 = 0x08F105AF, SUM = 0x56021B00, 0.332 sec(s), 3081.710 MB/s (including prefetching)
    chunked               : CRC32 = 0x08F105AF, SUM = 0x56021B00, 0.330 sec(s), 3098.610 MB/s

 folly::crc32()           : CRC32 = 0xDF220648, SUM = 0xDD955680, 0.092 sec(s), 11180.472 MB/s

 crc32c_hw_u32()          : CRC32 = 0x98387EBD, SUM = 0xFAE4F920, 0.274 sec(s), 3733.894 MB/s
 crc32c_hw_u64()          : CRC32 = 0x98387EBD, SUM = 0xFAE4F920, 0.138 sec(s), 7397.871 MB/s
 crc32c_hw_one_loop_x86() : CRC32 = 0x98387EBD, SUM = 0xFAE4F920, 0.273 sec(s), 3755.008 MB/s
 crc32c_hw_one_loop_x64() : CRC32 = 0x98387EBD, SUM = 0xFAE4F920, 0.137 sec(s), 7461.320 MB/s

 crc32c_hw_triplet_loop() : CRC32 = 0x98387EBD, SUM = 0xFAE4F920, 0.052 sec(s), 19564.910 MB/s
 folly::crc32c()          : CRC32 = 0x98387EBD, SUM = 0xFAE4F920, 0.054 sec(s), 18926.481 MB/s
```

## Contributors (贡献者)

* `shines77`: [https://github.com/shines77](https://github.com/shines77)
* ` Stephan Brumme`: [http://create.stephan-brumme.com/about.html](http://create.stephan-brumme.com/about.html)
* `Facebook`: [https://github.com/facebook](https://github.com/facebook)

## Official website (官网)

* `Github`: [https://github.com/shines77/crc32_test](https://github.com/shines77/crc32_test)
