// Copyright (c) 2014 Arista Networks, Inc.  All rights reserved.
// Arista Networks, Inc. Confidential and Proprietary.

#include <unordered_set>

#include <DA/agent.h>
#include <DA/acl.h>
#include <DA/intf.h>
#include <DA/class_map.h>
#include <DA/policy_map.h>
#include <DA/sdk.h>
#include <DA/system.h>
#include <DA/tracing.h>

#include <cstdio>

class policy_demo : public DA::agent_handler,
                    public DA::acl_handler,
                    public DA::policy_map_handler {
 public:
   DA::tracer t;
   explicit policy_demo(DA::sdk & sdk)
         : DA::agent_handler(sdk.get_agent_mgr()),
           DA::acl_handler(sdk.get_acl_mgr()),
           DA::policy_map_handler(sdk.get_policy_map_mgr()),
           t("PolicyDemo") {

      // Setup manager refs
      class_mgr = sdk.get_class_map_mgr();

      // Register for all ACL updates; when programming policies,
      // ACLs must be programmed before class maps before policy maps,
      // and they may fail independently of policy-map based PBR
      // programming. If ACL programming for ACLs used in class-maps
      // fails, the reported problem must be resolved before PBR
      // programming will complete.
      watch_all_acls(true);

   }

   ~policy_demo() {
   }

   void on_initialized() {
      printf("on_initialized - policy_demo starting\n");
      setup_policy();
   }

   // Configure PBR policymap pm1 with two class-maps and two single line raw
   // match rules.
   //
   // Service policy pm1
   //   Configured on: Management0
   //   Applied on:
   //   1: Class-map: cm1 (match-any)
   //     Match: 1 IP Access List acl1
   //         1 permit ip any 20.20.0.0/16 dscp 32
   //     Configured actions: drop
   //     Active routing action: No PBR action
   //   2: Class-map: cm2 (match-any)
   //     Match: 1 IPV6 Access List acl2
   //         1 permit ipv6 any 2001:db8:1234::/48 dscp 32
   //     Configured actions: drop
   //     Active routing action: No PBR action
   //   3: Single match statement
   //     Match:
   //         0 permit ip any 30.30.0.0/16 dscp 32
   //     Configured actions: set nexthop-group nexthopgroup2
   //     Active routing action: No PBR action
   //   4: Single match statement
   //     Match:
   //         0 permit ipv6 any 2001:db8:5678::/48 dscp 32
   //     Configured actions: set nexthop-group nexthopgroup2
   //     Active routing action: No PBR action

   void setup_policy() {
      DA::ip_addr_t dip;

      // Configure V4 ACL 
      auto acl1 = DA::acl_key_t("acl1", DA::ACL_TYPE_IPV4);
      auto rule1 = DA::acl_rule_ip_t();
      rule1.action_is(DA::ACL_PERMIT);
      DA::parse_ip_addr("20.20.0.0", &dip);
      rule1.destination_addr_is(DA::ip_addr_mask_t(dip, 16));
      rule1.priority_value_is(32);
      rule1.match_ip_priority_is(true);
      get_acl_mgr()->acl_rule_set(acl1, 1, rule1);
      get_acl_mgr()->acl_commit();

      // Now build the class map for that ACL and commit it
      auto cmkey = DA::class_map_key_t("cm1", DA::POLICY_FEATURE_PBR);
      auto cm = DA::class_map_t(cmkey);
      cm.rule_set(1, DA::class_map_rule_t(acl1));
      class_mgr->class_map_is(cm);
      t.trace0("class map cm input: %s", cm.to_string().c_str());
      auto cm_res = class_mgr->class_map(cmkey);
      t.trace0("class map cm outpt: %s", cm_res.to_string().c_str());

      // Build a policy map
      auto pmkey = DA::policy_map_key_t("pm1", DA::POLICY_FEATURE_PBR);
      auto pm = DA::policy_map_t(pmkey);

      // Add the policy map rule matching our class map and dropping the traffic
      auto pmrule4 = DA::policy_map_rule_t(cmkey);
      auto action = DA::policy_map_action_t(DA::POLICY_ACTION_DROP);
      pmrule4.action_set(action);
      pm.rule_set(1, pmrule4);

      // Configure V6 ACL 
      auto acl2 = DA::acl_key_t("acl2", DA::ACL_TYPE_IPV6);
      auto rule2 = DA::acl_rule_ip_t();
      rule2.action_is(DA::ACL_PERMIT);
      DA::parse_ip_addr("2001:db8:1234::", &dip);
      rule2.destination_addr_is(DA::ip_addr_mask_t(dip, 48));
      rule2.priority_value_is(129);
      rule2.priority_mask_is(255);
      get_acl_mgr()->acl_rule_set(acl2, 1, rule2);
      get_acl_mgr()->acl_commit();

      // Now build the class map for that ACL and commit it
      auto cmkey2 = DA::class_map_key_t("cm2", DA::POLICY_FEATURE_PBR);
      auto cm2 = DA::class_map_t(cmkey2);
      cm2.rule_set(1, DA::class_map_rule_t(acl2));
      class_mgr->class_map_is(cm2);
      t.trace0("class map cm2 input: %s", cm2.to_string().c_str());
      auto cm2_res = class_mgr->class_map(cmkey2);
      t.trace0("class map cm2 outpt: %s", cm2_res.to_string().c_str());

      // Add the policy map rule matching our class map and dropping the traffic
      auto pmrule6 = DA::policy_map_rule_t(cmkey2);
      pmrule6.action_set(action);
      pm.rule_set(2, pmrule6);

      // Add a V4 single line raw match rule.
      DA::acl_rule_ip_t raw_v4;
      DA::parse_ip_addr("30.30.0.0", &dip);
      raw_v4.destination_addr_is(DA::ip_addr_mask_t(dip, 16));
      raw_v4.priority_value_is(32);
      raw_v4.match_ip_priority_is(true);
      auto pmrule_raw_v4 = DA::policy_map_rule_t();
      pmrule_raw_v4.raw_rule_is(raw_v4, DA::POLICY_RULE_TYPE_IPV4);
      action = DA::policy_map_action_t(DA::POLICY_ACTION_NEXTHOP_GROUP);
      action.nexthop_group_name_is("nexthopgroup2");
      pmrule_raw_v4.action_set(action);
      pm.rule_set(3, pmrule_raw_v4);

      // Add a V6 single line raw match rule.
      DA::acl_rule_ip_t raw_v6;
      DA::parse_ip_addr("2001:db8:5678::", &dip);
      raw_v6.destination_addr_is(DA::ip_addr_mask_t(dip, 48));
      raw_v6.priority_value_is(129);
      raw_v6.priority_mask_is(255);
      auto pmrule_raw_v6 = DA::policy_map_rule_t();
      pmrule_raw_v6.raw_rule_is(raw_v6, DA::POLICY_RULE_TYPE_IPV6);
      action = DA::policy_map_action_t(DA::POLICY_ACTION_NEXTHOP_GROUP);
      action.nexthop_group_name_is("nexthopgroup2");
      pmrule_raw_v6.action_set(action);
      pm.rule_set(4, pmrule_raw_v6);

      // Commit the policy map
      get_policy_map_mgr()->policy_map_is(pm);

      // Finally, apply the policy map to the management interface to block access
      get_policy_map_mgr()->policy_map_apply(pmkey,
                                             DA::intf_id_t("Management0"),
                                             DA::ACL_IN,
                                             true);
   }

 private:
   DA::class_map_mgr * class_mgr;
};

int main(int argc, char ** argv) {
   DA::sdk sdk;
   policy_demo demo(sdk);

   printf("Starting PolicyDemo\n");
   sdk.main_loop(argc, argv);
}
