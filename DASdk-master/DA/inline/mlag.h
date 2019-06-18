// Copyright (c) 2014 Arista Networks, Inc.  All rights reserved.
// Arista Networks, Inc. Confidential and Proprietary.

#ifndef DA_INLINE_MLAG_H
#define DA_INLINE_MLAG_H

namespace DA {

inline mlag_mgr * mlag_handler::get_mlag_mgr() const {
   return mgr_;
}

}

#endif // DA_INLINE_MLAG_H
