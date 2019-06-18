// Copyright (c) 2014 Arista Networks, Inc.  All rights reserved.
// Arista Networks, Inc. Confidential and Proprietary.

#ifndef DA_MLAG_H
#define DA_MLAG_H

#include <DA/base.h>
#include <DA/base_handler.h>
#include <DA/base_mgr.h>
#include <DA/ip.h>

#include <DA/types/mlag.h>

namespace DA {

class mlag_mgr;

class DA_SDK_PUBLIC mlag_handler : public base_handler<mlag_mgr, mlag_handler> {
 public:
   explicit mlag_handler(mlag_mgr *mgr);
   mlag_mgr * get_mlag_mgr() const;

   virtual void on_mlag_state(mlag_state_t);
};

class DA_SDK_PUBLIC mlag_mgr : public base_mgr<mlag_handler> {
 public:
   virtual ~mlag_mgr();

   /// Returns whether or not MLAG is configured on this system
   virtual bool enabled() const = 0;
   /// Configures whether MLAG is enabled
   virtual void enabled_is(bool) = 0;

   /// Returns the configured IP address of our MLAG peer
   virtual ip_addr_t peer_address() const = 0;

   /// Returns the current operational state of our MLAG relationship
   virtual mlag_state_t state() const = 0;

 protected:
   mlag_mgr() DA_SDK_PRIVATE;
   friend class mlag_handler;

 private:
   DA_SDK_DISALLOW_COPY_CTOR(mlag_mgr);
};

}

#include <DA/inline/mlag.h>

#endif // DA_MLAG_H
