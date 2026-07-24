#pragma once

#ifndef CASC_SOURCE_H
# define CASC_SOURCE_H

# include <casc.c/error.h>
# include <casc.c/types.h>

# ifdef __cplusplus
extern "C" {
# endif

typedef enum casc_source_kind_e {
  CASC_SOURCE_KIND_FILE,
  CASC_SOURCE_KIND_MACRO_EXPANSION,
} casc_source_kind_t;

typedef enum {
  CASC_SOURCE_STORAGE_AUTO = 0,
  CASC_SOURCE_STORAGE_MALLOC,
  CASC_SOURCE_STORAGE_MMAP
} casc_source_storage_t;

typedef struct {
  casc_u32_t offset;
} casc_source_location_t;

typedef struct {
  casc_source_location_t begin;
  casc_source_location_t end;
} casc_source_range_t;

typedef casc_u32_t casc_file_id_t;

typedef struct casc_source_file_s {
  casc_file_id_t id;
  char *path;
  char *buffer;
  casc_size_t size;

  casc_source_storage_t storage;
  casc_bool_t loaded;

  casc_u32_t *line_offsets; // optional line table.
  casc_u32_t line_count;    // total lines of this file
} casc_source_file_t;

typedef struct casc_macro_expansion_s {
  casc_source_location_t spelling;
  casc_source_range_t expansion;
  casc_u32_t length;
} casc_macro_expansion_t;

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

casc_error_t casc_source_manager_init(casc_source_manager_t *manager);
casc_error_t casc_source_manager_destroy(casc_source_manager_t *manager);

# ifdef __cplusplus
}
# endif

#endif