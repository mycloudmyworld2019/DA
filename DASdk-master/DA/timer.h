// Copyright (c) 2013 Arista Networks, Inc.  All rights reserved.
// Arista Networks, Inc. Confidential and Proprietary.

#ifndef DA_TIMER_H
#define DA_TIMER_H

#include <DA/base.h>
#include <DA/base_handler.h>
#include <DA/base_mgr.h>

/**
 * @file
 * Periodic timeout support.
 *
 * This module allows timeouts to be set for a callback to be called
 * as soon as possible after some future time.
 */

namespace DA {

/**
 * Monotonically increasing time in seconds.
 *
 * Corresponds to the number of seconds since system boot plus an
 * arbitrary offset. As such, this value does not permit derivation of
 * a wallclock time (use time() from time.h for that).
 *
 * Instead, now() provides a monotonically increasing value unaffected
 * by leap seconds.
 */
seconds_t now() DA_SDK_PUBLIC;

/// 'never' represents a future time that now() can never reach.
static const seconds_t never = 0;

class timer_internal;
class timeout_mgr;

/**
 * The 'timeout_handler' class manages a single instance of a timer.
 *
 * To schedule a timeout, call timeout_time_is() using an absolute
 * time greater than now(). When the timeout_time has been exceeded,
 * on_timeout() is called and the timer is unset. To cancel a timer,
 * set the timeout to the 'never' constant.
 *
 * There are no guarantees that on_timeout() will be called exactly at
 * the configured timeout, though we guarantee this will never be
 * called *before* the configured timeout
 */
class DA_SDK_PUBLIC timeout_handler : public base_handler<timeout_mgr,
                                                           timeout_handler> {
 public:
   explicit timeout_handler(timeout_mgr * mgr);
   virtual ~timeout_handler();

   /**
    * Sets the next time on_timeout() will be called.
    *
    * The on_timeout() callback will be called as soon as possible
    * after the provided time.
    */
   void timeout_time_is(seconds_t);
   /// Returns the next timeout value.
   virtual void on_timeout() = 0;

 private:
   friend class timer_internal;
   timer_internal * timer_;
};

class DA_SDK_PUBLIC timeout_mgr : public base_mgr<timeout_handler> {
 public:
   virtual ~timeout_mgr();

 protected:
   virtual void init_handler(timeout_handler *) = 0;
   timeout_mgr() DA_SDK_PRIVATE;
   friend class timeout_handler;

 private:
   DA_SDK_DISALLOW_COPY_CTOR(timeout_mgr);
};

}

#endif // DA_TIMER_H
