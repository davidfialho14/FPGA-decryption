#ifndef HTIMER_H_
#define HTIMER_H_

#include "xtmrctr.h"

int init_timer (int cascade_mode);
void start_timer(int cascade_mode);
u32 get_timer_val(void);
u32 get_timer64_val(u32 *val0);
float conv_cycles_to_msecs (u32 cycles);
float conv2_cycles_to_msecs (u32 cycles1, u32 cycles0);

#endif /* HTIMER_H_ */
