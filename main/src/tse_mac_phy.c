//=====================================================================================
// Copyright (C) 2013-2021 Macnica Inc. All Rights Reserved.
//
// Use in source and binary forms, with or without modification, are permitted provided
// by agreeing to the following terms and conditions:
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//------------------------------------------------------------------------------------
//! @file
//! @brief  This file provides ALTERA triple Ethernet MAC and TI DP83867IR
//!         initialization function
//=====================================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../libscorpius/include/scorpius.h"
#include "../../main_bsp/HAL/inc/sys/alt_flash.h"
#include "../../main_bsp/HAL/inc/sys/alt_irq.h"
#include "../../main_bsp/drivers/inc/altera_avalon_pio_regs.h"
#include "../../main_bsp/drivers/inc/altera_avalon_tse.h"
#include "tse_mac_phy.h"


//------------------------------------------------------------------------------
// define and structures
//------------------------------------------------------------------------------
#define TXSHIFT16
#define PHY_SPECIFIC_STATUS_REG 17
#define LINK_STATUS_BIT 10
#define LINK_10M         0
#define LINK_100M        1
#define LINK_1000M       2
#define FULL_DUPLEX      1
#define HALF_DUPLEX      0
#define PIO_PORT_NUM_PTP 1

/* PHY register of all Page Any definition */
enum {
	TSE_PHY_PAGE_ADDRESS                   = 22,
};

/* PHY register of Page 3 definition */
enum {
	TSE_PHY_LED_FUNCTION_CONTROL           = 16,
	TSE_PHY_LED_TIMER_CONTROL              = 18,
};

/* PHY register of Page 6 definition */
enum {
	TSE_PHY_GENERAL_CONTROL                = 20,
};

/* PHY register of Page 8 definition */
enum {
	TSE_PHY_PTP_PORT_CONFIGURATION_0       = 0,
	TSE_PHY_PTP_PORT_CONFIGURATION_1       = 1,
	TSE_PHY_PTP_PORT_CONFIGURATION_2       = 2,
	TSE_PHY_PTP_PORT_CONFIGURATION_3       = 3,
	TSE_PHY_PTP_ARRIVAL_0_TIME_PORT_STATUS = 8,
	TSE_PHY_PTP_ARRIVAL_0_TIME_BYTES_1_0   = 9,
	TSE_PHY_PTP_ARRIVAL_0_TIME_BYTES_3_2   = 10,
	TSE_PHY_PTP_ARRIVAL_0_SEQUENCE_ID      = 11,
	TSE_PHY_PTP_ARRIVAL_1_TIME_PORT_STATUS = 12,
	TSE_PHY_PTP_ARRIVAL_1_TIME_BYTES_1_0   = 13,
	TSE_PHY_PTP_ARRIVAL_1_TIME_BYTES_3_2   = 14,
	TSE_PHY_PTP_ARRIVAL_1_SEQUENCE_ID      = 15,
};

/* PHY register of Page 9 definition */
enum {
	TSE_PHY_PTP_DEPARTURE_TIME_PORT_STATUS = 0,
	TSE_PHY_PTP_DEPARTURE_TIME_BYTES_1_0   = 1,
	TSE_PHY_PTP_DEPARTURE_TIME_BYTES_3_2   = 2,
	TSE_PHY_PTP_DEPARTURE_SEQUENCE_ID      = 3,
};

/* PHY register of Page 12 definition */
enum {
	TSE_PHY_TAI_GLOBAL_CONFIGURATION_0     = 0,
	TSE_PHY_TAI_GLOBAL_CONFIGURATION_1     = 1,
	TSE_PHY_TAI_GLOBAL_CONFIGURATION_2     = 2,
	TSE_PHY_TAI_GLOBAL_CONFIGURATION_3     = 3,
	TSE_PHY_TAI_GLOBAL_CONFIGURATION_5     = 5,
};

/* PHY register of Page 14 definition */
enum {
	TSE_PHY_PTP_GLOBAL_CONFIGURATION_0     = 0,
	TSE_PHY_PTP_GLOBAL_CONFIGURATION_1     = 1,
	TSE_PHY_PTP_GLOBAL_CONFIGURATION_2     = 2,
	TSE_PHY_PTP_GLOBAL_CONFIGURATION_3     = 3,
	TSE_PHY_READ_PLUS_COMMAND              = 14,
	TSE_PHY_READ_PLUS_DATA                 = 15,
};


//------------------------------------------------------------------------------
// prototypes
//------------------------------------------------------------------------------
static alt_32 alt_tse_phy_wr_mdio_reg_macspire(np_tse_mac *pmac, alt_u8 page_num, alt_u8 reg_num,
		alt_u8 lsb_num, alt_u8 bit_length, alt_u16 data);
static alt_u32 alt_tse_phy_rd_mdio_reg_macspire(np_tse_mac *pmac, alt_u8 page_num, alt_u8 reg_num,
		alt_u8 lsb_num, alt_u8 bit_length);
static alt_32 alt_tse_phy_set_adv_1000_macspire(np_tse_mac *pmac, alt_u8 enable);
static alt_32 alt_tse_phy_set_adv_100_macspire(np_tse_mac *pmac, alt_u8 enable);
static alt_32 alt_tse_phy_set_adv_10_macspire(np_tse_mac *pmac, alt_u8 enable);
static alt_32 alt_tse_phy_restart_an_macspire(np_tse_mac *pmac, alt_u32 timeout_threshold);

//------------------------------------------------------------------------------
// global variables
//------------------------------------------------------------------------------
static volatile alt_u8 g_current_page = 0;


//------------------------------------------------------------------------------
//! @brief Initialize triple Ethernet MAC
//
//! @retval  0 : Success
//! @retval -1 : Failure
//------------------------------------------------------------------------------
int init_tse_mac_and_phy(unsigned char *p_mac_addr)
{
	// variable for initialize MAC
	unsigned int base;
	unsigned int x;
	unsigned int dat;

	alt_u32 status;
//	alt_flash_fd *p_dev;
//	int ret;

	// variable for initialize PHY
	alt_32 phyid;
	alt_32 phyid2 = 0;
	alt_u8 phyadd;
	alt_u32 oui;
	alt_u8 model_number;
	alt_u8 revision_number;
	alt_u8 is_phy_in_profile;
	np_tse_mac *pmac_group_base;

	unsigned char pio_data;

	printf("Ethernet MAC INIT: Start\n");
	base = EMAC_BASE;

	// reset the MAC
//	IOWR_ALTERA_TSEMAC_CMD_CONFIG(base, mmac_cc_SW_RESET_mask | mmac_cc_TX_ENA_mask | mmac_cc_RX_ENA_mask);
	IOWR_ALTERA_TSEMAC_CMD_CONFIG(base, mmac_cc_SW_RESET_mask);
//	tse_mac_SwReset(base)

	// check reset
	x = 0;
	while (IORD_ALTERA_TSEMAC_CMD_CONFIG(base) & ALTERA_TSEMAC_CMD_SW_RESET_MSK) {
		if (x++ > ALTERA_TSE_SW_RESET_TIME_OUT_CNT) { break; }
	}
	if (x >= ALTERA_TSE_SW_RESET_TIME_OUT_CNT) { tse_dprintf(3, "Error: TSEMAC SW reset bit never cleared!\n"); }

	// check TX_ENA and RX_ENA bit
	dat = IORD_ALTERA_TSEMAC_CMD_CONFIG(base);
	if ((dat & 0x03) != 0) { tse_dprintf(3, "WARN: RX/TX not disabled after reset... missing PHY clock? CMD_CONFIG = %#x\n", dat); }
	else { tse_dprintf(3, "INFO: IORD_ALTERA_TSEMAC_CMD_CONFIG = %#x\n", dat); }

#if 0
	// open flash memory device
	p_dev = alt_flash_open_dev(EXT_FLASH_NAME);
	if (p_dev == NULL) { printf("Can not initialize CFI flash device!\r\n"); return -1; }

	// read MAC address from flash memory
	ret = alt_read_flash(p_dev, 0x3FE0000 + 4, p_mac_addr, 0x6);
	if (ret) { alt_flash_close_dev(p_dev); printf("Can not write CFI flash!\r\n"); return -1; }

	// close flash device
	alt_flash_close_dev(p_dev);

	if (
		((p_mac_addr[0] == 0)    && (p_mac_addr[1] == 0)    && (p_mac_addr[2] == 0) &&
		 (p_mac_addr[3] == 0)    && (p_mac_addr[4] == 0)    && (p_mac_addr[5] == 0)) ||
		((p_mac_addr[0] == 0xFF) && (p_mac_addr[1] == 0xFF) && (p_mac_addr[2] == 0xFF) &&
		 (p_mac_addr[3] == 0xFF) && (p_mac_addr[4] == 0xFF) && (p_mac_addr[5] == 0xFF))
		) {
#endif
		// set global the MAC address
		pio_data = IORD_ALTERA_AVALON_PIO_DATA(GEV_PIO_I_DIP_SW_BASE);

		if ((pio_data & 0x4) == 0) { /* check USER_DIP0 switch */
			p_mac_addr[0] = 0x08;
			p_mac_addr[1] = 0x00;
			p_mac_addr[2] = 0x42;
			p_mac_addr[3] = 0xF0;
			p_mac_addr[4] = 0xFF;
			p_mac_addr[5] = 0x00;
		}
		else {
			p_mac_addr[0] = 0x08;
			p_mac_addr[1] = 0x00;
			p_mac_addr[2] = 0x42;
			p_mac_addr[3] = 0xF0;
			p_mac_addr[4] = 0xFF;
			p_mac_addr[5] = 0x01;
		}
#if 0
	}
#endif

	/* Set the MAC address */
	IOWR_ALTERA_TSEMAC_MAC_0(base,
							 ((int)(p_mac_addr[0])       |
							  (int)(p_mac_addr[1] <<  8) |
							  (int)(p_mac_addr[2] << 16) |
							  (int)(p_mac_addr[3] << 24)));

	IOWR_ALTERA_TSEMAC_MAC_1(base,
							 (((int)(p_mac_addr[4])      |
							   (int)(p_mac_addr[5] <<  8)) & 0xFFFF));

	// initialize MAC registers


	// Base Configuration Registers (Dword Offset 0x00 - 0x17)
	IOWR_32DIRECT(base, 0x05 * 4, 9600);        // frm_length
	IOWR_32DIRECT(base, 0x06 * 4, 0x00);        // pause_guant
	IOWR_32DIRECT(base, 0x07 * 4, 0x7F0);       // rx_section_empty
	IOWR_32DIRECT(base, 0x08 * 4, 0x00);        // rx_section_full
	IOWR_32DIRECT(base, 0x09 * 4, 0x7F0);       // tx_section_empty
	IOWR_32DIRECT(base, 0x0A * 4, 0x10);        // tx_section_full
	IOWR_32DIRECT(base, 0x0B * 4, 0x8);         // rx_almost_empty
	IOWR_32DIRECT(base, 0x0C * 4, 0x8);         // rx_almost_full
	IOWR_32DIRECT(base, 0x0D * 4, 0x8);         // tx_almost_empty
	IOWR_32DIRECT(base, 0x0E * 4, 0x3);         // tx_almost_full
	IOWR_32DIRECT(base, 0x0F * 4, 0x00);        // mdio_addr0
	IOWR_32DIRECT(base, 0x11 * 4, 0x00);        // holdoff_quant

    /* IOWR_32DIRECT(base, 0x02 * 4, 0x00008030);  // command_config ( [1]RX_ENA=0, [0]TX_ENA=0 ) */

    /* IOWR_ALTERA_TSEMAC_CMD_CONFIG(base, mmac_cc_SW_RESET_mask | mmac_cc_TX_ENA_mask | mmac_cc_RX_ENA_mask); */

    /* dat = IORD_ALTERA_TSEMAC_CMD_CONFIG(base); */
    /* IOWR_ALTERA_TSEMAC_CMD_CONFIG(base, dat | mmac_cc_SW_RESET_mask ); */

	/* printf("MAC initial end\n"); */

	//------------------------------------------------------------------------------------
	// start initialization of PHY
	//------------------------------------------------------------------------------------
	printf("Ethernet PHY INIT: Start\n");
	pmac_group_base = (np_tse_mac *)base;

	/* alt_tse_phy_profile MV88E1512 = {"Marvell 88E1512",     // Marvell 88E1512 */
	/* 								 MV88E1512_OUI,          // OUI */
	/* 								 MV88E1512_MODEL,        // Vender Model Number */
	/* 								 MV88E1512_REV,          // Model Revision Number */
	/* 								 0x11,                   // Location of Status Register */
	/* 								 14,                     // Location of Speed Status */
	/* 								 13,                     // Location of Duplex Status */
	/* 								 10,                     // Location of Link Status */
	/* 								 &marvell_phy_cfg        // Function pointer to configure Marvell PHY */
	/* }; */
	/* alt_tse_phy_profile KSZ9031 = {"Micrel KSZ9031",        // Micrel KSZ9031 */
	/* 							   0x000885,               // OUI */
	/* 							   0x22,                   // Vender Model Number */
	/* 							   0x2,                    // Model Revision Number */
	/* 							   0,                      // Location of Status Register */
	/* 							   0,                      // Location of Speed Status */
	/* 							   0,                      // Location of Duplex Status */
	/* 							   0,                      // Location of Link Status */
	/* 							   0                       // No function pointer configure */
	/* }; */
	alt_tse_phy_profile DP83867IR = {"TI DP83867IR",        // TI DP83867IR
									 DP83867IR_OUI,          // OUI
									 DP83867IR_MODEL,        // Vender Model Number
									 DP83867IR_REV,          // Model Revision Number
									 0x11,                   // Location of Status Register
									 14,                     // Location of Speed Status
									 13,                     // Location of Duplex Status
									 2                       // Location of Link Status
	};
	/* loop all valid PHY address to look for connected PHY */
	for (phyadd = 0x00; phyadd < 0x20; phyadd++) {
	/* for (phyadd = 0x01; phyadd < 0x20; phyadd++) { */
		IOWR(&pmac_group_base->MDIO_ADDR1, 0, phyadd);
		/* IOWR(&pmac_group_base->mdio1, TSE_PHY_PAGE_ADDRESS, 0); */
		/* g_current_page = 0; */
		phyid  = IORD(&pmac_group_base->mdio1.PHY_ID1,0);    // read PHY ID
		phyid2 = IORD(&pmac_group_base->mdio1.PHY_ID2,0);    // read PHY ID


		/* PHY found */
		if (phyid != phyid2) {
			/* get OUI, model number, and revision number from PHYID and PHYID2 */
			oui = (phyid << 6) | ((phyid2 >> 10) & 0x3f);
			model_number = (phyid2 >> 4) & 0x3f;
			revision_number = phyid2 & 0x0f;

			/* map the PHY with PHY in profile */
			is_phy_in_profile = 0;

			// if PHY match with PHY in profile
			/* if ((MV88E1512.oui  == oui) && (MV88E1512.model_number  == model_number)) { */
			/* if ((KSZ9031.oui    == oui) && (KSZ9031.model_number    == model_number)) { */
			if ((DP83867IR.oui  == oui) && (DP83867IR.model_number  == model_number)) {

				// PHY found, add it to phy_list
				tse_dprintf(5, "INFO: PHY found at PHY address %#x of MAC Group\n", phyadd);
				is_phy_in_profile = 1;
				break;
			}

			// PHY not found in PHY profile
			if (is_phy_in_profile == 0) {
				tse_dprintf(3, "WARN: Unknown PHY found at PHY address %#x of MAC Group\n", phyadd);
				tse_dprintf(3, "WARN: Please add PHY information to PHY profile\n");
			}

			tse_dprintf(6, "INFO: PHY OUI             = %#x\n", (int) oui);
			tse_dprintf(6, "INFO: PHY Model Number    = %#x\n", model_number);
			tse_dprintf(6, "INFO: PHY Revision Number = %#x\n", revision_number);
		}
	}


	tse_dprintf(5, "TI : Enable GMII Timing Control\n");
	alt_tse_phy_wr_mdio_reg_macspire(pmac_group_base, 0, 0x0D, 0, 16, 0x001F);  // REGCR
	alt_tse_phy_wr_mdio_reg_macspire(pmac_group_base, 0, 0x0E, 0, 16, 0x0032);  // ADDR: 0x32
	alt_tse_phy_wr_mdio_reg_macspire(pmac_group_base, 0, 0x0D, 0, 16, 0x401F);  // ???? 0x401F
	alt_tse_phy_wr_mdio_reg_macspire(pmac_group_base, 0, 0x0E, 7, 1, 0);        // [7]=0
	alt_tse_phy_wr_mdio_reg_macspire(pmac_group_base, 0, 0x0E, 0, 16, 0x0050);  // DATA: 0x50


	// disable PHY loop back to allow Auto-Negotiation completed
	alt_tse_phy_wr_mdio_reg_macspire(pmac_group_base, 0, TSE_PHY_MDIO_CONTROL, TSE_PHY_MDIO_CONTROL_LOOPBACK, 1, 0);

	if (phyadd >= 0x20) { tse_dprintf(3, "WARN: Not Found DP83867IR PHY!\n"); return -1; }

	// reset auto-negotiation advertisement
	alt_tse_phy_set_adv_1000_macspire(pmac_group_base, 1);
	alt_tse_phy_set_adv_100_macspire(pmac_group_base, 1);
	alt_tse_phy_set_adv_10_macspire(pmac_group_base, 1);

	// check link connection for this PHY
	status = alt_tse_phy_restart_an_macspire(pmac_group_base, ALTERA_CHECKLINK_TIMEOUT_THRESHOLD);
	if (status != TSE_PHY_AN_COMPLETE) { printf("alt_tse_phy_restart_an_macspire() error!\r\n"); return -1; }

	printf("Ethernet PHY INIT: End\n");
	return 0;
}

//------------------------------------------------------------------------------
//! @brief Write data to MDIO register
//
//! @param[in] p_mac      : Pointer to alt_tse_phy_info structure
//! @param[in] page_num   : Page Number of MDIO register to be write.
//! @param[in] reg_num    : Location of MDIO register to be write.
//! @param[in] lsb_num    : Least significant bit location of MDIO register to be read.
//! @param[in] bit_length : Number of bits to be written to the register.
//! @param[in] data       : Data write to MDIO register.
//
//! @retval SUCCESS : Success
//------------------------------------------------------------------------------
static alt_32 alt_tse_phy_wr_mdio_reg_macspire(np_tse_mac *p_mac, alt_u8 page_num, alt_u8 reg_num,
	alt_u8 lsb_num, alt_u8 bit_length, alt_u16 data)
{
	alt_u16 temp_data;
	alt_u16 bit_mask;
	alt_32 i;
	u32 irq;

	bit_mask = 0x00;
	/* generate mask consist of bit_length number of 1
	 * eg: bit_length = 3, bit_mask = 0b0000 0000 0000 0111
	 */
	for (i = 0; i < bit_length; i++) {
		bit_mask <<= 1;
		bit_mask |= 0x01;
	}

	// shifting mask to left by bit_num
	bit_mask <<= lsb_num;

	disable_interrupt(&irq);

	if (g_current_page != page_num)
	{
		IOWR(&p_mac->mdio1, TSE_PHY_PAGE_ADDRESS, page_num);
		g_current_page = page_num;
	}

	// read register data
	temp_data = IORD(&p_mac->mdio1, reg_num);

	// clear bits to be written
	temp_data &= ~bit_mask;

	// OR-ed together corresponding bits data
	temp_data |= ((data << lsb_num) & bit_mask);

	// write data to MDIO register
	IOWR(&p_mac->mdio1, reg_num, temp_data);

	enable_interrupt(irq);

	return SUCCESS;
}

//------------------------------------------------------------------------------
//! @brief Read bit_length number of bits from MDIO register based on register
//         location reg_num and start from bit location lsb_num.
//
//! @param[in] p_mac      : Pointer to alt_tse_phy_info structure
//! @param[in] page_num   : Page Number of MDIO register to be read.
//! @param[in] reg_num    : Location of MDIO register to be read.
//! @param[in] lsb_num    : Least significant bit location of MDIO register to be read.
//! @param[in] bit_length : Number of bits to be read from the register.
//
//! @retval data read from MDIO register
//------------------------------------------------------------------------------
static alt_u32 alt_tse_phy_rd_mdio_reg_macspire(np_tse_mac *p_mac, alt_u8 page_num, alt_u8 reg_num,
	alt_u8 lsb_num, alt_u8 bit_length)
{
	alt_u16 temp_data;
	alt_u32 bit_mask;
	alt_32 i;
	u32 irq;

	bit_mask = 0x00;
	/* generate mask consist of bit_length number of 1
	 * eg: bit_length = 3, bit_mask = 0b0000 0000 0000 0111
	 */
	for (i = 0; i < bit_length; i++) {
		bit_mask <<= 1;
		bit_mask |= 0x01;
	}

	disable_interrupt(&irq);

	if (g_current_page != page_num)
	{
		IOWR(&p_mac->mdio1, TSE_PHY_PAGE_ADDRESS, page_num);
		g_current_page = page_num;
	}

	// read register data
	temp_data = IORD(&p_mac->mdio1, reg_num);

	enable_interrupt(irq);

	// shifting read data
	temp_data >>= lsb_num;

	return (temp_data & bit_mask);
}

//------------------------------------------------------------------------------
//! @brief Set the advertisement of PHY for 1000 Mbps
//
//! @param[in] p_mac  : Pointer to alt_tse_phy_info structure
//! @param[in] enable : Set Enable = 1 to advertise this speed if the PHY capable
//!                     Set Enable = 0 to disable advertise of this speed
//
//! @retval SUCCESS
//------------------------------------------------------------------------------
static alt_32 alt_tse_phy_set_adv_1000_macspire(np_tse_mac *p_mac, alt_u8 enable)
{
	alt_u8 cap;

	// if enable = 1, set advertisement based on PHY capability
	if (enable) {
		cap = alt_tse_phy_rd_mdio_reg_macspire(p_mac, 0, TSE_PHY_MDIO_EXT_STATUS, TSE_PHY_MDIO_EXT_STATUS_1000BASE_T_FULL, 1);
		alt_tse_phy_wr_mdio_reg_macspire(p_mac, 0, TSE_PHY_MDIO_1000BASE_T_CTRL, TSE_PHY_MDIO_1000BASE_T_CTRL_FULL_ADV, 1, cap);
		tse_dprintf(6, "INFO: PHY - Advertisement of 1000 Base-T Full Duplex set to %d\n", cap);

		// 1000 Mbps Half duplex not supported by TSE MAC
		/* cap = alt_tse_phy_rd_mdio_reg_macspire(p_mac, 0, TSE_PHY_MDIO_EXT_STATUS, TSE_PHY_MDIO_EXT_STATUS_1000BASE_T_HALF, 1); */
		/* alt_tse_phy_wr_mdio_reg_macspire(p_mac, 0, TSE_PHY_MDIO_1000BASE_T_CTRL, TSE_PHY_MDIO_1000BASE_T_CTRL_HALF_ADV, 1, cap); */
		/* tse_dprintf(6, "INFO: PHY - Advertisement of 1000 Base-T Half Duplex set to %d\n", cap); */
	}
	// else disable advertisement of this speed
	else {
		alt_tse_phy_wr_mdio_reg_macspire(p_mac, 0, TSE_PHY_MDIO_1000BASE_T_CTRL, TSE_PHY_MDIO_1000BASE_T_CTRL_FULL_ADV, 1, 0);
		tse_dprintf(6, "INFO: PHY - Advertisement of 1000 Base-T Full Duplex set to %d\n", 0);

		// 1000 Mbps Half duplex not supported by TSE MAC
		/* alt_tse_phy_wr_mdio_reg_macspire(p_mac, 0, TSE_PHY_MDIO_1000BASE_T_CTRL, TSE_PHY_MDIO_1000BASE_T_CTRL_HALF_ADV, 1, 0); */
		/* tse_dprintf(6, "INFO: PHY - Advertisement 1000 Base-T half Duplex set to %d\n", 0); */
	}

	return SUCCESS;
}

//------------------------------------------------------------------------------
//! @brief Set the advertisement of PHY for 100 Mbps
//
//! @param[in] p_mac  : Pointer to alt_tse_phy_info structure
//! @param[in] enable : Set Enable = 1 to advertise this speed if the PHY capable
//!                     Set Enable = 0 to disable advertise of this speed
//
//! @retval SUCCESS
//------------------------------------------------------------------------------
static alt_32 alt_tse_phy_set_adv_100_macspire(np_tse_mac *p_mac, alt_u8 enable)
{
	alt_u8 cap;

	// if enable = 1, set advertisement based on PHY capability
	if (enable) {
		cap = alt_tse_phy_rd_mdio_reg_macspire(p_mac, 0, TSE_PHY_MDIO_STATUS, TSE_PHY_MDIO_STATUS_100BASE_T4, 1);
		alt_tse_phy_wr_mdio_reg_macspire(p_mac, 0, TSE_PHY_MDIO_ADV, TSE_PHY_MDIO_ADV_100BASE_T4, 1, cap);
		tse_dprintf(6, "INFO: PHY - Advertisement of 100 Base-T4 set to %d\n", cap);

		cap = alt_tse_phy_rd_mdio_reg_macspire(p_mac, 0, TSE_PHY_MDIO_STATUS, TSE_PHY_MDIO_STATUS_100BASE_X_FULL, 1);
		alt_tse_phy_wr_mdio_reg_macspire(p_mac, 0, TSE_PHY_MDIO_ADV, TSE_PHY_MDIO_ADV_100BASE_TX_FULL, 1, cap);
		tse_dprintf(6, "INFO: PHY - Advertisement of 100 Base-TX Full Duplex set to %d\n", cap);

		/* cap = alt_tse_phy_rd_mdio_reg_macspire(p_mac, 0, TSE_PHY_MDIO_STATUS, TSE_PHY_MDIO_STATUS_100BASE_X_HALF, 1); */
		/* alt_tse_phy_wr_mdio_reg_macspire(p_mac, 0, TSE_PHY_MDIO_ADV, TSE_PHY_MDIO_ADV_100BASE_TX_HALF, 1, cap); */
		/* tse_dprintf(6, "INFO: PHY - Advertisement of 100 Base-TX Half Duplex set to %d\n", cap); */
	}
	// else disable advertisement of this speed
	else {
		alt_tse_phy_wr_mdio_reg_macspire(p_mac, 0, TSE_PHY_MDIO_ADV, TSE_PHY_MDIO_ADV_100BASE_T4, 1, 0);
		tse_dprintf(6, "INFO: PHY - Advertisement of 100 Base-T4 set to %d\n",  0);

		alt_tse_phy_wr_mdio_reg_macspire(p_mac, 0, TSE_PHY_MDIO_ADV, TSE_PHY_MDIO_ADV_100BASE_TX_FULL, 1, 0);
		tse_dprintf(6, "INFO: PHY - Advertisement of 100 Base-TX Full Duplex set to %d\n",  0);

		/* alt_tse_phy_wr_mdio_reg_macspire(p_mac, 0, TSE_PHY_MDIO_ADV, TSE_PHY_MDIO_ADV_100BASE_TX_HALF, 1, 0); */
		/* tse_dprintf(6, "INFO: PHY - Advertisement of 100 Base-TX Half Duplex set to %d\n",  0); */
	}

	return SUCCESS;
}


//------------------------------------------------------------------------------
//! @brief Set the advertisement of PHY for 10 Mbps
//
//! @param[in] p_mac  : Pointer to alt_tse_phy_info structure
//! @param[in] enable : Set Enable = 1 to advertise this speed if the PHY capable
//!                     Set Enable = 0 to disable advertise of this speed
//
//! @retval SUCCESS
//------------------------------------------------------------------------------
static alt_32 alt_tse_phy_set_adv_10_macspire(np_tse_mac *p_mac, alt_u8 enable)
{
	alt_u8 cap;

	// if enable = 1, set advertisement based on PHY capability
	if (enable) {
		cap = alt_tse_phy_rd_mdio_reg_macspire(p_mac, 0, TSE_PHY_MDIO_STATUS, TSE_PHY_MDIO_STATUS_10BASE_T_FULL, 1);
		alt_tse_phy_wr_mdio_reg_macspire(p_mac, 0, TSE_PHY_MDIO_ADV, TSE_PHY_MDIO_ADV_10BASE_TX_FULL, 1, cap);
		tse_dprintf(6, "INFO: PHY - Advertisement of 10 Base-TX Full Duplex set to %d\n", cap);

		/* cap = alt_tse_phy_rd_mdio_reg_macspire(p_mac, 0, TSE_PHY_MDIO_STATUS, TSE_PHY_MDIO_STATUS_10BASE_T_HALF, 1); */
		/* alt_tse_phy_wr_mdio_reg_macspire(p_mac, 0, TSE_PHY_MDIO_ADV, TSE_PHY_MDIO_ADV_10BASE_TX_HALF, 1, cap); */
		/* tse_dprintf(6, "INFO: PHY - Advertisement of 10 Base-TX Half Duplex set to %d\n", cap); */
	}
	// else disable advertisement of this speed
	else {
		alt_tse_phy_wr_mdio_reg_macspire(p_mac, 0, TSE_PHY_MDIO_ADV, TSE_PHY_MDIO_ADV_10BASE_TX_FULL, 1, 0);
		tse_dprintf(6, "INFO: PHY - Advertisement of 10 Base-TX Full Duplex set to %d\n", 0);

		/* alt_tse_phy_wr_mdio_reg_macspire(p_mac, 0, TSE_PHY_MDIO_ADV, TSE_PHY_MDIO_ADV_10BASE_TX_HALF, 1, 0); */
		/* tse_dprintf(6, "INFO: PHY - Advertisement of 10 Base-TX Half Duplex set to %d\n", 0); */
	}

	return SUCCESS;
}

//------------------------------------------------------------------------------
//! @brief Restart Auto-Negotiation for the PHY
//
//! @param[in] p_mac             : Pointer to alt_tse_phy_info structure
//! @param[in] timeout_threshold : Timeout value of Auto-Negotiation
//
//! @retval TSE_PHY_AN_COMPLETE     : Success
//! @retval TSE_PHY_AN_NOT_COMPLETE : Auto-negotiation not completed
//! @retval TSE_PHY_AN_NOT_CAPABLE  : The PHY not capable for AN
//------------------------------------------------------------------------------
static alt_32 alt_tse_phy_restart_an_macspire(np_tse_mac *p_mac, alt_u32 timeout_threshold)
{
	alt_32 timeout = 0;
	alt_u16 ret;

	alt_u8 speed, duplex;
	/* unsigned int dat; */
	alt_32 dat;

	if (!alt_tse_phy_rd_mdio_reg_macspire(p_mac, 0, TSE_PHY_MDIO_STATUS, TSE_PHY_MDIO_STATUS_AN_ABILITY, 1)) {
		tse_dprintf(3, "WARN: PHY - PHY not capable for Auto-Negotiation\n");
		return TSE_PHY_AN_NOT_CAPABLE;
	}

	// enable Auto-Negotiation
	alt_tse_phy_wr_mdio_reg_macspire(p_mac, 0, TSE_PHY_MDIO_CONTROL, TSE_PHY_MDIO_CONTROL_AN_ENA, 1, 1);

	// reset PHY
	alt_tse_phy_wr_mdio_reg_macspire(p_mac, 0, TSE_PHY_MDIO_CONTROL, TSE_PHY_MDIO_CONTROL_RESET, 1, 1);

	// wait reset to de-assert
	while (alt_tse_phy_rd_mdio_reg_macspire(p_mac, 0, TSE_PHY_MDIO_CONTROL, TSE_PHY_MDIO_CONTROL_RESET, 1)) {
	}

	int i;

	// send PHY reset command
	alt_tse_phy_wr_mdio_reg_macspire(p_mac, 0, TSE_PHY_MDIO_CONTROL, TSE_PHY_MDIO_CONTROL_RESTART_AN, 1, 1);
	tse_dprintf(5, "INFO: PHY - Restart Auto-Negotiation, checking PHY link...\n");

	alt_tse_phy_wr_mdio_reg_macspire(p_mac, 0, TSE_PHY_MDIO_CONTROL, TSE_PHY_MDIO_CONTROL_RESET, 1, 1);
	// wait reset to de-assert
	while (alt_tse_phy_rd_mdio_reg_macspire(p_mac, 0, TSE_PHY_MDIO_CONTROL, TSE_PHY_MDIO_CONTROL_RESET, 1)) {
	}


	while (alt_tse_phy_rd_mdio_reg_macspire(p_mac, 0, TSE_PHY_MDIO_STATUS, TSE_PHY_MDIO_STATUS_AN_COMPLETE, 1) == 0 ) {
		if (timeout++ > timeout_threshold) {
			tse_dprintf(4, "WARN: PHY - Auto-Negotiation FAILED\n");

			for (i = 0; i < 0x20; i++) {
				printf("register %02d : 0x%08X \r\n", i, (unsigned int)alt_tse_phy_rd_mdio_reg_macspire(p_mac, 0, i, 0, 16));
			}

			return TSE_PHY_AN_NOT_COMPLETE;
		}
	}
	tse_dprintf(5, "INFO: PHY - Auto-Negotiation PASSED\n");

	ret = alt_tse_phy_rd_mdio_reg_macspire(p_mac, 0, PHY_SPECIFIC_STATUS_REG, 13, 3); //check the "Speed-Select, Duplex-Mode Status"
	speed = ret >>1 ;
	duplex = ret & 0x001 ;

	dat = IORD_ALTERA_TSEMAC_CMD_CONFIG(p_mac);

	switch (speed) {
	case LINK_10M:
			IOWR_ALTERA_TSEMAC_CMD_CONFIG(p_mac, dat | mmac_cc_ENA_10_mask | mmac_cc_RX_ENA_mask | mmac_cc_TX_ENA_mask );
			printf("Speed Select: 10 Mbps\n");
			break;

	case LINK_100M:
			IOWR_ALTERA_TSEMAC_CMD_CONFIG(p_mac, dat | mmac_cc_RX_ENA_mask | mmac_cc_TX_ENA_mask );
			printf("Speed Select: 100 Mbps\n");
			break;

	case LINK_1000M:
			IOWR_ALTERA_TSEMAC_CMD_CONFIG(p_mac, dat | mmac_cc_ETH_SPEED_mask | mmac_cc_RX_ENA_mask | mmac_cc_TX_ENA_mask );
			printf("Speed Select: 1000 Mbps\n");
			break;
	default:
			IOWR_ALTERA_TSEMAC_CMD_CONFIG(p_mac, dat | mmac_cc_ETH_SPEED_mask | mmac_cc_RX_ENA_mask | mmac_cc_TX_ENA_mask );
			printf("Speed Select: ? Mbps\n");
			break;
	}

	switch (duplex) {
	case FULL_DUPLEX:
			printf("Duplex Mode: Full Duplex\n");
			printf("Ethernet MAC INIT: End\n");
			break;
	case HALF_DUPLEX:
			printf("Duplex Mode: Half Duplex\n");
			printf("Ethernet MAC INIT: End\n");
			break;
	default:
			printf("Duplex Mode: ?\n");
			printf("Ethernet MAC INIT: End\n");
			break;
	}

	return TSE_PHY_AN_COMPLETE;
}

//------------------------------------------------------------------------------
//! @brief Check link up/down
//
//! @param[in] p_mac             : Pointer to alt_tse_phy_info structure
//
//! @retval 1 : Link up
//! @retval 0 : Link down
//------------------------------------------------------------------------------
alt_32 alt_tse_phy_check_link_up_down(void)
{
	/* if (alt_tse_phy_rd_mdio_reg_macspire((np_tse_mac *)EMAC_BASE, 0, 0x13, 0, 16) & (1 << 1)) { return 1; } // KSZ9031 */
	if (alt_tse_phy_rd_mdio_reg_macspire((np_tse_mac *)EMAC_BASE, 0, PHY_SPECIFIC_STATUS_REG, 0, 16) & (1 << LINK_STATUS_BIT)) { return 1; } // MV88E1111, MV88E1512, DP83867IR

	else { return 0; }
}
