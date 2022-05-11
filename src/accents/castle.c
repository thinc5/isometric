#include "accent.h"

Accent create_castle(void)
{
    return (Accent) {
        .type = CASTLE,
        .animation = ANIM_SHRUB
    };
}
