#pragma once

#ifndef CASC_TYPES_H
# define CASC_TYPES_H

# include <stdbool.h>
# include <stddef.h>
# include <stdint.h>
# include <stdio.h>

# if UINTPTR_MAX == UINT32_MAX

#  define CASC_POINTER_BITS 32
#  define CASC_IS_32_BIT 1
#  define CASC_IS_64_BIT 0

# elif UINTPTR_MAX == UINT64_MAX

#  define CASC_POINTER_BITS 64
#  define CASC_IS_32_BIT 0
#  define CASC_IS_64_BIT 1

# else
#  error "CASC does not support this pointer width"
# endif

# define CASC_TRUE 1
# define CASC_FALSE 0
typedef bool casc_bool_t;
typedef const casc_bool_t casc_cbool_t;

typedef int8_t casc_s8_t;
typedef uint8_t casc_u8_t;

typedef int16_t casc_s16_t;
typedef uint16_t casc_u16_t;

typedef int32_t casc_s32_t;
typedef uint32_t casc_u32_t;

typedef int64_t casc_s64_t;
typedef uint64_t casc_u64_t;

typedef const casc_s8_t casc_cs8_t;
typedef const casc_u8_t casc_cu8_t;

typedef const casc_s16_t casc_cs16_t;
typedef const casc_u16_t casc_cu16_t;

typedef const casc_s32_t casc_cs32_t;
typedef const casc_u32_t casc_cu32_t;

typedef const casc_s64_t casc_cs64_t;
typedef const casc_u64_t casc_cu64_t;

typedef intptr_t casc_sptr_t;
typedef uintptr_t casc_uptr_t;

typedef const casc_sptr_t casc_csptr_t;
typedef const casc_uptr_t casc_cuptr_t;

typedef size_t casc_size_t;
typedef ptrdiff_t casc_diff_t;

typedef const casc_size_t casc_csize_t;
typedef const casc_diff_t casc_cdiff_t;

typedef float casc_f32_t;
typedef double casc_f64_t;

typedef const casc_f32_t casc_cf32_t;
typedef const casc_f64_t casc_cf64_t;

typedef char casc_char_t;
typedef uint8_t casc_byte_t;

typedef const casc_char_t casc_cchar_t;
typedef const casc_byte_t casc_cbyte_t;

typedef FILE casc_file_t;
typedef FILE *casc_file_pt;

#endif