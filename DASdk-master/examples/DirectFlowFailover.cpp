// Copyright (c) 2014 Arista Networks, Inc.  All rights reserved.
// Arista Networks, Inc. Confidential and Proprietary.

#include <DA/agent.h>
#include <DA/directflow.h>
#include <DA/intf.h>
#include <DA/sdk.h>
#include <DA/timer.h>

#include <cstdio>
#include <cassert>

#include "DirectFlowReprogrammer.cpp"

#define USE_REPROGRAMMER 1

static const DA::intf_id_t INPUT_INTF = DA::intf_id_t("Ethernet1");
static const DA::intf_id_t PRIMARY_OUT_INTF = DA::intf_id_t("Ethernet3");
static const DA::intf_id_t BACKUP_OUT_INTF = DA::intf_id_t("Ethernet5");

static const std::string FLOW_NAME = "MyFlow";

class dflow_helper : public DA::intf_handler,
                     public DA::agent_handler,
                     public DA::timeout_handler {
 public:
   explicit dflow_helper(DA::sdk & sdk)
      : DA::intf_handler(sdk.get_intf_mgr()),
        DA::agent_handler(sdk.get_agent_mgr()),
        DA::timeout_handler(sdk.get_timeout_mgr()),
        dflow_reprogrammer(sdk) {
      dflow_mgr = sdk.get_directflow_mgr();
      watch_intf(PRIMARY_OUT_INTF, true);
   }

   ~dflow_helper() {
   }

   void on_initialized() {
      printf("on_initialized - dflow_helper starting\n");
      create_dflow_entry(PRIMARY_OUT_INTF);
   }

   void on_oper_status(DA::intf_id_t intf, DA::oper_status_t status) {
      if(status == DA::INTF_OPER_UP) {
         printf("Waiting 0.1sec to use primary interface\n");
         timeout_time_is( DA::now() + 0.1 );
      } else {
         printf("Bringing up backup interface\n");
         create_dflow_entry(BACKUP_OUT_INTF);
      }
   }

   void on_timeout() {
      printf("Bringing up primary interface\n");
      create_dflow_entry(PRIMARY_OUT_INTF);
   }

   void create_dflow_entry(DA::intf_id_t const & output_intf) {
      // Match any traffic on the input intf...
      DA::flow_match_t match = DA::flow_match_t();
      DA::flow_match_field_set_t match_field_set;
      match_field_set.input_intfs_is(true);
      match.match_field_set_is(match_field_set);
      match.input_intf_set(INPUT_INTF);

      // ... and send it to the output intf parameter
      DA::flow_action_t action = DA::flow_action_t();
      DA::flow_action_set_t action_set;
      action_set.set_output_intfs_is(true);
      action.output_intf_set(output_intf);
      action.action_set_is(action_set);

      // Now define the flow entry
      DA::flow_priority_t priority = 1;
      DA::flow_entry_t entry("TunnelFlow", match, action, priority);
#if USE_REPROGRAMMER
      dflow_reprogrammer.updateFlow(entry);
#else
      dflow_mgr->flow_entry_set(entry);
#endif
   }

 private:
   DA::directflow_mgr * dflow_mgr;
   DirectFlowReprogrammer dflow_reprogrammer;
};

int main(int argc, char ** argv) {
   DA::sdk sdk("DflowFailoverAgent");
   dflow_helper demo(sdk);
   sdk.main_loop(argc, argv);
}

