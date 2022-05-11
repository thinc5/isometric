#include <stdbool.h>

#include "../jobs/move_actor.h"

#include "actor.h"

Actor create_minion(ActorPosition position, int args)
{
    Actor minion = {0};
    minion.type = MINION;
    minion.animation = ANIM_MINION_IDLE;
    minion.position = position;
    minion.on_click = (bool (*)(void *, ...)) & move_actor_job;
    return minion;
}
