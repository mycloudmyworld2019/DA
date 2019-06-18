// Copyright (c) 2014 Arista Networks, Inc.  All rights reserved.
// Arista Networks, Inc. Confidential and Proprietary.

#ifndef EXAMPLES_LIBEVENT_H
#define EXAMPLES_LIBEVENT_H

#include <sys/time.h>

#include <DA/event_loop.h>
#include <DA/sdk.h>

struct event_base;  // Forward declare libevent's main data structure.

class libevent_loop : public DA::event_loop_handler {
 public:
   libevent_loop(DA::sdk & sdk, event_base * eb)
      : eb_(eb), loop_(sdk.get_event_loop()) {
   }

   virtual ~libevent_loop() {
   }

   void init();
   virtual void watch_readable(int fd, bool interested);
   virtual void watch_writable(int fd, bool interested);
   virtual void watch_exception(int fd, bool interested);
   virtual void timeout_time_is(struct timeval deadline);

 private:
   event_base * const eb_;
   DA::event_loop * loop_;
};

#endif // EXAMPLES_LIBEVENT_H
