#include <casc.c/allocator.h>
#include <casc.c/config.h>
#include <casc.c/source.h>
#include <casc.c/types.h>

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

  casc_file_id_t id = manager->sloc_count;

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

  casc_file_id_t id = manager->sloc_count;

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

  casc_file_id_t id = manager->sloc_count;

  return id;
}

static casc_error_t casc_source_file_load_malloc(casc_source_file_t *file) {
  if (NULL == file)
    return CASC_ERROR_FILE_NOT_FOUND;

  if (file->loaded)
    return CASC_ERROR_NONE;

  if (NULL == file->path || '\0' == file->path[0]) {
    return CASC_ERROR_FILE_OPEN_FAILED;
  }

  struct stat st;
  if (0 != casc_stat(file->path, &st)) {
    return CASC_ERROR_FILE_NOT_FOUND;
  }

  file->size = (casc_size_t)st.st_size;

  casc_file_pt file_handle = casc_fopen(file->path, O_RDONLY);
  if (-1 == file_handle) {
    return CASC_ERROR_FILE_OPEN_FAILED;
  }

  file->buffer = casc_malloc(file->size + 1);
  file->storage = CASC_SOURCE_STORAGE_MALLOC;

  if (NULL == file->buffer) {
    casc_fclose(file_handle);
    return CASC_ERROR_OUT_OF_MEMORY;
  }

  casc_size_t _n = casc_fread(file->buffer, 1, file->size, file_handle);
  if (_n != file->size) {
    casc_free(file->buffer);
    file->buffer = NULL;
    casc_fclose(file_handle);
    return CASC_ERROR_FILE_READ_FAILED;
  }
  ((char *)file->buffer)[file->size] = '\0';
  casc_fclose(file_handle);
  file->loaded = CASC_TRUE;
  return CASC_ERROR_NONE;
}

static casc_error_t casc_source_file_load_mmap(casc_source_file_t *file) {
#if CASC_OS_WINDOWS

#elif CASC_HAVE_MMAP
  int file_handle = casc_fopen(file->path, O_RDONLY);
#else
  return CASC_ERROR_INTERNAL;
#endif
}

casc_error_t casc_source_manager_load(casc_source_manager_t *manager,
                                      casc_file_id_t file_id,
                                      casc_source_storage_t mode) {
  casc_source_file_t *file;

  if (file_id >= manager->sloc_count)
    return CASC_ERROR_INVALID_FILE_ID;

  file = &manager->sloc_entries[file_id];

  if (file->loaded)
    return CASC_ERROR_NONE;

  switch (mode) {
  case CASC_SOURCE_STORAGE_MALLOC:
    return casc_source_file_load_malloc(file);

  case CASC_SOURCE_STORAGE_MMAP:
    return casc_source_file_load_mmap(file);

  case CASC_SOURCE_STORAGE_AUTO:
#if CASC_HAS_MEMORYMAP
    return casc_source_file_load_mmap(file);
#else
    return casc_source_file_load_malloc(file);
#endif

  default:
    return CASC_ERROR_INTERNAL;
  }
}