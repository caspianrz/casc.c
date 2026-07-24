#include <casc.c/preprocess.h>

struct casc_preprocessor_s {
  casc_source_manager_t *source_manager;
  casc_macro_table_t macros;
  //   casc_conditional_stack_t if_stack;
  //   casc_include_stack_t include_stack;
};

void casc_preprocessor_init(casc_preprocessor_t *pp,
                            casc_source_manager_t *source_manager) {
  memset(pp, 0, sizeof(*pp));
  pp->source_manager = source_manager;
  pp->macros = cashash_create(64);
}
