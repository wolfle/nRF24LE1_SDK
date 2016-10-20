#include "interrupt.h"

void irq_config_ifp(irq_ifp_input_e irq_ifp_input, bool enable, bool edge)
{
	INTEXP &= ~(INTEXP_GP_INT0_ENABLE|INTEXP_GP_INT1_ENABLE|INTEXP_GP_INT2_ENABLE);
	//Set up TCON and T2CON register
	if(edge) {
		TCON |= TCON_IT0|TCON_IT1;
		T2CON |= T2CON_INT2_EDGE|T2CON_INT3_EDGE;
	}else{
		TCON &= ~(TCON_IT0|TCON_IT1);
		T2CON &= ~(T2CON_INT2_EDGE|T2CON_INT3_EDGE);
	}
	//Try to enable the pin interrupt
	if(enable)	INTEXP |= irq_ifp_input;
}

void irq_set_priority(irq_priority_group_e group, irq_priority_e level){
	uint8_t m=(1 << group);
	if(level & 0x1)	IP0 |= m;
	else IP0 &= ~m;

	if(level & 0x2)	IP1 |= m;
	else IP1 &= ~m;
}
