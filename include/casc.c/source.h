#pragma once

#ifndef CASC_SOURCE_H
#define CASC_SOURCE_H

#include <stdlib.h>

typedef struct casc_source_location_s
{
    const size_t fileid;
    const size_t offset;
} casc_source_location_t;

typedef struct casc_source_range_s
{
    casc_source_location_t begin;
    casc_source_location_t end;
} casc_source_range_t;

typedef struct casc_source_buffer_s
{

} casc_source_buffer_t;

char casc_source_buffer_read_char(casc_source_buffer_t *buffer)
{
    return 0;
}

#endif