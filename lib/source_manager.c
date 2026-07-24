#include <casc.c/source.h>

static casc_sloc_entry_t *
casc_source_manager_alloc_entry(casc_source_manager_t *manager) {
  if (manager->sloc_count == manager->sloc_capacity) {

    casc_u32_t capacity =
        manager->sloc_capacity == 0 ? 8 : manager->sloc_capacity * 2;

    void *p =
        realloc(manager->sloc_entries, capacity * sizeof(casc_sloc_entry_t));

    if (p == NULL)
      return NULL;

    manager->sloc_entries = p;
    manager->sloc_capacity = capacity;
  }

  return &manager->sloc_entries[manager->sloc_count++];
}

static inline void casc_source_file_destroy(casc_source_file_t *file) {
  if (file == NULL)
    return;

  free(file->buffer);
  free(file->line_offsets);
  free((char *)file->path);

  file->buffer = NULL;
  file->line_offsets = NULL;
  file->path = NULL;
}

void casc_source_manager_init(casc_source_manager_t *manager) {
  if (manager == NULL)
    return;

  manager->sloc_entries = NULL;

  manager->sloc_count = 0;
  manager->sloc_capacity = 0;

  manager->next_offset = 1;
}

void casc_source_manager_destroy(casc_source_manager_t *manager) {
  if (manager == NULL)
    return;

  for (casc_u32_t i = 0; i < manager->sloc_count; ++i) {
    casc_sloc_entry_t *entry = &manager->sloc_entries[i];

    switch (entry->kind) {
    case CASC_SOURCE_KIND_FILE:
      casc_source_file_destroy(&entry->file);
      break;

    case CASC_SOURCE_KIND_MACRO_EXPANSION:
      break;
    }
  }

  free(manager->sloc_entries);

  manager->sloc_entries = NULL;
  manager->sloc_count = 0;
  manager->sloc_capacity = 0;
  manager->next_offset = 1;
}

casc_file_id_t casc_source_manager_create_file(casc_source_manager_t *manager,
                                               const char *path) {
  casc_file_id_t invalid = {0};

  casc_sloc_entry_t *entry = casc_source_manager_alloc_entry(manager);

  if (entry == NULL)
    return invalid;

  memset(entry, 0, sizeof(*entry));

  entry->kind = CASC_SOURCE_KIND_FILE;

  entry->base_offset = manager->next_offset;

  if (!casc_source_file_load(&entry->file, path))
    return invalid;

  manager->next_offset += entry->file.size;

  casc_file_id_t id = {.id = manager->sloc_count};

  entry->file.id = id;

  return id;
}

casc_file_id_t
casc_source_manager_create_memory_file(casc_source_manager_t *manager,
                                       const char *name, const char *buffer,
                                       casc_u32_t size) {
  casc_file_id_t invalid = {0};

  casc_sloc_entry_t *entry = casc_source_manager_alloc_entry(manager);

  if (entry == NULL)
    return invalid;

  memset(entry, 0, sizeof(*entry));

  entry->kind = CASC_SOURCE_KIND_FILE;
  entry->base_offset = manager->next_offset;

  casc_source_file_init_memory(&entry->file, name, buffer, size);

  manager->next_offset += size;

  casc_file_id_t id = {.id = manager->sloc_count};

  entry->file.id = id;

  return id;
}

casc_file_id_t casc_source_manager_create_macro_expansion(
    casc_source_manager_t *manager, casc_source_location_t spelling,
    casc_source_range_t expansion, casc_u32_t length) {
  casc_file_id_t invalid = {0};

  casc_sloc_entry_t *entry = casc_source_manager_alloc_entry(manager);

  if (entry == NULL)
    return invalid;

  memset(entry, 0, sizeof(*entry));

  entry->kind = CASC_SOURCE_KIND_MACRO_EXPANSION;

  entry->base_offset = manager->next_offset;

  entry->expansion.spelling = spelling;
  entry->expansion.expansion = expansion;
  entry->expansion.length = length;

  manager->next_offset += length;

  casc_file_id_t id = {.id = manager->sloc_count};

  return id;
}