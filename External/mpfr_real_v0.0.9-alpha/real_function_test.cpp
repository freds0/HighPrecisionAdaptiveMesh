//
// SHORT DESCRIPTION
// =================
//
// mpfr::real is a C++ interface to the GNU MPFR library
// version 3.0.0 or later.
//
// COPYRIGHT/LICENSE
// =================
//
// Copyright 2010,2011,2012 Christian Schneider <software(at)chschneider(dot)eu>
//
// Version: 0.0.9-alpha
//
// This file is part of mpfr::real.
//
// mpfr::real is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, version 3 of the License, NOT any later
// version.
//
// mpfr::real is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with mpfr::real.  If not, see <http://www.gnu.org/licenses/>.
//

// ////////////////////////////////////////////////////////////////////
// redirect MPFR functions and collect debugging output
// ////////////////////////////////////////////////////////////////////

#include <sstream>

std::stringstream debug_sstream;

// this overwrites the template function in "mpfr_header_wrapper.hpp"
namespace mpfr {
  void debug_func(const char* s) {
    debug_sstream << s << ",";
  }
}  // namespace mpfr

std::string debug_out() {
  return debug_sstream.str();
}

void debug_clear() {
  debug_sstream.str("");
}

// ////////////////////////////////////////////////////////////////////
// include (further) headers
// ////////////////////////////////////////////////////////////////////

#include "mpfr_header_wrapper.hpp"
#include "real.hpp"
#include <iomanip>

// ////////////////////////////////////////////////////////////////////
// convert type to string
// ////////////////////////////////////////////////////////////////////

template <typename type>
const char* type2char(const type&) {
  return "unknown";
}

const char* type2char(const bool&) {
  return "bool";
}

const char* type2char(const unsigned long int&) {
  return "unsigned long int";
}

const char* type2char(const long int&) {
  return "long int";
}

const char* type2char(const unsigned int&) {
  return "unsigned int";
}

const char* type2char(const int&) {
  return "int";
}

const char* type2char(const unsigned short int&) {
  return "unsigned short int";
}

const char* type2char(const short int&) {
  return "short int";
}

const char* type2char(const unsigned char&) {
  return "unsigned char";
}

const char* type2char(const signed char&) {
  return "signed char";
}

const char* type2char(const char&) {
  return "char";
}

const char* type2char(const double&) {
  return "double";
}

const char* type2char(const float&) {
  return "float";
}

const char* type2char(const long double&) {
  return "long double";
}

const char* type2char(const mpz_t&) {
  return "mpz_t";
}

const char* type2char(const mpq_t&) {
  return "mpq_t";
}

const char* type2char(const mpf_t&) {
  return "mpf_t";
}

const char* type2char(const mpfr_t&) {
  return "mpfr_t";
}

const char* type2char(const mpz_ptr&) {
  return "mpz_ptr";
}

const char* type2char(const mpq_ptr&) {
  return "mpq_ptr";
}

const char* type2char(const mpf_ptr&) {
  return "mpf_ptr";
}

const char* type2char(const mpfr_ptr&) {
  return "mpfr_ptr";
}

const char* type2char(const mpz_srcptr&) {
  return "mpz_srcptr";
}

const char* type2char(const mpq_srcptr&) {
  return "mpq_srcptr";
}

const char* type2char(const mpf_srcptr&) {
  return "mpf_srcptr";
}

const char* type2char(const mpfr_srcptr&) {
  return "mpfr_srcptr";
}

const char* type2char(const std::string&) {
  return "std::string";
}

const char* type2char(const char*&) {
  return "char*";
}

const char* type2char(const mpfr::real<512>&) {
  return "mpfr::real<512>";
}

const char* type2char(const mpfr::real<1024>&) {
  return "mpfr::real<1024>";
}

const char* type2char(const mpfr::real<2048>&) {
  return "mpfr::real<2048>";
}

// ////////////////////////////////////////////////////////////////////
// initialize GMP/MPFR types
// ////////////////////////////////////////////////////////////////////

void my_mp_init(mpf_ptr m, double val) {
  mpf_init(m);
  mpf_set_d(m, val);
}

void my_mp_init(mpq_ptr m, double val) {
  mpq_init(m);
  mpq_set_si(m, static_cast<long int>(val), 1);
}

void my_mp_init(mpz_ptr m, double val) {
  mpz_init(m);
  mpz_set_d(m, val);
}

void my_mp_init(mpfr_ptr m, double val) {
  mpfr_init(m);
  mpfr_set_d(m, val, MPFR_RNDN);
}

void my_mp_clear(mpf_ptr m) {
  mpf_clear(m);
}

void my_mp_clear(mpq_ptr m) {
  mpq_clear(m);
}

void my_mp_clear(mpz_ptr m) {
  mpz_clear(m);
}

void my_mp_clear(mpfr_ptr m) {
  mpfr_clear(m);
}

// ////////////////////////////////////////////////////////////////////
// import functions from "std" namespace
// ////////////////////////////////////////////////////////////////////

using std::abs;
using std::fpclassify;
using std::isfinite;
using std::isgreater;
using std::isgreaterequal;
using std::isless;
using std::islessequal;
using std::islessgreater;
using std::isnormal;
using std::isunordered;
using std::signbit;

// ////////////////////////////////////////////////////////////////////
// POSIX 2008 math.h functions
// ////////////////////////////////////////////////////////////////////

#ifndef _MSC_VER
#define MPFR_REAL_MATH_H_POSIX
#endif  // MPFR_REAL_MATH_H_POSIX

// ////////////////////////////////////////////////////////////////////
// main function
// ////////////////////////////////////////////////////////////////////

int main () {
  std::cout.flags(std::ios_base::scientific);

  std::cout << "# function    "
            << "[ type a1    value a1    "
            << "[ type a2    value a2    "
            << "[ type a3    value a3 ] ] ]    "
            << "num. result    return type    "
            << "called MPFR functions" << std::endl;

  // //////////////////////////////////////////////////////////////////
  // functions without mpfr::real arguments
  // //////////////////////////////////////////////////////////////////

  {
    std::cout << std::setw(38) << std::left
              << "mpfr::zero<1024, MPFR_RNDN>(+1)"
              << std::right << std::setw(17)
              << mpfr::zero<1024, MPFR_RNDN>(+1);
    debug_clear();
    std::cout << std::setw(20)
              << type2char(mpfr::zero<1024, MPFR_RNDN>(+1));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    std::cout << std::setw(38) << std::left
              << "mpfr::zero<1024, MPFR_RNDN>(-1)"
              << std::right << std::setw(17)
              << mpfr::zero<1024, MPFR_RNDN>(-1);
    debug_clear();
    std::cout << std::setw(20)
              << type2char(mpfr::zero<1024, MPFR_RNDN>(-1));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    std::cout << std::setw(38) << std::left
              << "mpfr::inf<1024, MPFR_RNDN>(+1)"
              << std::right << std::setw(17)
              << mpfr::inf<1024, MPFR_RNDN>(+1);
    debug_clear();
    std::cout << std::setw(20)
              << type2char(mpfr::inf<1024, MPFR_RNDN>(+1));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    std::cout << std::setw(38) << std::left
              << "mpfr::inf<1024, MPFR_RNDN>(-1)"
              << std::right << std::setw(17)
              << mpfr::inf<1024, MPFR_RNDN>(-1);
    debug_clear();
    std::cout << std::setw(20)
              << type2char(mpfr::inf<1024, MPFR_RNDN>(-1));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    std::cout << std::setw(38) << std::left
              << "mpfr::nan<1024, MPFR_RNDN>(\"\")"
              << std::right << std::setw(17)
              << mpfr::nan<1024, MPFR_RNDN>("");
    debug_clear();
    std::cout << std::setw(20)
              << type2char(mpfr::nan<1024, MPFR_RNDN>(""));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    std::cout << std::setw(38) << std::left
              << "mpfr::const_log2<1024, MPFR_RNDN>()"
              << std::right << std::setw(17)
              << mpfr::const_log2<1024, MPFR_RNDN>();
    debug_clear();
    std::cout << std::setw(20)
              << type2char(mpfr::const_log2<1024, MPFR_RNDN>());
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    std::cout << std::setw(38) << std::left
              << "mpfr::const_pi<1024, MPFR_RNDN>()"
              << std::right << std::setw(17)
              << mpfr::const_pi<1024, MPFR_RNDN>();
    debug_clear();
    std::cout << std::setw(20)
              << type2char(mpfr::const_pi<1024, MPFR_RNDN>());
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    std::cout << std::setw(38) << std::left
              << "mpfr::const_euler<1024, MPFR_RNDN>()"
              << std::right << std::setw(17)
              << mpfr::const_euler<1024, MPFR_RNDN>();
    debug_clear();
    std::cout << std::setw(20)
              << type2char(mpfr::const_euler<1024, MPFR_RNDN>());
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    std::cout << std::setw(38) << std::left
              << "mpfr::const_catalan<1024, MPFR_RNDN>()"
              << std::right << std::setw(17)
              << mpfr::const_catalan<1024, MPFR_RNDN>();
    debug_clear();
    std::cout << std::setw(20)
              << type2char(mpfr::const_catalan<1024, MPFR_RNDN>());
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    std::cout << std::setw(38) << std::left
              << "mpfr::factorial<1024, MPFR_RNDN>(5)"
              << std::right << std::setw(17)
              << mpfr::factorial<1024, MPFR_RNDN>(5);
    debug_clear();
    std::cout << std::setw(20)
              << type2char(mpfr::factorial<1024, MPFR_RNDN>(5));
    std::cout << "    " << debug_out() << std::endl;
  }


  // //////////////////////////////////////////////////////////////////
  // functions with simple mapping to MPFR functions
  // //////////////////////////////////////////////////////////////////

  {
    mpfr::real<1024> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "isfinite(a1)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << isfinite(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(isfinite(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "isfinite(a1)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << isfinite(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(isfinite(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

#ifdef MPFR_REAL_MATH_H_POSIX
  {
    double a1 = static_cast<double>(0.70711);

    std::cout << std::setw(22) << std::left << "isfinite(a1)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(17) << isfinite(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(isfinite(a1));
    std::cout << "    " << debug_out() << std::endl;
  }
#endif  // MPFR_REAL_MATH_H_POSIX

  {
    mpfr::real<1024> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "isinf(a1)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << isinf(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(isinf(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "isinf(a1)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << isinf(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(isinf(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

#ifdef MPFR_REAL_MATH_H_POSIX
  {
    double a1 = static_cast<double>(0.70711);

    std::cout << std::setw(22) << std::left << "isinf(a1)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(17) << isinf(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(isinf(a1));
    std::cout << "    " << debug_out() << std::endl;
  }
#endif  // MPFR_REAL_MATH_H_POSIX

  {
    mpfr::real<1024> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "isnan(a1)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << isnan(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(isnan(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "isnan(a1)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << isnan(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(isnan(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

#ifdef MPFR_REAL_MATH_H_POSIX
  {
    double a1 = static_cast<double>(0.70711);

    std::cout << std::setw(22) << std::left << "isnan(a1)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(17) << isnan(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(isnan(a1));
    std::cout << "    " << debug_out() << std::endl;
  }
#endif  // MPFR_REAL_MATH_H_POSIX

  {
    mpfr::real<1024> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "isnormal(a1)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << isnormal(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(isnormal(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "isnormal(a1)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << isnormal(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(isnormal(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

#ifdef MPFR_REAL_MATH_H_POSIX
  {
    double a1 = static_cast<double>(0.70711);

    std::cout << std::setw(22) << std::left << "isnormal(a1)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(17) << isnormal(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(isnormal(a1));
    std::cout << "    " << debug_out() << std::endl;
  }
#endif  // MPFR_REAL_MATH_H_POSIX

  {
    mpfr::real<1024> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "signbit(a1)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << signbit(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(signbit(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "signbit(a1)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << signbit(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(signbit(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

#ifdef MPFR_REAL_MATH_H_POSIX
  {
    double a1 = static_cast<double>(0.70711);

    std::cout << std::setw(22) << std::left << "signbit(a1)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(17) << signbit(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(signbit(a1));
    std::cout << "    " << debug_out() << std::endl;
  }
#endif  // MPFR_REAL_MATH_H_POSIX

  {
    mpfr::real<1024> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "acos(a1)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << acos(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(acos(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "acos(a1)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << acos(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(acos(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    double a1 = static_cast<double>(0.70711);

    std::cout << std::setw(22) << std::left << "acos(a1)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(17) << acos(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(acos(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "acosh(a1)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << acosh(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(acosh(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "acosh(a1)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << acosh(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(acosh(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

#ifdef MPFR_REAL_MATH_H_POSIX
  {
    double a1 = static_cast<double>(0.70711);

    std::cout << std::setw(22) << std::left << "acosh(a1)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(17) << acosh(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(acosh(a1));
    std::cout << "    " << debug_out() << std::endl;
  }
#endif  // MPFR_REAL_MATH_H_POSIX

  {
    mpfr::real<1024> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "asin(a1)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << asin(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(asin(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "asin(a1)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << asin(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(asin(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    double a1 = static_cast<double>(0.70711);

    std::cout << std::setw(22) << std::left << "asin(a1)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(17) << asin(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(asin(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "asinh(a1)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << asinh(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(asinh(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "asinh(a1)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << asinh(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(asinh(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

#ifdef MPFR_REAL_MATH_H_POSIX
  {
    double a1 = static_cast<double>(0.70711);

    std::cout << std::setw(22) << std::left << "asinh(a1)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(17) << asinh(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(asinh(a1));
    std::cout << "    " << debug_out() << std::endl;
  }
#endif  // MPFR_REAL_MATH_H_POSIX

  {
    mpfr::real<1024> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "atan(a1)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << atan(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(atan(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "atan(a1)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << atan(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(atan(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    double a1 = static_cast<double>(0.70711);

    std::cout << std::setw(22) << std::left << "atan(a1)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(17) << atan(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(atan(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "atanh(a1)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << atanh(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(atanh(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "atanh(a1)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << atanh(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(atanh(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

#ifdef MPFR_REAL_MATH_H_POSIX
  {
    double a1 = static_cast<double>(0.70711);

    std::cout << std::setw(22) << std::left << "atanh(a1)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(17) << atanh(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(atanh(a1));
    std::cout << "    " << debug_out() << std::endl;
  }
#endif  // MPFR_REAL_MATH_H_POSIX

  {
    mpfr::real<1024> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "cbrt(a1)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << cbrt(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(cbrt(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "cbrt(a1)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << cbrt(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(cbrt(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

#ifdef MPFR_REAL_MATH_H_POSIX
  {
    double a1 = static_cast<double>(0.70711);

    std::cout << std::setw(22) << std::left << "cbrt(a1)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(17) << cbrt(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(cbrt(a1));
    std::cout << "    " << debug_out() << std::endl;
  }
#endif  // MPFR_REAL_MATH_H_POSIX

  {
    mpfr::real<1024> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "ceil(a1)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << ceil(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(ceil(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "ceil(a1)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << ceil(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(ceil(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    double a1 = static_cast<double>(0.70711);

    std::cout << std::setw(22) << std::left << "ceil(a1)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(17) << ceil(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(ceil(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "cos(a1)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << cos(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(cos(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "cos(a1)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << cos(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(cos(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    double a1 = static_cast<double>(0.70711);

    std::cout << std::setw(22) << std::left << "cos(a1)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(17) << cos(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(cos(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "cosh(a1)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << cosh(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(cosh(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "cosh(a1)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << cosh(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(cosh(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    double a1 = static_cast<double>(0.70711);

    std::cout << std::setw(22) << std::left << "cosh(a1)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(17) << cosh(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(cosh(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "erf(a1)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << erf(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(erf(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "erf(a1)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << erf(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(erf(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

#ifdef MPFR_REAL_MATH_H_POSIX
  {
    double a1 = static_cast<double>(0.70711);

    std::cout << std::setw(22) << std::left << "erf(a1)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(17) << erf(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(erf(a1));
    std::cout << "    " << debug_out() << std::endl;
  }
#endif  // MPFR_REAL_MATH_H_POSIX

  {
    mpfr::real<1024> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "erfc(a1)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << erfc(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(erfc(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "erfc(a1)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << erfc(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(erfc(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

#ifdef MPFR_REAL_MATH_H_POSIX
  {
    double a1 = static_cast<double>(0.70711);

    std::cout << std::setw(22) << std::left << "erfc(a1)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(17) << erfc(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(erfc(a1));
    std::cout << "    " << debug_out() << std::endl;
  }
#endif  // MPFR_REAL_MATH_H_POSIX

  {
    mpfr::real<1024> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "exp(a1)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << exp(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(exp(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "exp(a1)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << exp(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(exp(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    double a1 = static_cast<double>(0.70711);

    std::cout << std::setw(22) << std::left << "exp(a1)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(17) << exp(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(exp(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "exp2(a1)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << exp2(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(exp2(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "exp2(a1)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << exp2(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(exp2(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

#ifdef MPFR_REAL_MATH_H_POSIX
  {
    double a1 = static_cast<double>(0.70711);

    std::cout << std::setw(22) << std::left << "exp2(a1)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(17) << exp2(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(exp2(a1));
    std::cout << "    " << debug_out() << std::endl;
  }
#endif  // MPFR_REAL_MATH_H_POSIX

  {
    mpfr::real<1024> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "expm1(a1)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << expm1(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(expm1(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "expm1(a1)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << expm1(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(expm1(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

#ifdef MPFR_REAL_MATH_H_POSIX
  {
    double a1 = static_cast<double>(0.70711);

    std::cout << std::setw(22) << std::left << "expm1(a1)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(17) << expm1(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(expm1(a1));
    std::cout << "    " << debug_out() << std::endl;
  }
#endif  // MPFR_REAL_MATH_H_POSIX

  {
    mpfr::real<1024> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "fabs(a1)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << fabs(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(fabs(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "fabs(a1)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << fabs(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(fabs(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    double a1 = static_cast<double>(0.70711);

    std::cout << std::setw(22) << std::left << "fabs(a1)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(17) << fabs(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(fabs(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "abs(a1)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << abs(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(abs(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "abs(a1)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << abs(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(abs(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    double a1 = static_cast<double>(0.70711);

    std::cout << std::setw(22) << std::left << "abs(a1)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(17) << abs(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(abs(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "floor(a1)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << floor(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(floor(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "floor(a1)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << floor(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(floor(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    double a1 = static_cast<double>(0.70711);

    std::cout << std::setw(22) << std::left << "floor(a1)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(17) << floor(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(floor(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "log(a1)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << log(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(log(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "log(a1)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << log(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(log(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    double a1 = static_cast<double>(0.70711);

    std::cout << std::setw(22) << std::left << "log(a1)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(17) << log(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(log(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "log10(a1)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << log10(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(log10(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "log10(a1)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << log10(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(log10(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    double a1 = static_cast<double>(0.70711);

    std::cout << std::setw(22) << std::left << "log10(a1)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(17) << log10(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(log10(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "log1p(a1)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << log1p(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(log1p(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "log1p(a1)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << log1p(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(log1p(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

#ifdef MPFR_REAL_MATH_H_POSIX
  {
    double a1 = static_cast<double>(0.70711);

    std::cout << std::setw(22) << std::left << "log1p(a1)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(17) << log1p(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(log1p(a1));
    std::cout << "    " << debug_out() << std::endl;
  }
#endif  // MPFR_REAL_MATH_H_POSIX

  {
    mpfr::real<1024> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "log2(a1)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << log2(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(log2(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "log2(a1)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << log2(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(log2(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

#ifdef MPFR_REAL_MATH_H_POSIX
  {
    double a1 = static_cast<double>(0.70711);

    std::cout << std::setw(22) << std::left << "log2(a1)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(17) << log2(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(log2(a1));
    std::cout << "    " << debug_out() << std::endl;
  }
#endif  // MPFR_REAL_MATH_H_POSIX

  {
    mpfr::real<1024> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "nearbyint(a1)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << nearbyint(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(nearbyint(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "nearbyint(a1)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << nearbyint(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(nearbyint(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

#ifdef MPFR_REAL_MATH_H_POSIX
  {
    double a1 = static_cast<double>(0.70711);

    std::cout << std::setw(22) << std::left << "nearbyint(a1)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(17) << nearbyint(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(nearbyint(a1));
    std::cout << "    " << debug_out() << std::endl;
  }
#endif  // MPFR_REAL_MATH_H_POSIX

  {
    mpfr::real<1024> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "rint(a1)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << rint(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(rint(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "rint(a1)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << rint(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(rint(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

#ifdef MPFR_REAL_MATH_H_POSIX
  {
    double a1 = static_cast<double>(0.70711);

    std::cout << std::setw(22) << std::left << "rint(a1)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(17) << rint(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(rint(a1));
    std::cout << "    " << debug_out() << std::endl;
  }
#endif  // MPFR_REAL_MATH_H_POSIX

  {
    mpfr::real<1024> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "round(a1)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << round(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(round(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "round(a1)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << round(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(round(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

#ifdef MPFR_REAL_MATH_H_POSIX
  {
    double a1 = static_cast<double>(0.70711);

    std::cout << std::setw(22) << std::left << "round(a1)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(17) << round(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(round(a1));
    std::cout << "    " << debug_out() << std::endl;
  }
#endif  // MPFR_REAL_MATH_H_POSIX

  {
    mpfr::real<1024> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "sin(a1)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << sin(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(sin(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "sin(a1)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << sin(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(sin(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    double a1 = static_cast<double>(0.70711);

    std::cout << std::setw(22) << std::left << "sin(a1)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(17) << sin(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(sin(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "sinh(a1)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << sinh(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(sinh(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "sinh(a1)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << sinh(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(sinh(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    double a1 = static_cast<double>(0.70711);

    std::cout << std::setw(22) << std::left << "sinh(a1)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(17) << sinh(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(sinh(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "sqrt(a1)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << sqrt(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(sqrt(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "sqrt(a1)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << sqrt(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(sqrt(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    double a1 = static_cast<double>(0.70711);

    std::cout << std::setw(22) << std::left << "sqrt(a1)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(17) << sqrt(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(sqrt(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "tan(a1)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << tan(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(tan(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "tan(a1)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << tan(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(tan(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    double a1 = static_cast<double>(0.70711);

    std::cout << std::setw(22) << std::left << "tan(a1)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(17) << tan(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(tan(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "tanh(a1)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << tanh(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(tanh(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "tanh(a1)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << tanh(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(tanh(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    double a1 = static_cast<double>(0.70711);

    std::cout << std::setw(22) << std::left << "tanh(a1)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(17) << tanh(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(tanh(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "tgamma(a1)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << tgamma(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(tgamma(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "tgamma(a1)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << tgamma(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(tgamma(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

#ifdef MPFR_REAL_MATH_H_POSIX
  {
    double a1 = static_cast<double>(0.70711);

    std::cout << std::setw(22) << std::left << "tgamma(a1)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(17) << tgamma(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(tgamma(a1));
    std::cout << "    " << debug_out() << std::endl;
  }
#endif  // MPFR_REAL_MATH_H_POSIX

  {
    mpfr::real<1024> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "trunc(a1)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << trunc(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(trunc(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "trunc(a1)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << trunc(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(trunc(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

#ifdef MPFR_REAL_MATH_H_POSIX
  {
    double a1 = static_cast<double>(0.70711);

    std::cout << std::setw(22) << std::left << "trunc(a1)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(17) << trunc(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(trunc(a1));
    std::cout << "    " << debug_out() << std::endl;
  }
#endif  // MPFR_REAL_MATH_H_POSIX

  {
    mpfr::real<1024> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "j0(a1)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << j0(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(j0(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "j0(a1)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << j0(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(j0(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

#ifdef MPFR_REAL_MATH_H_POSIX
  {
    double a1 = static_cast<double>(0.70711);

    std::cout << std::setw(22) << std::left << "j0(a1)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(17) << j0(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(j0(a1));
    std::cout << "    " << debug_out() << std::endl;
  }
#endif  // MPFR_REAL_MATH_H_POSIX

  {
    mpfr::real<1024> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "j1(a1)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << j1(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(j1(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "j1(a1)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << j1(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(j1(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

#ifdef MPFR_REAL_MATH_H_POSIX
  {
    double a1 = static_cast<double>(0.70711);

    std::cout << std::setw(22) << std::left << "j1(a1)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(17) << j1(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(j1(a1));
    std::cout << "    " << debug_out() << std::endl;
  }
#endif  // MPFR_REAL_MATH_H_POSIX

  {
    mpfr::real<1024> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "y0(a1)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << y0(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(y0(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "y0(a1)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << y0(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(y0(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

#ifdef MPFR_REAL_MATH_H_POSIX
  {
    double a1 = static_cast<double>(0.70711);

    std::cout << std::setw(22) << std::left << "y0(a1)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(17) << y0(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(y0(a1));
    std::cout << "    " << debug_out() << std::endl;
  }
#endif  // MPFR_REAL_MATH_H_POSIX

  {
    mpfr::real<1024> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "y1(a1)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << y1(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(y1(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "y1(a1)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << y1(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(y1(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

#ifdef MPFR_REAL_MATH_H_POSIX
  {
    double a1 = static_cast<double>(0.70711);

    std::cout << std::setw(22) << std::left << "y1(a1)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(17) << y1(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(y1(a1));
    std::cout << "    " << debug_out() << std::endl;
  }
#endif  // MPFR_REAL_MATH_H_POSIX

  {
    mpfr::real<1024> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "ai(a1)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << ai(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(ai(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "ai(a1)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << ai(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(ai(a1));
    std::cout << "    " << debug_out() << std::endl;
  }


  {
    mpfr::real<1024> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "cot(a1)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << cot(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(cot(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "cot(a1)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << cot(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(cot(a1));
    std::cout << "    " << debug_out() << std::endl;
  }


  {
    mpfr::real<1024> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "coth(a1)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << coth(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(coth(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "coth(a1)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << coth(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(coth(a1));
    std::cout << "    " << debug_out() << std::endl;
  }


  {
    mpfr::real<1024> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "csc(a1)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << csc(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(csc(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "csc(a1)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << csc(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(csc(a1));
    std::cout << "    " << debug_out() << std::endl;
  }


  {
    mpfr::real<1024> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "csch(a1)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << csch(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(csch(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "csch(a1)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << csch(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(csch(a1));
    std::cout << "    " << debug_out() << std::endl;
  }


  {
    mpfr::real<1024> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "digamma(a1)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << digamma(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(digamma(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "digamma(a1)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << digamma(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(digamma(a1));
    std::cout << "    " << debug_out() << std::endl;
  }


  {
    mpfr::real<1024> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "exp10(a1)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << exp10(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(exp10(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "exp10(a1)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << exp10(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(exp10(a1));
    std::cout << "    " << debug_out() << std::endl;
  }


  {
    mpfr::real<1024> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "expint(a1)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << expint(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(expint(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "expint(a1)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << expint(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(expint(a1));
    std::cout << "    " << debug_out() << std::endl;
  }


  {
    mpfr::real<1024> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "frac(a1)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << frac(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(frac(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "frac(a1)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << frac(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(frac(a1));
    std::cout << "    " << debug_out() << std::endl;
  }


  {
    mpfr::real<1024> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "isinteger(a1)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << isinteger(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(isinteger(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "isinteger(a1)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << isinteger(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(isinteger(a1));
    std::cout << "    " << debug_out() << std::endl;
  }


  {
    mpfr::real<1024> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "iszero(a1)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << iszero(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(iszero(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "iszero(a1)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << iszero(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(iszero(a1));
    std::cout << "    " << debug_out() << std::endl;
  }


  {
    mpfr::real<1024> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "li2(a1)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << li2(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(li2(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "li2(a1)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << li2(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(li2(a1));
    std::cout << "    " << debug_out() << std::endl;
  }


  {
    mpfr::real<1024> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "rec_sqrt(a1)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << rec_sqrt(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(rec_sqrt(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "rec_sqrt(a1)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << rec_sqrt(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(rec_sqrt(a1));
    std::cout << "    " << debug_out() << std::endl;
  }


  {
    mpfr::real<1024> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "sec(a1)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << sec(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(sec(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "sec(a1)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << sec(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(sec(a1));
    std::cout << "    " << debug_out() << std::endl;
  }


  {
    mpfr::real<1024> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "sech(a1)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << sech(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(sech(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "sech(a1)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << sech(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(sech(a1));
    std::cout << "    " << debug_out() << std::endl;
  }


  {
    mpfr::real<1024> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "sgn(a1)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << sgn(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(sgn(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "sgn(a1)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << sgn(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(sgn(a1));
    std::cout << "    " << debug_out() << std::endl;
  }


  {
    mpfr::real<1024> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "zeta(a1)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << zeta(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(zeta(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "zeta(a1)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << zeta(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(zeta(a1));
    std::cout << "    " << debug_out() << std::endl;
  }


  {
    mpfr::real<1024> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "fpclassify(a1)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << fpclassify(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(fpclassify(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "fpclassify(a1)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << fpclassify(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(fpclassify(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

#ifdef MPFR_REAL_MATH_H_POSIX
  {
    double a1 = static_cast<double>(0.70711);

    std::cout << std::setw(22) << std::left << "fpclassify(a1)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(17) << fpclassify(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(fpclassify(a1));
    std::cout << "    " << debug_out() << std::endl;
  }
#endif  // MPFR_REAL_MATH_H_POSIX

  {
    mpfr::real<1024> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "ilogb(a1)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << ilogb(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(ilogb(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "ilogb(a1)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << ilogb(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(ilogb(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

#ifdef MPFR_REAL_MATH_H_POSIX
  {
    double a1 = static_cast<double>(0.70711);

    std::cout << std::setw(22) << std::left << "ilogb(a1)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(17) << ilogb(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(ilogb(a1));
    std::cout << "    " << debug_out() << std::endl;
  }
#endif  // MPFR_REAL_MATH_H_POSIX

  {
    mpfr::real<1024> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "lgamma(a1)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << lgamma(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(lgamma(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "lgamma(a1)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << lgamma(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(lgamma(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

#ifdef MPFR_REAL_MATH_H_POSIX
  {
    double a1 = static_cast<double>(0.70711);

    std::cout << std::setw(22) << std::left << "lgamma(a1)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(17) << lgamma(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(lgamma(a1));
    std::cout << "    " << debug_out() << std::endl;
  }
#endif  // MPFR_REAL_MATH_H_POSIX

  {
    mpfr::real<1024> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "logb(a1)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << logb(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(logb(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;

    std::cout << std::setw(22) << std::left << "logb(a1)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << logb(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(logb(a1));
    std::cout << "    " << debug_out() << std::endl;
  }

#ifdef MPFR_REAL_MATH_H_POSIX
  {
    double a1 = static_cast<double>(0.70711);

    std::cout << std::setw(22) << std::left << "logb(a1)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(17) << logb(a1);
    debug_clear();
    std::cout << std::setw(20) << type2char(logb(a1));
    std::cout << "    " << debug_out() << std::endl;
  }
#endif  // MPFR_REAL_MATH_H_POSIX


  {
    mpfr::real<1024> a1 = 0.70711;
    mpfr::real<1024> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "isgreater(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << isgreater(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(isgreater(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> a1 = 0.70711;
    mpfr::real<2048> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "isgreater(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << isgreater(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(isgreater(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> a1 = 0.70711;
    double a2 = static_cast<double>(-3.14159);

    std::cout << std::setw(22) << std::left << "isgreater(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "double"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << isgreater(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(isgreater(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;
    mpfr::real<1024> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "isgreater(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << isgreater(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(isgreater(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;
    mpfr::real<2048> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "isgreater(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << isgreater(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(isgreater(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;
    double a2 = static_cast<double>(-3.14159);

    std::cout << std::setw(22) << std::left << "isgreater(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "double"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << isgreater(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(isgreater(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    double a1 = static_cast<double>(0.70711);
    mpfr::real<1024> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "isgreater(a1, a2)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << isgreater(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(isgreater(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    double a1 = static_cast<double>(0.70711);
    mpfr::real<2048> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "isgreater(a1, a2)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << isgreater(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(isgreater(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

#ifdef MPFR_REAL_MATH_H_POSIX
  {
    double a1 = static_cast<double>(0.70711);
    double a2 = static_cast<double>(-3.14159);

    std::cout << std::setw(22) << std::left << "isgreater(a1, a2)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "double"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << isgreater(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(isgreater(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }
#endif  // MPFR_REAL_MATH_H_POSIX

  {
    mpfr::real<1024> a1 = 0.70711;
    mpfr::real<1024> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "isgreaterequal(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << isgreaterequal(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(isgreaterequal(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> a1 = 0.70711;
    mpfr::real<2048> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "isgreaterequal(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << isgreaterequal(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(isgreaterequal(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> a1 = 0.70711;
    double a2 = static_cast<double>(-3.14159);

    std::cout << std::setw(22) << std::left << "isgreaterequal(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "double"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << isgreaterequal(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(isgreaterequal(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;
    mpfr::real<1024> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "isgreaterequal(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << isgreaterequal(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(isgreaterequal(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;
    mpfr::real<2048> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "isgreaterequal(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << isgreaterequal(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(isgreaterequal(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;
    double a2 = static_cast<double>(-3.14159);

    std::cout << std::setw(22) << std::left << "isgreaterequal(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "double"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << isgreaterequal(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(isgreaterequal(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    double a1 = static_cast<double>(0.70711);
    mpfr::real<1024> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "isgreaterequal(a1, a2)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << isgreaterequal(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(isgreaterequal(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    double a1 = static_cast<double>(0.70711);
    mpfr::real<2048> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "isgreaterequal(a1, a2)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << isgreaterequal(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(isgreaterequal(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

#ifdef MPFR_REAL_MATH_H_POSIX
  {
    double a1 = static_cast<double>(0.70711);
    double a2 = static_cast<double>(-3.14159);

    std::cout << std::setw(22) << std::left << "isgreaterequal(a1, a2)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "double"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << isgreaterequal(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(isgreaterequal(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }
#endif  // MPFR_REAL_MATH_H_POSIX

  {
    mpfr::real<1024> a1 = 0.70711;
    mpfr::real<1024> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "isless(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << isless(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(isless(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> a1 = 0.70711;
    mpfr::real<2048> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "isless(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << isless(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(isless(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> a1 = 0.70711;
    double a2 = static_cast<double>(-3.14159);

    std::cout << std::setw(22) << std::left << "isless(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "double"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << isless(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(isless(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;
    mpfr::real<1024> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "isless(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << isless(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(isless(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;
    mpfr::real<2048> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "isless(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << isless(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(isless(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;
    double a2 = static_cast<double>(-3.14159);

    std::cout << std::setw(22) << std::left << "isless(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "double"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << isless(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(isless(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    double a1 = static_cast<double>(0.70711);
    mpfr::real<1024> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "isless(a1, a2)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << isless(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(isless(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    double a1 = static_cast<double>(0.70711);
    mpfr::real<2048> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "isless(a1, a2)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << isless(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(isless(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

#ifdef MPFR_REAL_MATH_H_POSIX
  {
    double a1 = static_cast<double>(0.70711);
    double a2 = static_cast<double>(-3.14159);

    std::cout << std::setw(22) << std::left << "isless(a1, a2)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "double"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << isless(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(isless(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }
#endif  // MPFR_REAL_MATH_H_POSIX

  {
    mpfr::real<1024> a1 = 0.70711;
    mpfr::real<1024> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "islessequal(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << islessequal(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(islessequal(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> a1 = 0.70711;
    mpfr::real<2048> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "islessequal(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << islessequal(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(islessequal(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> a1 = 0.70711;
    double a2 = static_cast<double>(-3.14159);

    std::cout << std::setw(22) << std::left << "islessequal(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "double"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << islessequal(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(islessequal(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;
    mpfr::real<1024> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "islessequal(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << islessequal(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(islessequal(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;
    mpfr::real<2048> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "islessequal(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << islessequal(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(islessequal(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;
    double a2 = static_cast<double>(-3.14159);

    std::cout << std::setw(22) << std::left << "islessequal(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "double"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << islessequal(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(islessequal(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    double a1 = static_cast<double>(0.70711);
    mpfr::real<1024> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "islessequal(a1, a2)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << islessequal(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(islessequal(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    double a1 = static_cast<double>(0.70711);
    mpfr::real<2048> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "islessequal(a1, a2)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << islessequal(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(islessequal(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

#ifdef MPFR_REAL_MATH_H_POSIX
  {
    double a1 = static_cast<double>(0.70711);
    double a2 = static_cast<double>(-3.14159);

    std::cout << std::setw(22) << std::left << "islessequal(a1, a2)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "double"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << islessequal(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(islessequal(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }
#endif  // MPFR_REAL_MATH_H_POSIX

  {
    mpfr::real<1024> a1 = 0.70711;
    mpfr::real<1024> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "islessgreater(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << islessgreater(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(islessgreater(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> a1 = 0.70711;
    mpfr::real<2048> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "islessgreater(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << islessgreater(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(islessgreater(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> a1 = 0.70711;
    double a2 = static_cast<double>(-3.14159);

    std::cout << std::setw(22) << std::left << "islessgreater(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "double"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << islessgreater(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(islessgreater(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;
    mpfr::real<1024> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "islessgreater(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << islessgreater(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(islessgreater(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;
    mpfr::real<2048> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "islessgreater(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << islessgreater(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(islessgreater(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;
    double a2 = static_cast<double>(-3.14159);

    std::cout << std::setw(22) << std::left << "islessgreater(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "double"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << islessgreater(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(islessgreater(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    double a1 = static_cast<double>(0.70711);
    mpfr::real<1024> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "islessgreater(a1, a2)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << islessgreater(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(islessgreater(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    double a1 = static_cast<double>(0.70711);
    mpfr::real<2048> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "islessgreater(a1, a2)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << islessgreater(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(islessgreater(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

#ifdef MPFR_REAL_MATH_H_POSIX
  {
    double a1 = static_cast<double>(0.70711);
    double a2 = static_cast<double>(-3.14159);

    std::cout << std::setw(22) << std::left << "islessgreater(a1, a2)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "double"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << islessgreater(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(islessgreater(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }
#endif  // MPFR_REAL_MATH_H_POSIX

  {
    mpfr::real<1024> a1 = 0.70711;
    mpfr::real<1024> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "isunordered(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << isunordered(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(isunordered(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> a1 = 0.70711;
    mpfr::real<2048> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "isunordered(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << isunordered(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(isunordered(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> a1 = 0.70711;
    double a2 = static_cast<double>(-3.14159);

    std::cout << std::setw(22) << std::left << "isunordered(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "double"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << isunordered(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(isunordered(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;
    mpfr::real<1024> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "isunordered(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << isunordered(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(isunordered(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;
    mpfr::real<2048> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "isunordered(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << isunordered(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(isunordered(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;
    double a2 = static_cast<double>(-3.14159);

    std::cout << std::setw(22) << std::left << "isunordered(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "double"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << isunordered(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(isunordered(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    double a1 = static_cast<double>(0.70711);
    mpfr::real<1024> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "isunordered(a1, a2)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << isunordered(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(isunordered(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    double a1 = static_cast<double>(0.70711);
    mpfr::real<2048> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "isunordered(a1, a2)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << isunordered(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(isunordered(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

#ifdef MPFR_REAL_MATH_H_POSIX
  {
    double a1 = static_cast<double>(0.70711);
    double a2 = static_cast<double>(-3.14159);

    std::cout << std::setw(22) << std::left << "isunordered(a1, a2)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "double"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << isunordered(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(isunordered(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }
#endif  // MPFR_REAL_MATH_H_POSIX

  {
    mpfr::real<1024> a1 = 0.70711;
    mpfr::real<1024> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "atan2(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << atan2(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(atan2(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> a1 = 0.70711;
    mpfr::real<2048> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "atan2(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << atan2(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(atan2(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> a1 = 0.70711;
    double a2 = static_cast<double>(-3.14159);

    std::cout << std::setw(22) << std::left << "atan2(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "double"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << atan2(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(atan2(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;
    mpfr::real<1024> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "atan2(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << atan2(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(atan2(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;
    mpfr::real<2048> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "atan2(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << atan2(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(atan2(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;
    double a2 = static_cast<double>(-3.14159);

    std::cout << std::setw(22) << std::left << "atan2(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "double"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << atan2(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(atan2(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    double a1 = static_cast<double>(0.70711);
    mpfr::real<1024> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "atan2(a1, a2)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << atan2(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(atan2(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    double a1 = static_cast<double>(0.70711);
    mpfr::real<2048> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "atan2(a1, a2)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << atan2(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(atan2(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    double a1 = static_cast<double>(0.70711);
    double a2 = static_cast<double>(-3.14159);

    std::cout << std::setw(22) << std::left << "atan2(a1, a2)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "double"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << atan2(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(atan2(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> a1 = 0.70711;
    mpfr::real<1024> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "copysign(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << copysign(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(copysign(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> a1 = 0.70711;
    mpfr::real<2048> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "copysign(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << copysign(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(copysign(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> a1 = 0.70711;
    double a2 = static_cast<double>(-3.14159);

    std::cout << std::setw(22) << std::left << "copysign(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "double"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << copysign(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(copysign(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;
    mpfr::real<1024> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "copysign(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << copysign(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(copysign(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;
    mpfr::real<2048> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "copysign(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << copysign(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(copysign(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;
    double a2 = static_cast<double>(-3.14159);

    std::cout << std::setw(22) << std::left << "copysign(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "double"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << copysign(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(copysign(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    double a1 = static_cast<double>(0.70711);
    mpfr::real<1024> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "copysign(a1, a2)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << copysign(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(copysign(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    double a1 = static_cast<double>(0.70711);
    mpfr::real<2048> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "copysign(a1, a2)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << copysign(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(copysign(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

#ifdef MPFR_REAL_MATH_H_POSIX
  {
    double a1 = static_cast<double>(0.70711);
    double a2 = static_cast<double>(-3.14159);

    std::cout << std::setw(22) << std::left << "copysign(a1, a2)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "double"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << copysign(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(copysign(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }
#endif  // MPFR_REAL_MATH_H_POSIX

  {
    mpfr::real<1024> a1 = 0.70711;
    mpfr::real<1024> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "fdim(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << fdim(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(fdim(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> a1 = 0.70711;
    mpfr::real<2048> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "fdim(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << fdim(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(fdim(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> a1 = 0.70711;
    double a2 = static_cast<double>(-3.14159);

    std::cout << std::setw(22) << std::left << "fdim(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "double"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << fdim(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(fdim(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;
    mpfr::real<1024> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "fdim(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << fdim(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(fdim(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;
    mpfr::real<2048> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "fdim(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << fdim(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(fdim(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;
    double a2 = static_cast<double>(-3.14159);

    std::cout << std::setw(22) << std::left << "fdim(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "double"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << fdim(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(fdim(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    double a1 = static_cast<double>(0.70711);
    mpfr::real<1024> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "fdim(a1, a2)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << fdim(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(fdim(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    double a1 = static_cast<double>(0.70711);
    mpfr::real<2048> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "fdim(a1, a2)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << fdim(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(fdim(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

#ifdef MPFR_REAL_MATH_H_POSIX
  {
    double a1 = static_cast<double>(0.70711);
    double a2 = static_cast<double>(-3.14159);

    std::cout << std::setw(22) << std::left << "fdim(a1, a2)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "double"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << fdim(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(fdim(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }
#endif  // MPFR_REAL_MATH_H_POSIX

  {
    mpfr::real<1024> a1 = 0.70711;
    mpfr::real<1024> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "fmax(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << fmax(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(fmax(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> a1 = 0.70711;
    mpfr::real<2048> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "fmax(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << fmax(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(fmax(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> a1 = 0.70711;
    double a2 = static_cast<double>(-3.14159);

    std::cout << std::setw(22) << std::left << "fmax(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "double"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << fmax(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(fmax(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;
    mpfr::real<1024> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "fmax(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << fmax(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(fmax(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;
    mpfr::real<2048> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "fmax(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << fmax(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(fmax(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;
    double a2 = static_cast<double>(-3.14159);

    std::cout << std::setw(22) << std::left << "fmax(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "double"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << fmax(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(fmax(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    double a1 = static_cast<double>(0.70711);
    mpfr::real<1024> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "fmax(a1, a2)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << fmax(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(fmax(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    double a1 = static_cast<double>(0.70711);
    mpfr::real<2048> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "fmax(a1, a2)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << fmax(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(fmax(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

#ifdef MPFR_REAL_MATH_H_POSIX
  {
    double a1 = static_cast<double>(0.70711);
    double a2 = static_cast<double>(-3.14159);

    std::cout << std::setw(22) << std::left << "fmax(a1, a2)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "double"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << fmax(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(fmax(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }
#endif  // MPFR_REAL_MATH_H_POSIX

  {
    mpfr::real<1024> a1 = 0.70711;
    mpfr::real<1024> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "fmin(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << fmin(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(fmin(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> a1 = 0.70711;
    mpfr::real<2048> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "fmin(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << fmin(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(fmin(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> a1 = 0.70711;
    double a2 = static_cast<double>(-3.14159);

    std::cout << std::setw(22) << std::left << "fmin(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "double"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << fmin(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(fmin(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;
    mpfr::real<1024> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "fmin(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << fmin(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(fmin(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;
    mpfr::real<2048> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "fmin(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << fmin(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(fmin(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;
    double a2 = static_cast<double>(-3.14159);

    std::cout << std::setw(22) << std::left << "fmin(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "double"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << fmin(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(fmin(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    double a1 = static_cast<double>(0.70711);
    mpfr::real<1024> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "fmin(a1, a2)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << fmin(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(fmin(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    double a1 = static_cast<double>(0.70711);
    mpfr::real<2048> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "fmin(a1, a2)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << fmin(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(fmin(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

#ifdef MPFR_REAL_MATH_H_POSIX
  {
    double a1 = static_cast<double>(0.70711);
    double a2 = static_cast<double>(-3.14159);

    std::cout << std::setw(22) << std::left << "fmin(a1, a2)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "double"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << fmin(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(fmin(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }
#endif  // MPFR_REAL_MATH_H_POSIX

  {
    mpfr::real<1024> a1 = 0.70711;
    mpfr::real<1024> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "fmod(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << fmod(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(fmod(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> a1 = 0.70711;
    mpfr::real<2048> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "fmod(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << fmod(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(fmod(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> a1 = 0.70711;
    double a2 = static_cast<double>(-3.14159);

    std::cout << std::setw(22) << std::left << "fmod(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "double"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << fmod(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(fmod(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;
    mpfr::real<1024> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "fmod(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << fmod(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(fmod(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;
    mpfr::real<2048> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "fmod(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << fmod(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(fmod(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;
    double a2 = static_cast<double>(-3.14159);

    std::cout << std::setw(22) << std::left << "fmod(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "double"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << fmod(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(fmod(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    double a1 = static_cast<double>(0.70711);
    mpfr::real<1024> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "fmod(a1, a2)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << fmod(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(fmod(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    double a1 = static_cast<double>(0.70711);
    mpfr::real<2048> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "fmod(a1, a2)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << fmod(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(fmod(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    double a1 = static_cast<double>(0.70711);
    double a2 = static_cast<double>(-3.14159);

    std::cout << std::setw(22) << std::left << "fmod(a1, a2)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "double"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << fmod(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(fmod(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> a1 = 0.70711;
    mpfr::real<1024> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "hypot(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << hypot(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(hypot(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> a1 = 0.70711;
    mpfr::real<2048> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "hypot(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << hypot(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(hypot(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> a1 = 0.70711;
    double a2 = static_cast<double>(-3.14159);

    std::cout << std::setw(22) << std::left << "hypot(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "double"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << hypot(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(hypot(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;
    mpfr::real<1024> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "hypot(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << hypot(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(hypot(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;
    mpfr::real<2048> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "hypot(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << hypot(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(hypot(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;
    double a2 = static_cast<double>(-3.14159);

    std::cout << std::setw(22) << std::left << "hypot(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "double"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << hypot(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(hypot(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    double a1 = static_cast<double>(0.70711);
    mpfr::real<1024> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "hypot(a1, a2)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << hypot(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(hypot(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    double a1 = static_cast<double>(0.70711);
    mpfr::real<2048> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "hypot(a1, a2)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << hypot(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(hypot(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

#ifdef MPFR_REAL_MATH_H_POSIX
  {
    double a1 = static_cast<double>(0.70711);
    double a2 = static_cast<double>(-3.14159);

    std::cout << std::setw(22) << std::left << "hypot(a1, a2)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "double"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << hypot(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(hypot(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }
#endif  // MPFR_REAL_MATH_H_POSIX

  {
    mpfr::real<1024> a1 = 0.70711;
    mpfr::real<1024> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "pow(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << pow(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(pow(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> a1 = 0.70711;
    mpfr::real<2048> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "pow(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << pow(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(pow(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> a1 = 0.70711;
    double a2 = static_cast<double>(-3.14159);

    std::cout << std::setw(22) << std::left << "pow(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "double"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << pow(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(pow(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;
    mpfr::real<1024> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "pow(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << pow(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(pow(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;
    mpfr::real<2048> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "pow(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << pow(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(pow(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;
    double a2 = static_cast<double>(-3.14159);

    std::cout << std::setw(22) << std::left << "pow(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "double"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << pow(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(pow(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    double a1 = static_cast<double>(0.70711);
    mpfr::real<1024> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "pow(a1, a2)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << pow(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(pow(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    double a1 = static_cast<double>(0.70711);
    mpfr::real<2048> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "pow(a1, a2)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << pow(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(pow(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    double a1 = static_cast<double>(0.70711);
    double a2 = static_cast<double>(-3.14159);

    std::cout << std::setw(22) << std::left << "pow(a1, a2)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "double"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << pow(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(pow(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> a1 = 0.70711;
    mpfr::real<1024> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "remainder(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << remainder(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(remainder(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> a1 = 0.70711;
    mpfr::real<2048> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "remainder(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << remainder(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(remainder(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> a1 = 0.70711;
    double a2 = static_cast<double>(-3.14159);

    std::cout << std::setw(22) << std::left << "remainder(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "double"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << remainder(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(remainder(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;
    mpfr::real<1024> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "remainder(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << remainder(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(remainder(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;
    mpfr::real<2048> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "remainder(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << remainder(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(remainder(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;
    double a2 = static_cast<double>(-3.14159);

    std::cout << std::setw(22) << std::left << "remainder(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "double"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << remainder(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(remainder(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    double a1 = static_cast<double>(0.70711);
    mpfr::real<1024> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "remainder(a1, a2)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << remainder(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(remainder(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    double a1 = static_cast<double>(0.70711);
    mpfr::real<2048> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "remainder(a1, a2)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << remainder(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(remainder(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

#ifdef MPFR_REAL_MATH_H_POSIX
  {
    double a1 = static_cast<double>(0.70711);
    double a2 = static_cast<double>(-3.14159);

    std::cout << std::setw(22) << std::left << "remainder(a1, a2)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "double"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << remainder(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(remainder(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }
#endif  // MPFR_REAL_MATH_H_POSIX

  {
    mpfr::real<1024> a1 = 0.70711;
    mpfr::real<1024> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "agm(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << agm(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(agm(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> a1 = 0.70711;
    mpfr::real<2048> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "agm(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << agm(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(agm(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> a1 = 0.70711;
    double a2 = static_cast<double>(-3.14159);

    std::cout << std::setw(22) << std::left << "agm(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "double"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << agm(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(agm(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;
    mpfr::real<1024> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "agm(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << agm(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(agm(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;
    mpfr::real<2048> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "agm(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << agm(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(agm(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;
    double a2 = static_cast<double>(-3.14159);

    std::cout << std::setw(22) << std::left << "agm(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "double"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << agm(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(agm(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    double a1 = static_cast<double>(0.70711);
    mpfr::real<1024> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "agm(a1, a2)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << agm(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(agm(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    double a1 = static_cast<double>(0.70711);
    mpfr::real<2048> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "agm(a1, a2)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << agm(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(agm(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }


  {
    mpfr::real<1024> a1 = 0.70711;
    mpfr::real<1024> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "cmpabs(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << cmpabs(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(cmpabs(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> a1 = 0.70711;
    mpfr::real<2048> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "cmpabs(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << cmpabs(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(cmpabs(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> a1 = 0.70711;
    double a2 = static_cast<double>(-3.14159);

    std::cout << std::setw(22) << std::left << "cmpabs(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "double"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << cmpabs(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(cmpabs(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;
    mpfr::real<1024> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "cmpabs(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << cmpabs(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(cmpabs(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;
    mpfr::real<2048> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "cmpabs(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << cmpabs(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(cmpabs(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;
    double a2 = static_cast<double>(-3.14159);

    std::cout << std::setw(22) << std::left << "cmpabs(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "double"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << cmpabs(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(cmpabs(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    double a1 = static_cast<double>(0.70711);
    mpfr::real<1024> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "cmpabs(a1, a2)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << cmpabs(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(cmpabs(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    double a1 = static_cast<double>(0.70711);
    mpfr::real<2048> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "cmpabs(a1, a2)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << cmpabs(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(cmpabs(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }


  {
    mpfr::real<1024> a1 = 0.70711;
    mpfr::real<1024> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "nextafter(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << nextafter(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(nextafter(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> a1 = 0.70711;
    mpfr::real<2048> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "nextafter(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << nextafter(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(nextafter(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> a1 = 0.70711;
    double a2 = static_cast<double>(-3.14159);

    std::cout << std::setw(22) << std::left << "nextafter(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "double"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << nextafter(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(nextafter(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;
    mpfr::real<1024> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "nextafter(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << nextafter(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(nextafter(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;
    mpfr::real<2048> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "nextafter(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << nextafter(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(nextafter(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;
    double a2 = static_cast<double>(-3.14159);

    std::cout << std::setw(22) << std::left << "nextafter(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "double"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << nextafter(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(nextafter(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    double a1 = static_cast<double>(0.70711);
    mpfr::real<1024> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "nextafter(a1, a2)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << nextafter(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(nextafter(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    double a1 = static_cast<double>(0.70711);
    mpfr::real<2048> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "nextafter(a1, a2)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << nextafter(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(nextafter(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

#ifdef MPFR_REAL_MATH_H_POSIX
  {
    double a1 = static_cast<double>(0.70711);
    double a2 = static_cast<double>(-3.14159);

    std::cout << std::setw(22) << std::left << "nextafter(a1, a2)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "double"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << nextafter(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(nextafter(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }
#endif  // MPFR_REAL_MATH_H_POSIX

  {
    mpfr::real<1024> a1 = 0.70711;
    mpfr::real<1024> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "nexttoward(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << nexttoward(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(nexttoward(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> a1 = 0.70711;
    mpfr::real<2048> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "nexttoward(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << nexttoward(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(nexttoward(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> a1 = 0.70711;
    double a2 = static_cast<double>(-3.14159);

    std::cout << std::setw(22) << std::left << "nexttoward(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "double"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << nexttoward(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(nexttoward(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;
    mpfr::real<1024> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "nexttoward(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << nexttoward(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(nexttoward(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;
    mpfr::real<2048> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "nexttoward(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << nexttoward(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(nexttoward(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;
    double a2 = static_cast<double>(-3.14159);

    std::cout << std::setw(22) << std::left << "nexttoward(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "double"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << nexttoward(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(nexttoward(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    double a1 = static_cast<double>(0.70711);
    mpfr::real<1024> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "nexttoward(a1, a2)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << nexttoward(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(nexttoward(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    double a1 = static_cast<double>(0.70711);
    mpfr::real<2048> a2 = -3.14159;

    std::cout << std::setw(22) << std::left << "nexttoward(a1, a2)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << nexttoward(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(nexttoward(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

#ifdef MPFR_REAL_MATH_H_POSIX
  {
    double a1 = static_cast<double>(0.70711);
    double a2 = static_cast<double>(-3.14159);

    std::cout << std::setw(22) << std::left << "nexttoward(a1, a2)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "double"
              << std::setw(12) << "-3.14159"
              << std::setw(17) << nexttoward(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(nexttoward(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }
#endif  // MPFR_REAL_MATH_H_POSIX


  {
    mpfr::real<1024> a1 = 0.70711;
    mpfr::real<1024> a2 = -3.14159;
    mpfr::real<1024> a3 = 0.33333;

    std::cout << std::setw(22) << std::left << "fma(a1, a2, a3)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "-3.14159"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.33333"
              << std::setw(17) << fma(a1, a2, a3);
    debug_clear();
    std::cout << std::setw(20) << type2char(fma(a1, a2, a3));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> a1 = 0.70711;
    mpfr::real<1024> a2 = -3.14159;
    mpfr::real<2048> a3 = 0.33333;

    std::cout << std::setw(22) << std::left << "fma(a1, a2, a3)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "-3.14159"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.33333"
              << std::setw(17) << fma(a1, a2, a3);
    debug_clear();
    std::cout << std::setw(20) << type2char(fma(a1, a2, a3));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> a1 = 0.70711;
    mpfr::real<1024> a2 = -3.14159;
    mpfr::real<512> a3 = 0.33333;

    std::cout << std::setw(22) << std::left << "fma(a1, a2, a3)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "-3.14159"
              << std::setw(20) << "mpfr::real<512>"
              << std::setw(12) << "0.33333"
              << std::setw(17) << fma(a1, a2, a3);
    debug_clear();
    std::cout << std::setw(20) << type2char(fma(a1, a2, a3));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> a1 = 0.70711;
    mpfr::real<1024> a2 = -3.14159;
    double a3 = static_cast<double>(0.33333);

    std::cout << std::setw(22) << std::left << "fma(a1, a2, a3)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "-3.14159"
              << std::setw(20) << "double"
              << std::setw(12) << "0.33333"
              << std::setw(17) << fma(a1, a2, a3);
    debug_clear();
    std::cout << std::setw(20) << type2char(fma(a1, a2, a3));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> a1 = 0.70711;
    mpfr::real<2048> a2 = -3.14159;
    mpfr::real<1024> a3 = 0.33333;

    std::cout << std::setw(22) << std::left << "fma(a1, a2, a3)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "-3.14159"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.33333"
              << std::setw(17) << fma(a1, a2, a3);
    debug_clear();
    std::cout << std::setw(20) << type2char(fma(a1, a2, a3));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> a1 = 0.70711;
    mpfr::real<2048> a2 = -3.14159;
    mpfr::real<2048> a3 = 0.33333;

    std::cout << std::setw(22) << std::left << "fma(a1, a2, a3)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "-3.14159"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.33333"
              << std::setw(17) << fma(a1, a2, a3);
    debug_clear();
    std::cout << std::setw(20) << type2char(fma(a1, a2, a3));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> a1 = 0.70711;
    mpfr::real<2048> a2 = -3.14159;
    mpfr::real<512> a3 = 0.33333;

    std::cout << std::setw(22) << std::left << "fma(a1, a2, a3)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "-3.14159"
              << std::setw(20) << "mpfr::real<512>"
              << std::setw(12) << "0.33333"
              << std::setw(17) << fma(a1, a2, a3);
    debug_clear();
    std::cout << std::setw(20) << type2char(fma(a1, a2, a3));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> a1 = 0.70711;
    mpfr::real<2048> a2 = -3.14159;
    double a3 = static_cast<double>(0.33333);

    std::cout << std::setw(22) << std::left << "fma(a1, a2, a3)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "-3.14159"
              << std::setw(20) << "double"
              << std::setw(12) << "0.33333"
              << std::setw(17) << fma(a1, a2, a3);
    debug_clear();
    std::cout << std::setw(20) << type2char(fma(a1, a2, a3));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> a1 = 0.70711;
    double a2 = static_cast<double>(-3.14159);
    mpfr::real<1024> a3 = 0.33333;

    std::cout << std::setw(22) << std::left << "fma(a1, a2, a3)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "double"
              << std::setw(12) << "-3.14159"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.33333"
              << std::setw(17) << fma(a1, a2, a3);
    debug_clear();
    std::cout << std::setw(20) << type2char(fma(a1, a2, a3));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> a1 = 0.70711;
    double a2 = static_cast<double>(-3.14159);
    mpfr::real<2048> a3 = 0.33333;

    std::cout << std::setw(22) << std::left << "fma(a1, a2, a3)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "double"
              << std::setw(12) << "-3.14159"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.33333"
              << std::setw(17) << fma(a1, a2, a3);
    debug_clear();
    std::cout << std::setw(20) << type2char(fma(a1, a2, a3));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> a1 = 0.70711;
    double a2 = static_cast<double>(-3.14159);
    mpfr::real<512> a3 = 0.33333;

    std::cout << std::setw(22) << std::left << "fma(a1, a2, a3)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "double"
              << std::setw(12) << "-3.14159"
              << std::setw(20) << "mpfr::real<512>"
              << std::setw(12) << "0.33333"
              << std::setw(17) << fma(a1, a2, a3);
    debug_clear();
    std::cout << std::setw(20) << type2char(fma(a1, a2, a3));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> a1 = 0.70711;
    double a2 = static_cast<double>(-3.14159);
    double a3 = static_cast<double>(0.33333);

    std::cout << std::setw(22) << std::left << "fma(a1, a2, a3)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "double"
              << std::setw(12) << "-3.14159"
              << std::setw(20) << "double"
              << std::setw(12) << "0.33333"
              << std::setw(17) << fma(a1, a2, a3);
    debug_clear();
    std::cout << std::setw(20) << type2char(fma(a1, a2, a3));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;
    mpfr::real<1024> a2 = -3.14159;
    mpfr::real<1024> a3 = 0.33333;

    std::cout << std::setw(22) << std::left << "fma(a1, a2, a3)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "-3.14159"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.33333"
              << std::setw(17) << fma(a1, a2, a3);
    debug_clear();
    std::cout << std::setw(20) << type2char(fma(a1, a2, a3));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;
    mpfr::real<1024> a2 = -3.14159;
    mpfr::real<2048> a3 = 0.33333;

    std::cout << std::setw(22) << std::left << "fma(a1, a2, a3)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "-3.14159"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.33333"
              << std::setw(17) << fma(a1, a2, a3);
    debug_clear();
    std::cout << std::setw(20) << type2char(fma(a1, a2, a3));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;
    mpfr::real<1024> a2 = -3.14159;
    mpfr::real<512> a3 = 0.33333;

    std::cout << std::setw(22) << std::left << "fma(a1, a2, a3)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "-3.14159"
              << std::setw(20) << "mpfr::real<512>"
              << std::setw(12) << "0.33333"
              << std::setw(17) << fma(a1, a2, a3);
    debug_clear();
    std::cout << std::setw(20) << type2char(fma(a1, a2, a3));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;
    mpfr::real<1024> a2 = -3.14159;
    double a3 = static_cast<double>(0.33333);

    std::cout << std::setw(22) << std::left << "fma(a1, a2, a3)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "-3.14159"
              << std::setw(20) << "double"
              << std::setw(12) << "0.33333"
              << std::setw(17) << fma(a1, a2, a3);
    debug_clear();
    std::cout << std::setw(20) << type2char(fma(a1, a2, a3));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;
    mpfr::real<2048> a2 = -3.14159;
    mpfr::real<1024> a3 = 0.33333;

    std::cout << std::setw(22) << std::left << "fma(a1, a2, a3)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "-3.14159"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.33333"
              << std::setw(17) << fma(a1, a2, a3);
    debug_clear();
    std::cout << std::setw(20) << type2char(fma(a1, a2, a3));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;
    mpfr::real<2048> a2 = -3.14159;
    mpfr::real<2048> a3 = 0.33333;

    std::cout << std::setw(22) << std::left << "fma(a1, a2, a3)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "-3.14159"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.33333"
              << std::setw(17) << fma(a1, a2, a3);
    debug_clear();
    std::cout << std::setw(20) << type2char(fma(a1, a2, a3));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;
    mpfr::real<2048> a2 = -3.14159;
    mpfr::real<512> a3 = 0.33333;

    std::cout << std::setw(22) << std::left << "fma(a1, a2, a3)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "-3.14159"
              << std::setw(20) << "mpfr::real<512>"
              << std::setw(12) << "0.33333"
              << std::setw(17) << fma(a1, a2, a3);
    debug_clear();
    std::cout << std::setw(20) << type2char(fma(a1, a2, a3));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;
    mpfr::real<2048> a2 = -3.14159;
    double a3 = static_cast<double>(0.33333);

    std::cout << std::setw(22) << std::left << "fma(a1, a2, a3)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "-3.14159"
              << std::setw(20) << "double"
              << std::setw(12) << "0.33333"
              << std::setw(17) << fma(a1, a2, a3);
    debug_clear();
    std::cout << std::setw(20) << type2char(fma(a1, a2, a3));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;
    double a2 = static_cast<double>(-3.14159);
    mpfr::real<1024> a3 = 0.33333;

    std::cout << std::setw(22) << std::left << "fma(a1, a2, a3)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "double"
              << std::setw(12) << "-3.14159"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.33333"
              << std::setw(17) << fma(a1, a2, a3);
    debug_clear();
    std::cout << std::setw(20) << type2char(fma(a1, a2, a3));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;
    double a2 = static_cast<double>(-3.14159);
    mpfr::real<2048> a3 = 0.33333;

    std::cout << std::setw(22) << std::left << "fma(a1, a2, a3)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "double"
              << std::setw(12) << "-3.14159"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.33333"
              << std::setw(17) << fma(a1, a2, a3);
    debug_clear();
    std::cout << std::setw(20) << type2char(fma(a1, a2, a3));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;
    double a2 = static_cast<double>(-3.14159);
    mpfr::real<512> a3 = 0.33333;

    std::cout << std::setw(22) << std::left << "fma(a1, a2, a3)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "double"
              << std::setw(12) << "-3.14159"
              << std::setw(20) << "mpfr::real<512>"
              << std::setw(12) << "0.33333"
              << std::setw(17) << fma(a1, a2, a3);
    debug_clear();
    std::cout << std::setw(20) << type2char(fma(a1, a2, a3));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;
    double a2 = static_cast<double>(-3.14159);
    double a3 = static_cast<double>(0.33333);

    std::cout << std::setw(22) << std::left << "fma(a1, a2, a3)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "double"
              << std::setw(12) << "-3.14159"
              << std::setw(20) << "double"
              << std::setw(12) << "0.33333"
              << std::setw(17) << fma(a1, a2, a3);
    debug_clear();
    std::cout << std::setw(20) << type2char(fma(a1, a2, a3));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    double a1 = static_cast<double>(0.70711);
    mpfr::real<1024> a2 = -3.14159;
    mpfr::real<1024> a3 = 0.33333;

    std::cout << std::setw(22) << std::left << "fma(a1, a2, a3)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "-3.14159"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.33333"
              << std::setw(17) << fma(a1, a2, a3);
    debug_clear();
    std::cout << std::setw(20) << type2char(fma(a1, a2, a3));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    double a1 = static_cast<double>(0.70711);
    mpfr::real<1024> a2 = -3.14159;
    mpfr::real<2048> a3 = 0.33333;

    std::cout << std::setw(22) << std::left << "fma(a1, a2, a3)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "-3.14159"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.33333"
              << std::setw(17) << fma(a1, a2, a3);
    debug_clear();
    std::cout << std::setw(20) << type2char(fma(a1, a2, a3));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    double a1 = static_cast<double>(0.70711);
    mpfr::real<1024> a2 = -3.14159;
    mpfr::real<512> a3 = 0.33333;

    std::cout << std::setw(22) << std::left << "fma(a1, a2, a3)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "-3.14159"
              << std::setw(20) << "mpfr::real<512>"
              << std::setw(12) << "0.33333"
              << std::setw(17) << fma(a1, a2, a3);
    debug_clear();
    std::cout << std::setw(20) << type2char(fma(a1, a2, a3));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    double a1 = static_cast<double>(0.70711);
    mpfr::real<1024> a2 = -3.14159;
    double a3 = static_cast<double>(0.33333);

    std::cout << std::setw(22) << std::left << "fma(a1, a2, a3)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "-3.14159"
              << std::setw(20) << "double"
              << std::setw(12) << "0.33333"
              << std::setw(17) << fma(a1, a2, a3);
    debug_clear();
    std::cout << std::setw(20) << type2char(fma(a1, a2, a3));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    double a1 = static_cast<double>(0.70711);
    mpfr::real<2048> a2 = -3.14159;
    mpfr::real<1024> a3 = 0.33333;

    std::cout << std::setw(22) << std::left << "fma(a1, a2, a3)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "-3.14159"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.33333"
              << std::setw(17) << fma(a1, a2, a3);
    debug_clear();
    std::cout << std::setw(20) << type2char(fma(a1, a2, a3));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    double a1 = static_cast<double>(0.70711);
    mpfr::real<2048> a2 = -3.14159;
    mpfr::real<2048> a3 = 0.33333;

    std::cout << std::setw(22) << std::left << "fma(a1, a2, a3)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "-3.14159"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.33333"
              << std::setw(17) << fma(a1, a2, a3);
    debug_clear();
    std::cout << std::setw(20) << type2char(fma(a1, a2, a3));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    double a1 = static_cast<double>(0.70711);
    mpfr::real<2048> a2 = -3.14159;
    mpfr::real<512> a3 = 0.33333;

    std::cout << std::setw(22) << std::left << "fma(a1, a2, a3)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "-3.14159"
              << std::setw(20) << "mpfr::real<512>"
              << std::setw(12) << "0.33333"
              << std::setw(17) << fma(a1, a2, a3);
    debug_clear();
    std::cout << std::setw(20) << type2char(fma(a1, a2, a3));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    double a1 = static_cast<double>(0.70711);
    mpfr::real<2048> a2 = -3.14159;
    double a3 = static_cast<double>(0.33333);

    std::cout << std::setw(22) << std::left << "fma(a1, a2, a3)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "-3.14159"
              << std::setw(20) << "double"
              << std::setw(12) << "0.33333"
              << std::setw(17) << fma(a1, a2, a3);
    debug_clear();
    std::cout << std::setw(20) << type2char(fma(a1, a2, a3));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    double a1 = static_cast<double>(0.70711);
    double a2 = static_cast<double>(-3.14159);
    mpfr::real<1024> a3 = 0.33333;

    std::cout << std::setw(22) << std::left << "fma(a1, a2, a3)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "double"
              << std::setw(12) << "-3.14159"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.33333"
              << std::setw(17) << fma(a1, a2, a3);
    debug_clear();
    std::cout << std::setw(20) << type2char(fma(a1, a2, a3));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    double a1 = static_cast<double>(0.70711);
    double a2 = static_cast<double>(-3.14159);
    mpfr::real<2048> a3 = 0.33333;

    std::cout << std::setw(22) << std::left << "fma(a1, a2, a3)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "double"
              << std::setw(12) << "-3.14159"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.33333"
              << std::setw(17) << fma(a1, a2, a3);
    debug_clear();
    std::cout << std::setw(20) << type2char(fma(a1, a2, a3));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    double a1 = static_cast<double>(0.70711);
    double a2 = static_cast<double>(-3.14159);
    mpfr::real<512> a3 = 0.33333;

    std::cout << std::setw(22) << std::left << "fma(a1, a2, a3)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "double"
              << std::setw(12) << "-3.14159"
              << std::setw(20) << "mpfr::real<512>"
              << std::setw(12) << "0.33333"
              << std::setw(17) << fma(a1, a2, a3);
    debug_clear();
    std::cout << std::setw(20) << type2char(fma(a1, a2, a3));
    std::cout << "    " << debug_out() << std::endl;
  }

#ifdef MPFR_REAL_MATH_H_POSIX
  {
    double a1 = static_cast<double>(0.70711);
    double a2 = static_cast<double>(-3.14159);
    double a3 = static_cast<double>(0.33333);

    std::cout << std::setw(22) << std::left << "fma(a1, a2, a3)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "double"
              << std::setw(12) << "-3.14159"
              << std::setw(20) << "double"
              << std::setw(12) << "0.33333"
              << std::setw(17) << fma(a1, a2, a3);
    debug_clear();
    std::cout << std::setw(20) << type2char(fma(a1, a2, a3));
    std::cout << "    " << debug_out() << std::endl;
  }
#endif  // MPFR_REAL_MATH_H_POSIX

  {
    mpfr::real<1024> a1 = 0.70711;
    mpfr::real<1024> a2 = -3.14159;
    mpfr::real<1024> a3 = 0.33333;

    std::cout << std::setw(22) << std::left << "fms(a1, a2, a3)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "-3.14159"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.33333"
              << std::setw(17) << fms(a1, a2, a3);
    debug_clear();
    std::cout << std::setw(20) << type2char(fms(a1, a2, a3));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> a1 = 0.70711;
    mpfr::real<1024> a2 = -3.14159;
    mpfr::real<2048> a3 = 0.33333;

    std::cout << std::setw(22) << std::left << "fms(a1, a2, a3)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "-3.14159"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.33333"
              << std::setw(17) << fms(a1, a2, a3);
    debug_clear();
    std::cout << std::setw(20) << type2char(fms(a1, a2, a3));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> a1 = 0.70711;
    mpfr::real<1024> a2 = -3.14159;
    mpfr::real<512> a3 = 0.33333;

    std::cout << std::setw(22) << std::left << "fms(a1, a2, a3)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "-3.14159"
              << std::setw(20) << "mpfr::real<512>"
              << std::setw(12) << "0.33333"
              << std::setw(17) << fms(a1, a2, a3);
    debug_clear();
    std::cout << std::setw(20) << type2char(fms(a1, a2, a3));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> a1 = 0.70711;
    mpfr::real<1024> a2 = -3.14159;
    double a3 = static_cast<double>(0.33333);

    std::cout << std::setw(22) << std::left << "fms(a1, a2, a3)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "-3.14159"
              << std::setw(20) << "double"
              << std::setw(12) << "0.33333"
              << std::setw(17) << fms(a1, a2, a3);
    debug_clear();
    std::cout << std::setw(20) << type2char(fms(a1, a2, a3));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> a1 = 0.70711;
    mpfr::real<2048> a2 = -3.14159;
    mpfr::real<1024> a3 = 0.33333;

    std::cout << std::setw(22) << std::left << "fms(a1, a2, a3)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "-3.14159"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.33333"
              << std::setw(17) << fms(a1, a2, a3);
    debug_clear();
    std::cout << std::setw(20) << type2char(fms(a1, a2, a3));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> a1 = 0.70711;
    mpfr::real<2048> a2 = -3.14159;
    mpfr::real<2048> a3 = 0.33333;

    std::cout << std::setw(22) << std::left << "fms(a1, a2, a3)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "-3.14159"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.33333"
              << std::setw(17) << fms(a1, a2, a3);
    debug_clear();
    std::cout << std::setw(20) << type2char(fms(a1, a2, a3));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> a1 = 0.70711;
    mpfr::real<2048> a2 = -3.14159;
    mpfr::real<512> a3 = 0.33333;

    std::cout << std::setw(22) << std::left << "fms(a1, a2, a3)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "-3.14159"
              << std::setw(20) << "mpfr::real<512>"
              << std::setw(12) << "0.33333"
              << std::setw(17) << fms(a1, a2, a3);
    debug_clear();
    std::cout << std::setw(20) << type2char(fms(a1, a2, a3));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> a1 = 0.70711;
    mpfr::real<2048> a2 = -3.14159;
    double a3 = static_cast<double>(0.33333);

    std::cout << std::setw(22) << std::left << "fms(a1, a2, a3)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "-3.14159"
              << std::setw(20) << "double"
              << std::setw(12) << "0.33333"
              << std::setw(17) << fms(a1, a2, a3);
    debug_clear();
    std::cout << std::setw(20) << type2char(fms(a1, a2, a3));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> a1 = 0.70711;
    double a2 = static_cast<double>(-3.14159);
    mpfr::real<1024> a3 = 0.33333;

    std::cout << std::setw(22) << std::left << "fms(a1, a2, a3)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "double"
              << std::setw(12) << "-3.14159"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.33333"
              << std::setw(17) << fms(a1, a2, a3);
    debug_clear();
    std::cout << std::setw(20) << type2char(fms(a1, a2, a3));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> a1 = 0.70711;
    double a2 = static_cast<double>(-3.14159);
    mpfr::real<2048> a3 = 0.33333;

    std::cout << std::setw(22) << std::left << "fms(a1, a2, a3)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "double"
              << std::setw(12) << "-3.14159"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.33333"
              << std::setw(17) << fms(a1, a2, a3);
    debug_clear();
    std::cout << std::setw(20) << type2char(fms(a1, a2, a3));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> a1 = 0.70711;
    double a2 = static_cast<double>(-3.14159);
    mpfr::real<512> a3 = 0.33333;

    std::cout << std::setw(22) << std::left << "fms(a1, a2, a3)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "double"
              << std::setw(12) << "-3.14159"
              << std::setw(20) << "mpfr::real<512>"
              << std::setw(12) << "0.33333"
              << std::setw(17) << fms(a1, a2, a3);
    debug_clear();
    std::cout << std::setw(20) << type2char(fms(a1, a2, a3));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> a1 = 0.70711;
    double a2 = static_cast<double>(-3.14159);
    double a3 = static_cast<double>(0.33333);

    std::cout << std::setw(22) << std::left << "fms(a1, a2, a3)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "double"
              << std::setw(12) << "-3.14159"
              << std::setw(20) << "double"
              << std::setw(12) << "0.33333"
              << std::setw(17) << fms(a1, a2, a3);
    debug_clear();
    std::cout << std::setw(20) << type2char(fms(a1, a2, a3));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;
    mpfr::real<1024> a2 = -3.14159;
    mpfr::real<1024> a3 = 0.33333;

    std::cout << std::setw(22) << std::left << "fms(a1, a2, a3)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "-3.14159"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.33333"
              << std::setw(17) << fms(a1, a2, a3);
    debug_clear();
    std::cout << std::setw(20) << type2char(fms(a1, a2, a3));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;
    mpfr::real<1024> a2 = -3.14159;
    mpfr::real<2048> a3 = 0.33333;

    std::cout << std::setw(22) << std::left << "fms(a1, a2, a3)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "-3.14159"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.33333"
              << std::setw(17) << fms(a1, a2, a3);
    debug_clear();
    std::cout << std::setw(20) << type2char(fms(a1, a2, a3));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;
    mpfr::real<1024> a2 = -3.14159;
    mpfr::real<512> a3 = 0.33333;

    std::cout << std::setw(22) << std::left << "fms(a1, a2, a3)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "-3.14159"
              << std::setw(20) << "mpfr::real<512>"
              << std::setw(12) << "0.33333"
              << std::setw(17) << fms(a1, a2, a3);
    debug_clear();
    std::cout << std::setw(20) << type2char(fms(a1, a2, a3));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;
    mpfr::real<1024> a2 = -3.14159;
    double a3 = static_cast<double>(0.33333);

    std::cout << std::setw(22) << std::left << "fms(a1, a2, a3)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "-3.14159"
              << std::setw(20) << "double"
              << std::setw(12) << "0.33333"
              << std::setw(17) << fms(a1, a2, a3);
    debug_clear();
    std::cout << std::setw(20) << type2char(fms(a1, a2, a3));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;
    mpfr::real<2048> a2 = -3.14159;
    mpfr::real<1024> a3 = 0.33333;

    std::cout << std::setw(22) << std::left << "fms(a1, a2, a3)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "-3.14159"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.33333"
              << std::setw(17) << fms(a1, a2, a3);
    debug_clear();
    std::cout << std::setw(20) << type2char(fms(a1, a2, a3));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;
    mpfr::real<2048> a2 = -3.14159;
    mpfr::real<2048> a3 = 0.33333;

    std::cout << std::setw(22) << std::left << "fms(a1, a2, a3)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "-3.14159"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.33333"
              << std::setw(17) << fms(a1, a2, a3);
    debug_clear();
    std::cout << std::setw(20) << type2char(fms(a1, a2, a3));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;
    mpfr::real<2048> a2 = -3.14159;
    mpfr::real<512> a3 = 0.33333;

    std::cout << std::setw(22) << std::left << "fms(a1, a2, a3)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "-3.14159"
              << std::setw(20) << "mpfr::real<512>"
              << std::setw(12) << "0.33333"
              << std::setw(17) << fms(a1, a2, a3);
    debug_clear();
    std::cout << std::setw(20) << type2char(fms(a1, a2, a3));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;
    mpfr::real<2048> a2 = -3.14159;
    double a3 = static_cast<double>(0.33333);

    std::cout << std::setw(22) << std::left << "fms(a1, a2, a3)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "-3.14159"
              << std::setw(20) << "double"
              << std::setw(12) << "0.33333"
              << std::setw(17) << fms(a1, a2, a3);
    debug_clear();
    std::cout << std::setw(20) << type2char(fms(a1, a2, a3));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;
    double a2 = static_cast<double>(-3.14159);
    mpfr::real<1024> a3 = 0.33333;

    std::cout << std::setw(22) << std::left << "fms(a1, a2, a3)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "double"
              << std::setw(12) << "-3.14159"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.33333"
              << std::setw(17) << fms(a1, a2, a3);
    debug_clear();
    std::cout << std::setw(20) << type2char(fms(a1, a2, a3));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;
    double a2 = static_cast<double>(-3.14159);
    mpfr::real<2048> a3 = 0.33333;

    std::cout << std::setw(22) << std::left << "fms(a1, a2, a3)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "double"
              << std::setw(12) << "-3.14159"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.33333"
              << std::setw(17) << fms(a1, a2, a3);
    debug_clear();
    std::cout << std::setw(20) << type2char(fms(a1, a2, a3));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;
    double a2 = static_cast<double>(-3.14159);
    mpfr::real<512> a3 = 0.33333;

    std::cout << std::setw(22) << std::left << "fms(a1, a2, a3)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "double"
              << std::setw(12) << "-3.14159"
              << std::setw(20) << "mpfr::real<512>"
              << std::setw(12) << "0.33333"
              << std::setw(17) << fms(a1, a2, a3);
    debug_clear();
    std::cout << std::setw(20) << type2char(fms(a1, a2, a3));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;
    double a2 = static_cast<double>(-3.14159);
    double a3 = static_cast<double>(0.33333);

    std::cout << std::setw(22) << std::left << "fms(a1, a2, a3)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "double"
              << std::setw(12) << "-3.14159"
              << std::setw(20) << "double"
              << std::setw(12) << "0.33333"
              << std::setw(17) << fms(a1, a2, a3);
    debug_clear();
    std::cout << std::setw(20) << type2char(fms(a1, a2, a3));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    double a1 = static_cast<double>(0.70711);
    mpfr::real<1024> a2 = -3.14159;
    mpfr::real<1024> a3 = 0.33333;

    std::cout << std::setw(22) << std::left << "fms(a1, a2, a3)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "-3.14159"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.33333"
              << std::setw(17) << fms(a1, a2, a3);
    debug_clear();
    std::cout << std::setw(20) << type2char(fms(a1, a2, a3));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    double a1 = static_cast<double>(0.70711);
    mpfr::real<1024> a2 = -3.14159;
    mpfr::real<2048> a3 = 0.33333;

    std::cout << std::setw(22) << std::left << "fms(a1, a2, a3)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "-3.14159"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.33333"
              << std::setw(17) << fms(a1, a2, a3);
    debug_clear();
    std::cout << std::setw(20) << type2char(fms(a1, a2, a3));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    double a1 = static_cast<double>(0.70711);
    mpfr::real<1024> a2 = -3.14159;
    mpfr::real<512> a3 = 0.33333;

    std::cout << std::setw(22) << std::left << "fms(a1, a2, a3)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "-3.14159"
              << std::setw(20) << "mpfr::real<512>"
              << std::setw(12) << "0.33333"
              << std::setw(17) << fms(a1, a2, a3);
    debug_clear();
    std::cout << std::setw(20) << type2char(fms(a1, a2, a3));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    double a1 = static_cast<double>(0.70711);
    mpfr::real<1024> a2 = -3.14159;
    double a3 = static_cast<double>(0.33333);

    std::cout << std::setw(22) << std::left << "fms(a1, a2, a3)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "-3.14159"
              << std::setw(20) << "double"
              << std::setw(12) << "0.33333"
              << std::setw(17) << fms(a1, a2, a3);
    debug_clear();
    std::cout << std::setw(20) << type2char(fms(a1, a2, a3));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    double a1 = static_cast<double>(0.70711);
    mpfr::real<2048> a2 = -3.14159;
    mpfr::real<1024> a3 = 0.33333;

    std::cout << std::setw(22) << std::left << "fms(a1, a2, a3)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "-3.14159"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.33333"
              << std::setw(17) << fms(a1, a2, a3);
    debug_clear();
    std::cout << std::setw(20) << type2char(fms(a1, a2, a3));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    double a1 = static_cast<double>(0.70711);
    mpfr::real<2048> a2 = -3.14159;
    mpfr::real<2048> a3 = 0.33333;

    std::cout << std::setw(22) << std::left << "fms(a1, a2, a3)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "-3.14159"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.33333"
              << std::setw(17) << fms(a1, a2, a3);
    debug_clear();
    std::cout << std::setw(20) << type2char(fms(a1, a2, a3));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    double a1 = static_cast<double>(0.70711);
    mpfr::real<2048> a2 = -3.14159;
    mpfr::real<512> a3 = 0.33333;

    std::cout << std::setw(22) << std::left << "fms(a1, a2, a3)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "-3.14159"
              << std::setw(20) << "mpfr::real<512>"
              << std::setw(12) << "0.33333"
              << std::setw(17) << fms(a1, a2, a3);
    debug_clear();
    std::cout << std::setw(20) << type2char(fms(a1, a2, a3));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    double a1 = static_cast<double>(0.70711);
    mpfr::real<2048> a2 = -3.14159;
    double a3 = static_cast<double>(0.33333);

    std::cout << std::setw(22) << std::left << "fms(a1, a2, a3)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "-3.14159"
              << std::setw(20) << "double"
              << std::setw(12) << "0.33333"
              << std::setw(17) << fms(a1, a2, a3);
    debug_clear();
    std::cout << std::setw(20) << type2char(fms(a1, a2, a3));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    double a1 = static_cast<double>(0.70711);
    double a2 = static_cast<double>(-3.14159);
    mpfr::real<1024> a3 = 0.33333;

    std::cout << std::setw(22) << std::left << "fms(a1, a2, a3)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "double"
              << std::setw(12) << "-3.14159"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.33333"
              << std::setw(17) << fms(a1, a2, a3);
    debug_clear();
    std::cout << std::setw(20) << type2char(fms(a1, a2, a3));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    double a1 = static_cast<double>(0.70711);
    double a2 = static_cast<double>(-3.14159);
    mpfr::real<2048> a3 = 0.33333;

    std::cout << std::setw(22) << std::left << "fms(a1, a2, a3)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "double"
              << std::setw(12) << "-3.14159"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.33333"
              << std::setw(17) << fms(a1, a2, a3);
    debug_clear();
    std::cout << std::setw(20) << type2char(fms(a1, a2, a3));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    double a1 = static_cast<double>(0.70711);
    double a2 = static_cast<double>(-3.14159);
    mpfr::real<512> a3 = 0.33333;

    std::cout << std::setw(22) << std::left << "fms(a1, a2, a3)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "double"
              << std::setw(12) << "-3.14159"
              << std::setw(20) << "mpfr::real<512>"
              << std::setw(12) << "0.33333"
              << std::setw(17) << fms(a1, a2, a3);
    debug_clear();
    std::cout << std::setw(20) << type2char(fms(a1, a2, a3));
    std::cout << "    " << debug_out() << std::endl;
  }



  // //////////////////////////////////////////////////////////////////
  // functions without simple mapping to MPFR functions
  // //////////////////////////////////////////////////////////////////

  {
    mpfr::real<1024> a1 = 0.70711;
    long int a2 = static_cast<long int>(5);

    std::cout << std::setw(22) << std::left << "ldexp(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "long int"
              << std::setw(12) << "5"
              << std::setw(17) << ldexp(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(ldexp(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;
    long int a2 = static_cast<long int>(5);

    std::cout << std::setw(22) << std::left << "ldexp(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "long int"
              << std::setw(12) << "5"
              << std::setw(17) << ldexp(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(ldexp(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    double a1 = static_cast<double>(0.70711);
    long int a2 = static_cast<long int>(5);

    std::cout << std::setw(22) << std::left << "ldexp(a1, a2)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "long int"
              << std::setw(12) << "5"
              << std::setw(17) << ldexp(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(ldexp(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }


  {
    mpfr::real<1024> a1 = 0.70711;
    long int a2 = static_cast<long int>(5);

    std::cout << std::setw(22) << std::left << "scalbln(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "long int"
              << std::setw(12) << "5"
              << std::setw(17) << scalbln(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(scalbln(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;
    long int a2 = static_cast<long int>(5);

    std::cout << std::setw(22) << std::left << "scalbln(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "long int"
              << std::setw(12) << "5"
              << std::setw(17) << scalbln(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(scalbln(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

#ifdef MPFR_REAL_MATH_H_POSIX
  {
    double a1 = static_cast<double>(0.70711);
    long int a2 = static_cast<long int>(5);

    std::cout << std::setw(22) << std::left << "scalbln(a1, a2)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "long int"
              << std::setw(12) << "5"
              << std::setw(17) << scalbln(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(scalbln(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }
#endif  // MPFR_REAL_MATH_H_POSIX


  {
    mpfr::real<1024> a1 = 0.70711;
    int a2 = static_cast<int>(5);

    std::cout << std::setw(22) << std::left << "scalbn(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "int"
              << std::setw(12) << "5"
              << std::setw(17) << scalbn(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(scalbn(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;
    int a2 = static_cast<int>(5);

    std::cout << std::setw(22) << std::left << "scalbn(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "int"
              << std::setw(12) << "5"
              << std::setw(17) << scalbn(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(scalbn(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

#ifdef MPFR_REAL_MATH_H_POSIX
  {
    double a1 = static_cast<double>(0.70711);
    int a2 = static_cast<int>(5);

    std::cout << std::setw(22) << std::left << "scalbn(a1, a2)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "int"
              << std::setw(12) << "5"
              << std::setw(17) << scalbn(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(scalbn(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }
#endif  // MPFR_REAL_MATH_H_POSIX


  {
    mpfr::real<1024> a1 = 0.70711;
    unsigned long int a2 = static_cast<unsigned long int>(5);

    std::cout << std::setw(22) << std::left << "root(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "unsigned long int"
              << std::setw(12) << "5"
              << std::setw(17) << root(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(root(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;
    unsigned long int a2 = static_cast<unsigned long int>(5);

    std::cout << std::setw(22) << std::left << "root(a1, a2)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "unsigned long int"
              << std::setw(12) << "5"
              << std::setw(17) << root(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(root(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }



  {
    long int a1 = static_cast<long int>(5);
    mpfr::real<1024> a2 = 0.70711;

    std::cout << std::setw(22) << std::left << "jn(a1, a2)"
              << std::right << std::setw(20) << "long int"
              << std::setw(12) << "5"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << jn(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(jn(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    long int a1 = static_cast<long int>(5);
    mpfr::real<2048> a2 = 0.70711;

    std::cout << std::setw(22) << std::left << "jn(a1, a2)"
              << std::right << std::setw(20) << "long int"
              << std::setw(12) << "5"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << jn(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(jn(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

#ifdef MPFR_REAL_MATH_H_POSIX
  {
    long int a1 = static_cast<long int>(5);
    double a2 = static_cast<double>(0.70711);

    std::cout << std::setw(22) << std::left << "jn(a1, a2)"
              << std::right << std::setw(20) << "long int"
              << std::setw(12) << "5"
              << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(17) << jn(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(jn(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }
#endif  // MPFR_REAL_MATH_H_POSIX


  {
    long int a1 = static_cast<long int>(5);
    mpfr::real<1024> a2 = 0.70711;

    std::cout << std::setw(22) << std::left << "yn(a1, a2)"
              << std::right << std::setw(20) << "long int"
              << std::setw(12) << "5"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << yn(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(yn(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    long int a1 = static_cast<long int>(5);
    mpfr::real<2048> a2 = 0.70711;

    std::cout << std::setw(22) << std::left << "yn(a1, a2)"
              << std::right << std::setw(20) << "long int"
              << std::setw(12) << "5"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << yn(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(yn(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

#ifdef MPFR_REAL_MATH_H_POSIX
  {
    long int a1 = static_cast<long int>(5);
    double a2 = static_cast<double>(0.70711);

    std::cout << std::setw(22) << std::left << "yn(a1, a2)"
              << std::right << std::setw(20) << "long int"
              << std::setw(12) << "5"
              << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(17) << yn(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(yn(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }
#endif  // MPFR_REAL_MATH_H_POSIX


  {
    long int a1 = static_cast<long int>(5);
    mpfr::real<1024> a2 = 0.70711;

    std::cout << std::setw(22) << std::left << "cyl_bessel_j(a1, a2)"
              << std::right << std::setw(20) << "long int"
              << std::setw(12) << "5"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << cyl_bessel_j(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(cyl_bessel_j(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    long int a1 = static_cast<long int>(5);
    mpfr::real<2048> a2 = 0.70711;

    std::cout << std::setw(22) << std::left << "cyl_bessel_j(a1, a2)"
              << std::right << std::setw(20) << "long int"
              << std::setw(12) << "5"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << cyl_bessel_j(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(cyl_bessel_j(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }



  {
    long int a1 = static_cast<long int>(5);
    mpfr::real<1024> a2 = 0.70711;

    std::cout << std::setw(22) << std::left << "cyl_neumann(a1, a2)"
              << std::right << std::setw(20) << "long int"
              << std::setw(12) << "5"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << cyl_neumann(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(cyl_neumann(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    long int a1 = static_cast<long int>(5);
    mpfr::real<2048> a2 = 0.70711;

    std::cout << std::setw(22) << std::left << "cyl_neumann(a1, a2)"
              << std::right << std::setw(20) << "long int"
              << std::setw(12) << "5"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(17) << cyl_neumann(a1, a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(cyl_neumann(a1, a2));
    std::cout << "    " << debug_out() << std::endl;
  }



  // //////////////////////////////////////////////////////////////////
  // functions with pointers as arguments
  // //////////////////////////////////////////////////////////////////

  {
    mpfr::real<1024> a1 = 0.70711;
    mpfr::real_exp_t a2 = 0;
    frexp(a1, &a2);

    std::cout << std::setw(22) << std::left << "frexp(a1, &a2)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real_exp_t"
              << std::setw(17) << a2
              << std::setw(17) << frexp(a1, &a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(frexp(a1, &a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;
    mpfr::real_exp_t a2 = 0;
    frexp(a1, &a2);

    std::cout << std::setw(22) << std::left << "frexp(a1, &a2)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real_exp_t"
              << std::setw(17) << a2
              << std::setw(17) << frexp(a1, &a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(frexp(a1, &a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    double a1 = static_cast<double>(0.70711);
    int a2 = static_cast<int>(0);
    frexp(a1, &a2);

    std::cout << std::setw(22) << std::left << "frexp(a1, &a2)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "int"
              << std::setw(17) << a2
              << std::setw(17) << frexp(a1, &a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(frexp(a1, &a2));
    std::cout << "    " << debug_out() << std::endl;
  }


  {
    mpfr::real<1024> a1 = 0.70711;
    mpfr::real<1024> a2 = 0;
    modf(a1, &a2);

    std::cout << std::setw(22) << std::left << "modf(a1, &a2)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(17) << a2
              << std::setw(17) << modf(a1, &a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(modf(a1, &a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> a1 = 0.70711;
    mpfr::real<2048> a2 = 0;
    modf(a1, &a2);

    std::cout << std::setw(22) << std::left << "modf(a1, &a2)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(17) << a2
              << std::setw(17) << modf(a1, &a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(modf(a1, &a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;
    mpfr::real<1024> a2 = 0;
    modf(a1, &a2);

    std::cout << std::setw(22) << std::left << "modf(a1, &a2)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(17) << a2
              << std::setw(17) << modf(a1, &a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(modf(a1, &a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;
    mpfr::real<2048> a2 = 0;
    modf(a1, &a2);

    std::cout << std::setw(22) << std::left << "modf(a1, &a2)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(17) << a2
              << std::setw(17) << modf(a1, &a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(modf(a1, &a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    double a1 = static_cast<double>(0.70711);
    mpfr::real<1024> a2 = 0;
    modf(a1, &a2);

    std::cout << std::setw(22) << std::left << "modf(a1, &a2)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(17) << a2
              << std::setw(17) << modf(a1, &a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(modf(a1, &a2));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    double a1 = static_cast<double>(0.70711);
    mpfr::real<2048> a2 = 0;
    modf(a1, &a2);

    std::cout << std::setw(22) << std::left << "modf(a1, &a2)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(17) << a2
              << std::setw(17) << modf(a1, &a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(modf(a1, &a2));
    std::cout << "    " << debug_out() << std::endl;
  }


  {
    double a1 = static_cast<double>(0.70711);
    double a2 = static_cast<double>(0);
    modf(a1, &a2);

    std::cout << std::setw(22) << std::left << "modf(a1, &a2)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "double"
              << std::setw(17) << a2
              << std::setw(17) << modf(a1, &a2);
    debug_clear();
    std::cout << std::setw(20) << type2char(modf(a1, &a2));
    std::cout << "    " << debug_out() << std::endl;
  }


  {
    mpfr::real<1024> a1 = 0.70711;
    mpfr::real<1024> a2 = -3.14159;
    long int a3 = static_cast<long int>(0);
    remquo(a1, a2, &a3);

    std::cout << std::setw(22) << std::left << "remquo(a1, a2, &a3)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "-3.14159"
              << std::setw(20) << "long int"
              << std::setw(17) << a3
              << std::setw(17) << remquo(a1, a2, &a3);
    debug_clear();
    std::cout << std::setw(20) << type2char(remquo(a1, a2, &a3));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> a1 = 0.70711;
    mpfr::real<2048> a2 = -3.14159;
    long int a3 = static_cast<long int>(0);
    remquo(a1, a2, &a3);

    std::cout << std::setw(22) << std::left << "remquo(a1, a2, &a3)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "-3.14159"
              << std::setw(20) << "long int"
              << std::setw(17) << a3
              << std::setw(17) << remquo(a1, a2, &a3);
    debug_clear();
    std::cout << std::setw(20) << type2char(remquo(a1, a2, &a3));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> a1 = 0.70711;
    double a2 = static_cast<double>(-3.14159);
    long int a3 = static_cast<long int>(0);
    remquo(a1, a2, &a3);

    std::cout << std::setw(22) << std::left << "remquo(a1, a2, &a3)"
              << std::right << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "double"
              << std::setw(12) << "-3.14159"
              << std::setw(20) << "long int"
              << std::setw(17) << a3
              << std::setw(17) << remquo(a1, a2, &a3);
    debug_clear();
    std::cout << std::setw(20) << type2char(remquo(a1, a2, &a3));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;
    mpfr::real<1024> a2 = -3.14159;
    long int a3 = static_cast<long int>(0);
    remquo(a1, a2, &a3);

    std::cout << std::setw(22) << std::left << "remquo(a1, a2, &a3)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "-3.14159"
              << std::setw(20) << "long int"
              << std::setw(17) << a3
              << std::setw(17) << remquo(a1, a2, &a3);
    debug_clear();
    std::cout << std::setw(20) << type2char(remquo(a1, a2, &a3));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;
    mpfr::real<2048> a2 = -3.14159;
    long int a3 = static_cast<long int>(0);
    remquo(a1, a2, &a3);

    std::cout << std::setw(22) << std::left << "remquo(a1, a2, &a3)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "-3.14159"
              << std::setw(20) << "long int"
              << std::setw(17) << a3
              << std::setw(17) << remquo(a1, a2, &a3);
    debug_clear();
    std::cout << std::setw(20) << type2char(remquo(a1, a2, &a3));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<2048> a1 = 0.70711;
    double a2 = static_cast<double>(-3.14159);
    long int a3 = static_cast<long int>(0);
    remquo(a1, a2, &a3);

    std::cout << std::setw(22) << std::left << "remquo(a1, a2, &a3)"
              << std::right << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "double"
              << std::setw(12) << "-3.14159"
              << std::setw(20) << "long int"
              << std::setw(17) << a3
              << std::setw(17) << remquo(a1, a2, &a3);
    debug_clear();
    std::cout << std::setw(20) << type2char(remquo(a1, a2, &a3));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    double a1 = static_cast<double>(0.70711);
    mpfr::real<1024> a2 = -3.14159;
    long int a3 = static_cast<long int>(0);
    remquo(a1, a2, &a3);

    std::cout << std::setw(22) << std::left << "remquo(a1, a2, &a3)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<1024>"
              << std::setw(12) << "-3.14159"
              << std::setw(20) << "long int"
              << std::setw(17) << a3
              << std::setw(17) << remquo(a1, a2, &a3);
    debug_clear();
    std::cout << std::setw(20) << type2char(remquo(a1, a2, &a3));
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    double a1 = static_cast<double>(0.70711);
    mpfr::real<2048> a2 = -3.14159;
    long int a3 = static_cast<long int>(0);
    remquo(a1, a2, &a3);

    std::cout << std::setw(22) << std::left << "remquo(a1, a2, &a3)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "mpfr::real<2048>"
              << std::setw(12) << "-3.14159"
              << std::setw(20) << "long int"
              << std::setw(17) << a3
              << std::setw(17) << remquo(a1, a2, &a3);
    debug_clear();
    std::cout << std::setw(20) << type2char(remquo(a1, a2, &a3));
    std::cout << "    " << debug_out() << std::endl;
  }

#ifdef MPFR_REAL_MATH_H_POSIX
  {
    double a1 = static_cast<double>(0.70711);
    double a2 = static_cast<double>(-3.14159);
    int a3 = static_cast<int>(0);
    remquo(a1, a2, &a3);

    std::cout << std::setw(22) << std::left << "remquo(a1, a2, &a3)"
              << std::right << std::setw(20) << "double"
              << std::setw(12) << "0.70711"
              << std::setw(20) << "double"
              << std::setw(12) << "-3.14159"
              << std::setw(20) << "int"
              << std::setw(17) << a3
              << std::setw(17) << remquo(a1, a2, &a3);
    debug_clear();
    std::cout << std::setw(20) << type2char(remquo(a1, a2, &a3));
    std::cout << "    " << debug_out() << std::endl;
  }
#endif  // MPFR_REAL_MATH_H_POSIX


  return 0;
}
