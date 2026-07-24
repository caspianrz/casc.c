#pragma once

#ifndef CASC_PREPROCESS_H
# define CASC_PREPROCESS_H

# include <casc.c/source.h>

# include <cashash.c/cashash.h>

# ifdef __cplusplus
extern "C" {
# endif

typedef struct casc_preprocessor_s casc_preprocessor_t;
typedef cashash_t *casc_macro_table_t;

void casc_preprocessor_init(casc_preprocessor_t *pp,
                            casc_source_manager_t *source_manager);

#endif