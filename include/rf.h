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
////////////////////////////////////////////////////////////////////////////////////
// SPI commands
////////////////////////////////////////////////////////////////////////////////////
//SPI command defines
#define RF_R_REGISTER			0x00	//Read register command
#define RF_W_REGISTER			0x20	//Write register command
#define RF_R_RX_PAYLOAD			0x61	//Read RX payload command
#define RF_W_TX_PAYLOAD			0xA0	//Write TX payload command
#define RF_FLUSH_TX				0xE1	//Flush TX FIFO command
#define RF_FLUSH_RX				0xE2	//Flush RX FIFO command
#define RF_REUSE_TX_PL			0xE3	//Reuse last TX payload command
#define RF_R_RX_PL_WID			0x60	//Read RX payload width command
#define RF_W_ACK_PAYLOAD		0xA8	//Write acknowledge payload command
#define RF_W_TX_PAYLOAD_NOACK	0xB0	//Read RX payload width command
#define RF_NOP					0xFF	//No operation command

//SPI command data mask defines
#define RF_R_REGISTER_DATA		0x1F	//Mask for read register command data
#define RF_W_REGISTER_DATA		0x1F	//Mask for write register command data
#define RF_W_ACK_PAYLOAD_PIPE	0x07	//Mask for write ACK payload command data


////////////////////////////////////////////////////////////////////////////////////
// Register definitions
////////////////////////////////////////////////////////////////////////////////////
#define RF_CONFIG				0x00
#define RF_EN_AA				0x01
#define RF_EN_RXADDR				0x02
#define RF_SETUP_AW				0x03
#define RF_SETUP_RETR				0x04
#define RF_RF_CH				0x05
#define RF_RF_SETUP				0x06
#define RF_STATUS				0x07
#define RF_OBSERVE_TX				0x08
#define RF_RPD					0x09
#define RF_RX_ADDR_P0				0x0A
#define RF_RX_ADDR_P1				0x0B
#define RF_RX_ADDR_P2				0x0C
#define RF_RX_ADDR_P3				0x0D
#define RF_RX_ADDR_P4				0x0E
#define RF_RX_ADDR_P5				0x0F
#define RF_TX_ADDR				0x10
#define RF_RX_PW_P0				0x11
#define RF_RX_PW_P1				0x12
#define RF_RX_PW_P2				0x13
#define RF_RX_PW_P3				0x14
#define RF_RX_PW_P4				0x15
#define RF_RX_PW_P5				0x16
#define RF_FIFO_STATUS				0x17
#define RF_DYNPD				0x1C
#define RF_FEATURE				0x1D


////////////////////////////////////////////////////////////////////////////////////
// Default register values
//
// Below are the defines for each register's default value in the RF section. Multi-byte
//   registers use notation _BX_, "X" is the byte number.
////////////////////////////////////////////////////////////////////////////////////
#define RF_CONFIG_DEFAULT_VAL			0x08
#define RF_EN_AA_DEFAULT_VAL			0x3F
#define RF_EN_RXADDR_DEFAULT_VAL		0x03
#define RF_SETUP_AW_DEFAULT_VAL			0x03
#define RF_SETUP_RETR_DEFAULT_VAL		0x03
#define RF_RF_CH_DEFAULT_VAL			0x02
#define RF_RF_SETUP_DEFAULT_VAL			0x0F
#define RF_STATUS_DEFAULT_VAL			0x0E
#define RF_OBSERVE_TX_DEFAULT_VAL		0x00
#define RF_RPD_DEFAULT_VAL			0x00
#define RF_RX_ADDR_P0_B0_DEFAULT_VAL		0xE7
#define RF_RX_ADDR_P0_B1_DEFAULT_VAL		0xE7
#define RF_RX_ADDR_P0_B2_DEFAULT_VAL		0xE7
#define RF_RX_ADDR_P0_B3_DEFAULT_VAL		0xE7
#define RF_RX_ADDR_P0_B4_DEFAULT_VAL		0xE7
#define RF_RX_ADDR_P1_B0_DEFAULT_VAL		0xC2
#define RF_RX_ADDR_P1_B1_DEFAULT_VAL		0xC2
#define RF_RX_ADDR_P1_B2_DEFAULT_VAL		0xC2
#define RF_RX_ADDR_P1_B3_DEFAULT_VAL		0xC2
#define RF_RX_ADDR_P1_B4_DEFAULT_VAL		0xC2
#define RF_RX_ADDR_P2_DEFAULT_VAL		0xC3
#define RF_RX_ADDR_P3_DEFAULT_VAL		0xC4
#define RF_RX_ADDR_P4_DEFAULT_VAL		0xC5
#define RF_RX_ADDR_P5_DEFAULT_VAL		0xC6
#define RF_TX_ADDR_B0_DEFAULT_VAL		0xE7
#define RF_TX_ADDR_B1_DEFAULT_VAL		0xE7
#define RF_TX_ADDR_B2_DEFAULT_VAL		0xE7
#define RF_TX_ADDR_B3_DEFAULT_VAL		0xE7
#define RF_TX_ADDR_B4_DEFAULT_VAL		0xE7
#define RF_RX_PW_P0_DEFAULT_VAL			0x00
#define RF_RX_PW_P1_DEFAULT_VAL			0x00
#define RF_RX_PW_P2_DEFAULT_VAL			0x00
#define RF_RX_PW_P3_DEFAULT_VAL			0x00
#define RF_RX_PW_P4_DEFAULT_VAL			0x00
#define RF_RX_PW_P5_DEFAULT_VAL			0x00
#define RF_FIFO_STATUS_DEFAULT_VAL		0x11
#define RF_DYNPD_DEFAULT_VAL			0x00
#define RF_FEATURE_DEFAULT_VAL			0x00


////////////////////////////////////////////////////////////////////////////////////
// Register bitwise definitions
////////////////////////////////////////////////////////////////////////////////////
//CONFIG register bitwise definitions
#define RF_CONFIG_RESERVED			0x80
#define	RF_CONFIG_MASK_RX_DR			0x40
#define	RF_CONFIG_MASK_TX_DS			0x20
#define	RF_CONFIG_MASK_MAX_RT			0x10
#define	RF_CONFIG_EN_CRC			0x08
#define	RF_CONFIG_CRCO				0x04
#define	RF_CONFIG_PWR_UP			0x02
#define	RF_CONFIG_PRIM_RX			0x01

//EN_AA register bitwise definitions
#define RF_EN_AA_RESERVED			0xC0
#define RF_EN_AA_ENAA_ALL			0x3F
#define RF_EN_AA_ENAA_P5			0x20
#define RF_EN_AA_ENAA_P4			0x10
#define RF_EN_AA_ENAA_P3			0x08
#define RF_EN_AA_ENAA_P2			0x04
#define RF_EN_AA_ENAA_P1			0x02
#define RF_EN_AA_ENAA_P0			0x01
#define RF_EN_AA_ENAA_NONE			0x00

//EN_RXADDR register bitwise definitions
#define RF_EN_RXADDR_RESERVED			0xC0
#define RF_EN_RXADDR_ERX_ALL			0x3F
#define RF_EN_RXADDR_ERX_P5			0x20
#define RF_EN_RXADDR_ERX_P4			0x10
#define RF_EN_RXADDR_ERX_P3			0x08
#define RF_EN_RXADDR_ERX_P2			0x04
#define RF_EN_RXADDR_ERX_P1			0x02
#define RF_EN_RXADDR_ERX_P0			0x01
#define RF_EN_RXADDR_ERX_NONE			0x00

//SETUP_AW register bitwise definitions
#define RF_SETUP_AW_RESERVED			0xFC
#define RF_SETUP_AW				0x03
#define RF_SETUP_AW_5BYTES			0x03
#define RF_SETUP_AW_4BYTES			0x02
#define RF_SETUP_AW_3BYTES			0x01
#define RF_SETUP_AW_ILLEGAL			0x00

//SETUP_RETR register bitwise definitions
#define RF_SETUP_RETR_ARD			0xF0
#define RF_SETUP_RETR_ARC			0x0F
#define RF_SETUP_RETR_DISABLE			0x00

//RF_CH register bitwise definitions
#define RF_RF_CH_RESERVED			0x80
#define RF_RF_CH_MAX_CHAN_NUM			 127

//RF_SETUP register bitwise definitions
#define RF_RF_SETUP_CONT_WAVE			0x80
#define RF_RF_SETUP_RESERVED			0x40
#define RF_RF_SETUP_RF_DR			0x28
#define RF_RF_SETUP_RF_DR_LOW			0x20
#define RF_RF_SETUP_PLL_LOCK			0x10
#define RF_RF_SETUP_RF_DR_HIGH			0x08
#define RF_RF_SETUP_RF_PWR			0x06
#define RF_RF_SETUP_RF_PWR_0_DBM		0x06
#define RF_RF_SETUP_RF_PWR_NEG_6_DBM 		0x04
#define RF_RF_SETUP_RF_PWR_NEG_12_DBM		0x02
#define RF_RF_SETUP_RF_PWR_NEG_18_DBM		0x00
//The following can be used in lieu of RF_RF_SETUP_RF_DR_LOW and RF_RF_SETUP_RF_DR_HIGH to set the data rate
#define RF_RF_SETUP_RF_DR_2_MBPS		RF_RF_SETUP_RF_DR_HIGH
#define RF_RF_SETUP_RF_DR_1_MBPS		0
#define RF_RF_SETUP_RF_DR_250_KBPS		RF_RF_SETUP_RF_DR_LOW

//STATUS register bitwise definitions
#define RF_STATUS_RESERVED			0x80
#define RF_STATUS_IRQ_MASKS			(RF_STATUS_RX_DR | RF_STATUS_TX_DS | RF_STATUS_MAX_RT)
#define RF_STATUS_RX_DR				0x40
#define RF_STATUS_TX_DS				0x20
#define RF_STATUS_MAX_RT			0x10
#define RF_STATUS_RX_P_NO			0x0E
#define RF_STATUS_RX_P_NO_RX_FIFO_EMPTY		0x0E
#define RF_STATUS_RX_P_NO_UNUSED		0x0C
#define RF_STATUS_RX_P_NO_5			0x0A
#define RF_STATUS_RX_P_NO_4			0x08
#define RF_STATUS_RX_P_NO_3			0x06
#define RF_STATUS_RX_P_NO_2			0x04
#define RF_STATUS_RX_P_NO_1			0x02
#define RF_STATUS_RX_P_NO_0			0x00
#define RF_STATUS_TX_FULL			0x01

//OBSERVE_TX register bitwise definitions
#define RF_OBSERVE_TX_PLOS_CNT			0xF0
#define RF_OBSERVE_TX_ARC_CNT			0x0F

//RPD register bitwise definitions
#define RF_RPD_RESERVED				0xFE
#define RF_RPD_RPD				0x01

//RX_PW_P0 register bitwise definitions
#define RF_RX_PW_P0_RESERVED			0xC0

//RX_PW_P1 register bitwise definitions
#define RF_RX_PW_P1_RESERVED			0xC0

//RX_PW_P2 register bitwise definitions
#define RF_RX_PW_P2_RESERVED			0xC0

//RX_PW_P3 register bitwise definitions
#define RF_RX_PW_P3_RESERVED			0xC0

//RX_PW_P4 register bitwise definitions
#define RF_RX_PW_P4_RESERVED			0xC0

//RX_PW_P5 register bitwise definitions
#define RF_RX_PW_P5_RESERVED			0xC0

//FIFO_STATUS register bitwise definitions
#define RF_FIFO_STATUS_RESERVED		0x8C
#define RF_FIFO_STATUS_TX_REUSE		0x40
#define RF_FIFO_STATUS_TX_FULL		0x20
#define RF_FIFO_STATUS_TX_EMPTY		0x10
#define RF_FIFO_STATUS_RX_FULL		0x02
#define RF_FIFO_STATUS_RX_EMPTY		0x01

//DYNPD register bitwise definitions
#define RF_DYNPD_RESERVED	0xC0
#define RF_DYNPD_DPL_ALL	0x3F
#define RF_DYNPD_DPL_P5		0x20
#define RF_DYNPD_DPL_P4		0x10
#define RF_DYNPD_DPL_P3		0x08
#define RF_DYNPD_DPL_P2		0x04
#define RF_DYNPD_DPL_P1		0x02
#define RF_DYNPD_DPL_P0		0x01
#define RF_DYNPD_DPL_NONE	0x00

//FEATURE register bitwise definitions
#define RF_FEATURE_RESERVED		0xF8
#define RF_FEATURE_ALL			0x07
#define RF_FEATURE_EN_DPL		0x04
#define RF_FEATURE_EN_ACK_PAY	0x02
#define RF_FEATURE_EN_DYN_ACK	0x01
#define RF_FEATURE_NONE			0x00


////////////////////////////////////////////////////////////////////////////////////
// macros
////////////////////////////////////////////////////////////////////////////////////
#define rf_clear_ce()	sbit_clear(RFCON_SB_RFCE)	//Clears the CE pin
#define rf_set_ce()		sbit_set(RFCON_SB_RFCE)
#define rf_set_ce_d()	rf_set_ce(); delay_us(4)	//Sets the CE pin, waits 4 us until we can use SPI again
#define rf_get_ce()		RFCON_SB_RFCE				//Gets the value of the CE pin
#define rf_get_csn()	RFCON_SB_RFCSN				//Gets the value of the CSN pin
#define rf_get_cken()	RFCON_SB_RFCKEN				//Gets the value of the CKEN pin
#define rf_set_cken()	sbit_set(RFCON_SB_RFCKEN)
#define rf_clear_cken()	sbit_clear(RFCON_SB_RFCKEN)

#define rf_nop()				rf_spi_cmd_zero(RF_NOP)			//nop command wrapper
#define rf_flush_tx()			rf_spi_cmd_zero(RF_FLUSH_TX)		//flush tx command wrapper
#define rf_flush_rx()			rf_spi_cmd_zero(RF_FLUSH_RX)		//flush rx command wrapper
#define rf_reuse_tx_pl()		rf_spi_cmd_zero(RF_REUSE_TX_PL)	//reuse tx payload command wrapper
#define rf_get_status()			rf_nop()												//get status register (wrapper of nop function)

#define rf_irq_flag				(IRCON_SB_RFIRQ)									//true if the IRQ pin from the 24L01 is active
#define rf_get_irq_flags()		(rf_get_status() & (RF_STATUS_RX_DR|RF_STATUS_TX_DSRF_STATUS_MAX_RT))

#define rf_get_pipe_num(status_val)					(((status_val) & RF_STATUS_RX_P_NO)>>1)											//return the pipe that is active in the STATUS register value
#define rf_is_rxed_packet_on_pipe(status_val, pipe)	 ((rf_get_pipe_num(status_val) == (pipe_val)) ? true : false)				//true if the supplied pipe define is equal to the pipe number
#define rf_is_rx_fifo_empty(status_val)				rf_is_rxed_packet_on_pipe(status_val, RF_FIFO_STATUS_RX_EMPTY)		//true if the RX FIFO is empty in STATUS register value
#define rf_is_rx_fifo_full(status_val)				rf_is_rxed_packet_on_pipe(status_val, RF_FIFO_STATUS_RX_FULL)		//true if the RX FIFO is empty in STATUS register value
#define rf_is_tx_ds_active(status_val)				((status_val) & RF_STATUS_TX_DS)												//true if TX_DS bit is active in STATUS register value
#define rf_is_rx_dr_active(status_val)				((status_val) & RF_STATUS_RX_DR)												//true if RX_DR bit is active in STATUS register value
#define rf_is_max_rt_active(status_val)				((status_val) & RF_STATUS_MAX_RT)												//true if MAX_RT bit is active in STATUS register value
#define rf_is_tx_fifo_empty(status_val)				((status_val) & RF_FIFO_STATUS_TX_EMPTY)												//true if TX_FULL bit is active in STATUS regiser value
#define rf_is_tx_fifo_full(status_val)				((status_val) & RF_FIFO_STATUS_TX_FULL)												//true if TX_FULL bit is active in STATUS regiser value

#define rf_is_config_rx()			((rf_read_register_one(RF_CONFIG) & RF_CONFIG_PRIM_RX) ? 1 : 0)
#define rf_is_config_tx()			(!rf_is_config_rx())
#define rf_is_config_powered_up		((rf_read_register_one(RF_CONFIG) & RF_CONFIG_PWR_UP) ? 1 : 0)
#define rf_is_config_powered_down	(!rf_is_config_powered_up())

#define CE_H rf_set_ce()
#define CE_L rf_clear_ce()
#define CE_H_D rf_set_ce_d()
#define CE_P rf_tranmit_one()

//The errata says you need to first write RFCON with 0x02, then you can enable the SPI pins
#define rf_clock_enable() {	RFCON = 0x02; rf_set_cken(); }

#define rf_clock_disable()	rf_clear_cken()

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

typedef void(*rf_rcb_t)(uint8_t *buf, uint8_t len); //callback when received a payload
typedef void(*rf_scb_t)(uint8_t fail);	//callback when sent a payload, >0 means failed

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

uint8_t rf_spi_cmd_zero(uint8_t cmd);
uint8_t rf_read_register_one(uint8_t reg);
uint8_t rf_write_register_one(uint8_t reg, uint8_t val);

//void rf_power_up_param(uint8_t config);
//void rf_power_down_param(uint8_t config);
void rf_power_up();
void rf_power_down();
//void rf_set_data_rate_pa(rf_date_rate_e dr);
//void rf_set_auto_rtr(uint8_t cnt, uint16_t udelay);
//void rf_set_irq_mask_crc_rt(uint8_t mask, rf_crc_e crc, bool prx, bool powerup);// mask : 2:0 rx_dr|tx_ds|max_rt
//void rf_set_address_width(rf_address_width_e aw);

//set address for ptx mode
void rf_set_tx_address(const uint8_t * addr, uint8_t len); //also set address width by len
//set address for prx mode
void rf_set_rx_address(const uint8_t * addr, uint8_t len, uint8_t pipe); //also set addr width if pipe==0
void rf_enable_shockburst(rf_date_rate_e dr, rf_pa_e pa, uint8_t retrans, bool prx, bool powerup);

uint8_t rf_read_rx_payload(uint8_t * buf, uint8_t len, uint8_t ** end); // if payload length > len, end will be NULL; return reg STATUS, the pipe of the payload can be determined from it  
uint8_t rf_write_payload(const uint8_t * buf, uint8_t len);

//void rf_set_rx_payload_width(uint8_t pipe, uint8_t pwidth);
//void rf_set_as_rx();
//void rf_set_as_tx();
//void rf_transmit_one();
uint8_t rf_write_noack_payload(const uint8_t * buf, uint8_t len);
uint8_t rf_write_ack_payload(uint8_t pipe, const uint8_t * buf, uint8_t len);
//uint8_t rf_write_payload(rf_payload_e type, const uint8_t * buf, uint8_t len);

inline void rf_set_channel(uint8_t channel){
	rf_write_register_one(RF_RF_CH, channel);
}

inline uint8_t rf_observe_tx(){
	return rf_read_register_one(RF_OBSERVE_TX);
}
inline bool rf_rpd_active() {
	return ((rf_read_register_one(RF_RPD) & RF_RPD_RPD) ? true : false);
}



#endif /*RF_H_*/
