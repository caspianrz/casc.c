#include <stdio.h>
#include <stdlib.h>

#include <casc.c/source.h>
#include <casc.c/types.h>

#include <casc.c/keywords.h>

int casc_c_main(int argc, char *argv[]) {
  fprintf(stderr, "casc.c: is not implemented yet.\n");
  exit(EXIT_FAILURE);

  return EXIT_FAILURE;
}

int main(int argc, char *argv[]) {
  casc_source_manager_context_t *context = casc_init_source_manager_context();
  casc_free_source_manager_context(context);

  return casc_c_main(argc, argv);
}