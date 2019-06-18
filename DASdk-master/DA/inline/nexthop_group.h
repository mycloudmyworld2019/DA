// Copyright (c) 2015 Arista Networks, Inc.  All rights reserved.
// Arista Networks, Inc. Confidential and Proprietary.

#ifndef DA_INLINE_NEXTHOP_GROUP_H
#define DA_INLINE_NEXTHOP_GROUP_H

namespace DA {

inline nexthop_group_mgr * nexthop_group_handler::get_nexthop_group_mgr() const {
   return mgr_;
}

}

#endif // DA_INLINE_NEXTHOP_GROUP_H
