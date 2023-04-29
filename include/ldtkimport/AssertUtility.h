#ifndef LDTK_IMPORT_ASSERT_UTILITY_H
#define LDTK_IMPORT_ASSERT_UTILITY_H

#include <sstream>
#include <ostream>
#include <iostream>


namespace ldtkimport
{

// should you use assert, or exception?
// there are no hard and fast rules, but:
//
// asserts - Mistakes in your own code, you want to assert that a certain condition is true.
//
// exceptions - Used when a function enocunters an irrecoverable error outside its control:
//              hard drive is full, memory allocation failed (could be that ram usage is full), etc.
//              Probably, the code outside that function can recover from it, but not that function
//              itself. That's why you can catch from all the way up in main if you wanted to.
//
// https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2271.html#Appendix_17
//
// The ASSERT_THROW macro below is still considered an assert, it's just that the error message is
// packaged as an exception, to make it easier to verify, using the unit testing library (catch2).
//
// these macros are only enabled if NDEBUG is not defined
//
//
//
// note: C++20 has std::source_location
// g++ has the non-standard __PRETTY_FUNCTION__

#if !defined(NDEBUG) && defined(LDTK_IMPORT_ASSERTS)
#  define ASSERT(condition, message) \
   do { \
      if (! (condition)) { \
         std::cerr << "Assertion `" #condition "` failed in " << __FILE__ \
                  << ":" << __LINE__ << " (" << __func__ << "): " << message << std::endl; \
         std::terminate(); \
      } \
   } while (false)
#else
#  define ASSERT(condition, message) ((void)0)
#endif

#if !defined(NDEBUG) && defined(LDTK_IMPORT_ASSERTS)
#  define ASSERT_THROW(condition, objectThrown, message) \
   do { \
      if (! (condition)) { \
         throw objectThrown((std::ostringstream() << "Assertion `" #condition "` failed in " << __FILE__ << ":" << __LINE__ << " (" << __func__ << "):\n" << message).str()); \
      } \
   } while (false)
#else
#  define ASSERT_THROW(condition, objectThrown, message) ((void)0)
#endif

} // namespace ldtkimport

#endif // LDTK_IMPORT_ASSERT_UTILITY_H
