#ifndef _GP2Y1051_H_
#define _GP2Y1051_H_
#include <stdint.h>

typedef void (*gp2ycb_t)(uint16_t) __reentrant;

void gp2y_init(gp2ycb_t cb_);

#endif
