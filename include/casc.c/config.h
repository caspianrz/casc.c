#pragma once

#ifndef CASC_CONF_H
# define CASC_CONF_H

# include <casc.c/symbols.h>

# define CASC_OS_WINDOWS 0
# define CASC_OS_MACOS 0
# define CASC_OS_LINUX 0
# define CASC_OS_BSD 0
# define CASC_OS_UNIX 0
# define CASC_OS_UNKNOWN 0

# if defined(_WIN32) || defined(_WIN64)
#  undef CASC_OS_WINDOWS
#  define CASC_OS_WINDOWS 1

#  ifndef NOMINMAX
#   define NOMINMAX
#  endif

#  ifndef WIN32_LEAN_AND_MEAN
#   define WIN32_LEAN_AND_MEAN
#   include <Windows.h>
#  endif
# elif defined(__APPLE__) && defined(__MACH__)
#  undef CASC_OS_MACOS
#  define CASC_OS_MACOS 1
# elif defined(__linux__)
#  undef CASC_OS_LINUX
#  define CASC_OS_LINUX 1
# elif defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__NetBSD__)
#  undef CASC_OS_BSD
#  define CASC_OS_BSD 1
# elif defined(__unix__) || defined(__unix)
#  undef CASC_OS_UNIX
#  define CASC_OS_UNIX 1
# else
#  undef CASC_OS_UNKNOWN
#  define CASC_OS_UNKNOWN 1
# endif

# define CASC_POSIX 0
# if defined(_POSIX_VERSION) || defined(__unix__) || defined(__unix)
#  undef CASC_POSIX
#  define CASC_POSIX 1
# endif

# if CASC_HAVE_STDIO_H
#  include <stdio.h>
#  define casc_fopen(__filename, __mode) fopen(__filename, __mode)
#  define casc_fread(ptr, size, nitems, stream) fread(ptr, size, nitems, stream)
#  define casc_fclose(__file) fclose(__file)
# endif

# if CASC_HAVE_UNISTD_H
#  include <unistd.h>
#  if CASC_HAVE_ACCESS
#   define casc_access(path, mode) access(path, mode)
#  endif
# endif

# if CASC_HAVE_SYS_STAT_H
#  include <sys/stat.h>
#  if CASC_HAVE_STAT
#   define casc_stat(path, buf) stat(path, buf)
#  endif
# endif

#endif