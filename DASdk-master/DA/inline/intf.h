// Copyright (c) 2013 Arista Networks, Inc.  All rights reserved.
// Arista Networks, Inc. Confidential and Proprietary.

#ifndef DA_INLINE_INTF_H
#define DA_INLINE_INTF_H

#include <DA/panic.h>

namespace DA {

inline intf_mgr * intf_handler::get_intf_mgr() const {
   return mgr_;
}

}

#endif // DA_INLINE_INTF_H
