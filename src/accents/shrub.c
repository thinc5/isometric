#include "accent.h"

Accent create_shrub(void)
{
    return (Accent) {
        .type = SHRUB,
        .animation = ANIM_SHRUB
    };
}
