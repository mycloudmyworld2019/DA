// Copyright (c) 2016 Arista Networks, Inc.  All rights reserved.
// Arista Networks, Inc. Confidential and Proprietary.

#ifndef DA_TYPES_MLAG_H
#define DA_TYPES_MLAG_H


namespace DA {

enum mlag_state_t {
   MLAG_STATE_UNKNOWN,
   MLAG_STATE_DISABLED,
   MLAG_STATE_INACTIVE,
   MLAG_STATE_PRIMARY,
   MLAG_STATE_SECONDARY,
};
/** Appends a string representation of enum mlag_state_t value to the ostream. */
std::ostream& operator<<(std::ostream& os, const mlag_state_t & enum_val);
}

#include <DA/inline/types/mlag.h>

#endif // DA_TYPES_MLAG_H
