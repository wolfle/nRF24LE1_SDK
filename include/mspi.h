/////////////////////////////////////////////////////////////////////////////
//
// File: mspi.h
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

#ifndef MSPI_H_
#define MSPI_H_

#include "reg24le1.h"
#include "pt.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Configuration defines for use with mspi_configure() function...OR these together to form the mspi_config_options argument
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define MSPI_ENABLE								(SPIMCON0_ENABLE)						//Enables MSPI communication
#define MSPI_DISABLE								(0)																					//Disables MSPI communication

#define MSPI_CPHA_SAMPLE_ON_MSCK_EDGE_TRAILING	(SPIMCON0_CPHA)						//Configures MSPI to sample on trailing edge of MSCK, and shift on leading edge
#define MSPI_CPHA_SAMPLE_ON_MSCK_EDGE_LEADING		(0)																					//Configures MSPI to sample on leading edge of MSCK, and shift on trailing edge

#define MSPI_CPOL_MSCK_ACTIVE_LOW					(SPIMCON0_CPOL)						//Configures the clock polarity to be active low
#define MSPI_CPOL_MSCK_ACTIVE_HIGH				(0)																					//Configures the clock polarity to be active high

#define MSPI_DATA_ORDER_LSB_FIRST					(SPIMCON0_DATA_ORDER)					//Send data with LSb first
#define MSPI_DATA_ORDER_MSB_FIRST					(0)																					//Send data with MSb first

#define	MSPI_MCLK_IS_CCLK_DIV_2					(0x00)		//Set the MCLK frequency to CCLK/2  (  8 MHz if CCLK = 16 MHz)
#define	MSPI_MCLK_IS_CCLK_DIV_4					(0x01 << SPIMCON0_CLK_DIV_SHIFT)		//Set the MCLK frequency to CCLK/4  (  4 MHz if CCLK = 16 MHz)
#define	MSPI_MCLK_IS_CCLK_DIV_8					(0x02 << SPIMCON0_CLK_DIV_SHIFT)		//Set the MCLK frequency to CCLK/8  (  2 MHz if CCLK = 16 MHz)
#define	MSPI_MCLK_IS_CCLK_DIV_16					(0x03 << SPIMCON0_CLK_DIV_SHIFT)		//Set the MCLK frequency to CCLK/16 (  1 MHz if CCLK = 16 MHz)
#define	MSPI_MCLK_IS_CCLK_DIV_32					(0x04 << SPIMCON0_CLK_DIV_SHIFT)		//Set the MCLK frequency to CCLK/32 (500 kHz if CCLK = 16 MHz)
#define	MSPI_MCLK_IS_CCLK_DIV_64					(0x05 << SPIMCON0_CLK_DIV_SHIFT)		//Set the MCLK frequency to CCLK/64 (250 kHz if CCLK = 16 MHz)



///////////////////////////////////////////
// Function macros
///////////////////////////////////////////
#define mspi_enable()				(SPIMCON0 |= SPIMCON0_ENABLE)									//Enable MSPI
#define mspi_disable()				(SPIMCON0 &= ~SPIMCON0_ENABLE)									//Disable MSPI

///////////////////////////////////////////
// Function prototypes
///////////////////////////////////////////
void mspi_init(uint8_t mspi_config_options);

/* In order for the clock to run you must be writing, so if you want to read
 * data you still need to do the mspi_read_write with a zero argument to avoid
 * sending any meaningful data to the other side. This will drive the clock and
 * let you read data back.
 */
PT(mspi_io,__pdata void *data,uint16_t size);//1

#endif /*MSPI_H_*/
