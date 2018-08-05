// ===================================================================
// Copied and modified from Google's Protocol Buffers library.
// google/protobuf/stub/common.h
//
// Protocol Buffers - Google's data interchange format
// Copyright 2008 Google Inc.  All rights reserved.
// http://code.google.com/p/protobuf/
//
// ===================================================================
#ifndef ARGUS_COMMON_SCOPED_PTR_H_
#define ARGUS_COMMON_SCOPED_PTR_H_

#include <boost/scoped_ptr.hpp>
#include <boost/scoped_array.hpp>


namespace argus {
namespace common {

using boost::scoped_ptr;
using boost::scoped_array;

} // namespace common
} // namespace argus

#endif // ARGUS_COMMON_SCOPED_PTR_H_

