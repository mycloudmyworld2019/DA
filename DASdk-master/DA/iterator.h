// Copyright (c) 2014 Arista Networks, Inc.  All rights reserved.
// Arista Networks, Inc. Confidential and Proprietary.

#ifndef DA_ITERATOR_H
#define DA_ITERATOR_H

#include <iterator>
#include <DA/base.h>

namespace DA {

template <typename T, typename Impl>
class DA_SDK_PUBLIC iter_base : public std::iterator<std::input_iterator_tag, T> {
 public:
   iter_base(iter_base const &);
   ~iter_base();
   iter_base & operator=(iter_base const & other);
   iter_base & operator++();
   iter_base operator++(int);
   bool operator==(iter_base const & rhs) const;
   bool operator!=(iter_base const & rhs) const;
   operator bool() const;
   T operator*() const;
   T const * operator->() const;
   T * operator->();

 protected:
   Impl * impl;
   explicit iter_base(Impl * const) DA_SDK_PRIVATE;
   iter_base() DA_SDK_PRIVATE;
};

}

#include <DA/inline/iterator.h>

#endif // DA_ITERATOR_H
