// Copyright (c) 2014 Arista Networks, Inc.  All rights reserved.
// Arista Networks, Inc. Confidential and Proprietary.

#ifndef DA_SDK_HASH_MIX
#define DA_SDK_HASH_MIX

#include <DA/base.h>

namespace DA {

/**
 * This is a helper class for incrementally-computed implementation of Bob
 * Jenkins' one-at-a-time hash, in C++.  It's needed in hash function.
 */

class DA_SDK_PUBLIC hash_mix {
 public:
   static uint32_t mix( uint8_t * key, int len, uint32_t seed );
   static uint32_t final_mix( uint32_t seed );
};

}

#include <DA/inline/hash_mix.h>

#endif
