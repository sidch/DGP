//============================================================================
//
// DGP: Digital Geometry Processing toolkit
// Copyright (C) 2016, Siddhartha Chaudhuri
//
// This software is covered by a BSD license. Portions derived from other
// works are covered by their respective licenses. For full licensing
// information see the LICENSE.txt file.
//
//============================================================================

#ifndef __DGP_CommonEnums_hpp__
#define __DGP_CommonEnums_hpp__

#include "Platform.hpp"
#include "EnumClass.hpp"
#include "NumericTypes.hpp"

namespace DGP {

/** Coordinate axis-aligned directions upto 4D (enum class). */
struct DGP_API AxisAlignedDirection
{
  /** Supported values. */
  enum Value
  {
    POS_X = 0,  ///< The positive X direction.
    NEG_X,      ///< The negative X direction.
    POS_Y,      ///< The positive Y direction.
    NEG_Y,      ///< The negative Y direction.
    POS_Z,      ///< The positive Z direction.
    NEG_Z,      ///< The negative Z direction.
    POS_W,      ///< The positive W direction.
    NEG_W       ///< The negative W direction.
  };

  DGP_ENUM_CLASS_BODY(AxisAlignedDirection)

  DGP_ENUM_CLASS_STRINGS_BEGIN(AxisAlignedDirection)
    DGP_ENUM_CLASS_STRING(POS_X,  "+X")
    DGP_ENUM_CLASS_STRING(NEG_X,  "-X")
    DGP_ENUM_CLASS_STRING(POS_Y,  "+Y")
    DGP_ENUM_CLASS_STRING(NEG_Y,  "-Y")
    DGP_ENUM_CLASS_STRING(POS_Z,  "+Z")
    DGP_ENUM_CLASS_STRING(NEG_Z,  "-Z")
    DGP_ENUM_CLASS_STRING(POS_W,  "+W")
    DGP_ENUM_CLASS_STRING(NEG_W,  "-W")
  DGP_ENUM_CLASS_STRINGS_END(AxisAlignedDirection)
};

/** Comparison operators (enum class). */
struct DGP_API CompareOp
{
  /** Supported values. */
  enum Value
  {
    EQUAL,      ///< Equality.
    NOT_EQUAL,  ///< Inequality.
    LESS,       ///< Less-than.
    LEQUAL,     ///< Less-than-or-equal.
    GREATER,    ///< Greater-than.
    GEQUAL      ///< Greater-than-or-equal.
  };

  DGP_ENUM_CLASS_BODY(CompareOp)

  DGP_ENUM_CLASS_STRINGS_BEGIN(CompareOp)
    DGP_ENUM_CLASS_STRING(EQUAL,      "==")
    DGP_ENUM_CLASS_STRING(NOT_EQUAL,  "!=")
    DGP_ENUM_CLASS_STRING(LESS,       "<")
    DGP_ENUM_CLASS_STRING(LEQUAL,     "<=")
    DGP_ENUM_CLASS_STRING(GREATER,    ">")
    DGP_ENUM_CLASS_STRING(GEQUAL,     ">=")
  DGP_ENUM_CLASS_STRINGS_END(CompareOp)
};

/** Common distance metrics. */
struct DistanceType
{
  /** Supported values. */
  enum Value
  {
    EUCLIDEAN,  ///< Euclidean distance.
    GEODESIC    ///< Geodesic distance on a surface.
  };

  DGP_ENUM_CLASS_BODY(DistanceType)

  DGP_ENUM_CLASS_STRINGS_BEGIN(DistanceType)
    DGP_ENUM_CLASS_STRING(EUCLIDEAN,  "euclidean")
    DGP_ENUM_CLASS_STRING(GEODESIC,   "geodesic")
  DGP_ENUM_CLASS_STRINGS_END(DistanceType)
};

/** %Endianness values (little-endian and big-endian) (enum class). Also has a function to check the machine endianness. */
struct DGP_API Endianness
{
  /** Supported values. */
  enum Value
  {
    BIG,
    LITTLE
  };

  DGP_ENUM_CLASS_BODY(Endianness)

  DGP_ENUM_CLASS_STRINGS_BEGIN(Endianness)
    DGP_ENUM_CLASS_STRING(BIG,    "big-endian")
    DGP_ENUM_CLASS_STRING(LITTLE, "little-endian")
  DGP_ENUM_CLASS_STRINGS_END(Endianness)

  /** Get the machine endian-ness. */
  static Endianness machine()
  {
    union
    {
      uint32 i;
      char c[4];
    } b = { 0x01020304 };

    return (b.c[0] == 1 ? BIG : LITTLE);
  }
};

} // namespace DGP

#endif
