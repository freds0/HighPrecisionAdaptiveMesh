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

#define MPFR_REAL_ENABLE_CONV_OPS 1

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
// main function
// ////////////////////////////////////////////////////////////////////

int main () {
  std::cout.flags(std::ios_base::scientific);

  std::cout << "# mpfr::real operator test" << std::endl
            << "#" << std::endl
            << "# types of variables:          r: mpfr::real<1024>, x: other type"
            << std::endl
            << "# initial values (literally):  r <- -1.41421, x <- 3.14159 "
            << std::endl
            << "#" << std::endl;
  std::cout << "# operation "
            << std::setw(22) << "type of x"
            << std::setw(17) << "num. result"
            << std::setw(20) << "return type"
            << "    called MPFR functions" << std::endl;
  std::cout << "# ==============================================="
            << "================================================="
            << std::endl;

  // ////////////////////////////////////////////////////////////////////
  // implicit and explicit constructor test
  // ////////////////////////////////////////////////////////////////////

  {
    unsigned long int x = static_cast<unsigned long int>(3.14159);

    debug_clear();
    mpfr::real<1024> r = x;
    std::string temp = debug_out();
    std::cout << "r = x (ctor)"
              << std::setw(22) << "unsigned long int"
              << std::setw(17) << r
              << std::setw(20) << "-"
              << "    " << temp << std::endl;
  }

  {
    long int x = static_cast<long int>(3.14159);

    debug_clear();
    mpfr::real<1024> r = x;
    std::string temp = debug_out();
    std::cout << "r = x (ctor)"
              << std::setw(22) << "long int"
              << std::setw(17) << r
              << std::setw(20) << "-"
              << "    " << temp << std::endl;
  }

  {
    unsigned int x = static_cast<unsigned int>(3.14159);

    debug_clear();
    mpfr::real<1024> r = x;
    std::string temp = debug_out();
    std::cout << "r = x (ctor)"
              << std::setw(22) << "unsigned int"
              << std::setw(17) << r
              << std::setw(20) << "-"
              << "    " << temp << std::endl;
  }

  {
    int x = static_cast<int>(3.14159);

    debug_clear();
    mpfr::real<1024> r = x;
    std::string temp = debug_out();
    std::cout << "r = x (ctor)"
              << std::setw(22) << "int"
              << std::setw(17) << r
              << std::setw(20) << "-"
              << "    " << temp << std::endl;
  }

  {
    unsigned short int x = static_cast<unsigned short int>(3.14159);

    debug_clear();
    mpfr::real<1024> r = x;
    std::string temp = debug_out();
    std::cout << "r = x (ctor)"
              << std::setw(22) << "unsigned short int"
              << std::setw(17) << r
              << std::setw(20) << "-"
              << "    " << temp << std::endl;
  }

  {
    short int x = static_cast<short int>(3.14159);

    debug_clear();
    mpfr::real<1024> r = x;
    std::string temp = debug_out();
    std::cout << "r = x (ctor)"
              << std::setw(22) << "short int"
              << std::setw(17) << r
              << std::setw(20) << "-"
              << "    " << temp << std::endl;
  }

  {
    unsigned char x = static_cast<unsigned char>(3.14159);

    debug_clear();
    mpfr::real<1024> r = x;
    std::string temp = debug_out();
    std::cout << "r = x (ctor)"
              << std::setw(22) << "unsigned char"
              << std::setw(17) << r
              << std::setw(20) << "-"
              << "    " << temp << std::endl;
  }

  {
    signed char x = static_cast<signed char>(3.14159);

    debug_clear();
    mpfr::real<1024> r = x;
    std::string temp = debug_out();
    std::cout << "r = x (ctor)"
              << std::setw(22) << "signed char"
              << std::setw(17) << r
              << std::setw(20) << "-"
              << "    " << temp << std::endl;
  }

  {
    char x = static_cast<char>(3.14159);

    debug_clear();
    mpfr::real<1024> r = x;
    std::string temp = debug_out();
    std::cout << "r = x (ctor)"
              << std::setw(22) << "char"
              << std::setw(17) << r
              << std::setw(20) << "-"
              << "    " << temp << std::endl;
  }

  {
    double x = static_cast<double>(3.14159);

    debug_clear();
    mpfr::real<1024> r = x;
    std::string temp = debug_out();
    std::cout << "r = x (ctor)"
              << std::setw(22) << "double"
              << std::setw(17) << r
              << std::setw(20) << "-"
              << "    " << temp << std::endl;
  }

  {
    float x = static_cast<float>(3.14159);

    debug_clear();
    mpfr::real<1024> r = x;
    std::string temp = debug_out();
    std::cout << "r = x (ctor)"
              << std::setw(22) << "float"
              << std::setw(17) << r
              << std::setw(20) << "-"
              << "    " << temp << std::endl;
  }

  {
    long double x = static_cast<long double>(3.14159);

    debug_clear();
    mpfr::real<1024> r = x;
    std::string temp = debug_out();
    std::cout << "r = x (ctor)"
              << std::setw(22) << "long double"
              << std::setw(17) << r
              << std::setw(20) << "-"
              << "    " << temp << std::endl;
  }

#ifndef _MSC_VER
  {
    mpz_t x;
    my_mp_init(x, 3.14159);

    debug_clear();
    mpfr::real<1024> r = x;
    std::string temp = debug_out();
    std::cout << "r = x (ctor)"
              << std::setw(22) << "mpz_t"
              << std::setw(17) << r
              << std::setw(20) << "-"
              << "    " << temp << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

#ifndef _MSC_VER
  {
    mpq_t x;
    my_mp_init(x, 3.14159);

    debug_clear();
    mpfr::real<1024> r = x;
    std::string temp = debug_out();
    std::cout << "r = x (ctor)"
              << std::setw(22) << "mpq_t"
              << std::setw(17) << r
              << std::setw(20) << "-"
              << "    " << temp << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

#ifndef _MSC_VER
  {
    mpf_t x;
    my_mp_init(x, 3.14159);

    debug_clear();
    mpfr::real<1024> r = x;
    std::string temp = debug_out();
    std::cout << "r = x (ctor)"
              << std::setw(22) << "mpf_t"
              << std::setw(17) << r
              << std::setw(20) << "-"
              << "    " << temp << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

#ifndef _MSC_VER
  {
    mpfr_t x;
    my_mp_init(x, 3.14159);

    debug_clear();
    mpfr::real<1024> r = x;
    std::string temp = debug_out();
    std::cout << "r = x (ctor)"
              << std::setw(22) << "mpfr_t"
              << std::setw(17) << r
              << std::setw(20) << "-"
              << "    " << temp << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

  {
    mpz_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpz_ptr x = x_nc;

    debug_clear();
    mpfr::real<1024> r = x;
    std::string temp = debug_out();
    std::cout << "r = x (ctor)"
              << std::setw(22) << "mpz_ptr"
              << std::setw(17) << r
              << std::setw(20) << "-"
              << "    " << temp << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpq_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpq_ptr x = x_nc;

    debug_clear();
    mpfr::real<1024> r = x;
    std::string temp = debug_out();
    std::cout << "r = x (ctor)"
              << std::setw(22) << "mpq_ptr"
              << std::setw(17) << r
              << std::setw(20) << "-"
              << "    " << temp << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpf_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpf_ptr x = x_nc;

    debug_clear();
    mpfr::real<1024> r = x;
    std::string temp = debug_out();
    std::cout << "r = x (ctor)"
              << std::setw(22) << "mpf_ptr"
              << std::setw(17) << r
              << std::setw(20) << "-"
              << "    " << temp << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpfr_ptr x = x_nc;

    debug_clear();
    mpfr::real<1024> r = x;
    std::string temp = debug_out();
    std::cout << "r = x (ctor)"
              << std::setw(22) << "mpfr_ptr"
              << std::setw(17) << r
              << std::setw(20) << "-"
              << "    " << temp << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpz_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpz_srcptr x = x_nc;

    debug_clear();
    mpfr::real<1024> r = x;
    std::string temp = debug_out();
    std::cout << "r = x (ctor)"
              << std::setw(22) << "mpz_srcptr"
              << std::setw(17) << r
              << std::setw(20) << "-"
              << "    " << temp << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpq_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpq_srcptr x = x_nc;

    debug_clear();
    mpfr::real<1024> r = x;
    std::string temp = debug_out();
    std::cout << "r = x (ctor)"
              << std::setw(22) << "mpq_srcptr"
              << std::setw(17) << r
              << std::setw(20) << "-"
              << "    " << temp << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpf_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpf_srcptr x = x_nc;

    debug_clear();
    mpfr::real<1024> r = x;
    std::string temp = debug_out();
    std::cout << "r = x (ctor)"
              << std::setw(22) << "mpf_srcptr"
              << std::setw(17) << r
              << std::setw(20) << "-"
              << "    " << temp << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpfr_srcptr x = x_nc;

    debug_clear();
    mpfr::real<1024> r = x;
    std::string temp = debug_out();
    std::cout << "r = x (ctor)"
              << std::setw(22) << "mpfr_srcptr"
              << std::setw(17) << r
              << std::setw(20) << "-"
              << "    " << temp << std::endl;

    my_mp_clear(x_nc);
  }

  {
    std::string x = "3.14159";

    debug_clear();
    mpfr::real<1024> r = x;
    std::string temp = debug_out();
    std::cout << "r = x (ctor)"
              << std::setw(22) << "std::string"
              << std::setw(17) << r
              << std::setw(20) << "-"
              << "    " << temp << std::endl;
  }

  {
    char x_nc[] = "3.14159";
    char* x = x_nc;

    debug_clear();
    mpfr::real<1024> r = x;
    std::string temp = debug_out();
    std::cout << "r = x (ctor)"
              << std::setw(22) << "char*"
              << std::setw(17) << r
              << std::setw(20) << "-"
              << "    " << temp << std::endl;
  }

  {
    char x_nc[] = "3.14159";
    const char* x = x_nc;

    debug_clear();
    mpfr::real<1024> r = x;
    std::string temp = debug_out();
    std::cout << "r = x (ctor)"
              << std::setw(22) << "const char*"
              << std::setw(17) << r
              << std::setw(20) << "-"
              << "    " << temp << std::endl;
  }

#ifndef _MSC_VER
  {
    char x[] = "3.14159";

    debug_clear();
    mpfr::real<1024> r = x;
    std::string temp = debug_out();
    std::cout << "r = x (ctor)"
              << std::setw(22) << "char[]"
              << std::setw(17) << r
              << std::setw(20) << "-"
              << "    " << temp << std::endl;
  }
#endif  // _MSC_VER

  {
    mpfr::real<512> x = 3.14159;

    debug_clear();
    mpfr::real<1024> r = x;
    std::string temp = debug_out();
    std::cout << "r = x (ctor)"
              << std::setw(22) << "mpfr::real<512>"
              << std::setw(17) << r
              << std::setw(20) << "-"
              << "    " << temp << std::endl;
  }

  {
    mpfr::real<1024> x = 3.14159;

    debug_clear();
    mpfr::real<1024> r = x;
    std::string temp = debug_out();
    std::cout << "r = x (ctor)"
              << std::setw(22) << "mpfr::real<1024>"
              << std::setw(17) << r
              << std::setw(20) << "-"
              << "    " << temp << std::endl;
  }

  {
    mpfr::real<2048> x = 3.14159;

    debug_clear();
    mpfr::real<1024> r = x;
    std::string temp = debug_out();
    std::cout << "r = x (ctor)"
              << std::setw(22) << "mpfr::real<2048>"
              << std::setw(17) << r
              << std::setw(20) << "-"
              << "    " << temp << std::endl;
  }


  {
    unsigned long int x = static_cast<unsigned long int>(3.14159);

    debug_clear();
    mpfr::real<1024> r(x);
    std::string temp = debug_out();
    std::cout << "r(x)  (ctor)"
              << std::setw(22) << "unsigned long int"
              << std::setw(17) << r
              << std::setw(20) << "-"
              << "    " << temp << std::endl;
  }

  {
    long int x = static_cast<long int>(3.14159);

    debug_clear();
    mpfr::real<1024> r(x);
    std::string temp = debug_out();
    std::cout << "r(x)  (ctor)"
              << std::setw(22) << "long int"
              << std::setw(17) << r
              << std::setw(20) << "-"
              << "    " << temp << std::endl;
  }

  {
    unsigned int x = static_cast<unsigned int>(3.14159);

    debug_clear();
    mpfr::real<1024> r(x);
    std::string temp = debug_out();
    std::cout << "r(x)  (ctor)"
              << std::setw(22) << "unsigned int"
              << std::setw(17) << r
              << std::setw(20) << "-"
              << "    " << temp << std::endl;
  }

  {
    int x = static_cast<int>(3.14159);

    debug_clear();
    mpfr::real<1024> r(x);
    std::string temp = debug_out();
    std::cout << "r(x)  (ctor)"
              << std::setw(22) << "int"
              << std::setw(17) << r
              << std::setw(20) << "-"
              << "    " << temp << std::endl;
  }

  {
    unsigned short int x = static_cast<unsigned short int>(3.14159);

    debug_clear();
    mpfr::real<1024> r(x);
    std::string temp = debug_out();
    std::cout << "r(x)  (ctor)"
              << std::setw(22) << "unsigned short int"
              << std::setw(17) << r
              << std::setw(20) << "-"
              << "    " << temp << std::endl;
  }

  {
    short int x = static_cast<short int>(3.14159);

    debug_clear();
    mpfr::real<1024> r(x);
    std::string temp = debug_out();
    std::cout << "r(x)  (ctor)"
              << std::setw(22) << "short int"
              << std::setw(17) << r
              << std::setw(20) << "-"
              << "    " << temp << std::endl;
  }

  {
    unsigned char x = static_cast<unsigned char>(3.14159);

    debug_clear();
    mpfr::real<1024> r(x);
    std::string temp = debug_out();
    std::cout << "r(x)  (ctor)"
              << std::setw(22) << "unsigned char"
              << std::setw(17) << r
              << std::setw(20) << "-"
              << "    " << temp << std::endl;
  }

  {
    signed char x = static_cast<signed char>(3.14159);

    debug_clear();
    mpfr::real<1024> r(x);
    std::string temp = debug_out();
    std::cout << "r(x)  (ctor)"
              << std::setw(22) << "signed char"
              << std::setw(17) << r
              << std::setw(20) << "-"
              << "    " << temp << std::endl;
  }

  {
    char x = static_cast<char>(3.14159);

    debug_clear();
    mpfr::real<1024> r(x);
    std::string temp = debug_out();
    std::cout << "r(x)  (ctor)"
              << std::setw(22) << "char"
              << std::setw(17) << r
              << std::setw(20) << "-"
              << "    " << temp << std::endl;
  }

  {
    double x = static_cast<double>(3.14159);

    debug_clear();
    mpfr::real<1024> r(x);
    std::string temp = debug_out();
    std::cout << "r(x)  (ctor)"
              << std::setw(22) << "double"
              << std::setw(17) << r
              << std::setw(20) << "-"
              << "    " << temp << std::endl;
  }

  {
    float x = static_cast<float>(3.14159);

    debug_clear();
    mpfr::real<1024> r(x);
    std::string temp = debug_out();
    std::cout << "r(x)  (ctor)"
              << std::setw(22) << "float"
              << std::setw(17) << r
              << std::setw(20) << "-"
              << "    " << temp << std::endl;
  }

  {
    long double x = static_cast<long double>(3.14159);

    debug_clear();
    mpfr::real<1024> r(x);
    std::string temp = debug_out();
    std::cout << "r(x)  (ctor)"
              << std::setw(22) << "long double"
              << std::setw(17) << r
              << std::setw(20) << "-"
              << "    " << temp << std::endl;
  }

#ifndef _MSC_VER
  {
    mpz_t x;
    my_mp_init(x, 3.14159);

    debug_clear();
    mpfr::real<1024> r(x);
    std::string temp = debug_out();
    std::cout << "r(x)  (ctor)"
              << std::setw(22) << "mpz_t"
              << std::setw(17) << r
              << std::setw(20) << "-"
              << "    " << temp << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

#ifndef _MSC_VER
  {
    mpq_t x;
    my_mp_init(x, 3.14159);

    debug_clear();
    mpfr::real<1024> r(x);
    std::string temp = debug_out();
    std::cout << "r(x)  (ctor)"
              << std::setw(22) << "mpq_t"
              << std::setw(17) << r
              << std::setw(20) << "-"
              << "    " << temp << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

#ifndef _MSC_VER
  {
    mpf_t x;
    my_mp_init(x, 3.14159);

    debug_clear();
    mpfr::real<1024> r(x);
    std::string temp = debug_out();
    std::cout << "r(x)  (ctor)"
              << std::setw(22) << "mpf_t"
              << std::setw(17) << r
              << std::setw(20) << "-"
              << "    " << temp << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

#ifndef _MSC_VER
  {
    mpfr_t x;
    my_mp_init(x, 3.14159);

    debug_clear();
    mpfr::real<1024> r(x);
    std::string temp = debug_out();
    std::cout << "r(x)  (ctor)"
              << std::setw(22) << "mpfr_t"
              << std::setw(17) << r
              << std::setw(20) << "-"
              << "    " << temp << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

  {
    mpz_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpz_ptr x = x_nc;

    debug_clear();
    mpfr::real<1024> r(x);
    std::string temp = debug_out();
    std::cout << "r(x)  (ctor)"
              << std::setw(22) << "mpz_ptr"
              << std::setw(17) << r
              << std::setw(20) << "-"
              << "    " << temp << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpq_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpq_ptr x = x_nc;

    debug_clear();
    mpfr::real<1024> r(x);
    std::string temp = debug_out();
    std::cout << "r(x)  (ctor)"
              << std::setw(22) << "mpq_ptr"
              << std::setw(17) << r
              << std::setw(20) << "-"
              << "    " << temp << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpf_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpf_ptr x = x_nc;

    debug_clear();
    mpfr::real<1024> r(x);
    std::string temp = debug_out();
    std::cout << "r(x)  (ctor)"
              << std::setw(22) << "mpf_ptr"
              << std::setw(17) << r
              << std::setw(20) << "-"
              << "    " << temp << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpfr_ptr x = x_nc;

    debug_clear();
    mpfr::real<1024> r(x);
    std::string temp = debug_out();
    std::cout << "r(x)  (ctor)"
              << std::setw(22) << "mpfr_ptr"
              << std::setw(17) << r
              << std::setw(20) << "-"
              << "    " << temp << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpz_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpz_srcptr x = x_nc;

    debug_clear();
    mpfr::real<1024> r(x);
    std::string temp = debug_out();
    std::cout << "r(x)  (ctor)"
              << std::setw(22) << "mpz_srcptr"
              << std::setw(17) << r
              << std::setw(20) << "-"
              << "    " << temp << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpq_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpq_srcptr x = x_nc;

    debug_clear();
    mpfr::real<1024> r(x);
    std::string temp = debug_out();
    std::cout << "r(x)  (ctor)"
              << std::setw(22) << "mpq_srcptr"
              << std::setw(17) << r
              << std::setw(20) << "-"
              << "    " << temp << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpf_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpf_srcptr x = x_nc;

    debug_clear();
    mpfr::real<1024> r(x);
    std::string temp = debug_out();
    std::cout << "r(x)  (ctor)"
              << std::setw(22) << "mpf_srcptr"
              << std::setw(17) << r
              << std::setw(20) << "-"
              << "    " << temp << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpfr_srcptr x = x_nc;

    debug_clear();
    mpfr::real<1024> r(x);
    std::string temp = debug_out();
    std::cout << "r(x)  (ctor)"
              << std::setw(22) << "mpfr_srcptr"
              << std::setw(17) << r
              << std::setw(20) << "-"
              << "    " << temp << std::endl;

    my_mp_clear(x_nc);
  }

  {
    std::string x = "3.14159";

    debug_clear();
    mpfr::real<1024> r(x);
    std::string temp = debug_out();
    std::cout << "r(x)  (ctor)"
              << std::setw(22) << "std::string"
              << std::setw(17) << r
              << std::setw(20) << "-"
              << "    " << temp << std::endl;
  }

  {
    char x_nc[] = "3.14159";
    char* x = x_nc;

    debug_clear();
    mpfr::real<1024> r(x);
    std::string temp = debug_out();
    std::cout << "r(x)  (ctor)"
              << std::setw(22) << "char*"
              << std::setw(17) << r
              << std::setw(20) << "-"
              << "    " << temp << std::endl;
  }

  {
    char x_nc[] = "3.14159";
    const char* x = x_nc;

    debug_clear();
    mpfr::real<1024> r(x);
    std::string temp = debug_out();
    std::cout << "r(x)  (ctor)"
              << std::setw(22) << "const char*"
              << std::setw(17) << r
              << std::setw(20) << "-"
              << "    " << temp << std::endl;
  }

#ifndef _MSC_VER
  {
    char x[] = "3.14159";

    debug_clear();
    mpfr::real<1024> r(x);
    std::string temp = debug_out();
    std::cout << "r(x)  (ctor)"
              << std::setw(22) << "char[]"
              << std::setw(17) << r
              << std::setw(20) << "-"
              << "    " << temp << std::endl;
  }
#endif  // _MSC_VER

  {
    mpfr::real<512> x = 3.14159;

    debug_clear();
    mpfr::real<1024> r(x);
    std::string temp = debug_out();
    std::cout << "r(x)  (ctor)"
              << std::setw(22) << "mpfr::real<512>"
              << std::setw(17) << r
              << std::setw(20) << "-"
              << "    " << temp << std::endl;
  }

  {
    mpfr::real<1024> x = 3.14159;

    debug_clear();
    mpfr::real<1024> r(x);
    std::string temp = debug_out();
    std::cout << "r(x)  (ctor)"
              << std::setw(22) << "mpfr::real<1024>"
              << std::setw(17) << r
              << std::setw(20) << "-"
              << "    " << temp << std::endl;
  }

  {
    mpfr::real<2048> x = 3.14159;

    debug_clear();
    mpfr::real<1024> r(x);
    std::string temp = debug_out();
    std::cout << "r(x)  (ctor)"
              << std::setw(22) << "mpfr::real<2048>"
              << std::setw(17) << r
              << std::setw(20) << "-"
              << "    " << temp << std::endl;
  }

  // ////////////////////////////////////////////////////////////////////
  // assignment, arithmetic, and comparison operators
  // ////////////////////////////////////////////////////////////////////

  {
    mpfr::real<1024> r = -1.41421;
    unsigned long int x = static_cast<unsigned long int>(3.14159);

    std::cout << "r " << std::setw(2) << "+=" << " x      "
              << std::setw(22) << "unsigned long int"
              << std::setw(17) << (r += x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r += x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    long int x = static_cast<long int>(3.14159);

    std::cout << "r " << std::setw(2) << "+=" << " x      "
              << std::setw(22) << "long int"
              << std::setw(17) << (r += x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r += x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    unsigned int x = static_cast<unsigned int>(3.14159);

    std::cout << "r " << std::setw(2) << "+=" << " x      "
              << std::setw(22) << "unsigned int"
              << std::setw(17) << (r += x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r += x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    int x = static_cast<int>(3.14159);

    std::cout << "r " << std::setw(2) << "+=" << " x      "
              << std::setw(22) << "int"
              << std::setw(17) << (r += x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r += x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    unsigned short int x = static_cast<unsigned short int>(3.14159);

    std::cout << "r " << std::setw(2) << "+=" << " x      "
              << std::setw(22) << "unsigned short int"
              << std::setw(17) << (r += x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r += x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    short int x = static_cast<short int>(3.14159);

    std::cout << "r " << std::setw(2) << "+=" << " x      "
              << std::setw(22) << "short int"
              << std::setw(17) << (r += x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r += x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    unsigned char x = static_cast<unsigned char>(3.14159);

    std::cout << "r " << std::setw(2) << "+=" << " x      "
              << std::setw(22) << "unsigned char"
              << std::setw(17) << (r += x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r += x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    signed char x = static_cast<signed char>(3.14159);

    std::cout << "r " << std::setw(2) << "+=" << " x      "
              << std::setw(22) << "signed char"
              << std::setw(17) << (r += x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r += x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    char x = static_cast<char>(3.14159);

    std::cout << "r " << std::setw(2) << "+=" << " x      "
              << std::setw(22) << "char"
              << std::setw(17) << (r += x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r += x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    double x = static_cast<double>(3.14159);

    std::cout << "r " << std::setw(2) << "+=" << " x      "
              << std::setw(22) << "double"
              << std::setw(17) << (r += x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r += x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    float x = static_cast<float>(3.14159);

    std::cout << "r " << std::setw(2) << "+=" << " x      "
              << std::setw(22) << "float"
              << std::setw(17) << (r += x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r += x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    long double x = static_cast<long double>(3.14159);

    std::cout << "r " << std::setw(2) << "+=" << " x      "
              << std::setw(22) << "long double"
              << std::setw(17) << (r += x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r += x);
    std::cout << "    " << debug_out() << std::endl;
  }

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    mpz_t x;
    my_mp_init(x, 3.14159);

    std::cout << "r " << std::setw(2) << "+=" << " x      "
              << std::setw(22) << "mpz_t"
              << std::setw(17) << (r += x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r += x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    mpq_t x;
    my_mp_init(x, 3.14159);

    std::cout << "r " << std::setw(2) << "+=" << " x      "
              << std::setw(22) << "mpq_t"
              << std::setw(17) << (r += x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r += x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    mpf_t x;
    my_mp_init(x, 3.14159);

    std::cout << "r " << std::setw(2) << "+=" << " x      "
              << std::setw(22) << "mpf_t"
              << std::setw(17) << (r += x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r += x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    mpfr_t x;
    my_mp_init(x, 3.14159);

    std::cout << "r " << std::setw(2) << "+=" << " x      "
              << std::setw(22) << "mpfr_t"
              << std::setw(17) << (r += x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r += x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

  {
    mpfr::real<1024> r = -1.41421;
    mpz_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpz_ptr x = x_nc;

    std::cout << "r " << std::setw(2) << "+=" << " x      "
              << std::setw(22) << "mpz_ptr"
              << std::setw(17) << (r += x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r += x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpq_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpq_ptr x = x_nc;

    std::cout << "r " << std::setw(2) << "+=" << " x      "
              << std::setw(22) << "mpq_ptr"
              << std::setw(17) << (r += x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r += x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpf_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpf_ptr x = x_nc;

    std::cout << "r " << std::setw(2) << "+=" << " x      "
              << std::setw(22) << "mpf_ptr"
              << std::setw(17) << (r += x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r += x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpfr_ptr x = x_nc;

    std::cout << "r " << std::setw(2) << "+=" << " x      "
              << std::setw(22) << "mpfr_ptr"
              << std::setw(17) << (r += x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r += x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpz_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpz_srcptr x = x_nc;

    std::cout << "r " << std::setw(2) << "+=" << " x      "
              << std::setw(22) << "mpz_srcptr"
              << std::setw(17) << (r += x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r += x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpq_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpq_srcptr x = x_nc;

    std::cout << "r " << std::setw(2) << "+=" << " x      "
              << std::setw(22) << "mpq_srcptr"
              << std::setw(17) << (r += x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r += x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpf_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpf_srcptr x = x_nc;

    std::cout << "r " << std::setw(2) << "+=" << " x      "
              << std::setw(22) << "mpf_srcptr"
              << std::setw(17) << (r += x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r += x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpfr_srcptr x = x_nc;

    std::cout << "r " << std::setw(2) << "+=" << " x      "
              << std::setw(22) << "mpfr_srcptr"
              << std::setw(17) << (r += x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r += x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    std::string x = "3.14159";

    std::cout << "r " << std::setw(2) << "+=" << " x      "
              << std::setw(22) << "std::string"
              << std::setw(17) << (r += x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r += x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    char x_nc[] = "3.14159";
    char* x = x_nc;

    std::cout << "r " << std::setw(2) << "+=" << " x      "
              << std::setw(22) << "char*"
              << std::setw(17) << (r += x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r += x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    char x_nc[] = "3.14159";
    const char* x = x_nc;

    std::cout << "r " << std::setw(2) << "+=" << " x      "
              << std::setw(22) << "const char*"
              << std::setw(17) << (r += x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r += x);
    std::cout << "    " << debug_out() << std::endl;
  }

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    char x[] = "3.14159";

    std::cout << "r " << std::setw(2) << "+=" << " x      "
              << std::setw(22) << "char[]"
              << std::setw(17) << (r += x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r += x);
    std::cout << "    " << debug_out() << std::endl;
  }
#endif  // _MSC_VER

  {
    mpfr::real<1024> r = -1.41421;
    mpfr::real<512> x = 3.14159;

    std::cout << "r " << std::setw(2) << "+=" << " x      "
              << std::setw(22) << "mpfr::real<512>"
              << std::setw(17) << (r += x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r += x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr::real<1024> x = 3.14159;

    std::cout << "r " << std::setw(2) << "+=" << " x      "
              << std::setw(22) << "mpfr::real<1024>"
              << std::setw(17) << (r += x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r += x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr::real<2048> x = 3.14159;

    std::cout << "r " << std::setw(2) << "+=" << " x      "
              << std::setw(22) << "mpfr::real<2048>"
              << std::setw(17) << (r += x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r += x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    unsigned long int x = static_cast<unsigned long int>(3.14159);

    std::cout << "r " << std::setw(2) << "-=" << " x      "
              << std::setw(22) << "unsigned long int"
              << std::setw(17) << (r -= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r -= x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    long int x = static_cast<long int>(3.14159);

    std::cout << "r " << std::setw(2) << "-=" << " x      "
              << std::setw(22) << "long int"
              << std::setw(17) << (r -= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r -= x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    unsigned int x = static_cast<unsigned int>(3.14159);

    std::cout << "r " << std::setw(2) << "-=" << " x      "
              << std::setw(22) << "unsigned int"
              << std::setw(17) << (r -= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r -= x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    int x = static_cast<int>(3.14159);

    std::cout << "r " << std::setw(2) << "-=" << " x      "
              << std::setw(22) << "int"
              << std::setw(17) << (r -= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r -= x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    unsigned short int x = static_cast<unsigned short int>(3.14159);

    std::cout << "r " << std::setw(2) << "-=" << " x      "
              << std::setw(22) << "unsigned short int"
              << std::setw(17) << (r -= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r -= x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    short int x = static_cast<short int>(3.14159);

    std::cout << "r " << std::setw(2) << "-=" << " x      "
              << std::setw(22) << "short int"
              << std::setw(17) << (r -= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r -= x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    unsigned char x = static_cast<unsigned char>(3.14159);

    std::cout << "r " << std::setw(2) << "-=" << " x      "
              << std::setw(22) << "unsigned char"
              << std::setw(17) << (r -= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r -= x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    signed char x = static_cast<signed char>(3.14159);

    std::cout << "r " << std::setw(2) << "-=" << " x      "
              << std::setw(22) << "signed char"
              << std::setw(17) << (r -= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r -= x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    char x = static_cast<char>(3.14159);

    std::cout << "r " << std::setw(2) << "-=" << " x      "
              << std::setw(22) << "char"
              << std::setw(17) << (r -= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r -= x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    double x = static_cast<double>(3.14159);

    std::cout << "r " << std::setw(2) << "-=" << " x      "
              << std::setw(22) << "double"
              << std::setw(17) << (r -= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r -= x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    float x = static_cast<float>(3.14159);

    std::cout << "r " << std::setw(2) << "-=" << " x      "
              << std::setw(22) << "float"
              << std::setw(17) << (r -= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r -= x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    long double x = static_cast<long double>(3.14159);

    std::cout << "r " << std::setw(2) << "-=" << " x      "
              << std::setw(22) << "long double"
              << std::setw(17) << (r -= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r -= x);
    std::cout << "    " << debug_out() << std::endl;
  }

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    mpz_t x;
    my_mp_init(x, 3.14159);

    std::cout << "r " << std::setw(2) << "-=" << " x      "
              << std::setw(22) << "mpz_t"
              << std::setw(17) << (r -= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r -= x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    mpq_t x;
    my_mp_init(x, 3.14159);

    std::cout << "r " << std::setw(2) << "-=" << " x      "
              << std::setw(22) << "mpq_t"
              << std::setw(17) << (r -= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r -= x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    mpf_t x;
    my_mp_init(x, 3.14159);

    std::cout << "r " << std::setw(2) << "-=" << " x      "
              << std::setw(22) << "mpf_t"
              << std::setw(17) << (r -= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r -= x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    mpfr_t x;
    my_mp_init(x, 3.14159);

    std::cout << "r " << std::setw(2) << "-=" << " x      "
              << std::setw(22) << "mpfr_t"
              << std::setw(17) << (r -= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r -= x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

  {
    mpfr::real<1024> r = -1.41421;
    mpz_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpz_ptr x = x_nc;

    std::cout << "r " << std::setw(2) << "-=" << " x      "
              << std::setw(22) << "mpz_ptr"
              << std::setw(17) << (r -= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r -= x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpq_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpq_ptr x = x_nc;

    std::cout << "r " << std::setw(2) << "-=" << " x      "
              << std::setw(22) << "mpq_ptr"
              << std::setw(17) << (r -= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r -= x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpf_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpf_ptr x = x_nc;

    std::cout << "r " << std::setw(2) << "-=" << " x      "
              << std::setw(22) << "mpf_ptr"
              << std::setw(17) << (r -= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r -= x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpfr_ptr x = x_nc;

    std::cout << "r " << std::setw(2) << "-=" << " x      "
              << std::setw(22) << "mpfr_ptr"
              << std::setw(17) << (r -= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r -= x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpz_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpz_srcptr x = x_nc;

    std::cout << "r " << std::setw(2) << "-=" << " x      "
              << std::setw(22) << "mpz_srcptr"
              << std::setw(17) << (r -= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r -= x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpq_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpq_srcptr x = x_nc;

    std::cout << "r " << std::setw(2) << "-=" << " x      "
              << std::setw(22) << "mpq_srcptr"
              << std::setw(17) << (r -= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r -= x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpf_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpf_srcptr x = x_nc;

    std::cout << "r " << std::setw(2) << "-=" << " x      "
              << std::setw(22) << "mpf_srcptr"
              << std::setw(17) << (r -= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r -= x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpfr_srcptr x = x_nc;

    std::cout << "r " << std::setw(2) << "-=" << " x      "
              << std::setw(22) << "mpfr_srcptr"
              << std::setw(17) << (r -= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r -= x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    std::string x = "3.14159";

    std::cout << "r " << std::setw(2) << "-=" << " x      "
              << std::setw(22) << "std::string"
              << std::setw(17) << (r -= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r -= x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    char x_nc[] = "3.14159";
    char* x = x_nc;

    std::cout << "r " << std::setw(2) << "-=" << " x      "
              << std::setw(22) << "char*"
              << std::setw(17) << (r -= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r -= x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    char x_nc[] = "3.14159";
    const char* x = x_nc;

    std::cout << "r " << std::setw(2) << "-=" << " x      "
              << std::setw(22) << "const char*"
              << std::setw(17) << (r -= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r -= x);
    std::cout << "    " << debug_out() << std::endl;
  }

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    char x[] = "3.14159";

    std::cout << "r " << std::setw(2) << "-=" << " x      "
              << std::setw(22) << "char[]"
              << std::setw(17) << (r -= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r -= x);
    std::cout << "    " << debug_out() << std::endl;
  }
#endif  // _MSC_VER

  {
    mpfr::real<1024> r = -1.41421;
    mpfr::real<512> x = 3.14159;

    std::cout << "r " << std::setw(2) << "-=" << " x      "
              << std::setw(22) << "mpfr::real<512>"
              << std::setw(17) << (r -= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r -= x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr::real<1024> x = 3.14159;

    std::cout << "r " << std::setw(2) << "-=" << " x      "
              << std::setw(22) << "mpfr::real<1024>"
              << std::setw(17) << (r -= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r -= x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr::real<2048> x = 3.14159;

    std::cout << "r " << std::setw(2) << "-=" << " x      "
              << std::setw(22) << "mpfr::real<2048>"
              << std::setw(17) << (r -= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r -= x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    unsigned long int x = static_cast<unsigned long int>(3.14159);

    std::cout << "r " << std::setw(2) << "*=" << " x      "
              << std::setw(22) << "unsigned long int"
              << std::setw(17) << (r *= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r *= x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    long int x = static_cast<long int>(3.14159);

    std::cout << "r " << std::setw(2) << "*=" << " x      "
              << std::setw(22) << "long int"
              << std::setw(17) << (r *= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r *= x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    unsigned int x = static_cast<unsigned int>(3.14159);

    std::cout << "r " << std::setw(2) << "*=" << " x      "
              << std::setw(22) << "unsigned int"
              << std::setw(17) << (r *= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r *= x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    int x = static_cast<int>(3.14159);

    std::cout << "r " << std::setw(2) << "*=" << " x      "
              << std::setw(22) << "int"
              << std::setw(17) << (r *= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r *= x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    unsigned short int x = static_cast<unsigned short int>(3.14159);

    std::cout << "r " << std::setw(2) << "*=" << " x      "
              << std::setw(22) << "unsigned short int"
              << std::setw(17) << (r *= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r *= x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    short int x = static_cast<short int>(3.14159);

    std::cout << "r " << std::setw(2) << "*=" << " x      "
              << std::setw(22) << "short int"
              << std::setw(17) << (r *= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r *= x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    unsigned char x = static_cast<unsigned char>(3.14159);

    std::cout << "r " << std::setw(2) << "*=" << " x      "
              << std::setw(22) << "unsigned char"
              << std::setw(17) << (r *= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r *= x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    signed char x = static_cast<signed char>(3.14159);

    std::cout << "r " << std::setw(2) << "*=" << " x      "
              << std::setw(22) << "signed char"
              << std::setw(17) << (r *= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r *= x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    char x = static_cast<char>(3.14159);

    std::cout << "r " << std::setw(2) << "*=" << " x      "
              << std::setw(22) << "char"
              << std::setw(17) << (r *= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r *= x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    double x = static_cast<double>(3.14159);

    std::cout << "r " << std::setw(2) << "*=" << " x      "
              << std::setw(22) << "double"
              << std::setw(17) << (r *= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r *= x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    float x = static_cast<float>(3.14159);

    std::cout << "r " << std::setw(2) << "*=" << " x      "
              << std::setw(22) << "float"
              << std::setw(17) << (r *= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r *= x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    long double x = static_cast<long double>(3.14159);

    std::cout << "r " << std::setw(2) << "*=" << " x      "
              << std::setw(22) << "long double"
              << std::setw(17) << (r *= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r *= x);
    std::cout << "    " << debug_out() << std::endl;
  }

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    mpz_t x;
    my_mp_init(x, 3.14159);

    std::cout << "r " << std::setw(2) << "*=" << " x      "
              << std::setw(22) << "mpz_t"
              << std::setw(17) << (r *= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r *= x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    mpq_t x;
    my_mp_init(x, 3.14159);

    std::cout << "r " << std::setw(2) << "*=" << " x      "
              << std::setw(22) << "mpq_t"
              << std::setw(17) << (r *= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r *= x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    mpf_t x;
    my_mp_init(x, 3.14159);

    std::cout << "r " << std::setw(2) << "*=" << " x      "
              << std::setw(22) << "mpf_t"
              << std::setw(17) << (r *= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r *= x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    mpfr_t x;
    my_mp_init(x, 3.14159);

    std::cout << "r " << std::setw(2) << "*=" << " x      "
              << std::setw(22) << "mpfr_t"
              << std::setw(17) << (r *= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r *= x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

  {
    mpfr::real<1024> r = -1.41421;
    mpz_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpz_ptr x = x_nc;

    std::cout << "r " << std::setw(2) << "*=" << " x      "
              << std::setw(22) << "mpz_ptr"
              << std::setw(17) << (r *= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r *= x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpq_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpq_ptr x = x_nc;

    std::cout << "r " << std::setw(2) << "*=" << " x      "
              << std::setw(22) << "mpq_ptr"
              << std::setw(17) << (r *= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r *= x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpf_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpf_ptr x = x_nc;

    std::cout << "r " << std::setw(2) << "*=" << " x      "
              << std::setw(22) << "mpf_ptr"
              << std::setw(17) << (r *= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r *= x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpfr_ptr x = x_nc;

    std::cout << "r " << std::setw(2) << "*=" << " x      "
              << std::setw(22) << "mpfr_ptr"
              << std::setw(17) << (r *= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r *= x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpz_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpz_srcptr x = x_nc;

    std::cout << "r " << std::setw(2) << "*=" << " x      "
              << std::setw(22) << "mpz_srcptr"
              << std::setw(17) << (r *= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r *= x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpq_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpq_srcptr x = x_nc;

    std::cout << "r " << std::setw(2) << "*=" << " x      "
              << std::setw(22) << "mpq_srcptr"
              << std::setw(17) << (r *= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r *= x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpf_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpf_srcptr x = x_nc;

    std::cout << "r " << std::setw(2) << "*=" << " x      "
              << std::setw(22) << "mpf_srcptr"
              << std::setw(17) << (r *= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r *= x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpfr_srcptr x = x_nc;

    std::cout << "r " << std::setw(2) << "*=" << " x      "
              << std::setw(22) << "mpfr_srcptr"
              << std::setw(17) << (r *= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r *= x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    std::string x = "3.14159";

    std::cout << "r " << std::setw(2) << "*=" << " x      "
              << std::setw(22) << "std::string"
              << std::setw(17) << (r *= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r *= x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    char x_nc[] = "3.14159";
    char* x = x_nc;

    std::cout << "r " << std::setw(2) << "*=" << " x      "
              << std::setw(22) << "char*"
              << std::setw(17) << (r *= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r *= x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    char x_nc[] = "3.14159";
    const char* x = x_nc;

    std::cout << "r " << std::setw(2) << "*=" << " x      "
              << std::setw(22) << "const char*"
              << std::setw(17) << (r *= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r *= x);
    std::cout << "    " << debug_out() << std::endl;
  }

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    char x[] = "3.14159";

    std::cout << "r " << std::setw(2) << "*=" << " x      "
              << std::setw(22) << "char[]"
              << std::setw(17) << (r *= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r *= x);
    std::cout << "    " << debug_out() << std::endl;
  }
#endif  // _MSC_VER

  {
    mpfr::real<1024> r = -1.41421;
    mpfr::real<512> x = 3.14159;

    std::cout << "r " << std::setw(2) << "*=" << " x      "
              << std::setw(22) << "mpfr::real<512>"
              << std::setw(17) << (r *= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r *= x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr::real<1024> x = 3.14159;

    std::cout << "r " << std::setw(2) << "*=" << " x      "
              << std::setw(22) << "mpfr::real<1024>"
              << std::setw(17) << (r *= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r *= x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr::real<2048> x = 3.14159;

    std::cout << "r " << std::setw(2) << "*=" << " x      "
              << std::setw(22) << "mpfr::real<2048>"
              << std::setw(17) << (r *= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r *= x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    unsigned long int x = static_cast<unsigned long int>(3.14159);

    std::cout << "r " << std::setw(2) << "/=" << " x      "
              << std::setw(22) << "unsigned long int"
              << std::setw(17) << (r /= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r /= x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    long int x = static_cast<long int>(3.14159);

    std::cout << "r " << std::setw(2) << "/=" << " x      "
              << std::setw(22) << "long int"
              << std::setw(17) << (r /= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r /= x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    unsigned int x = static_cast<unsigned int>(3.14159);

    std::cout << "r " << std::setw(2) << "/=" << " x      "
              << std::setw(22) << "unsigned int"
              << std::setw(17) << (r /= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r /= x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    int x = static_cast<int>(3.14159);

    std::cout << "r " << std::setw(2) << "/=" << " x      "
              << std::setw(22) << "int"
              << std::setw(17) << (r /= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r /= x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    unsigned short int x = static_cast<unsigned short int>(3.14159);

    std::cout << "r " << std::setw(2) << "/=" << " x      "
              << std::setw(22) << "unsigned short int"
              << std::setw(17) << (r /= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r /= x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    short int x = static_cast<short int>(3.14159);

    std::cout << "r " << std::setw(2) << "/=" << " x      "
              << std::setw(22) << "short int"
              << std::setw(17) << (r /= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r /= x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    unsigned char x = static_cast<unsigned char>(3.14159);

    std::cout << "r " << std::setw(2) << "/=" << " x      "
              << std::setw(22) << "unsigned char"
              << std::setw(17) << (r /= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r /= x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    signed char x = static_cast<signed char>(3.14159);

    std::cout << "r " << std::setw(2) << "/=" << " x      "
              << std::setw(22) << "signed char"
              << std::setw(17) << (r /= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r /= x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    char x = static_cast<char>(3.14159);

    std::cout << "r " << std::setw(2) << "/=" << " x      "
              << std::setw(22) << "char"
              << std::setw(17) << (r /= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r /= x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    double x = static_cast<double>(3.14159);

    std::cout << "r " << std::setw(2) << "/=" << " x      "
              << std::setw(22) << "double"
              << std::setw(17) << (r /= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r /= x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    float x = static_cast<float>(3.14159);

    std::cout << "r " << std::setw(2) << "/=" << " x      "
              << std::setw(22) << "float"
              << std::setw(17) << (r /= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r /= x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    long double x = static_cast<long double>(3.14159);

    std::cout << "r " << std::setw(2) << "/=" << " x      "
              << std::setw(22) << "long double"
              << std::setw(17) << (r /= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r /= x);
    std::cout << "    " << debug_out() << std::endl;
  }

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    mpz_t x;
    my_mp_init(x, 3.14159);

    std::cout << "r " << std::setw(2) << "/=" << " x      "
              << std::setw(22) << "mpz_t"
              << std::setw(17) << (r /= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r /= x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    mpq_t x;
    my_mp_init(x, 3.14159);

    std::cout << "r " << std::setw(2) << "/=" << " x      "
              << std::setw(22) << "mpq_t"
              << std::setw(17) << (r /= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r /= x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    mpf_t x;
    my_mp_init(x, 3.14159);

    std::cout << "r " << std::setw(2) << "/=" << " x      "
              << std::setw(22) << "mpf_t"
              << std::setw(17) << (r /= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r /= x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    mpfr_t x;
    my_mp_init(x, 3.14159);

    std::cout << "r " << std::setw(2) << "/=" << " x      "
              << std::setw(22) << "mpfr_t"
              << std::setw(17) << (r /= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r /= x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

  {
    mpfr::real<1024> r = -1.41421;
    mpz_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpz_ptr x = x_nc;

    std::cout << "r " << std::setw(2) << "/=" << " x      "
              << std::setw(22) << "mpz_ptr"
              << std::setw(17) << (r /= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r /= x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpq_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpq_ptr x = x_nc;

    std::cout << "r " << std::setw(2) << "/=" << " x      "
              << std::setw(22) << "mpq_ptr"
              << std::setw(17) << (r /= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r /= x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpf_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpf_ptr x = x_nc;

    std::cout << "r " << std::setw(2) << "/=" << " x      "
              << std::setw(22) << "mpf_ptr"
              << std::setw(17) << (r /= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r /= x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpfr_ptr x = x_nc;

    std::cout << "r " << std::setw(2) << "/=" << " x      "
              << std::setw(22) << "mpfr_ptr"
              << std::setw(17) << (r /= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r /= x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpz_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpz_srcptr x = x_nc;

    std::cout << "r " << std::setw(2) << "/=" << " x      "
              << std::setw(22) << "mpz_srcptr"
              << std::setw(17) << (r /= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r /= x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpq_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpq_srcptr x = x_nc;

    std::cout << "r " << std::setw(2) << "/=" << " x      "
              << std::setw(22) << "mpq_srcptr"
              << std::setw(17) << (r /= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r /= x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpf_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpf_srcptr x = x_nc;

    std::cout << "r " << std::setw(2) << "/=" << " x      "
              << std::setw(22) << "mpf_srcptr"
              << std::setw(17) << (r /= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r /= x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpfr_srcptr x = x_nc;

    std::cout << "r " << std::setw(2) << "/=" << " x      "
              << std::setw(22) << "mpfr_srcptr"
              << std::setw(17) << (r /= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r /= x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    std::string x = "3.14159";

    std::cout << "r " << std::setw(2) << "/=" << " x      "
              << std::setw(22) << "std::string"
              << std::setw(17) << (r /= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r /= x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    char x_nc[] = "3.14159";
    char* x = x_nc;

    std::cout << "r " << std::setw(2) << "/=" << " x      "
              << std::setw(22) << "char*"
              << std::setw(17) << (r /= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r /= x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    char x_nc[] = "3.14159";
    const char* x = x_nc;

    std::cout << "r " << std::setw(2) << "/=" << " x      "
              << std::setw(22) << "const char*"
              << std::setw(17) << (r /= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r /= x);
    std::cout << "    " << debug_out() << std::endl;
  }

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    char x[] = "3.14159";

    std::cout << "r " << std::setw(2) << "/=" << " x      "
              << std::setw(22) << "char[]"
              << std::setw(17) << (r /= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r /= x);
    std::cout << "    " << debug_out() << std::endl;
  }
#endif  // _MSC_VER

  {
    mpfr::real<1024> r = -1.41421;
    mpfr::real<512> x = 3.14159;

    std::cout << "r " << std::setw(2) << "/=" << " x      "
              << std::setw(22) << "mpfr::real<512>"
              << std::setw(17) << (r /= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r /= x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr::real<1024> x = 3.14159;

    std::cout << "r " << std::setw(2) << "/=" << " x      "
              << std::setw(22) << "mpfr::real<1024>"
              << std::setw(17) << (r /= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r /= x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr::real<2048> x = 3.14159;

    std::cout << "r " << std::setw(2) << "/=" << " x      "
              << std::setw(22) << "mpfr::real<2048>"
              << std::setw(17) << (r /= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r /= x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    unsigned long int x = static_cast<unsigned long int>(3.14159);

    std::cout << "r " << std::setw(2) << "+" << " x      "
              << std::setw(22) << "unsigned long int"
              << std::setw(17) << (r + x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r + x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    long int x = static_cast<long int>(3.14159);

    std::cout << "r " << std::setw(2) << "+" << " x      "
              << std::setw(22) << "long int"
              << std::setw(17) << (r + x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r + x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    unsigned int x = static_cast<unsigned int>(3.14159);

    std::cout << "r " << std::setw(2) << "+" << " x      "
              << std::setw(22) << "unsigned int"
              << std::setw(17) << (r + x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r + x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    int x = static_cast<int>(3.14159);

    std::cout << "r " << std::setw(2) << "+" << " x      "
              << std::setw(22) << "int"
              << std::setw(17) << (r + x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r + x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    unsigned short int x = static_cast<unsigned short int>(3.14159);

    std::cout << "r " << std::setw(2) << "+" << " x      "
              << std::setw(22) << "unsigned short int"
              << std::setw(17) << (r + x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r + x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    short int x = static_cast<short int>(3.14159);

    std::cout << "r " << std::setw(2) << "+" << " x      "
              << std::setw(22) << "short int"
              << std::setw(17) << (r + x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r + x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    unsigned char x = static_cast<unsigned char>(3.14159);

    std::cout << "r " << std::setw(2) << "+" << " x      "
              << std::setw(22) << "unsigned char"
              << std::setw(17) << (r + x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r + x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    signed char x = static_cast<signed char>(3.14159);

    std::cout << "r " << std::setw(2) << "+" << " x      "
              << std::setw(22) << "signed char"
              << std::setw(17) << (r + x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r + x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    char x = static_cast<char>(3.14159);

    std::cout << "r " << std::setw(2) << "+" << " x      "
              << std::setw(22) << "char"
              << std::setw(17) << (r + x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r + x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    double x = static_cast<double>(3.14159);

    std::cout << "r " << std::setw(2) << "+" << " x      "
              << std::setw(22) << "double"
              << std::setw(17) << (r + x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r + x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    float x = static_cast<float>(3.14159);

    std::cout << "r " << std::setw(2) << "+" << " x      "
              << std::setw(22) << "float"
              << std::setw(17) << (r + x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r + x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    long double x = static_cast<long double>(3.14159);

    std::cout << "r " << std::setw(2) << "+" << " x      "
              << std::setw(22) << "long double"
              << std::setw(17) << (r + x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r + x);
    std::cout << "    " << debug_out() << std::endl;
  }

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    mpz_t x;
    my_mp_init(x, 3.14159);

    std::cout << "r " << std::setw(2) << "+" << " x      "
              << std::setw(22) << "mpz_t"
              << std::setw(17) << (r + x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r + x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    mpq_t x;
    my_mp_init(x, 3.14159);

    std::cout << "r " << std::setw(2) << "+" << " x      "
              << std::setw(22) << "mpq_t"
              << std::setw(17) << (r + x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r + x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    mpf_t x;
    my_mp_init(x, 3.14159);

    std::cout << "r " << std::setw(2) << "+" << " x      "
              << std::setw(22) << "mpf_t"
              << std::setw(17) << (r + x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r + x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    mpfr_t x;
    my_mp_init(x, 3.14159);

    std::cout << "r " << std::setw(2) << "+" << " x      "
              << std::setw(22) << "mpfr_t"
              << std::setw(17) << (r + x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r + x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

  {
    mpfr::real<1024> r = -1.41421;
    mpz_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpz_ptr x = x_nc;

    std::cout << "r " << std::setw(2) << "+" << " x      "
              << std::setw(22) << "mpz_ptr"
              << std::setw(17) << (r + x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r + x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpq_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpq_ptr x = x_nc;

    std::cout << "r " << std::setw(2) << "+" << " x      "
              << std::setw(22) << "mpq_ptr"
              << std::setw(17) << (r + x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r + x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpf_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpf_ptr x = x_nc;

    std::cout << "r " << std::setw(2) << "+" << " x      "
              << std::setw(22) << "mpf_ptr"
              << std::setw(17) << (r + x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r + x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpfr_ptr x = x_nc;

    std::cout << "r " << std::setw(2) << "+" << " x      "
              << std::setw(22) << "mpfr_ptr"
              << std::setw(17) << (r + x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r + x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpz_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpz_srcptr x = x_nc;

    std::cout << "r " << std::setw(2) << "+" << " x      "
              << std::setw(22) << "mpz_srcptr"
              << std::setw(17) << (r + x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r + x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpq_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpq_srcptr x = x_nc;

    std::cout << "r " << std::setw(2) << "+" << " x      "
              << std::setw(22) << "mpq_srcptr"
              << std::setw(17) << (r + x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r + x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpf_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpf_srcptr x = x_nc;

    std::cout << "r " << std::setw(2) << "+" << " x      "
              << std::setw(22) << "mpf_srcptr"
              << std::setw(17) << (r + x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r + x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpfr_srcptr x = x_nc;

    std::cout << "r " << std::setw(2) << "+" << " x      "
              << std::setw(22) << "mpfr_srcptr"
              << std::setw(17) << (r + x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r + x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    std::string x = "3.14159";

    std::cout << "r " << std::setw(2) << "+" << " x      "
              << std::setw(22) << "std::string"
              << std::setw(17) << (r + x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r + x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    char x_nc[] = "3.14159";
    char* x = x_nc;

    std::cout << "r " << std::setw(2) << "+" << " x      "
              << std::setw(22) << "char*"
              << std::setw(17) << (r + x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r + x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    char x_nc[] = "3.14159";
    const char* x = x_nc;

    std::cout << "r " << std::setw(2) << "+" << " x      "
              << std::setw(22) << "const char*"
              << std::setw(17) << (r + x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r + x);
    std::cout << "    " << debug_out() << std::endl;
  }

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    char x[] = "3.14159";

    std::cout << "r " << std::setw(2) << "+" << " x      "
              << std::setw(22) << "char[]"
              << std::setw(17) << (r + x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r + x);
    std::cout << "    " << debug_out() << std::endl;
  }
#endif  // _MSC_VER

  {
    mpfr::real<1024> r = -1.41421;
    mpfr::real<512> x = 3.14159;

    std::cout << "r " << std::setw(2) << "+" << " x      "
              << std::setw(22) << "mpfr::real<512>"
              << std::setw(17) << (r + x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r + x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr::real<1024> x = 3.14159;

    std::cout << "r " << std::setw(2) << "+" << " x      "
              << std::setw(22) << "mpfr::real<1024>"
              << std::setw(17) << (r + x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r + x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr::real<2048> x = 3.14159;

    std::cout << "r " << std::setw(2) << "+" << " x      "
              << std::setw(22) << "mpfr::real<2048>"
              << std::setw(17) << (r + x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r + x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    unsigned long int x = static_cast<unsigned long int>(3.14159);

    std::cout << "r " << std::setw(2) << "-" << " x      "
              << std::setw(22) << "unsigned long int"
              << std::setw(17) << (r - x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r - x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    long int x = static_cast<long int>(3.14159);

    std::cout << "r " << std::setw(2) << "-" << " x      "
              << std::setw(22) << "long int"
              << std::setw(17) << (r - x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r - x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    unsigned int x = static_cast<unsigned int>(3.14159);

    std::cout << "r " << std::setw(2) << "-" << " x      "
              << std::setw(22) << "unsigned int"
              << std::setw(17) << (r - x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r - x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    int x = static_cast<int>(3.14159);

    std::cout << "r " << std::setw(2) << "-" << " x      "
              << std::setw(22) << "int"
              << std::setw(17) << (r - x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r - x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    unsigned short int x = static_cast<unsigned short int>(3.14159);

    std::cout << "r " << std::setw(2) << "-" << " x      "
              << std::setw(22) << "unsigned short int"
              << std::setw(17) << (r - x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r - x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    short int x = static_cast<short int>(3.14159);

    std::cout << "r " << std::setw(2) << "-" << " x      "
              << std::setw(22) << "short int"
              << std::setw(17) << (r - x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r - x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    unsigned char x = static_cast<unsigned char>(3.14159);

    std::cout << "r " << std::setw(2) << "-" << " x      "
              << std::setw(22) << "unsigned char"
              << std::setw(17) << (r - x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r - x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    signed char x = static_cast<signed char>(3.14159);

    std::cout << "r " << std::setw(2) << "-" << " x      "
              << std::setw(22) << "signed char"
              << std::setw(17) << (r - x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r - x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    char x = static_cast<char>(3.14159);

    std::cout << "r " << std::setw(2) << "-" << " x      "
              << std::setw(22) << "char"
              << std::setw(17) << (r - x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r - x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    double x = static_cast<double>(3.14159);

    std::cout << "r " << std::setw(2) << "-" << " x      "
              << std::setw(22) << "double"
              << std::setw(17) << (r - x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r - x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    float x = static_cast<float>(3.14159);

    std::cout << "r " << std::setw(2) << "-" << " x      "
              << std::setw(22) << "float"
              << std::setw(17) << (r - x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r - x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    long double x = static_cast<long double>(3.14159);

    std::cout << "r " << std::setw(2) << "-" << " x      "
              << std::setw(22) << "long double"
              << std::setw(17) << (r - x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r - x);
    std::cout << "    " << debug_out() << std::endl;
  }

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    mpz_t x;
    my_mp_init(x, 3.14159);

    std::cout << "r " << std::setw(2) << "-" << " x      "
              << std::setw(22) << "mpz_t"
              << std::setw(17) << (r - x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r - x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    mpq_t x;
    my_mp_init(x, 3.14159);

    std::cout << "r " << std::setw(2) << "-" << " x      "
              << std::setw(22) << "mpq_t"
              << std::setw(17) << (r - x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r - x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    mpf_t x;
    my_mp_init(x, 3.14159);

    std::cout << "r " << std::setw(2) << "-" << " x      "
              << std::setw(22) << "mpf_t"
              << std::setw(17) << (r - x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r - x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    mpfr_t x;
    my_mp_init(x, 3.14159);

    std::cout << "r " << std::setw(2) << "-" << " x      "
              << std::setw(22) << "mpfr_t"
              << std::setw(17) << (r - x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r - x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

  {
    mpfr::real<1024> r = -1.41421;
    mpz_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpz_ptr x = x_nc;

    std::cout << "r " << std::setw(2) << "-" << " x      "
              << std::setw(22) << "mpz_ptr"
              << std::setw(17) << (r - x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r - x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpq_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpq_ptr x = x_nc;

    std::cout << "r " << std::setw(2) << "-" << " x      "
              << std::setw(22) << "mpq_ptr"
              << std::setw(17) << (r - x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r - x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpf_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpf_ptr x = x_nc;

    std::cout << "r " << std::setw(2) << "-" << " x      "
              << std::setw(22) << "mpf_ptr"
              << std::setw(17) << (r - x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r - x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpfr_ptr x = x_nc;

    std::cout << "r " << std::setw(2) << "-" << " x      "
              << std::setw(22) << "mpfr_ptr"
              << std::setw(17) << (r - x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r - x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpz_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpz_srcptr x = x_nc;

    std::cout << "r " << std::setw(2) << "-" << " x      "
              << std::setw(22) << "mpz_srcptr"
              << std::setw(17) << (r - x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r - x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpq_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpq_srcptr x = x_nc;

    std::cout << "r " << std::setw(2) << "-" << " x      "
              << std::setw(22) << "mpq_srcptr"
              << std::setw(17) << (r - x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r - x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpf_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpf_srcptr x = x_nc;

    std::cout << "r " << std::setw(2) << "-" << " x      "
              << std::setw(22) << "mpf_srcptr"
              << std::setw(17) << (r - x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r - x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpfr_srcptr x = x_nc;

    std::cout << "r " << std::setw(2) << "-" << " x      "
              << std::setw(22) << "mpfr_srcptr"
              << std::setw(17) << (r - x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r - x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    std::string x = "3.14159";

    std::cout << "r " << std::setw(2) << "-" << " x      "
              << std::setw(22) << "std::string"
              << std::setw(17) << (r - x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r - x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    char x_nc[] = "3.14159";
    char* x = x_nc;

    std::cout << "r " << std::setw(2) << "-" << " x      "
              << std::setw(22) << "char*"
              << std::setw(17) << (r - x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r - x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    char x_nc[] = "3.14159";
    const char* x = x_nc;

    std::cout << "r " << std::setw(2) << "-" << " x      "
              << std::setw(22) << "const char*"
              << std::setw(17) << (r - x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r - x);
    std::cout << "    " << debug_out() << std::endl;
  }

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    char x[] = "3.14159";

    std::cout << "r " << std::setw(2) << "-" << " x      "
              << std::setw(22) << "char[]"
              << std::setw(17) << (r - x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r - x);
    std::cout << "    " << debug_out() << std::endl;
  }
#endif  // _MSC_VER

  {
    mpfr::real<1024> r = -1.41421;
    mpfr::real<512> x = 3.14159;

    std::cout << "r " << std::setw(2) << "-" << " x      "
              << std::setw(22) << "mpfr::real<512>"
              << std::setw(17) << (r - x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r - x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr::real<1024> x = 3.14159;

    std::cout << "r " << std::setw(2) << "-" << " x      "
              << std::setw(22) << "mpfr::real<1024>"
              << std::setw(17) << (r - x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r - x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr::real<2048> x = 3.14159;

    std::cout << "r " << std::setw(2) << "-" << " x      "
              << std::setw(22) << "mpfr::real<2048>"
              << std::setw(17) << (r - x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r - x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    unsigned long int x = static_cast<unsigned long int>(3.14159);

    std::cout << "r " << std::setw(2) << "*" << " x      "
              << std::setw(22) << "unsigned long int"
              << std::setw(17) << (r * x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r * x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    long int x = static_cast<long int>(3.14159);

    std::cout << "r " << std::setw(2) << "*" << " x      "
              << std::setw(22) << "long int"
              << std::setw(17) << (r * x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r * x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    unsigned int x = static_cast<unsigned int>(3.14159);

    std::cout << "r " << std::setw(2) << "*" << " x      "
              << std::setw(22) << "unsigned int"
              << std::setw(17) << (r * x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r * x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    int x = static_cast<int>(3.14159);

    std::cout << "r " << std::setw(2) << "*" << " x      "
              << std::setw(22) << "int"
              << std::setw(17) << (r * x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r * x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    unsigned short int x = static_cast<unsigned short int>(3.14159);

    std::cout << "r " << std::setw(2) << "*" << " x      "
              << std::setw(22) << "unsigned short int"
              << std::setw(17) << (r * x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r * x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    short int x = static_cast<short int>(3.14159);

    std::cout << "r " << std::setw(2) << "*" << " x      "
              << std::setw(22) << "short int"
              << std::setw(17) << (r * x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r * x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    unsigned char x = static_cast<unsigned char>(3.14159);

    std::cout << "r " << std::setw(2) << "*" << " x      "
              << std::setw(22) << "unsigned char"
              << std::setw(17) << (r * x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r * x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    signed char x = static_cast<signed char>(3.14159);

    std::cout << "r " << std::setw(2) << "*" << " x      "
              << std::setw(22) << "signed char"
              << std::setw(17) << (r * x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r * x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    char x = static_cast<char>(3.14159);

    std::cout << "r " << std::setw(2) << "*" << " x      "
              << std::setw(22) << "char"
              << std::setw(17) << (r * x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r * x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    double x = static_cast<double>(3.14159);

    std::cout << "r " << std::setw(2) << "*" << " x      "
              << std::setw(22) << "double"
              << std::setw(17) << (r * x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r * x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    float x = static_cast<float>(3.14159);

    std::cout << "r " << std::setw(2) << "*" << " x      "
              << std::setw(22) << "float"
              << std::setw(17) << (r * x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r * x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    long double x = static_cast<long double>(3.14159);

    std::cout << "r " << std::setw(2) << "*" << " x      "
              << std::setw(22) << "long double"
              << std::setw(17) << (r * x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r * x);
    std::cout << "    " << debug_out() << std::endl;
  }

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    mpz_t x;
    my_mp_init(x, 3.14159);

    std::cout << "r " << std::setw(2) << "*" << " x      "
              << std::setw(22) << "mpz_t"
              << std::setw(17) << (r * x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r * x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    mpq_t x;
    my_mp_init(x, 3.14159);

    std::cout << "r " << std::setw(2) << "*" << " x      "
              << std::setw(22) << "mpq_t"
              << std::setw(17) << (r * x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r * x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    mpf_t x;
    my_mp_init(x, 3.14159);

    std::cout << "r " << std::setw(2) << "*" << " x      "
              << std::setw(22) << "mpf_t"
              << std::setw(17) << (r * x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r * x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    mpfr_t x;
    my_mp_init(x, 3.14159);

    std::cout << "r " << std::setw(2) << "*" << " x      "
              << std::setw(22) << "mpfr_t"
              << std::setw(17) << (r * x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r * x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

  {
    mpfr::real<1024> r = -1.41421;
    mpz_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpz_ptr x = x_nc;

    std::cout << "r " << std::setw(2) << "*" << " x      "
              << std::setw(22) << "mpz_ptr"
              << std::setw(17) << (r * x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r * x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpq_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpq_ptr x = x_nc;

    std::cout << "r " << std::setw(2) << "*" << " x      "
              << std::setw(22) << "mpq_ptr"
              << std::setw(17) << (r * x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r * x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpf_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpf_ptr x = x_nc;

    std::cout << "r " << std::setw(2) << "*" << " x      "
              << std::setw(22) << "mpf_ptr"
              << std::setw(17) << (r * x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r * x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpfr_ptr x = x_nc;

    std::cout << "r " << std::setw(2) << "*" << " x      "
              << std::setw(22) << "mpfr_ptr"
              << std::setw(17) << (r * x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r * x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpz_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpz_srcptr x = x_nc;

    std::cout << "r " << std::setw(2) << "*" << " x      "
              << std::setw(22) << "mpz_srcptr"
              << std::setw(17) << (r * x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r * x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpq_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpq_srcptr x = x_nc;

    std::cout << "r " << std::setw(2) << "*" << " x      "
              << std::setw(22) << "mpq_srcptr"
              << std::setw(17) << (r * x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r * x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpf_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpf_srcptr x = x_nc;

    std::cout << "r " << std::setw(2) << "*" << " x      "
              << std::setw(22) << "mpf_srcptr"
              << std::setw(17) << (r * x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r * x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpfr_srcptr x = x_nc;

    std::cout << "r " << std::setw(2) << "*" << " x      "
              << std::setw(22) << "mpfr_srcptr"
              << std::setw(17) << (r * x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r * x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    std::string x = "3.14159";

    std::cout << "r " << std::setw(2) << "*" << " x      "
              << std::setw(22) << "std::string"
              << std::setw(17) << (r * x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r * x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    char x_nc[] = "3.14159";
    char* x = x_nc;

    std::cout << "r " << std::setw(2) << "*" << " x      "
              << std::setw(22) << "char*"
              << std::setw(17) << (r * x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r * x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    char x_nc[] = "3.14159";
    const char* x = x_nc;

    std::cout << "r " << std::setw(2) << "*" << " x      "
              << std::setw(22) << "const char*"
              << std::setw(17) << (r * x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r * x);
    std::cout << "    " << debug_out() << std::endl;
  }

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    char x[] = "3.14159";

    std::cout << "r " << std::setw(2) << "*" << " x      "
              << std::setw(22) << "char[]"
              << std::setw(17) << (r * x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r * x);
    std::cout << "    " << debug_out() << std::endl;
  }
#endif  // _MSC_VER

  {
    mpfr::real<1024> r = -1.41421;
    mpfr::real<512> x = 3.14159;

    std::cout << "r " << std::setw(2) << "*" << " x      "
              << std::setw(22) << "mpfr::real<512>"
              << std::setw(17) << (r * x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r * x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr::real<1024> x = 3.14159;

    std::cout << "r " << std::setw(2) << "*" << " x      "
              << std::setw(22) << "mpfr::real<1024>"
              << std::setw(17) << (r * x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r * x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr::real<2048> x = 3.14159;

    std::cout << "r " << std::setw(2) << "*" << " x      "
              << std::setw(22) << "mpfr::real<2048>"
              << std::setw(17) << (r * x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r * x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    unsigned long int x = static_cast<unsigned long int>(3.14159);

    std::cout << "r " << std::setw(2) << "/" << " x      "
              << std::setw(22) << "unsigned long int"
              << std::setw(17) << (r / x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r / x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    long int x = static_cast<long int>(3.14159);

    std::cout << "r " << std::setw(2) << "/" << " x      "
              << std::setw(22) << "long int"
              << std::setw(17) << (r / x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r / x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    unsigned int x = static_cast<unsigned int>(3.14159);

    std::cout << "r " << std::setw(2) << "/" << " x      "
              << std::setw(22) << "unsigned int"
              << std::setw(17) << (r / x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r / x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    int x = static_cast<int>(3.14159);

    std::cout << "r " << std::setw(2) << "/" << " x      "
              << std::setw(22) << "int"
              << std::setw(17) << (r / x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r / x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    unsigned short int x = static_cast<unsigned short int>(3.14159);

    std::cout << "r " << std::setw(2) << "/" << " x      "
              << std::setw(22) << "unsigned short int"
              << std::setw(17) << (r / x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r / x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    short int x = static_cast<short int>(3.14159);

    std::cout << "r " << std::setw(2) << "/" << " x      "
              << std::setw(22) << "short int"
              << std::setw(17) << (r / x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r / x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    unsigned char x = static_cast<unsigned char>(3.14159);

    std::cout << "r " << std::setw(2) << "/" << " x      "
              << std::setw(22) << "unsigned char"
              << std::setw(17) << (r / x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r / x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    signed char x = static_cast<signed char>(3.14159);

    std::cout << "r " << std::setw(2) << "/" << " x      "
              << std::setw(22) << "signed char"
              << std::setw(17) << (r / x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r / x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    char x = static_cast<char>(3.14159);

    std::cout << "r " << std::setw(2) << "/" << " x      "
              << std::setw(22) << "char"
              << std::setw(17) << (r / x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r / x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    double x = static_cast<double>(3.14159);

    std::cout << "r " << std::setw(2) << "/" << " x      "
              << std::setw(22) << "double"
              << std::setw(17) << (r / x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r / x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    float x = static_cast<float>(3.14159);

    std::cout << "r " << std::setw(2) << "/" << " x      "
              << std::setw(22) << "float"
              << std::setw(17) << (r / x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r / x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    long double x = static_cast<long double>(3.14159);

    std::cout << "r " << std::setw(2) << "/" << " x      "
              << std::setw(22) << "long double"
              << std::setw(17) << (r / x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r / x);
    std::cout << "    " << debug_out() << std::endl;
  }

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    mpz_t x;
    my_mp_init(x, 3.14159);

    std::cout << "r " << std::setw(2) << "/" << " x      "
              << std::setw(22) << "mpz_t"
              << std::setw(17) << (r / x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r / x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    mpq_t x;
    my_mp_init(x, 3.14159);

    std::cout << "r " << std::setw(2) << "/" << " x      "
              << std::setw(22) << "mpq_t"
              << std::setw(17) << (r / x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r / x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    mpf_t x;
    my_mp_init(x, 3.14159);

    std::cout << "r " << std::setw(2) << "/" << " x      "
              << std::setw(22) << "mpf_t"
              << std::setw(17) << (r / x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r / x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    mpfr_t x;
    my_mp_init(x, 3.14159);

    std::cout << "r " << std::setw(2) << "/" << " x      "
              << std::setw(22) << "mpfr_t"
              << std::setw(17) << (r / x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r / x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

  {
    mpfr::real<1024> r = -1.41421;
    mpz_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpz_ptr x = x_nc;

    std::cout << "r " << std::setw(2) << "/" << " x      "
              << std::setw(22) << "mpz_ptr"
              << std::setw(17) << (r / x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r / x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpq_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpq_ptr x = x_nc;

    std::cout << "r " << std::setw(2) << "/" << " x      "
              << std::setw(22) << "mpq_ptr"
              << std::setw(17) << (r / x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r / x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpf_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpf_ptr x = x_nc;

    std::cout << "r " << std::setw(2) << "/" << " x      "
              << std::setw(22) << "mpf_ptr"
              << std::setw(17) << (r / x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r / x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpfr_ptr x = x_nc;

    std::cout << "r " << std::setw(2) << "/" << " x      "
              << std::setw(22) << "mpfr_ptr"
              << std::setw(17) << (r / x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r / x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpz_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpz_srcptr x = x_nc;

    std::cout << "r " << std::setw(2) << "/" << " x      "
              << std::setw(22) << "mpz_srcptr"
              << std::setw(17) << (r / x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r / x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpq_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpq_srcptr x = x_nc;

    std::cout << "r " << std::setw(2) << "/" << " x      "
              << std::setw(22) << "mpq_srcptr"
              << std::setw(17) << (r / x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r / x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpf_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpf_srcptr x = x_nc;

    std::cout << "r " << std::setw(2) << "/" << " x      "
              << std::setw(22) << "mpf_srcptr"
              << std::setw(17) << (r / x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r / x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpfr_srcptr x = x_nc;

    std::cout << "r " << std::setw(2) << "/" << " x      "
              << std::setw(22) << "mpfr_srcptr"
              << std::setw(17) << (r / x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r / x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    std::string x = "3.14159";

    std::cout << "r " << std::setw(2) << "/" << " x      "
              << std::setw(22) << "std::string"
              << std::setw(17) << (r / x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r / x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    char x_nc[] = "3.14159";
    char* x = x_nc;

    std::cout << "r " << std::setw(2) << "/" << " x      "
              << std::setw(22) << "char*"
              << std::setw(17) << (r / x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r / x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    char x_nc[] = "3.14159";
    const char* x = x_nc;

    std::cout << "r " << std::setw(2) << "/" << " x      "
              << std::setw(22) << "const char*"
              << std::setw(17) << (r / x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r / x);
    std::cout << "    " << debug_out() << std::endl;
  }

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    char x[] = "3.14159";

    std::cout << "r " << std::setw(2) << "/" << " x      "
              << std::setw(22) << "char[]"
              << std::setw(17) << (r / x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r / x);
    std::cout << "    " << debug_out() << std::endl;
  }
#endif  // _MSC_VER

  {
    mpfr::real<1024> r = -1.41421;
    mpfr::real<512> x = 3.14159;

    std::cout << "r " << std::setw(2) << "/" << " x      "
              << std::setw(22) << "mpfr::real<512>"
              << std::setw(17) << (r / x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r / x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr::real<1024> x = 3.14159;

    std::cout << "r " << std::setw(2) << "/" << " x      "
              << std::setw(22) << "mpfr::real<1024>"
              << std::setw(17) << (r / x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r / x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr::real<2048> x = 3.14159;

    std::cout << "r " << std::setw(2) << "/" << " x      "
              << std::setw(22) << "mpfr::real<2048>"
              << std::setw(17) << (r / x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r / x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    unsigned long int x = static_cast<unsigned long int>(3.14159);

    std::cout << "r " << std::setw(2) << "==" << " x      "
              << std::setw(22) << "unsigned long int"
              << std::setw(17) << (r == x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r == x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    long int x = static_cast<long int>(3.14159);

    std::cout << "r " << std::setw(2) << "==" << " x      "
              << std::setw(22) << "long int"
              << std::setw(17) << (r == x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r == x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    unsigned int x = static_cast<unsigned int>(3.14159);

    std::cout << "r " << std::setw(2) << "==" << " x      "
              << std::setw(22) << "unsigned int"
              << std::setw(17) << (r == x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r == x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    int x = static_cast<int>(3.14159);

    std::cout << "r " << std::setw(2) << "==" << " x      "
              << std::setw(22) << "int"
              << std::setw(17) << (r == x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r == x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    unsigned short int x = static_cast<unsigned short int>(3.14159);

    std::cout << "r " << std::setw(2) << "==" << " x      "
              << std::setw(22) << "unsigned short int"
              << std::setw(17) << (r == x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r == x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    short int x = static_cast<short int>(3.14159);

    std::cout << "r " << std::setw(2) << "==" << " x      "
              << std::setw(22) << "short int"
              << std::setw(17) << (r == x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r == x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    unsigned char x = static_cast<unsigned char>(3.14159);

    std::cout << "r " << std::setw(2) << "==" << " x      "
              << std::setw(22) << "unsigned char"
              << std::setw(17) << (r == x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r == x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    signed char x = static_cast<signed char>(3.14159);

    std::cout << "r " << std::setw(2) << "==" << " x      "
              << std::setw(22) << "signed char"
              << std::setw(17) << (r == x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r == x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    char x = static_cast<char>(3.14159);

    std::cout << "r " << std::setw(2) << "==" << " x      "
              << std::setw(22) << "char"
              << std::setw(17) << (r == x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r == x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    double x = static_cast<double>(3.14159);

    std::cout << "r " << std::setw(2) << "==" << " x      "
              << std::setw(22) << "double"
              << std::setw(17) << (r == x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r == x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    float x = static_cast<float>(3.14159);

    std::cout << "r " << std::setw(2) << "==" << " x      "
              << std::setw(22) << "float"
              << std::setw(17) << (r == x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r == x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    long double x = static_cast<long double>(3.14159);

    std::cout << "r " << std::setw(2) << "==" << " x      "
              << std::setw(22) << "long double"
              << std::setw(17) << (r == x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r == x);
    std::cout << "    " << debug_out() << std::endl;
  }

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    mpz_t x;
    my_mp_init(x, 3.14159);

    std::cout << "r " << std::setw(2) << "==" << " x      "
              << std::setw(22) << "mpz_t"
              << std::setw(17) << (r == x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r == x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    mpq_t x;
    my_mp_init(x, 3.14159);

    std::cout << "r " << std::setw(2) << "==" << " x      "
              << std::setw(22) << "mpq_t"
              << std::setw(17) << (r == x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r == x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    mpf_t x;
    my_mp_init(x, 3.14159);

    std::cout << "r " << std::setw(2) << "==" << " x      "
              << std::setw(22) << "mpf_t"
              << std::setw(17) << (r == x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r == x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    mpfr_t x;
    my_mp_init(x, 3.14159);

    std::cout << "r " << std::setw(2) << "==" << " x      "
              << std::setw(22) << "mpfr_t"
              << std::setw(17) << (r == x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r == x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

  {
    mpfr::real<1024> r = -1.41421;
    mpz_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpz_ptr x = x_nc;

    std::cout << "r " << std::setw(2) << "==" << " x      "
              << std::setw(22) << "mpz_ptr"
              << std::setw(17) << (r == x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r == x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpq_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpq_ptr x = x_nc;

    std::cout << "r " << std::setw(2) << "==" << " x      "
              << std::setw(22) << "mpq_ptr"
              << std::setw(17) << (r == x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r == x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpf_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpf_ptr x = x_nc;

    std::cout << "r " << std::setw(2) << "==" << " x      "
              << std::setw(22) << "mpf_ptr"
              << std::setw(17) << (r == x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r == x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpfr_ptr x = x_nc;

    std::cout << "r " << std::setw(2) << "==" << " x      "
              << std::setw(22) << "mpfr_ptr"
              << std::setw(17) << (r == x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r == x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpz_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpz_srcptr x = x_nc;

    std::cout << "r " << std::setw(2) << "==" << " x      "
              << std::setw(22) << "mpz_srcptr"
              << std::setw(17) << (r == x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r == x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpq_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpq_srcptr x = x_nc;

    std::cout << "r " << std::setw(2) << "==" << " x      "
              << std::setw(22) << "mpq_srcptr"
              << std::setw(17) << (r == x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r == x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpf_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpf_srcptr x = x_nc;

    std::cout << "r " << std::setw(2) << "==" << " x      "
              << std::setw(22) << "mpf_srcptr"
              << std::setw(17) << (r == x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r == x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpfr_srcptr x = x_nc;

    std::cout << "r " << std::setw(2) << "==" << " x      "
              << std::setw(22) << "mpfr_srcptr"
              << std::setw(17) << (r == x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r == x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    std::string x = "3.14159";

    std::cout << "r " << std::setw(2) << "==" << " x      "
              << std::setw(22) << "std::string"
              << std::setw(17) << (r == x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r == x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    char x_nc[] = "3.14159";
    char* x = x_nc;

    std::cout << "r " << std::setw(2) << "==" << " x      "
              << std::setw(22) << "char*"
              << std::setw(17) << (r == x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r == x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    char x_nc[] = "3.14159";
    const char* x = x_nc;

    std::cout << "r " << std::setw(2) << "==" << " x      "
              << std::setw(22) << "const char*"
              << std::setw(17) << (r == x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r == x);
    std::cout << "    " << debug_out() << std::endl;
  }

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    char x[] = "3.14159";

    std::cout << "r " << std::setw(2) << "==" << " x      "
              << std::setw(22) << "char[]"
              << std::setw(17) << (r == x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r == x);
    std::cout << "    " << debug_out() << std::endl;
  }
#endif  // _MSC_VER

  {
    mpfr::real<1024> r = -1.41421;
    mpfr::real<512> x = 3.14159;

    std::cout << "r " << std::setw(2) << "==" << " x      "
              << std::setw(22) << "mpfr::real<512>"
              << std::setw(17) << (r == x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r == x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr::real<1024> x = 3.14159;

    std::cout << "r " << std::setw(2) << "==" << " x      "
              << std::setw(22) << "mpfr::real<1024>"
              << std::setw(17) << (r == x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r == x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr::real<2048> x = 3.14159;

    std::cout << "r " << std::setw(2) << "==" << " x      "
              << std::setw(22) << "mpfr::real<2048>"
              << std::setw(17) << (r == x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r == x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    unsigned long int x = static_cast<unsigned long int>(3.14159);

    std::cout << "r " << std::setw(2) << "!=" << " x      "
              << std::setw(22) << "unsigned long int"
              << std::setw(17) << (r != x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r != x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    long int x = static_cast<long int>(3.14159);

    std::cout << "r " << std::setw(2) << "!=" << " x      "
              << std::setw(22) << "long int"
              << std::setw(17) << (r != x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r != x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    unsigned int x = static_cast<unsigned int>(3.14159);

    std::cout << "r " << std::setw(2) << "!=" << " x      "
              << std::setw(22) << "unsigned int"
              << std::setw(17) << (r != x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r != x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    int x = static_cast<int>(3.14159);

    std::cout << "r " << std::setw(2) << "!=" << " x      "
              << std::setw(22) << "int"
              << std::setw(17) << (r != x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r != x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    unsigned short int x = static_cast<unsigned short int>(3.14159);

    std::cout << "r " << std::setw(2) << "!=" << " x      "
              << std::setw(22) << "unsigned short int"
              << std::setw(17) << (r != x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r != x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    short int x = static_cast<short int>(3.14159);

    std::cout << "r " << std::setw(2) << "!=" << " x      "
              << std::setw(22) << "short int"
              << std::setw(17) << (r != x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r != x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    unsigned char x = static_cast<unsigned char>(3.14159);

    std::cout << "r " << std::setw(2) << "!=" << " x      "
              << std::setw(22) << "unsigned char"
              << std::setw(17) << (r != x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r != x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    signed char x = static_cast<signed char>(3.14159);

    std::cout << "r " << std::setw(2) << "!=" << " x      "
              << std::setw(22) << "signed char"
              << std::setw(17) << (r != x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r != x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    char x = static_cast<char>(3.14159);

    std::cout << "r " << std::setw(2) << "!=" << " x      "
              << std::setw(22) << "char"
              << std::setw(17) << (r != x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r != x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    double x = static_cast<double>(3.14159);

    std::cout << "r " << std::setw(2) << "!=" << " x      "
              << std::setw(22) << "double"
              << std::setw(17) << (r != x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r != x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    float x = static_cast<float>(3.14159);

    std::cout << "r " << std::setw(2) << "!=" << " x      "
              << std::setw(22) << "float"
              << std::setw(17) << (r != x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r != x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    long double x = static_cast<long double>(3.14159);

    std::cout << "r " << std::setw(2) << "!=" << " x      "
              << std::setw(22) << "long double"
              << std::setw(17) << (r != x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r != x);
    std::cout << "    " << debug_out() << std::endl;
  }

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    mpz_t x;
    my_mp_init(x, 3.14159);

    std::cout << "r " << std::setw(2) << "!=" << " x      "
              << std::setw(22) << "mpz_t"
              << std::setw(17) << (r != x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r != x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    mpq_t x;
    my_mp_init(x, 3.14159);

    std::cout << "r " << std::setw(2) << "!=" << " x      "
              << std::setw(22) << "mpq_t"
              << std::setw(17) << (r != x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r != x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    mpf_t x;
    my_mp_init(x, 3.14159);

    std::cout << "r " << std::setw(2) << "!=" << " x      "
              << std::setw(22) << "mpf_t"
              << std::setw(17) << (r != x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r != x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    mpfr_t x;
    my_mp_init(x, 3.14159);

    std::cout << "r " << std::setw(2) << "!=" << " x      "
              << std::setw(22) << "mpfr_t"
              << std::setw(17) << (r != x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r != x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

  {
    mpfr::real<1024> r = -1.41421;
    mpz_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpz_ptr x = x_nc;

    std::cout << "r " << std::setw(2) << "!=" << " x      "
              << std::setw(22) << "mpz_ptr"
              << std::setw(17) << (r != x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r != x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpq_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpq_ptr x = x_nc;

    std::cout << "r " << std::setw(2) << "!=" << " x      "
              << std::setw(22) << "mpq_ptr"
              << std::setw(17) << (r != x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r != x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpf_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpf_ptr x = x_nc;

    std::cout << "r " << std::setw(2) << "!=" << " x      "
              << std::setw(22) << "mpf_ptr"
              << std::setw(17) << (r != x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r != x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpfr_ptr x = x_nc;

    std::cout << "r " << std::setw(2) << "!=" << " x      "
              << std::setw(22) << "mpfr_ptr"
              << std::setw(17) << (r != x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r != x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpz_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpz_srcptr x = x_nc;

    std::cout << "r " << std::setw(2) << "!=" << " x      "
              << std::setw(22) << "mpz_srcptr"
              << std::setw(17) << (r != x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r != x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpq_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpq_srcptr x = x_nc;

    std::cout << "r " << std::setw(2) << "!=" << " x      "
              << std::setw(22) << "mpq_srcptr"
              << std::setw(17) << (r != x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r != x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpf_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpf_srcptr x = x_nc;

    std::cout << "r " << std::setw(2) << "!=" << " x      "
              << std::setw(22) << "mpf_srcptr"
              << std::setw(17) << (r != x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r != x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpfr_srcptr x = x_nc;

    std::cout << "r " << std::setw(2) << "!=" << " x      "
              << std::setw(22) << "mpfr_srcptr"
              << std::setw(17) << (r != x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r != x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    std::string x = "3.14159";

    std::cout << "r " << std::setw(2) << "!=" << " x      "
              << std::setw(22) << "std::string"
              << std::setw(17) << (r != x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r != x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    char x_nc[] = "3.14159";
    char* x = x_nc;

    std::cout << "r " << std::setw(2) << "!=" << " x      "
              << std::setw(22) << "char*"
              << std::setw(17) << (r != x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r != x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    char x_nc[] = "3.14159";
    const char* x = x_nc;

    std::cout << "r " << std::setw(2) << "!=" << " x      "
              << std::setw(22) << "const char*"
              << std::setw(17) << (r != x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r != x);
    std::cout << "    " << debug_out() << std::endl;
  }

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    char x[] = "3.14159";

    std::cout << "r " << std::setw(2) << "!=" << " x      "
              << std::setw(22) << "char[]"
              << std::setw(17) << (r != x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r != x);
    std::cout << "    " << debug_out() << std::endl;
  }
#endif  // _MSC_VER

  {
    mpfr::real<1024> r = -1.41421;
    mpfr::real<512> x = 3.14159;

    std::cout << "r " << std::setw(2) << "!=" << " x      "
              << std::setw(22) << "mpfr::real<512>"
              << std::setw(17) << (r != x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r != x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr::real<1024> x = 3.14159;

    std::cout << "r " << std::setw(2) << "!=" << " x      "
              << std::setw(22) << "mpfr::real<1024>"
              << std::setw(17) << (r != x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r != x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr::real<2048> x = 3.14159;

    std::cout << "r " << std::setw(2) << "!=" << " x      "
              << std::setw(22) << "mpfr::real<2048>"
              << std::setw(17) << (r != x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r != x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    unsigned long int x = static_cast<unsigned long int>(3.14159);

    std::cout << "r " << std::setw(2) << "<" << " x      "
              << std::setw(22) << "unsigned long int"
              << std::setw(17) << (r < x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r < x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    long int x = static_cast<long int>(3.14159);

    std::cout << "r " << std::setw(2) << "<" << " x      "
              << std::setw(22) << "long int"
              << std::setw(17) << (r < x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r < x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    unsigned int x = static_cast<unsigned int>(3.14159);

    std::cout << "r " << std::setw(2) << "<" << " x      "
              << std::setw(22) << "unsigned int"
              << std::setw(17) << (r < x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r < x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    int x = static_cast<int>(3.14159);

    std::cout << "r " << std::setw(2) << "<" << " x      "
              << std::setw(22) << "int"
              << std::setw(17) << (r < x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r < x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    unsigned short int x = static_cast<unsigned short int>(3.14159);

    std::cout << "r " << std::setw(2) << "<" << " x      "
              << std::setw(22) << "unsigned short int"
              << std::setw(17) << (r < x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r < x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    short int x = static_cast<short int>(3.14159);

    std::cout << "r " << std::setw(2) << "<" << " x      "
              << std::setw(22) << "short int"
              << std::setw(17) << (r < x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r < x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    unsigned char x = static_cast<unsigned char>(3.14159);

    std::cout << "r " << std::setw(2) << "<" << " x      "
              << std::setw(22) << "unsigned char"
              << std::setw(17) << (r < x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r < x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    signed char x = static_cast<signed char>(3.14159);

    std::cout << "r " << std::setw(2) << "<" << " x      "
              << std::setw(22) << "signed char"
              << std::setw(17) << (r < x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r < x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    char x = static_cast<char>(3.14159);

    std::cout << "r " << std::setw(2) << "<" << " x      "
              << std::setw(22) << "char"
              << std::setw(17) << (r < x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r < x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    double x = static_cast<double>(3.14159);

    std::cout << "r " << std::setw(2) << "<" << " x      "
              << std::setw(22) << "double"
              << std::setw(17) << (r < x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r < x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    float x = static_cast<float>(3.14159);

    std::cout << "r " << std::setw(2) << "<" << " x      "
              << std::setw(22) << "float"
              << std::setw(17) << (r < x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r < x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    long double x = static_cast<long double>(3.14159);

    std::cout << "r " << std::setw(2) << "<" << " x      "
              << std::setw(22) << "long double"
              << std::setw(17) << (r < x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r < x);
    std::cout << "    " << debug_out() << std::endl;
  }

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    mpz_t x;
    my_mp_init(x, 3.14159);

    std::cout << "r " << std::setw(2) << "<" << " x      "
              << std::setw(22) << "mpz_t"
              << std::setw(17) << (r < x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r < x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    mpq_t x;
    my_mp_init(x, 3.14159);

    std::cout << "r " << std::setw(2) << "<" << " x      "
              << std::setw(22) << "mpq_t"
              << std::setw(17) << (r < x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r < x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    mpf_t x;
    my_mp_init(x, 3.14159);

    std::cout << "r " << std::setw(2) << "<" << " x      "
              << std::setw(22) << "mpf_t"
              << std::setw(17) << (r < x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r < x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    mpfr_t x;
    my_mp_init(x, 3.14159);

    std::cout << "r " << std::setw(2) << "<" << " x      "
              << std::setw(22) << "mpfr_t"
              << std::setw(17) << (r < x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r < x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

  {
    mpfr::real<1024> r = -1.41421;
    mpz_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpz_ptr x = x_nc;

    std::cout << "r " << std::setw(2) << "<" << " x      "
              << std::setw(22) << "mpz_ptr"
              << std::setw(17) << (r < x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r < x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpq_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpq_ptr x = x_nc;

    std::cout << "r " << std::setw(2) << "<" << " x      "
              << std::setw(22) << "mpq_ptr"
              << std::setw(17) << (r < x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r < x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpf_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpf_ptr x = x_nc;

    std::cout << "r " << std::setw(2) << "<" << " x      "
              << std::setw(22) << "mpf_ptr"
              << std::setw(17) << (r < x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r < x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpfr_ptr x = x_nc;

    std::cout << "r " << std::setw(2) << "<" << " x      "
              << std::setw(22) << "mpfr_ptr"
              << std::setw(17) << (r < x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r < x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpz_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpz_srcptr x = x_nc;

    std::cout << "r " << std::setw(2) << "<" << " x      "
              << std::setw(22) << "mpz_srcptr"
              << std::setw(17) << (r < x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r < x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpq_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpq_srcptr x = x_nc;

    std::cout << "r " << std::setw(2) << "<" << " x      "
              << std::setw(22) << "mpq_srcptr"
              << std::setw(17) << (r < x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r < x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpf_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpf_srcptr x = x_nc;

    std::cout << "r " << std::setw(2) << "<" << " x      "
              << std::setw(22) << "mpf_srcptr"
              << std::setw(17) << (r < x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r < x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpfr_srcptr x = x_nc;

    std::cout << "r " << std::setw(2) << "<" << " x      "
              << std::setw(22) << "mpfr_srcptr"
              << std::setw(17) << (r < x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r < x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    std::string x = "3.14159";

    std::cout << "r " << std::setw(2) << "<" << " x      "
              << std::setw(22) << "std::string"
              << std::setw(17) << (r < x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r < x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    char x_nc[] = "3.14159";
    char* x = x_nc;

    std::cout << "r " << std::setw(2) << "<" << " x      "
              << std::setw(22) << "char*"
              << std::setw(17) << (r < x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r < x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    char x_nc[] = "3.14159";
    const char* x = x_nc;

    std::cout << "r " << std::setw(2) << "<" << " x      "
              << std::setw(22) << "const char*"
              << std::setw(17) << (r < x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r < x);
    std::cout << "    " << debug_out() << std::endl;
  }

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    char x[] = "3.14159";

    std::cout << "r " << std::setw(2) << "<" << " x      "
              << std::setw(22) << "char[]"
              << std::setw(17) << (r < x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r < x);
    std::cout << "    " << debug_out() << std::endl;
  }
#endif  // _MSC_VER

  {
    mpfr::real<1024> r = -1.41421;
    mpfr::real<512> x = 3.14159;

    std::cout << "r " << std::setw(2) << "<" << " x      "
              << std::setw(22) << "mpfr::real<512>"
              << std::setw(17) << (r < x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r < x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr::real<1024> x = 3.14159;

    std::cout << "r " << std::setw(2) << "<" << " x      "
              << std::setw(22) << "mpfr::real<1024>"
              << std::setw(17) << (r < x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r < x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr::real<2048> x = 3.14159;

    std::cout << "r " << std::setw(2) << "<" << " x      "
              << std::setw(22) << "mpfr::real<2048>"
              << std::setw(17) << (r < x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r < x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    unsigned long int x = static_cast<unsigned long int>(3.14159);

    std::cout << "r " << std::setw(2) << "<=" << " x      "
              << std::setw(22) << "unsigned long int"
              << std::setw(17) << (r <= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r <= x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    long int x = static_cast<long int>(3.14159);

    std::cout << "r " << std::setw(2) << "<=" << " x      "
              << std::setw(22) << "long int"
              << std::setw(17) << (r <= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r <= x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    unsigned int x = static_cast<unsigned int>(3.14159);

    std::cout << "r " << std::setw(2) << "<=" << " x      "
              << std::setw(22) << "unsigned int"
              << std::setw(17) << (r <= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r <= x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    int x = static_cast<int>(3.14159);

    std::cout << "r " << std::setw(2) << "<=" << " x      "
              << std::setw(22) << "int"
              << std::setw(17) << (r <= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r <= x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    unsigned short int x = static_cast<unsigned short int>(3.14159);

    std::cout << "r " << std::setw(2) << "<=" << " x      "
              << std::setw(22) << "unsigned short int"
              << std::setw(17) << (r <= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r <= x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    short int x = static_cast<short int>(3.14159);

    std::cout << "r " << std::setw(2) << "<=" << " x      "
              << std::setw(22) << "short int"
              << std::setw(17) << (r <= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r <= x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    unsigned char x = static_cast<unsigned char>(3.14159);

    std::cout << "r " << std::setw(2) << "<=" << " x      "
              << std::setw(22) << "unsigned char"
              << std::setw(17) << (r <= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r <= x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    signed char x = static_cast<signed char>(3.14159);

    std::cout << "r " << std::setw(2) << "<=" << " x      "
              << std::setw(22) << "signed char"
              << std::setw(17) << (r <= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r <= x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    char x = static_cast<char>(3.14159);

    std::cout << "r " << std::setw(2) << "<=" << " x      "
              << std::setw(22) << "char"
              << std::setw(17) << (r <= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r <= x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    double x = static_cast<double>(3.14159);

    std::cout << "r " << std::setw(2) << "<=" << " x      "
              << std::setw(22) << "double"
              << std::setw(17) << (r <= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r <= x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    float x = static_cast<float>(3.14159);

    std::cout << "r " << std::setw(2) << "<=" << " x      "
              << std::setw(22) << "float"
              << std::setw(17) << (r <= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r <= x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    long double x = static_cast<long double>(3.14159);

    std::cout << "r " << std::setw(2) << "<=" << " x      "
              << std::setw(22) << "long double"
              << std::setw(17) << (r <= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r <= x);
    std::cout << "    " << debug_out() << std::endl;
  }

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    mpz_t x;
    my_mp_init(x, 3.14159);

    std::cout << "r " << std::setw(2) << "<=" << " x      "
              << std::setw(22) << "mpz_t"
              << std::setw(17) << (r <= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r <= x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    mpq_t x;
    my_mp_init(x, 3.14159);

    std::cout << "r " << std::setw(2) << "<=" << " x      "
              << std::setw(22) << "mpq_t"
              << std::setw(17) << (r <= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r <= x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    mpf_t x;
    my_mp_init(x, 3.14159);

    std::cout << "r " << std::setw(2) << "<=" << " x      "
              << std::setw(22) << "mpf_t"
              << std::setw(17) << (r <= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r <= x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    mpfr_t x;
    my_mp_init(x, 3.14159);

    std::cout << "r " << std::setw(2) << "<=" << " x      "
              << std::setw(22) << "mpfr_t"
              << std::setw(17) << (r <= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r <= x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

  {
    mpfr::real<1024> r = -1.41421;
    mpz_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpz_ptr x = x_nc;

    std::cout << "r " << std::setw(2) << "<=" << " x      "
              << std::setw(22) << "mpz_ptr"
              << std::setw(17) << (r <= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r <= x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpq_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpq_ptr x = x_nc;

    std::cout << "r " << std::setw(2) << "<=" << " x      "
              << std::setw(22) << "mpq_ptr"
              << std::setw(17) << (r <= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r <= x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpf_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpf_ptr x = x_nc;

    std::cout << "r " << std::setw(2) << "<=" << " x      "
              << std::setw(22) << "mpf_ptr"
              << std::setw(17) << (r <= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r <= x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpfr_ptr x = x_nc;

    std::cout << "r " << std::setw(2) << "<=" << " x      "
              << std::setw(22) << "mpfr_ptr"
              << std::setw(17) << (r <= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r <= x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpz_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpz_srcptr x = x_nc;

    std::cout << "r " << std::setw(2) << "<=" << " x      "
              << std::setw(22) << "mpz_srcptr"
              << std::setw(17) << (r <= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r <= x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpq_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpq_srcptr x = x_nc;

    std::cout << "r " << std::setw(2) << "<=" << " x      "
              << std::setw(22) << "mpq_srcptr"
              << std::setw(17) << (r <= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r <= x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpf_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpf_srcptr x = x_nc;

    std::cout << "r " << std::setw(2) << "<=" << " x      "
              << std::setw(22) << "mpf_srcptr"
              << std::setw(17) << (r <= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r <= x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpfr_srcptr x = x_nc;

    std::cout << "r " << std::setw(2) << "<=" << " x      "
              << std::setw(22) << "mpfr_srcptr"
              << std::setw(17) << (r <= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r <= x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    std::string x = "3.14159";

    std::cout << "r " << std::setw(2) << "<=" << " x      "
              << std::setw(22) << "std::string"
              << std::setw(17) << (r <= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r <= x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    char x_nc[] = "3.14159";
    char* x = x_nc;

    std::cout << "r " << std::setw(2) << "<=" << " x      "
              << std::setw(22) << "char*"
              << std::setw(17) << (r <= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r <= x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    char x_nc[] = "3.14159";
    const char* x = x_nc;

    std::cout << "r " << std::setw(2) << "<=" << " x      "
              << std::setw(22) << "const char*"
              << std::setw(17) << (r <= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r <= x);
    std::cout << "    " << debug_out() << std::endl;
  }

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    char x[] = "3.14159";

    std::cout << "r " << std::setw(2) << "<=" << " x      "
              << std::setw(22) << "char[]"
              << std::setw(17) << (r <= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r <= x);
    std::cout << "    " << debug_out() << std::endl;
  }
#endif  // _MSC_VER

  {
    mpfr::real<1024> r = -1.41421;
    mpfr::real<512> x = 3.14159;

    std::cout << "r " << std::setw(2) << "<=" << " x      "
              << std::setw(22) << "mpfr::real<512>"
              << std::setw(17) << (r <= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r <= x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr::real<1024> x = 3.14159;

    std::cout << "r " << std::setw(2) << "<=" << " x      "
              << std::setw(22) << "mpfr::real<1024>"
              << std::setw(17) << (r <= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r <= x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr::real<2048> x = 3.14159;

    std::cout << "r " << std::setw(2) << "<=" << " x      "
              << std::setw(22) << "mpfr::real<2048>"
              << std::setw(17) << (r <= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r <= x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    unsigned long int x = static_cast<unsigned long int>(3.14159);

    std::cout << "r " << std::setw(2) << ">" << " x      "
              << std::setw(22) << "unsigned long int"
              << std::setw(17) << (r > x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r > x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    long int x = static_cast<long int>(3.14159);

    std::cout << "r " << std::setw(2) << ">" << " x      "
              << std::setw(22) << "long int"
              << std::setw(17) << (r > x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r > x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    unsigned int x = static_cast<unsigned int>(3.14159);

    std::cout << "r " << std::setw(2) << ">" << " x      "
              << std::setw(22) << "unsigned int"
              << std::setw(17) << (r > x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r > x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    int x = static_cast<int>(3.14159);

    std::cout << "r " << std::setw(2) << ">" << " x      "
              << std::setw(22) << "int"
              << std::setw(17) << (r > x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r > x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    unsigned short int x = static_cast<unsigned short int>(3.14159);

    std::cout << "r " << std::setw(2) << ">" << " x      "
              << std::setw(22) << "unsigned short int"
              << std::setw(17) << (r > x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r > x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    short int x = static_cast<short int>(3.14159);

    std::cout << "r " << std::setw(2) << ">" << " x      "
              << std::setw(22) << "short int"
              << std::setw(17) << (r > x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r > x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    unsigned char x = static_cast<unsigned char>(3.14159);

    std::cout << "r " << std::setw(2) << ">" << " x      "
              << std::setw(22) << "unsigned char"
              << std::setw(17) << (r > x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r > x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    signed char x = static_cast<signed char>(3.14159);

    std::cout << "r " << std::setw(2) << ">" << " x      "
              << std::setw(22) << "signed char"
              << std::setw(17) << (r > x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r > x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    char x = static_cast<char>(3.14159);

    std::cout << "r " << std::setw(2) << ">" << " x      "
              << std::setw(22) << "char"
              << std::setw(17) << (r > x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r > x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    double x = static_cast<double>(3.14159);

    std::cout << "r " << std::setw(2) << ">" << " x      "
              << std::setw(22) << "double"
              << std::setw(17) << (r > x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r > x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    float x = static_cast<float>(3.14159);

    std::cout << "r " << std::setw(2) << ">" << " x      "
              << std::setw(22) << "float"
              << std::setw(17) << (r > x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r > x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    long double x = static_cast<long double>(3.14159);

    std::cout << "r " << std::setw(2) << ">" << " x      "
              << std::setw(22) << "long double"
              << std::setw(17) << (r > x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r > x);
    std::cout << "    " << debug_out() << std::endl;
  }

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    mpz_t x;
    my_mp_init(x, 3.14159);

    std::cout << "r " << std::setw(2) << ">" << " x      "
              << std::setw(22) << "mpz_t"
              << std::setw(17) << (r > x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r > x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    mpq_t x;
    my_mp_init(x, 3.14159);

    std::cout << "r " << std::setw(2) << ">" << " x      "
              << std::setw(22) << "mpq_t"
              << std::setw(17) << (r > x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r > x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    mpf_t x;
    my_mp_init(x, 3.14159);

    std::cout << "r " << std::setw(2) << ">" << " x      "
              << std::setw(22) << "mpf_t"
              << std::setw(17) << (r > x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r > x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    mpfr_t x;
    my_mp_init(x, 3.14159);

    std::cout << "r " << std::setw(2) << ">" << " x      "
              << std::setw(22) << "mpfr_t"
              << std::setw(17) << (r > x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r > x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

  {
    mpfr::real<1024> r = -1.41421;
    mpz_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpz_ptr x = x_nc;

    std::cout << "r " << std::setw(2) << ">" << " x      "
              << std::setw(22) << "mpz_ptr"
              << std::setw(17) << (r > x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r > x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpq_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpq_ptr x = x_nc;

    std::cout << "r " << std::setw(2) << ">" << " x      "
              << std::setw(22) << "mpq_ptr"
              << std::setw(17) << (r > x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r > x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpf_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpf_ptr x = x_nc;

    std::cout << "r " << std::setw(2) << ">" << " x      "
              << std::setw(22) << "mpf_ptr"
              << std::setw(17) << (r > x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r > x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpfr_ptr x = x_nc;

    std::cout << "r " << std::setw(2) << ">" << " x      "
              << std::setw(22) << "mpfr_ptr"
              << std::setw(17) << (r > x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r > x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpz_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpz_srcptr x = x_nc;

    std::cout << "r " << std::setw(2) << ">" << " x      "
              << std::setw(22) << "mpz_srcptr"
              << std::setw(17) << (r > x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r > x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpq_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpq_srcptr x = x_nc;

    std::cout << "r " << std::setw(2) << ">" << " x      "
              << std::setw(22) << "mpq_srcptr"
              << std::setw(17) << (r > x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r > x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpf_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpf_srcptr x = x_nc;

    std::cout << "r " << std::setw(2) << ">" << " x      "
              << std::setw(22) << "mpf_srcptr"
              << std::setw(17) << (r > x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r > x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpfr_srcptr x = x_nc;

    std::cout << "r " << std::setw(2) << ">" << " x      "
              << std::setw(22) << "mpfr_srcptr"
              << std::setw(17) << (r > x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r > x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    std::string x = "3.14159";

    std::cout << "r " << std::setw(2) << ">" << " x      "
              << std::setw(22) << "std::string"
              << std::setw(17) << (r > x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r > x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    char x_nc[] = "3.14159";
    char* x = x_nc;

    std::cout << "r " << std::setw(2) << ">" << " x      "
              << std::setw(22) << "char*"
              << std::setw(17) << (r > x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r > x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    char x_nc[] = "3.14159";
    const char* x = x_nc;

    std::cout << "r " << std::setw(2) << ">" << " x      "
              << std::setw(22) << "const char*"
              << std::setw(17) << (r > x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r > x);
    std::cout << "    " << debug_out() << std::endl;
  }

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    char x[] = "3.14159";

    std::cout << "r " << std::setw(2) << ">" << " x      "
              << std::setw(22) << "char[]"
              << std::setw(17) << (r > x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r > x);
    std::cout << "    " << debug_out() << std::endl;
  }
#endif  // _MSC_VER

  {
    mpfr::real<1024> r = -1.41421;
    mpfr::real<512> x = 3.14159;

    std::cout << "r " << std::setw(2) << ">" << " x      "
              << std::setw(22) << "mpfr::real<512>"
              << std::setw(17) << (r > x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r > x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr::real<1024> x = 3.14159;

    std::cout << "r " << std::setw(2) << ">" << " x      "
              << std::setw(22) << "mpfr::real<1024>"
              << std::setw(17) << (r > x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r > x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr::real<2048> x = 3.14159;

    std::cout << "r " << std::setw(2) << ">" << " x      "
              << std::setw(22) << "mpfr::real<2048>"
              << std::setw(17) << (r > x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r > x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    unsigned long int x = static_cast<unsigned long int>(3.14159);

    std::cout << "r " << std::setw(2) << ">=" << " x      "
              << std::setw(22) << "unsigned long int"
              << std::setw(17) << (r >= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r >= x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    long int x = static_cast<long int>(3.14159);

    std::cout << "r " << std::setw(2) << ">=" << " x      "
              << std::setw(22) << "long int"
              << std::setw(17) << (r >= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r >= x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    unsigned int x = static_cast<unsigned int>(3.14159);

    std::cout << "r " << std::setw(2) << ">=" << " x      "
              << std::setw(22) << "unsigned int"
              << std::setw(17) << (r >= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r >= x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    int x = static_cast<int>(3.14159);

    std::cout << "r " << std::setw(2) << ">=" << " x      "
              << std::setw(22) << "int"
              << std::setw(17) << (r >= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r >= x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    unsigned short int x = static_cast<unsigned short int>(3.14159);

    std::cout << "r " << std::setw(2) << ">=" << " x      "
              << std::setw(22) << "unsigned short int"
              << std::setw(17) << (r >= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r >= x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    short int x = static_cast<short int>(3.14159);

    std::cout << "r " << std::setw(2) << ">=" << " x      "
              << std::setw(22) << "short int"
              << std::setw(17) << (r >= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r >= x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    unsigned char x = static_cast<unsigned char>(3.14159);

    std::cout << "r " << std::setw(2) << ">=" << " x      "
              << std::setw(22) << "unsigned char"
              << std::setw(17) << (r >= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r >= x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    signed char x = static_cast<signed char>(3.14159);

    std::cout << "r " << std::setw(2) << ">=" << " x      "
              << std::setw(22) << "signed char"
              << std::setw(17) << (r >= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r >= x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    char x = static_cast<char>(3.14159);

    std::cout << "r " << std::setw(2) << ">=" << " x      "
              << std::setw(22) << "char"
              << std::setw(17) << (r >= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r >= x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    double x = static_cast<double>(3.14159);

    std::cout << "r " << std::setw(2) << ">=" << " x      "
              << std::setw(22) << "double"
              << std::setw(17) << (r >= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r >= x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    float x = static_cast<float>(3.14159);

    std::cout << "r " << std::setw(2) << ">=" << " x      "
              << std::setw(22) << "float"
              << std::setw(17) << (r >= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r >= x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    long double x = static_cast<long double>(3.14159);

    std::cout << "r " << std::setw(2) << ">=" << " x      "
              << std::setw(22) << "long double"
              << std::setw(17) << (r >= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r >= x);
    std::cout << "    " << debug_out() << std::endl;
  }

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    mpz_t x;
    my_mp_init(x, 3.14159);

    std::cout << "r " << std::setw(2) << ">=" << " x      "
              << std::setw(22) << "mpz_t"
              << std::setw(17) << (r >= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r >= x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    mpq_t x;
    my_mp_init(x, 3.14159);

    std::cout << "r " << std::setw(2) << ">=" << " x      "
              << std::setw(22) << "mpq_t"
              << std::setw(17) << (r >= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r >= x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    mpf_t x;
    my_mp_init(x, 3.14159);

    std::cout << "r " << std::setw(2) << ">=" << " x      "
              << std::setw(22) << "mpf_t"
              << std::setw(17) << (r >= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r >= x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    mpfr_t x;
    my_mp_init(x, 3.14159);

    std::cout << "r " << std::setw(2) << ">=" << " x      "
              << std::setw(22) << "mpfr_t"
              << std::setw(17) << (r >= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r >= x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

  {
    mpfr::real<1024> r = -1.41421;
    mpz_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpz_ptr x = x_nc;

    std::cout << "r " << std::setw(2) << ">=" << " x      "
              << std::setw(22) << "mpz_ptr"
              << std::setw(17) << (r >= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r >= x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpq_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpq_ptr x = x_nc;

    std::cout << "r " << std::setw(2) << ">=" << " x      "
              << std::setw(22) << "mpq_ptr"
              << std::setw(17) << (r >= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r >= x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpf_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpf_ptr x = x_nc;

    std::cout << "r " << std::setw(2) << ">=" << " x      "
              << std::setw(22) << "mpf_ptr"
              << std::setw(17) << (r >= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r >= x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpfr_ptr x = x_nc;

    std::cout << "r " << std::setw(2) << ">=" << " x      "
              << std::setw(22) << "mpfr_ptr"
              << std::setw(17) << (r >= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r >= x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpz_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpz_srcptr x = x_nc;

    std::cout << "r " << std::setw(2) << ">=" << " x      "
              << std::setw(22) << "mpz_srcptr"
              << std::setw(17) << (r >= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r >= x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpq_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpq_srcptr x = x_nc;

    std::cout << "r " << std::setw(2) << ">=" << " x      "
              << std::setw(22) << "mpq_srcptr"
              << std::setw(17) << (r >= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r >= x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpf_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpf_srcptr x = x_nc;

    std::cout << "r " << std::setw(2) << ">=" << " x      "
              << std::setw(22) << "mpf_srcptr"
              << std::setw(17) << (r >= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r >= x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpfr_srcptr x = x_nc;

    std::cout << "r " << std::setw(2) << ">=" << " x      "
              << std::setw(22) << "mpfr_srcptr"
              << std::setw(17) << (r >= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r >= x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    std::string x = "3.14159";

    std::cout << "r " << std::setw(2) << ">=" << " x      "
              << std::setw(22) << "std::string"
              << std::setw(17) << (r >= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r >= x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    char x_nc[] = "3.14159";
    char* x = x_nc;

    std::cout << "r " << std::setw(2) << ">=" << " x      "
              << std::setw(22) << "char*"
              << std::setw(17) << (r >= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r >= x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    char x_nc[] = "3.14159";
    const char* x = x_nc;

    std::cout << "r " << std::setw(2) << ">=" << " x      "
              << std::setw(22) << "const char*"
              << std::setw(17) << (r >= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r >= x);
    std::cout << "    " << debug_out() << std::endl;
  }

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    char x[] = "3.14159";

    std::cout << "r " << std::setw(2) << ">=" << " x      "
              << std::setw(22) << "char[]"
              << std::setw(17) << (r >= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r >= x);
    std::cout << "    " << debug_out() << std::endl;
  }
#endif  // _MSC_VER

  {
    mpfr::real<1024> r = -1.41421;
    mpfr::real<512> x = 3.14159;

    std::cout << "r " << std::setw(2) << ">=" << " x      "
              << std::setw(22) << "mpfr::real<512>"
              << std::setw(17) << (r >= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r >= x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr::real<1024> x = 3.14159;

    std::cout << "r " << std::setw(2) << ">=" << " x      "
              << std::setw(22) << "mpfr::real<1024>"
              << std::setw(17) << (r >= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r >= x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr::real<2048> x = 3.14159;

    std::cout << "r " << std::setw(2) << ">=" << " x      "
              << std::setw(22) << "mpfr::real<2048>"
              << std::setw(17) << (r >= x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r >= x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    unsigned long int x = static_cast<unsigned long int>(3.14159);

    std::cout << "r " << std::setw(2) << "=" << " x      "
              << std::setw(22) << "unsigned long int"
              << std::setw(17) << (r = x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r = x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    long int x = static_cast<long int>(3.14159);

    std::cout << "r " << std::setw(2) << "=" << " x      "
              << std::setw(22) << "long int"
              << std::setw(17) << (r = x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r = x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    unsigned int x = static_cast<unsigned int>(3.14159);

    std::cout << "r " << std::setw(2) << "=" << " x      "
              << std::setw(22) << "unsigned int"
              << std::setw(17) << (r = x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r = x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    int x = static_cast<int>(3.14159);

    std::cout << "r " << std::setw(2) << "=" << " x      "
              << std::setw(22) << "int"
              << std::setw(17) << (r = x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r = x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    unsigned short int x = static_cast<unsigned short int>(3.14159);

    std::cout << "r " << std::setw(2) << "=" << " x      "
              << std::setw(22) << "unsigned short int"
              << std::setw(17) << (r = x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r = x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    short int x = static_cast<short int>(3.14159);

    std::cout << "r " << std::setw(2) << "=" << " x      "
              << std::setw(22) << "short int"
              << std::setw(17) << (r = x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r = x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    unsigned char x = static_cast<unsigned char>(3.14159);

    std::cout << "r " << std::setw(2) << "=" << " x      "
              << std::setw(22) << "unsigned char"
              << std::setw(17) << (r = x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r = x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    signed char x = static_cast<signed char>(3.14159);

    std::cout << "r " << std::setw(2) << "=" << " x      "
              << std::setw(22) << "signed char"
              << std::setw(17) << (r = x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r = x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    char x = static_cast<char>(3.14159);

    std::cout << "r " << std::setw(2) << "=" << " x      "
              << std::setw(22) << "char"
              << std::setw(17) << (r = x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r = x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    double x = static_cast<double>(3.14159);

    std::cout << "r " << std::setw(2) << "=" << " x      "
              << std::setw(22) << "double"
              << std::setw(17) << (r = x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r = x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    float x = static_cast<float>(3.14159);

    std::cout << "r " << std::setw(2) << "=" << " x      "
              << std::setw(22) << "float"
              << std::setw(17) << (r = x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r = x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    long double x = static_cast<long double>(3.14159);

    std::cout << "r " << std::setw(2) << "=" << " x      "
              << std::setw(22) << "long double"
              << std::setw(17) << (r = x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r = x);
    std::cout << "    " << debug_out() << std::endl;
  }

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    mpz_t x;
    my_mp_init(x, 3.14159);

    std::cout << "r " << std::setw(2) << "=" << " x      "
              << std::setw(22) << "mpz_t"
              << std::setw(17) << (r = x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r = x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    mpq_t x;
    my_mp_init(x, 3.14159);

    std::cout << "r " << std::setw(2) << "=" << " x      "
              << std::setw(22) << "mpq_t"
              << std::setw(17) << (r = x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r = x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    mpf_t x;
    my_mp_init(x, 3.14159);

    std::cout << "r " << std::setw(2) << "=" << " x      "
              << std::setw(22) << "mpf_t"
              << std::setw(17) << (r = x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r = x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    mpfr_t x;
    my_mp_init(x, 3.14159);

    std::cout << "r " << std::setw(2) << "=" << " x      "
              << std::setw(22) << "mpfr_t"
              << std::setw(17) << (r = x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r = x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

  {
    mpfr::real<1024> r = -1.41421;
    mpz_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpz_ptr x = x_nc;

    std::cout << "r " << std::setw(2) << "=" << " x      "
              << std::setw(22) << "mpz_ptr"
              << std::setw(17) << (r = x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r = x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpq_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpq_ptr x = x_nc;

    std::cout << "r " << std::setw(2) << "=" << " x      "
              << std::setw(22) << "mpq_ptr"
              << std::setw(17) << (r = x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r = x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpf_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpf_ptr x = x_nc;

    std::cout << "r " << std::setw(2) << "=" << " x      "
              << std::setw(22) << "mpf_ptr"
              << std::setw(17) << (r = x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r = x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpfr_ptr x = x_nc;

    std::cout << "r " << std::setw(2) << "=" << " x      "
              << std::setw(22) << "mpfr_ptr"
              << std::setw(17) << (r = x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r = x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpz_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpz_srcptr x = x_nc;

    std::cout << "r " << std::setw(2) << "=" << " x      "
              << std::setw(22) << "mpz_srcptr"
              << std::setw(17) << (r = x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r = x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpq_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpq_srcptr x = x_nc;

    std::cout << "r " << std::setw(2) << "=" << " x      "
              << std::setw(22) << "mpq_srcptr"
              << std::setw(17) << (r = x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r = x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpf_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpf_srcptr x = x_nc;

    std::cout << "r " << std::setw(2) << "=" << " x      "
              << std::setw(22) << "mpf_srcptr"
              << std::setw(17) << (r = x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r = x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpfr_srcptr x = x_nc;

    std::cout << "r " << std::setw(2) << "=" << " x      "
              << std::setw(22) << "mpfr_srcptr"
              << std::setw(17) << (r = x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r = x);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    std::string x = "3.14159";

    std::cout << "r " << std::setw(2) << "=" << " x      "
              << std::setw(22) << "std::string"
              << std::setw(17) << (r = x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r = x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    char x_nc[] = "3.14159";
    char* x = x_nc;

    std::cout << "r " << std::setw(2) << "=" << " x      "
              << std::setw(22) << "char*"
              << std::setw(17) << (r = x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r = x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    char x_nc[] = "3.14159";
    const char* x = x_nc;

    std::cout << "r " << std::setw(2) << "=" << " x      "
              << std::setw(22) << "const char*"
              << std::setw(17) << (r = x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r = x);
    std::cout << "    " << debug_out() << std::endl;
  }

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    char x[] = "3.14159";

    std::cout << "r " << std::setw(2) << "=" << " x      "
              << std::setw(22) << "char[]"
              << std::setw(17) << (r = x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r = x);
    std::cout << "    " << debug_out() << std::endl;
  }
#endif  // _MSC_VER

  {
    mpfr::real<1024> r = -1.41421;
    mpfr::real<512> x = 3.14159;

    std::cout << "r " << std::setw(2) << "=" << " x      "
              << std::setw(22) << "mpfr::real<512>"
              << std::setw(17) << (r = x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r = x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr::real<1024> x = 3.14159;

    std::cout << "r " << std::setw(2) << "=" << " x      "
              << std::setw(22) << "mpfr::real<1024>"
              << std::setw(17) << (r = x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r = x);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr::real<2048> x = 3.14159;

    std::cout << "r " << std::setw(2) << "=" << " x      "
              << std::setw(22) << "mpfr::real<2048>"
              << std::setw(17) << (r = x);
    debug_clear();
    std::cout << std::setw(20) << type2char(r = x);
    std::cout << "    " << debug_out() << std::endl;
  }


  {
    mpfr::real<1024> r = -1.41421;
    unsigned long int x = static_cast<unsigned long int>(3.14159);

    std::cout << "x " << std::setw(2) << "+" << " r      "
              << std::setw(22) << "unsigned long int"
              << std::setw(17) << (x + r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x + r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    long int x = static_cast<long int>(3.14159);

    std::cout << "x " << std::setw(2) << "+" << " r      "
              << std::setw(22) << "long int"
              << std::setw(17) << (x + r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x + r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    unsigned int x = static_cast<unsigned int>(3.14159);

    std::cout << "x " << std::setw(2) << "+" << " r      "
              << std::setw(22) << "unsigned int"
              << std::setw(17) << (x + r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x + r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    int x = static_cast<int>(3.14159);

    std::cout << "x " << std::setw(2) << "+" << " r      "
              << std::setw(22) << "int"
              << std::setw(17) << (x + r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x + r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    unsigned short int x = static_cast<unsigned short int>(3.14159);

    std::cout << "x " << std::setw(2) << "+" << " r      "
              << std::setw(22) << "unsigned short int"
              << std::setw(17) << (x + r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x + r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    short int x = static_cast<short int>(3.14159);

    std::cout << "x " << std::setw(2) << "+" << " r      "
              << std::setw(22) << "short int"
              << std::setw(17) << (x + r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x + r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    unsigned char x = static_cast<unsigned char>(3.14159);

    std::cout << "x " << std::setw(2) << "+" << " r      "
              << std::setw(22) << "unsigned char"
              << std::setw(17) << (x + r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x + r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    signed char x = static_cast<signed char>(3.14159);

    std::cout << "x " << std::setw(2) << "+" << " r      "
              << std::setw(22) << "signed char"
              << std::setw(17) << (x + r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x + r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    char x = static_cast<char>(3.14159);

    std::cout << "x " << std::setw(2) << "+" << " r      "
              << std::setw(22) << "char"
              << std::setw(17) << (x + r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x + r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    double x = static_cast<double>(3.14159);

    std::cout << "x " << std::setw(2) << "+" << " r      "
              << std::setw(22) << "double"
              << std::setw(17) << (x + r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x + r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    float x = static_cast<float>(3.14159);

    std::cout << "x " << std::setw(2) << "+" << " r      "
              << std::setw(22) << "float"
              << std::setw(17) << (x + r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x + r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    long double x = static_cast<long double>(3.14159);

    std::cout << "x " << std::setw(2) << "+" << " r      "
              << std::setw(22) << "long double"
              << std::setw(17) << (x + r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x + r);
    std::cout << "    " << debug_out() << std::endl;
  }

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    mpz_t x;
    my_mp_init(x, 3.14159);

    std::cout << "x " << std::setw(2) << "+" << " r      "
              << std::setw(22) << "mpz_t"
              << std::setw(17) << (x + r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x + r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    mpq_t x;
    my_mp_init(x, 3.14159);

    std::cout << "x " << std::setw(2) << "+" << " r      "
              << std::setw(22) << "mpq_t"
              << std::setw(17) << (x + r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x + r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    mpf_t x;
    my_mp_init(x, 3.14159);

    std::cout << "x " << std::setw(2) << "+" << " r      "
              << std::setw(22) << "mpf_t"
              << std::setw(17) << (x + r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x + r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    mpfr_t x;
    my_mp_init(x, 3.14159);

    std::cout << "x " << std::setw(2) << "+" << " r      "
              << std::setw(22) << "mpfr_t"
              << std::setw(17) << (x + r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x + r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

  {
    mpfr::real<1024> r = -1.41421;
    mpz_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpz_ptr x = x_nc;

    std::cout << "x " << std::setw(2) << "+" << " r      "
              << std::setw(22) << "mpz_ptr"
              << std::setw(17) << (x + r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x + r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpq_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpq_ptr x = x_nc;

    std::cout << "x " << std::setw(2) << "+" << " r      "
              << std::setw(22) << "mpq_ptr"
              << std::setw(17) << (x + r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x + r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpf_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpf_ptr x = x_nc;

    std::cout << "x " << std::setw(2) << "+" << " r      "
              << std::setw(22) << "mpf_ptr"
              << std::setw(17) << (x + r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x + r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpfr_ptr x = x_nc;

    std::cout << "x " << std::setw(2) << "+" << " r      "
              << std::setw(22) << "mpfr_ptr"
              << std::setw(17) << (x + r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x + r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpz_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpz_srcptr x = x_nc;

    std::cout << "x " << std::setw(2) << "+" << " r      "
              << std::setw(22) << "mpz_srcptr"
              << std::setw(17) << (x + r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x + r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpq_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpq_srcptr x = x_nc;

    std::cout << "x " << std::setw(2) << "+" << " r      "
              << std::setw(22) << "mpq_srcptr"
              << std::setw(17) << (x + r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x + r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpf_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpf_srcptr x = x_nc;

    std::cout << "x " << std::setw(2) << "+" << " r      "
              << std::setw(22) << "mpf_srcptr"
              << std::setw(17) << (x + r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x + r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpfr_srcptr x = x_nc;

    std::cout << "x " << std::setw(2) << "+" << " r      "
              << std::setw(22) << "mpfr_srcptr"
              << std::setw(17) << (x + r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x + r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    std::string x = "3.14159";

    std::cout << "x " << std::setw(2) << "+" << " r      "
              << std::setw(22) << "std::string"
              << std::setw(17) << (x + r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x + r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    char x_nc[] = "3.14159";
    char* x = x_nc;

    std::cout << "x " << std::setw(2) << "+" << " r      "
              << std::setw(22) << "char*"
              << std::setw(17) << (x + r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x + r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    char x_nc[] = "3.14159";
    const char* x = x_nc;

    std::cout << "x " << std::setw(2) << "+" << " r      "
              << std::setw(22) << "const char*"
              << std::setw(17) << (x + r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x + r);
    std::cout << "    " << debug_out() << std::endl;
  }

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    char x[] = "3.14159";

    std::cout << "x " << std::setw(2) << "+" << " r      "
              << std::setw(22) << "char[]"
              << std::setw(17) << (x + r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x + r);
    std::cout << "    " << debug_out() << std::endl;
  }
#endif  // _MSC_VER

  {
    mpfr::real<1024> r = -1.41421;
    mpfr::real<512> x = 3.14159;

    std::cout << "x " << std::setw(2) << "+" << " r      "
              << std::setw(22) << "mpfr::real<512>"
              << std::setw(17) << (x + r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x + r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr::real<1024> x = 3.14159;

    std::cout << "x " << std::setw(2) << "+" << " r      "
              << std::setw(22) << "mpfr::real<1024>"
              << std::setw(17) << (x + r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x + r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr::real<2048> x = 3.14159;

    std::cout << "x " << std::setw(2) << "+" << " r      "
              << std::setw(22) << "mpfr::real<2048>"
              << std::setw(17) << (x + r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x + r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    unsigned long int x = static_cast<unsigned long int>(3.14159);

    std::cout << "x " << std::setw(2) << "-" << " r      "
              << std::setw(22) << "unsigned long int"
              << std::setw(17) << (x - r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x - r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    long int x = static_cast<long int>(3.14159);

    std::cout << "x " << std::setw(2) << "-" << " r      "
              << std::setw(22) << "long int"
              << std::setw(17) << (x - r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x - r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    unsigned int x = static_cast<unsigned int>(3.14159);

    std::cout << "x " << std::setw(2) << "-" << " r      "
              << std::setw(22) << "unsigned int"
              << std::setw(17) << (x - r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x - r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    int x = static_cast<int>(3.14159);

    std::cout << "x " << std::setw(2) << "-" << " r      "
              << std::setw(22) << "int"
              << std::setw(17) << (x - r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x - r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    unsigned short int x = static_cast<unsigned short int>(3.14159);

    std::cout << "x " << std::setw(2) << "-" << " r      "
              << std::setw(22) << "unsigned short int"
              << std::setw(17) << (x - r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x - r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    short int x = static_cast<short int>(3.14159);

    std::cout << "x " << std::setw(2) << "-" << " r      "
              << std::setw(22) << "short int"
              << std::setw(17) << (x - r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x - r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    unsigned char x = static_cast<unsigned char>(3.14159);

    std::cout << "x " << std::setw(2) << "-" << " r      "
              << std::setw(22) << "unsigned char"
              << std::setw(17) << (x - r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x - r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    signed char x = static_cast<signed char>(3.14159);

    std::cout << "x " << std::setw(2) << "-" << " r      "
              << std::setw(22) << "signed char"
              << std::setw(17) << (x - r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x - r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    char x = static_cast<char>(3.14159);

    std::cout << "x " << std::setw(2) << "-" << " r      "
              << std::setw(22) << "char"
              << std::setw(17) << (x - r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x - r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    double x = static_cast<double>(3.14159);

    std::cout << "x " << std::setw(2) << "-" << " r      "
              << std::setw(22) << "double"
              << std::setw(17) << (x - r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x - r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    float x = static_cast<float>(3.14159);

    std::cout << "x " << std::setw(2) << "-" << " r      "
              << std::setw(22) << "float"
              << std::setw(17) << (x - r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x - r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    long double x = static_cast<long double>(3.14159);

    std::cout << "x " << std::setw(2) << "-" << " r      "
              << std::setw(22) << "long double"
              << std::setw(17) << (x - r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x - r);
    std::cout << "    " << debug_out() << std::endl;
  }

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    mpz_t x;
    my_mp_init(x, 3.14159);

    std::cout << "x " << std::setw(2) << "-" << " r      "
              << std::setw(22) << "mpz_t"
              << std::setw(17) << (x - r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x - r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    mpq_t x;
    my_mp_init(x, 3.14159);

    std::cout << "x " << std::setw(2) << "-" << " r      "
              << std::setw(22) << "mpq_t"
              << std::setw(17) << (x - r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x - r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    mpf_t x;
    my_mp_init(x, 3.14159);

    std::cout << "x " << std::setw(2) << "-" << " r      "
              << std::setw(22) << "mpf_t"
              << std::setw(17) << (x - r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x - r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    mpfr_t x;
    my_mp_init(x, 3.14159);

    std::cout << "x " << std::setw(2) << "-" << " r      "
              << std::setw(22) << "mpfr_t"
              << std::setw(17) << (x - r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x - r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

  {
    mpfr::real<1024> r = -1.41421;
    mpz_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpz_ptr x = x_nc;

    std::cout << "x " << std::setw(2) << "-" << " r      "
              << std::setw(22) << "mpz_ptr"
              << std::setw(17) << (x - r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x - r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpq_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpq_ptr x = x_nc;

    std::cout << "x " << std::setw(2) << "-" << " r      "
              << std::setw(22) << "mpq_ptr"
              << std::setw(17) << (x - r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x - r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpf_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpf_ptr x = x_nc;

    std::cout << "x " << std::setw(2) << "-" << " r      "
              << std::setw(22) << "mpf_ptr"
              << std::setw(17) << (x - r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x - r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpfr_ptr x = x_nc;

    std::cout << "x " << std::setw(2) << "-" << " r      "
              << std::setw(22) << "mpfr_ptr"
              << std::setw(17) << (x - r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x - r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpz_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpz_srcptr x = x_nc;

    std::cout << "x " << std::setw(2) << "-" << " r      "
              << std::setw(22) << "mpz_srcptr"
              << std::setw(17) << (x - r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x - r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpq_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpq_srcptr x = x_nc;

    std::cout << "x " << std::setw(2) << "-" << " r      "
              << std::setw(22) << "mpq_srcptr"
              << std::setw(17) << (x - r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x - r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpf_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpf_srcptr x = x_nc;

    std::cout << "x " << std::setw(2) << "-" << " r      "
              << std::setw(22) << "mpf_srcptr"
              << std::setw(17) << (x - r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x - r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpfr_srcptr x = x_nc;

    std::cout << "x " << std::setw(2) << "-" << " r      "
              << std::setw(22) << "mpfr_srcptr"
              << std::setw(17) << (x - r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x - r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    std::string x = "3.14159";

    std::cout << "x " << std::setw(2) << "-" << " r      "
              << std::setw(22) << "std::string"
              << std::setw(17) << (x - r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x - r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    char x_nc[] = "3.14159";
    char* x = x_nc;

    std::cout << "x " << std::setw(2) << "-" << " r      "
              << std::setw(22) << "char*"
              << std::setw(17) << (x - r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x - r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    char x_nc[] = "3.14159";
    const char* x = x_nc;

    std::cout << "x " << std::setw(2) << "-" << " r      "
              << std::setw(22) << "const char*"
              << std::setw(17) << (x - r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x - r);
    std::cout << "    " << debug_out() << std::endl;
  }

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    char x[] = "3.14159";

    std::cout << "x " << std::setw(2) << "-" << " r      "
              << std::setw(22) << "char[]"
              << std::setw(17) << (x - r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x - r);
    std::cout << "    " << debug_out() << std::endl;
  }
#endif  // _MSC_VER

  {
    mpfr::real<1024> r = -1.41421;
    mpfr::real<512> x = 3.14159;

    std::cout << "x " << std::setw(2) << "-" << " r      "
              << std::setw(22) << "mpfr::real<512>"
              << std::setw(17) << (x - r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x - r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr::real<1024> x = 3.14159;

    std::cout << "x " << std::setw(2) << "-" << " r      "
              << std::setw(22) << "mpfr::real<1024>"
              << std::setw(17) << (x - r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x - r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr::real<2048> x = 3.14159;

    std::cout << "x " << std::setw(2) << "-" << " r      "
              << std::setw(22) << "mpfr::real<2048>"
              << std::setw(17) << (x - r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x - r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    unsigned long int x = static_cast<unsigned long int>(3.14159);

    std::cout << "x " << std::setw(2) << "*" << " r      "
              << std::setw(22) << "unsigned long int"
              << std::setw(17) << (x * r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x * r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    long int x = static_cast<long int>(3.14159);

    std::cout << "x " << std::setw(2) << "*" << " r      "
              << std::setw(22) << "long int"
              << std::setw(17) << (x * r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x * r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    unsigned int x = static_cast<unsigned int>(3.14159);

    std::cout << "x " << std::setw(2) << "*" << " r      "
              << std::setw(22) << "unsigned int"
              << std::setw(17) << (x * r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x * r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    int x = static_cast<int>(3.14159);

    std::cout << "x " << std::setw(2) << "*" << " r      "
              << std::setw(22) << "int"
              << std::setw(17) << (x * r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x * r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    unsigned short int x = static_cast<unsigned short int>(3.14159);

    std::cout << "x " << std::setw(2) << "*" << " r      "
              << std::setw(22) << "unsigned short int"
              << std::setw(17) << (x * r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x * r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    short int x = static_cast<short int>(3.14159);

    std::cout << "x " << std::setw(2) << "*" << " r      "
              << std::setw(22) << "short int"
              << std::setw(17) << (x * r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x * r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    unsigned char x = static_cast<unsigned char>(3.14159);

    std::cout << "x " << std::setw(2) << "*" << " r      "
              << std::setw(22) << "unsigned char"
              << std::setw(17) << (x * r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x * r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    signed char x = static_cast<signed char>(3.14159);

    std::cout << "x " << std::setw(2) << "*" << " r      "
              << std::setw(22) << "signed char"
              << std::setw(17) << (x * r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x * r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    char x = static_cast<char>(3.14159);

    std::cout << "x " << std::setw(2) << "*" << " r      "
              << std::setw(22) << "char"
              << std::setw(17) << (x * r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x * r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    double x = static_cast<double>(3.14159);

    std::cout << "x " << std::setw(2) << "*" << " r      "
              << std::setw(22) << "double"
              << std::setw(17) << (x * r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x * r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    float x = static_cast<float>(3.14159);

    std::cout << "x " << std::setw(2) << "*" << " r      "
              << std::setw(22) << "float"
              << std::setw(17) << (x * r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x * r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    long double x = static_cast<long double>(3.14159);

    std::cout << "x " << std::setw(2) << "*" << " r      "
              << std::setw(22) << "long double"
              << std::setw(17) << (x * r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x * r);
    std::cout << "    " << debug_out() << std::endl;
  }

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    mpz_t x;
    my_mp_init(x, 3.14159);

    std::cout << "x " << std::setw(2) << "*" << " r      "
              << std::setw(22) << "mpz_t"
              << std::setw(17) << (x * r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x * r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    mpq_t x;
    my_mp_init(x, 3.14159);

    std::cout << "x " << std::setw(2) << "*" << " r      "
              << std::setw(22) << "mpq_t"
              << std::setw(17) << (x * r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x * r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    mpf_t x;
    my_mp_init(x, 3.14159);

    std::cout << "x " << std::setw(2) << "*" << " r      "
              << std::setw(22) << "mpf_t"
              << std::setw(17) << (x * r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x * r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    mpfr_t x;
    my_mp_init(x, 3.14159);

    std::cout << "x " << std::setw(2) << "*" << " r      "
              << std::setw(22) << "mpfr_t"
              << std::setw(17) << (x * r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x * r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

  {
    mpfr::real<1024> r = -1.41421;
    mpz_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpz_ptr x = x_nc;

    std::cout << "x " << std::setw(2) << "*" << " r      "
              << std::setw(22) << "mpz_ptr"
              << std::setw(17) << (x * r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x * r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpq_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpq_ptr x = x_nc;

    std::cout << "x " << std::setw(2) << "*" << " r      "
              << std::setw(22) << "mpq_ptr"
              << std::setw(17) << (x * r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x * r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpf_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpf_ptr x = x_nc;

    std::cout << "x " << std::setw(2) << "*" << " r      "
              << std::setw(22) << "mpf_ptr"
              << std::setw(17) << (x * r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x * r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpfr_ptr x = x_nc;

    std::cout << "x " << std::setw(2) << "*" << " r      "
              << std::setw(22) << "mpfr_ptr"
              << std::setw(17) << (x * r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x * r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpz_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpz_srcptr x = x_nc;

    std::cout << "x " << std::setw(2) << "*" << " r      "
              << std::setw(22) << "mpz_srcptr"
              << std::setw(17) << (x * r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x * r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpq_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpq_srcptr x = x_nc;

    std::cout << "x " << std::setw(2) << "*" << " r      "
              << std::setw(22) << "mpq_srcptr"
              << std::setw(17) << (x * r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x * r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpf_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpf_srcptr x = x_nc;

    std::cout << "x " << std::setw(2) << "*" << " r      "
              << std::setw(22) << "mpf_srcptr"
              << std::setw(17) << (x * r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x * r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpfr_srcptr x = x_nc;

    std::cout << "x " << std::setw(2) << "*" << " r      "
              << std::setw(22) << "mpfr_srcptr"
              << std::setw(17) << (x * r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x * r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    std::string x = "3.14159";

    std::cout << "x " << std::setw(2) << "*" << " r      "
              << std::setw(22) << "std::string"
              << std::setw(17) << (x * r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x * r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    char x_nc[] = "3.14159";
    char* x = x_nc;

    std::cout << "x " << std::setw(2) << "*" << " r      "
              << std::setw(22) << "char*"
              << std::setw(17) << (x * r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x * r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    char x_nc[] = "3.14159";
    const char* x = x_nc;

    std::cout << "x " << std::setw(2) << "*" << " r      "
              << std::setw(22) << "const char*"
              << std::setw(17) << (x * r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x * r);
    std::cout << "    " << debug_out() << std::endl;
  }

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    char x[] = "3.14159";

    std::cout << "x " << std::setw(2) << "*" << " r      "
              << std::setw(22) << "char[]"
              << std::setw(17) << (x * r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x * r);
    std::cout << "    " << debug_out() << std::endl;
  }
#endif  // _MSC_VER

  {
    mpfr::real<1024> r = -1.41421;
    mpfr::real<512> x = 3.14159;

    std::cout << "x " << std::setw(2) << "*" << " r      "
              << std::setw(22) << "mpfr::real<512>"
              << std::setw(17) << (x * r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x * r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr::real<1024> x = 3.14159;

    std::cout << "x " << std::setw(2) << "*" << " r      "
              << std::setw(22) << "mpfr::real<1024>"
              << std::setw(17) << (x * r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x * r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr::real<2048> x = 3.14159;

    std::cout << "x " << std::setw(2) << "*" << " r      "
              << std::setw(22) << "mpfr::real<2048>"
              << std::setw(17) << (x * r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x * r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    unsigned long int x = static_cast<unsigned long int>(3.14159);

    std::cout << "x " << std::setw(2) << "/" << " r      "
              << std::setw(22) << "unsigned long int"
              << std::setw(17) << (x / r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x / r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    long int x = static_cast<long int>(3.14159);

    std::cout << "x " << std::setw(2) << "/" << " r      "
              << std::setw(22) << "long int"
              << std::setw(17) << (x / r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x / r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    unsigned int x = static_cast<unsigned int>(3.14159);

    std::cout << "x " << std::setw(2) << "/" << " r      "
              << std::setw(22) << "unsigned int"
              << std::setw(17) << (x / r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x / r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    int x = static_cast<int>(3.14159);

    std::cout << "x " << std::setw(2) << "/" << " r      "
              << std::setw(22) << "int"
              << std::setw(17) << (x / r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x / r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    unsigned short int x = static_cast<unsigned short int>(3.14159);

    std::cout << "x " << std::setw(2) << "/" << " r      "
              << std::setw(22) << "unsigned short int"
              << std::setw(17) << (x / r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x / r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    short int x = static_cast<short int>(3.14159);

    std::cout << "x " << std::setw(2) << "/" << " r      "
              << std::setw(22) << "short int"
              << std::setw(17) << (x / r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x / r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    unsigned char x = static_cast<unsigned char>(3.14159);

    std::cout << "x " << std::setw(2) << "/" << " r      "
              << std::setw(22) << "unsigned char"
              << std::setw(17) << (x / r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x / r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    signed char x = static_cast<signed char>(3.14159);

    std::cout << "x " << std::setw(2) << "/" << " r      "
              << std::setw(22) << "signed char"
              << std::setw(17) << (x / r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x / r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    char x = static_cast<char>(3.14159);

    std::cout << "x " << std::setw(2) << "/" << " r      "
              << std::setw(22) << "char"
              << std::setw(17) << (x / r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x / r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    double x = static_cast<double>(3.14159);

    std::cout << "x " << std::setw(2) << "/" << " r      "
              << std::setw(22) << "double"
              << std::setw(17) << (x / r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x / r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    float x = static_cast<float>(3.14159);

    std::cout << "x " << std::setw(2) << "/" << " r      "
              << std::setw(22) << "float"
              << std::setw(17) << (x / r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x / r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    long double x = static_cast<long double>(3.14159);

    std::cout << "x " << std::setw(2) << "/" << " r      "
              << std::setw(22) << "long double"
              << std::setw(17) << (x / r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x / r);
    std::cout << "    " << debug_out() << std::endl;
  }

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    mpz_t x;
    my_mp_init(x, 3.14159);

    std::cout << "x " << std::setw(2) << "/" << " r      "
              << std::setw(22) << "mpz_t"
              << std::setw(17) << (x / r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x / r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    mpq_t x;
    my_mp_init(x, 3.14159);

    std::cout << "x " << std::setw(2) << "/" << " r      "
              << std::setw(22) << "mpq_t"
              << std::setw(17) << (x / r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x / r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    mpf_t x;
    my_mp_init(x, 3.14159);

    std::cout << "x " << std::setw(2) << "/" << " r      "
              << std::setw(22) << "mpf_t"
              << std::setw(17) << (x / r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x / r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    mpfr_t x;
    my_mp_init(x, 3.14159);

    std::cout << "x " << std::setw(2) << "/" << " r      "
              << std::setw(22) << "mpfr_t"
              << std::setw(17) << (x / r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x / r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

  {
    mpfr::real<1024> r = -1.41421;
    mpz_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpz_ptr x = x_nc;

    std::cout << "x " << std::setw(2) << "/" << " r      "
              << std::setw(22) << "mpz_ptr"
              << std::setw(17) << (x / r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x / r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpq_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpq_ptr x = x_nc;

    std::cout << "x " << std::setw(2) << "/" << " r      "
              << std::setw(22) << "mpq_ptr"
              << std::setw(17) << (x / r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x / r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpf_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpf_ptr x = x_nc;

    std::cout << "x " << std::setw(2) << "/" << " r      "
              << std::setw(22) << "mpf_ptr"
              << std::setw(17) << (x / r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x / r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpfr_ptr x = x_nc;

    std::cout << "x " << std::setw(2) << "/" << " r      "
              << std::setw(22) << "mpfr_ptr"
              << std::setw(17) << (x / r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x / r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpz_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpz_srcptr x = x_nc;

    std::cout << "x " << std::setw(2) << "/" << " r      "
              << std::setw(22) << "mpz_srcptr"
              << std::setw(17) << (x / r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x / r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpq_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpq_srcptr x = x_nc;

    std::cout << "x " << std::setw(2) << "/" << " r      "
              << std::setw(22) << "mpq_srcptr"
              << std::setw(17) << (x / r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x / r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpf_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpf_srcptr x = x_nc;

    std::cout << "x " << std::setw(2) << "/" << " r      "
              << std::setw(22) << "mpf_srcptr"
              << std::setw(17) << (x / r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x / r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpfr_srcptr x = x_nc;

    std::cout << "x " << std::setw(2) << "/" << " r      "
              << std::setw(22) << "mpfr_srcptr"
              << std::setw(17) << (x / r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x / r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    std::string x = "3.14159";

    std::cout << "x " << std::setw(2) << "/" << " r      "
              << std::setw(22) << "std::string"
              << std::setw(17) << (x / r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x / r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    char x_nc[] = "3.14159";
    char* x = x_nc;

    std::cout << "x " << std::setw(2) << "/" << " r      "
              << std::setw(22) << "char*"
              << std::setw(17) << (x / r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x / r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    char x_nc[] = "3.14159";
    const char* x = x_nc;

    std::cout << "x " << std::setw(2) << "/" << " r      "
              << std::setw(22) << "const char*"
              << std::setw(17) << (x / r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x / r);
    std::cout << "    " << debug_out() << std::endl;
  }

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    char x[] = "3.14159";

    std::cout << "x " << std::setw(2) << "/" << " r      "
              << std::setw(22) << "char[]"
              << std::setw(17) << (x / r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x / r);
    std::cout << "    " << debug_out() << std::endl;
  }
#endif  // _MSC_VER

  {
    mpfr::real<1024> r = -1.41421;
    mpfr::real<512> x = 3.14159;

    std::cout << "x " << std::setw(2) << "/" << " r      "
              << std::setw(22) << "mpfr::real<512>"
              << std::setw(17) << (x / r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x / r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr::real<1024> x = 3.14159;

    std::cout << "x " << std::setw(2) << "/" << " r      "
              << std::setw(22) << "mpfr::real<1024>"
              << std::setw(17) << (x / r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x / r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr::real<2048> x = 3.14159;

    std::cout << "x " << std::setw(2) << "/" << " r      "
              << std::setw(22) << "mpfr::real<2048>"
              << std::setw(17) << (x / r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x / r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    unsigned long int x = static_cast<unsigned long int>(3.14159);

    std::cout << "x " << std::setw(2) << "==" << " r      "
              << std::setw(22) << "unsigned long int"
              << std::setw(17) << (x == r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x == r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    long int x = static_cast<long int>(3.14159);

    std::cout << "x " << std::setw(2) << "==" << " r      "
              << std::setw(22) << "long int"
              << std::setw(17) << (x == r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x == r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    unsigned int x = static_cast<unsigned int>(3.14159);

    std::cout << "x " << std::setw(2) << "==" << " r      "
              << std::setw(22) << "unsigned int"
              << std::setw(17) << (x == r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x == r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    int x = static_cast<int>(3.14159);

    std::cout << "x " << std::setw(2) << "==" << " r      "
              << std::setw(22) << "int"
              << std::setw(17) << (x == r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x == r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    unsigned short int x = static_cast<unsigned short int>(3.14159);

    std::cout << "x " << std::setw(2) << "==" << " r      "
              << std::setw(22) << "unsigned short int"
              << std::setw(17) << (x == r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x == r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    short int x = static_cast<short int>(3.14159);

    std::cout << "x " << std::setw(2) << "==" << " r      "
              << std::setw(22) << "short int"
              << std::setw(17) << (x == r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x == r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    unsigned char x = static_cast<unsigned char>(3.14159);

    std::cout << "x " << std::setw(2) << "==" << " r      "
              << std::setw(22) << "unsigned char"
              << std::setw(17) << (x == r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x == r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    signed char x = static_cast<signed char>(3.14159);

    std::cout << "x " << std::setw(2) << "==" << " r      "
              << std::setw(22) << "signed char"
              << std::setw(17) << (x == r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x == r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    char x = static_cast<char>(3.14159);

    std::cout << "x " << std::setw(2) << "==" << " r      "
              << std::setw(22) << "char"
              << std::setw(17) << (x == r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x == r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    double x = static_cast<double>(3.14159);

    std::cout << "x " << std::setw(2) << "==" << " r      "
              << std::setw(22) << "double"
              << std::setw(17) << (x == r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x == r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    float x = static_cast<float>(3.14159);

    std::cout << "x " << std::setw(2) << "==" << " r      "
              << std::setw(22) << "float"
              << std::setw(17) << (x == r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x == r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    long double x = static_cast<long double>(3.14159);

    std::cout << "x " << std::setw(2) << "==" << " r      "
              << std::setw(22) << "long double"
              << std::setw(17) << (x == r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x == r);
    std::cout << "    " << debug_out() << std::endl;
  }

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    mpz_t x;
    my_mp_init(x, 3.14159);

    std::cout << "x " << std::setw(2) << "==" << " r      "
              << std::setw(22) << "mpz_t"
              << std::setw(17) << (x == r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x == r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    mpq_t x;
    my_mp_init(x, 3.14159);

    std::cout << "x " << std::setw(2) << "==" << " r      "
              << std::setw(22) << "mpq_t"
              << std::setw(17) << (x == r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x == r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    mpf_t x;
    my_mp_init(x, 3.14159);

    std::cout << "x " << std::setw(2) << "==" << " r      "
              << std::setw(22) << "mpf_t"
              << std::setw(17) << (x == r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x == r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    mpfr_t x;
    my_mp_init(x, 3.14159);

    std::cout << "x " << std::setw(2) << "==" << " r      "
              << std::setw(22) << "mpfr_t"
              << std::setw(17) << (x == r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x == r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

  {
    mpfr::real<1024> r = -1.41421;
    mpz_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpz_ptr x = x_nc;

    std::cout << "x " << std::setw(2) << "==" << " r      "
              << std::setw(22) << "mpz_ptr"
              << std::setw(17) << (x == r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x == r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpq_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpq_ptr x = x_nc;

    std::cout << "x " << std::setw(2) << "==" << " r      "
              << std::setw(22) << "mpq_ptr"
              << std::setw(17) << (x == r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x == r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpf_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpf_ptr x = x_nc;

    std::cout << "x " << std::setw(2) << "==" << " r      "
              << std::setw(22) << "mpf_ptr"
              << std::setw(17) << (x == r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x == r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpfr_ptr x = x_nc;

    std::cout << "x " << std::setw(2) << "==" << " r      "
              << std::setw(22) << "mpfr_ptr"
              << std::setw(17) << (x == r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x == r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpz_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpz_srcptr x = x_nc;

    std::cout << "x " << std::setw(2) << "==" << " r      "
              << std::setw(22) << "mpz_srcptr"
              << std::setw(17) << (x == r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x == r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpq_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpq_srcptr x = x_nc;

    std::cout << "x " << std::setw(2) << "==" << " r      "
              << std::setw(22) << "mpq_srcptr"
              << std::setw(17) << (x == r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x == r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpf_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpf_srcptr x = x_nc;

    std::cout << "x " << std::setw(2) << "==" << " r      "
              << std::setw(22) << "mpf_srcptr"
              << std::setw(17) << (x == r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x == r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpfr_srcptr x = x_nc;

    std::cout << "x " << std::setw(2) << "==" << " r      "
              << std::setw(22) << "mpfr_srcptr"
              << std::setw(17) << (x == r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x == r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    std::string x = "3.14159";

    std::cout << "x " << std::setw(2) << "==" << " r      "
              << std::setw(22) << "std::string"
              << std::setw(17) << (x == r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x == r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    char x_nc[] = "3.14159";
    char* x = x_nc;

    std::cout << "x " << std::setw(2) << "==" << " r      "
              << std::setw(22) << "char*"
              << std::setw(17) << (x == r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x == r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    char x_nc[] = "3.14159";
    const char* x = x_nc;

    std::cout << "x " << std::setw(2) << "==" << " r      "
              << std::setw(22) << "const char*"
              << std::setw(17) << (x == r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x == r);
    std::cout << "    " << debug_out() << std::endl;
  }

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    char x[] = "3.14159";

    std::cout << "x " << std::setw(2) << "==" << " r      "
              << std::setw(22) << "char[]"
              << std::setw(17) << (x == r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x == r);
    std::cout << "    " << debug_out() << std::endl;
  }
#endif  // _MSC_VER

  {
    mpfr::real<1024> r = -1.41421;
    mpfr::real<512> x = 3.14159;

    std::cout << "x " << std::setw(2) << "==" << " r      "
              << std::setw(22) << "mpfr::real<512>"
              << std::setw(17) << (x == r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x == r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr::real<1024> x = 3.14159;

    std::cout << "x " << std::setw(2) << "==" << " r      "
              << std::setw(22) << "mpfr::real<1024>"
              << std::setw(17) << (x == r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x == r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr::real<2048> x = 3.14159;

    std::cout << "x " << std::setw(2) << "==" << " r      "
              << std::setw(22) << "mpfr::real<2048>"
              << std::setw(17) << (x == r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x == r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    unsigned long int x = static_cast<unsigned long int>(3.14159);

    std::cout << "x " << std::setw(2) << "!=" << " r      "
              << std::setw(22) << "unsigned long int"
              << std::setw(17) << (x != r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x != r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    long int x = static_cast<long int>(3.14159);

    std::cout << "x " << std::setw(2) << "!=" << " r      "
              << std::setw(22) << "long int"
              << std::setw(17) << (x != r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x != r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    unsigned int x = static_cast<unsigned int>(3.14159);

    std::cout << "x " << std::setw(2) << "!=" << " r      "
              << std::setw(22) << "unsigned int"
              << std::setw(17) << (x != r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x != r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    int x = static_cast<int>(3.14159);

    std::cout << "x " << std::setw(2) << "!=" << " r      "
              << std::setw(22) << "int"
              << std::setw(17) << (x != r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x != r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    unsigned short int x = static_cast<unsigned short int>(3.14159);

    std::cout << "x " << std::setw(2) << "!=" << " r      "
              << std::setw(22) << "unsigned short int"
              << std::setw(17) << (x != r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x != r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    short int x = static_cast<short int>(3.14159);

    std::cout << "x " << std::setw(2) << "!=" << " r      "
              << std::setw(22) << "short int"
              << std::setw(17) << (x != r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x != r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    unsigned char x = static_cast<unsigned char>(3.14159);

    std::cout << "x " << std::setw(2) << "!=" << " r      "
              << std::setw(22) << "unsigned char"
              << std::setw(17) << (x != r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x != r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    signed char x = static_cast<signed char>(3.14159);

    std::cout << "x " << std::setw(2) << "!=" << " r      "
              << std::setw(22) << "signed char"
              << std::setw(17) << (x != r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x != r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    char x = static_cast<char>(3.14159);

    std::cout << "x " << std::setw(2) << "!=" << " r      "
              << std::setw(22) << "char"
              << std::setw(17) << (x != r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x != r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    double x = static_cast<double>(3.14159);

    std::cout << "x " << std::setw(2) << "!=" << " r      "
              << std::setw(22) << "double"
              << std::setw(17) << (x != r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x != r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    float x = static_cast<float>(3.14159);

    std::cout << "x " << std::setw(2) << "!=" << " r      "
              << std::setw(22) << "float"
              << std::setw(17) << (x != r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x != r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    long double x = static_cast<long double>(3.14159);

    std::cout << "x " << std::setw(2) << "!=" << " r      "
              << std::setw(22) << "long double"
              << std::setw(17) << (x != r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x != r);
    std::cout << "    " << debug_out() << std::endl;
  }

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    mpz_t x;
    my_mp_init(x, 3.14159);

    std::cout << "x " << std::setw(2) << "!=" << " r      "
              << std::setw(22) << "mpz_t"
              << std::setw(17) << (x != r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x != r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    mpq_t x;
    my_mp_init(x, 3.14159);

    std::cout << "x " << std::setw(2) << "!=" << " r      "
              << std::setw(22) << "mpq_t"
              << std::setw(17) << (x != r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x != r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    mpf_t x;
    my_mp_init(x, 3.14159);

    std::cout << "x " << std::setw(2) << "!=" << " r      "
              << std::setw(22) << "mpf_t"
              << std::setw(17) << (x != r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x != r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    mpfr_t x;
    my_mp_init(x, 3.14159);

    std::cout << "x " << std::setw(2) << "!=" << " r      "
              << std::setw(22) << "mpfr_t"
              << std::setw(17) << (x != r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x != r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

  {
    mpfr::real<1024> r = -1.41421;
    mpz_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpz_ptr x = x_nc;

    std::cout << "x " << std::setw(2) << "!=" << " r      "
              << std::setw(22) << "mpz_ptr"
              << std::setw(17) << (x != r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x != r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpq_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpq_ptr x = x_nc;

    std::cout << "x " << std::setw(2) << "!=" << " r      "
              << std::setw(22) << "mpq_ptr"
              << std::setw(17) << (x != r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x != r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpf_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpf_ptr x = x_nc;

    std::cout << "x " << std::setw(2) << "!=" << " r      "
              << std::setw(22) << "mpf_ptr"
              << std::setw(17) << (x != r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x != r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpfr_ptr x = x_nc;

    std::cout << "x " << std::setw(2) << "!=" << " r      "
              << std::setw(22) << "mpfr_ptr"
              << std::setw(17) << (x != r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x != r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpz_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpz_srcptr x = x_nc;

    std::cout << "x " << std::setw(2) << "!=" << " r      "
              << std::setw(22) << "mpz_srcptr"
              << std::setw(17) << (x != r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x != r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpq_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpq_srcptr x = x_nc;

    std::cout << "x " << std::setw(2) << "!=" << " r      "
              << std::setw(22) << "mpq_srcptr"
              << std::setw(17) << (x != r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x != r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpf_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpf_srcptr x = x_nc;

    std::cout << "x " << std::setw(2) << "!=" << " r      "
              << std::setw(22) << "mpf_srcptr"
              << std::setw(17) << (x != r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x != r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpfr_srcptr x = x_nc;

    std::cout << "x " << std::setw(2) << "!=" << " r      "
              << std::setw(22) << "mpfr_srcptr"
              << std::setw(17) << (x != r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x != r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    std::string x = "3.14159";

    std::cout << "x " << std::setw(2) << "!=" << " r      "
              << std::setw(22) << "std::string"
              << std::setw(17) << (x != r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x != r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    char x_nc[] = "3.14159";
    char* x = x_nc;

    std::cout << "x " << std::setw(2) << "!=" << " r      "
              << std::setw(22) << "char*"
              << std::setw(17) << (x != r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x != r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    char x_nc[] = "3.14159";
    const char* x = x_nc;

    std::cout << "x " << std::setw(2) << "!=" << " r      "
              << std::setw(22) << "const char*"
              << std::setw(17) << (x != r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x != r);
    std::cout << "    " << debug_out() << std::endl;
  }

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    char x[] = "3.14159";

    std::cout << "x " << std::setw(2) << "!=" << " r      "
              << std::setw(22) << "char[]"
              << std::setw(17) << (x != r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x != r);
    std::cout << "    " << debug_out() << std::endl;
  }
#endif  // _MSC_VER

  {
    mpfr::real<1024> r = -1.41421;
    mpfr::real<512> x = 3.14159;

    std::cout << "x " << std::setw(2) << "!=" << " r      "
              << std::setw(22) << "mpfr::real<512>"
              << std::setw(17) << (x != r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x != r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr::real<1024> x = 3.14159;

    std::cout << "x " << std::setw(2) << "!=" << " r      "
              << std::setw(22) << "mpfr::real<1024>"
              << std::setw(17) << (x != r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x != r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr::real<2048> x = 3.14159;

    std::cout << "x " << std::setw(2) << "!=" << " r      "
              << std::setw(22) << "mpfr::real<2048>"
              << std::setw(17) << (x != r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x != r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    unsigned long int x = static_cast<unsigned long int>(3.14159);

    std::cout << "x " << std::setw(2) << "<" << " r      "
              << std::setw(22) << "unsigned long int"
              << std::setw(17) << (x < r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x < r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    long int x = static_cast<long int>(3.14159);

    std::cout << "x " << std::setw(2) << "<" << " r      "
              << std::setw(22) << "long int"
              << std::setw(17) << (x < r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x < r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    unsigned int x = static_cast<unsigned int>(3.14159);

    std::cout << "x " << std::setw(2) << "<" << " r      "
              << std::setw(22) << "unsigned int"
              << std::setw(17) << (x < r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x < r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    int x = static_cast<int>(3.14159);

    std::cout << "x " << std::setw(2) << "<" << " r      "
              << std::setw(22) << "int"
              << std::setw(17) << (x < r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x < r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    unsigned short int x = static_cast<unsigned short int>(3.14159);

    std::cout << "x " << std::setw(2) << "<" << " r      "
              << std::setw(22) << "unsigned short int"
              << std::setw(17) << (x < r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x < r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    short int x = static_cast<short int>(3.14159);

    std::cout << "x " << std::setw(2) << "<" << " r      "
              << std::setw(22) << "short int"
              << std::setw(17) << (x < r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x < r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    unsigned char x = static_cast<unsigned char>(3.14159);

    std::cout << "x " << std::setw(2) << "<" << " r      "
              << std::setw(22) << "unsigned char"
              << std::setw(17) << (x < r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x < r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    signed char x = static_cast<signed char>(3.14159);

    std::cout << "x " << std::setw(2) << "<" << " r      "
              << std::setw(22) << "signed char"
              << std::setw(17) << (x < r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x < r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    char x = static_cast<char>(3.14159);

    std::cout << "x " << std::setw(2) << "<" << " r      "
              << std::setw(22) << "char"
              << std::setw(17) << (x < r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x < r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    double x = static_cast<double>(3.14159);

    std::cout << "x " << std::setw(2) << "<" << " r      "
              << std::setw(22) << "double"
              << std::setw(17) << (x < r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x < r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    float x = static_cast<float>(3.14159);

    std::cout << "x " << std::setw(2) << "<" << " r      "
              << std::setw(22) << "float"
              << std::setw(17) << (x < r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x < r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    long double x = static_cast<long double>(3.14159);

    std::cout << "x " << std::setw(2) << "<" << " r      "
              << std::setw(22) << "long double"
              << std::setw(17) << (x < r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x < r);
    std::cout << "    " << debug_out() << std::endl;
  }

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    mpz_t x;
    my_mp_init(x, 3.14159);

    std::cout << "x " << std::setw(2) << "<" << " r      "
              << std::setw(22) << "mpz_t"
              << std::setw(17) << (x < r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x < r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    mpq_t x;
    my_mp_init(x, 3.14159);

    std::cout << "x " << std::setw(2) << "<" << " r      "
              << std::setw(22) << "mpq_t"
              << std::setw(17) << (x < r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x < r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    mpf_t x;
    my_mp_init(x, 3.14159);

    std::cout << "x " << std::setw(2) << "<" << " r      "
              << std::setw(22) << "mpf_t"
              << std::setw(17) << (x < r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x < r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    mpfr_t x;
    my_mp_init(x, 3.14159);

    std::cout << "x " << std::setw(2) << "<" << " r      "
              << std::setw(22) << "mpfr_t"
              << std::setw(17) << (x < r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x < r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

  {
    mpfr::real<1024> r = -1.41421;
    mpz_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpz_ptr x = x_nc;

    std::cout << "x " << std::setw(2) << "<" << " r      "
              << std::setw(22) << "mpz_ptr"
              << std::setw(17) << (x < r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x < r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpq_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpq_ptr x = x_nc;

    std::cout << "x " << std::setw(2) << "<" << " r      "
              << std::setw(22) << "mpq_ptr"
              << std::setw(17) << (x < r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x < r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpf_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpf_ptr x = x_nc;

    std::cout << "x " << std::setw(2) << "<" << " r      "
              << std::setw(22) << "mpf_ptr"
              << std::setw(17) << (x < r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x < r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpfr_ptr x = x_nc;

    std::cout << "x " << std::setw(2) << "<" << " r      "
              << std::setw(22) << "mpfr_ptr"
              << std::setw(17) << (x < r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x < r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpz_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpz_srcptr x = x_nc;

    std::cout << "x " << std::setw(2) << "<" << " r      "
              << std::setw(22) << "mpz_srcptr"
              << std::setw(17) << (x < r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x < r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpq_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpq_srcptr x = x_nc;

    std::cout << "x " << std::setw(2) << "<" << " r      "
              << std::setw(22) << "mpq_srcptr"
              << std::setw(17) << (x < r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x < r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpf_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpf_srcptr x = x_nc;

    std::cout << "x " << std::setw(2) << "<" << " r      "
              << std::setw(22) << "mpf_srcptr"
              << std::setw(17) << (x < r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x < r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpfr_srcptr x = x_nc;

    std::cout << "x " << std::setw(2) << "<" << " r      "
              << std::setw(22) << "mpfr_srcptr"
              << std::setw(17) << (x < r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x < r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    std::string x = "3.14159";

    std::cout << "x " << std::setw(2) << "<" << " r      "
              << std::setw(22) << "std::string"
              << std::setw(17) << (x < r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x < r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    char x_nc[] = "3.14159";
    char* x = x_nc;

    std::cout << "x " << std::setw(2) << "<" << " r      "
              << std::setw(22) << "char*"
              << std::setw(17) << (x < r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x < r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    char x_nc[] = "3.14159";
    const char* x = x_nc;

    std::cout << "x " << std::setw(2) << "<" << " r      "
              << std::setw(22) << "const char*"
              << std::setw(17) << (x < r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x < r);
    std::cout << "    " << debug_out() << std::endl;
  }

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    char x[] = "3.14159";

    std::cout << "x " << std::setw(2) << "<" << " r      "
              << std::setw(22) << "char[]"
              << std::setw(17) << (x < r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x < r);
    std::cout << "    " << debug_out() << std::endl;
  }
#endif  // _MSC_VER

  {
    mpfr::real<1024> r = -1.41421;
    mpfr::real<512> x = 3.14159;

    std::cout << "x " << std::setw(2) << "<" << " r      "
              << std::setw(22) << "mpfr::real<512>"
              << std::setw(17) << (x < r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x < r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr::real<1024> x = 3.14159;

    std::cout << "x " << std::setw(2) << "<" << " r      "
              << std::setw(22) << "mpfr::real<1024>"
              << std::setw(17) << (x < r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x < r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr::real<2048> x = 3.14159;

    std::cout << "x " << std::setw(2) << "<" << " r      "
              << std::setw(22) << "mpfr::real<2048>"
              << std::setw(17) << (x < r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x < r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    unsigned long int x = static_cast<unsigned long int>(3.14159);

    std::cout << "x " << std::setw(2) << "<=" << " r      "
              << std::setw(22) << "unsigned long int"
              << std::setw(17) << (x <= r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x <= r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    long int x = static_cast<long int>(3.14159);

    std::cout << "x " << std::setw(2) << "<=" << " r      "
              << std::setw(22) << "long int"
              << std::setw(17) << (x <= r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x <= r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    unsigned int x = static_cast<unsigned int>(3.14159);

    std::cout << "x " << std::setw(2) << "<=" << " r      "
              << std::setw(22) << "unsigned int"
              << std::setw(17) << (x <= r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x <= r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    int x = static_cast<int>(3.14159);

    std::cout << "x " << std::setw(2) << "<=" << " r      "
              << std::setw(22) << "int"
              << std::setw(17) << (x <= r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x <= r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    unsigned short int x = static_cast<unsigned short int>(3.14159);

    std::cout << "x " << std::setw(2) << "<=" << " r      "
              << std::setw(22) << "unsigned short int"
              << std::setw(17) << (x <= r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x <= r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    short int x = static_cast<short int>(3.14159);

    std::cout << "x " << std::setw(2) << "<=" << " r      "
              << std::setw(22) << "short int"
              << std::setw(17) << (x <= r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x <= r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    unsigned char x = static_cast<unsigned char>(3.14159);

    std::cout << "x " << std::setw(2) << "<=" << " r      "
              << std::setw(22) << "unsigned char"
              << std::setw(17) << (x <= r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x <= r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    signed char x = static_cast<signed char>(3.14159);

    std::cout << "x " << std::setw(2) << "<=" << " r      "
              << std::setw(22) << "signed char"
              << std::setw(17) << (x <= r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x <= r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    char x = static_cast<char>(3.14159);

    std::cout << "x " << std::setw(2) << "<=" << " r      "
              << std::setw(22) << "char"
              << std::setw(17) << (x <= r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x <= r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    double x = static_cast<double>(3.14159);

    std::cout << "x " << std::setw(2) << "<=" << " r      "
              << std::setw(22) << "double"
              << std::setw(17) << (x <= r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x <= r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    float x = static_cast<float>(3.14159);

    std::cout << "x " << std::setw(2) << "<=" << " r      "
              << std::setw(22) << "float"
              << std::setw(17) << (x <= r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x <= r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    long double x = static_cast<long double>(3.14159);

    std::cout << "x " << std::setw(2) << "<=" << " r      "
              << std::setw(22) << "long double"
              << std::setw(17) << (x <= r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x <= r);
    std::cout << "    " << debug_out() << std::endl;
  }

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    mpz_t x;
    my_mp_init(x, 3.14159);

    std::cout << "x " << std::setw(2) << "<=" << " r      "
              << std::setw(22) << "mpz_t"
              << std::setw(17) << (x <= r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x <= r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    mpq_t x;
    my_mp_init(x, 3.14159);

    std::cout << "x " << std::setw(2) << "<=" << " r      "
              << std::setw(22) << "mpq_t"
              << std::setw(17) << (x <= r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x <= r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    mpf_t x;
    my_mp_init(x, 3.14159);

    std::cout << "x " << std::setw(2) << "<=" << " r      "
              << std::setw(22) << "mpf_t"
              << std::setw(17) << (x <= r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x <= r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    mpfr_t x;
    my_mp_init(x, 3.14159);

    std::cout << "x " << std::setw(2) << "<=" << " r      "
              << std::setw(22) << "mpfr_t"
              << std::setw(17) << (x <= r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x <= r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

  {
    mpfr::real<1024> r = -1.41421;
    mpz_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpz_ptr x = x_nc;

    std::cout << "x " << std::setw(2) << "<=" << " r      "
              << std::setw(22) << "mpz_ptr"
              << std::setw(17) << (x <= r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x <= r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpq_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpq_ptr x = x_nc;

    std::cout << "x " << std::setw(2) << "<=" << " r      "
              << std::setw(22) << "mpq_ptr"
              << std::setw(17) << (x <= r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x <= r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpf_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpf_ptr x = x_nc;

    std::cout << "x " << std::setw(2) << "<=" << " r      "
              << std::setw(22) << "mpf_ptr"
              << std::setw(17) << (x <= r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x <= r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpfr_ptr x = x_nc;

    std::cout << "x " << std::setw(2) << "<=" << " r      "
              << std::setw(22) << "mpfr_ptr"
              << std::setw(17) << (x <= r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x <= r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpz_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpz_srcptr x = x_nc;

    std::cout << "x " << std::setw(2) << "<=" << " r      "
              << std::setw(22) << "mpz_srcptr"
              << std::setw(17) << (x <= r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x <= r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpq_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpq_srcptr x = x_nc;

    std::cout << "x " << std::setw(2) << "<=" << " r      "
              << std::setw(22) << "mpq_srcptr"
              << std::setw(17) << (x <= r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x <= r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpf_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpf_srcptr x = x_nc;

    std::cout << "x " << std::setw(2) << "<=" << " r      "
              << std::setw(22) << "mpf_srcptr"
              << std::setw(17) << (x <= r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x <= r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpfr_srcptr x = x_nc;

    std::cout << "x " << std::setw(2) << "<=" << " r      "
              << std::setw(22) << "mpfr_srcptr"
              << std::setw(17) << (x <= r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x <= r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    std::string x = "3.14159";

    std::cout << "x " << std::setw(2) << "<=" << " r      "
              << std::setw(22) << "std::string"
              << std::setw(17) << (x <= r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x <= r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    char x_nc[] = "3.14159";
    char* x = x_nc;

    std::cout << "x " << std::setw(2) << "<=" << " r      "
              << std::setw(22) << "char*"
              << std::setw(17) << (x <= r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x <= r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    char x_nc[] = "3.14159";
    const char* x = x_nc;

    std::cout << "x " << std::setw(2) << "<=" << " r      "
              << std::setw(22) << "const char*"
              << std::setw(17) << (x <= r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x <= r);
    std::cout << "    " << debug_out() << std::endl;
  }

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    char x[] = "3.14159";

    std::cout << "x " << std::setw(2) << "<=" << " r      "
              << std::setw(22) << "char[]"
              << std::setw(17) << (x <= r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x <= r);
    std::cout << "    " << debug_out() << std::endl;
  }
#endif  // _MSC_VER

  {
    mpfr::real<1024> r = -1.41421;
    mpfr::real<512> x = 3.14159;

    std::cout << "x " << std::setw(2) << "<=" << " r      "
              << std::setw(22) << "mpfr::real<512>"
              << std::setw(17) << (x <= r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x <= r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr::real<1024> x = 3.14159;

    std::cout << "x " << std::setw(2) << "<=" << " r      "
              << std::setw(22) << "mpfr::real<1024>"
              << std::setw(17) << (x <= r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x <= r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr::real<2048> x = 3.14159;

    std::cout << "x " << std::setw(2) << "<=" << " r      "
              << std::setw(22) << "mpfr::real<2048>"
              << std::setw(17) << (x <= r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x <= r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    unsigned long int x = static_cast<unsigned long int>(3.14159);

    std::cout << "x " << std::setw(2) << ">" << " r      "
              << std::setw(22) << "unsigned long int"
              << std::setw(17) << (x > r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x > r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    long int x = static_cast<long int>(3.14159);

    std::cout << "x " << std::setw(2) << ">" << " r      "
              << std::setw(22) << "long int"
              << std::setw(17) << (x > r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x > r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    unsigned int x = static_cast<unsigned int>(3.14159);

    std::cout << "x " << std::setw(2) << ">" << " r      "
              << std::setw(22) << "unsigned int"
              << std::setw(17) << (x > r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x > r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    int x = static_cast<int>(3.14159);

    std::cout << "x " << std::setw(2) << ">" << " r      "
              << std::setw(22) << "int"
              << std::setw(17) << (x > r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x > r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    unsigned short int x = static_cast<unsigned short int>(3.14159);

    std::cout << "x " << std::setw(2) << ">" << " r      "
              << std::setw(22) << "unsigned short int"
              << std::setw(17) << (x > r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x > r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    short int x = static_cast<short int>(3.14159);

    std::cout << "x " << std::setw(2) << ">" << " r      "
              << std::setw(22) << "short int"
              << std::setw(17) << (x > r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x > r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    unsigned char x = static_cast<unsigned char>(3.14159);

    std::cout << "x " << std::setw(2) << ">" << " r      "
              << std::setw(22) << "unsigned char"
              << std::setw(17) << (x > r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x > r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    signed char x = static_cast<signed char>(3.14159);

    std::cout << "x " << std::setw(2) << ">" << " r      "
              << std::setw(22) << "signed char"
              << std::setw(17) << (x > r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x > r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    char x = static_cast<char>(3.14159);

    std::cout << "x " << std::setw(2) << ">" << " r      "
              << std::setw(22) << "char"
              << std::setw(17) << (x > r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x > r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    double x = static_cast<double>(3.14159);

    std::cout << "x " << std::setw(2) << ">" << " r      "
              << std::setw(22) << "double"
              << std::setw(17) << (x > r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x > r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    float x = static_cast<float>(3.14159);

    std::cout << "x " << std::setw(2) << ">" << " r      "
              << std::setw(22) << "float"
              << std::setw(17) << (x > r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x > r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    long double x = static_cast<long double>(3.14159);

    std::cout << "x " << std::setw(2) << ">" << " r      "
              << std::setw(22) << "long double"
              << std::setw(17) << (x > r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x > r);
    std::cout << "    " << debug_out() << std::endl;
  }

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    mpz_t x;
    my_mp_init(x, 3.14159);

    std::cout << "x " << std::setw(2) << ">" << " r      "
              << std::setw(22) << "mpz_t"
              << std::setw(17) << (x > r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x > r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    mpq_t x;
    my_mp_init(x, 3.14159);

    std::cout << "x " << std::setw(2) << ">" << " r      "
              << std::setw(22) << "mpq_t"
              << std::setw(17) << (x > r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x > r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    mpf_t x;
    my_mp_init(x, 3.14159);

    std::cout << "x " << std::setw(2) << ">" << " r      "
              << std::setw(22) << "mpf_t"
              << std::setw(17) << (x > r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x > r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    mpfr_t x;
    my_mp_init(x, 3.14159);

    std::cout << "x " << std::setw(2) << ">" << " r      "
              << std::setw(22) << "mpfr_t"
              << std::setw(17) << (x > r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x > r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

  {
    mpfr::real<1024> r = -1.41421;
    mpz_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpz_ptr x = x_nc;

    std::cout << "x " << std::setw(2) << ">" << " r      "
              << std::setw(22) << "mpz_ptr"
              << std::setw(17) << (x > r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x > r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpq_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpq_ptr x = x_nc;

    std::cout << "x " << std::setw(2) << ">" << " r      "
              << std::setw(22) << "mpq_ptr"
              << std::setw(17) << (x > r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x > r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpf_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpf_ptr x = x_nc;

    std::cout << "x " << std::setw(2) << ">" << " r      "
              << std::setw(22) << "mpf_ptr"
              << std::setw(17) << (x > r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x > r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpfr_ptr x = x_nc;

    std::cout << "x " << std::setw(2) << ">" << " r      "
              << std::setw(22) << "mpfr_ptr"
              << std::setw(17) << (x > r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x > r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpz_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpz_srcptr x = x_nc;

    std::cout << "x " << std::setw(2) << ">" << " r      "
              << std::setw(22) << "mpz_srcptr"
              << std::setw(17) << (x > r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x > r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpq_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpq_srcptr x = x_nc;

    std::cout << "x " << std::setw(2) << ">" << " r      "
              << std::setw(22) << "mpq_srcptr"
              << std::setw(17) << (x > r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x > r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpf_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpf_srcptr x = x_nc;

    std::cout << "x " << std::setw(2) << ">" << " r      "
              << std::setw(22) << "mpf_srcptr"
              << std::setw(17) << (x > r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x > r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpfr_srcptr x = x_nc;

    std::cout << "x " << std::setw(2) << ">" << " r      "
              << std::setw(22) << "mpfr_srcptr"
              << std::setw(17) << (x > r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x > r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    std::string x = "3.14159";

    std::cout << "x " << std::setw(2) << ">" << " r      "
              << std::setw(22) << "std::string"
              << std::setw(17) << (x > r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x > r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    char x_nc[] = "3.14159";
    char* x = x_nc;

    std::cout << "x " << std::setw(2) << ">" << " r      "
              << std::setw(22) << "char*"
              << std::setw(17) << (x > r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x > r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    char x_nc[] = "3.14159";
    const char* x = x_nc;

    std::cout << "x " << std::setw(2) << ">" << " r      "
              << std::setw(22) << "const char*"
              << std::setw(17) << (x > r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x > r);
    std::cout << "    " << debug_out() << std::endl;
  }

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    char x[] = "3.14159";

    std::cout << "x " << std::setw(2) << ">" << " r      "
              << std::setw(22) << "char[]"
              << std::setw(17) << (x > r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x > r);
    std::cout << "    " << debug_out() << std::endl;
  }
#endif  // _MSC_VER

  {
    mpfr::real<1024> r = -1.41421;
    mpfr::real<512> x = 3.14159;

    std::cout << "x " << std::setw(2) << ">" << " r      "
              << std::setw(22) << "mpfr::real<512>"
              << std::setw(17) << (x > r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x > r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr::real<1024> x = 3.14159;

    std::cout << "x " << std::setw(2) << ">" << " r      "
              << std::setw(22) << "mpfr::real<1024>"
              << std::setw(17) << (x > r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x > r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr::real<2048> x = 3.14159;

    std::cout << "x " << std::setw(2) << ">" << " r      "
              << std::setw(22) << "mpfr::real<2048>"
              << std::setw(17) << (x > r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x > r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    unsigned long int x = static_cast<unsigned long int>(3.14159);

    std::cout << "x " << std::setw(2) << ">=" << " r      "
              << std::setw(22) << "unsigned long int"
              << std::setw(17) << (x >= r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x >= r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    long int x = static_cast<long int>(3.14159);

    std::cout << "x " << std::setw(2) << ">=" << " r      "
              << std::setw(22) << "long int"
              << std::setw(17) << (x >= r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x >= r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    unsigned int x = static_cast<unsigned int>(3.14159);

    std::cout << "x " << std::setw(2) << ">=" << " r      "
              << std::setw(22) << "unsigned int"
              << std::setw(17) << (x >= r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x >= r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    int x = static_cast<int>(3.14159);

    std::cout << "x " << std::setw(2) << ">=" << " r      "
              << std::setw(22) << "int"
              << std::setw(17) << (x >= r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x >= r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    unsigned short int x = static_cast<unsigned short int>(3.14159);

    std::cout << "x " << std::setw(2) << ">=" << " r      "
              << std::setw(22) << "unsigned short int"
              << std::setw(17) << (x >= r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x >= r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    short int x = static_cast<short int>(3.14159);

    std::cout << "x " << std::setw(2) << ">=" << " r      "
              << std::setw(22) << "short int"
              << std::setw(17) << (x >= r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x >= r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    unsigned char x = static_cast<unsigned char>(3.14159);

    std::cout << "x " << std::setw(2) << ">=" << " r      "
              << std::setw(22) << "unsigned char"
              << std::setw(17) << (x >= r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x >= r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    signed char x = static_cast<signed char>(3.14159);

    std::cout << "x " << std::setw(2) << ">=" << " r      "
              << std::setw(22) << "signed char"
              << std::setw(17) << (x >= r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x >= r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    char x = static_cast<char>(3.14159);

    std::cout << "x " << std::setw(2) << ">=" << " r      "
              << std::setw(22) << "char"
              << std::setw(17) << (x >= r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x >= r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    double x = static_cast<double>(3.14159);

    std::cout << "x " << std::setw(2) << ">=" << " r      "
              << std::setw(22) << "double"
              << std::setw(17) << (x >= r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x >= r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    float x = static_cast<float>(3.14159);

    std::cout << "x " << std::setw(2) << ">=" << " r      "
              << std::setw(22) << "float"
              << std::setw(17) << (x >= r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x >= r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    long double x = static_cast<long double>(3.14159);

    std::cout << "x " << std::setw(2) << ">=" << " r      "
              << std::setw(22) << "long double"
              << std::setw(17) << (x >= r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x >= r);
    std::cout << "    " << debug_out() << std::endl;
  }

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    mpz_t x;
    my_mp_init(x, 3.14159);

    std::cout << "x " << std::setw(2) << ">=" << " r      "
              << std::setw(22) << "mpz_t"
              << std::setw(17) << (x >= r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x >= r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    mpq_t x;
    my_mp_init(x, 3.14159);

    std::cout << "x " << std::setw(2) << ">=" << " r      "
              << std::setw(22) << "mpq_t"
              << std::setw(17) << (x >= r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x >= r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    mpf_t x;
    my_mp_init(x, 3.14159);

    std::cout << "x " << std::setw(2) << ">=" << " r      "
              << std::setw(22) << "mpf_t"
              << std::setw(17) << (x >= r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x >= r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    mpfr_t x;
    my_mp_init(x, 3.14159);

    std::cout << "x " << std::setw(2) << ">=" << " r      "
              << std::setw(22) << "mpfr_t"
              << std::setw(17) << (x >= r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x >= r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

  {
    mpfr::real<1024> r = -1.41421;
    mpz_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpz_ptr x = x_nc;

    std::cout << "x " << std::setw(2) << ">=" << " r      "
              << std::setw(22) << "mpz_ptr"
              << std::setw(17) << (x >= r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x >= r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpq_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpq_ptr x = x_nc;

    std::cout << "x " << std::setw(2) << ">=" << " r      "
              << std::setw(22) << "mpq_ptr"
              << std::setw(17) << (x >= r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x >= r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpf_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpf_ptr x = x_nc;

    std::cout << "x " << std::setw(2) << ">=" << " r      "
              << std::setw(22) << "mpf_ptr"
              << std::setw(17) << (x >= r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x >= r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpfr_ptr x = x_nc;

    std::cout << "x " << std::setw(2) << ">=" << " r      "
              << std::setw(22) << "mpfr_ptr"
              << std::setw(17) << (x >= r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x >= r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpz_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpz_srcptr x = x_nc;

    std::cout << "x " << std::setw(2) << ">=" << " r      "
              << std::setw(22) << "mpz_srcptr"
              << std::setw(17) << (x >= r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x >= r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpq_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpq_srcptr x = x_nc;

    std::cout << "x " << std::setw(2) << ">=" << " r      "
              << std::setw(22) << "mpq_srcptr"
              << std::setw(17) << (x >= r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x >= r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpf_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpf_srcptr x = x_nc;

    std::cout << "x " << std::setw(2) << ">=" << " r      "
              << std::setw(22) << "mpf_srcptr"
              << std::setw(17) << (x >= r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x >= r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpfr_srcptr x = x_nc;

    std::cout << "x " << std::setw(2) << ">=" << " r      "
              << std::setw(22) << "mpfr_srcptr"
              << std::setw(17) << (x >= r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x >= r);
    std::cout << "    " << debug_out() << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    std::string x = "3.14159";

    std::cout << "x " << std::setw(2) << ">=" << " r      "
              << std::setw(22) << "std::string"
              << std::setw(17) << (x >= r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x >= r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    char x_nc[] = "3.14159";
    char* x = x_nc;

    std::cout << "x " << std::setw(2) << ">=" << " r      "
              << std::setw(22) << "char*"
              << std::setw(17) << (x >= r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x >= r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    char x_nc[] = "3.14159";
    const char* x = x_nc;

    std::cout << "x " << std::setw(2) << ">=" << " r      "
              << std::setw(22) << "const char*"
              << std::setw(17) << (x >= r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x >= r);
    std::cout << "    " << debug_out() << std::endl;
  }

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    char x[] = "3.14159";

    std::cout << "x " << std::setw(2) << ">=" << " r      "
              << std::setw(22) << "char[]"
              << std::setw(17) << (x >= r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x >= r);
    std::cout << "    " << debug_out() << std::endl;
  }
#endif  // _MSC_VER

  {
    mpfr::real<1024> r = -1.41421;
    mpfr::real<512> x = 3.14159;

    std::cout << "x " << std::setw(2) << ">=" << " r      "
              << std::setw(22) << "mpfr::real<512>"
              << std::setw(17) << (x >= r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x >= r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr::real<1024> x = 3.14159;

    std::cout << "x " << std::setw(2) << ">=" << " r      "
              << std::setw(22) << "mpfr::real<1024>"
              << std::setw(17) << (x >= r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x >= r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr::real<2048> x = 3.14159;

    std::cout << "x " << std::setw(2) << ">=" << " r      "
              << std::setw(22) << "mpfr::real<2048>"
              << std::setw(17) << (x >= r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x >= r);
    std::cout << "    " << debug_out() << std::endl;
  }

  // ////////////////////////////////////////////////////////////////////
  // implicit conversion operators
  // ////////////////////////////////////////////////////////////////////

#ifdef MPFR_REAL_ENABLE_CONV_OPS
  {
    mpfr::real<1024> r = -1.41421;
    unsigned long int x = static_cast<unsigned long int>(3.14159);

    std::cout << "x " << std::setw(2) << "=" << " r      "
              << std::setw(22) << "unsigned long int"
              << std::setw(17) << (x = r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x = r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    long int x = static_cast<long int>(3.14159);

    std::cout << "x " << std::setw(2) << "=" << " r      "
              << std::setw(22) << "long int"
              << std::setw(17) << (x = r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x = r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    unsigned int x = static_cast<unsigned int>(3.14159);

    std::cout << "x " << std::setw(2) << "=" << " r      "
              << std::setw(22) << "unsigned int"
              << std::setw(17) << (x = r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x = r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    int x = static_cast<int>(3.14159);

    std::cout << "x " << std::setw(2) << "=" << " r      "
              << std::setw(22) << "int"
              << std::setw(17) << (x = r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x = r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    unsigned short int x = static_cast<unsigned short int>(3.14159);

    std::cout << "x " << std::setw(2) << "=" << " r      "
              << std::setw(22) << "unsigned short int"
              << std::setw(17) << (x = r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x = r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    short int x = static_cast<short int>(3.14159);

    std::cout << "x " << std::setw(2) << "=" << " r      "
              << std::setw(22) << "short int"
              << std::setw(17) << (x = r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x = r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    unsigned char x = static_cast<unsigned char>(3.14159);

    std::cout << "x " << std::setw(2) << "=" << " r      "
              << std::setw(22) << "unsigned char"
              << std::setw(17) << (x = r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x = r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    signed char x = static_cast<signed char>(3.14159);

    std::cout << "x " << std::setw(2) << "=" << " r      "
              << std::setw(22) << "signed char"
              << std::setw(17) << (x = r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x = r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    char x = static_cast<char>(3.14159);

    std::cout << "x " << std::setw(2) << "=" << " r      "
              << std::setw(22) << "char"
              << std::setw(17) << (x = r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x = r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    double x = static_cast<double>(3.14159);

    std::cout << "x " << std::setw(2) << "=" << " r      "
              << std::setw(22) << "double"
              << std::setw(17) << (x = r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x = r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    float x = static_cast<float>(3.14159);

    std::cout << "x " << std::setw(2) << "=" << " r      "
              << std::setw(22) << "float"
              << std::setw(17) << (x = r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x = r);
    std::cout << "    " << debug_out() << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    long double x = static_cast<long double>(3.14159);

    std::cout << "x " << std::setw(2) << "=" << " r      "
              << std::setw(22) << "long double"
              << std::setw(17) << (x = r);
    debug_clear();
    std::cout << std::setw(20) << type2char(x = r);
    std::cout << "    " << debug_out() << std::endl;
  }

#endif  // MPFR_REAL_ENABLE_CONV_OPS

  // ////////////////////////////////////////////////////////////////////
  // conversion functions
  // ////////////////////////////////////////////////////////////////////

  {
    mpfr::real<1024> r = -1.41421;
    unsigned long int x = static_cast<unsigned long int>(3.14159);

    debug_clear();
    r.conv(x);
    std::string temp = debug_out();
    std::cout << "r.conv(x)   "
              << std::setw(22) << "unsigned long int"
              << std::setw(17) << mpfr::real<1024>(x)
              << std::setw(20) << "-"
              << "    " << temp << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    long int x = static_cast<long int>(3.14159);

    debug_clear();
    r.conv(x);
    std::string temp = debug_out();
    std::cout << "r.conv(x)   "
              << std::setw(22) << "long int"
              << std::setw(17) << mpfr::real<1024>(x)
              << std::setw(20) << "-"
              << "    " << temp << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    unsigned int x = static_cast<unsigned int>(3.14159);

    debug_clear();
    r.conv(x);
    std::string temp = debug_out();
    std::cout << "r.conv(x)   "
              << std::setw(22) << "unsigned int"
              << std::setw(17) << mpfr::real<1024>(x)
              << std::setw(20) << "-"
              << "    " << temp << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    int x = static_cast<int>(3.14159);

    debug_clear();
    r.conv(x);
    std::string temp = debug_out();
    std::cout << "r.conv(x)   "
              << std::setw(22) << "int"
              << std::setw(17) << mpfr::real<1024>(x)
              << std::setw(20) << "-"
              << "    " << temp << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    unsigned short int x = static_cast<unsigned short int>(3.14159);

    debug_clear();
    r.conv(x);
    std::string temp = debug_out();
    std::cout << "r.conv(x)   "
              << std::setw(22) << "unsigned short int"
              << std::setw(17) << mpfr::real<1024>(x)
              << std::setw(20) << "-"
              << "    " << temp << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    short int x = static_cast<short int>(3.14159);

    debug_clear();
    r.conv(x);
    std::string temp = debug_out();
    std::cout << "r.conv(x)   "
              << std::setw(22) << "short int"
              << std::setw(17) << mpfr::real<1024>(x)
              << std::setw(20) << "-"
              << "    " << temp << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    unsigned char x = static_cast<unsigned char>(3.14159);

    debug_clear();
    r.conv(x);
    std::string temp = debug_out();
    std::cout << "r.conv(x)   "
              << std::setw(22) << "unsigned char"
              << std::setw(17) << mpfr::real<1024>(x)
              << std::setw(20) << "-"
              << "    " << temp << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    signed char x = static_cast<signed char>(3.14159);

    debug_clear();
    r.conv(x);
    std::string temp = debug_out();
    std::cout << "r.conv(x)   "
              << std::setw(22) << "signed char"
              << std::setw(17) << mpfr::real<1024>(x)
              << std::setw(20) << "-"
              << "    " << temp << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    char x = static_cast<char>(3.14159);

    debug_clear();
    r.conv(x);
    std::string temp = debug_out();
    std::cout << "r.conv(x)   "
              << std::setw(22) << "char"
              << std::setw(17) << mpfr::real<1024>(x)
              << std::setw(20) << "-"
              << "    " << temp << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    double x = static_cast<double>(3.14159);

    debug_clear();
    r.conv(x);
    std::string temp = debug_out();
    std::cout << "r.conv(x)   "
              << std::setw(22) << "double"
              << std::setw(17) << mpfr::real<1024>(x)
              << std::setw(20) << "-"
              << "    " << temp << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    float x = static_cast<float>(3.14159);

    debug_clear();
    r.conv(x);
    std::string temp = debug_out();
    std::cout << "r.conv(x)   "
              << std::setw(22) << "float"
              << std::setw(17) << mpfr::real<1024>(x)
              << std::setw(20) << "-"
              << "    " << temp << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    long double x = static_cast<long double>(3.14159);

    debug_clear();
    r.conv(x);
    std::string temp = debug_out();
    std::cout << "r.conv(x)   "
              << std::setw(22) << "long double"
              << std::setw(17) << mpfr::real<1024>(x)
              << std::setw(20) << "-"
              << "    " << temp << std::endl;
  }

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    mpz_t x;
    my_mp_init(x, 3.14159);

    debug_clear();
    r.conv(x);
    std::string temp = debug_out();
    std::cout << "r.conv(x)   "
              << std::setw(22) << "mpz_t"
              << std::setw(17) << mpfr::real<1024>(x)
              << std::setw(20) << "-"
              << "    " << temp << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    mpf_t x;
    my_mp_init(x, 3.14159);

    debug_clear();
    r.conv(x);
    std::string temp = debug_out();
    std::cout << "r.conv(x)   "
              << std::setw(22) << "mpf_t"
              << std::setw(17) << mpfr::real<1024>(x)
              << std::setw(20) << "-"
              << "    " << temp << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

#ifndef _MSC_VER
  {
    mpfr::real<1024> r = -1.41421;
    mpfr_t x;
    my_mp_init(x, 3.14159);

    debug_clear();
    r.conv(x);
    std::string temp = debug_out();
    std::cout << "r.conv(x)   "
              << std::setw(22) << "mpfr_t"
              << std::setw(17) << mpfr::real<1024>(x)
              << std::setw(20) << "-"
              << "    " << temp << std::endl;

    my_mp_clear(x);
  }
#endif  // _MSC_VER

  {
    mpfr::real<1024> r = -1.41421;
    mpz_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpz_ptr x = x_nc;

    debug_clear();
    r.conv(x);
    std::string temp = debug_out();
    std::cout << "r.conv(x)   "
              << std::setw(22) << "mpz_ptr"
              << std::setw(17) << mpfr::real<1024>(x)
              << std::setw(20) << "-"
              << "    " << temp << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpf_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpf_ptr x = x_nc;

    debug_clear();
    r.conv(x);
    std::string temp = debug_out();
    std::cout << "r.conv(x)   "
              << std::setw(22) << "mpf_ptr"
              << std::setw(17) << mpfr::real<1024>(x)
              << std::setw(20) << "-"
              << "    " << temp << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr_t x_nc;
    my_mp_init(x_nc, 3.14159);
    mpfr_ptr x = x_nc;

    debug_clear();
    r.conv(x);
    std::string temp = debug_out();
    std::cout << "r.conv(x)   "
              << std::setw(22) << "mpfr_ptr"
              << std::setw(17) << mpfr::real<1024>(x)
              << std::setw(20) << "-"
              << "    " << temp << std::endl;

    my_mp_clear(x_nc);
  }

  {
    mpfr::real<1024> r = -1.41421;
    std::string x = "3.14159";

    debug_clear();
    r.conv(x);
    std::string temp = debug_out();
    std::cout << "r.conv(x)   "
              << std::setw(22) << "std::string"
              << std::setw(17) << mpfr::real<1024>(x)
              << std::setw(20) << "-"
              << "    " << temp << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    char x_nc[2048];
    char* x = x_nc;

    debug_clear();
    r.conv(x);
    std::string temp = debug_out();
    std::cout << "r.conv(x)   "
              << std::setw(22) << "char*"
              << std::setw(17) << mpfr::real<1024>(x)
              << std::setw(20) << "-"
              << "    " << temp << std::endl;
  }

  // ////////////////////////////////////////////////////////////////////
  // unary operators
  // ////////////////////////////////////////////////////////////////////

  {
    mpfr::real<1024> r = -1.41421;
    mpfr::real<1024> r1 = r;
    mpfr::real<1024> r2 = -r1;

    debug_clear();
    -r;
    std::string temp = debug_out();
    std::cout << std::setw(3) << "-r" << "         "
              << std::setw(22) << "-"
              << std::setw(17) << r2
              << std::setw(20) << type2char(-r)
              << "    " << temp << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr::real<1024> r1 = r;
    mpfr::real<1024> r2 = ++r1;

    debug_clear();
    ++r;
    std::string temp = debug_out();
    std::cout << std::setw(3) << "++r" << "         "
              << std::setw(22) << "-"
              << std::setw(17) << r2
              << std::setw(20) << type2char(++r)
              << "    " << temp << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr::real<1024> r1 = r;
    mpfr::real<1024> r2 = --r1;

    debug_clear();
    --r;
    std::string temp = debug_out();
    std::cout << std::setw(3) << "--r" << "         "
              << std::setw(22) << "-"
              << std::setw(17) << r2
              << std::setw(20) << type2char(--r)
              << "    " << temp << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr::real<1024> r1 = r;
    mpfr::real<1024> r2 = r1++;

    debug_clear();
    r++;
    std::string temp = debug_out();
    std::cout << std::setw(3) << "r++" << "         "
              << std::setw(22) << "-"
              << std::setw(17) << r2
              << std::setw(20) << type2char(r++)
              << "    " << temp << std::endl;
  }

  {
    mpfr::real<1024> r = -1.41421;
    mpfr::real<1024> r1 = r;
    mpfr::real<1024> r2 = r1--;

    debug_clear();
    r--;
    std::string temp = debug_out();
    std::cout << std::setw(3) << "r--" << "         "
              << std::setw(22) << "-"
              << std::setw(17) << r2
              << std::setw(20) << type2char(r--)
              << "    " << temp << std::endl;
  }

  return 0;
}
