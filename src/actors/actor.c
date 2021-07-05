#include "actor.h"
#include "minion.h"

/**
 * Initialization array for Actors.
 */
Actor (*create_actor[NUM_ACTORS])(ActorPosition, int, ...) = {
    [MINION] = (Actor (*)(ActorPosition, int, ...))&create_minion,
};
