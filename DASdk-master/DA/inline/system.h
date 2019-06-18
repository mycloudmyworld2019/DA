// Copyright (c) 2014 Arista Networks, Inc.  All rights reserved.
// Arista Networks, Inc. Confidential and Proprietary.

#ifndef DA_INLINE_SYSTEM_H
#define DA_INLINE_SYSTEM_H

namespace DA {

inline system_mgr * system_handler::get_system_mgr() const {
   return mgr_;
}

}

#endif // DA_INLINE_SYSTEM_H
