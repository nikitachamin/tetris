#include "fsm.h"



void sigact()
{
    switch (*state)
    {
        case START:
            start_game();
            break;
        case SPAWN:
            spawn_piece();
            break;
        case MOVING:
            on_moving_state(sig, state, map, frog_pos);
            break;
        case SHIFTING:
            on_shifting_state(state, stats, map, frog_pos);
            break;
        case REACH:
            on_reach_state(state, stats, map, frog_pos);
            break;
        case COLLIDE:
            on_collide_state(state, stats, frog_pos);
            break;
        case GAMEOVER:
            print_banner(stats);
            break;
        default:
            break;
    }
}
