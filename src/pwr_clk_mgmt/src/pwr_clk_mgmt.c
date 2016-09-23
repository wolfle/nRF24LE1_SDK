#include "pwr_clk_mgmt.h"


uint32_t get_cclk_freq_in_hz()
{
	return  CCLK_MAX_FREQ_HZ >> ((CLKCTRL & CLKCTRL_CLK_FREQ_MASK) );
}

uint16_t get_cclk_freq_in_khz()
{
	return  CCLK_MAX_FREQ_KHZ >> ((CLKCTRL & CLKCTRL_CLK_FREQ_MASK) );

}

uint8_t get_cclk_freq_in_mhz()
{
	return  CCLK_MAX_FREQ_MHZ >> ((CLKCTRL & CLKCTRL_CLK_FREQ_MASK) );

}
