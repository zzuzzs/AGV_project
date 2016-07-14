#ifndef __AGV_H
#define __AGV_H
#include "public.h"

#include "board.h"

void AGV_control(void);
void AGV_run(void);
void AGV_stop(void);
void AGV_V_set(float speed);
void AGV_state_init(void);
#endif //__AGV_H

