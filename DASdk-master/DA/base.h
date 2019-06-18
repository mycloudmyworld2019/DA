// Copyright (c) 2013 Arista Networks, Inc.  All rights reserved.
// Arista Networks, Inc. Confidential and Proprietary.

#ifndef DA_BASE_H
#define DA_BASE_H

#include <stdint.h> // for uint8_t, uint16_t, uint32_t, uint64_t, etc.

/**
 * @file
 * Arista DA Software Development Kit
 */

namespace DA {

// Big endian (network byte order) hinted forms of uint16_t/uint32_t/uint64_t
/// A uint16_t in big endian (network byte) order
typedef uint16_t uint16_be_t;
/// A uint32_t in big endian (network byte) order
typedef uint32_t uint32_be_t;
/// A uint64_t in big endian (network byte) order
typedef uint64_t uint64_be_t;

/// A decimal number of seconds, used for timestamps and durations
typedef double seconds_t;

#if defined(__GNUC__) && !defined(SWIG)
#define DA_SDK_PUBLIC __attribute__ ((visibility ("default")))
#ifdef DA_SDK_ALLOW_MOCKS // Allows the use of gMock and such.
# define DA_SDK_PRIVATE DA_SDK_PUBLIC
#else
# define DA_SDK_PRIVATE __attribute__ ((visibility ("hidden")))
#endif
// Things we need to expose but that aren't part of the API.
// TODO(tsuna): Why can't we make this visibility protected?
#define DA_SDK_INTERNAL __attribute__ ((visibility ("default")))
#define DA_SDK_NORETURN __attribute__ ((noreturn))
#define DA_SDK_DEPRECATED __attribute__ ((deprecated))
#define DA_SDK_FORMAT_STRING_CHECK(N) __attribute__ ((format (printf, N, N+1)))
#else
// SWIG doesn't understand __attribute__ overload macros
#define DA_SDK_PUBLIC
#define DA_SDK_PRIVATE
#define DA_SDK_INTERNAL
#define DA_SDK_NORETURN
#define DA_SDK_DEPRECATED
#define DA_SDK_FORMAT_STRING_CHECK(N)
#endif

#define DA_SDK_DISALLOW_COPY_CTOR(ClassName) \
 private: \
   ClassName(ClassName const &) DA_SDK_PRIVATE; \
   ClassName & operator=(ClassName const &) DA_SDK_PRIVATE;

}

#endif // DA_BASE_H
