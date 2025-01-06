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
//! @brief  Main routine of firmware.
//=====================================================================================

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "../../libgev/include/gev_api.h"
#include "../../main_bsp/HAL/inc/alt_types.h"
#include "../../main_bsp/HAL/inc/io.h"
#include "../../main_bsp/HAL/inc/sys/alt_cache.h"
#include "../../main_bsp/HAL/inc/sys/alt_flash.h"
#include "../../main_bsp/drivers/inc/altera_avalon_pio_regs.h"
#include "../../main_bsp/system.h"
#include "flash_mem_map.h"
#include "tse_mac_phy.h"
#include "xml_file_v12.h"
#include "xml_file_v20.h"


//------------------------------------------------------------------------------
// define and structures
//------------------------------------------------------------------------------
/* #define TEST_GEV_VERSION_20 1 */

#define GVBS_SENSOR_WIDTH_X_OFFSET                      0x0000C000
#define GVBS_SENSOR_HEIGHT_Y_OFFSET                     0x0000C004

#define GVBS_TIMESTAMP_SELECT_OFFSET                    0x0000C010
#define GVBS_PTP_SET_PRIORITY1_OFFSET                   0x0000C020
#define GVBS_PTP_SET_PRIORITY2_OFFSET                   0x0000C024
#define GVBS_PTP_SET_DOMAIN_NUMBER_OFFSET               0x0000C028
#define GVBS_PTP_SET_SLAVE_ONLY_OFFSET                  0x0000C02C
#define GVBS_PTP_SET_LOG_ANNOUNCE_INTERVAL_OFFSET       0x0000C030
#define GVBS_PTP_SET_LOG_SYNC_INTERVAL_OFFSET           0x0000C034
#define GVBS_PTP_SET_LOG_MIN_DELAY_REQ_INTERVAL_OFFSET  0x0000C038
#define GVBS_PTP_SET_ANNOUNCE_RECEIPT_TIMEOUT_OFFSET    0x0000C03C
#define GVBS_SET_TIMESTAMP_INITIAL_VALUE_OFFSET         0x0000C040
#define GVBS_TIMESTAMP_INITIAL_VALUE_HIGH_OFFSET        0x0000C044
#define GVBS_TIMESTAMP_INITIAL_VALUE_LOW_OFFSET         0x0000C048
#define GVBS_ACCEPT_BROADCAST_WRITEREG_OFFSET           0x0000C050  // Not on the GenICam XML list (GEV BSR Only)

#define GVBS_DEVICE_RESET_OFFSET                        0x0000D000

#define GEV_TIMESTAMP_FREQ_HIGH_HZ                      0
/* #define GEV_TIMESTAMP_FREQ_LOW_HZ                       50000000 */
#define GEV_TIMESTAMP_FREQ_LOW_HZ                       100000000

#define GEV_TX_CLK_FREQ                                 100000000
#define GEV_AVM_CLK_FREQ                                100000000

#define ROUND_OFF(x, unit) (((x) / (unit)) * (unit))


//! data parameter definition  36 byte
typedef struct data_param {
	u32 sensor_width;  //!< sensor width
	u32 sensor_height; //!< sensor height
	u32 width;         //!< image width to transport
	u32 height;        //!< image height to transport
	u32 offset_x;      //!< image horizontal offset from sensor start position
	u32 offset_y;      //!< image vertical offset from sensor start position
	u32 pixel_format;  //!< pixel format
	u32 payload_type;  //!< payload type
	u32 field_count;   //!< field count (0: progressive 1:two fields interlace)
} data_param_t, *p_data_param_t;

//! timestamp setting value
typedef struct set_time {
	u32 low;
	u32 high;
} set_time_t, *p_set_time_t;

//------------------------------------------------------------------------------
// prototypes
//------------------------------------------------------------------------------
void user_task_2(void);
void user_task_1(void);
void idle_task(void);

static int width_write(u32 arg, u32 *p_data);
static int height_write(u32 arg, u32 *p_data);
static int offset_x_write(u32 arg, u32 *p_data);
static int offset_y_write(u32 arg, u32 *p_data);
static int pixel_format_write(u32 arg, u32 *p_data);
static int payload_type_write(u32 arg, u32 *p_data);
static int acquisition_start(u32 arg, u32 *p_data);
static int acquisition_stop(u32 arg, u32 *p_data);
static int field_count_write(u32 arg, u32 *p_data);

static int width_max_write(u32 arg, u32 *p_data);
static int width_max_read(u32 arg, u32 *p_data);
static int height_max_write(u32 arg, u32 *p_data);
static int height_max_read(u32 arg, u32 *p_data);

static void event_observe(void);
static int read_mem_xml(u32 arg, u32 ofst, u32 size, u8 *p_data);

static void* flash_open_dev(const char* name);
static void flash_close_dev(void *p_dev);
static int flash_write_dev(void* p_dev, int offset, const void* p_src, int len);
static int flash_read_dev(void* p_dev, int offset, void* p_dst, int len);

static int ipconf_done(u32 arg, u32 *p_data);

static void act_cmd_0(u32 arg);
static void act_cmd_1(u32 arg);
static void act_cmd_2(u32 arg);
static void act_cmd_3(u32 arg);
static void act_cmd_4(u32 arg);
static void act_cmd_5(u32 arg);
static void act_cmd_6(u32 arg);
static void act_cmd_7(u32 arg);

#if defined(PTP_SUPPORT)
static int write_ptp_set_domain_number(u32 arg, u32 *p_data);
static int read_ptp_set_domain_number(u32 arg, u32 *p_data);
#if !defined(PTP_SLAVE_ONLY)
static int write_ptp_set_priority1(u32 arg, u32 *p_data);
static int read_ptp_set_priority1(u32 arg, u32 *p_data);
static int write_ptp_set_priority2(u32 arg, u32 *p_data);
static int read_ptp_set_priority2(u32 arg, u32 *p_data);
static int write_ptp_set_slave_only(u32 arg, u32 *p_data);
static int read_ptp_set_slave_only(u32 arg, u32 *p_data);
static int write_ptp_set_log_announce_interval(u32 arg, u32 *p_data);
static int read_ptp_set_log_announce_interval(u32 arg, u32 *p_data);
static int write_ptp_set_log_sync_interval(u32 arg, u32 *p_data);
static int read_ptp_set_log_sync_interval(u32 arg, u32 *p_data);
static int write_ptp_set_log_min_delay_req_interval(u32 arg, u32 *p_data);
static int read_ptp_set_log_min_delay_req_interval(u32 arg, u32 *p_data);
static int write_ptp_set_announce_receipt_timeout(u32 arg, u32 *p_data);
static int read_ptp_set_announce_receipt_timeout(u32 arg, u32 *p_data);
#endif  // !defined(PTP_SLAVE_ONLY)
#endif  // defined(PTP_SUPPORT)

static int write_timestamp_sel(u32 arg, u32 *p_data);
static int read_timestamp_sel(u32 arg, u32 *p_data);

static int write_set_timestamp_initial_value(u32 arg, u32 *p_data);
static int write_timestamp_initial_value_high(u32 arg, u32 *p_data);
static int write_timestamp_initial_value_low(u32 arg, u32 *p_data);
static int write_accept_broadcast_writereg(u32 arg, u32 *p_data);
static int read_accept_broadcast_writereg(u32 arg, u32 *p_data);
static int write_device_reset(u32 arg, u32 *p_data);

//------------------------------------------------------------------------------
// global variables
//------------------------------------------------------------------------------
GEV_HANDLE g_handle;
data_param_t g_data_param;
bool g_gvsp_active = FALSE;
int g_test_version = GEV_VERSION_20;
// int g_test_version = GEV_VERSION_12;
bool g_link_status;

#if defined(PTP_SUPPORT)
ptp_param_t g_ptp_param;
#endif  // defined(PTP_SUPPORT)

set_time_t g_set_time;
bool g_accept_broadcast_writereg = FALSE;

//------------------------------------------------------------------------------
//! @brief Main routine
//
//! @retval  0 : Success
//! @retval -1 : Error
//------------------------------------------------------------------------------
int main(void)
{
	// start RTOS scheduler
	start_scheduler();

	while (1) {

	}

	return (0);
}

//------------------------------------------------------------------------------
//! @brief Task 3 function.
//
//! @return None
//------------------------------------------------------------------------------
void idle_task(void)
{
	int ret;
	u8 task_id;

	ret = get_task_id_in_entry(user_task_2, &task_id);
	if (ret != 0) { printf("ERROR : Can't get task ID!\r\n"); }

	while (1) {
		// check event
		event_observe();

		//
		// Fix me!
		// When Ethernet link down, you should do this in interrupt
		// or high priority task.
		//
		if (g_link_status == true) {
			if (alt_tse_phy_check_link_up_down() == 0) {
				g_link_status = false;
				wakeup_task(task_id);
			}
		}

		// sleep
		usleep(10000);
	}
}

//------------------------------------------------------------------------------
//! @brief Observe event
//
//! @return None
//------------------------------------------------------------------------------
static void event_observe(void)
{
	unsigned char pio_data;
	u8 task_id;
	int ret;

	ret = get_task_id_in_entry(user_task_2, &task_id);
	if (ret != 0) { printf("ERROR : Can't get task ID!\r\n"); }

	// get button status
	pio_data = IORD_ALTERA_AVALON_PIO_DATA(GEV_PIO_I_PUSH_SW_BASE);

	// check USER_PB3 button
//	if ((pio_data & 0x80) == 0) {
	if ((pio_data & 0x1) == 0) {
		wakeup_task(task_id);
		usleep(1000000);
	}

}

//------------------------------------------------------------------------------
//! @brief User task 1
//
//! @return None
//------------------------------------------------------------------------------
void user_task_1(void)
{
	gev_err_t ret;
	u8 mac_addr[MAC_SIZE];
	u32 frm_mem_rng_start_addr;
	u32 frm_mem_rng_size;
	gev_frm_mem_param_t frm_mem_param;
	u8 major, minor, sub;
	gev_ip_info_t hw_ip_info;

	gev_img_param_t image_param;
	ptp_param_t init_ptp_param;

	unsigned char manufacture_name[] = "Macnica, Inc.";
	unsigned char model_name[] = "CVGT DevKit 1GEV w/ IEEE1588";
	unsigned char device_version[] = "2021.04.02";
	unsigned char manufacture_info[] = "https://www.m-pression.com";
	unsigned char serial_number[] = "0123456789ABCDE";
	gev_bs_reg_t *p_bs_reg;
	gev_bs_reg_mem_t *p_bs_reg_mem;

	int i;
	flash_dev_func_t flash_func;
	unsigned char pio_data;
	manifest_table_t manifest_table_data;
	flash_mem_offset_t flash_mem_offset_adder;

	pio_data = IORD_ALTERA_AVALON_PIO_DATA(GEV_PIO_I_DIP_SW_BASE);
	// check USER_DIP0 switch
	if ((pio_data & 0x1) == 0) { g_test_version = GEV_VERSION_12; }

	// check USER_DIP1 switch
	if ((pio_data & 0x2) == 0) { image_param.payload_type = GVSP_PAYLOAD_TYPE_RAW_DATA_BINARY; }
	else                        { image_param.payload_type = GVSP_PAYLOAD_TYPE_IMAGE_DATA_UNCOMPRESSED; }

	p_bs_reg = malloc(sizeof(gev_bs_reg_t) * 0x10);
	p_bs_reg_mem = malloc(sizeof(gev_bs_reg_mem_t) * 0x200);

	// clear image data parameter
	memset((char *)&g_data_param, 0x00, sizeof(data_param_t));

	while (1) {
		// initialize MAC and PHY
		while (1) {
			ret = init_tse_mac_and_phy(mac_addr);
			if (ret == 0) { g_link_status = true; break; }
		}

		/* hw_ip_info.fpga_register_base           = FPGA_REGISTER_BASE; */
		hw_ip_info.fpga_register_base           = GEV_HWIP_AVS0_FPGA_REG_BASE;
		hw_ip_info.fpga_register_irq            = GEV_HWIP_AVS0_FPGA_REG_IRQ;
		hw_ip_info.fpga_register_irq_cntl_id    = GEV_HWIP_AVS0_FPGA_REG_IRQ_INTERRUPT_CONTROLLER_ID;

		/* hw_ip_info.receive_packet_mem_base      = RECEIVE_PACKET_MEM_BASE; */
		hw_ip_info.receive_packet_mem_base      = GEV_HWIP_AVS3_RX_PACKET_BASE;

		/* hw_ip_info.transmitter_chunk_mem_base   = TRANSMITTER_CHUNK_MEM_BASE; */
		hw_ip_info.transmitter_chunk_mem_base   = GEV_HWIP_AVS4_TX_CHUNK_BASE;

		/* hw_ip_info.transmitter_gvcp_mem_base    = TRANSMITTER_GVCP_MEM_BASE; */
		hw_ip_info.transmitter_gvcp_mem_base    = GEV_HWIP_AVS2_TX_GVCP_BASE;

		/* hw_ip_info.transmitter_gvsp_mem_base    = TRANSMITTER_GVSP_MEM_BASE; */
		hw_ip_info.transmitter_gvsp_mem_base    = GEV_HWIP_AVS1_TX_GVSP_BASE;

		hw_ip_info.timer_base                   = TIMER_GEV_BASE;
		hw_ip_info.timer_irq                    = TIMER_GEV_IRQ;
		hw_ip_info.timer_irq_cntl_id            = TIMER_GEV_IRQ_INTERRUPT_CONTROLLER_ID;

		hw_ip_info.avalon_st_clock_frequency    = GEV_TX_CLK_FREQ;
		hw_ip_info.frm_ctl_mem_clk_frequency    = GEV_AVM_CLK_FREQ;

		hw_ip_info.tse_mac_base                 = EMAC_BASE;

		hw_ip_info.ptp_clock_base               = PTP_CLOCK_HWIP_BASE;
		hw_ip_info.ptp_clock_irq_cntl_id        = PTP_CLOCK_HWIP_IRQ_INTERRUPT_CONTROLLER_ID;
		hw_ip_info.ptp_clock_irq                = PTP_CLOCK_HWIP_IRQ;

		hw_ip_info.ptp_ts_cap_csr_base          = PTP_TS_CAP_HWIP_CSR_BASE;
		hw_ip_info.ptp_ts_cap_csr_irq_cntl_id   = PTP_TS_CAP_HWIP_CSR_IRQ_INTERRUPT_CONTROLLER_ID;
		hw_ip_info.ptp_ts_cap_csr_irq           = PTP_TS_CAP_HWIP_CSR_IRQ;
		hw_ip_info.ptp_ts_cap_mem_base          = PTP_TS_CAP_HWIP_MEM_BASE;
		hw_ip_info.ptp_ts_cap_mem_irq_cntl_id   = PTP_TS_CAP_HWIP_MEM_IRQ_INTERRUPT_CONTROLLER_ID;
		hw_ip_info.ptp_ts_cap_mem_irq           = PTP_TS_CAP_HWIP_MEM_IRQ;

		// 640x480@60Hz(Industry Standard), Freq.25.175MHz
		/* image_param.img_width     = 640; */
		/* image_param.img_height    = 480; */

		// 800x600@60Hz(VESA Guidelines), Freq.40MHz
		// 800x600@72Hz(VESA Guidelines), Freq.50MHz
		image_param.img_width     = 800;
		image_param.img_height    = 600;

		// 848x480@60Hz(VESA Standard), Freq.33.750MHz
		/* image_param.img_width     = 848; */
		/* image_param.img_height    = 480; */

		// 1280x720@60Hz(CEA Standard), Freq.74.25MHz
		/* image_param.img_width     = 1280; */
		/* image_param.img_height    = 720; */

		image_param.x_ofst        = 0;
		image_param.y_ofst        = 0;
		image_param.pixel_format  = GVSP_PIX_RGB8;
		image_param.raw_data_size = 0x00100000;

		/* g_data_param.sensor_width = 1280; */
		/* g_data_param.sensor_height = 1752;   // dummy blank = 1752-720 */
		g_data_param.sensor_width = image_param.img_width;
		g_data_param.sensor_height = image_param.img_height;

		g_data_param.width = image_param.img_width;
		g_data_param.height = image_param.img_height;

		g_data_param.offset_x = image_param.x_ofst;
		g_data_param.offset_y = image_param.y_ofst;

		g_data_param.pixel_format = image_param.pixel_format;
		g_data_param.payload_type = image_param.payload_type;

		// open GEV engine
		if (g_test_version == GEV_VERSION_20) {
			printf("GEV Specification: version 2.0 \r\n");
			g_handle = gev_open(GEV_VERSION_20, hw_ip_info, image_param);
		}
		else  {
			printf("GEV Specification: version 1.2 \r\n");
			g_handle = gev_open(GEV_VERSION_12, hw_ip_info, image_param);
		}

		// get library version
		ret = gev_get_version(g_handle, &major, &minor, &sub);
		if (ret != GEV_ERR_SUCCESS)  { break; }
		printf("GEV Library: Ver. %d.%d.%d\r\n", major, minor, sub);

		// XML file size
		if (g_test_version == GEV_VERSION_20) {
			i = strlen((char *)g_xml_file_v20_p1);

			printf("XML file size: %d bytes \r\n", i);
		}
		else  {
			i = strlen((char *)g_xml_file_v12_p1);

			printf("XML file size: %d bytes \r\n", i);
		}

		// set Bootstrap register callback functions
		ret = gev_reg_bs_reg_width_write_func(g_handle, width_write, 0);
		if (ret != GEV_ERR_SUCCESS)  { break; }
		ret = gev_reg_bs_reg_height_write_func(g_handle, height_write, 0);
		if (ret != GEV_ERR_SUCCESS)  { break; }
		ret = gev_reg_bs_reg_ofstx_write_func(g_handle, offset_x_write, 0);
		if (ret != GEV_ERR_SUCCESS)  { break; }
		ret = gev_reg_bs_reg_ofsty_write_func(g_handle, offset_y_write, 0);
		if (ret != GEV_ERR_SUCCESS)  { break; }
		ret = gev_reg_bs_reg_acquisition_start_func(g_handle, acquisition_start, 0);
		if (ret != GEV_ERR_SUCCESS)  { break; }
		ret = gev_reg_bs_reg_acquisition_stop_func(g_handle, acquisition_stop, 0);
		if (ret != GEV_ERR_SUCCESS)  { break; }
		ret = gev_reg_bs_reg_payload_type_write_func(g_handle, payload_type_write, 0);
		if (ret != GEV_ERR_SUCCESS)  { break; }
		ret = gev_reg_bs_reg_pixel_fromat_write_func(g_handle, pixel_format_write, 0);
		if (ret != GEV_ERR_SUCCESS)  { break; }
		ret = gev_reg_bs_reg_field_count_write_func(g_handle, field_count_write, 0);
		if (ret != GEV_ERR_SUCCESS)  { break; }

		// set flash support information
		flash_func.dev_open  = flash_open_dev;
		flash_func.dev_close = flash_close_dev;
		flash_func.dev_write = flash_write_dev;
		flash_func.dev_read  = flash_read_dev;

		flash_mem_offset_adder.current_ip_configuration     = FLASH_OFFSET_CURRENT_IP_CONFIGURATION;
		flash_mem_offset_adder.user_defined_name            = FLASH_OFFSET_USER_DEFINED_NAME;
		flash_mem_offset_adder.persistent_ip_address_0      = FLASH_OFFSET_PERSISTENT_IP_ADDRESS_0;
		flash_mem_offset_adder.persistent_subnet_mask_0     = FLASH_OFFSET_PERSISTENT_SUBNET_MASK_0;
		flash_mem_offset_adder.persistent_gateway_0         = FLASH_OFFSET_PERSISTENT_GATEWAY_0;
		flash_mem_offset_adder.gvcp_configuration           = FLASH_OFFSET_GVCP_CONFIGURATION;

#if 1 // Non-volatile memory supported
		ret = gev_set_flash_param(g_handle, TRUE, SF_CTR_AVL_MEM_BASE, SF_CTR_AVL_MEM_NAME, &flash_func, &flash_mem_offset_adder);
		if (ret != GEV_ERR_SUCCESS)  { break; }

		// set bootstrap Network Interface Capability register
		ret = gev_set_bs_ntwk_if_cap(g_handle,
					 GVBS_IP_CONFIGURATION_PERSISTENT |
					 GVBS_IP_CONFIGURATION_DHCP       |
					 GVBS_IP_CONFIGURATION_LLA);
		if (ret != GEV_ERR_SUCCESS)  { break; }
#else
		ret = gev_set_flash_param(g_handle, FALSE, 0, NULL, NULL);
		if (ret != GEV_ERR_SUCCESS)  { break; }

		ret = gev_set_bs_ntwk_if_cap(g_handle,
					 GVBS_IP_CONFIGURATION_DHCP       |
					 GVBS_IP_CONFIGURATION_LLA);
		if (ret != GEV_ERR_SUCCESS)  { break; }
#endif

		if (g_test_version == GEV_VERSION_20) {
			// set bootstrap device mode register
			ret = gev_set_bs_dev_mode(g_handle,
						 (1 << GVBS_DEVICE_MODE_ENDIAN_POS) |
						 GVBS_DEVICE_MODE_CHARACTER_SET_UTF8);  // because validation tool does not support devices which use ASCIIcharacter sets
			if (ret != GEV_ERR_SUCCESS)  { break; }
		}

#if defined(PTP_SUPPORT)
		if (g_test_version == GEV_VERSION_20) {
			// GVSP capability register
			ret = gev_set_bs_gvsp_cap(g_handle, GVBS_GVSP_CAPABILITY_SUPPORT_SCSP | GVBS_GVSP_CAPABILITY_SUPPORT_16BIT);
			if (ret != GEV_ERR_SUCCESS)  { break; }

			// GVCP capability register
			ret = gev_set_bs_gvcp_cap(g_handle,
									  GVBS_GVCP_CAPABILITY_USER_DEFINED_NAME |
									  GVBS_GVCP_CAPABILITY_SERIAL_NUMBER |
									  GVBS_GVCP_CAPABILITY_CCP_APPLICATION_PORT_AND_IP |
									  GVBS_GVCP_CAPABILITY_MANIFEST_TABLE |
									  GVBS_GVCP_CAPABILITY_EXTENSTION_STATUS |
									  GVBS_GVCP_CAPABILITY_IEEE1588 |
									  GVBS_GVCP_CAPABILITY_ES2 |
									  GVBS_GVCP_CAPABILITY_SCHEDULED_ACTION |
									  GVBS_GVCP_CAPABILITY_ACTION |
									  GVBS_GVCP_CAPABILITY_PENDING |
									  GVBS_GVCP_CAPABILITY_EVENT_DATA |
									  GVBS_GVCP_CAPABILITY_EVENT |
									  GVBS_GVCP_CAPABILITY_PACKET_RESEND |
									  GVBS_GVCP_CAPABILITY_WRITE_MEMORY |
									  GVBS_GVCP_CAPABILITY_CONCATENATION);
			if (ret != GEV_ERR_SUCCESS)  { break; }

			// stream channel capability register
			ret = gev_set_bs_strm_ch_cap(g_handle, GVBS_SCC_PACKET_RESEND_DESTINATION_OPTION | GVBS_SCC_BIG_ENDIAN);
			if (ret != GEV_ERR_SUCCESS)  { break; }
		}
		else {
			// GVCP capability register
			ret = gev_set_bs_gvcp_cap(g_handle,
									  GVBS_GVCP_CAPABILITY_USER_DEFINED_NAME |
									  GVBS_GVCP_CAPABILITY_SERIAL_NUMBER |
									  GVBS_GVCP_CAPABILITY_CCP_APPLICATION_PORT_AND_IP |
									  GVBS_GVCP_CAPABILITY_MANIFEST_TABLE |
									  GVBS_GVCP_CAPABILITY_EXTENSTION_STATUS |
									  GVBS_GVCP_CAPABILITY_ACTION |
									  GVBS_GVCP_CAPABILITY_PENDING |
									  GVBS_GVCP_CAPABILITY_EVENT_DATA |
									  GVBS_GVCP_CAPABILITY_EVENT |
									  GVBS_GVCP_CAPABILITY_PACKET_RESEND |
									  GVBS_GVCP_CAPABILITY_WRITE_MEMORY |
									  GVBS_GVCP_CAPABILITY_CONCATENATION);
			if (ret != GEV_ERR_SUCCESS)  { break; }

			// stream channel capability register
			ret = gev_set_bs_strm_ch_cap(g_handle, GVBS_SCC_EXTENDED_CHUNK_DATA | GVBS_SCC_BIG_ENDIAN);
			if (ret != GEV_ERR_SUCCESS)  { break; }
		}
#else
		if (g_test_version == GEV_VERSION_20) {
			// GVSP capability register
			ret = gev_set_bs_gvsp_cap(g_handle, GVBS_GVSP_CAPABILITY_SUPPORT_SCSP | GVBS_GVSP_CAPABILITY_SUPPORT_16BIT);
			if (ret != GEV_ERR_SUCCESS)  { break; }

			// GVCP capability register
			ret = gev_set_bs_gvcp_cap(g_handle,
									  GVBS_GVCP_CAPABILITY_USER_DEFINED_NAME |
									  GVBS_GVCP_CAPABILITY_SERIAL_NUMBER |
									  GVBS_GVCP_CAPABILITY_CCP_APPLICATION_PORT_AND_IP |
									  GVBS_GVCP_CAPABILITY_MANIFEST_TABLE |
									  GVBS_GVCP_CAPABILITY_EXTENSTION_STATUS |
									  GVBS_GVCP_CAPABILITY_ES2 |
									  GVBS_GVCP_CAPABILITY_ACTION |
									  GVBS_GVCP_CAPABILITY_PENDING |
									  GVBS_GVCP_CAPABILITY_EVENT_DATA |
									  GVBS_GVCP_CAPABILITY_EVENT |
									  GVBS_GVCP_CAPABILITY_PACKET_RESEND |
									  GVBS_GVCP_CAPABILITY_WRITE_MEMORY |
									  GVBS_GVCP_CAPABILITY_CONCATENATION);
			if (ret != GEV_ERR_SUCCESS)  { break; }

			// stream channel capability register
			ret = gev_set_bs_strm_ch_cap(g_handle, GVBS_SCC_PACKET_RESEND_DESTINATION_OPTION | GVBS_SCC_BIG_ENDIAN);
			if (ret != GEV_ERR_SUCCESS)  { break; }
		}
		else {
			// GVCP capability register
			ret = gev_set_bs_gvcp_cap(g_handle,
									  GVBS_GVCP_CAPABILITY_USER_DEFINED_NAME |
									  GVBS_GVCP_CAPABILITY_SERIAL_NUMBER |
									  GVBS_GVCP_CAPABILITY_CCP_APPLICATION_PORT_AND_IP |
									  GVBS_GVCP_CAPABILITY_MANIFEST_TABLE |
									  GVBS_GVCP_CAPABILITY_EXTENSTION_STATUS |
									  GVBS_GVCP_CAPABILITY_ACTION |
									  GVBS_GVCP_CAPABILITY_PENDING |
									  GVBS_GVCP_CAPABILITY_EVENT_DATA |
									  GVBS_GVCP_CAPABILITY_EVENT |
									  GVBS_GVCP_CAPABILITY_PACKET_RESEND |
									  GVBS_GVCP_CAPABILITY_WRITE_MEMORY |
									  GVBS_GVCP_CAPABILITY_CONCATENATION);
			if (ret != GEV_ERR_SUCCESS)  { break; }

			// stream channel capability register
			ret = gev_set_bs_strm_ch_cap(g_handle, GVBS_SCC_EXTENDED_CHUNK_DATA | GVBS_SCC_BIG_ENDIAN);
			if (ret != GEV_ERR_SUCCESS)  { break; }
		}
#endif  // defined(PTP_SUPPORT)

		// manifest table
		memset((char *)&manifest_table_data, 0x00, sizeof(manifest_table_data));
		manifest_table_data.header.header_bit_field.num_entries = 0;

		manifest_table_data.entry[0].entry_bit_field.xml_major_version = 3;
		manifest_table_data.entry[0].entry_bit_field.xml_minor_version = 0;
		manifest_table_data.entry[0].entry_bit_field.xml_sub_minor_version = 0;
		manifest_table_data.entry[0].entry_bit_field.schema_major_version = 1;
		manifest_table_data.entry[0].entry_bit_field.schema_minor_version = 1;

		manifest_table_data.entry[0].url_register_address = 0x00000200;

		ret = gev_set_bs_mnfest_tbl(g_handle, (unsigned char *)&manifest_table_data, sizeof(manifest_table_data));
		if (ret != GEV_ERR_SUCCESS)  { break; }

		if (g_test_version == GEV_VERSION_20) {
			// first URL
			ret = gev_set_bs_fst_url(g_handle, (unsigned char *)g_first_url_v20, strlen((const char *)g_first_url_v20));
			if (ret != GEV_ERR_SUCCESS)  { break; }

			// second URL
			ret = gev_set_bs_scnd_url(g_handle, (unsigned char *)g_second_url_v20, strlen((const char *)g_second_url_v20));
			if (ret != GEV_ERR_SUCCESS)  { break; }
		}
		else  {
			// first URL
			ret = gev_set_bs_fst_url(g_handle, (unsigned char *)g_first_url_v12, strlen((const char *)g_first_url_v12));
			if (ret != GEV_ERR_SUCCESS)  { break; }

			// second URL
			ret = gev_set_bs_scnd_url(g_handle, (unsigned char *)g_second_url_v12, strlen((const char *)g_second_url_v12));
			if (ret != GEV_ERR_SUCCESS)  { break; }
		}

		// manufacturer name
		ret = gev_set_bs_mnft_name(g_handle, manufacture_name, strlen((const char *)manufacture_name));
		if (ret != GEV_ERR_SUCCESS)  { break; }

		// model name
		ret = gev_set_bs_model_name(g_handle, model_name, strlen((const char *)model_name));
		if (ret != GEV_ERR_SUCCESS)  { break; }

		// device version
		ret = gev_set_bs_dev_ver(g_handle, device_version, strlen((const char *)device_version));
		if (ret != GEV_ERR_SUCCESS)  { break; }

		// manufacturer informations
		ret = gev_set_bs_mnft_info(g_handle, manufacture_info, strlen((const char *)manufacture_info));
		if (ret != GEV_ERR_SUCCESS)  { break; }

		// serial number
		ret = gev_set_bs_seri_num(g_handle, serial_number, 15);
		if (ret != GEV_ERR_SUCCESS)  { break; }

		// pending timeout unit : ms
		ret = gev_set_bs_pend_tmout(g_handle, 3000);
		if (ret != GEV_ERR_SUCCESS)  { break; }

		// message channel source port
		ret = gev_set_bs_msg_ch_src_port(g_handle, 49152);
		if (ret != GEV_ERR_SUCCESS)  { break; }

		// stream channel source port
		ret = gev_set_bs_strm_ch_src_port(g_handle, 49153);
		if (ret != GEV_ERR_SUCCESS)  { break; }

		// set action command function
		ret = gev_reg_act_func(g_handle, 0, act_cmd_0, 100);
		ret = gev_reg_act_func(g_handle, 1, act_cmd_1, 101);
		ret = gev_reg_act_func(g_handle, 2, act_cmd_2, 102);
		ret = gev_reg_act_func(g_handle, 3, act_cmd_3, 103);
		ret = gev_reg_act_func(g_handle, 4, act_cmd_4, 104);
		ret = gev_reg_act_func(g_handle, 5, act_cmd_5, 105);
		ret = gev_reg_act_func(g_handle, 6, act_cmd_6, 106);
		ret = gev_reg_act_func(g_handle, 7, act_cmd_7, 107);

		// set MAC address
		ret = gev_set_bs_dev_mac_addr(g_handle, mac_addr);
		if (ret != GEV_ERR_SUCCESS) { gev_close(g_handle); break; }

		// set frame memory parameters
		frm_mem_rng_size                  = 0x08000000;

	//	frm_mem_rng_start_addr            = 0x18000000;
		frm_mem_rng_start_addr            = EMIF_BASE + frm_mem_rng_size ;

		frm_mem_param.frm_start_addr_ofst = 0;
		frm_mem_param.frm_ofst            = 0x00600000;
		frm_mem_param.frm_num             = 6 + 2; // FRM_RESND_HOLD_MEM = 2
//		frm_mem_param.frm_num             = 1 + 2; // FRM_RESND_HOLD_MEM = 2
		frm_mem_param.frm_read_dly        = 0x00000400;

		ret = gev_set_frm_mem_param(g_handle, frm_mem_rng_start_addr, frm_mem_rng_size, &frm_mem_param);
		if (ret != GEV_ERR_SUCCESS) { gev_close(g_handle); break; }

		// set user extended bootstrap register
		ret = gev_set_ext_bs_reg_num(g_handle, 0x1000, 0x10, 0x100);
		if (ret != GEV_ERR_SUCCESS)  { break; }

// ========================================================================== //

		p_bs_reg[0].addr       = GVBS_SENSOR_WIDTH_X_OFFSET;
		p_bs_reg[0].writable   = TRUE;
		p_bs_reg[0].write_func = width_max_write;
		p_bs_reg[0].write_arg  = 0;
		p_bs_reg[0].readable   = TRUE;
		p_bs_reg[0].read_func  = width_max_read;
		p_bs_reg[0].read_arg   = 0;

		p_bs_reg[1].addr       = GVBS_SENSOR_HEIGHT_Y_OFFSET;
		p_bs_reg[1].writable   = TRUE;
		p_bs_reg[1].write_func = height_max_write;
		p_bs_reg[1].write_arg  = 0;
		p_bs_reg[1].readable   = TRUE;
		p_bs_reg[1].read_func  = height_max_read;
		p_bs_reg[1].read_arg   = 0;

		// add bootstrap register
		ret = gev_add_bs_reg(g_handle, p_bs_reg, 2);
		if (ret != GEV_ERR_SUCCESS)  { break; }

// ========================================================================== //

		p_bs_reg[0].addr       = GVBS_TIMESTAMP_SELECT_OFFSET;
		p_bs_reg[0].writable   = TRUE;
		p_bs_reg[0].write_func = write_timestamp_sel;
		p_bs_reg[0].write_arg  = 0;
		p_bs_reg[0].readable   = TRUE;
		p_bs_reg[0].read_func  = read_timestamp_sel;
		p_bs_reg[0].read_arg   = 0;

		// add bootstrap register
		ret = gev_add_bs_reg(g_handle, p_bs_reg, 1);
		if (ret != GEV_ERR_SUCCESS)  { break; }

// ========================================================================== //

#if defined(PTP_SUPPORT)
#if defined(PTP_SLAVE_ONLY)
		p_bs_reg[0].addr       = GVBS_PTP_SET_DOMAIN_NUMBER_OFFSET;
		p_bs_reg[0].writable   = TRUE;
		p_bs_reg[0].write_func = write_ptp_set_domain_number;
		p_bs_reg[0].write_arg  = 0;
		p_bs_reg[0].readable   = TRUE;
		p_bs_reg[0].read_func  = read_ptp_set_domain_number;
		p_bs_reg[0].read_arg   = 0;

		// add bootstrap register
		ret = gev_add_bs_reg(g_handle, p_bs_reg, 1);
		if (ret != GEV_ERR_SUCCESS)  { break; }
#else
		p_bs_reg[0].addr       = GVBS_PTP_SET_PRIORITY1_OFFSET;
		p_bs_reg[0].writable   = TRUE;
		p_bs_reg[0].write_func = write_ptp_set_priority1;
		p_bs_reg[0].write_arg  = 0;
		p_bs_reg[0].readable   = TRUE;
		p_bs_reg[0].read_func  = read_ptp_set_priority1;
		p_bs_reg[0].read_arg   = 0;

		p_bs_reg[1].addr       = GVBS_PTP_SET_PRIORITY2_OFFSET;
		p_bs_reg[1].writable   = TRUE;
		p_bs_reg[1].write_func = write_ptp_set_priority2;
		p_bs_reg[1].write_arg  = 0;
		p_bs_reg[1].readable   = TRUE;
		p_bs_reg[1].read_func  = read_ptp_set_priority2;
		p_bs_reg[1].read_arg   = 0;

		p_bs_reg[2].addr       = GVBS_PTP_SET_DOMAIN_NUMBER_OFFSET;
		p_bs_reg[2].writable   = TRUE;
		p_bs_reg[2].write_func = write_ptp_set_domain_number;
		p_bs_reg[2].write_arg  = 0;
		p_bs_reg[2].readable   = TRUE;
		p_bs_reg[2].read_func  = read_ptp_set_domain_number;
		p_bs_reg[2].read_arg   = 0;

		p_bs_reg[3].addr       = GVBS_PTP_SET_SLAVE_ONLY_OFFSET;
		p_bs_reg[3].writable   = TRUE;
		p_bs_reg[3].write_func = write_ptp_set_slave_only;
		p_bs_reg[3].write_arg  = 0;
		p_bs_reg[3].readable   = TRUE;
		p_bs_reg[3].read_func  = read_ptp_set_slave_only;
		p_bs_reg[3].read_arg   = 0;

		p_bs_reg[4].addr       = GVBS_PTP_SET_LOG_ANNOUNCE_INTERVAL_OFFSET;
		p_bs_reg[4].writable   = TRUE;
		p_bs_reg[4].write_func = write_ptp_set_log_announce_interval;
		p_bs_reg[4].write_arg  = 0;
		p_bs_reg[4].readable   = TRUE;
		p_bs_reg[4].read_func  = read_ptp_set_log_announce_interval;
		p_bs_reg[4].read_arg   = 0;

		p_bs_reg[5].addr       = GVBS_PTP_SET_LOG_SYNC_INTERVAL_OFFSET;
		p_bs_reg[5].writable   = TRUE;
		p_bs_reg[5].write_func = write_ptp_set_log_sync_interval;
		p_bs_reg[5].write_arg  = 0;
		p_bs_reg[5].readable   = TRUE;
		p_bs_reg[5].read_func  = read_ptp_set_log_sync_interval;
		p_bs_reg[5].read_arg   = 0;

		p_bs_reg[6].addr       = GVBS_PTP_SET_LOG_MIN_DELAY_REQ_INTERVAL_OFFSET;
		p_bs_reg[6].writable   = TRUE;
		p_bs_reg[6].write_func = write_ptp_set_log_min_delay_req_interval;
		p_bs_reg[6].write_arg  = 0;
		p_bs_reg[6].readable   = TRUE;
		p_bs_reg[6].read_func  = read_ptp_set_log_min_delay_req_interval;
		p_bs_reg[6].read_arg   = 0;

		p_bs_reg[7].addr       = GVBS_PTP_SET_ANNOUNCE_RECEIPT_TIMEOUT_OFFSET;
		p_bs_reg[7].writable   = TRUE;
		p_bs_reg[7].write_func = write_ptp_set_announce_receipt_timeout;
		p_bs_reg[7].write_arg  = 0;
		p_bs_reg[7].readable   = TRUE;
		p_bs_reg[7].read_func  = read_ptp_set_announce_receipt_timeout;
		p_bs_reg[7].read_arg   = 0;

		// add bootstrap register
		ret = gev_add_bs_reg(g_handle, p_bs_reg, 8);
		if (ret != GEV_ERR_SUCCESS)  { break; }
#endif  // defined(PTP_SLAVE_ONLY)
#endif  // defined(PTP_SUPPORT)

		p_bs_reg[0].addr       = GVBS_SET_TIMESTAMP_INITIAL_VALUE_OFFSET;
		p_bs_reg[0].writable   = TRUE;
		p_bs_reg[0].write_func = write_set_timestamp_initial_value;
		p_bs_reg[0].write_arg  = 0;
		p_bs_reg[0].readable   = FALSE;
		p_bs_reg[0].read_func  = NULL;
		p_bs_reg[0].read_arg   = 0;

		p_bs_reg[1].addr       = GVBS_TIMESTAMP_INITIAL_VALUE_HIGH_OFFSET;
		p_bs_reg[1].writable   = TRUE;
		p_bs_reg[1].write_func = write_timestamp_initial_value_high;
		p_bs_reg[1].write_arg  = 0;
		p_bs_reg[1].readable   = FALSE;
		p_bs_reg[1].read_func  = NULL;
		p_bs_reg[1].read_arg   = 0;

		p_bs_reg[2].addr       = GVBS_TIMESTAMP_INITIAL_VALUE_LOW_OFFSET;
		p_bs_reg[2].writable   = TRUE;
		p_bs_reg[2].write_func = write_timestamp_initial_value_low;
		p_bs_reg[2].write_arg  = 0;
		p_bs_reg[2].readable   = FALSE;
		p_bs_reg[2].read_func  = NULL;
		p_bs_reg[2].read_arg   = 0;

		p_bs_reg[3].addr       = GVBS_ACCEPT_BROADCAST_WRITEREG_OFFSET;
		p_bs_reg[3].writable   = TRUE;
		p_bs_reg[3].write_func = write_accept_broadcast_writereg;
		p_bs_reg[3].write_arg  = 0;
		p_bs_reg[3].readable   = TRUE;
		p_bs_reg[3].read_func  = read_accept_broadcast_writereg;
		p_bs_reg[3].read_arg   = 0;

		p_bs_reg[4].addr       = GVBS_DEVICE_RESET_OFFSET;
		p_bs_reg[4].writable   = TRUE;
		p_bs_reg[4].write_func = write_device_reset;
		p_bs_reg[4].write_arg  = 0;
		p_bs_reg[4].readable   = FALSE;
		p_bs_reg[4].read_func  = NULL;
		p_bs_reg[4].read_arg   = 0;

		// add bootstrap register
		ret = gev_add_bs_reg(g_handle, p_bs_reg, 5);
		if (ret != GEV_ERR_SUCCESS)  { break; }

// ========================================================================== //

		p_bs_reg_mem[0].addr       = 0x200000;
		p_bs_reg_mem[0].mem_size   = 0x30000;
		p_bs_reg_mem[0].writable   = FALSE;
		p_bs_reg_mem[0].write_func = NULL;
		p_bs_reg_mem[0].write_arg  = 1;
		p_bs_reg_mem[0].readable   = TRUE;
		p_bs_reg_mem[0].read_func  = read_mem_xml;
		p_bs_reg_mem[0].read_arg   = 0;

		// add bootstrap register
		ret = gev_add_bs_reg_mem(g_handle, p_bs_reg_mem, 1);
		if (ret != GEV_ERR_SUCCESS)  { break; }

// ========================================================================== //

		// timestamp tick frequency-high unit : Hz
		ret = gev_set_bs_timestamp_tick_high(g_handle, GEV_TIMESTAMP_FREQ_HIGH_HZ);
		if (ret != GEV_ERR_SUCCESS)  { break; }

		// timestamp tick frequency-low unit : Hz
		ret = gev_set_bs_timestamp_tick_low(g_handle, GEV_TIMESTAMP_FREQ_LOW_HZ);
		if (ret != GEV_ERR_SUCCESS)  { break; }

		// avalon-st clock frequency unit : Hz
		ret = gev_set_tx_clk_freq(g_handle, GEV_TX_CLK_FREQ);
		if (ret != GEV_ERR_SUCCESS)  { break; }

		// frame memory controller memory I/F frequency unit : Hz
		ret = gev_set_avm_clk_freq(g_handle, GEV_AVM_CLK_FREQ);
		if (ret != GEV_ERR_SUCCESS)  { break; }

		// set ipconfig done callback functions
		ret = gev_reg_ipconf_done_func(g_handle, ipconf_done, 0);
		if (ret != GEV_ERR_SUCCESS)  { break; }

		// set default chunk id
//		for (i = 0; i < 8; i++) {
//			ret = gev_set_bs_chunk_id(g_handle, i, i + 1);
//			if (ret != GEV_ERR_SUCCESS)  { break; }
//		}
		ret = gev_set_bs_chunk_id(g_handle, 0, 0x12344321);
		if (ret != GEV_ERR_SUCCESS)  { break; }
		ret = gev_set_bs_chunk_id(g_handle, 1, 0x23455432);
		if (ret != GEV_ERR_SUCCESS)  { break; }
		ret = gev_set_bs_chunk_id(g_handle, 2, 0x34566543);
		if (ret != GEV_ERR_SUCCESS)  { break; }
		ret = gev_set_bs_chunk_id(g_handle, 3, 0x45677654);
		if (ret != GEV_ERR_SUCCESS)  { break; }
		ret = gev_set_bs_chunk_id(g_handle, 4, 0x56788765);
		if (ret != GEV_ERR_SUCCESS)  { break; }
		ret = gev_set_bs_chunk_id(g_handle, 5, 0x67899876);
		if (ret != GEV_ERR_SUCCESS)  { break; }
		ret = gev_set_bs_chunk_id(g_handle, 6, 0x789AA987);
		if (ret != GEV_ERR_SUCCESS)  { break; }
		ret = gev_set_bs_chunk_id(g_handle, 7, 0x89ABBA98);
		if (ret != GEV_ERR_SUCCESS)  { break; }

		// set default chunk length
		for (i = 0; i < 8; i++) {
			ret = gev_set_bs_chunk_length(g_handle, i, 16);
			if (ret != GEV_ERR_SUCCESS)  { break; }
		}

		// action signals number
		ret = gev_set_bs_action_signals_num(g_handle, 8);
		if (ret != GEV_ERR_SUCCESS)  { break; }

		// set multicast ip address
		ret = gev_set_multicast_ip(g_handle, 0xE0000181);
		if (ret != GEV_ERR_SUCCESS)  { break; }

		// set multicast ttl
		ret = gev_set_multicast_ttl(g_handle, 1);
		if (ret != GEV_ERR_SUCCESS)  { break; }

#if defined(PTP_SUPPORT)
		init_ptp_param.priority1                    = 128;      // it is invalid if PTP_SLAVE_ONLY is defined
		init_ptp_param.priority2                    = 128;      // it is invalid if PTP_SLAVE_ONLY is defined
		init_ptp_param.domain_number                = 0;
		init_ptp_param.slave_only                   = FALSE;    // it is invalid if PTP_SLAVE_ONLY is defined
		init_ptp_param.log_announce_interval        = 1;        // it is invalid if PTP_SLAVE_ONLY is defined
		init_ptp_param.log_sync_interval            = 0;        // it is invalid if PTP_SLAVE_ONLY is defined
		init_ptp_param.log_min_delay_req_interval   = 0;        // it is invalid if PTP_SLAVE_ONLY is defined
		init_ptp_param.announce_receipt_timeout     = 3;        // it is invalid if PTP_SLAVE_ONLY is defined

		// set ptp initial value setting
		ret = gev_set_ptp_init_val(g_handle, init_ptp_param);
		if (ret != GEV_ERR_SUCCESS)  { break; }

		g_ptp_param.priority1                   = init_ptp_param.priority1;
		g_ptp_param.priority2                   = init_ptp_param.priority2;
		g_ptp_param.domain_number               = init_ptp_param.domain_number;
		g_ptp_param.slave_only                  = init_ptp_param.slave_only;
		g_ptp_param.log_announce_interval       = init_ptp_param.log_announce_interval;
		g_ptp_param.log_sync_interval           = init_ptp_param.log_sync_interval;
		g_ptp_param.log_min_delay_req_interval  = init_ptp_param.log_min_delay_req_interval;
		g_ptp_param.announce_receipt_timeout    = init_ptp_param.announce_receipt_timeout;
#endif  // defined(PTP_SUPPORT)

		// set test clock interval 1msec
		/* IOWR_ALTERA_AVALON_PIO_DATA(GEV_PIO_INTERVAL_BASE, 1000000); */

		// set GevSCSPPacketSize initial value
		ret = gev_set_gev_scps_packet_size(g_handle, 0x1000);
		if (ret != GEV_ERR_SUCCESS)  { break; }

		// set Stream Channel Packet Delay
		/* ret = gev_set_scpd(g_handle, 0x1000); */
		ret = gev_set_scpd(g_handle, 0);
		if (ret != GEV_ERR_SUCCESS)  { break; }

		// checking process if mcda, scda exist on the network
		/* ret = gev_mcda_scda_check_process(g_handle, TRUE); */
		ret = gev_mcda_scda_check_process(g_handle, FALSE);
		if (ret != GEV_ERR_SUCCESS)  { break; }

		// accepting broadcast writereg command
		g_accept_broadcast_writereg = FALSE;
		ret = gev_accept_broadcast_writereg(g_handle, g_accept_broadcast_writereg);
		if (ret != GEV_ERR_SUCCESS)  { break; }

		ret = gev_set_ref_timestamp_param(g_handle, FALSE);
		if (ret != GEV_ERR_SUCCESS)  { break; }

		// DHCP Discover/Request Option enable (enabled as "Parameter Request List")
		ret = gev_dhcp_para_req_list_opt(g_handle, TRUE);
		/* ret = gev_dhcp_para_req_list_opt(g_handle, FALSE); */
		if (ret != GEV_ERR_SUCCESS)  { break; }

		// Behavior if "0" is written to LLA bit of Network Interface Configration register
		ret = gev_accept_lla_false_access(g_handle, TRUE);      // Return "Success"
		/* ret = gev_accept_lla_false_access(g_handle, FALSE);     // Return "Invalid Parameter" */
		if (ret != GEV_ERR_SUCCESS)  { break; }

		// init peripherals
		width_write(0, &(g_data_param.width));
		height_write(0, &(g_data_param.height));
		width_max_write(0, &(g_data_param.sensor_width));
		height_max_write(0, &(g_data_param.sensor_height));
		offset_x_write(0, &(g_data_param.offset_x));
		offset_y_write(0, &(g_data_param.offset_y));
		pixel_format_write(0, &(g_data_param.pixel_format));
		payload_type_write(0, &(g_data_param.payload_type));

		// run GEV engine
		ret = gev_run(g_handle);
		if (ret) { printf("Exit gev_run : return = 0x%x\r\n", ret); }
		else { printf("GEV engine has stopped! please reset system \r\n"); }

		// close GEV engine
		gev_close(g_handle);
	}

	free(p_bs_reg);

	while (1) {

	}
}

//------------------------------------------------------------------------------
//! @brief User task 2
//
//! @return None
//------------------------------------------------------------------------------
void user_task_2(void)
{
	while (1) {
		sleep_task();
		gev_stop(g_handle);
	}
}

//------------------------------------------------------------------------------
//! @brief Bootstrap register 0x200000 read function
//
//! @param[in]  arg    : Argument that set by user
//! @param[in]  ofst   : offset of this bootstrap register memory
//! @param[in]  size   : Access memory size
//! @param[out] p_data : Memory data
//
//! @retval  0 : Success
//------------------------------------------------------------------------------
static int read_mem_xml(u32 arg, u32 ofst, u32 size, u8 *p_data)
{
//	memcpy(p_data, g_xml_file_p1 + ofst, size);

	if (g_test_version == GEV_VERSION_20) {
		memcpy(p_data, g_xml_file_v20_p1 + ofst, size);
	}
	else  {
		memcpy(p_data, g_xml_file_v12_p1 + ofst, size);
	}

	return 0;
}

//------------------------------------------------------------------------------
//! @brief Action Command function 0
//
//! @param[in] arg   : argument
//
//! @return None
//------------------------------------------------------------------------------
static void act_cmd_0(u32 arg)
{
	printf("Action Command function 0. arg = %d\r\n", (int)arg);
// send event command
	gev_send_evt_cmd(g_handle, 0xA001);
	printf("send event command\r\n");

#if defined(PTP_SUPPORT)
	u32 value_high, value_low;

	gev_get_scheduled_action_time_high(g_handle, &value_high);
	gev_get_scheduled_action_time_low(g_handle, &value_low);
	printf("Action Time High = %08x\r\n", (unsigned int)value_high);
	printf("Action Time Low  = %08x\r\n", (unsigned int)value_low);
#endif  // defined(PTP_SUPPORT)
}
//------------------------------------------------------------------------------
//! @brief Action Command function 1
//
//! @param[in] arg   : argument
//
//! @return None
//------------------------------------------------------------------------------
static void act_cmd_1(u32 arg)
{
	char eventdata[] = "12345678";

	printf("Action Command function 1. arg = %d\r\n", (int)arg);
// send event data command
	gev_send_evtdt_cmd(g_handle, 0xA005, (u8 *)eventdata, 8);
	printf("send event data command\r\n");

#if defined(PTP_SUPPORT)
	u32 value_high, value_low;

	gev_get_scheduled_action_time_high(g_handle, &value_high);
	gev_get_scheduled_action_time_low(g_handle, &value_low);
	printf("Action Time High = %08x\r\n", (unsigned int)value_high);
	printf("Action Time Low  = %08x\r\n", (unsigned int)value_low);
#endif  // defined(PTP_SUPPORT)
}
//------------------------------------------------------------------------------
//! @brief Action Command function 2
//
//! @param[in] arg   : argument
//
//! @return None
//------------------------------------------------------------------------------
static void act_cmd_2(u32 arg)
{
	u8 chunk_data_1[] = "1234";
	u8 chunk_data_2[] = "2222";
	u8 chunk_data_3[] = "3333";
	u8 chunk_data_4[] = "4444";
	u8 chunk_data_5[] = "5555";
	u8 chunk_data_6[] = "6666";
	u8 chunk_data_7[] = "7777";
	u8 chunk_data_8[] = "8888";
	static u8 tgl = 0;

	gev_err_t ret ;

	printf("Action Command function 2. arg = %d\r\n", (int)arg);
// toggle chunk data
	if (tgl == 0) {
		printf("Chunk data: pattern 1 - 8 \r\n");
		tgl = 1;
		ret = gev_update_chunk_data(g_handle, 0x12344321, chunk_data_1, 0, 4);
		if (ret == GEV_ERR_SUCCESS)  { printf("Update Chunk Data : ID 0x12344321\r\n"); }
		ret = gev_update_chunk_data(g_handle, 0x23455432, chunk_data_2, 0, 4);
		if (ret == GEV_ERR_SUCCESS)  { printf("Update Chunk Data : ID 0x23455432\r\n"); }
		ret = gev_update_chunk_data(g_handle, 0x34566543, chunk_data_3, 0, 4);
		if (ret == GEV_ERR_SUCCESS)  { printf("Update Chunk Data : ID 0x34566543\r\n"); }
		ret = gev_update_chunk_data(g_handle, 0x45677654, chunk_data_4, 0, 4);
		if (ret == GEV_ERR_SUCCESS)  { printf("Update Chunk Data : ID 0x45677654\r\n"); }
		ret = gev_update_chunk_data(g_handle, 0x56788765, chunk_data_5, 0, 4);
		if (ret == GEV_ERR_SUCCESS)  { printf("Update Chunk Data : ID 0x56788765\r\n"); }
		ret = gev_update_chunk_data(g_handle, 0x67899876, chunk_data_6, 0, 4);
		if (ret == GEV_ERR_SUCCESS)  { printf("Update Chunk Data : ID 0x67899876\r\n"); }
		ret = gev_update_chunk_data(g_handle, 0x789AA987, chunk_data_7, 0, 4);
		if (ret == GEV_ERR_SUCCESS)  { printf("Update Chunk Data : ID 0x789AA987\r\n"); }
		ret = gev_update_chunk_data(g_handle, 0x89ABBA98, chunk_data_8, 0, 4);
		if (ret == GEV_ERR_SUCCESS)  { printf("Update Chunk Data : ID 0x89ABBA98\r\n"); }
	}
	else {
		printf("Chunk data: pattern 8 - 1 \r\n");
		tgl = 0;
		ret = gev_update_chunk_data(g_handle, 0x89ABBA98, chunk_data_1, 0, 4);
		if (ret == GEV_ERR_SUCCESS)  { printf("Update Chunk Data : ID 0x89ABBA98\r\n"); }
		ret = gev_update_chunk_data(g_handle, 0x789AA987, chunk_data_2, 0, 4);
		if (ret == GEV_ERR_SUCCESS)  { printf("Update Chunk Data : ID 0x789AA987\r\n"); }
		ret = gev_update_chunk_data(g_handle, 0x67899876, chunk_data_3, 0, 4);
		if (ret == GEV_ERR_SUCCESS)  { printf("Update Chunk Data : ID 0x67899876\r\n"); }
		ret = gev_update_chunk_data(g_handle, 0x56788765, chunk_data_4, 0, 4);
		if (ret == GEV_ERR_SUCCESS)  { printf("Update Chunk Data : ID 0x56788765\r\n"); }
		ret = gev_update_chunk_data(g_handle, 0x45677654, chunk_data_5, 0, 4);
		if (ret == GEV_ERR_SUCCESS)  { printf("Update Chunk Data : ID 0x45677654\r\n"); }
		ret = gev_update_chunk_data(g_handle, 0x34566543, chunk_data_6, 0, 4);
		if (ret == GEV_ERR_SUCCESS)  { printf("Update Chunk Data : ID 0x34566543\r\n"); }
		ret = gev_update_chunk_data(g_handle, 0x23455432, chunk_data_7, 0, 4);
		if (ret == GEV_ERR_SUCCESS)  { printf("Update Chunk Data : ID 0x23455432\r\n"); }
		ret = gev_update_chunk_data(g_handle, 0x12344321, chunk_data_8, 0, 4);
		if (ret == GEV_ERR_SUCCESS)  { printf("Update Chunk Data : ID 0x12344321\r\n"); }
	}

#if defined(PTP_SUPPORT)
	u32 value_high, value_low;

	gev_get_scheduled_action_time_high(g_handle, &value_high);
	gev_get_scheduled_action_time_low(g_handle, &value_low);
	printf("Action Time High = %08x\r\n", (unsigned int)value_high);
	printf("Action Time Low  = %08x\r\n", (unsigned int)value_low);
#endif  // defined(PTP_SUPPORT)
}
//------------------------------------------------------------------------------
//! @brief Action Command function 3
//
//! @param[in] arg   : argument
//
//! @return None
//------------------------------------------------------------------------------
static void act_cmd_3(u32 arg)
{
	printf("Action Command function 3. arg = %d\r\n", (int)arg);

#if defined(PTP_SUPPORT)
	u32 value_high, value_low;

	gev_get_scheduled_action_time_high(g_handle, &value_high);
	gev_get_scheduled_action_time_low(g_handle, &value_low);
	printf("Action Time High = %08x\r\n", (unsigned int)value_high);
	printf("Action Time Low  = %08x\r\n", (unsigned int)value_low);
#endif  // defined(PTP_SUPPORT)
}
//------------------------------------------------------------------------------
//! @brief Action Command function 4
//
//! @param[in] arg   : argument
//
//! @return None
//------------------------------------------------------------------------------
static void act_cmd_4(u32 arg)
{
	printf("Action Command function 4. arg = %d\r\n", (int)arg);

#if defined(PTP_SUPPORT)
	u32 value_high, value_low;

	gev_get_scheduled_action_time_high(g_handle, &value_high);
	gev_get_scheduled_action_time_low(g_handle, &value_low);
	printf("Action Time High = %08x\r\n", (unsigned int)value_high);
	printf("Action Time Low  = %08x\r\n", (unsigned int)value_low);
#endif  // defined(PTP_SUPPORT)
}
//------------------------------------------------------------------------------
//! @brief Action Command function 5
//
//! @param[in] arg   : argument
//
//! @return None
//------------------------------------------------------------------------------
static void act_cmd_5(u32 arg)
{
	printf("Action Command function 5. arg = %d\r\n", (int)arg);

#if defined(PTP_SUPPORT)
	u32 value_high, value_low;

	gev_get_scheduled_action_time_high(g_handle, &value_high);
	gev_get_scheduled_action_time_low(g_handle, &value_low);
	printf("Action Time High = %08x\r\n", (unsigned int)value_high);
	printf("Action Time Low  = %08x\r\n", (unsigned int)value_low);
#endif  // defined(PTP_SUPPORT)
}
//------------------------------------------------------------------------------
//! @brief Action Command function 6
//
//! @param[in] arg   : argument
//
//! @return None
//------------------------------------------------------------------------------
static void act_cmd_6(u32 arg)
{
	printf("Action Command function 6. arg = %d\r\n", (int)arg);

#if defined(PTP_SUPPORT)
	u32 value_high, value_low;

	gev_get_scheduled_action_time_high(g_handle, &value_high);
	gev_get_scheduled_action_time_low(g_handle, &value_low);
	printf("Action Time High = %08x\r\n", (unsigned int)value_high);
	printf("Action Time Low  = %08x\r\n", (unsigned int)value_low);
#endif  // defined(PTP_SUPPORT)
}
//------------------------------------------------------------------------------
//! @brief Action Command function 7
//
//! @param[in] arg   : argument
//
//! @return None
//------------------------------------------------------------------------------
static void act_cmd_7(u32 arg)
{
	printf("Action Command function 7. arg = %d\r\n", (int)arg);

#if defined(PTP_SUPPORT)
	u32 value_high, value_low;

	gev_get_scheduled_action_time_high(g_handle, &value_high);
	gev_get_scheduled_action_time_low(g_handle, &value_low);
	printf("Action Time High = %08x\r\n", (unsigned int)value_high);
	printf("Action Time Low  = %08x\r\n", (unsigned int)value_low);
#endif  // defined(PTP_SUPPORT)
}

//------------------------------------------------------------------------------
//! @brief Bootstrap register Width write function
//
//! @param[in] arg    : Argument that set by user
//! @param[in] p_data : Register value
//
//! @retval  0 : Success
//------------------------------------------------------------------------------
static int width_write(u32 arg, u32 *p_data)
{
	printf("width_write() arg = %#x data = %d\r\n", (unsigned int)arg, (unsigned int)*p_data);
	IOWR_ALTERA_AVALON_PIO_DATA(GEV_PIO_O_WIDTH_BASE, *p_data);
	g_data_param.width = *p_data;
	return 0;
}

//------------------------------------------------------------------------------
//! @brief Bootstrap register Height write function
//
//! @param[in] arg    : Argument that set by user
//! @param[in] p_data : Register value
//
//! @retval  0 : Success
//------------------------------------------------------------------------------
static int height_write(u32 arg, u32 *p_data)
{
	printf("height_write() arg = %#x data = %d\r\n", (unsigned int)arg, (unsigned int)*p_data);
	IOWR_ALTERA_AVALON_PIO_DATA(GEV_PIO_O_HEIGHT_BASE, *p_data);
	g_data_param.height = *p_data;

	return 0;
}

//------------------------------------------------------------------------------
//! @brief Bootstrap register Sensor Width write function
//
//! @param[in] arg    : Argument that set by user
//! @param[in] p_data : Register value
//
//! @retval  0 : Success
//------------------------------------------------------------------------------
static int width_max_write(u32 arg, u32 *p_data)
{
	printf("width_max_write() arg = %#x data = %d\r\n", (unsigned int)arg, (unsigned int)*p_data);
	IOWR_ALTERA_AVALON_PIO_DATA(GEV_PIO_O_WIDTH_MAX_BASE, *p_data);
	g_data_param.sensor_width = *p_data;
	return 0;
}

//------------------------------------------------------------------------------
//! @brief Bootstrap register Sensor Width read function
//
//! @param[in] arg    : Argument that set by user
//! @param[in] p_data : Register value
//
//! @retval  0 : Success
//------------------------------------------------------------------------------
static int width_max_read(u32 arg, u32 *p_data)
{
	*p_data = IORD_ALTERA_AVALON_PIO_DATA(GEV_PIO_O_WIDTH_MAX_BASE);
	printf("width_max_read() arg = %#x data = %d\r\n", (unsigned int)arg, (unsigned int)*p_data);

	return 0;
}

//------------------------------------------------------------------------------
//! @brief Bootstrap register Sensor Height write function
//
//! @param[in] arg    : Argument that set by user
//! @param[in] p_data : Register value
//
//! @retval  0 : Success
//------------------------------------------------------------------------------
static int height_max_write(u32 arg, u32 *p_data)
{
	printf("height_max_write() arg = %#x data = %d\r\n", (unsigned int)arg, (unsigned int)*p_data);
	IOWR_ALTERA_AVALON_PIO_DATA(GEV_PIO_O_HEIGHT_MAX_BASE, *p_data);
	g_data_param.sensor_height = *p_data;

	return 0;
}

//------------------------------------------------------------------------------
//! @brief Bootstrap register Sensor Height read function
//
//! @param[in] arg    : Argument that set by user
//! @param[in] p_data : Register value
//
//! @retval  0 : Success
//------------------------------------------------------------------------------
static int height_max_read(u32 arg, u32 *p_data)
{
	*p_data = IORD_ALTERA_AVALON_PIO_DATA(GEV_PIO_O_HEIGHT_MAX_BASE);
	printf("height_max_read() arg = %#x data = %d\r\n", (unsigned int)arg, (unsigned int)*p_data);

	return 0;
}

//------------------------------------------------------------------------------
//! @brief Bootstrap register OffsetX write function
//
//! @param[in] arg    : Argument that set by user
//! @param[in] p_data : Register value
//
//! @retval  0 : Success
//------------------------------------------------------------------------------
static int offset_x_write(u32 arg, u32 *p_data)
{
	printf("offset_x_write() arg = %#x data = %#x\r\n", (unsigned int)arg, (unsigned int)*p_data);
	IOWR_ALTERA_AVALON_PIO_DATA(GEV_PIO_O_OFFSET_X_BASE, *p_data);
	g_data_param.offset_x = *p_data;

	return 0;
}

//------------------------------------------------------------------------------
//! @brief Bootstrap register OffsetY write function
//
//! @param[in] arg    : Argument that set by user
//! @param[in] p_data : Register value
//
//! @retval  0 : Success
//------------------------------------------------------------------------------
static int offset_y_write(u32 arg, u32 *p_data)
{
	printf("offset_y_write() arg = %#x data = %#x\r\n", (unsigned int)arg, (unsigned int)*p_data);
	IOWR_ALTERA_AVALON_PIO_DATA(GEV_PIO_O_OFFSET_Y_BASE, *p_data);
	g_data_param.offset_y = *p_data;

	return 0;
}

//------------------------------------------------------------------------------
//! @brief Bootstrap register PixelFormat write function
//
//! @param[in] arg    : Argument that set by user
//! @param[in] p_data : Register value
//
//! @retval  0 : Success
//------------------------------------------------------------------------------
static int pixel_format_write(u32 arg, u32 *p_data)
{
	printf("pixel_format_write() arg = %#x data = %#x\r\n", (unsigned int)arg, (unsigned int)*p_data);
	IOWR_ALTERA_AVALON_PIO_DATA(GEV_PIO_O_PIXEL_FORMAT_BASE, *p_data);
	g_data_param.pixel_format = *p_data;

	return 0;
}

//------------------------------------------------------------------------------
//! @brief Bootstrap register PayloadType write function
//
//! @param[in] arg    : Argument that set by user
//! @param[in] p_data : Register value
//
//! @retval  0 : Success
//------------------------------------------------------------------------------
static int payload_type_write(u32 arg, u32 *p_data)
{
	printf("payload_type_write() arg = %#x data = %#x\r\n", (unsigned int)arg, (unsigned int)*p_data);
	IOWR_ALTERA_AVALON_PIO_DATA(GEV_PIO_O_PAYLOAD_TYPE_BASE, *p_data);
	g_data_param.payload_type = *p_data;

	return 0;
}

//------------------------------------------------------------------------------
//! @brief Bootstrap register AcquisitionStart write function
//
//! @param[in] arg    : Argument that set by user
//! @param[in] p_data : Register value
//
//! @retval  0 : Success
//------------------------------------------------------------------------------
static int acquisition_start(u32 arg, u32 *p_data)
{
	printf("acquisition_start() arg = %#x data = %#x\r\n", (unsigned int)arg, (unsigned int)*p_data);

	if ((g_data_param.payload_type == GVSP_PAYLOAD_TYPE_IMAGE_DATA_UNCOMPRESSED) ||
		(g_data_param.payload_type == GVSP_PAYLOAD_TYPE_IMAGE_EXTENDED_CHUNK)) {
		if (g_data_param.offset_x + g_data_param.width > g_data_param.sensor_width) {
			return -1;
		}
		if (g_data_param.offset_y + g_data_param.height > g_data_param.sensor_height) {
			return -1;
		}
	}

	return 0;
}
//------------------------------------------------------------------------------
//! @brief Bootstrap register AcquisitionStop write function
//
//! @param[in] arg    : Argument that set by user
//! @param[in] p_data : Register value
//
//! @retval  0 : Success
//------------------------------------------------------------------------------
static int acquisition_stop(u32 arg, u32 *p_data)
{
	printf("acquisition_stop() arg = %#x data = %#x\r\n", (unsigned int)arg, (unsigned int)*p_data);
	g_gvsp_active = FALSE;

	return 0;
}

//------------------------------------------------------------------------------
//! @brief Bootstrap register FieldCount write function
//
//! @param[in] arg    : Argument that set by user
//! @param[in] p_data : Register value
//
//! @retval  0 : Success
//------------------------------------------------------------------------------
static int field_count_write(u32 arg, u32 *p_data)
{
	printf("field_count_write() arg = %#x data = %#x\r\n", (unsigned int)arg, (unsigned int)*p_data);
	g_data_param.field_count = *p_data;

	return 0;
}

//------------------------------------------------------------------------------
//! @brief flash open function
//
//! @param[in] name : device name
//
//! @retval  0     : Error
//! @retval  Other : device handle
//------------------------------------------------------------------------------
static void* flash_open_dev(const char* name)
{
	return (void *)alt_flash_open_dev(name);
}
//------------------------------------------------------------------------------
//! @brief flash close function
//
//! @param[in] name : device handle
//
//! @return  None
//------------------------------------------------------------------------------
static void flash_close_dev(void *p_dev)
{
	alt_flash_close_dev((alt_flash_fd *)p_dev);
}

//------------------------------------------------------------------------------
//! @brief flash write function
//
//! @param[in] name   : device name
//! @param[in] offset : flash write address offset
//! @param[in] p_src  : source data address
//! @param[in] len    : length to write
//
//! @retval  written size
//------------------------------------------------------------------------------
static int flash_write_dev(void* p_dev, int offset, const void* p_src, int len)
{
//	return alt_write_flash((alt_flash_fd *)p_dev, offset, p_src, len);

	const int access_offset = ROUND_OFF(offset, SF_CTR_AVL_MEM_SECTOR_SIZE);
	const int access_size
	 = ((((offset - access_offset) + (int) len) > SF_CTR_AVL_MEM_SECTOR_SIZE) ? (SF_CTR_AVL_MEM_SECTOR_SIZE * 2) : SF_CTR_AVL_MEM_SECTOR_SIZE);
	static u8 cache[SF_CTR_AVL_MEM_SECTOR_SIZE * 2];

	int ret;

	if (len <= 0) goto DONE;

	// reads the flash memory
	ret = alt_read_flash((alt_flash_fd *)p_dev, access_offset, &(cache[0]), access_size);
	if (ret != 0) goto ERR1;

	// copies the write data to the buffer
	memcpy(&(cache[offset - access_offset]), p_src, len);

	// writes the flash memory
	ret = alt_write_flash((alt_flash_fd *)p_dev, access_offset, &(cache[0]), access_size);
	if (ret != 0) goto ERR2;

DONE:
	return 0;

ERR1:
	printf("Fail : alt_read_flash() : ret =%d\n", ret);
	return -1;

ERR2:
	printf("Fail : alt_write_flash() : ret =%d\n", ret);
	return -1;
}

//------------------------------------------------------------------------------
//! @brief flash read function
//
//! @param[in] name   : device name
//! @param[in] offset : flash read address offset
//! @param[in] p_src  : destination data address
//! @param[in] len    : length to read
//
//! @retval  read size
//------------------------------------------------------------------------------
static int flash_read_dev(void* p_dev, int offset, void* p_dst, int len)
{
	return alt_read_flash((alt_flash_fd *)p_dev, offset, p_dst, len);
}

//------------------------------------------------------------------------------
//! @brief IP config done function
//
//! @param[in] arg    : Argument that set by user
//! @param[in] p_data : Register value
//
//! @retval  0 : Success
//------------------------------------------------------------------------------
static int ipconf_done(u32 arg, u32 *p_data)
{
	if (*p_data == GVBS_IP_CONFIGURATION_LLA) {
		printf("LLA IP Set End\n");
	}
	else if (*p_data == GVBS_IP_CONFIGURATION_DHCP) {
		printf("DHCP IP Set End\n");
	}
	else { // *p_data == GVBS_IP_CONFIGURATION_PERSISTENT
		printf("Persistent IP Set End\n");
	}

	return 0;
}

//------------------------------------------------------------------------------
//! @brief reference timestamp parameter write function
//
//! @param[in] arg    : Argument that set by user
//! @param[in] p_data : Register value
//
//! @retval  0 : Success
//------------------------------------------------------------------------------
static int write_timestamp_sel(u32 arg, u32 *p_data)
{
	gev_err_t ret;

	if (*p_data){
		ret = gev_set_ref_timestamp_param(g_handle, TRUE);
	}
	else {
		ret = gev_set_ref_timestamp_param(g_handle, FALSE);
	}

	if ( ret == GEV_ERR_SUCCESS ) {
		printf("Success : write_timestamp_sel : value = %s\n", *p_data ? "true" : "false");
	}
	else {
		printf("Fail : write_timestamp_sel : ret = %d\n", (unsigned int)ret);
	}

	return ret;
}

//------------------------------------------------------------------------------
//! @brief reference timestamp parameter read function
//
//! @param[in]  arg    : Argument that set by user
//! @param[out] p_data : Register value
//
//! @retval  0 : Success
//------------------------------------------------------------------------------
static int read_timestamp_sel(u32 arg, u32 *p_data)
{
	gev_err_t ret;
	bool temp;

	ret = gev_get_ref_timestamp_param(g_handle, &temp);
	*p_data = temp ? 1 : 0 ;

	if ( ret == GEV_ERR_SUCCESS ) {
		printf("Success : read_timestamp_sel : value = %s\n", *p_data ? "true" : "false");
	}
	else {
		printf("Fail : read_timestamp_sel : ret = %d\n", (unsigned int)ret);
	}

	return ret;
}

#if defined(PTP_SUPPORT)
#if !defined(PTP_SLAVE_ONLY)
//------------------------------------------------------------------------------
//! @brief set priority1 function
//
//! @param[in] arg    : Argument that set by user
//! @param[in] p_data : Register value
//
//! @retval  0 : Success
//------------------------------------------------------------------------------
static int write_ptp_set_priority1(u32 arg, u32 *p_data)
{
	gev_err_t ret;

	g_ptp_param.priority1 = (u8)(*p_data);
	ret = gev_ptp_set_priority1(g_handle, g_ptp_param.priority1);

	if ( ret == GEV_ERR_SUCCESS ) {
		printf("Success : write_ptp_set_priority1 : value = %d\n", (unsigned int)(*p_data));
	}
	else {
		printf("Fail : write_ptp_set_priority1 : value = %d : ret = %d\n", (unsigned int)(*p_data), (unsigned int)ret);
	}

	return ret;
}

//------------------------------------------------------------------------------
//! @brief get priority1 function
//
//! @param[in]  arg    : Argument that set by user
//! @param[out] p_data : Register value
//
//! @retval  0 : Success
//------------------------------------------------------------------------------
static int read_ptp_set_priority1(u32 arg, u32 *p_data)
{

	*p_data = (u32)g_ptp_param.priority1;
	printf("Access : read_ptp_set_priority1 : value = %d\n", (unsigned int)(*p_data));

	return 0;
}

//------------------------------------------------------------------------------
//! @brief set priority2 function
//
//! @param[in] arg    : Argument that set by user
//! @param[in] p_data : Register value
//
//! @retval  0 : Success
//------------------------------------------------------------------------------
static int write_ptp_set_priority2(u32 arg, u32 *p_data)
{
	gev_err_t ret;

	g_ptp_param.priority2 = (u8)(*p_data);
	ret = gev_ptp_set_priority2(g_handle, g_ptp_param.priority2);

	if ( ret == GEV_ERR_SUCCESS ) {
		printf("Success : write_ptp_set_priority2 : value = %d\n", (unsigned int)(*p_data));
	}
	else {
		printf("Fail : write_ptp_set_priority2 : value = %d : ret = %d\n", (unsigned int)(*p_data), (unsigned int)ret);
	}

	return ret;
}

//------------------------------------------------------------------------------
//! @brief get priority2 function
//
//! @param[in]  arg    : Argument that set by user
//! @param[out] p_data : Register value
//
//! @retval  0 : Success
//------------------------------------------------------------------------------
static int read_ptp_set_priority2(u32 arg, u32 *p_data)
{

	*p_data = (u32)g_ptp_param.priority2;
	printf("Access : read_ptp_set_priority2 : value = %d\n", (unsigned int)(*p_data));

	return 0;
}

#endif  // !defined(PTP_SLAVE_ONLY)
//------------------------------------------------------------------------------
//! @brief set domain number function
//
//! @param[in] arg    : Argument that set by user
//! @param[in] p_data : Register value
//
//! @retval  0 : Success
//------------------------------------------------------------------------------
static int write_ptp_set_domain_number(u32 arg, u32 *p_data)
{
	gev_err_t ret;

	g_ptp_param.domain_number = (u8)(*p_data);
	ret = gev_ptp_set_domain_number(g_handle, g_ptp_param.domain_number);

	if ( ret == GEV_ERR_SUCCESS ) {
		printf("Success : write_ptp_set_domain_number : value = %d\n", (unsigned int)(*p_data));
	}
	else {
		printf("Fail : write_ptp_set_domain_number : value = %d : ret = %d\n", (unsigned int)(*p_data), (unsigned int)ret);
	}

	return ret;
}

//------------------------------------------------------------------------------
//! @brief get domain number function
//
//! @param[in]  arg    : Argument that set by user
//! @param[out] p_data : Register value
//
//! @retval  0 : Success
//------------------------------------------------------------------------------
static int read_ptp_set_domain_number(u32 arg, u32 *p_data)
{

	*p_data = (u32)g_ptp_param.domain_number;
	printf("Access : read_ptp_set_domain_number : value = %d\n", (unsigned int)(*p_data));

	return 0;
}

#if !defined(PTP_SLAVE_ONLY)
//------------------------------------------------------------------------------
//! @brief set slave only function
//
//! @param[in] arg    : Argument that set by user
//! @param[in] p_data : Register value
//
//! @retval  0 : Success
//------------------------------------------------------------------------------
static int write_ptp_set_slave_only(u32 arg, u32 *p_data)
{
	gev_err_t ret;

	if (*p_data){
		ret = gev_ptp_set_slave_only(g_handle, TRUE);
		g_ptp_param.slave_only = TRUE;
	}
	else {
		ret = gev_ptp_set_slave_only(g_handle, FALSE);
		g_ptp_param.slave_only = FALSE;
	}

	if ( ret == GEV_ERR_SUCCESS ) {
		printf("Success : write_ptp_set_slave_only : value = %s\n", *p_data ? "true" : "false");
	}
	else {
		printf("Fail : write_ptp_set_slave_only : value = %s : ret = %d\n", *p_data ? "true" : "false", (unsigned int)ret);
	}

	return ret;
}

//------------------------------------------------------------------------------
//! @brief get slave only function
//
//! @param[in]  arg    : Argument that set by user
//! @param[out] p_data : Register value
//
//! @retval  0 : Success
//------------------------------------------------------------------------------
static int read_ptp_set_slave_only(u32 arg, u32 *p_data)
{

	*p_data = g_ptp_param.slave_only ? 1 : 0 ;
	printf("Access : read_ptp_set_slave_only : value = %s\n", *p_data ? "true" : "false");

	return 0;
}

//------------------------------------------------------------------------------
//! @brief set log announce interval function
//
//! @param[in] arg    : Argument that set by user
//! @param[in] p_data : Register value
//
//! @retval  0 : Success
//------------------------------------------------------------------------------
static int write_ptp_set_log_announce_interval(u32 arg, u32 *p_data)
{
	gev_err_t ret;

	g_ptp_param.log_announce_interval = (s8)(*p_data);
	ret = gev_ptp_set_log_announce_interval(g_handle, g_ptp_param.log_announce_interval);

	if ( ret == GEV_ERR_SUCCESS ) {
		printf("Success : write_ptp_set_log_announce_interval : value = %d\n", (int)(*p_data));
	}
	else {
		printf("Fail : write_ptp_set_log_announce_interval : value = %d : ret = %d\n", (int)(*p_data), (unsigned int)ret);
	}

	return ret;
}

//------------------------------------------------------------------------------
//! @brief get log announce interval function
//
//! @param[in]  arg    : Argument that set by user
//! @param[out] p_data : Register value
//
//! @retval  0 : Success
//------------------------------------------------------------------------------
static int read_ptp_set_log_announce_interval(u32 arg, u32 *p_data)
{

	*p_data = (u32)((s32)g_ptp_param.log_announce_interval);
	printf("Access : read_ptp_set_log_announce_interval : value = %d\n", (int)(*p_data));

	return 0;
}

//------------------------------------------------------------------------------
//! @brief set log sync interval function
//
//! @param[in] arg    : Argument that set by user
//! @param[in] p_data : Register value
//
//! @retval  0 : Success
//------------------------------------------------------------------------------
static int write_ptp_set_log_sync_interval(u32 arg, u32 *p_data)
{
	gev_err_t ret;

	g_ptp_param.log_sync_interval = (s8)(*p_data);
	ret = gev_ptp_set_log_sync_interval(g_handle, g_ptp_param.log_sync_interval);

	if ( ret == GEV_ERR_SUCCESS ) {
		printf("Success : write_ptp_set_log_sync_interval : value = %d\n", (int)(*p_data));
	}
	else {
		printf("Fail : write_ptp_set_log_sync_interval : value = %d : ret = %d\n", (int)(*p_data), (unsigned int)ret);
	}

	return ret;
}

//------------------------------------------------------------------------------
//! @brief get log sync interval function
//
//! @param[in]  arg    : Argument that set by user
//! @param[out] p_data : Register value
//
//! @retval  0 : Success
//------------------------------------------------------------------------------
static int read_ptp_set_log_sync_interval(u32 arg, u32 *p_data)
{

	*p_data = (u32)((s32)g_ptp_param.log_sync_interval);
	printf("Access : read_ptp_set_log_sync_interval : value = %d\n", (int)(*p_data));

	return 0;
}

//------------------------------------------------------------------------------
//! @brief set log min delay req interval function
//
//! @param[in] arg    : Argument that set by user
//! @param[in] p_data : Register value
//
//! @retval  0 : Success
//------------------------------------------------------------------------------
static int write_ptp_set_log_min_delay_req_interval(u32 arg, u32 *p_data)
{
	gev_err_t ret;

	g_ptp_param.log_min_delay_req_interval = (s8)(*p_data);
	ret = gev_ptp_set_log_min_delay_req_interval(g_handle, g_ptp_param.log_min_delay_req_interval);

	if ( ret == GEV_ERR_SUCCESS ) {
		printf("Success : write_ptp_set_log_min_delay_req_interval : value = %d\n", (int)(*p_data));
	}
	else {
		printf("Fail : write_ptp_set_log_min_delay_req_interval : value = %d : ret = %d\n", (int)(*p_data), (unsigned int)ret);
	}

	return ret;
}

//------------------------------------------------------------------------------
//! @brief get log min delay req interval function
//
//! @param[in]  arg    : Argument that set by user
//! @param[out] p_data : Register value
//
//! @retval  0 : Success
//------------------------------------------------------------------------------
static int read_ptp_set_log_min_delay_req_interval(u32 arg, u32 *p_data)
{

	*p_data = (u32)((s32)g_ptp_param.log_min_delay_req_interval);
	printf("Access : read_ptp_set_log_min_delay_req_interval : value = %d\n", (int)(*p_data));

	return 0;
}

//------------------------------------------------------------------------------
//! @brief set announce receipt timeout function
//
//! @param[in] arg    : Argument that set by user
//! @param[in] p_data : Register value
//
//! @retval  0 : Success
//------------------------------------------------------------------------------
static int write_ptp_set_announce_receipt_timeout(u32 arg, u32 *p_data)
{
	gev_err_t ret;

	g_ptp_param.announce_receipt_timeout = (u8)(*p_data);
	ret = gev_ptp_set_announce_receipt_timeout(g_handle, g_ptp_param.announce_receipt_timeout);

	if ( ret == GEV_ERR_SUCCESS ) {
		printf("Success : write_ptp_set_log_min_delay_req_interval : value = %d\n", (unsigned int)(*p_data));
	}
	else {
		printf("Fail : write_ptp_set_log_min_delay_req_interval : value = %d : ret = %d\n", (unsigned int)(*p_data), (unsigned int)ret);
	}

	return ret;
}

//------------------------------------------------------------------------------
//! @brief get announce receipt timeout function
//
//! @param[in]  arg    : Argument that set by user
//! @param[out] p_data : Register value
//
//! @retval  0 : Success
//------------------------------------------------------------------------------
static int read_ptp_set_announce_receipt_timeout(u32 arg, u32 *p_data)
{

	*p_data = (u32)g_ptp_param.announce_receipt_timeout;
	printf("Access : read_ptp_set_announce_receipt_timeout : value = %d\n", (unsigned int)(*p_data));

	return 0;
}
#endif  // !defined(PTP_SLAVE_ONLY)
#endif  // defined(PTP_SUPPORT)

//------------------------------------------------------------------------------
//! @brief set initial timestamp function
//
//! @param[in] arg    : Argument that set by user
//! @param[in] p_data : Register value
//
//! @retval  0 : Success
//------------------------------------------------------------------------------
static int write_set_timestamp_initial_value(u32 arg, u32 *p_data)
{
	gev_err_t ret;

	if (g_test_version == GEV_VERSION_12) {
		ret = gev_set_local_time(g_handle, g_set_time.low, g_set_time.high);
		printf("GEV_VERSION_12 : gev_set_local_time\n");
	}
	else {
		ret = gev_set_local_time(g_handle, g_set_time.low, g_set_time.high);
		printf("GEV_VERSION_20 : gev_set_local_time ret = %d\n", (int)ret);

		if (ret == GEV_ERR_NOT_SUPPORT) { // ptp enable
			ret = gev_ptp_set_master_time(g_handle, g_set_time.low, g_set_time.high);
			printf("GEV_VERSION_20 : gev_ptp_set_master_time\n");
		}
	}

	if ( ret == GEV_ERR_SUCCESS ) {
		printf("Success : write_set_timestamp_initial_value\n");
	}
	else {
		printf("Fail : write_set_timestamp_initial_value : ret = %d\n", (unsigned int)ret);
	}

	return ret;
}

//------------------------------------------------------------------------------
//! @brief set timestamp value [High] function
//
//! @param[in] arg    : Argument that set by user
//! @param[in] p_data : Register value
//
//! @retval  0 : Success
//------------------------------------------------------------------------------
static int write_timestamp_initial_value_high(u32 arg, u32 *p_data)
{

	g_set_time.high = *p_data;
	printf("Access : write_timestamp_initial_value_high : value = %d\n", (unsigned int)(*p_data));

	return 0;
}

//------------------------------------------------------------------------------
//! @brief set timestamp value [Low] function
//
//! @param[in] arg    : Argument that set by user
//! @param[in] p_data : Register value
//
//! @retval  0 : Success
//------------------------------------------------------------------------------
static int write_timestamp_initial_value_low(u32 arg, u32 *p_data)
{

	g_set_time.low = *p_data;
	printf("Access : write_timestamp_initial_value_low : value = %d\n", (unsigned int)(*p_data));

	return 0;
}

//------------------------------------------------------------------------------
//! @brief set gev_accept_broadcast_writereg function
//
//! @param[in] arg    : Argument that set by user
//! @param[in] p_data : Register value
//
//! @retval  0 : Success
//------------------------------------------------------------------------------
static int write_accept_broadcast_writereg(u32 arg, u32 *p_data)
{
	gev_err_t ret;

	if (*p_data){
		ret = gev_accept_broadcast_writereg(g_handle, TRUE);
		g_accept_broadcast_writereg = TRUE;
	}
	else {
		ret = gev_accept_broadcast_writereg(g_handle, FALSE);
		g_accept_broadcast_writereg = FALSE;
	}

	if ( ret == GEV_ERR_SUCCESS ) {
		printf("Success : accept_broadcast_writereg : value = %s\n", *p_data ? "true" : "false");
	}
	else {
		printf("Fail : accept_broadcast_writereg : value = %s : ret = %d\n", *p_data ? "true" : "false", (unsigned int)ret);
	}

	return ret;
}

//------------------------------------------------------------------------------
//! @brief get gev_accept_broadcast_writereg function
//
//! @param[in]  arg    : Argument that set by user
//! @param[out] p_data : Register value
//
//! @retval  0 : Success
//------------------------------------------------------------------------------
static int read_accept_broadcast_writereg(u32 arg, u32 *p_data)
{

	*p_data = g_accept_broadcast_writereg ? 1 : 0 ;
	printf("Access : read_accept_broadcast_writereg : value = %s\n", *p_data ? "true" : "false");

	return 0;
}


//------------------------------------------------------------------------------
//! @brief set write_device_reset function
//
//! @param[in] arg    : Argument that set by user
//! @param[in] p_data : Register value
//
//! @retval  0 : Success
//------------------------------------------------------------------------------
static int write_device_reset(u32 arg, u32 *p_data)
{
	int ret;
	u8 task_id;

	printf("Access : write_device_reset\n");
	ret = get_task_id_in_entry(user_task_2, &task_id);
	if (ret != 0) { printf("ERROR : Can't get task ID!\r\n"); }

	wakeup_task(task_id);
	usleep(1000000); // To pass validation tool. Delete this if it is not needed.

	return ret;
}
