// Copyright (c) 2014 Arista Networks, Inc.  All rights reserved.
// Arista Networks, Inc. Confidential and Proprietary.

#ifndef DA_INLINE_ETH_INTF_H
#define DA_INLINE_ETH_INTF_H

namespace DA {

inline eth_intf_mgr * eth_intf_handler::get_eth_intf_mgr() const {
   return mgr_;
}

}

#endif // DA_INLINE_ETH_INTF_H
