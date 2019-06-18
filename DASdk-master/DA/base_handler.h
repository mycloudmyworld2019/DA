// Copyright (c) 2014 Arista Networks, Inc.  All rights reserved.
// Arista Networks, Inc. Confidential and Proprietary.

#ifndef DA_BASE_HANDLER_H
#define DA_BASE_HANDLER_H

#include <DA/base.h>

namespace DA {

template <typename Mgr, typename Handler>
class DA_SDK_PRIVATE base_handler {
 protected:
    explicit base_handler(Mgr * mgr) : mgr_(mgr) {
    }

    virtual ~base_handler() {
       if (mgr_) {
          // The add_handler has to remove every interface-specific handler...
          mgr_->add_handler(static_cast<Handler*>(this));
          mgr_->remove_handler(static_cast<Handler*>(this));
       }
    }

    void unregister_mgr() {
       mgr_ = 0;
    }

    template <typename T, typename Key> friend class base_mgr;
    Mgr * mgr_;
};

}

#endif // DA_BASE_HANDLER_H
