#pragma once

#ifndef CASC_ALLOCATOR_H
# define CASC_ALLOCATOR_H

# include <casc.c/symbols.h>

# ifndef casc_malloc
#  if CASC_HAVE_MALLOC
#   define casc_malloc(size) malloc(size)
#  else
#   error "malloc() is required"
#  endif
# endif

# ifndef casc_calloc
#  if CASC_HAVE_CALLOC
#   define casc_calloc(count, size) calloc((count), (size))
#  else
#   error "calloc() is required"
#  endif
# endif

# ifndef casc_realloc
#  if CASC_HAVE_REALLOC
#   define casc_realloc(ptr, size) realloc((ptr), (size))
#  else
#   error "realloc() is required"
#  endif
# endif

# ifndef casc_free
#  if CASC_HAVE_FREE
#   define casc_free(ptr) free(ptr)
#  else
#   error "free() is required"
#  endif
# endif

#endif