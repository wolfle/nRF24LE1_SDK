#ifndef UART_INT_H
#define UART_INT_H

#include "interrupt.h"

/* This must be included from the file that has the main() function so the
 * interrupt function will be setup properly in the interrupt table.
 */
irq_isr_uart() __using(1);

/* Initialize the UART to 38400 8n1 and prepare the interrupt.
 * User must enable global interrupts before the UART can work. */
void uart_int_init(void);

/* Write a single char to the UART (or the to buffer if the UART is busy).
 * Blocks if the buffer is empty, the next UART interrupt will release the block. */
void putchar(char ch);

/* Get a character from the receive buffer, blocks until a character is available. */
char getchar(void);

/* Checks if there is data in the receive buffer, non-blocking. */
char uart_int_rx_pending(void);

/* Checks if there is data in the transmit buffer, non-blocking. */
char uart_int_tx_pending(void);

#endif
