// Copyright (c) 2014 Arista Networks, Inc.  All rights reserved.
// Arista Networks, Inc. Confidential and Proprietary.

#include <DA/agent.h>
#include <DA/ip.h>
#include <DA/ip_route.h>
#include <DA/mpls.h>
#include <DA/nexthop_group.h>
#include <DA/sdk.h>

#include <stdio.h>

class my_agent : public DA::agent_handler {
 public:
   DA::agent_mgr * agentMgr_;
   DA::ip_route_mgr * ipMgr_;
   DA::nexthop_group_mgr * nhgMgr_;

   explicit my_agent(DA::sdk & sdk)
         : DA::agent_handler(sdk.get_agent_mgr()),
           agentMgr_(sdk.get_agent_mgr()),
           ipMgr_(sdk.get_ip_route_mgr()),
           nhgMgr_(sdk.get_nexthop_group_mgr()) {
      printf("Constructing my agent...\n");
   }

   void create_nexthop_group(std::string name) {
      printf("Creating nexthop group\n");
      DA::nexthop_group_t nhg = DA::nexthop_group_t(name,
                                                      DA::NEXTHOP_GROUP_MPLS);

      DA::nexthop_group_mpls_action_t mplsActionA(
            DA::MPLS_ACTION_PUSH, {DA::mpls_label_t(30), DA::mpls_label_t(31)});
      DA::nexthop_group_mpls_action_t mplsActionB(
            DA::MPLS_ACTION_PUSH, {DA::mpls_label_t(40), DA::mpls_label_t(41)});

      // Send 2/3rds of traffic to nexthop 10.0.0.33 with the [30, 31]
      // label stack:

      DA::nexthop_group_entry_t nhe1(DA::ip_addr_t("10.0.0.33"));
      nhe1.mpls_action_is(mplsActionA);
      nhg.nexthop_set(1, nhe1);
      nhg.nexthop_set(2, nhe1);

      // Send 1/3rd of traffic to nexthop 10.0.0.44 with the [40, 41]
      // label stack:
      DA::nexthop_group_entry_t nhe2(DA::ip_addr_t("10.0.0.44"));
      nhe2.mpls_action_is(mplsActionB);
      nhg.nexthop_set(3, nhe2);

      // And commit it to Sysdb!
      nhgMgr_->nexthop_group_set(nhg);
   }

   void on_initialized() {
      printf("Initializing my agent...\n");

      create_nexthop_group("MplsNhg1");
      DA::ip_route_key_t routeKey(
            DA::ip_prefix_t(DA::ip_addr_t("172.1.1.4"), 32));
      DA::ip_route_t route(routeKey);
      DA::ip_route_via_t via(routeKey);
      via.nexthop_group_is("MplsNhg1");

      ipMgr_->ip_route_set(route);
      ipMgr_->ip_route_via_set(via);

      printf("Done!\n");
      agentMgr_->exit(); // (we'll just exit after this demo)
   }
};

int main(int argc, char ** argv) {
   DA::sdk sdk;
   my_agent agent(sdk);
   sdk.main_loop(argc, argv);
}
