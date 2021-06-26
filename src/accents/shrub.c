#include "accent.h"
#include "shrub.h"

Accent create_shrub(void)
{
    Accent shrub = {0};
    shrub.type = SHRUB;
    shrub.animation = ANIM_SHRUB;
    return shrub;
}
