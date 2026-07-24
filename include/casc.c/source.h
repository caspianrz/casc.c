#pragma once

#ifndef CASC_SOURCE_H
# define CASC_SOURCE_H

# include <casc.c/types.h>

# ifdef __cplusplus
extern "C" {
# endif

typedef struct casc_source_manager_s casc_source_manager_t;

typedef struct {
  casc_u32_t offset;
} casc_source_location_t;

typedef struct {
  casc_source_location_t begin;
  casc_source_location_t end;
} casc_source_range_t;

typedef struct {
  casc_u32_t id;
} casc_file_id_t;

typedef struct casc_source_file_s {
  casc_file_id_t id;
  const char *path;
  const char *buffer;
  casc_u32_t size;
  casc_u32_t *line_offsets; // optional line table.
  casc_u32_t line_count;    // total lines of this file
} casc_source_file_t;

typedef struct casc_macro_expansion_s {
  casc_source_location_t spelling;
  casc_source_range_t expansion;
  casc_u32_t length;
} casc_macro_expansion_t;

typedef enum casc_source_kind_e {
  CASC_SOURCE_KIND_FILE,
  CASC_SOURCE_KIND_MACRO_EXPANSION,
} casc_source_kind_t;

typedef struct casc_sloc_entry_s {
  casc_source_kind_t kind;
  union {
    casc_source_file_t file;          // Normal source file
    casc_macro_expansion_t expansion; // Result of macro expansion.
  };
  casc_u32_t base_offset;
} casc_sloc_entry_t;

typedef struct casc_source_manager_s {
  casc_sloc_entry_t *sloc_entries;
  casc_u32_t sloc_count;
  casc_u32_t sloc_capacity;
  casc_u32_t next_offset;
} casc_source_manager_t;

// Functions

void casc_source_manager_init(casc_source_manager_t *manager);
void casc_source_manager_destroy(casc_source_manager_t *manager);

casc_file_id_t casc_source_manager_create_file(casc_source_manager_t *manager,
                                               const char *path);
casc_file_id_t
casc_source_manager_create_memory_file(casc_source_manager_t *manager,
                                       const char *name, const char *buffer,
                                       casc_u32_t size);

casc_file_id_t casc_source_manager_create_macro_expansion(
    casc_source_manager_t *manager, casc_source_location_t spelling,
    casc_source_range_t expansion, casc_u32_t length);

const casc_sloc_entry_t *
casc_source_manager_get_entry(const casc_source_manager_t *manager,
                              casc_file_id_t id);

casc_sloc_entry_t *
casc_source_manager_get_entry_mut(casc_source_manager_t *manager,
                                  casc_file_id_t id);

const casc_source_file_t *
casc_source_manager_get_file(const casc_source_manager_t *manager,
                             casc_file_id_t id);

casc_source_file_t *
casc_source_manager_get_file_mut(casc_source_manager_t *manager,
                                 casc_file_id_t id);

const casc_macro_expansion_t *
casc_source_manager_get_expansion(const casc_source_manager_t *manager,
                                  casc_file_id_t id);

casc_macro_expansion_t *
casc_source_manager_get_expansion_mut(casc_source_manager_t *manager,
                                      casc_file_id_t id);

casc_source_location_t
casc_source_manager_get_location(const casc_source_manager_t *manager,
                                 casc_file_id_t file, casc_u32_t file_offset);

casc_file_id_t
casc_source_manager_get_file_id(const casc_source_manager_t *manager,
                                casc_source_location_t loc);

casc_u32_t
casc_source_manager_get_file_offset(const casc_source_manager_t *manager,
                                    casc_source_location_t loc);

const char *casc_source_manager_get_buffer(const casc_source_manager_t *manager,
                                           casc_file_id_t file);

casc_u32_t
casc_source_manager_get_buffer_size(const casc_source_manager_t *manager,
                                    casc_file_id_t file);

const char *
casc_source_manager_get_filename(const casc_source_manager_t *manager,
                                 casc_source_location_t loc);

const char *casc_source_manager_get_path(const casc_source_manager_t *manager,
                                         casc_file_id_t file);

casc_u32_t casc_source_manager_get_line(const casc_source_manager_t *manager,
                                        casc_source_location_t loc);

casc_u32_t casc_source_manager_get_column(const casc_source_manager_t *manager,
                                          casc_source_location_t loc);

casc_source_location_t casc_source_manager_advance(casc_source_location_t loc,
                                                   casc_u32_t amount);

casc_source_location_t casc_source_manager_retreat(casc_source_location_t loc,
                                                   casc_u32_t amount);

bool casc_source_manager_is_macro(const casc_source_manager_t *manager,
                                  casc_source_location_t loc);

casc_source_location_t
casc_source_manager_get_spelling_location(const casc_source_manager_t *manager,
                                          casc_source_location_t loc);

casc_source_location_t
casc_source_manager_get_expansion_location(const casc_source_manager_t *manager,
                                           casc_source_location_t loc);

casc_source_range_t
casc_source_manager_get_expansion_range(const casc_source_manager_t *manager,
                                        casc_source_location_t loc);

# ifdef __cplusplus
}
# endif

#endif