#pragma once

#ifndef CASC_SOURCE_H
# define CASC_SOURCE_H

# include <casc.c/config.h>
# include <casc.c/types.h>

# ifdef CAS_USE_XXHASH
#  define CASHASH_USE_XXHASH
# endif
# include <cashash.c/cashash.h>

typedef struct casc_source_location_s {
  casc_csize_t fileid;
  casc_csize_t offset;
} casc_source_location_t;

typedef struct casc_source_range_s {
  casc_source_location_t begin;
  casc_source_location_t end;
} casc_source_range_t;

typedef struct casc_source_manager_context_s {
  cashash_t *source_file_map;
} casc_source_manager_context_t;

casc_source_manager_context_t *casc_init_source_manager_context() {
  casc_source_manager_context_t *context =
      (casc_source_manager_context_t *)casc_malloc(
          sizeof(casc_source_manager_context_t));
  context->source_file_map = cashash_create_open_addressing_with_strategy(
      64, CASHASH_HASH_STRATEGY_XXH3);

  return context;
}

void casc_source_manager_insert_source(casc_source_manager_context_t *context,
                                       casc_cu8_t *path) {}

void casc_free_source_manager_context(casc_source_manager_context_t *context) {
  cashash_destroy(context->source_file_map);
  casc_free(context);
}

#endif