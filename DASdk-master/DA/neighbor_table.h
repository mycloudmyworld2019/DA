// Copyright (c) 2014 Arista Networks, Inc.  All rights reserved.
// Arista Networks, Inc. Confidential and Proprietary.

/**
 * @file
 * The neighbor_table module manages IP/MAC mapping tables, including
 * IPv4 ARP table and IPv6 neighbor table.
 *
 * This module provides APIs to create and delete static IPv4/IPv6
 * entries. It also provides APIs to query the configured static
 * entries and the learned (both static/dynamic) entries on the system
 * from IPv4 ARP table and IPv6 neighbor table.
 *
 * Two handler APIs are provided to allow 3rd party agents to react to
 * the addition and deletion of entries in the learned ARP table or
 * neighbor table on the system.
 */

#ifndef DA_NEIGHBOR_TABLE_H
#define DA_NEIGHBOR_TABLE_H

#include <DA/eth.h>
#include <DA/base_handler.h>
#include <DA/intf.h>
#include <DA/ip.h>
#include <DA/iterator.h>

#include <DA/types/neighbor_table.h>

namespace DA {

class neighbor_table_mgr;
class neighbor_table_iter_impl;
class neighbor_table_status_iter_impl;

/**
 * The neighbor table handler.
 *
 * This class provides handler APIs to react to neighbor table entry deletion
 * and addition. It works for both the IPv4 ARP table and IPv6 neighbor table.
 */
class DA_SDK_PUBLIC neighbor_table_handler :
              public base_handler<neighbor_table_mgr, neighbor_table_handler> {
 public:
   explicit neighbor_table_handler(neighbor_table_mgr *mgr);
   neighbor_table_mgr * get_neighbor_table_mgr() const;

   /**
    * Registers this class to receive change updates on all neighbor entries. This
    * includes both IPv4 ARP table entries and IPv6 Neighbor Discovery table
    * entries.
    */
   void watch_all_neighbor_entries(bool interest);

   /**
    * Registers this class to receive change updates on a given neighbor entry. This
    * includes both IPv4 ARP table entries and IPv6 Neighbor Discovery table
    * entries.
    */
   void watch_neighbor_entry(neighbor_key_t const & key, bool interest);

   // Handler when an entry shows up in ARP table or Neighbor Discovery table.
   // The input parameter 'entry' is the newly created or updated neighbor entry.
   // Note if neighbor_table_mgr's API neighbor_entry_status() is called in this
   // function using 'entry.neighbor_key()', it may not return a neighbor_entry
   // that is the same as 'entry', since the neighbor entry might have changed.
   virtual void on_neighbor_entry_set(neighbor_entry_t const & entry);

   // Handler when an entry disappears from ARP table or Neighbor Discovery table.
   virtual void on_neighbor_entry_del(neighbor_key_t const & key);
};

/**
 * An iterator over the configured ARP and Neighbor Discovery tables.
 */
class DA_SDK_PUBLIC neighbor_table_iter_t : public iter_base<neighbor_key_t, 
                                                    neighbor_table_iter_impl> {
 private:
   friend class neighbor_table_iter_impl;
   explicit neighbor_table_iter_t(neighbor_table_iter_impl * const) DA_SDK_PRIVATE;
};

/**
 * An iterator over the resolved ARP and Neighbor Discovery tables.
 */
class DA_SDK_PUBLIC neighbor_table_status_iter_t : public iter_base<neighbor_key_t,
                                                   neighbor_table_status_iter_impl> {
 private:
   friend class neighbor_table_status_iter_impl;
   explicit neighbor_table_status_iter_t(
               neighbor_table_status_iter_impl * const) DA_SDK_PRIVATE;
};

/**
 * The neighbor table manager.
 *
 * This class provides access to the resolved MAC address for a given IP address
 * off of an L3 interface (aka the ARP table for IPv4 addresses and the Neighbor
 * Discovery table for IPv6 addresses). It also provides APIs to delete/add v4
 * static ARP entry and v6 static neighbor table entries.
 */
class DA_SDK_PUBLIC neighbor_table_mgr : public base_mgr<neighbor_table_handler,
                                                          neighbor_key_t> {
 public:
   virtual ~neighbor_table_mgr();

   /**
    * Iterates through the statically configured ARP and Neighbor Discovery tables.
    * A neighbor_key_t is returned for each entry.
    */
   virtual neighbor_table_iter_t neighbor_table_iter() const = 0;

   /**
    * Iterates through the resolved ARP and Neighbor Discovery tables. 
    * A neighbor_key_t is returned for each entry.
    */
   virtual neighbor_table_status_iter_t neighbor_table_status_iter() const = 0;

   // Attribute accessors

   /// Looks up a resolved neighbor entry for a given L3 interface and IP address.
   virtual neighbor_entry_t neighbor_entry_status(
                               neighbor_key_t const & key) const = 0;

   /**
    * Configures a new static entry in either ARP table for IPv4 or Neighbor table
    * for IPv6.
    * Note "intf_id" is needed only for v6 case, since the IP/MAC pair may not be
    * unique. It's not needed for v4 case.
    */
   virtual void neighbor_entry_set(neighbor_entry_t const & entry) = 0;
   /**
    * Deletes a static entry in either ARP table (IPv4) or Neighbor table for IPv6.
    * Note "intf_id" is needed only for v6 case, since the IP/MAC pair may not be
    * unique. It's not needed for v4 case.
    */
   virtual void neighbor_entry_del(neighbor_key_t const & key) = 0;

   /// Looks up a configured neighbor entry for a given L3 interface and IP address.
   virtual neighbor_entry_t neighbor_entry(neighbor_key_t const & key) const = 0;

 protected:
   neighbor_table_mgr() DA_SDK_PRIVATE;
   friend class neighbor_table_handler;

 private:
   DA_SDK_DISALLOW_COPY_CTOR(neighbor_table_mgr);
};

#include <DA/inline/neighbor_table.h>

} // end namespace DA

#endif // DA_NEIGHBOR_TABLE_H
