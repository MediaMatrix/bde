// bdldfp_decimalutil.cpp                                             -*-C++-*-
#include <bdldfp_decimalutil.h>
#include <bdldfp_decimalimputil_decnumber.h>

#include <bsls_ident.h>
BSLS_IDENT_RCSID(bdldfp_decimalutil_cpp,"$Id$ $CSID$")

#include <bdldfp_decimalplatform.h>
#include <bdldfp_decimalimputil.h>

#include <bsls_assert.h>
#include <bslmf_assert.h>

#include <bsl_cmath.h>
#include <bsl_cstring.h>
#include <errno.h>
#include <math.h>  // For the  FP_* macros

#ifdef BDLDFP_DECIMALPLATFORM_C99_TR
#  ifndef  __STDC_WANT_DEC_FP__
#    error __STDC_WANT_DEC_FP__ must be defined on the command line!
     BSLMF_ASSERT(false);
#  endif
#endif

#define BDLDFP_DISABLE_COMPILE BSLMF_ASSERT(false)

#ifdef BDLDFP_DECIMALPLATFORM_INTELDFP
#include <bdldfp_intelimpwrapper.h>
#endif


#include <errno.h>

namespace BloombergLP {
namespace bdldfp {


namespace {

bool isNanString(const char *str) {
    // Return 'true' if the specified 'str' represents a NaN value, and 'false'
    // otherwise.  Note that the IEEE 754 standard specifies sequence of
    // characters equivalent to "NaN" or "sNaN" except for case is a valid
    // representation of NaN.

    // For the sake of efficiency, we rely on the trick that any alphabetic
    // character [a-zA-Z] in ASCII encoding can be bit-wise 'or'ed with '_'
    // (0x20) to get the corresponding lower case character.

    bsl::size_t len = bsl::strlen(str);
    if (len < 3) {
        return false;
    }

    if ((str[0] | ' ') == 's') {
        ++str;
        if (len != 4) {
            return false;
        }
    }
    else if (len != 3) {
        return false;
    }

    return ((str[0] | ' ') == 'n' &&
            (str[1] | ' ') == 'a' &&
            (str[2] | ' ') == 'n');
}

}  // close unnamed namespace


                             // Creator functions


int DecimalUtil::parseDecimal32(Decimal32 *out, const char *str)
{
    BSLS_ASSERT(out != 0);
    BSLS_ASSERT(str != 0);

    Decimal32 d = DecimalImpUtil::parse32(str);
    if (isNan(d) && !isNanString(str)) {
        return -1;
    }
    *out = d;
    return 0;
}

int DecimalUtil::parseDecimal64(Decimal64 *out, const char *str)
{

    BSLS_ASSERT(out != 0);
    BSLS_ASSERT(str != 0);

    Decimal64 d = DecimalImpUtil::parse64(str);
    if (isNan(d) && !isNanString(str)) {
        return -1;
    }
    *out = d;
    return 0;
}

int DecimalUtil::parseDecimal128(Decimal128 *out, const char *str)
{
    BSLS_ASSERT(out != 0);
    BSLS_ASSERT(str != 0);

    Decimal128 d = DecimalImpUtil::parse128(str);

    if (isNan(d) && !isNanString(str)) {
        return -1;
    }
    *out = d;
    return 0;
}


int DecimalUtil::parseDecimal32(Decimal32 *out, const bsl::string& str)
{
    BSLS_ASSERT(out != 0);

    return parseDecimal32(out, str.c_str());
}
int DecimalUtil::parseDecimal64(Decimal64 *out, const bsl::string& str)
{
    BSLS_ASSERT(out != 0);

    return parseDecimal64(out, str.c_str());
}
int DecimalUtil::parseDecimal128(Decimal128 *out, const bsl::string& str)
{
    BSLS_ASSERT(out != 0);

    return parseDecimal128(out, str.c_str());
}

                                // Formatting functions

void DecimalUtil::format(Decimal32 value, bsl::string *out)
{
    char buffer[BDLDFP_DECIMALPLATFORM_SNPRINTF_BUFFER_SIZE];

    DenselyPackedDecimalImpUtil::StorageType32 dpdStorage;
    dpdStorage = DecimalImpUtil::convertToDPD(*value.data());

    DecimalImpUtil_DecNumber::ValueType32 dpdValue;
    bsl::memcpy(&dpdValue, &dpdStorage, sizeof(dpdValue));

    DecimalImpUtil_DecNumber::format(dpdValue, buffer);
    out->assign(buffer);
}

void DecimalUtil::format(Decimal64 value, bsl::string *out)
{
    char buffer[BDLDFP_DECIMALPLATFORM_SNPRINTF_BUFFER_SIZE];

    DenselyPackedDecimalImpUtil::StorageType64 dpdStorage;
    dpdStorage = DecimalImpUtil::convertToDPD(*value.data());

    DecimalImpUtil_DecNumber::ValueType64 dpdValue;
    bsl::memcpy(&dpdValue, &dpdStorage, sizeof(dpdValue));

    DecimalImpUtil_DecNumber::format(dpdValue, buffer);
    out->assign(buffer);
}

void DecimalUtil::format(Decimal128 value, bsl::string *out)
{
    char buffer[BDLDFP_DECIMALPLATFORM_SNPRINTF_BUFFER_SIZE];
    DenselyPackedDecimalImpUtil::StorageType128 dpdStorage;
    dpdStorage = DecimalImpUtil::convertToDPD(*value.data());

    DecimalImpUtil_DecNumber::ValueType128 dpdValue;
    bsl::memcpy(&dpdValue, &dpdStorage, sizeof(dpdValue));

    DecimalImpUtil_DecNumber::format(dpdValue, buffer);
    out->assign(buffer);
}
                        // classification functions

int DecimalUtil::classify(Decimal32 x)
{
    return DecimalImpUtil::classify(x.value());
}

int DecimalUtil::classify(Decimal64 x)
{
    return DecimalImpUtil::classify(x.value());
}

int DecimalUtil::classify(Decimal128 x)
{
return DecimalImpUtil::classify(x.value());
}

bool DecimalUtil::isFinite(Decimal32 x)
{
    int cl = classify(x);
    return cl != FP_INFINITE && cl != FP_NAN;
}
bool DecimalUtil::isFinite(Decimal64 x)
{
    int cl = classify(x);
    return cl != FP_INFINITE && cl != FP_NAN;
}
bool DecimalUtil::isFinite(Decimal128 x)
{
    int cl = classify(x);
    return cl != FP_INFINITE && cl != FP_NAN;
}


bool DecimalUtil::isInf(Decimal32 x)
{
    return classify(x) == FP_INFINITE;
}
bool DecimalUtil::isInf(Decimal64 x)
{
    return classify(x) == FP_INFINITE;
}
bool DecimalUtil::isInf(Decimal128 x)
{
    return classify(x) == FP_INFINITE;
}

bool DecimalUtil::isNan(Decimal32 x)
{
    return classify(x) == FP_NAN;
}
bool DecimalUtil::isNan(Decimal64 x)
{
    return classify(x) == FP_NAN;
}
bool DecimalUtil::isNan(Decimal128 x)
{
    return classify(x) == FP_NAN;
}

bool DecimalUtil::isNormal(Decimal32 x)
{
    return classify(x) == FP_NORMAL;
}
bool DecimalUtil::isNormal(Decimal64 x)
{
    return classify(x) == FP_NORMAL;
}
bool DecimalUtil::isNormal(Decimal128 x)
{
    return classify(x) == FP_NORMAL;
}

                           // Comparison functions

bool DecimalUtil::isUnordered(Decimal32 x, Decimal32 y)
{
    return isNan(x) || isNan(y);
}
bool DecimalUtil::isUnordered(Decimal64 x, Decimal64 y)
{
    return isNan(x) || isNan(y);
}
bool DecimalUtil::isUnordered(Decimal128 x, Decimal128 y)
{
    return isNan(x) || isNan(y);
}

                             // Quantum functions

// Decimal64 DecimalUtil::multiplyByPowerOf10(Decimal64 value, Decimal64 exponent)
// {
//     BSLS_ASSERT_SAFE(
//       makeDecimal64(-1999999997, 0) <= exponent);
//     BSLS_ASSERT_SAFE(                  exponent <= makeDecimal64(99999999, 0));

// #ifdef BDLDFP_DECIMALPLATFORM_C99_TR
//     const int intExponent = __d64_to_long_long(*exponent.data());
//     return scalblnd64(*value.data(), intExponent);
// #elif defined(BDLDFP_DECIMALPLATFORM_DPD)
//     Decimal64 result = value;
//     decDoubleScaleB(result.data(),
//                     value.data(),
//                     exponent.data(),
//                     DecimalImpUtil_DecNumber::getDecNumberContext());
//     return result;
// #elif defined(BDLDFP_DECIMALPLATFORM_INTELDFP)
//     _IDEC_flags flags;
//     value.data()->d_raw =
//         __bid64_scalbn(value.data()->d_raw,
//                        __bid64_to_int32_int(exponent.data()->d_raw, &flags),
//                        &flags);
//     return value;
// #else
// BDLDFP_DISABLE_COMPILE; // Unsupported platform
// #endif
// }

// Decimal128 DecimalUtil::multiplyByPowerOf10(Decimal128 value,
//                                             Decimal128 exponent)
// {
// #ifdef BDLDFP_DECIMALPLATFORM_C99_TR
//     const int intExponent = __d128_to_long_long(*exponent.data());
//     return scalblnd128(*value.data(), intExponent);
// #elif defined(BDLDFP_DECIMALPLATFORM_DPD)
//     Decimal128 result = value;
//     decQuadScaleB(result.data(),
//                   value.data(),
//                   exponent.data(),
//                   DecimalImpUtil_DecNumber::getDecNumberContext());
//     return result;
// #elif defined(BDLDFP_DECIMALPLATFORM_INTELDFP)
//     _IDEC_flags flags;
//     value.data()->d_raw =
//         __bid128_scalbn(value.data()->d_raw,
//                         __bid128_to_int32_int(exponent.data()->d_raw, &flags),
//                         &flags);
//     return value;
// #else
// BDLDFP_DISABLE_COMPILE; // Unsupported platform
// #endif
// }

int DecimalUtil::quantum(Decimal32 value)
{
    BSLS_ASSERT(!isInf(value));
    BSLS_ASSERT(!isNan(value));
    int          sign;
    unsigned int significand;
    int          exponent;
    decompose(&sign, &significand, &exponent, value);
    return exponent;
}

int DecimalUtil::quantum(Decimal64 value)
{
    BSLS_ASSERT(!isInf(value));
    BSLS_ASSERT(!isNan(value));
    int                 sign;
    bsls::Types::Uint64 significand;
    int                 exponent;
    decompose(&sign, &significand, &exponent, value);
    return exponent;
}

int DecimalUtil::quantum(Decimal128 value)
{
    BSLS_ASSERT(!isInf(value));
    BSLS_ASSERT(!isNan(value));
    int     sign;
    Uint128 significand;
    int     exponent;
    decompose(&sign, &significand, &exponent, value);
    return exponent;
}

int DecimalUtil::decompose(int          *sign,
                           unsigned int *significand,
                           int          *exponent,
                           Decimal32     value)
{
    return  DecimalImpUtil::decompose(sign,
                                      significand,
                                      exponent,
                                      value.value());
}

int DecimalUtil::decompose(int                 *sign,
                           bsls::Types::Uint64 *significand,
                           int                 *exponent,
                           Decimal64            value)
{
    return  DecimalImpUtil::decompose(sign,
                                      significand,
                                      exponent,
                                      value.value());
}

int DecimalUtil::decompose(int                 *sign,
                           Uint128             *significand,
                           int                 *exponent,
                           Decimal128           value)
{
    return  DecimalImpUtil::decompose(sign,
                                      significand,
                                      exponent,
                                      value.value());

}

}  // close package namespace
}  // close enterprise namespace

// ----------------------------------------------------------------------------
// Copyright 2014 Bloomberg Finance L.P.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// ----------------------------- END-OF-FILE ----------------------------------
