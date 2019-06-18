// Copyright (c) 2015 Arista Networks, Inc.  All rights reserved.
// Arista Networks, Inc. Confidential and Proprietary.

#include <assert.h>

#include <DA/agent.h>
#include <DA/eth_intf.h>
#include <DA/eth_lag_intf.h>
#include <DA/exception.h>
#include <DA/panic.h>
#include <DA/sdk.h>
#include <DA/tracing.h>

/**
 * This example agent program shows how to make use of DASdk eth_lag_intf APIs.
 * It demonstrates how to create new port channels and adding member interfaces
 * to port channels.
 *
 * To run this example:
 *
 * $ scp EthLagIntfExample switch:/mnt/flash/
 * $ ssh switch
 * switch# bash
 * Arista Networks DA shell
 *
 * $ TRACE="EthLagIntfExample*" /mnt/flash/EthLagIntfExample
 *
 * Note, on_lag_member_set will only be called when the member interface becomes
 * active in the port channel.
 */


class ApplicationAgent : public DA::agent_handler,
                         public DA::eth_lag_intf_handler,
                         public DA::eth_intf_handler,
                         public DA::intf_handler {
 public:
   DA::eth_lag_intf_mgr *eth_lag_intf_mgr_;
   DA::tracer t;

   explicit ApplicationAgent(DA::sdk & sdk) :
            DA::agent_handler(sdk.get_agent_mgr()),
            DA::eth_lag_intf_handler(sdk.get_eth_lag_intf_mgr()),
            DA::eth_intf_handler(sdk.get_eth_intf_mgr()),
            DA::intf_handler(sdk.get_intf_mgr()),
            eth_lag_intf_mgr_(sdk.get_eth_lag_intf_mgr()),
            t("EthLagIntfExample") {
      watch_intf(DA::intf_id_t("Port-Channel23"), true);
      watch_eth_lag_intf(DA::intf_id_t("Port-Channel23"), true);
   }

   // Called when all agents are initialized
   void on_initialized() {
      // Creates LAG 23, no mode specified
      eth_lag_intf_mgr_->eth_lag_intf_is(DA::intf_id_t("Port-Channel23"));
      t.trace8("ApplicationAgent initialized.");
   }

   // Handler for LAG interface creation
   void on_intf_create(DA::intf_id_t lag) {
      t.trace8("ApplicationAgent on_intf_create, for interface: %s.",
               lag.to_string().c_str());
      if (lag == DA::intf_id_t("Port-Channel23")) {
         t.trace8("ApplicationAgent Port-Channel23 created.");

         // Application logic goes here, such as creating more LAG interfaces
         // or adding member interfaces to it.
         eth_lag_intf_mgr_->membership_set(DA::intf_id_t("Ethernet1"),
                  lag,
                  DA::ETH_LAG_INTF_MEMBER_LACP_MODE_PASSIVE);
         t.trace8("ApplicationAgent: added Ethernet1 to %s.",
                  lag.to_string().c_str());
      }
      return;
   }

   // Handler called when a physical interface becomes active in a LAG.
   // In the case of LACP managed interfaces, members do not become
   // active until LACP negotiation is complete.
   void on_lag_member_set(DA::intf_id_t lag, DA::intf_id_t member) {
      t.trace8("ApplicationAgent on_lag_member_set: %s added to %s",
                member.to_string().c_str(), lag.to_string().c_str());
      // Perform some relevant action, such as reporting to a monitoring
      // system or provisioning workflow manager.
   }

   // Handler called when a physical interface is no longer active in a LAG.
   void on_lag_member_del(DA::intf_id_t lag, DA::intf_id_t member) {
      // Custom application code to react to a physical interface
      // leaving a LAG for any reason. For example, to get the reason:
      auto stat = eth_lag_intf_mgr_->eth_lag_intf_membership_status(member);
      std::string reason(stat.reason());  // e.g., 'not link up'
      t.trace8("ApplicationAgent on_lag_member_del: %s deleted from %s",
                member.to_string().c_str(), lag.to_string().c_str());
   }

};

int main(int argc, char **argv) {
   DA::sdk sdk;
   ApplicationAgent agent(sdk);
   sdk.main_loop(argc, argv);
}
