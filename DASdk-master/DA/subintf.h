// Copyright (c) 2014 Arista Networks, Inc.  All rights reserved.
// Arista Networks, Inc. Confidential and Proprietary.

/**
 * @file
 * The subintf module manages subinterfaces.
 *
 * Subinterfaces are multiple logical routed interfaces under an Ethernet or
 * LAG routed interface (also referred to as the parent interface). Each
 * subinterface is assigned a VLAN tag that is unique to the subinterface
 * (among all its sibling subinterfaces under the same parent interface). The
 * VLAN tag is used for mux/demux of data traffic from/to all subinterfaces
 * under the parent interface.
 *
 * This module provides APIs to provision subinterfaces. APIs include subinterface
 * creation, deletion, retrieval and getter/setter for the subinterface VLAN tag.
 * An iterator is also provided to iterate through all the subinterfaces that are
 * configured in the system.
 *
 * The subinterface APIs can be used with the ones in "intf" module, as shown
 * in the sample code below. For example, the DA::intf_handler's on_intf_create()
 * API can be used to react to the creation of new subinterfaces.
 *
 * @code
 *
   #include <DA/agent.h>
   #include <DA/subintf.h>
   #include <DA/panic.h>
   #include <DA/sdk.h>

   static DA::subintf_mgr *subintf_mgr;

   class ApplicationAgent : public DA::agent_handler, DA::intf_handler {
    public:
      explicit ApplicationAgent(DA::sdk & sdk) :
                                DA::agent_handler(sdk.get_agent_mgr()),
                                DA::intf_handler(sdk.get_intf_mgr()) {
          watch_intf(DA::intf_id_t("Ethernet1.1"), true);
      }

      // Called when all agents are initialized
      void on_initialized() {
         // Creates subinterface
         subintf_mgr->subintf_is(DA::intf_id_t("Ethernet1.1"));
      }

      // Handler for subinterface creation
      void on_intf_create(DA::intf_id_t i) {
         if (i == DA::intf_id_t("Ethernet1.1")) {
            // Ethernet1.1 is now usable.
            // Other application logic goes here, such as creating more subinterfaces
         }
         return;
      }
   }

   int main(int argc, char **argv) {
      DA::sdk sdk;
      subintf_mgr = sdk.get_subintf_mgr();
      ApplicationAgent agent(sdk);
      sdk.main_loop(argc, argv);
   }
 * @endcode
 */


#ifndef DA_SUBINTF_H
#define DA_SUBINTF_H

#include <DA/intf.h>
#include <DA/iterator.h>
#include <DA/types/subintf.h>

namespace DA {

class subintf_iter_impl;

/**
 * An iterator over all subinterfaces in the system.
 */
class DA_SDK_PUBLIC subintf_iter_t : public iter_base<intf_id_t,
                                                       subintf_iter_impl> {
 private:
   explicit subintf_iter_t(subintf_iter_impl * const) DA_SDK_PRIVATE;
   friend class subintf_iter_impl;
};

/**
 * The manager for subinterfaces.  This is the main entry point for applications
 * to use DASdk subintf APIs.
 */
class DA_SDK_PUBLIC subintf_mgr {
 public:
   virtual ~subintf_mgr();

   /**
    * Returns an iterator for iterating over the subinterfaces in the system.
    * The iterator yields an intf_id_t for each configured subinterface.
    */
   virtual subintf_iter_t subintf_iter() const = 0;

   /**
    * Returns whether the given subinterface exists.
    *
    * If exists returns true, then this intf_id_t can be successfully
    * passed into every method of the subintf_mgr. If not, then
    * methods of the subintf_mgr can throw a no_such_interface_error
    * exception.
    *
    * The exists method of all *intf_mgr classes that manage a given
    * interface (ie intf_mgr, eth_intf_mgr, and subintf_mgr for
    * subinterfaces) are all guaranteed to return the same result.
    */
   virtual bool exists(intf_id_t) const = 0;

   /**
    * Creates a subinterface when given the subinterface ID.
    * No action will be taken if the subinterface exists already.
    * The subinterface will be created even if its parent interface does not exist.
    * But the subinterface will become operationally up on only after its parent
    * interface becomes operationally up and is in routed mode.
    */
   virtual void subintf_is(intf_id_t) = 0;

   /**
    * Creates a subinterface and sets its VLAN tag.  If the subinterface already
    * exists, its VLAN tag will be updated.
    */
   virtual void subintf_is(intf_id_t, vlan_id_t) = 0;

   /**
    * Returns the subinterface with the given ID.
    * Will return an empty subintf_t if the subinterface does not exist.
    */
   virtual subintf_t subintf(intf_id_t) const = 0;

   /**
    * Deletes a subinterface.  It will simply return if the given subinterface
    * does not exist.
    */
   virtual void subintf_del(intf_id_t) = 0;

   /**
    * Returns the configured VLAN tag of a given subinterface. Returns zero if
    * the subinterface does not exist or if no VLAN tag is configured for the
    * subinterface.
    */
   virtual vlan_id_t vlan_tag(intf_id_t) const = 0;

   /**
    * Configures the VLAN tag of a given subinterface.  Throws
    * invalid_argument_error exception if the subinterface does not exist.
    */
   virtual void vlan_tag_is(intf_id_t, vlan_id_t) = 0;

   /**
    * Returns the parent interface ID for a given subinterface. The parent
    * interface will be an Ethernet or a LAG interface.
    * Returns an empty intf_id_t (with intf_type as INTF_TYPE_NULL), if the given
    * interface is not a subinterface.
    */
   virtual intf_id_t parent_intf(intf_id_t) const = 0;

 protected:
   subintf_mgr() DA_SDK_PRIVATE;

 private:
   DA_SDK_DISALLOW_COPY_CTOR(subintf_mgr);
};

}

#endif // DA_SUBINTF_H
