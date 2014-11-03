// test_arcsine_dist.cpp

// Copyright John Maddock 2014.
// Copyright  Paul A. Bristow 2014.

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

// Tests for the arcsine Distribution.

#include <pch.hpp> // Must be 1st include, and include_directory /libs/math/src/tr1/ is needed.

#ifdef _MSC_VER
#  pragma warning(disable: 4127) // Conditional expression is constant.
#  pragma warning (disable : 4996) // POSIX name for this item is deprecated.
#  pragma warning (disable : 4224) // Nonstandard extension used : formal parameter 'arg' was previously defined as a type.
#endif

#include <boost/math/concepts/real_concept.hpp> // for real_concept.
using ::boost::math::concepts::real_concept;

#include <boost/math/distributions/arcsine.hpp> // for arcsine_distribution.
using boost::math::arcsine_distribution;

#include <boost/math/constants/constants.hpp>
using boost::math::constants::one_div_root_two;

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp> // for test_main
#include <boost/test/floating_point_comparison.hpp> // for BOOST_CHECK_CLOSE_FRACTION

#include <cmath>

#include "test_out_of_range.hpp"

#include <iostream>
using std::cout;
using std::endl;
#include <limits>
using std::numeric_limits;

template <class RealType>
RealType informax()
{
  return ((std::numeric_limits<RealType>::has_infinity) ?
     std::numeric_limits<RealType>::infinity() : boost::math::tools::max_value<RealType>());
}

/*
template <class RealType>
void test_spot(
  RealType a,    // alpha a
  RealType b,    // arcsine b
  RealType x,    // Probability
  RealType P,    // CDF of arcsine(a, b)
  RealType Q,    // Complement of CDF
  RealType tol)  // Test tolerance.
{
  boost::math::arcsine_distribution<RealType> arcsine(a, b);
  BOOST_CHECK_CLOSE_FRACTION(cdf(aarcsine, x), P, tol);
  if ((P < 0.99) && (Q < 0.99))
  {  // We can only check this if P is not too close to 1,
    // so that we can guarantee that Q is free of error,
    // (and similarly for Q)
    BOOST_CHECK_CLOSE_FRACTION(
      cdf(complement(aarcsine, x)), Q, tol);
    if (x != 0)
    {
      BOOST_CHECK_CLOSE_FRACTION(
        quantile(aarcsine, P), x, tol);
    }
    else
    {
      // Just check quantile is very small:
      if ((std::numeric_limits<RealType>::max_exponent <= std::numeric_limits<double>::max_exponent)
        && (boost::is_floating_point<RealType>::value))
      {
        // Limit where this is checked: if exponent range is very large we may
        // run out of iterations in our root finding algorithm.
        BOOST_CHECK(quantile(aarcsine, P) < boost::math::tools::epsilon<RealType>() * 10);
      }
    } // if k
    if (x != 0)
    {
      BOOST_CHECK_CLOSE_FRACTION(quantile(complement(aarcsine, Q)), x, tol);
    }
    else
    {  // Just check quantile is very small:
      if ((std::numeric_limits<RealType>::max_exponent <= std::numeric_limits<double>::max_exponent) && (boost::is_floating_point<RealType>::value))
      {  // Limit where this is checked: if exponent range is very large we may
        // run out of iterations in our root finding algorithm.
        BOOST_CHECK(quantile(complement(aarcsine, Q)) < boost::math::tools::epsilon<RealType>() * 10);
      }
    } // if x
  }
} // template <class RealType> void test_spot
*/

template <class RealType> // Any floating-point type RealType.
void test_spots(RealType)
{
    // Basic sanity checks with 'known good' values.
    // so set tolerance to a few eps expressed as a fraction, or
    // few eps of type double expressed as a fraction,
    // whichever is the larger.

    RealType tolerance = (std::max)
      (boost::math::tools::epsilon<RealType>(),
      static_cast<RealType>(std::numeric_limits<double>::epsilon())); // 0 if real_concept.

    RealType max_value = boost::math::tools::max_value<RealType>();
    RealType epsilon = boost::math::tools::epsilon<RealType>();

    //cout << "Boost::math::tools::epsilon = " << boost::math::tools::epsilon<RealType>() << endl;
    //cout << "std::numeric_limits::epsilon = " << std::numeric_limits<RealType>::epsilon() << endl;

    tolerance *= 2; // Note: NO * 100 because tolerance is a fraction, NOT %.
    cout << "tolerance = " << tolerance << endl;

    using boost::math::arcsine_distribution;
    using  ::boost::math::cdf;
    using  ::boost::math::pdf;
    using  ::boost::math::complement;
    using  ::boost::math::quantile;

    // Basic sanity-check spot values.

    // Test values from Wolfram alpha, for example:
    // http://www.wolframalpha.com/input/?i=+N%5BPDF%5Barcsinedistribution%5B0%2C+1%5D%2C+0.5%5D%2C+50%5D
    // N[PDF[arcsinedistribution[0, 1], 0.5], 50]
    // 0.63661977236758134307553505349005744813783858296183

    arcsine_distribution<RealType> arcsine_01; // (Our) Standard arcsine.

    // PDF

    BOOST_CHECK_CLOSE_FRACTION(pdf(arcsine_01, 0.000001), static_cast<RealType>(318.31004533885312973989414360099118178698415543136L), tolerance);
    BOOST_CHECK_CLOSE_FRACTION(pdf(arcsine_01, 0.000005), static_cast<RealType>(142.35286456604168061345817902422241622116338936911L), tolerance);
    BOOST_CHECK_CLOSE_FRACTION(pdf(arcsine_01, 0.05), static_cast<RealType>(1.4605059227421865250256574657088244053723856445614L), tolerance);
    BOOST_CHECK_CLOSE_FRACTION(pdf(arcsine_01, 0.5), static_cast<RealType>(0.63661977236758134307553505349005744813783858296183L), tolerance);
    // Note loss of significance when x is near x_max.
    BOOST_CHECK_CLOSE_FRACTION(pdf(arcsine_01, 0.95), static_cast<RealType>(1.4605059227421865250256574657088244053723856445614L), 8 * tolerance); // Less accurate.
    BOOST_CHECK_CLOSE_FRACTION(pdf(arcsine_01, 0.999995), static_cast<RealType>(142.35286456604168061345817902422241622116338936911L), 50000 * tolerance); // Much less accurate.
    BOOST_CHECK_CLOSE_FRACTION(pdf(arcsine_01, 0.999999), static_cast<RealType>(318.31004533885312973989414360099118178698415543136L), 100000 * tolerance);// Even less accurate.

    // Extreme x.
    if (std::numeric_limits<RealType>::has_infinity)
    { //
      BOOST_CHECK_EQUAL(pdf(arcsine_01, 0), informax<RealType>()); //
      BOOST_CHECK_EQUAL(pdf(arcsine_01, 1), informax<RealType>()); //
    }

    BOOST_CHECK_CLOSE_FRACTION(pdf(arcsine_01, tolerance),
      1 /(sqrt(tolerance) * boost::math::constants::pi<RealType>()), 2 * tolerance); //
    BOOST_CHECK_CLOSE_FRACTION(pdf(arcsine_01, static_cast<RealType>(1) - tolerance),
      1 /(sqrt(tolerance) * boost::math::constants::pi<RealType>()), 2 * tolerance); //

    // CDF
    BOOST_CHECK_CLOSE_FRACTION(cdf(arcsine_01, 0.000001), static_cast<RealType>(0.00063661987847092448418377367957384866092127786060574L), tolerance);
    BOOST_CHECK_CLOSE_FRACTION(cdf(arcsine_01, 0.000005), static_cast<RealType>(0.0014235262731079289297302426454125318201831474507326L), tolerance);
    BOOST_CHECK_CLOSE_FRACTION(cdf(arcsine_01, 0.05), static_cast<RealType>(0.14356629312870627075094188477505571882161519989741L), tolerance);
    BOOST_CHECK_CLOSE_FRACTION(cdf(arcsine_01, 0.5), static_cast<RealType>(0.5L), tolerance); // Exact.
    BOOST_CHECK_CLOSE_FRACTION(cdf(arcsine_01, 0.95), static_cast<RealType>(0.85643370687129372924905811522494428117838480010259L), 2 * tolerance);
    // Values near unity should use the cdf complemented for better accuracy,
    BOOST_CHECK_CLOSE_FRACTION(cdf(arcsine_01, 0.999995), static_cast<RealType>(0.99857647372689207107026975735458746817981685254927L), 100 * tolerance); // Less accurate.
    BOOST_CHECK_CLOSE_FRACTION(cdf(arcsine_01, 0.999999), static_cast<RealType>(0.99936338012152907551581622632042615133907872213939L), 1000 * tolerance); // Less accurate.

    //  Complement CDF
    BOOST_CHECK_CLOSE_FRACTION(cdf(complement(arcsine_01, 0.000001)), static_cast<RealType>(1 - 0.00063661987847092448418377367957384866092127786060574L), 2 * tolerance);
    BOOST_CHECK_CLOSE_FRACTION(cdf(complement(arcsine_01, 0.000001)), static_cast<RealType>(0.99936338012152907551581622632043L), 2 * tolerance); //
    BOOST_CHECK_CLOSE_FRACTION(cdf(complement(arcsine_01, 0.05)), static_cast<RealType>(0.85643370687129372924905811522494428117838480010259L), tolerance);
    BOOST_CHECK_CLOSE_FRACTION(cdf(complement(arcsine_01, 0.5)), static_cast<RealType>(0.5L), tolerance); // Exact.
    // Some values near unity when complement is expected to be less accurate.
    BOOST_CHECK_CLOSE_FRACTION(cdf(complement(arcsine_01, 0.95)), static_cast<RealType>(0.14356629312870627075094188477505571882161519989741L), 8 * tolerance); // 2 for asin
    BOOST_CHECK_CLOSE_FRACTION(cdf(complement(arcsine_01, 0.999999)), static_cast<RealType>(1 - 0.99936338012152907551581622632042615133907872213939L), 1000000 * tolerance); // 10000 for asin, 1000000 for acos.

    // Quantile.

    // Check 1st, 2nd and 3rd quartiles.

    // N[PDF[arcsinedistribution[0, 1], 0.25], 50]
    // 0.73510519389572273268176866441729258852984864048885

    BOOST_CHECK_CLOSE_FRACTION(quantile(arcsine_01, static_cast<RealType>(0.25L)), static_cast<RealType>(0.14644660940672624L), tolerance);
    BOOST_CHECK_CLOSE_FRACTION(quantile(arcsine_01, static_cast<RealType>(0.5L)), 0.5, 2 * tolerance);  // probability = 0.5, x = 0.5
    BOOST_CHECK_CLOSE_FRACTION(quantile(arcsine_01, static_cast<RealType>(0.75L)), static_cast<RealType>(0.85355339059327373L), tolerance);

    // N[CDF[arcsinedistribution[0, 1], 0.05], 50]  == 0.14356629312870627075094188477505571882161519989741
    BOOST_CHECK_CLOSE_FRACTION(quantile(arcsine_01, static_cast<RealType>(0.14356629312870627075094188477505571882161519989741L)), 0.05, tolerance);

    // Quantile of complement.
    // N[1-CDF[arcsinedistribution[0, 1], 0.05], 50] == 0.85643370687129372924905811522494428117838480010259
    BOOST_CHECK_CLOSE_FRACTION(quantile(complement(arcsine_01, static_cast<RealType>(0.85643370687129372924905811522494428117838480010259L))), 0.05, tolerance);
    // N[sin^2[0.75 * pi/2],50] == 0.85355339059327376220042218105242451964241796884424
    BOOST_CHECK_CLOSE_FRACTION(quantile(complement(arcsine_01, static_cast<RealType>(0.25L))), static_cast<RealType>(0.85355339059327376220042218105242451964241796884424L), tolerance);
    BOOST_CHECK_CLOSE_FRACTION(quantile(complement(arcsine_01, static_cast<RealType>(0.5L))), 0.5, 2 * tolerance);  // probability = 0.5, x = 0.5
    BOOST_CHECK_CLOSE_FRACTION(quantile(complement(arcsine_01, static_cast<RealType>(0.75L))), static_cast<RealType>(0.14644660940672623779957781894757548035758203115576L), 2 * tolerance); // Less accurate.


    // N[CDF[arcsinedistribution[0, 1], 0.25], 5
    // 0.33333333333333333333333333333333333333333333333333
    BOOST_CHECK_CLOSE_FRACTION(quantile(arcsine_01, static_cast<RealType>(1) / 3), static_cast<RealType>(0.25L), 2 * tolerance);
    BOOST_CHECK_CLOSE_FRACTION(quantile(arcsine_01, static_cast<RealType>(0.5L)), 0.5, 2 * tolerance);  // probability = 0.5, x = 0.5
    BOOST_CHECK_CLOSE_FRACTION(quantile(arcsine_01, static_cast<RealType>(2) / 3), static_cast<RealType>(0.75L), tolerance);

    // Arcsine(-1, +1)    xmin = -1, x_max = +1  symmetric about zero.
    arcsine_distribution<RealType> as_m11(-1, +1);

    BOOST_CHECK_EQUAL(as_m11.x_min(), -1); //
    BOOST_CHECK_EQUAL(as_m11.x_max(), +1);
    BOOST_CHECK_EQUAL(mean(as_m11), 0); //
    BOOST_CHECK_EQUAL(median(as_m11), 0); //
    BOOST_CHECK_EQUAL(standard_deviation(as_m11), one_div_root_two<RealType>()); //

    BOOST_CHECK_EQUAL(variance(as_m11), 0.5); // 1 - (-1) = 2 ^ 2 = 4 /8 = 0.5
    BOOST_CHECK_EQUAL(skewness(as_m11), 0); //
    BOOST_CHECK_EQUAL(kurtosis_excess(as_m11), -1.5); // 3/2


    BOOST_CHECK_CLOSE_FRACTION(pdf(as_m11, 0.05), static_cast<RealType>(0.31870852113797122803869876869296281629727218095644L), tolerance);
    BOOST_CHECK_CLOSE_FRACTION(pdf(as_m11, 0.5), static_cast<RealType>(0.36755259694786136634088433220864629426492432024443L), tolerance);
    BOOST_CHECK_CLOSE_FRACTION(pdf(as_m11, 0.95), static_cast<RealType>(1.0194074882503562519812229448639426942621591013381L), 2 * tolerance); // Less accurate.

    BOOST_CHECK_CLOSE_FRACTION(cdf(as_m11, 0.05), static_cast<RealType>(0.51592213323666034437274347433261364289389772737836L), tolerance);
    BOOST_CHECK_CLOSE_FRACTION(cdf(as_m11, 0.5), static_cast<RealType>(0.66666666666666666666666666666666666666666666666667L), 2 * tolerance);
    BOOST_CHECK_CLOSE_FRACTION(cdf(as_m11, 0.95), static_cast<RealType>(0.89891737589574013042121018491729701360300248368629L), tolerance); //  Not less accurate.

    // Quantile
    BOOST_CHECK_CLOSE_FRACTION(quantile(as_m11, static_cast<RealType>(1) / 3), -static_cast<RealType>(0.5L), 2 * tolerance); // p = 1/3 x = -0.5
    BOOST_CHECK_SMALL(quantile(as_m11, static_cast<RealType>(0.5L)), 2 * tolerance);                             // p = 0.5, x = 0
    BOOST_CHECK_CLOSE_FRACTION(quantile(as_m11, static_cast<RealType>(2) / 3), +static_cast<RealType>(0.5L), 4 * tolerance);     // p = 2/3, x = +0.5


    // Arcsine(-2, -1) xmin = -2, x_max = -1  - Asymmetric both negative.
    arcsine_distribution<RealType> as_m2m1(-2, -1);

    BOOST_CHECK_EQUAL(as_m2m1.x_min(), -2); //
    BOOST_CHECK_EQUAL(as_m2m1.x_max(), -1);
    BOOST_CHECK_EQUAL(mean(as_m2m1), -1.5); // 1 / (1 + 1) = 1/2 exactly.
    BOOST_CHECK_EQUAL(median(as_m2m1), -1.5); // 1 / (1 + 1) = 1/2 exactly.
    BOOST_CHECK_EQUAL(variance(as_m2m1), 0.125);
    BOOST_CHECK_EQUAL(skewness(as_m2m1), 0); //
    BOOST_CHECK_EQUAL(kurtosis_excess(as_m2m1), -1.5); // 3/2

    BOOST_CHECK_CLOSE_FRACTION(pdf(as_m2m1, -1.95), static_cast<RealType>(1.4605059227421865250256574657088244053723856445614L), 4 * tolerance);
    BOOST_CHECK_CLOSE_FRACTION(pdf(as_m2m1, -1.5), static_cast<RealType>(0.63661977236758134307553505349005744813783858296183L), tolerance);
    BOOST_CHECK_CLOSE_FRACTION(pdf(as_m2m1, -1.05), static_cast<RealType>(1.4605059227421865250256574657088244053723856445614L), 4 * tolerance); // Less accurate.

    BOOST_CHECK_CLOSE_FRACTION(cdf(as_m2m1, -1.05), static_cast<RealType>(0.85643370687129372924905811522494428117838480010259L), tolerance);
    BOOST_CHECK_CLOSE_FRACTION(cdf(as_m2m1, -1.5), static_cast<RealType>(0.5L), tolerance);
    BOOST_CHECK_CLOSE_FRACTION(cdf(as_m2m1, -1.95), static_cast<RealType>(0.14356629312870627075094188477505571882161519989741L), 8 * tolerance); //  Not much less accurate.

    // Quantile
    BOOST_CHECK_CLOSE_FRACTION(quantile(as_m2m1, static_cast<RealType>(0.85643370687129372924905811522494428117838480010259L)), -static_cast<RealType>(1.05L), 2 * tolerance); //
    BOOST_CHECK_CLOSE_FRACTION(quantile(as_m2m1, static_cast<RealType>(0.5L)), -static_cast<RealType>(1.5L), 2 * tolerance);                             //
    BOOST_CHECK_CLOSE_FRACTION(quantile(as_m2m1, static_cast<RealType>(0.14356629312870627075094188477505571882161519989741L)), -static_cast<RealType>(1.95L), 4 * tolerance);     //

    BOOST_CHECK_CLOSE_FRACTION(quantile(complement(as_m2m1, static_cast<RealType>(0.14356629312870627075094188477505571882161519989741L))), -static_cast<RealType>(1.05L), 2 * tolerance); //
    BOOST_CHECK_CLOSE_FRACTION(quantile(as_m2m1, static_cast<RealType>(0.5L)), -static_cast<RealType>(1.5L), 2 * tolerance);                             //
    BOOST_CHECK_CLOSE_FRACTION(quantile(complement(as_m2m1, static_cast<RealType>(0.85643370687129372924905811522494428117838480010259L))), -static_cast<RealType>(1.95L), 4 * tolerance);

    // Tests that should throw:
    BOOST_CHECK_THROW(mode(arcsine_distribution<RealType>(static_cast<RealType>(0), static_cast<RealType>(1))), std::domain_error);
    // mode is undefined, and must throw domain_error!

    BOOST_CHECK_THROW( // For various bad arguments.
      pdf(
      arcsine_distribution<RealType>(static_cast<RealType>(+1), static_cast<RealType>(-1)), // min_x > max_x
      static_cast<RealType>(1)), std::domain_error);

    BOOST_CHECK_THROW(
      pdf(
      arcsine_distribution<RealType>(static_cast<RealType>(1), static_cast<RealType>(0)), // bad constructor parameters.
      static_cast<RealType>(1)), std::domain_error);

    BOOST_CHECK_THROW(
      pdf(
      arcsine_distribution<RealType>(static_cast<RealType>(1), static_cast<RealType>(-1)), // bad constructor parameters.
      static_cast<RealType>(1)), std::domain_error);

    BOOST_CHECK_THROW(
      pdf(
      arcsine_distribution<RealType>(static_cast<RealType>(1), static_cast<RealType>(1)), // equal constructor parameters.
      static_cast<RealType>(-1)), std::domain_error);

    BOOST_CHECK_THROW(
      pdf(
      arcsine_distribution<RealType>(static_cast<RealType>(0), static_cast<RealType>(1)), // bad x > 1.
      static_cast<RealType>(999)), std::domain_error);

    // Checks on things that are errors.

    // Construction with 'bad' parameters.
    BOOST_CHECK_THROW(arcsine_distribution<RealType>(+1, -1), std::domain_error);
    BOOST_CHECK_THROW(arcsine_distribution<RealType>(+1, 0), std::domain_error);

    arcsine_distribution<> dist;
    BOOST_CHECK_THROW(pdf(dist, -1), std::domain_error);
    BOOST_CHECK_THROW(cdf(dist, -1), std::domain_error);
    BOOST_CHECK_THROW(cdf(complement(dist, -1)), std::domain_error);
    BOOST_CHECK_THROW(quantile(dist, -1), std::domain_error);
    BOOST_CHECK_THROW(quantile(complement(dist, -1)), std::domain_error);
    BOOST_CHECK_THROW(quantile(dist, -1), std::domain_error);
    BOOST_CHECK_THROW(quantile(complement(dist, -1)), std::domain_error);

    // Various combinations of bad contructor and member function parameters.
    BOOST_CHECK_THROW(pdf(boost::math::arcsine_distribution<RealType>(0, 1), -1), std::domain_error);
    BOOST_CHECK_THROW(pdf(boost::math::arcsine_distribution<RealType>(-1, 1), +2), std::domain_error);
    BOOST_CHECK_THROW(quantile(boost::math::arcsine_distribution<RealType>(1, 1), -1), std::domain_error);
    BOOST_CHECK_THROW(quantile(boost::math::arcsine_distribution<RealType>(1, 1), 2), std::domain_error);

    // No longer allow any parameter to be NaN or inf, so all these tests should throw.
    if (std::numeric_limits<RealType>::has_quiet_NaN)
    {
      // Attempt to construct from non-finite parameters should throw.
      RealType nan = std::numeric_limits<RealType>::quiet_NaN();
      BOOST_CHECK_THROW(arcsine_distribution<RealType> w(nan), std::domain_error);
      BOOST_CHECK_THROW(arcsine_distribution<RealType> w(1, nan), std::domain_error);
      BOOST_CHECK_THROW(arcsine_distribution<RealType> w(nan, 1), std::domain_error);

      arcsine_distribution<RealType> w(RealType(-1), RealType(+1));
      // NaN parameters to member functions should throw.
      BOOST_CHECK_THROW(pdf(w, +nan), std::domain_error); // x = NaN
      BOOST_CHECK_THROW(cdf(w, +nan), std::domain_error); // x = NaN
      BOOST_CHECK_THROW(cdf(complement(w, +nan)), std::domain_error); // x = + nan
      BOOST_CHECK_THROW(quantile(w, +nan), std::domain_error); // p = + nan
      BOOST_CHECK_THROW(quantile(complement(w, +nan)), std::domain_error); // p = + nan
    } // has_quiet_NaN

    if (std::numeric_limits<RealType>::has_infinity)
    {
      // Attempt to construct from non-finite should throw.
      RealType inf = std::numeric_limits<RealType>::infinity();

      BOOST_CHECK_THROW(arcsine_distribution<RealType> w(inf), std::domain_error);
      BOOST_CHECK_THROW(arcsine_distribution<RealType> w(1, inf), std::domain_error);

      // Infinite parameters to member functions should throw.
      arcsine_distribution<RealType> w(RealType(0), RealType(1));
      BOOST_CHECK_THROW(arcsine_distribution<RealType> w(inf), std::domain_error);
      BOOST_CHECK_THROW(arcsine_distribution<RealType> w(1, inf), std::domain_error);
      BOOST_CHECK_THROW(pdf(w, +inf), std::domain_error); // x = inf
      BOOST_CHECK_THROW(cdf(w, +inf), std::domain_error); // x = inf
      BOOST_CHECK_THROW(cdf(complement(w, +inf)), std::domain_error); // x = + inf
      BOOST_CHECK_THROW(quantile(w, +inf), std::domain_error); // p = + inf
      BOOST_CHECK_THROW(quantile(complement(w, +inf)), std::domain_error); // p = + inf
    } // has_infinity

    // Error handling checks:
    check_out_of_range<boost::math::arcsine_distribution<RealType> >(-1, +1); // (All) valid constructor parameter values.
    // and range and non-finite.
  } // template <class RealType>void test_spots(RealType)

  BOOST_AUTO_TEST_CASE(test_main)
  {
    BOOST_MATH_CONTROL_FP;

    // Check that can generate arcsine distribution using one convenience methods:
    using boost::math::arcsine;

    arcsine_distribution<> arcsine_01; // Using default RealType double.
    // Note: NOT arcsine01() - or compiler will assume a function.

    arcsine as; // Using typedef for default standard arcsine.

    BOOST_CHECK_EQUAL(as.x_min(), 0); //
    BOOST_CHECK_EQUAL(as.x_max(), 1);
    BOOST_CHECK_EQUAL(mean(as), 0.5); // 1 / (1 + 1) = 1/2 exactly.
    BOOST_CHECK_EQUAL(median(as), 0.5); // 1 / (1 + 1) = 1/2 exactly.
    BOOST_CHECK_EQUAL(variance(as), 0.125); //0.125
    BOOST_CHECK_CLOSE_FRACTION(standard_deviation(as), one_div_root_two<double>() / 2, std::numeric_limits<double>::epsilon()); // 0.353553
    BOOST_CHECK_EQUAL(skewness(as), 0); //
    BOOST_CHECK_EQUAL(kurtosis_excess(as), -1.5); // 3/2
    BOOST_CHECK_EQUAL(support(as).first, 0); //
    BOOST_CHECK_EQUAL(range(as).first, 0); //
    BOOST_CHECK_THROW(mode(as), std::domain_error); //  Two modes at x_min and x_max, so throw instead.
   // BOOST_CHECK_THROW(arcsine_distribution<double>(+1, -1), std::domain_error); // min > max

     // (Parameter value, arbitrarily zero, only communicates the floating point type).
    test_spots(0.0F); // Test float.
    test_spots(0.0); // Test double.
    #ifndef BOOST_MATH_NO_LONG_DOUBLE_MATH_FUNCTIONS
      test_spots(0.0L); // Test long double.
      #if !BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x582))
        test_spots(boost::math::concepts::real_concept(0.)); // Test real concept.
      #endif
    #endif
  /*    */
  } // BOOST_AUTO_TEST_CASE( test_main )

  /*

  Sample Output is:

  1>  Running 1 test case...
  1>  Platform: Win32
  1>  Compiler: Microsoft Visual C++ version 12.0
  1>  STL     : Dinkumware standard library version 610
  1>  Boost   : 1.56.0
  1>  tolerance = 2.38419e-007
  1>  tolerance = 4.44089e-016
  1>  tolerance = 4.44089e-016
  1>  tolerance = 4.44089e-016
  1>
  1>  *** No errors detected



  */



