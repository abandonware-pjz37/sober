#ifndef SOBER_CONFIG_HPP_
#define SOBER_CONFIG_HPP_

// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#if !defined(SOBER_DEBUG)
# if defined(SOBER_NDEBUG)
#  if (SOBER_NDEBUG)
#   define SOBER_DEBUG 0
#  else // SOBER_NDEBUG
#   define SOBER_DEBUG 1
#  endif // SOBER_NDEBUG
# elif defined(NDEBUG)
#  define SOBER_DEBUG 0
# else
#  define SOBER_DEBUG 1
# endif
#endif

#if !defined(SOBER_NDEBUG)
# if defined(SOBER_DEBUG)
#  if (SOBER_DEBUG)
#   define SOBER_NDEBUG 0
#  else // SOBER_DEBUG
#   define SOBER_NDEBUG 1
#  endif // SOBER_DEBUG
# elif defined(NDEBUG)
#  define SOBER_NDEBUG 1
# else
#  define SOBER_NDEBUG 0
# endif
#endif

#if ((SOBER_DEBUG) != 0) && ((SOBER_DEBUG) != 1)
# error "Please set SOBER_DEBUG to 0 or 1"
#endif

#if ((SOBER_NDEBUG) != 0) && ((SOBER_NDEBUG) != 1)
# error "Please set SOBER_NDEBUG to 0 or 1"
#endif

#if (SOBER_DEBUG) == (SOBER_NDEBUG)
# error "Both SOBER_DEBUG and SOBER_NDEBUG have equal values"
#endif

#endif // SOBER_CONFIG_HPP_
