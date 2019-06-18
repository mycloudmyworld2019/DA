// Copyright (c) 2013 Arista Networks, Inc.  All rights reserved.
// Arista Networks, Inc. Confidential and Proprietary.

#ifndef DA_MPLS_H
#define DA_MPLS_H

#include <DA/base.h>
#include <DA/types/mpls.h>

/**
 * @file
 * MPLS related type definitions.
 */

namespace DA {

/**
 * MPLS reserved label definitions (0..15).
 * See http://www.iana.org/assignments/mpls-label-values/mpls-label-values.xhtml
 */
static mpls_label_t const mpls_label_explicit_null_v4 = mpls_label_t(0);
static mpls_label_t const mpls_label_router_alert = mpls_label_t(1);
static mpls_label_t const mpls_label_explicit_null_v6 = mpls_label_t(2);
static mpls_label_t const mpls_label_implicit_null = mpls_label_t(3);
// 4..6 unassigned
static mpls_label_t const mpls_label_entropy_indicator = mpls_label_t(7);
// 8..12 unassigned
static mpls_label_t const mpls_label_gal = mpls_label_t(13);
static mpls_label_t const mpls_label_oam_alert = mpls_label_t(14);
// 15 unassigned

}  // end namespace DA

#endif // DA_MPLS_H
