#include "uart_int.h"

#include "gpio.h"
#include "uart.h"
#include "interrupt.h"

/* The buffer definitions are sane defaults, you may want to change them and
 * rebuild the library if you need different values.
 */

#ifndef XBUFLEN
#define XBUFLEN 32
#endif

#ifndef RBUFLEN
#define RBUFLEN 16
#endif

/* You might want to specify idata, pdata or idata for the buffers */
static unsigned char __pdata rbuf[RBUFLEN], xbuf[XBUFLEN];
static volatile unsigned char rcnt, xcnt, rpos, xpos;
static __bit busy;

void uart_int_init(void)
{
	rcnt = xcnt = rpos = xpos = 0;  /* init buffers */
	busy = 0;

	// Setup UART pins
	gpio_pin_configure(GPIO_PIN_ID_FUNC_RXD,
			GPIO_PIN_CONFIG_OPTION_DIR_INPUT |
			GPIO_PIN_CONFIG_OPTION_PIN_MODE_INPUT_BUFFER_ON_NO_RESISTORS);

	gpio_pin_configure(GPIO_PIN_ID_FUNC_TXD,
			GPIO_PIN_CONFIG_OPTION_DIR_OUTPUT |
			GPIO_PIN_CONFIG_OPTION_OUTPUT_VAL_SET |
			GPIO_PIN_CONFIG_OPTION_PIN_MODE_OUTPUT_BUFFER_NORMAL_DRIVE_STRENGTH);

	uart_configure_8_n_1_38400();
	irq_uart_enable();
	// The user needs to enable global interrupts for this to work
}

irq_isr_uart() __using(1)
{
	if (S0CON_SB_RI0) {
		S0CON_SB_RI0 = 0;
		/* don't overwrite chars already in buffer */
		if (rcnt != RBUFLEN) {
			rbuf [(unsigned char)(rpos+rcnt) % RBUFLEN] = S0BUF;
			rcnt++;
		}
	}
	if (S0CON_SB_TI0) {
		S0CON_SB_TI0 = 0;
		if (xcnt > 0) {
			xcnt--;
			S0BUF = xbuf [xpos];
			xpos = (unsigned char)(xpos+1) % XBUFLEN;
		} else {
			busy = 0;
		}
	}
}

void putchar(char c)
{
	while (xcnt >= XBUFLEN) /* wait for room in buffer */
		;
	irq_uart_disable();
	if (busy) {
		xbuf[(unsigned char)(xpos+xcnt++) % XBUFLEN] = c;
	} else {
		S0BUF = c;
		busy = 1;
	}
	irq_uart_enable();
}

char getchar(void)
{
	unsigned char c;
	while (!rcnt)   /* wait for character */
		;
	irq_uart_disable();
	rcnt--;
	c = rbuf [rpos++];
	if (rpos >= RBUFLEN)
		rpos = 0;
	irq_uart_enable();
	return (c);
}

char uart_int_rx_pending(void)
{
	return rcnt;
}

char uart_int_tx_pending(void)
{
	if (busy)
		return 1;
	return xcnt;
}
