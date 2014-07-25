// bdldfp_decimalimputil.h                                            -*-C++-*-

#ifndef INCLUDED_BDLDFP_DECIMALIMPUTIL
#define INCLUDED_BDLDFP_DECIMALIMPUTIL

#ifndef INCLUDED_BSLS_IDENT
#include <bsls_ident.h>
#endif
BSLS_IDENT("$Id$")

//@PURPOSE: Provide a unified low-level interface for decimal floating point.
//
//@CLASSES:
//  bdldfp::DecimalImpUtil: Unified low level decimal floating point functions.
//
//@SEE ALSO: bdldfp_decimalimputil_inteldfp, bdldfp_decimalimputil_decnumber
//
//@DESCRIPTION: The 'bdldfp::DecimalImpUtil' component provides a unified
// namespace for utility functions for the decimal floating-point operations
// defined in lower level components.
//
///Usage
///-----
// This section shows the intended use of this component.
//
///Example 1: Constructing a representation of a value in decimal
/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Floating-point numbers are built from a sign, a significand and an exponent.
// All those 3 are integers (of various sizes), therefore it is possible to
// build decimals from integers:
//..
//  long long coefficient = 42; // Yet another name for significand
//  int exponent          = -1;
//
//  typedef bdldfp::DecimalImpUtil Util;
//
//  Util::ValueType32  d32  = Util::makeDecimal32( coefficient, exponent);
//  Util::ValueType64  d64  = Util::makeDecimal64( coefficient, exponent);
//  Util::ValueType128 d128 = Util::makeDecimal128(coefficient, exponent);
//
//  assert(Util::binaryToDecimal32( 4.2) == d32);
//  assert(Util::binaryToDecimal64( 4.2) == d64);
//  assert(Util::binaryToDecimal128(4.2) == d128);
//..

#ifndef INCLUDED_BDLDFP_DECIMALIMPUTIL_INTELDFP
#include <bdldfp_decimalimputil_inteldfp.h>
#endif

#ifndef INCLUDED_BDLDFP_DECIMALIMPUTIL_DECNUMBER
#include <bdldfp_decimalimputil_decnumber.h>
#endif

#ifndef INCLUDED_BDLDFP_DECIMALIMPUTIL_IBMXLC
#include <bdldfp_decimalimputil_ibmxlc.h>
#endif

#ifndef INCLUDED_BDLDFP_DECIMALPLATFORM
#include <bdldfp_decimalplatform.h>
#endif

#ifndef INCLUDED_BSLS_ASSERT
#include <bsls_assert.h>
#endif

#ifndef INCLUDED_BSL_ALGORITHM
#include <bsl_algorithm.h>
#endif

#ifdef BDLDFP_DECIMALPLATFORM_SOFTWARE

                // DECIMAL FLOATING-POINT LITERAL EMULATION


#define BDLDFP_DECIMALIMPUTIL_DF(lit)                                        \
    BloombergLP::bdldfp::DecimalImpUtil::parse32(                            \
        (BloombergLP::bdldfp::DecimalImpUtil::checkLiteral(lit), #lit))

#define BDLDFP_DECIMALIMPUTIL_DD(lit)                                        \
    BloombergLP::bdldfp::DecimalImpUtil::parse64(                            \
        (BloombergLP::bdldfp::DecimalImpUtil::checkLiteral(lit), #lit))

#define BDLDFP_DECIMALIMPUTIL_DL(lit)                                        \
    BloombergLP::bdldfp::DecimalImpUtil::parse128(                           \
        (BloombergLP::bdldfp::DecimalImpUtil::checkLiteral(lit), #lit))

#elif defined(BDLDFP_DECIMALPLATFORM_C99_TR) || defined( __IBM_DFP__ )

#define BDLDFP_DECIMALIMPUTIL_JOIN_(a,b) a##b

               // Portable decimal floating-point literal support

#define BDLDFP_DECIMALIMPUTIL_DF(lit) BDLDFP_DECIMALIMPUTIL_JOIN_(lit,df)

#define BDLDFP_DECIMALIMPUTIL_DD(lit) BDLDFP_DECIMALIMPUTIL_JOIN_(lit,dd)

#define BDLDFP_DECIMALIMPUTIL_DL(lit) BDLDFP_DECIMALIMPUTIL_JOIN_(lit,dl)

#endif


namespace BloombergLP {
namespace bdldfp {

class DecimalImpUtil
{
  public:
    typedef DecimalImpUtil_Platform::ValueType32  ValueType32;
    typedef DecimalImpUtil_Platform::ValueType64  ValueType64;
    typedef DecimalImpUtil_Platform::ValueType128 ValueType128;

    static ValueType64 makeDecimal64(                   int mantissa,
                                                        int exponent);
    static ValueType64 makeDecimal64(unsigned           int mantissa,
                                                        int exponent);
    static ValueType64 makeDecimal64(         long long int mantissa,
                                                        int exponent);
    static ValueType64 makeDecimal64(unsigned long long int mantissa,
                                                        int exponent);
        // Return a 'Decimal64' object that has the specified 'mantissa' and
        // 'exponent', rounded according to the current decimal rounding mode,
        // if necessary.  If an overflow condition occurs. store the value of
        // the macro 'ERANGE' into 'errno' and return infinity with the
        // appropriate sign.

    static ValueType64 makeInfinity64(bool isNegative = false);
        // Return a 'ValueType64' representing an infinity.  Optionally specify
        // whether the infinity 'isNegative'.  If 'isNegative' is not
        // specified, the infinite value will be positive.  Note that if
        // 'isNegative' is true, the infinite value will be negative, and
        // positive otherwise.

    // CLASS METHODS
#ifdef BDLDFP_DECIMALPLATFORM_SOFTWARE

    struct This_is_not_a_floating_point_literal {};
        // This 'struct' is a helper type used togenerate error messages for
        // bad literals.

    template <class TYPE>
    static void checkLiteral(const TYPE& t);
        // Generate an error if the specified 't' is bad decimal
        // floating-point.  Note that this function is intended for use with
        // literals

    static void checkLiteral(double);
        // Overload to avoid an error when the decimal floating-point literal
        // (without the suffix) can be interpreted as a 'double' literal.

#elif defined(BDLDFP_DECIMALPLATFORM_HARDWARE)

#else

#error Improperly configured decimal floating point platform settings

#endif

    // CLASS METHODS

                        // compose and decompose

    //static ValueType32  composeDecimal32 (DecimalTriple triple);
    //static ValueType64  composeDecimal64 (DecimalTriple triple);
    //static ValueType128 composeDecimal128(DecimalTriple triple);
        // Return a 'ValueTypeXX' number having the value as specified by the
        // salient attributes of the specified 'triple'.  The behavior is
        // undefined if the 'mantissa' has too many decimal digits for
        // 'ValueType', or the 'exponent' is too large for 'ValueType'

    //static DecimalTriple decomposeDecimal(ValueType32  value);
    //static DecimalTriple decomposeDecimal(ValueType64  value);
    //static DecimalTriple decomposeDecimal(ValueType128 value);
        // Return a 'DecimalTriple' object representing the salient attributes
        // of the specified 'value'.  The behavior is undefined, unless 'value'
        // is neither 'NaN' nor 'Inf'.

                        // Integer construction

    static ValueType32  int32ToDecimal32(                   int value);
    static ValueType32 uint32ToDecimal32(unsigned           int value);
    static ValueType32  int64ToDecimal32(         long long int value);
    static ValueType32 uint64ToDecimal32(unsigned long long int value);
        // Return a 'Decimal32' object having the value closest to the
        // specified 'value' following the conversion rules as defined by
        // IEEE-754:
        //
        //: o If 'value' is zero then initialize this object to a zero with an
        //:   unspecified sign and an unspecified exponent.
        //:
        //: o Otherwise if 'value' has an absolute value that is larger than
        //:   'std::numeric_limits<Decimal32>::max()' then raise the "overflow"
        //:   floating-point exception and initialize this object to infinity
        //:   with the same sign as 'other'.
        //:
        //: o Otherwise if 'value' has a value that is not exactly
        //:   representable using 'std::numeric_limits<Decimal32>::max_digit'
        //:   decimal digits then raise the "inexact" floating-point exception
        //:   and initialize this object to the value of 'other' rounded
        //:   according to the rounding direction.
        //:
        //: o Otherwise initialize this object to the value of the 'value'.
        //
        // The exponent 0 (quantum 1e-6) is preferred during conversion unless
        // it would cause unnecessary loss of precision.

    static ValueType64  int32ToDecimal64(                   int value);
    static ValueType64 uint32ToDecimal64(unsigned           int value);
    static ValueType64  int64ToDecimal64(         long long int value);
    static ValueType64 uint64ToDecimal64(unsigned long long int value);
        // Return a 'Decimal64' object having the value closest to the
        // specified 'value' following the conversion rules as defined by
        // IEEE-754:
        //
        //: o If 'value' is zero then initialize this object to a zero with an
        //:   unspecified sign and an unspecified exponent.
        //:
        //: o Otherwise if 'value' has an absolute value that is larger than
        //:   'std::numeric_limits<Decimal64>::max()' then raise the "overflow"
        //:   floating-point exception and initialize this object to infinity
        //:   with the same sign as 'other'.
        //:
        //: o Otherwise if 'value' has a value that is not exactly
        //:   representable using 'std::numeric_limits<Decimal64>::max_digit'
        //:   decimal digits then raise the "inexact" floating-point exception
        //:   and initialize this object to the value of 'other' rounded
        //:   according to the rounding direction.
        //:
        //: o Otherwise initialize this object to the value of the 'value'.
        //
        // The exponent 0 (quantum 1e-15) is preferred during conversion unless
        // it would cause unnecessary loss of precision.

    static ValueType128  int32ToDecimal128(                   int value);
    static ValueType128 uint32ToDecimal128(unsigned           int value);
    static ValueType128  int64ToDecimal128(         long long int value);
    static ValueType128 uint64ToDecimal128(unsigned long long int value);
        // Return a 'Decimal128' object having the value closest to the
        // specified 'value' subject to the conversion rules as defined by
        // IEEE-754:
        //
        //: o If 'value' is zero then initialize this object to a zero with an
        //:   unspecified sign and an unspecified exponent.
        //:
        //: o Otherwise if 'value' has an absolute value that is larger than
        //:   'std::numeric_limits<Decimal128>::max()' then raise the
        //:   "overflow" floating-point exception and initialize this object to
        //:   infinity with the same sign as 'other'.
        //:
        //: o Otherwise if 'value' has a value that is not exactly
        //:   representable using 'std::numeric_limits<Decimal128>::max_digit'
        //:   decimal digits then raise the "inexact" floating-point exception
        //:   and initialize this object to the value of 'value' rounded
        //:   according to the rounding direction.
        //:
        //: o Otherwise initialize this object to 'value'.
        //
        // The exponent 0 (quantum 1e-33) is preferred during conversion unless
        // it would cause unnecessary loss of precision.



                        // Arithmetic

    static ValueType64  add(ValueType64  lhs, ValueType64  rhs);
    static ValueType128 add(ValueType128 lhs, ValueType128 rhs);
        // Add the value of the specified 'rhs' to the value of the specified
        // 'lhs' as described by IEEE-754 and return the result.
        //
        //: o If either of 'lhs' or 'rhs' is NaN, then raise the "invalid"
        //:   floating-point exception and return a NaN.
        //:
        //: o Otherwise if 'lhs' and 'rhs' are infinities of differing signs,
        //:   raise the "invalid" floating-point exception and return a NaN.
        //:
        //: o Otherwise if 'lhs' and 'rhs' are infinities of the same sign then
        //:   return infinity of that sign.
        //:
        //: o Otherwise if 'rhs' is zero (positive or negative), return 'lhs'.
        //:
        //: o Otherwise if the sum of 'lhs' and 'rhs' has an absolute value
        //:   that is larger than 'std::numeric_limits<Decimal64>::max()' then
        //:   raise the "overflow" floating-point exception and return infinity
        //:   with the same sign as that result.
        //:
        //: o Otherwise return the sum of the number represented by 'lhs' and
        //:   the number represented by 'rhs'.

    static ValueType64  subtract(ValueType64  lhs, ValueType64  rhs);
    static ValueType128 subtract(ValueType128 lhs, ValueType128 rhs);
        // Subtract the value of the specified 'rhs' from the value of the
        // specified 'lhs' as described by IEEE-754 and return the result.
        //
        //: o If either 'lhs' or 'rhs' is NaN, then raise the "invalid"
        //:   floating-point exception and return a NaN.
        //:
        //: o Otherwise if 'lhs' and the 'rhs' have infinity values of the same
        //:   sign, then raise the "invalid" floating-point exception and
        //:   return a NaN.
        //:
        //: o Otherwise if 'lhs' and the 'rhs' have infinity values of
        //:   differing signs, then return 'lhs'.
        //:
        //: o Otherwise if 'rhs' has a zero value (positive or negative), then
        //:   return 'lhs'.
        //:
        //: o Otherwise if subtracting the value of the 'rhs' object from the
        //:   value of 'lhs' results in an absolute value that is larger than
        //:   'std::numeric_limits<Decimal64>::max()' then raise the "overflow"
        //:   floating-point exception and return infinity with the same sign
        //:   as that result.
        //:
        //: o Otherwise return the result of subtracting the value of 'rhs'
        //:   from the value of 'lhs'.

    static ValueType64  multiply(ValueType64  lhs, ValueType64  rhs);
    static ValueType128 multiply(ValueType128 lhs, ValueType128 rhs);
        // Multiply the value of the specified 'lhs' object by the value of the
        // specified 'rhs' as described by IEEE-754 and return the result.
        //
        //: o If either of 'lhs' or 'rhs' is NaN, return a NaN.
        //:
        //: o Otherwise if one of the operands is infinity (positive or
        //:   negative) and the other is zero (positive or negative), then
        //:   raise the "invalid" floating-point exception raised and return a
        //:   NaN.
        //:
        //: o Otherwise if both 'lhs' and 'rhs' are infinity (positive or
        //:   negative), return infinity.  The sign of the returned value will
        //:   be positive if 'lhs' and 'rhs' have the same sign, and negative
        //:   otherwise.
        //:
        //: o Otherwise, if either 'lhs' or 'rhs' is zero, return zero.  The
        //:   sign of the returned value will be positive if 'lhs' and 'rhs'
        //:   have the same sign, and negative otherwise.
        //:
        //: o Otherwise if the product of 'lhs' and 'rhs' has an absolute value
        //:   that is larger than 'std::numeric_limits<Decimal64>::max()' then
        //:   raise the "overflow" floating-point exception and return infinity
        //:   with the same sign as that result.
        //:
        //: o Otherwise if the product of 'lhs' and 'rhs' has an absolute value
        //:   that is smaller than 'std::numeric_limits<Decimal64>::min()' then
        //:   raise the "underflow" floating-point exception and return zero
        //:   with the same sign as that result.
        //:
        //: o Otherwise return the product of the value of 'rhs' and the number
        //:   represented by 'rhs'.

    static ValueType64  divide(ValueType64  lhs, ValueType64  rhs);
    static ValueType128 divide(ValueType128 lhs, ValueType128 rhs);
        // Divide the value of the specified 'lhs' by the value of the
        // specified 'rhs' as described by IEEE-754, and return the result.
        //
        //: o If 'lhs' or 'rhs' is NaN, raise the "invalid" floating-point
        //:   exception and return a NaN.
        //:
        //: o Otherwise if 'lhs' and 'rhs' are both infinity (positive or
        //:   negative) or both zero (positive or negative), raise the
        //:   "invalid" floating-point exception and return a NaN.
        //:
        //: o Otherwise if 'rhs' has a positive zero value, raise the
        //:   "overflow" floating-point exception and return infinity with the
        //:   sign of 'lhs'.
        //:
        //: o Otherwise if 'rhs' has a negative zero value, raise the
        //:   "overflow" floating-point exception and return infinity with the
        //:   opposite sign as 'lhs'.
        //:
        //: o Otherwise if dividing the value of 'lhs' with the value of 'rhs'
        //:   results in an absolute value that is larger than
        //:   'std::numeric_limits<Decimal64>::max()' then raise the "overflow"
        //:   floating-point exception and return infinity with the same sign
        //:   as that result.
        //:
        //: o Otherwise if dividing the value of 'lhs' with the value of 'rhs'
        //:   results in an absolute value that is smaller than
        //:   'std::numeric_limits<Decimal64>::min()' then raise the
        //:   "underflow" floating-point exception and return zero with the
        //:   same sign as that result.
        //:
        //: o Otherwise return the result of dividing the value of 'lhs' with
        //:   the value of 'rhs'.

    static ValueType32  negate(ValueType32  value);
    static ValueType64  negate(ValueType64  value);
    static ValueType128 negate(ValueType128 value);
        // Return the result of applying the unary - operator to the specified
        // 'value' as described by IEEE-754.  Note that floating-point numbers
        // have signed zero, therefore this operation is not the same as
        // '0-value'.

                        // Comparison

    static bool less(ValueType32  lhs, ValueType32  rhs);
    static bool less(ValueType64  lhs, ValueType64  rhs);
    static bool less(ValueType128 lhs, ValueType128 rhs);
        // Return 'true' if the specified 'lhs' has a value less than the
        // specified 'rhs' and 'false' otherwise.  The value of a 'Decimal64'
        // object 'lhs' is less than that of an object 'rhs' if the
        // 'compareQuietLess' operation (IEEE-754 defined, non-total ordering
        // comparison) considers the underlying IEEE representation of 'lhs' to
        // be less than of that of 'rhs'.  In other words, 'lhs' is less than
        // 'rhs' if:
        //
        //: o neither 'lhs' nor 'rhs' are NaN, or
        //: o 'lhs' is zero (positive or negative) and 'rhs' is positive, or
        //: o 'rhs' is zero (positive or negative) and 'lhs' negative, or
        //: o 'lhs' is not positive infinity, or
        //: o 'lhs' is negative infinity and 'rhs' is not, or
        //: o 'lhs' and 'rhs' both represent a real number and the real number
        //:   of 'lhs' is less than that of 'rhs'
        //
        // This operation raises the "invalid" floating-point exception if
        // either or both operands are NaN.

    static bool greater(ValueType32  lhs, ValueType32  rhs);
    static bool greater(ValueType64  lhs, ValueType64  rhs);
    static bool greater(ValueType128 lhs, ValueType128 rhs);
        // Return 'true' if the specified 'lhs' has a greater value than the
        // specified 'rhs' and 'false' otherwise.  The value of a 'Decimal64'
        // object 'lhs' is greater than that of an object 'rhs' if the
        // 'compareQuietGreater' operation (IEEE-754 defined, non-total
        // ordering comparison) considers the underlying IEEE representation of
        // 'lhs' to be greater than of that of 'rhs'.  In other words, 'lhs' is
        // greater than 'rhs' if:
        //
        //: o neither 'lhs' nor 'rhs' are NaN, or
        //: o 'rhs' is zero (positive or negative) and 'lhs' positive, or
        //: o 'lhs' is zero (positive or negative) and 'rhs' negative, or
        //: o 'lhs' is not negative infinity, or
        //: o 'lhs' is positive infinity and 'rhs' is not, or
        //: o 'lhs' and 'rhs' both represent a real number and the real number
        //:   of 'lhs' is greater than that of 'rhs'
        //
        // This operation raises the "invalid" floating-point exception if
        // either or both operands are NaN.

    static bool lessEqual(ValueType32  lhs, ValueType32  rhs);
    static bool lessEqual(ValueType64  lhs, ValueType64  rhs);
    static bool lessEqual(ValueType128 lhs, ValueType128 rhs);
        // Return 'true' if the specified 'lhs' has a value less than or equal
        // the value of the specified 'rhs' and 'false' otherwise.  The value
        // of a 'Decimal64' object 'lhs' is less than or equal to the value of
        // an object 'rhs' if the 'compareQuietLessEqual' operation (IEEE-754
        // defined, non-total ordering comparison) considers the underlying
        // IEEE representation of 'lhs' to be less or equal to that of 'rhs'.
        // In other words, 'lhs' is less or equal than 'rhs' if:
        //
        //: o neither 'lhs' nor 'rhs' are NaN, or
        //: o 'lhs' and 'rhs' are both zero (positive or negative), or
        //: o both 'lhs' and 'rhs' are positive infinity, or
        //: o 'lhs' is negative infinity, or
        //: o 'lhs' and 'rhs' both represent a real number and the real number
        //:   of 'lhs' is less or equal to that of 'rhs'
        //
        // This operation raises the "invalid" floating-point exception if
        // either or both operands are NaN.

    static bool greaterEqual(ValueType32  lhs, ValueType32  rhs);
    static bool greaterEqual(ValueType64  lhs, ValueType64  rhs);
    static bool greaterEqual(ValueType128 lhs, ValueType128 rhs);
        // Return 'true' if the specified 'lhs' has a value greater than or
        // equal to the value of the specified 'rhs' and 'false' otherwise.
        // The value of a 'Decimal64' object 'lhs' is greater or equal to a
        // 'Decimal64' object 'rhs' if the 'compareQuietGreaterEqual' operation
        // (IEEE-754 defined, non-total ordering comparison ) considers the
        // underlying IEEE representation of 'lhs' to be greater or equal to
        // that of 'rhs'.  In other words, 'lhs' is greater than or equal to
        // 'rhs' if:
        //
        //: o neither 'lhs' nor 'rhs' are NaN, or
        //: o 'lhs' and 'rhs' are both zero (positive or negative), or
        //: o both 'lhs' and 'rhs' are negative infinity, or
        //: o 'lhs' is positive infinity, or
        //: o 'lhs' and 'rhs' both represent a real number and the real number
        //:   of 'lhs' is greater or equal to that of 'rhs'
        //
        // This operation raises the "invalid" floating-point exception if
        // either or both operands are NaN.

    static bool equal(ValueType32  lhs, ValueType32  rhs);
    static bool equal(ValueType64  lhs, ValueType64  rhs);
    static bool equal(ValueType128 lhs, ValueType128 rhs);
        // Return 'true' if the specified 'lhs' and 'rhs' have the same value,
        // and 'false' otherwise.  Two decimal objects have the same value if
        // the 'compareQuietEqual' operation (IEEE-754 defined, non-total
        // ordering comparison) considers the underlying IEEE representations
        // equal.  In other words, two decimal objects have the same value if:
        //
        //: o both have a zero value (positive or negative), or
        //:
        //: o both have the same infinity value (both positive or negative), or
        //:
        //: o both have the value of a real number that are equal, even if they
        //:   are represented differently (cohorts have the same value)
        //
        // This operation raises the "invalid" floating-point exception if
        // either or both operands are NaN.

    static bool notEqual(ValueType32  lhs, ValueType32  rhs);
    static bool notEqual(ValueType64  lhs, ValueType64  rhs);
    static bool notEqual(ValueType128 lhs, ValueType128 rhs);
        // Return 'false' if the specified 'lhs' and 'rhs' have the same value,
        // and 'true' otherwise.  Two decimal objects have the same value if
        // the 'compareQuietEqual' operation (IEEE-754 defined, non-total
        // ordering comparison) considers the underlying IEEE representations
        // equal.  In other words, two decimal objects have the same value if:
        //
        //: o both have a zero value (positive or negative), or
        //:
        //: o both have the same infinity value (both positive or negative), or
        //:
        //: o both have the value of a real number that are equal, even if they
        //:   are represented differently (cohorts have the same value)
        //
        // This operation raises the "invalid" floating-point exception if
        // either or both operands are NaN.

    static ValueType32  convertToDecimal32 (const ValueType64&  input);
    static ValueType64  convertToDecimal64 (const ValueType32&  input);
    static ValueType64  convertToDecimal64 (const ValueType128& input);
    static ValueType128 convertToDecimal128(const ValueType32&  input);
    static ValueType128 convertToDecimal128(const ValueType64&  input);
        // Convert the specified 'input' to the indicated result type.  Note
        // that a conversion from 'ValueType128' to 'ValueType32' is not
        // provided (because such a conversion is not provided by the
        // 'decNumber' library).  A conversion from 128-bit to 32-bit
        // representations is *not* identical to the composing the conversions
        // from 128-bit to 64-bit, and 64-bit to 32-bit representations,
        // because rounding should only be performed once.

    static ValueType32  binaryToDecimal32(      float value);
    static ValueType32  binaryToDecimal32(     double value);
    static ValueType32  binaryToDecimal32(long double value);
        // Create a 'Decimal32' object having the value closest to the
        // specified 'value' following the conversion rules as defined by
        // IEEE-754:
        //
        //: o If 'value' is NaN, return a NaN.
        //:
        //: o Otherwise if 'value' is infinity (positive or negative), then
        //:   return an object equal to infinity with the same sign.
        //:
        //: o Otherwise if 'value' is a zero value, then return an object equal
        //:   to zero with the same sign.
        //:
        //: o Otherwise if 'value' has an absolute value that is larger than
        //:   'std::numeric_limits<Decimal32>::max()' then raise the "overflow"
        //:   floating-point exception and return an infinity with the same
        //:   sign as 'value'.
        //:
        //: o Otherwise if 'value' has an absolute value that is smaller than
        //:   'std::numeric_limits<Decimal32>::min()' then raise the
        //:   "underflow" floating-point exception and return a zero with the
        //:   same sign as 'value'.
        //:
        //: o Otherwise if 'value' needs more than
        //:   'std::numeric_limits<Decimal32>::max_digit' significant decimal
        //:   digits to represent then raise the "inexact" floating-point
        //:   exception and return the 'value' rounded according to the
        //:   rounding direction.
        //:
        //: o Otherwise return a 'Decimal32' object representing 'value'.

    static ValueType64  binaryToDecimal64(      float value);
    static ValueType64  binaryToDecimal64(     double value);
    static ValueType64  binaryToDecimal64(long double value);
        // Create a 'Decimal64' object having the value closest to the
        // specified 'value' following the conversion rules as defined by
        // IEEE-754:
        //
        //: o If 'value' is NaN, return a NaN.
        //:
        //: o Otherwise if 'value' is infinity (positive or negative), then
        //:   return an object equal to infinity with the same sign.
        //:
        //: o Otherwise if 'value' is a zero value, then return an object equal
        //:   to zero with the same sign.
        //:
        //: o Otherwise if 'value' has an absolute value that is larger than
        //:   'std::numeric_limits<Decimal64>::max()' then raise the "overflow"
        //:   floating-point exception and return an infinity with the same
        //:   sign as 'value'.
        //:
        //: o Otherwise if 'value' has an absolute value that is smaller than
        //:   'std::numeric_limits<Decimal64>::min()' then raise the
        //:   "underflow" floating-point exception and return a zero with the
        //:   same sign as 'value'.
        //:
        //: o Otherwise if 'value' needs more than
        //:   'std::numeric_limits<Decimal64>::max_digit' significant decimal
        //:   digits to represent then raise the "inexact" floating-point
        //:   exception and return the 'value' rounded according to the
        //:   rounding direction.
        //:
        //: o Otherwise return a 'Decimal64' object representing 'value'.

    static ValueType128 binaryToDecimal128(      float value);
    static ValueType128 binaryToDecimal128(     double value);
    static ValueType128 binaryToDecimal128(long double value);
        // Create a 'Decimal128' object having the value closest to the
        // specified 'value' following the conversion rules as defined by
        // IEEE-754:
        //
        //: o If 'value' is NaN, return a NaN.
        //:
        //: o Otherwise if 'value' is infinity (positive or negative), then
        //:   return an object equal to infinity with the same sign.
        //:
        //: o Otherwise if 'value' is a zero value, then return an object equal
        //:   to zero with the same sign.
        //:
        //: o Otherwise if 'value' has an absolute value that is larger than
        //:   'std::numeric_limits<Decimal128>::max()' then raise the
        //:   "overflow" floating-point exception and return an infinity with
        //:   the same sign as 'value'.
        //:
        //: o Otherwise if 'value' has an absolute value that is smaller than
        //:   'std::numeric_limits<Decimal128>::min()' then raise the
        //:   "underflow" floating-point exception and return a zero with the
        //:   same sign as 'value'.
        //:
        //: o Otherwise if 'value' needs more than
        //:   'std::numeric_limits<Decimal128>::max_digit' significant decimal
        //:   digits to represent then raise the "inexact" floating-point
        //:   exception and return the 'value' rounded according to the
        //:   rounding direction.
        //:
        //: o Otherwise return a 'Decimal128' object representing 'value'.

    static ValueType32  makeDecimalRaw32(int mantissa, int exponent);
        // Create a 'ValueType32' object representing a decimal floating point
        // number consisting of the specified 'mantissa' and 'exponent', with
        // the sign given by 'mantissa'.  The behavior is undefined unless
        // 'abs(mantissa) <= 9,999,999' and '-101 <= exponent <= 90'.

    static ValueType64 makeDecimalRaw64(unsigned long long mantissa,
                                        int                exponent);
    static ValueType64 makeDecimalRaw64(long long          mantissa,
                                        int                exponent);
    static ValueType64 makeDecimalRaw64(unsigned int       mantissa,
                                        int                exponent);
    static ValueType64 makeDecimalRaw64(int                mantissa,
                                        int                exponent);
        // Create a 'ValueType64' object representing a decimal floating point
        // number consisting of the specified 'mantissa' and 'exponent', with
        // the sign given by 'mantissa'.  The behavior is undefined unless
        // 'abs(mantissa) <= 9,999,999,999,999,999' and
        // '-398 <= exponent <= 369'.

    static ValueType128 makeDecimalRaw128(unsigned long long mantissa,
                                          int                exponent);
    static ValueType128 makeDecimalRaw128(long long          mantissa,
                                          int                exponent);
    static ValueType128 makeDecimalRaw128(unsigned int       mantissa,
                                          int                exponent);
    static ValueType128 makeDecimalRaw128(int                mantissa,
                                          int                exponent);
        // Create a 'ValueType128' object representing a decimal floating point
        // number consisting of the specified 'mantissa' and 'exponent', with
        // the sign given by 'mantissa'.  The behavior is undefined unless
        // '-6176 <= exponent <= 6111'.

    static ValueType32  scaleB(ValueType32  value, int exponent);
    static ValueType64  scaleB(ValueType64  value, int exponent);
    static ValueType128 scaleB(ValueType128 value, int exponent);
        // Return the result of multiplying the specified 'value' by ten raised
        // to the specified 'exponent'.  The quantum of 'value' is scaled
        // according to IEEE 754's 'scaleB' operations.  The result is
        // unspecified if 'value' is NaN or infinity.  The behavior is
        // undefined unless '-1999999997 <= y <= 99999999'.


    static ValueType32 parse32(const char *input);
        // Parse the specified 'input' string as a 32 bit decimal floating-
        // point value and return the result.  The parsing is as specified for
        // the 'strtod32' function in section 9.6 of the ISO/EIC TR 24732 C
        // Decimal Floating-Point Technical Report, except that it is
        // unspecified whether the NaNs returned are quiet or signaling.  The
        // behavior is undefined unless 'input' represents a valid 32 bit
        // decimal floating-point number in scientific or fixed notation, and
        // no unrelated characters precede (not even whitespace) that textual
        // representation and a terminating nul character immediately follows
        // it.  Note that this method does not guarantee the behavior of
        // ISO/EIC TR 24732 C when parsing NaN because the AIX compiler
        // intrinsics return a signaling NaN.

    static ValueType64 parse64(const char *input);
        // Parse the specified 'input' string as a 64 bit decimal floating-
        // point value and return the result.  The parsing is as specified for
        // the 'strtod64' function in section 9.6 of the ISO/EIC TR 24732 C
        // Decimal Floating-Point Technical Report, except that it is
        // unspecified whether the NaNs returned are quiet or signaling.  The
        // behavior is undefined unless 'input' represents a valid 64 bit
        // decimal floating-point number in scientific or fixed notation, and
        // no unrelated characters precede (not even whitespace) that textual
        // representation and a terminating nul character immediately follows
        // it.  Note that this method does not guarantee the behavior of
        // ISO/EIC TR 24732 C when parsing NaN because the AIX compiler
        // intrinsics return a signaling NaN.

    static ValueType128 parse128(const char *input);
        // Parse the specified 'input' string as a 128 bit decimal floating-
        // point value and return the result.  The parsing is as specified for
        // the 'strtod128' function in section 9.6 of the ISO/EIC TR 24732 C
        // Decimal Floating-Point Technical Report, except that it is
        // unspecified whether the NaNs returned are quiet or signaling.  The
        // behavior is undefined unless 'input' represents a valid 128 bit
        // decimal floating-point number in scientific or fixed notation, and
        // no unrelated characters precede (not even whitespace) that textual
        // representation and a terminating nul character immediately follows
        // it.  Note that this method does not guarantee the behavior of
        // ISO/EIC TR 24732 C when parsing NaN because the AIX compiler
        // intrinsics return a signaling NaN.
};

// ============================================================================
//                      INLINE FUNCTION DEFINITIONS
// ============================================================================

                          // --------------------
                          // class DecimalImpUtil
                          // --------------------

#ifdef BDLDFP_DECIMALPLATFORM_SOFTWARE

template <class TYPE>
inline
void DecimalImpUtil::checkLiteral(const TYPE& t)
{
    (void)static_cast<This_is_not_a_floating_point_literal>(t);
}

inline
void DecimalImpUtil::checkLiteral(double)
{
}
#endif


// CLASS METHODS

                    // Integer construction

inline
DecimalImpUtil::ValueType32
DecimalImpUtil::int32ToDecimal32(int value)
{
    return DecimalImpUtil_Platform::int32ToDecimal32(value);
}

inline
DecimalImpUtil::ValueType64
DecimalImpUtil::int32ToDecimal64(int value)
{
    return DecimalImpUtil_Platform::int32ToDecimal64(value);
}

inline
DecimalImpUtil::ValueType128
DecimalImpUtil::int32ToDecimal128(int value)
{
    return DecimalImpUtil_Platform::int32ToDecimal128(value);
}


inline
DecimalImpUtil::ValueType32
DecimalImpUtil::uint32ToDecimal32(unsigned int value)
{
    return DecimalImpUtil_Platform::uint32ToDecimal32(value);
}

inline
DecimalImpUtil::ValueType64
DecimalImpUtil::uint32ToDecimal64(unsigned int value)
{
    return DecimalImpUtil_Platform::uint32ToDecimal64(value);
}

inline
DecimalImpUtil::ValueType128
DecimalImpUtil::uint32ToDecimal128(unsigned int value)
{
    return DecimalImpUtil_Platform::uint32ToDecimal128(value);
}


inline
DecimalImpUtil::ValueType32
DecimalImpUtil::int64ToDecimal32(long long int value)
{
    return DecimalImpUtil_Platform::int64ToDecimal32(value);
}

inline
DecimalImpUtil::ValueType64
DecimalImpUtil::int64ToDecimal64(long long int value)
{
    return DecimalImpUtil_Platform::int64ToDecimal64(value);
}

inline
DecimalImpUtil::ValueType128
DecimalImpUtil::int64ToDecimal128(long long int value)
{
    return DecimalImpUtil_Platform::int64ToDecimal128(value);
}


inline
DecimalImpUtil::ValueType32
DecimalImpUtil::uint64ToDecimal32(unsigned long long int value)
{
    return DecimalImpUtil_Platform::uint64ToDecimal32(value);
}

inline
DecimalImpUtil::ValueType64
DecimalImpUtil::uint64ToDecimal64(unsigned long long int value)
{
    return DecimalImpUtil_Platform::uint64ToDecimal64(value);
}

inline
DecimalImpUtil::ValueType128
DecimalImpUtil::uint64ToDecimal128(unsigned long long int value)
{
    return DecimalImpUtil_Platform::uint64ToDecimal128(value);
}

                    // Arithmetic

inline
DecimalImpUtil::ValueType64
DecimalImpUtil::add(DecimalImpUtil::ValueType64 lhs,
                    DecimalImpUtil::ValueType64 rhs)
{
    return DecimalImpUtil_Platform::add(lhs, rhs);
}

inline DecimalImpUtil::ValueType128
DecimalImpUtil::add(DecimalImpUtil::ValueType128 lhs,
                    DecimalImpUtil::ValueType128 rhs)
{
    return DecimalImpUtil_Platform::add(lhs, rhs);
}


inline
DecimalImpUtil::ValueType64
DecimalImpUtil::subtract(DecimalImpUtil::ValueType64 lhs,
                         DecimalImpUtil::ValueType64 rhs)
{
    return DecimalImpUtil_Platform::subtract(lhs, rhs);
}

inline
DecimalImpUtil::ValueType128
DecimalImpUtil::subtract(DecimalImpUtil::ValueType128 lhs,
                         DecimalImpUtil::ValueType128 rhs)
{
    return DecimalImpUtil_Platform::subtract(lhs, rhs);
}


inline
DecimalImpUtil::ValueType64
DecimalImpUtil::multiply(DecimalImpUtil::ValueType64 lhs,
                         DecimalImpUtil::ValueType64 rhs)
{
    return DecimalImpUtil_Platform::multiply(lhs, rhs);
}

inline
DecimalImpUtil::ValueType128
DecimalImpUtil::multiply(DecimalImpUtil::ValueType128 lhs,
                         DecimalImpUtil::ValueType128 rhs)
{
    return DecimalImpUtil_Platform::multiply(lhs, rhs);
}


inline
DecimalImpUtil::ValueType64
DecimalImpUtil::divide(DecimalImpUtil::ValueType64 lhs,
                       DecimalImpUtil::ValueType64 rhs)
{
    return DecimalImpUtil_Platform::divide(lhs, rhs);
}

inline
DecimalImpUtil::ValueType128
DecimalImpUtil::divide(DecimalImpUtil::ValueType128 lhs,
                       DecimalImpUtil::ValueType128 rhs)
{
    return DecimalImpUtil_Platform::divide(lhs, rhs);
}


inline
DecimalImpUtil::ValueType32
DecimalImpUtil::negate(DecimalImpUtil::ValueType32 value)
{
    return DecimalImpUtil_Platform::negate(value);
}

inline
DecimalImpUtil::ValueType64
DecimalImpUtil::negate(DecimalImpUtil::ValueType64 value)
{
    return DecimalImpUtil_Platform::negate(value);
}

inline
DecimalImpUtil::ValueType128
DecimalImpUtil::negate(DecimalImpUtil::ValueType128 value)
{
    return DecimalImpUtil_Platform::negate(value);
}

                    // Comparison

inline
bool
DecimalImpUtil::less(DecimalImpUtil::ValueType32 lhs,
                     DecimalImpUtil::ValueType32 rhs)
{
    return DecimalImpUtil_Platform::less(lhs, rhs);
}

inline
bool
DecimalImpUtil::less(DecimalImpUtil::ValueType64 lhs,
                     DecimalImpUtil::ValueType64 rhs)
{
    return DecimalImpUtil_Platform::less(lhs, rhs);
}

inline
bool
DecimalImpUtil::less(DecimalImpUtil::ValueType128 lhs,
                     DecimalImpUtil::ValueType128 rhs)
{
    return DecimalImpUtil_Platform::less(lhs, rhs);
}


inline
bool
DecimalImpUtil::greater(DecimalImpUtil::ValueType32 lhs,
                        DecimalImpUtil::ValueType32 rhs)
{
    return DecimalImpUtil_Platform::greater(lhs, rhs);
}

inline
bool
DecimalImpUtil::greater(DecimalImpUtil::ValueType64 lhs,
                        DecimalImpUtil::ValueType64 rhs)
{
    return DecimalImpUtil_Platform::greater(lhs, rhs);
}

inline
bool
DecimalImpUtil::greater(DecimalImpUtil::ValueType128 lhs,
                        DecimalImpUtil::ValueType128 rhs)
{
    return DecimalImpUtil_Platform::greater(lhs, rhs);
}


inline
bool
DecimalImpUtil::lessEqual(DecimalImpUtil::ValueType32 lhs,
                          DecimalImpUtil::ValueType32 rhs)
{
    return DecimalImpUtil_Platform::lessEqual(lhs, rhs);
}

inline
bool
DecimalImpUtil::lessEqual(DecimalImpUtil::ValueType64 lhs,
                          DecimalImpUtil::ValueType64 rhs)
{
    return DecimalImpUtil_Platform::lessEqual(lhs, rhs);
}

inline
bool
DecimalImpUtil::lessEqual(DecimalImpUtil::ValueType128 lhs,
                          DecimalImpUtil::ValueType128 rhs)
{
    return DecimalImpUtil_Platform::lessEqual(lhs, rhs);
}


inline
bool
DecimalImpUtil::greaterEqual(DecimalImpUtil::ValueType32 lhs,
                             DecimalImpUtil::ValueType32 rhs)
{
    return DecimalImpUtil_Platform::greaterEqual(lhs, rhs);
}

inline
bool
DecimalImpUtil::greaterEqual(DecimalImpUtil::ValueType64 lhs,
                             DecimalImpUtil::ValueType64 rhs)
{
    return DecimalImpUtil_Platform::greaterEqual(lhs, rhs);
}

inline
bool
DecimalImpUtil::greaterEqual(DecimalImpUtil::ValueType128 lhs,
                             DecimalImpUtil::ValueType128 rhs)
{
    return DecimalImpUtil_Platform::greaterEqual(lhs, rhs);
}


inline
bool
DecimalImpUtil::equal(DecimalImpUtil::ValueType32 lhs,
                      DecimalImpUtil::ValueType32 rhs)
{
    return DecimalImpUtil_Platform::equal(lhs, rhs);
}

inline
bool
DecimalImpUtil::equal(DecimalImpUtil::ValueType64 lhs,
                      DecimalImpUtil::ValueType64 rhs)
{
    return DecimalImpUtil_Platform::equal(lhs, rhs);
}

inline
bool
DecimalImpUtil::equal(DecimalImpUtil::ValueType128 lhs,
                      DecimalImpUtil::ValueType128 rhs)
{
    return DecimalImpUtil_Platform::equal(lhs, rhs);
}


inline
bool
DecimalImpUtil::notEqual(DecimalImpUtil::ValueType32 lhs,
                         DecimalImpUtil::ValueType32 rhs)
{
    return DecimalImpUtil_Platform::notEqual(lhs, rhs);
}

inline
bool
DecimalImpUtil::notEqual(DecimalImpUtil::ValueType64 lhs,
                         DecimalImpUtil::ValueType64 rhs)
{
    return DecimalImpUtil_Platform::notEqual(lhs, rhs);
}

inline
bool
DecimalImpUtil::notEqual(DecimalImpUtil::ValueType128 lhs,
                         DecimalImpUtil::ValueType128 rhs)
{
    return DecimalImpUtil_Platform::notEqual(lhs, rhs);
}


inline
DecimalImpUtil::ValueType32
DecimalImpUtil::convertToDecimal32(const DecimalImpUtil::ValueType64& input)
{
    return DecimalImpUtil_Platform::convertToDecimal32(input);
}

inline
DecimalImpUtil::ValueType64
DecimalImpUtil::convertToDecimal64 (const DecimalImpUtil::ValueType32& input)
{
    return DecimalImpUtil_Platform::convertToDecimal64(input);
}

inline
DecimalImpUtil::ValueType64
DecimalImpUtil::convertToDecimal64(const DecimalImpUtil::ValueType128& input)
{
    return DecimalImpUtil_Platform::convertToDecimal64(input);
}

inline
DecimalImpUtil::ValueType128
DecimalImpUtil::convertToDecimal128(const DecimalImpUtil::ValueType32& input)
{
    return DecimalImpUtil_Platform::convertToDecimal128(input);
}

inline
DecimalImpUtil::ValueType128
DecimalImpUtil::convertToDecimal128(const DecimalImpUtil::ValueType64& input)
{
    return DecimalImpUtil_Platform::convertToDecimal128(input);
}


inline
DecimalImpUtil::ValueType32
DecimalImpUtil::binaryToDecimal32(float value)
{
    return DecimalImpUtil_Platform::binaryToDecimal32(value);
}

inline
DecimalImpUtil::ValueType32
DecimalImpUtil::binaryToDecimal32(double value)
{
    return DecimalImpUtil_Platform::binaryToDecimal32(value);
}

inline
DecimalImpUtil::ValueType32
DecimalImpUtil::binaryToDecimal32(long double value)
{
    return DecimalImpUtil_Platform::binaryToDecimal32(value);
}


inline
DecimalImpUtil::ValueType64
DecimalImpUtil::binaryToDecimal64(float value)
{
    return DecimalImpUtil_Platform::binaryToDecimal64(value);
}

inline
DecimalImpUtil::ValueType64
DecimalImpUtil::binaryToDecimal64(double value)
{
    return DecimalImpUtil_Platform::binaryToDecimal64(value);
}

inline
DecimalImpUtil::ValueType64
DecimalImpUtil::binaryToDecimal64(long double value)
{
    return DecimalImpUtil_Platform::binaryToDecimal64(value);
}


inline
DecimalImpUtil::ValueType128
DecimalImpUtil::binaryToDecimal128(float value)
{
    return DecimalImpUtil_Platform::binaryToDecimal128(value);
}

inline
DecimalImpUtil::ValueType128
DecimalImpUtil::binaryToDecimal128(double value)
{
    return DecimalImpUtil_Platform::binaryToDecimal128(value);
}

inline
DecimalImpUtil::ValueType128
DecimalImpUtil::binaryToDecimal128(long double value)
{
    return DecimalImpUtil_Platform::binaryToDecimal128(value);
}


inline
DecimalImpUtil::ValueType32
DecimalImpUtil::makeDecimalRaw32(int mantissa, int exponent)
{
    BSLS_ASSERT(-101     <= exponent);
    BSLS_ASSERT(            exponent <= 90);
    BSLS_ASSERT(bsl::max(mantissa, -mantissa) <= 9999999);
    return DecimalImpUtil_Platform::makeDecimalRaw32(mantissa, exponent);
}


inline
DecimalImpUtil::ValueType64
DecimalImpUtil::makeDecimalRaw64(unsigned long long mantissa, int exponent)
{
    BSLS_ASSERT(-398     <= exponent);
    BSLS_ASSERT(            exponent <= 369);
    BSLS_ASSERT(mantissa <= 9999999999999999LL);

    return DecimalImpUtil_Platform::makeDecimalRaw64(mantissa, exponent);
}

inline
DecimalImpUtil::ValueType64
DecimalImpUtil::makeDecimalRaw64(long long mantissa, int exponent)
{
    BSLS_ASSERT(-398     <= exponent);
    BSLS_ASSERT(            exponent <= 369);
    BSLS_ASSERT(std::max(mantissa, -mantissa) <= 9999999999999999LL);

    return DecimalImpUtil_Platform::makeDecimalRaw64(mantissa, exponent);
}

inline
DecimalImpUtil::ValueType64
DecimalImpUtil::makeDecimalRaw64(unsigned int mantissa, int exponent)
{
    BSLS_ASSERT(-398     <= exponent);
    BSLS_ASSERT(            exponent <= 369);

    return DecimalImpUtil_Platform::makeDecimalRaw64(mantissa, exponent);
}

inline
DecimalImpUtil::ValueType64
DecimalImpUtil::makeDecimalRaw64(int mantissa, int exponent)
{
    BSLS_ASSERT(-398     <= exponent);
    BSLS_ASSERT(            exponent <= 369);
    return DecimalImpUtil_Platform::makeDecimalRaw64(mantissa, exponent);
}


inline
DecimalImpUtil::ValueType128
DecimalImpUtil::makeDecimalRaw128(unsigned long long mantissa, int exponent)
{
    BSLS_ASSERT(-6176    <= exponent);
    BSLS_ASSERT(            exponent <= 6111);

    return DecimalImpUtil_Platform::makeDecimalRaw128(mantissa, exponent);
}

inline
DecimalImpUtil::ValueType128
DecimalImpUtil::makeDecimalRaw128(long long mantissa, int exponent)
{
    BSLS_ASSERT(-6176    <= exponent);
    BSLS_ASSERT(            exponent <= 6111);

    return DecimalImpUtil_Platform::makeDecimalRaw128(mantissa, exponent);
}

inline
DecimalImpUtil::ValueType128
DecimalImpUtil::makeDecimalRaw128(unsigned int mantissa, int exponent)
{
    BSLS_ASSERT(-6176    <= exponent);
    BSLS_ASSERT(            exponent <= 6111);

    return DecimalImpUtil_Platform::makeDecimalRaw128(mantissa, exponent);
}

inline
DecimalImpUtil::ValueType128
DecimalImpUtil::makeDecimalRaw128(int mantissa, int exponent)
{
    BSLS_ASSERT(-6176    <= exponent);
    BSLS_ASSERT(            exponent <= 6111);

    return DecimalImpUtil_Platform::makeDecimalRaw128(mantissa, exponent);
}


inline
DecimalImpUtil::ValueType32
DecimalImpUtil::scaleB(DecimalImpUtil::ValueType32 value, int exponent)
{
    return DecimalImpUtil_Platform::scaleB(value, exponent);
}

inline
DecimalImpUtil::ValueType64
DecimalImpUtil::scaleB(DecimalImpUtil::ValueType64 value, int exponent)
{
    return DecimalImpUtil_Platform::scaleB(value, exponent);
}

inline
DecimalImpUtil::ValueType128
DecimalImpUtil::scaleB(DecimalImpUtil::ValueType128 value, int exponent)
{
    return DecimalImpUtil_Platform::scaleB(value, exponent);
}


inline
DecimalImpUtil::ValueType32
DecimalImpUtil::parse32(const char *input)
{
    return DecimalImpUtil_Platform::parse32(input);
}

inline
DecimalImpUtil::ValueType64
DecimalImpUtil::parse64(const char *input)
{
    return DecimalImpUtil_Platform::parse64(input);
}

inline
DecimalImpUtil::ValueType128
DecimalImpUtil::parse128(const char *input)
{
    return DecimalImpUtil_Platform::parse128(input);
}

}  // close package namespace
}  // close enterprise namespace

#endif

// ----------------------------------------------------------------------------
// Copyright (C) 2014 Bloomberg L.P.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
// ----------------------------- END-OF-FILE ----------------------------------
