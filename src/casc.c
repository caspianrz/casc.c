#include <stdio.h>
#include <stdlib.h>

#include <casc.c/source.h>
#include <casc.c/types.h>

#include <casc.c/keywords.h>

static casc_source_manager_t source_manager;

static void on_exit_defer() { casc_source_manager_destroy(&source_manager); }

int casc_c_main(int argc, char *argv[]) {
  casc_source_manager_init(&source_manager);
  on_exit_defer();
  fprintf(stderr, "casc.c: is not implemented yet.\n");
  exit(EXIT_FAILURE);

  return EXIT_FAILURE;
}

int main(int argc, char *argv[]) { return casc_c_main(argc, argv); }