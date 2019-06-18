// Copyright (c) 2014 Arista Networks, Inc.  All rights reserved.
// Arista Networks, Inc. Confidential and Proprietary.

#ifndef DA_UTILITY_H
#define DA_UTILITY_H

#include <iostream>

namespace DA {

/**
 * Overwriting "<<" operator for uint8_t. The default behavior is to
 * print the value out as "char" (sometimes even invisible). What we
 * want is numerical value.
 * 
 * This is used in to_string() method that is auto-generated.
 */

std::ostream &operator<<(std::ostream &os, uint8_t v) DA_SDK_PUBLIC;

}

#include <DA/inline/utility.h>

#endif // DA_UTILITY_H
