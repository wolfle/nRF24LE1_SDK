#include "rf.h"
#include "interrupt.h"
#include "pwr_clk_mgmt.h"
#include "delay.h"

//spi operations
#define rf_clear_csn()	sbit_clear(RFCON_SB_RFCSN)	//Clears the CSN pin
#define rf_set_csn()	sbit_set(RFCON_SB_RFCSN)	//Sets the CSN pin
#define rf_get_csn()	RFCON_SB_RFCSN				//Gets the value of the CSN pin

static uint8_t rf_spi_exchange_byte(uint8_t byte)
{
	SPIRDAT = byte; //Send byte over SPI

	//while(!(SPIRSTAT & SPIRSTAT_IRQ_RX_FIFO_READY)); //Wait for the transaction to finish
	//IRCON_SB_RFRDY = BIT_FALSE; //Clear the IRQ bit

	irq_wait_flag(irq_flag_rfrdy);

	return SPIRDAT; //Return the received value
}

static void rf_spi_send_read(uint8_t * dataptr, uint16_t len)
{
	uint8_t tempbyte;

	//Loop through the values that are to be sent/read
	while(len--)
	{
		tempbyte = rf_spi_exchange_byte(*dataptr); //Send the value, and then save the received value

		*dataptr++ = tempbyte;
	}
}

static void rf_spi_send(const uint8_t * dataptr, uint16_t len)
{
	while(len--)rf_spi_exchange_byte(*dataptr++);
}

static uint8_t rf_spi_write(uint8_t cmd, const uint8_t * dataptr, uint16_t len)
{
	rf_clear_csn();
	cmd=rf_spi_exchange_byte(cmd);
	rf_spi_send(dataptr,len);
	rf_set_csn();
	return cmd;	
}

static uint8_t rf_spi_read(uint8_t cmd, uint8_t * dataptr, uint16_t len)
{
	rf_clear_csn(); //Clear CSN to start the transaction

	//Send the instruction first (and receive the STATUS value) then send/receive the other data
	cmd=rf_spi_exchange_byte(cmd);
	rf_spi_send_read(dataptr, len);

	rf_set_csn(); //Set CSN to end the transaction

	return cmd; //Return the value of STATUS
}

uint8_t rf_spi_cmd_zero(uint8_t cmd){
	rf_clear_csn(); //Clear CSN to start the transaction
	cmd=rf_spi_exchange_byte(cmd);
	rf_set_csn(); //Set CSN to end the transaction
	return cmd;
}
static uint8_t rf_spi_read_one(uint8_t cmd){
	rf_clear_csn(); //Clear CSN to start the transaction
	rf_spi_exchange_byte(cmd);
	cmd=rf_spi_exchange_byte(0);
	rf_set_csn(); //Set CSN to end the transaction
	return cmd;
}

static uint8_t rf_spi_write_one(uint8_t cmd, uint8_t val){
	rf_clear_csn(); //Clear CSN to start the transaction
	cmd=rf_spi_exchange_byte(cmd);
	rf_spi_exchange_byte(val);
	rf_set_csn(); //Set CSN to end the transaction
	return cmd;
}
//spi operations end

static uint8_t rf_write_register(uint8_t reg, const uint8_t * buf, uint16_t len){ //return status
	return rf_spi_write(RF_W_REGISTER | (reg & RF_W_REGISTER_DATA), (uint8_t*)buf, len);
}
static uint8_t rf_read_register(uint8_t reg, uint8_t * buf, uint16_t len){ //return status
	return rf_spi_read(reg & RF_R_REGISTER_DATA, buf, len);
}
uint8_t rf_write_register_one(uint8_t reg, uint8_t val){ //return status
	return rf_spi_write_one(RF_W_REGISTER | (reg & RF_W_REGISTER_DATA), val);
}
uint8_t rf_read_register_one(uint8_t reg){ //return reg value
	return rf_spi_read_one(reg & RF_R_REGISTER_DATA);
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
void rf_power_down(){
	rf_power_down_param(rf_read_register_one(RF_CONFIG));
}
static void rf_set_address_width(rf_address_width_e aw){//FIXME: sizeof (enum)
	rf_write_register_one(RF_SETUP_AW, (uint8_t)aw);
}
void rf_set_tx_address(const uint8_t * addr, uint8_t len){
	rf_write_register(RF_TX_ADDR, addr, len);
	rf_set_address_width(len-2); //set address width according to len
}
void rf_set_rx_address(const uint8_t * addr, uint8_t len, uint8_t pipe){
	rf_write_register(RF_RX_ADDR_P0 + pipe, addr, len);
}
static void rf_set_auto_rtr(uint8_t cnt, uint16_t udelay){
	cnt &= 0xf;
	udelay /=250;
	if(udelay>1)--udelay;
	udelay &=0xf;
	cnt |= udelay << 4;
	rf_write_register_one(RF_SETUP_RETR,cnt);
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
void rf_enable_shockburst(rf_date_rate_e dr, rf_pa_e pa, uint8_t retrans, bool prx, bool powerup){// enable dyn payload length, auto ack, ack payload, dyn ack
	rf_write_register_one(RF_EN_AA,RF_EN_AA_ENAA_ALL);
	rf_write_register_one(RF_DYNPD,RF_DYNPD_DPL_ALL);
	rf_write_register_one(RF_FEATURE,RF_FEATURE_ALL);
	rf_set_data_rate_pa(dr,pa,false,false);
	rf_set_auto_rtr(retrans,dr==dr250k?1500:500);// ard=500 for 1m or 2m; ard=1500 for 250k in shockburst setting.
	rf_set_irq_mask_crc_rt(RF_CONFIG_MASK_RX_DR|RF_CONFIG_MASK_TX_DS|RF_CONFIG_MASK_MAX_RT,crc1,prx,powerup);
}

uint8_t rf_write_payload(const uint8_t * buf, uint8_t len){
	return rf_spi_write(RF_W_TX_PAYLOAD, buf, len);
}
/*
uint8_t rf_write_payload(rf_payload_e type, const uint8_t * buf, uint8_t len){
	return rf_spi_write(type, buf, len);
}
*/
static uint8_t rf_read_rx_payload_width(){
	return rf_spi_read_one(RF_R_RX_PL_WID);
}

uint8_t rf_read_rx_payload(uint8_t * buf, uint8_t len, uint8_t ** end){
	uint8_t l=rf_read_rx_payload_width();
	*end=(l>len?NULL:buf+l);
	return rf_spi_read(RF_R_RX_PAYLOAD, buf, l);
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
*/
void rf_transmit_one(){
	rf_set_ce();
	delay_us(10);
	rf_clear_ce();
}

uint8_t rf_write_noack_payload(const uint8_t * buf, uint8_t len){
	return rf_spi_write(RF_W_TX_PAYLOAD_NOACK, buf, len);
}
uint8_t rf_write_ack_payload(const uint8_t * buf, uint8_t len){
	return rf_spi_write(RF_W_ACK_PAYLOAD, buf, len);
}

