// Copyright (c) 2014 Arista Networks, Inc.  All rights reserved.
// Arista Networks, Inc. Confidential and Proprietary.

#ifndef DA_INLINE_POLICY_H
#define DA_INLINE_POLICY_H

namespace DA {

inline policy_map_mgr * policy_map_handler::get_policy_map_mgr() const {
   return mgr_;
}

} // namespace DA

#endif // DA_INLINE_POLICY_H
