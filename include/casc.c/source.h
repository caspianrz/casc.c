#pragma once

#ifndef CASC_SOURCE_H
# define CASC_SOURCE_H

# include <casc.c/config.h>
# include <casc.c/types.h>
# include <cashash.c/cashash.h>

typedef struct casc_source_location_s {
  casc_cu8_t *file_path;
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
  cashash_strategy_option_t option = {.used = false};
  context->source_file_map = cashash_create_open_addressing_with_strategy(
      64, CASHASH_HASH_STRATEGY_XXH3, option, CASHASH_OA_PROBE_LINEAR);

  return context;
}

casc_bool_t
casc_source_manager_insert_source(casc_source_manager_context_t *context,
                                  casc_cu8_t *path) {
  cashash_key_datum_t path_datum = {
      .data = path,
      .length = strlen(path),
  };
  return cashash_insert(context->source_file_map, path_datum, NULL);
}

casc_bool_t
casc_source_manager_remove_source(casc_source_manager_context_t *context,
                                  casc_cu8_t *path) {
  cashash_key_datum_t path_datum = {
      .data = path,
      .length = strlen(path),
  };
  return cashash_remove(context->source_file_map, path_datum);
}

casc_cbool_t
casc_source_manager_has_source_buffered(casc_source_manager_context_t *context,
                                        casc_cu8_t *path) {
  cashash_key_datum_t path_datum = {
      .data = path,
      .length = strlen(path),
  };
  void *result = cashash_find(context->source_file_map, path_datum);
  return result == NULL;
}

void casc_free_source_manager_context(casc_source_manager_context_t *context) {
  cashash_destroy(context->source_file_map);
  casc_free(context);
}

#endif