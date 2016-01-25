#include "interrupt.h"
void irq_set_priority(irq_priority_group_e group, irq_priority_e level){
	if(level & 0x1)	IP0 |= (1 << group);
	else IP0 &= ~(1 << group);

	if(level & 0x2)	IP1 |= (1 << group);
	else IP1 &= ~(1 << group);
}
