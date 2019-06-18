// Copyright (c) 2016 Arista Networks, Inc.  All rights reserved.
// Arista Networks, Inc. Confidential and Proprietary.

#ifndef DA_TYPES_SDK_H
#define DA_TYPES_SDK_H

#include <sstream>

namespace DA {

/** Enums for Manager mode type. */
enum mgr_mode_type_t {
   /** Reader without notifications. */
   MODE_TYPE_READ_ONLY,
   /** Reader with notifications. */
   MODE_TYPE_READ_NOTIFYING,
   /** Writer, manager can read and write state. */
   MODE_TYPE_WRITE,
};
/** Appends a string representation of enum mgr_mode_type_t value to the ostream. */
std::ostream& operator<<(std::ostream& os, const mgr_mode_type_t & enum_val);
}

#include <DA/inline/types/sdk.h>

#endif // DA_TYPES_SDK_H
