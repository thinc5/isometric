#ifndef SERIALIZE_H
#define SERIALIZE_H

#include <stdbool.h>

#include "data.h"

bool serialize_map(World *world, const char *filename);

bool deserialize_map(World *world, const char *filename);

#endif
