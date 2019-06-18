// Copyright (c) 2014 Arista Networks, Inc.  All rights reserved.
// Arista Networks, Inc. Confidential and Proprietary.

#ifndef DA_INLINE_AGENT_H
#define DA_INLINE_AGENT_H

namespace DA {

inline agent_mgr * agent_handler::get_agent_mgr() const {
   return mgr_;
}

}

#endif // DA_INLINE_AGENT_H
