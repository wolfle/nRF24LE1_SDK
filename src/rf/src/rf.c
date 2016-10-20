#include "rf.h"
#include "interrupt.h"
#include "pwr_clk_mgmt.h"
#include "delay.h"

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

//spi operations
#define rf_clear_csn()	sbit_clear(RFCON_SB_RFCSN)	//Clears the CSN pin
#define rf_set_csn()	sbit_set(RFCON_SB_RFCSN)	//Sets the CSN pin
#define rf_get_csn()	RFCON_SB_RFCSN				//Gets the value of the CSN pin

static volatile __pdata uint8_t rbuf[PAYLOAD_SIZE+1]; //last byte for payload length
static volatile __pdata uint16_t lost=0, sent=0;
static volatile __bit busy=0, incoming=0;

static uint8_t rf_spi_exchange_byte(uint8_t byte) __reentrant
{
	SPIRDAT = byte; //Send byte over SPI

	//while(!(SPIRSTAT & SPIRSTAT_IRQ_RX_FIFO_READY)); //Wait for the transaction to finish
	//IRCON_SB_RFRDY = BIT_FALSE; //Clear the IRQ bit

	irq_wait_flag(irq_flag_rfrdy);

	return SPIRDAT; //Return the received value
}

static void rf_spi_send_read(uint8_t * dataptr, uint8_t len) __reentrant 
{
//	uint8_t tempbyte;

	//Loop through the values that are to be sent/read
	do{
		*dataptr = rf_spi_exchange_byte(*dataptr); //Send the value, and then save the received value
		++dataptr;
	}while(--len);

}

static void rf_spi_send(const uint8_t * dataptr, uint8_t len)
{
	do{rf_spi_exchange_byte(*dataptr++);}while(--len);
}

static uint8_t rf_spi_write(uint8_t cmd, const uint8_t * dataptr, uint8_t len)
{
	rf_clear_csn();
	cmd=rf_spi_exchange_byte(cmd);
	rf_spi_send(dataptr,len);
	rf_set_csn();
	return cmd;	
}

static uint8_t rf_spi_read(uint8_t cmd, uint8_t * dataptr, uint8_t len) __reentrant 
{
	rf_clear_csn(); //Clear CSN to start the transaction

	//Send the instruction first (and receive the STATUS value) then send/receive the other data
	cmd=rf_spi_exchange_byte(cmd);
	rf_spi_send_read(dataptr, len);

	rf_set_csn(); //Set CSN to end the transaction

	return cmd; //Return the value of STATUS
}

static uint8_t rf_spi_cmd_zero(uint8_t cmd) __reentrant {
	rf_clear_csn(); //Clear CSN to start the transaction
	cmd=rf_spi_exchange_byte(cmd);
	rf_set_csn(); //Set CSN to end the transaction
	return cmd;
}
static uint8_t rf_spi_read_one(uint8_t cmd) __reentrant {
	rf_clear_csn(); //Clear CSN to start the transaction
	rf_spi_exchange_byte(cmd);
	cmd=rf_spi_exchange_byte(0);
	rf_set_csn(); //Set CSN to end the transaction
	return cmd;
}

static uint8_t rf_spi_write_one(uint8_t cmd, uint8_t val)__reentrant {
	rf_clear_csn(); //Clear CSN to start the transaction
	cmd=rf_spi_exchange_byte(cmd);
	rf_spi_exchange_byte(val);
	rf_set_csn(); //Set CSN to end the transaction
	return cmd;
}
//spi operations end

static inline uint8_t rf_write_register(uint8_t reg, const uint8_t * buf, uint8_t len){ //return status
	return rf_spi_write(RF_W_REGISTER | (reg & RF_W_REGISTER_DATA), (uint8_t*)buf, len);
}
static inline uint8_t rf_read_register(uint8_t reg, uint8_t * buf, uint8_t len){ //return status
	return rf_spi_read(reg & RF_R_REGISTER_DATA, buf, len);
}
static inline uint8_t rf_write_register_one(uint8_t reg, uint8_t val){ //return status
	return rf_spi_write_one(RF_W_REGISTER | (reg & RF_W_REGISTER_DATA), val);
}
static inline uint8_t rf_read_register_one(uint8_t reg){ //return reg value
	return rf_spi_read_one(reg & RF_R_REGISTER_DATA);
}

void rf_set_channel(uint8_t channel){
	rf_write_register_one(RF_RF_CH, channel);
}

uint8_t rf_observe_tx(){
	return rf_read_register_one(RF_OBSERVE_TX);
}

bool rf_rpd_active() {
	return ((rf_read_register_one(RF_RPD) & RF_RPD_RPD) ? true : false);
}


static void rf_power_up_param(uint8_t config){
	config |= RF_CONFIG_PWR_UP;
	rf_write_register_one(RF_CONFIG, config);
	while(!pwr_clk_mgmt_is_cclk_src_xosc16m()); //wait for the clock to get switched to the 16 MHz crystal, otherwise the RF can't be run
	//delay_us(1); //Delay for power up
}
void rf_power_up(){
	rf_power_up_param(rf_read_register_one(RF_CONFIG));
}
static void rf_power_down_param(uint8_t config){
	rf_clear_ce(); //Ensure the CE pin is cleared
	config &= (~RF_CONFIG_PWR_UP); //Use the passed in value of CONFIG and ensure the PWR_UP bit is cleared, then write the register
	rf_write_register_one(RF_CONFIG, config);
}
void rf_power_down(void){
	rf_power_down_param(rf_read_register_one(RF_CONFIG));
}
static inline void rf_set_address_width(rf_address_width_e aw){//FIXME: sizeof (enum)
	rf_write_register_one(RF_SETUP_AW, (uint8_t)aw);
}
static void rf_set_tx_address(__code const uint8_t * addr, uint8_t len){ //always set tx addr == rx pipe0 addr
	rf_set_address_width(len-2); //set address width according to len
	rf_write_register(RF_TX_ADDR, addr, len);
	rf_write_register(RF_RX_ADDR_P0, addr, len);
	rf_write_register_one(RF_EN_RXADDR, RF_EN_RXADDR_ERX_P0); //only enable pipe0 in ptx mode
}
static void rf_set_rx_address(__code const uint8_t * addr, uint8_t len, uint8_t pipe){//always set tx addr == rx pipe0 addr
	rf_write_register(RF_RX_ADDR_P0 + pipe, addr, len);
	if(pipe==0)	rf_set_address_width(len-2); //set address width according to len
	rf_write_register_one(RF_EN_RXADDR,rf_read_register_one(RF_EN_RXADDR)| (1<<pipe));
}
static void rf_set_auto_rtr(uint8_t cnt, uint16_t udelay){
	udelay /=250;
	if(udelay>1)--udelay;
	rf_write_register_one(RF_SETUP_RETR,(uint8_t)(((uint8_t)udelay&0xf) << 4)|(cnt&0xf));
}
static void rf_set_irq_mask_crc_rt(uint8_t mask, rf_crc_e crc, bool prx, bool powerup){// mask : 2:0 rx_dr|tx_ds|max_rt
		if(crc!=crc0)mask|=RF_CONFIG_EN_CRC;
		if(crc==crc2)mask|=RF_CONFIG_CRCO;
		if(prx)mask|=RF_CONFIG_PRIM_RX;
		if(powerup)mask|=RF_CONFIG_PWR_UP;
		rf_write_register_one(RF_CONFIG,mask);
}
static void rf_set_data_rate_pa(rf_date_rate_e dr, rf_pa_e pa,bool plllock, bool contwave){
	dr<<=3;
	dr|=pa;
	if(plllock)dr|=RF_RF_SETUP_PLL_LOCK;
	if(contwave)dr|=RF_RF_SETUP_CONT_WAVE;
    rf_write_register_one(RF_RF_SETUP, dr);
}
static void rf_enable_shockburst(rf_date_rate_e dr, rf_pa_e pa, uint8_t retrans, bool prx, bool powerup){// enable dyn payload length, auto ack, ack payload, dyn ack
	rf_write_register_one(RF_EN_AA,RF_EN_AA_ENAA_ALL);
	rf_write_register_one(RF_DYNPD,RF_DYNPD_DPL_ALL);
	rf_write_register_one(RF_FEATURE,RF_FEATURE_ALL);
	rf_set_data_rate_pa(dr,pa,false,false);
	rf_set_auto_rtr(retrans,dr==dr250k?1500:500);// ard=500 for 1m or 2m; ard=1500 for 250k in shockburst setting.
	rf_set_irq_mask_crc_rt(RF_CONFIG_MASK_RX_DR|RF_CONFIG_MASK_TX_DS|RF_CONFIG_MASK_MAX_RT,crc1,prx,powerup);
}

// Configures RF parameters before Enhanced Shockburst can be used.
void rf_init(const struct rf_params *p,bool prx, bool powerup)
{
  // Enable the radio clock
  rf_clock_enable();
  // Set pipe address
  if(prx)rf_set_rx_address(p->addr,p->addrlen,0);
  else rf_set_tx_address(p->addr,p->addrlen); //also set addr width and pipe 0 addr
  // Set initial channelC
  rf_set_channel(p->channel);
  // Enable shockburst
  rf_enable_shockburst(p->dr,p->pa,p->retrans,prx,powerup); //data rate 2m, pa=0dbm, retransmit 5 times, ptx mode, powerup
 }

static inline uint8_t rf_write_payload(__pdata const uint8_t * buf, uint8_t len){
	return rf_spi_write(RF_W_TX_PAYLOAD, buf, len);
}
static inline uint8_t rf_read_rx_payload_width(void) __reentrant{
	return rf_spi_read_one(RF_R_RX_PL_WID);
}

static inline uint8_t rf_read_rx_payload(void) __reentrant{
	rbuf[PAYLOAD_SIZE]=rf_read_rx_payload_width();
	return rf_spi_read(RF_R_RX_PAYLOAD, rbuf, rbuf[PAYLOAD_SIZE]);
}
/*
void rf_set_as_rx(){
        rf_clear_ce(); //Clear the CE pin
		rf_write_register_one(RF_CONFIG, rf_read_register_one(RF_CONFIG)|RF_CONFIG_PRIM_RX);
}
void rf_set_as_tx(){
    rf_clear_ce(); //Clear the CE pin
	rf_write_register_one(RF_CONFIG, rf_read_register_one(RF_CONFIG)&(~RF_CONFIG_PRIM_RX));
}
void rf_set_rx_payload_width(uint8_t pipe, uint8_t pwidth){ //only if not using dynamic payload length, ptx must write the same length to tx fifo
	rf_write_register_one(RF_RX_PW_P0 + pipe, pwidth);
}
void rf_transmit_one(){
	rf_set_ce();
	delay_us(10);
	rf_clear_ce();
}


uint8_t rf_write_noack_payload(const uint8_t * buf, uint8_t len){
	return rf_spi_write(RF_W_TX_PAYLOAD_NOACK, buf, len);
}
uint8_t rf_write_ack_payload(uint8_t pipe, const uint8_t * buf, uint8_t len){
	return rf_spi_write(RF_W_ACK_PAYLOAD|(pipe&0x7), buf, len);
}
*/

irq_isr_rfirq(){
  uint8_t status=rf_get_status();

  CE_L;
  // Transmission success
  if(rf_is_tx_ds_active(status))
	  ++sent; // Data has been sent
  
  // Transmission failed (maximum re-transmits)
  if(rf_is_max_rt_active(status)){
      rf_flush_tx();
      ++lost;
  }

  // Data received 
  if(rf_is_rx_dr_active(status)){
	  incoming=0;
      rf_read_rx_payload();
      incoming=1;
  }
  
  busy=0;
}

uint16_t rf_lost(void){return lost;}
uint16_t rf_sent(void){return sent;}
bool rf_incoming(void){return incoming;}

PT(rf_send,__pdata void * buf, uint8_t size)
{
	PT_B
	PT_WAIT(!busy);
	if(RF_STATUS_TX_FULL&rf_write_payload(buf, size))busy=1;
	// Activate sender
	CE_H;
	PT_E
}
