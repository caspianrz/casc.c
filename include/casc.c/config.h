#pragma once

#ifndef CASC_CONFIG_H
# define CASC_CONFIG_H

# include <stdlib.h>

# define casc_calloc calloc;
# define casc_malloc malloc;
# define casc_realloc realloc;
# define casc_free free;

#endif