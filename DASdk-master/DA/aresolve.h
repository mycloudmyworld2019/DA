// Copyright (c) 2014 Arista Networks, Inc.  All rights reserved.
// Arista Networks, Inc. Confidential and Proprietary.

#ifndef DA_ARESOLVE_H
#define DA_ARESOLVE_H

#include <string>

#include <DA/base.h>
#include <DA/base_handler.h>
#include <DA/base_mgr.h>
#include <DA/types/aresolve.h>

/**
 * @file
 * Aresolve provides asynchronous DNS host resolution.
 *
 * Aresolve resolves and automatically re-resolves DNS information for
 * requests made via the aresolve_handler's watch_* methods. Whenever
 * resolution information about the watched name changes, the
 * aresolve_handler's on_aresolve_host() function is called.
 *
 * For example, an application may wish to track the IP address of a
 * particular infrastructure hostname (e.g., a syslog receiver we'll
 * call "loghost"). In this example, an IP address object is created
 * to track the current address of the loghost:
 *
 * @code
 * #include <assert.h>
 *
 * #include <DA/agent.h>
 * #include <DA/aresolve.h>
 * #include <DA/ip.h>
 * #include <DA/sdk.h>
 *
 * class Aresolver : public DA::aresolve_handler, public DA::agent_handler {
 *  public:
 *    explicit Aresolver(DA::sdk & sdk)
 *       : DA::aresolve_handler(sdk.get_aresolve_mgr()),
 *         DA::agent_handler(sdk.get_agent_mgr()) {
 *    }
 *
 *    void on_initialized() {
 *       // Register our interest in the IP address of "loghost"
 *       watch_host("loghost", true);
 *    }
 *
 *    virtual void on_aresolve_host(DA::aresolve_record_host const & record) {
 *       // Called on initial host resolution or updates in host address
 *       if (record.valid()) {
 *          // We should only ever see results for "loghost".
 *          assert(record.qname() == "loghost");
 *          // Get the IP address; we'll assume there's only one in this case.
 *          if (record.addr_v4().size() > 0) {  // IPv4
 *             address = record.addr_v4().front();
 *          } else if (record.addr_v6().size() > 0) {  // IPv6
 *             address = record.addr_v6().front();
 *          }
 *          // Do something with the new address, like send a syslog message.
 *       } else {
 *          // There was a host resolution error. Inspect record.last_error(),
 *          // which is an EAI_* error from netdb.h.
 *       }
 *    }
 *
 *  private:
 *    DA::ip_addr_t address;
 * };
 * @endcode
 */

namespace DA {

class aresolve_mgr;

/**
 * The Aresolve handler.
 *
 * This handler receives callback notifications about objects that are
 * being resolved via watch_* calls.
 */
class DA_SDK_PUBLIC aresolve_handler : public base_handler<aresolve_mgr,
                                                            aresolve_handler> {
 public:
   explicit aresolve_handler(aresolve_mgr *);
   aresolve_mgr * get_aresolve_mgr() const;

   /**
    * Starts or stops watching IPv4 and IPv6 addresses for the provided host name.
    *
    * When host resolution completes or an error occurs,
    * on_aresolve_host() is called.  After host resolution completes
    * initially, watched hosts will be refreshed automatically. When
    * changes in the result happen, your handler's on_aresolve_host(),
    * method is called. In this way, you need only watch hostnames
    * you're interested in and implement the on_aresolve_host() method
    * to deal with the changes in results.
    *
    * @param host The hostname to watch
    * @param watch If true, start watching the hostname, else stop watching.
    */
   void watch_host(std::string const & host, bool watch);

   /**
    * Callback called by Aresolve when host resolution completes.
    *
    * Called by Aresolve when DNS resolution of a host previously
    * watched by watch_host() completes.
    *
    * Will be called upon successful updates or otherwise, when either
    * the valid or last update time member attributes change. May be
    * called more than once for a given hostname, if DNS had a
    * temporary failure.
    *
    * @param record A record containing new or updated information
    * about a watched host.
    */
   virtual void on_aresolve_host(aresolve_record_host const & record);
};

class DA_SDK_PUBLIC aresolve_mgr : public base_mgr<aresolve_handler, std::string> {
 public:
   virtual ~aresolve_mgr();

   /**
    * Aresolve implementation configuration settings.
    *
    * The short time defines the minimum period between DNS
    * resolutions. (default: 1s)
    */
   virtual uint32_t aresolve_short_time() const = 0;

   /**
    * Returns the long timer, or seconds between repeated DNS queries.
    *
    * You will receive at most one notification per DNS query (watched
    * host) every aresolve_long_time() number of seconds. (default: 300s)
    */
   virtual uint32_t aresolve_long_time() const = 0;

   /// Sets the Aresolve short timer to the value provided.
   virtual void aresolve_short_time_is(uint32_t) = 0;
   /// Sets the Aresolve long timer to the value provided.
   virtual void aresolve_long_time_is(uint32_t) = 0;

 protected:
   aresolve_mgr() DA_SDK_PRIVATE;
   friend class aresolve_handler;

 private:
   DA_SDK_DISALLOW_COPY_CTOR(aresolve_mgr);

};

}

#include <DA/inline/aresolve.h>

#endif // DA_ARESOLVE_H
