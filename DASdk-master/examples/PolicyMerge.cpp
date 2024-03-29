// Copyright (c) 2014 Arista Networks, Inc.  All rights reserved.
// Arista Networks, Inc. Confidential and Proprietary.
#include <DA/acl.h>
#include <DA/agent.h>
#include <DA/class_map.h>
#include <DA/intf.h>
#include <DA/policy_map.h>
#include <DA/sdk.h>
#include <DA/tracing.h>

static const std::string AGENT_NAME = "PolicyMerge";

/* This example shows how to configure a PBR policy, access the rules
 * in an existing policy configuration and register for policy change
 * notifications using DASdk APIs. Two policies "pm1" and "pm2" are
 * configured and applied to interfaces. Another policy "pm3" is
 * constructed by merging the rules in the other two policies. When
 * "pm1" or "pm2" changes the overridable policy config change
 * notification handler on_policy_map_config_set() is invoked. This
 * example overrides on_policy_map_config_set() method to reconstruct
 * "pm3" when "pm1" or "pm2" is modified.
 */
class policy_merge : public DA::agent_handler,
                     public DA::acl_handler,
                     public DA::policy_map_handler {
 public:
   explicit policy_merge(DA::sdk & sdk)
         : DA::agent_handler(sdk.get_agent_mgr()),
           DA::acl_handler(sdk.get_acl_mgr()),
           DA::policy_map_handler(sdk.get_policy_map_mgr()),
           t(AGENT_NAME.c_str()) {

      // Setup manager refs.
      class_mgr = sdk.get_class_map_mgr();
      t.trace0("%s: Agent constructed.", __FUNCTION__);
   }

   ~policy_merge() {
   }

   void on_initialized() {
      t.trace0("%s: Policy_merge starting.", __FUNCTION__);

      // Register for updates to both "pm1" and "pm2".
      DA::policy_map_key_t pmkey;
      pmkey = DA::policy_map_key_t("pm1", DA::POLICY_FEATURE_PBR);
      watch_policy_map(pmkey, true);
      pmkey = DA::policy_map_key_t("pm2", DA::POLICY_FEATURE_PBR);
      watch_policy_map(pmkey, true);

      // Configure policy "pm1" and register for updates to "pm1".
      setup_policy_pm1();

      // Configure policy "pm2" and register for updates to "pm2".
      setup_policy_pm2();

      // Apply policy "pm3" on an interface.
      apply_policy_pm3();
   }

   // Policy update notification. This handler is invoked when a policy
   // being watched is modified. Get the rules from both policies "pm1"
   // and "pm2" and re-construct the policy "pm3" by merging the rules.
   void on_policy_map_config_set(DA::policy_map_key_t const & key) {
      t.trace0("%s: Policy %s changed.", __FUNCTION__, key.name().c_str());

      auto pmkey3 = DA::policy_map_key_t("pm3", DA::POLICY_FEATURE_PBR);
      auto pm3 = DA::policy_map_t(pmkey3);
      uint32_t seq = 0;

      // Construct policy-map key and perform lookup with the key to get
      // policy_map_t.
      auto pmkey1 = DA::policy_map_key_t("pm1", DA::POLICY_FEATURE_PBR);
      DA::policy_map_t pm1 = get_policy_map_mgr()->policy_map(pmkey1);

      // Get existing rules in the policy "pm1".
      std::map<uint32_t, DA::policy_map_rule_t> pmap1_rules = pm1.rules();

      // Iterate over the rules in the policy "pm1" and add them to "pm3".
      for (auto iter = pmap1_rules.begin(); iter != pmap1_rules.end(); iter++) {
         seq++;
         pm3.rule_set(seq, iter->second);
      }

      // Construct policy-map key and perform lookup with the key to get 
      // policy_map_t.
      auto pmkey2 = DA::policy_map_key_t("pm2", DA::POLICY_FEATURE_PBR);
      DA::policy_map_t pm2 = get_policy_map_mgr()->policy_map(pmkey2);

      // Get existing rules in the policy "pm2".
      std::map<uint32_t, DA::policy_map_rule_t> pmap2_rules = pm2.rules();

      // Iterate over the rules in the policy "pm2" and add them to "pm3".
      for (auto iter = pmap2_rules.begin(); iter != pmap2_rules.end(); iter++) {
         seq++;
         pm3.rule_set(seq, iter->second);
      }

      // Commit the modified policy "pm3".
      get_policy_map_mgr()->policy_map_is(pm3);
   }

   void on_agent_option(std::string const & option_name,
                        std::string const & value) {
      // One of the user configurable option has changed.
      t.trace0("%s: option_name=%s, value=%s.", __FUNCTION__,
               option_name.c_str(), value.c_str());
      if (option_name == "update_policy") {
         if (value == "pm1") {
            // Add one more rule to "pm1".
            update_policy_pm1();
         } else if (value == "pm2") {
            // Add one more rule to "pm2".
            update_policy_pm2();
         }
      }
   }

   // Configure PBR policy "pm1".
   void setup_policy_pm1() {

      // Configure V4 ACL.
      auto acl1 = DA::acl_key_t("acl1", DA::ACL_TYPE_IPV4);
      auto rule1 = DA::acl_rule_ip_t();
      rule1.action_is(DA::ACL_PERMIT);
      DA::ip_addr_t dip;
      DA::parse_ip_addr("20.20.0.0", &dip);
      rule1.destination_addr_is(DA::ip_addr_mask_t(dip, 16));
      rule1.priority_value_is(32);
      rule1.match_ip_priority_is(true);
      get_acl_mgr()->acl_rule_set(acl1, 1, rule1);
      get_acl_mgr()->acl_commit();

      // Now build the class map for that ACL and commit it.
      auto cmkey = DA::class_map_key_t("cm1", DA::POLICY_FEATURE_PBR);
      auto cm = DA::class_map_t(cmkey);
      cm.rule_set(1, DA::class_map_rule_t(acl1));
      class_mgr->class_map_is(cm);

      // Build policy-map
      auto pmkey = DA::policy_map_key_t("pm1", DA::POLICY_FEATURE_PBR);
      auto pm = DA::policy_map_t(pmkey);

      // Add the policy-map rule matching our class map and dropping the traffic.
      auto pmrule4 = DA::policy_map_rule_t(cmkey);
      auto action = DA::policy_map_action_t(DA::POLICY_ACTION_DROP);
      pmrule4.action_set(action);
      pm.rule_set(1, pmrule4);

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
      pm.rule_set(2, pmrule_raw_v4);

      // Add a V6 single line raw match rule.
      DA::acl_rule_ip_t raw_v6;
      DA::parse_ip_addr("2001:db8:100::", &dip);
      raw_v6.destination_addr_is(DA::ip_addr_mask_t(dip, 48));
      raw_v6.priority_value_is(129);
      raw_v6.priority_mask_is(255);
      auto pmrule_raw_v6 = DA::policy_map_rule_t();
      pmrule_raw_v6.raw_rule_is(raw_v6, DA::POLICY_RULE_TYPE_IPV6);
      action = DA::policy_map_action_t(DA::POLICY_ACTION_NEXTHOP_GROUP);
      action.nexthop_group_name_is("nexthopgroup2");
      pmrule_raw_v6.action_set(action);
      pm.rule_set(3, pmrule_raw_v6);

      // Commit the policy.
      get_policy_map_mgr()->policy_map_is(pm);

      // Finally, apply the policy to an interface to block access.
      get_policy_map_mgr()->policy_map_apply(pmkey,
                                             DA::intf_id_t("Ethernet1"),
                                             DA::ACL_IN,
                                             true);
   }

   // Add one more rule to "pm1".
   void update_policy_pm1() {
      // Construct policy-map key and perform lookup with the key to get
      // policy_map_t.
      auto pmkey = DA::policy_map_key_t("pm1", DA::POLICY_FEATURE_PBR);
      DA::policy_map_t pm = get_policy_map_mgr()->policy_map(pmkey);

      // Configure V6 ACL.
      auto acl2 = DA::acl_key_t("acl2", DA::ACL_TYPE_IPV6);
      auto rule2 = DA::acl_rule_ip_t();
      rule2.action_is(DA::ACL_PERMIT);
      DA::ip_addr_t dip;
      DA::parse_ip_addr("2001:db8:200::", &dip);
      rule2.destination_addr_is(DA::ip_addr_mask_t(dip, 48));
      rule2.priority_value_is(129);
      rule2.priority_mask_is(255);
      get_acl_mgr()->acl_rule_set(acl2, 1, rule2);
      get_acl_mgr()->acl_commit();

      // Build the class map for the new ACL and commit it.
      auto cmkey2 = DA::class_map_key_t("cm2", DA::POLICY_FEATURE_PBR);
      auto cm2 = DA::class_map_t(cmkey2);
      cm2.rule_set(1, DA::class_map_rule_t(acl2));
      class_mgr->class_map_is(cm2);

      // Add the policy map rule matching our class map and dropping the
      // traffic.
      auto pmrule6 = DA::policy_map_rule_t(cmkey2);
      auto action = DA::policy_map_action_t(DA::POLICY_ACTION_DROP);
      pmrule6.action_set(action);
      pm.rule_set(4, pmrule6);

      // Commit the policy.
      get_policy_map_mgr()->policy_map_is(pm);

      t.trace0("%s: Added one more rule to pm1.", __FUNCTION__);
   }

   // Configure PBR policy "pm2".
   void setup_policy_pm2() {

      // Configure V4 ACL
      auto acl1 = DA::acl_key_t("acl3", DA::ACL_TYPE_IPV4);
      auto rule1 = DA::acl_rule_ip_t();
      DA::ip_addr_t dip;
      DA::parse_ip_addr("40.40.0.0", &dip);
      rule1.destination_addr_is(DA::ip_addr_mask_t(dip, 16));
      rule1.priority_value_is(32);
      rule1.match_ip_priority_is(true);
      rule1.action_is(DA::ACL_PERMIT);
      get_acl_mgr()->acl_rule_set(acl1, 1, rule1);
      get_acl_mgr()->acl_commit();

      // Build the class map for the new ACL and commit it.
      auto cmkey = DA::class_map_key_t("cm3", DA::POLICY_FEATURE_PBR);
      auto cm = DA::class_map_t(cmkey);
      cm.rule_set(1, DA::class_map_rule_t(acl1));
      class_mgr->class_map_is(cm);

      // Build policy-map.
      auto pmkey = DA::policy_map_key_t("pm2", DA::POLICY_FEATURE_PBR);
      auto pm = DA::policy_map_t(pmkey);

      // Add the policy-map rule matching our class-map and dropping the
      // traffic.
      auto pmrule4 = DA::policy_map_rule_t(cmkey);
      auto action = DA::policy_map_action_t(DA::POLICY_ACTION_DROP);
      pmrule4.action_set(action);
      pm.rule_set(1, pmrule4);

      // Commit the policy.
      get_policy_map_mgr()->policy_map_is(pm);

      // Finally, apply the policy to an interface to block access.
      get_policy_map_mgr()->policy_map_apply(pmkey,
                                             DA::intf_id_t("Ethernet2"),
                                             DA::ACL_IN,
                                             true);
   }

   // Add one more rule to "pm2".
   void update_policy_pm2() {
      // Construct policy-map key and perform lookup with the key to get
      // policy_map_t.
      auto pmkey = DA::policy_map_key_t("pm2", DA::POLICY_FEATURE_PBR);
      DA::policy_map_t pm = get_policy_map_mgr()->policy_map(pmkey);

      // Construct V6 ACL rule.
      DA::acl_rule_ip_t raw_v6;
      DA::ip_addr_t dip;
      DA::parse_ip_addr("2002:db8:300::", &dip);
      raw_v6.destination_addr_is(DA::ip_addr_mask_t(dip, 48));
      raw_v6.priority_value_is(129);
      raw_v6.priority_mask_is(255);

      // Construct V6 raw-match policy rule.
      auto pmrule_raw_v6 = DA::policy_map_rule_t();
      pmrule_raw_v6.raw_rule_is(raw_v6, DA::POLICY_RULE_TYPE_IPV6);
      auto action = DA::policy_map_action_t(DA::POLICY_ACTION_NEXTHOP_GROUP);
      action.nexthop_group_name_is("nexthopgroup2");
      pmrule_raw_v6.action_set(action);

      // Add raw-match policy rule to the policy.
      pm.rule_set(2, pmrule_raw_v6);

      // Commit the policy.
      get_policy_map_mgr()->policy_map_is(pm);

      t.trace0("%s: Added one more rule to pm2.", __FUNCTION__);
   }

   // Configure PBR policy "pm3" and apply it on an interface.
   void apply_policy_pm3() {
      auto pmkey = DA::policy_map_key_t("pm3", DA::POLICY_FEATURE_PBR);
      get_policy_map_mgr()->policy_map_apply(pmkey,
                                             DA::intf_id_t("Ethernet3"),
                                             DA::ACL_IN,
                                             true);
      t.trace0("%s: Configured PBR policy pm3 and applied it on Ethernet3.",
               __FUNCTION__);
   }

 private:
   DA::tracer t;
   DA::class_map_mgr * class_mgr;
};

int main(int argc, char ** argv) {
   DA::sdk sdk;
   policy_merge policy_merge(sdk);
   sdk.main_loop(argc, argv);
}
