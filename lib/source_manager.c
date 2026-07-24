#include <casc.c/allocator.h>
#include <casc.c/source.h>

casc_error_t casc_source_manager_init(casc_source_manager_t *manager) {
  if (NULL == manager) {
    return CASC_ERROR_NULL_REFERENCE;
  }

  manager->sloc_entries = NULL;
  manager->sloc_count = 0;
  manager->sloc_capacity = 0;
  manager->next_offset = 1;

  return CASC_ERROR_NONE;
}

casc_error_t casc_source_manager_destroy(casc_source_manager_t *manager) {
  if (NULL == manager) {
    return CASC_ERROR_NULL_REFERENCE;
  }

  if (NULL != manager->sloc_entries) {
    casc_free(manager->sloc_entries);
    manager->sloc_entries = NULL;
  }

  manager->sloc_count = 0;
  manager->sloc_capacity = 0;
  manager->next_offset = 1;

  return CASC_ERROR_NONE;
}