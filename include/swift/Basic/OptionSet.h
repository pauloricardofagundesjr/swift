//===--- OptionSet - Option Set Utility -------------------------*- C++ -*-===//
//
// This source file is part of the Swift.org open source project
//
// Copyright (c) 2014 - 2015 Apple Inc. and the Swift project authors
// Licensed under Apache License v2.0 with Runtime Library Exception
//
// See http://swift.org/LICENSE.txt for license information
// See http://swift.org/CONTRIBUTORS.txt for the list of Swift project authors
//
//===----------------------------------------------------------------------===//
//
//  This file defines the OptionSet class template.
//
//===----------------------------------------------------------------------===//

#ifndef SWIFT_BASIC_OPTIONSET_H
#define SWIFT_BASIC_OPTIONSET_H

#include "llvm/ADT/None.h"

#include <type_traits>

namespace swift {

using llvm::None;

/// The class template \c OptionSet captures a set of options stored as the
/// bits in an unsigned integral value.
///
/// Each option corresponds to a particular flag value in the provided
/// enumeration type (\c Flags). The option set provides ways to add options,
/// remove options, intersect sets, etc., providing a thin type-safe layer
/// over the underlying unsigned value.
///
/// \tparam Flags An enumeration type that provides the individual flags
/// for options. Each enumerator should have a power-of-two value, indicating
/// which bit it is associated with.
///
/// \tparam StorageType The unsigned integral type to use to store the flags
/// enabled within this option set. This defaults to the unsigned form of the
/// underlying type of the enumeration.
template<typename Flags,
         typename StorageType = typename std::make_unsigned<
                                  typename std::underlying_type<Flags>::type
                                 >::type>
class OptionSet {
  StorageType Storage;

public:
  /// Create an empty option set.
  OptionSet() : Storage() { }

  /// Create an empty option set.
  OptionSet(llvm::NoneType) : Storage() { }

  /// Create an option set with only the given option set.
  OptionSet(Flags flag) : Storage(static_cast<StorageType>(flag)) { }

  /// Create an option set from raw storage.
  explicit OptionSet(StorageType storage) : Storage(storage) { }

  /// Check whether an option set is non-empty.
  explicit operator bool() const { return Storage != 0; }

  /// Explicitly convert an option set to its underlying storage.
  explicit operator StorageType() const { return Storage; }

  /// Determine whether this option set contains all of the options in the
  /// given set.
  bool contains(OptionSet set) const {
    return static_cast<bool>(*this & set);
  }

  /// Produce the union of two option sets.
  friend OptionSet operator|(OptionSet lhs, OptionSet rhs) {
    return OptionSet(lhs.Storage | rhs.Storage);
  }

  /// Produce the union of two option sets.
  friend OptionSet &operator|=(OptionSet &lhs, OptionSet rhs) {
    lhs.Storage |= rhs.Storage;
    return lhs;
 }

  /// Produce the intersection of two option sets.
  friend OptionSet operator&(OptionSet lhs, OptionSet rhs) {
    return OptionSet(lhs.Storage & rhs.Storage);
  }

  /// Produce the intersection of two option sets.
  friend OptionSet &operator&=(OptionSet &lhs, OptionSet rhs) {
    lhs.Storage &= rhs.Storage;
    return lhs;
  }

  /// Produce the complement of the given option set.
  friend OptionSet operator~(OptionSet set) {
    return OptionSet(~set.Storage);
  }

  /// Produce the difference of two option sets.
  friend OptionSet operator-(OptionSet lhs, OptionSet rhs) {
    return OptionSet(lhs.Storage & ~rhs.Storage);
  }

  /// Produce the intersection of two option sets.
  friend OptionSet &operator-=(OptionSet &lhs, OptionSet rhs) {
    lhs.Storage &= ~rhs.Storage;
    return lhs;
  }
};

}

#endif
