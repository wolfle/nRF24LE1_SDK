#ifndef _UARTRX_BITBANG_H_
#define _UARTRX_BITBANG_H_

typedef void (*uartcb_t)(unsigned char) __reentrant;

void uartrx_init(uartcb_t cb);

#endif
