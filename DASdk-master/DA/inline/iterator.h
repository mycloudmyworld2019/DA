// Copyright (c) 2014 Arista Networks, Inc.  All rights reserved.
// Arista Networks, Inc. Confidential and Proprietary.

#ifndef DA_INLINE_ITERATOR_H
#define DA_INLINE_ITERATOR_H

namespace DA {

template <typename T, typename Impl>
inline iter_base<T, Impl>
iter_base<T, Impl>::operator++(int) {
   iter_base<T, Impl> me = *this;
   ++(*this);
   return me;
}

template <typename T, typename Impl>
inline bool
iter_base<T, Impl>::operator!=(iter_base<T, Impl> const & rhs) const {
   return !(*this == rhs);
}

}

#endif // DA_INLINE_ITERATOR_H
