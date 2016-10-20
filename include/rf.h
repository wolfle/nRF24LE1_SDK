/////////////////////////////////////////////////////////////////////////////
//
// File: rf.h
//
// Copyright S. Brennen Ball, 2011
//
// The author provides no guarantees, warantees, or promises, implied or
//  otherwise.  By using this software you agree to indemnify the author
//  of any damages incurred by using it.
//
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
/////////////////////////////////////////////////////////////////////////////

#ifndef _RF_H_
#define _RF_H_

#include <stddef.h>
#include "reg24le1.h"
#include "pt.h"

#define PAYLOAD_SIZE 32


typedef enum {
	dr2m=1, dr1m=0, dr250k=4
} rf_date_rate_e;

typedef enum {
	n18dbm, n12dbm, n6dbm, n0dbm
} rf_pa_e;

typedef enum {
	aw3=1, aw4, aw5
} rf_address_width_e;

typedef enum {
	crc0, crc1, crc2
} rf_crc_e;

struct rf_params{
	__code const uint8_t * addr;
	uint8_t addrlen,channel,retrans;
	rf_date_rate_e dr;
	rf_pa_e pa;
};

//config: 0x08 : ptx, 1bit crc, power down, enable all interrupts; 6 mask rx_dr, 5 mask tx_ds, 4 mask max_rt, 3 en_crc, 2 en_2b_crc, 1 pwr_up, 0 prim_rx
//en_aa: 0x3f : enable auto ack for all pipes
//en_rxaddr: 0x03 : enable pipe0 and pipe1
//setup_aw: 0x03 : 5 bytes address width
//setup_retr: 0x03 : 250us 3 times. 250us * (x>>4 + 1) for waiting, x&0xf for retransmitting times
//rf_ch : 0x02 : 2--126 for 2m bps wide channel
//setup : 0x0f :  2m bps and 0dbm (highest) pa power. 7th bit for wave_cont; 4th bit for force pll lock; 5th bit date_rate low; 3rd bit date_rate high; 2:1 for pa power level
//status : 0x0e : 6 rx_dr, 5 tx_ds, 4 max_rt, 3:1 rx_p_n0 (111 for no rx data), 0 tx_full
//observ_tx : 0x00 : 7:4 plos_cnt, 3:0 arc_cnt
//rpd : 0x00 : 0 rpd, received power detector
//fifo_status: 0x11 : 6 tx_reuse, 5 tx_full, 4 tx_empty, 1 rx_full, 0 rx_empty
//dynpd : 0x00 : 5:0 enable dynamic payload length for 5:0 pipes
//feature : 0x00 : 2 enable dynamic payload lenght, 1 enable payload with ack, 0 enable dynamic ack (Enables the W_TX_PAYLOAD_NOACK command)



////////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////////

//void rf_power_up_param(uint8_t config);
//void rf_power_down_param(uint8_t config);
void rf_power_up();
void rf_power_down();
//void rf_set_data_rate_pa(rf_date_rate_e dr);
//void rf_set_auto_rtr(uint8_t cnt, uint16_t udelay);
//void rf_set_irq_mask_crc_rt(uint8_t mask, rf_crc_e crc, bool prx, bool powerup);// mask : 2:0 rx_dr|tx_ds|max_rt
//void rf_set_address_width(rf_address_width_e aw);

//set address for ptx mode
//void rf_set_tx_address(__code const uint8_t * addr,  uint8_t len); //also set address width by len
//set address for prx mode
//void rf_set_rx_address(__code const uint8_t * addr, uint8_t len, uint8_t pipe); //also set addr width if pipe==0
//void rf_enable_shockburst(rf_date_rate_e dr, rf_pa_e pa, uint8_t retrans, bool prx, bool powerup);
void rf_init(const struct rf_params *p,bool prx, bool powerup);
//uint8_t rf_write_noack_payload(const uint8_t * buf, uint8_t len);
//uint8_t rf_write_ack_payload(uint8_t pipe, const uint8_t * buf, uint8_t len);

void rf_set_channel(uint8_t channel);
uint8_t rf_observe_tx();
bool rf_rpd_active();

bool rf_incoming(void);
uint16_t rf_lost(void);
uint16_t rf_sent(void);
PT(rf_send,__pdata void * buf, uint8_t size);

#endif /*RF_H_*/
