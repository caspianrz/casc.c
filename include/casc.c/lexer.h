#pragma once

#ifndef CASC_LEXER_H
#define CASC_LEXER_H

#include <cashash.c/cashash.h>

#define CASC_C_KEYWORD_BUCKET_SIZE 64

static cashash_t *casc_c_keywords = cashash_create_open_addressing(CASC_C_KEYWORD_BUCKET_SIZE);

#endif