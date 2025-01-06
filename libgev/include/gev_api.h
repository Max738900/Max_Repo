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
//! @brief  This header file defines structures defined by the specification,
//!         main program data structure, and definition of all API functions.
//=====================================================================================

#if !defined(__GEV_API_H__)
#define __GEV_API_H__

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#include "../../libscorpius/include/scorpius.h"
#include "bootstrap_reg.h"
#include "package_config.h"

//------------------------------------------------------------------------------
// define and structures
//------------------------------------------------------------------------------
// user task id definition
#define USER_TASK_1_ID       3
#define USER_TASK_2_ID       4
#define IDLE_TASK_ID         5

// define MAC address size
#define MAC_SIZE             6

// PIXEL FORMATS
// indicate if pixel is monochrome or RGB
#define GVSP_PIX_MONO        0x01000000
#define GVSP_PIX_RGB         0x02000000 // deprecated in version 1.1
#define GVSP_PIX_COLOR       0x02000000
#define GVSP_PIX_CUSTOM      0x80000000
#define GVSP_PIX_COLOR_MASK  0xFF000000
// indicate effective number of bits occupied by the pixel (including padding).
// This can be used to compute amount of memory required to store an image.
#define GVSP_PIX_OCCUPY1BIT  0x00010000
#define GVSP_PIX_OCCUPY2BIT  0x00020000
#define GVSP_PIX_OCCUPY4BIT  0x00040000
#define GVSP_PIX_OCCUPY8BIT  0x00080000
#define GVSP_PIX_OCCUPY12BIT 0x000C0000
#define GVSP_PIX_OCCUPY16BIT 0x00100000
#define GVSP_PIX_OCCUPY24BIT 0x00180000
#define GVSP_PIX_OCCUPY32BIT 0x00200000
#define GVSP_PIX_OCCUPY36BIT 0x00240000
#define GVSP_PIX_OCCUPY48BIT 0x00300000
#define GVSP_PIX_EFFECTIVE_PIXEL_SIZE_MASK  0x00FF0000
#define GVSP_PIX_EFFECTIVE_PIXEL_SIZE_SHIFT 16
// Pixel ID: lower 16-bit of the pixel formats
#define GVSP_PIX_ID_MASK 0x0000FFFF
#define GVSP_PIX_COUNT   0x46            // next Pixel ID available

#define GVSP_PIX_MONO1P                  (GVSP_PIX_MONO | GVSP_PIX_OCCUPY1BIT   | 0x0037)
#define GVSP_PIX_MONO2P                  (GVSP_PIX_MONO | GVSP_PIX_OCCUPY2BIT   | 0x0038)
#define GVSP_PIX_MONO4P                  (GVSP_PIX_MONO | GVSP_PIX_OCCUPY4BIT   | 0x0039)
#define GVSP_PIX_MONO8                   (GVSP_PIX_MONO | GVSP_PIX_OCCUPY8BIT   | 0x0001)
#define GVSP_PIX_MONO8S                  (GVSP_PIX_MONO | GVSP_PIX_OCCUPY8BIT   | 0x0002)
#define GVSP_PIX_MONO10                  (GVSP_PIX_MONO | GVSP_PIX_OCCUPY16BIT  | 0x0003)
#define GVSP_PIX_MONO10_PACKED           (GVSP_PIX_MONO | GVSP_PIX_OCCUPY12BIT  | 0x0004)
#define GVSP_PIX_MONO12                  (GVSP_PIX_MONO | GVSP_PIX_OCCUPY16BIT  | 0x0005)
#define GVSP_PIX_MONO12_PACKED           (GVSP_PIX_MONO | GVSP_PIX_OCCUPY12BIT  | 0x0006)
#define GVSP_PIX_MONO14                  (GVSP_PIX_MONO | GVSP_PIX_OCCUPY16BIT  | 0x0025)
#define GVSP_PIX_MONO16                  (GVSP_PIX_MONO | GVSP_PIX_OCCUPY16BIT  | 0x0007)
#define GVSP_PIX_BAYGR8                  (GVSP_PIX_MONO | GVSP_PIX_OCCUPY8BIT   | 0x0008)
#define GVSP_PIX_BAYRG8                  (GVSP_PIX_MONO | GVSP_PIX_OCCUPY8BIT   | 0x0009)
#define GVSP_PIX_BAYGB8                  (GVSP_PIX_MONO | GVSP_PIX_OCCUPY8BIT   | 0x000A)
#define GVSP_PIX_BAYBG8                  (GVSP_PIX_MONO | GVSP_PIX_OCCUPY8BIT   | 0x000B)
#define GVSP_PIX_BAYGR10                 (GVSP_PIX_MONO | GVSP_PIX_OCCUPY16BIT  | 0x000C)
#define GVSP_PIX_BAYRG10                 (GVSP_PIX_MONO | GVSP_PIX_OCCUPY16BIT  | 0x000D)
#define GVSP_PIX_BAYGB10                 (GVSP_PIX_MONO | GVSP_PIX_OCCUPY16BIT  | 0x000E)
#define GVSP_PIX_BAYBG10                 (GVSP_PIX_MONO | GVSP_PIX_OCCUPY16BIT  | 0x000F)
#define GVSP_PIX_BAYGR12                 (GVSP_PIX_MONO | GVSP_PIX_OCCUPY16BIT  | 0x0010)
#define GVSP_PIX_BAYRG12                 (GVSP_PIX_MONO | GVSP_PIX_OCCUPY16BIT  | 0x0011)
#define GVSP_PIX_BAYGB12                 (GVSP_PIX_MONO | GVSP_PIX_OCCUPY16BIT  | 0x0012)
#define GVSP_PIX_BAYBG12                 (GVSP_PIX_MONO | GVSP_PIX_OCCUPY16BIT  | 0x0013)
#define GVSP_PIX_BAYGR10_PACKED          (GVSP_PIX_MONO | GVSP_PIX_OCCUPY12BIT  | 0x0026)
#define GVSP_PIX_BAYRG10_PACKED          (GVSP_PIX_MONO | GVSP_PIX_OCCUPY12BIT  | 0x0027)
#define GVSP_PIX_BAYGB10_PACKED          (GVSP_PIX_MONO | GVSP_PIX_OCCUPY12BIT  | 0x0028)
#define GVSP_PIX_BAYBG10_PACKED          (GVSP_PIX_MONO | GVSP_PIX_OCCUPY12BIT  | 0x0029)
#define GVSP_PIX_BAYGR12_PACKED          (GVSP_PIX_MONO | GVSP_PIX_OCCUPY12BIT  | 0x002A)
#define GVSP_PIX_BAYRG12_PACKED          (GVSP_PIX_MONO | GVSP_PIX_OCCUPY12BIT  | 0x002B)
#define GVSP_PIX_BAYGB12_PACKED          (GVSP_PIX_MONO | GVSP_PIX_OCCUPY12BIT  | 0x002C)
#define GVSP_PIX_BAYBG12_PACKED          (GVSP_PIX_MONO | GVSP_PIX_OCCUPY12BIT  | 0x002D)
#define GVSP_PIX_BAYGR16                 (GVSP_PIX_MONO | GVSP_PIX_OCCUPY16BIT  | 0x002E)
#define GVSP_PIX_BAYRG16                 (GVSP_PIX_MONO | GVSP_PIX_OCCUPY16BIT  | 0x002F)
#define GVSP_PIX_BAYGB16                 (GVSP_PIX_MONO | GVSP_PIX_OCCUPY16BIT  | 0x0030)
#define GVSP_PIX_BAYBG16                 (GVSP_PIX_MONO | GVSP_PIX_OCCUPY16BIT  | 0x0031)
#define GVSP_PIX_RGB8                    (GVSP_PIX_COLOR | GVSP_PIX_OCCUPY24BIT | 0x0014)
#define GVSP_PIX_BGR8                    (GVSP_PIX_COLOR | GVSP_PIX_OCCUPY24BIT | 0x0015)
#define GVSP_PIX_RGBA8                   (GVSP_PIX_COLOR | GVSP_PIX_OCCUPY32BIT | 0x0016)
#define GVSP_PIX_BGRA8                   (GVSP_PIX_COLOR | GVSP_PIX_OCCUPY32BIT | 0x0017)
#define GVSP_PIX_RGB10                   (GVSP_PIX_COLOR | GVSP_PIX_OCCUPY48BIT | 0x0018)
#define GVSP_PIX_BGR10                   (GVSP_PIX_COLOR | GVSP_PIX_OCCUPY48BIT | 0x0019)
#define GVSP_PIX_RGB12                   (GVSP_PIX_COLOR | GVSP_PIX_OCCUPY48BIT | 0x001A)
#define GVSP_PIX_BGR12                   (GVSP_PIX_COLOR | GVSP_PIX_OCCUPY48BIT | 0x001B)
#define GVSP_PIX_RGB16                   (GVSP_PIX_COLOR | GVSP_PIX_OCCUPY48BIT | 0x0033)
#define GVSP_PIX_RGB10V1_PACKED          (GVSP_PIX_COLOR | GVSP_PIX_OCCUPY32BIT | 0x001C)
#define GVSP_PIX_RGB10P32                (GVSP_PIX_COLOR | GVSP_PIX_OCCUPY32BIT | 0x001D)
#define GVSP_PIX_RGB12V1_PACKED          (GVSP_PIX_COLOR | GVSP_PIX_OCCUPY36BIT | 0X0034)
#define GVSP_PIX_RGB565P                 (GVSP_PIX_COLOR | GVSP_PIX_OCCUPY16BIT | 0x0035)
#define GVSP_PIX_BGR565P                 (GVSP_PIX_COLOR | GVSP_PIX_OCCUPY16BIT | 0X0036)
#define GVSP_PIX_YUV411_8_UYYVYY         (GVSP_PIX_COLOR | GVSP_PIX_OCCUPY12BIT | 0x001E)
#define GVSP_PIX_YUV422_8_UYVY           (GVSP_PIX_COLOR | GVSP_PIX_OCCUPY16BIT | 0x001F)
#define GVSP_PIX_YUV422_8                (GVSP_PIX_COLOR | GVSP_PIX_OCCUPY16BIT | 0x0032)
#define GVSP_PIX_YUV8_UYV                (GVSP_PIX_COLOR | GVSP_PIX_OCCUPY24BIT | 0x0020)
#define GVSP_PIX_YCBCR8_CBYCR            (GVSP_PIX_COLOR | GVSP_PIX_OCCUPY24BIT | 0x003A)
#define GVSP_PIX_YCBCR422_8              (GVSP_PIX_COLOR | GVSP_PIX_OCCUPY16BIT | 0x003B)
#define GVSP_PIX_YCBCR422_8_CBYCRY       (GVSP_PIX_COLOR | GVSP_PIX_OCCUPY16BIT | 0x0043)
#define GVSP_PIX_YCBCR411_8_CBYYCRYY     (GVSP_PIX_COLOR | GVSP_PIX_OCCUPY12BIT | 0x003C)
#define GVSP_PIX_YCBCR601_8_CBYCR        (GVSP_PIX_COLOR | GVSP_PIX_OCCUPY24BIT | 0x003D)
#define GVSP_PIX_YCBCR601_422_8          (GVSP_PIX_COLOR | GVSP_PIX_OCCUPY16BIT | 0x003E)
#define GVSP_PIX_YCBCR601_422_8_CBYCRY   (GVSP_PIX_COLOR | GVSP_PIX_OCCUPY16BIT | 0x0044)
#define GVSP_PIX_YCBCR601_411_8_CBYYCRYY (GVSP_PIX_COLOR | GVSP_PIX_OCCUPY12BIT | 0x003F)
#define GVSP_PIX_YCBCR709_8_CBYCR        (GVSP_PIX_COLOR | GVSP_PIX_OCCUPY24BIT | 0x0040)
#define GVSP_PIX_YCBCR709_422_8          (GVSP_PIX_COLOR | GVSP_PIX_OCCUPY16BIT | 0x0041)
#define GVSP_PIX_YCBCR709_422_8_CBYCRY   (GVSP_PIX_COLOR | GVSP_PIX_OCCUPY16BIT | 0x0045)
#define GVSP_PIX_YCBCR709_411_8_CBYYCRYY (GVSP_PIX_COLOR | GVSP_PIX_OCCUPY12BIT | 0x0042)
#define GVSP_PIX_RGB8_PLANAR             (GVSP_PIX_COLOR | GVSP_PIX_OCCUPY24BIT | 0x0021)
#define GVSP_PIX_RGB10_PLANAR            (GVSP_PIX_COLOR | GVSP_PIX_OCCUPY48BIT | 0x0022)
#define GVSP_PIX_RGB12_PLANAR            (GVSP_PIX_COLOR | GVSP_PIX_OCCUPY48BIT | 0x0023)
#define GVSP_PIX_RGB16_PLANAR            (GVSP_PIX_COLOR | GVSP_PIX_OCCUPY48BIT | 0x0024)


// type define for API
typedef void* GEV_HANDLE;
typedef void (*gev_act_func_t)(u32 arg);
typedef int (*gev_bs_reg_func_t)(u32 arg, u32 *p_data);
typedef int (*gev_bs_reg_rng_func_t)(u32 arg, u32 offset, u32 *p_data);
typedef int (*gev_bs_reg_mem_func_t)(u32 arg, u32 offset, u32 size, u8 *p_data);

typedef void* (*gev_flash_dev_open_t)(const char* name);
typedef void (*gev_flash_dev_close_t)(void* p_dev);
typedef int (*gev_flash_dev_write_t)(void* p_dev, int offset, const void* p_src, int len);
typedef int (*gev_flash_dev_read_t)(void* p_dev, int offset, void* p_dst, int len);

//! GEV specification version
typedef enum gev_ver {
	GEV_VERSION_12 = 0x00010002,   //!< version 1.2
	GEV_VERSION_20 = 0x00020000    //!< version 2.0
} gev_ver_t, *p_gev_ver_t;

//! GEV IP information  // 22*4 = 88 byte
typedef struct gev_ip_info {
	u32 fpga_register_base;         //!< GEV IP register base address                            = FPGA_REGISTOR_BASE
	u32 fpga_register_irq;          //!< GEV IP register interrupt                               = FPGA_REGISTOR_IRQ
	u32 fpga_register_irq_cntl_id;  //!< GEV IP register interrupt controller ID                 = FPGA_REGISTOR_IRQ_INTERRUPT_CONTROLLER_ID
	u32 timer_base;                 //!< timer base address                                      = TIMER_BASE
	u32 timer_irq;                  //!< timer interrupt                                         = TIMER_IRQ
	u32 timer_irq_cntl_id;          //!< timer interrupt controller ID                           = TIMER_IRQ_INTERRUPT_CONTROLLER_ID
	u32 receive_packet_mem_base;    //!< receive packet memory base address                      = RECEIVE_PACKET_MEM_BASE
	u32 transmitter_chunk_mem_base; //!< transmitter chunk memory base address                   = TRANSMITTER_CHUNK_MEM_BASE
	u32 transmitter_gvcp_mem_base;  //!< transmitter GVCP memory base address                    = TRANSMITTER_GVCP_MEM_BASE
	u32 transmitter_gvsp_mem_base;  //!< transmitter GVCP memory base address                    = TRANSMITTER_GVSP_MEM_BASE
	u32 avalon_st_clock_frequency;  //!< avalon st clock frequency                               = AVALON_ST_CLOCK_FREQUENCY
	u32 frm_ctl_mem_clk_frequency;  //!< avm_clk frequency
	u32 tse_mac_base;               //!< Triple Speed Ethernet IP base address                   = EMAC_BASE
	u32 ptp_clock_base;             //!< PTP Clock IP base address base address                  = PTP_CLOCK_HWIP_BASE
	u32 ptp_clock_irq;              //!< PTP Clock IP base address base address                  = PTP_CLOCK_HWIP_IRQ
	u32 ptp_clock_irq_cntl_id;      //!< PTP Clock IP base address base address                  = PTP_CLOCK_HWIP_IRQ_INTERRUPT_CONTROLLER_ID
	u32 ptp_ts_cap_csr_base;        //!< PTP Timestamp Capture IP CSR base address               = PTP_TIME_CAP_HWIP_CSR_BASE
	u32 ptp_ts_cap_csr_irq;         //!< PTP Timestamp Capture IP CSR interrupt                  = PTP_TIME_CAP_HWIP_CSR_IRQ
	u32 ptp_ts_cap_csr_irq_cntl_id; //!< PTP Timestamp Capture IP CSR interrupt controller ID    = PTP_TIME_CAP_HWIP_CSR_IRQ_INTERRUPT_CONTROLLER_ID
	u32 ptp_ts_cap_mem_base;        //!< PTP Timestamp Capture IP memory base address            = PTP_TIME_CAP_HWIP_MEM_BASE
	u32 ptp_ts_cap_mem_irq;         //!< PTP Timestamp Capture IP memory interrupt               = PTP_TIME_CAP_HWIP_MEM_IRQ
	u32 ptp_ts_cap_mem_irq_cntl_id; //!< PTP Timestamp Capture IP memory interrupt controller ID = PTP_TIME_CAP_HWIP_MEM_IRQ_INTERRUPT_CONTROLLER_ID
} gev_ip_info_t, *p_gev_ip_info_t;

//! error type of GEV API
typedef enum gev_err {
	GEV_ERR_SUCCESS      = 0x0000,   //!< success
	GEV_ERR_RESTART      = 0x0001,   //!< GEV engine restart
	GEV_ERR_HANDLE       = 0x0002,   //!< handle err
	GEV_ERR_RUN          = 0x0003,   //!< GEV engine is running
	GEV_ERR_LACK_OF_INIT = 0x0004,   //!< lack of initialization to run GEV engine
	GEV_ERR_NOT_RUN      = 0x0005,   //!< GEV engine is not running
	GEV_ERR_INTERNAL     = 0x0006,   //!< GEV engine internal error occurred
	GEV_ERR_INVALID_ARG  = 0x0007,   //!< invalid argument
	GEV_ERR_NO_MSG_CH    = 0x0008,   //!< no message channel to send EVENT_CMD or EVENTDATA_CMD
	GEV_ERR_NO_RESOURCE  = 0x0009,   //!< GEV library lack of internal resource
	GEV_ERR_NOT_SUPPORT  = 0x000A,   //!< Not supported
	GEV_ERR_FLASH_ACCESS = 0x000B,   //!< flash access error
	GEV_ERR_LACK_OF_MEM  = 0x000C,   //!< Lack of memory resource
	GEV_ERR_NOT_OPEN     = 0x000D,   //!< GEV library is not open
	GEV_ERR_ALREADY_SET  = 0x000E,   //!< parameter is already set
	GEV_ERR_NO_CHUNK_ID  = 0x000F,   //!< no such chunk ID
	GEV_ERR_NOT_EXE      = 0x0010,   //!< can not run
	GEV_ERR_SYSTEM       = 0x0011,   //!< system error
	GEV_ERR_ERROR        = 0x0012    //!< Other error
} gev_err_t, *p_gev_err_t;

//! frame memory paramter structure
typedef struct gev_frm_mem_param_t {
	u32 frm_start_addr_ofst;         //!< offset first frame start address from frame memory range
	u32 frm_ofst;                    //!< frame offset
	u16 frm_num;                     //!< frame number
	u32 frm_read_dly;                //!< delay of read frame data from frame memory
} gev_frm_mem_param_t, *p_gev_frm_mem_param_t;

//! GVSP paylaod type
typedef enum gev_payload_type {
	GVSP_PAYLOAD_TYPE_IMAGE_DATA_UNCOMPRESSED = 0x0001, //!< image data uncompressed
	GVSP_PAYLOAD_TYPE_RAW_DATA_BINARY         = 0x0002, //!< raw data binary
	GVSP_PAYLOAD_TYPE_EXTENDED_CHUNK          = 0x0005, //!< extended chunk (ver 1.2 only)
	GVSP_PAYLOAD_TYPE_IMAGE_EXTENDED_CHUNK    = 0x4001, //!< extended chunk with image data (ver 2.0 only)
	GVSP_PAYLOAD_TYPE_RAW_DATA_EXTENDED_CHUNK = 0x4002  //!< extended chunk with raw data (ver 2.0 only)
} gev_payload_type_t, *p_gev_payload_type_t;

//! event identifier type
typedef enum gev_evt_id {
	GEV_EVENT_LINK_SPEED_CHANGE        = 0x0008, //!< The link speed has changed.
	GEV_EVENT_ACTION_LATE              = 0x0009, //!<  Scheduled Action Command was late.
	GEV_EVENT_ERROR_NOT_IMPLEMENTED    = 0x8001, //!< Command is not supported.
	GEV_EVENT_ERROR_INVALID_PARAMETER  = 0x8002, //!< At least one parameter provided in the
	                                             //!< command is invalid(or out of range) for the device.
	GEV_EVENT_ERROR_INVALID_ADDRESS    = 0x8003, //!< An attempt was made to access a non-existent address space location.
	GEV_EVENT_ERROR_WRITE_PROTECT      = 0x8004, //!< The addressed register cannot be written to.
	GEV_EVENT_ERROR_BAD_ALIGNMENT      = 0x8005, //!< A badly aligned address offset or data size was specified.
	GEV_EVENT_ERROR_ACCESS_DENIED      = 0x8006, //!< An attempt was made to access an address location which is
	                                             //!< currently/momentary not accessible.
	GEV_EVENT_ERROR_BUSY               = 0x8007, //!< A required resource t service the request is not currently available.
	GEV_EVENT_ERROR_PACKET_UNAVAILABLE = 0x800C, //!< The requested packet is not available anymore.
	GEV_EVENT_ERROR_DATA_OVERRUN       = 0x800D, //!< Internal memory of GVSP transmitter overrun.
	GEV_EVENT_ERROR_INVALID_HEADER     = 0x800E, //!< The message header is not available.
	GEV_EVENT_ERROR_PACKET_NOT_YET_AVAILABLE            = 0x8010,
	                                             //!< The requested packet has not yet been acquired.
	GEV_EVENT_ERROR_PACKET_AND_PREV_REMOVED_FROM_MEMORY = 0x8011,
	                                             //!< The requested packet and all previous ones are not available anymore
	                                             //!< and have been discarded from the GVSP transmitter memory.
	GEV_EVENT_ERROR_PACKET_REMOVED_FROM_MEMORY          = 0x8012,
	                                             //!< The requested packet is not available anymore and has been discard
	                                             //!< from the GVSP transmitter memory.
	GEV_EVENT_ERROR_NO_REF_TIME        = 0x8013, //!< The device is not synchronized to a master clock to be used as time reference.
	GEV_EVENT_ERROR_PACKET_TEMPORARILY_UNAVAILABLR      = 0x8014,
	                                             //!< The packet cannot be resent at the moment due to temporary bandwidth
	                                             //!< issues and should be requested again in the future.
	GEV_EVENT_ERROR_OVERFLOW           = 0x8015, //!< A device queue or packet data has overflowed.
	GEV_EVENT_ERROR_ACTION_LATE        = 0x8016, //!< The requested scheduled action command was requested at a time that
	                                             //!< is already past.
	GEV_EVENT_ERROR_LIB_IP_RELEASE     = 0x9000, //!< The device will release IP address soon.
	GEV_EVENT_ERROR_RTOS               = 0x9001, //!< RTOS system call failure
	GEV_EVENT_ERROR_NO_SIGNAL_STOP     = 0x9002, //!< stop because of no image signal
	GEV_EVENT_ERROR_DEVICE_ERROR       = 0xA000, //!< Device-specific Event
	GEV_EVENT_ONE_MULTISHOT_BUFF_END   = 0xB000  //!< oneshot/multishot buffering end interrupt
} gev_evt_id_t, *p_gev_evt_id_t;

#define CHUNK_DATA_SIZE_MAX 512

//! single chunk information structure
typedef struct gev_chunk_single {
	bool enable;                                 //!< current chunk enable
	u32 chunk_id;                                //!< chunk id
	u16 chunk_size;                              //!< chunk size
	u8 chunk_data[CHUNK_DATA_SIZE_MAX];          //!< chunk data
} gev_chunk_single_t, *p_gev_chunk_single;

//! chunk information structure
typedef struct gev_chunk_info {
	bool enable;                                 //!< chunk enable
	gev_chunk_single_t chunk[GVBS_CHUNK_NUM_MAX];//!< total chunk information
	u32 chunk_total_size;                        //!< chunk data total size
} gev_chunk_info_t, *p_gev_chunk_info_t;

//! GVSP image field type
typedef enum gev_img_field_type {
	FIELD_TYPE_PROGRESSIVE = 0,                  //!< progressive
	FIELD_TYPE_INTERLACE   = 1                   //!< interlace
} gev_img_field_type_t, *p_gev_img_field_type_t;

//! bootstrap register information structure
typedef struct gev_bs_reg {
	u32 addr;                                    //!< register address
	bool writable;                               //!< writable
	gev_bs_reg_func_t write_func;                //!< write function
	u32 write_arg;                               //!< write function argument
	bool readable;                               //!< readable
	gev_bs_reg_func_t read_func;                 //!< read function
	u32 read_arg;                                //!< read function argument
} gev_bs_reg_t, *p_gev_bs_reg_t;

//! bootstrap register range information structure
typedef struct gev_bs_reg_rng {
	u32 addr;                                    //!< register address
	u32 rng_size;                                //!< register range size
	bool writable;                               //!< writable
	gev_bs_reg_rng_func_t write_func;            //!< write function
	u32 write_arg;                               //!< write function argument 1
	bool readable;                               //!< readable
	gev_bs_reg_rng_func_t read_func;             //!< read function
	u32 read_arg;                                //!< read function argument 1
} gev_bs_reg_rng_t, *p_gev_bs_reg_rng_t;

typedef struct bs_gev_reg_mem {
	u32 addr;                                    //!< register address
	u32 mem_size;                                //!< register memory area size
	bool writable;                               //!< writable
	gev_bs_reg_mem_func_t write_func;            //!< write function
	u32 write_arg;                               //!< write function argument 1
	bool readable;                               //!< readable
	gev_bs_reg_mem_func_t read_func;             //!< read function
	u32 read_arg;                                //!< read function argument 1
} gev_bs_reg_mem_t, *p_gev_bs_reg_mem_t;

//! type of debug mode type
typedef enum gev_debug_mode_type {
	GEV_DEBUG_MODE_NONE = 0,                     //!< normal mode
	GEV_DEBUG_MODE_1    = 1,                     //!< debug mode 1
	GEV_DEBUG_MODE_2    = 2,                     //!< debug mode 2
	GEV_DEBUG_MODE_3    = 3                      //!< debug mode 3
} gev_debug_mode_type_t, *p_gev_debug_mode_type_t;

//! type of debug mode data pattern
typedef enum gev_debug_mode_pattern {
	GEV_DEBUG_MODE_PATTERN_WORD_INC   = 0,       //!< word data increment
	GEV_DEBUG_MODE_PATTERN_BYTE_INC   = 1,       //!< byte data increment
	GEV_DEBUG_MODE_PATTERN_COLOR_BAR  = 2,       //!< color bar
	GEV_DEBUG_MODE_PATTERN_GRAY_SCALE = 3        //!< gray scale
} gev_debug_mode_pattern_t, *p_gev_debug_mode_pattern_t;

//! type of debug mode timing
typedef enum gev_debug_mode_timing {
	GEV_DEBUG_MODE_TIMING_INPUT_SIGNAL      = 0, //!< input signal create timing
	GEV_DEBUG_MODE_TIMING_PATTERN_GENERATOR = 1  //!< pattern generator create timing
} gev_debug_mode_timing_t, *p_gev_debug_mode_timing_t;

//! definition of debug mode structure
typedef struct gev_debug_mode {
	gev_debug_mode_type_t type;                  //!< debug mode type
	gev_debug_mode_pattern_t pattern;            //!< debug data pattern
//	gev_debug_mode_timing_t timing;              //!< timing
	u32 width;                                   //!< image width
	u32 height;                                  //!< image height
} gev_debug_mode_t, *p_gev_debug_mode_t;

//! defination of flash device access functions
typedef struct flash_dev_func {
	const char *p_dev_name;                      //!< device name
	gev_flash_dev_open_t dev_open;               //!< device open function
	gev_flash_dev_close_t dev_close;             //!< device open function
	gev_flash_dev_write_t dev_write;             //!< device open function
	gev_flash_dev_read_t dev_read;               //!< device open function
} flash_dev_func_t, *p_flash_dev_func_t;

//! image parameter structure 2+2+2+2 +4 +4 =
typedef struct gev_img_param {
	gev_payload_type_t payload_type;  //!< payload type
	u16 img_width;                    //!< horizontal pixels to transfer
	u16 img_height;                   //!< vertical pixels to transfer
	u16 x_ofst;                       //!< offset of horizontal pixels to transfer
	u16 y_ofst;                       //!< offset of vertical pixels to transfer
	u32 pixel_format;                 //!< pixel format
	u32 raw_data_size;                //!< raw data size
} gev_img_param_t, *p_gev_img_param_t;

//! Manifest header union
typedef struct manifest_header {
	struct {
		u32 reserved        : 26;
		u32 num_entries     : 6;
	} header_bit_field;
	u32 reserved;
} manifest_header_t, *p_manifest_header_t;

//! Manifest entry union
typedef struct manifest_entry {
	struct {
		u32 reserved              : 2;
		u32 schema_minor_version  : 6;
		u32 schema_major_version  : 6;
		u32 xml_sub_minor_version : 6;
		u32 xml_minor_version     : 6;
		u32 xml_major_version     : 6;
	} entry_bit_field;
	u32 url_register_address;
} manifest_entry_t, *p_manifest_entry_t;

//! Manifest header union
typedef struct manifest_table {
	manifest_header_t header;
	manifest_entry_t entry[GVBS_MANIFEST_TABLE_ENTRY_MAX - 1];
} manifest_table_t, *p_manifest_table_t;

//! PTP parameter definition
typedef struct ptp_param {
	u8   priority1;
	u8   priority2;
	u8   domain_number;
	bool slave_only;
	s8   log_announce_interval;
	s8   log_sync_interval;
	s8   log_min_delay_req_interval;
	u8   announce_receipt_timeout;
} ptp_param_t, *p_ptp_param_t;

//! flash memory address definition
typedef struct flash_mem_offset {
	u32	current_ip_configuration;
	u32	user_defined_name;
	u32	persistent_ip_address_0;
	u32	persistent_subnet_mask_0;
	u32	persistent_gateway_0;
	u32	gvcp_configuration;
} flash_mem_offset_t, *p_flash_mem_offset_t;

//------------------------------------------------------------------------------
// global variables
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// prototypes
//------------------------------------------------------------------------------
GEV_HANDLE gev_open(gev_ver_t ver, gev_ip_info_t info, gev_img_param_t image_param);  //hard 
gev_err_t gev_get_version(GEV_HANDLE handle, u8 *p_major, u8 *p_minor, u8 *p_sub);   //TRANS OK
gev_err_t gev_run(GEV_HANDLE handle);  //easy
gev_err_t gev_stop(GEV_HANDLE handle);  //easy
gev_err_t gev_close(GEV_HANDLE handle); //easy OK
//	gev_err_t gev_set_flash_param(GEV_HANDLE handle, bool flash_support_flag, u32 flash_blk_addr, const char *p_dev_name, flash_dev_func_t* p_funcs);
gev_err_t gev_set_flash_param(GEV_HANDLE handle, bool flash_support_flag, u32 flash_blk_addr, const char *p_dev_name, flash_dev_func_t* p_funcs, const flash_mem_offset_t *p_mem_offset);

gev_err_t gev_set_bs_dev_mac_addr(GEV_HANDLE handle, u8 *p_mac);   //easy  OK
gev_err_t gev_set_bs_dev_mode(GEV_HANDLE handle, u32 val);         //easy
gev_err_t gev_set_bs_ntwk_if_cap(GEV_HANDLE handle, u32 val);       //OK
gev_err_t gev_set_bs_gvsp_cap(GEV_HANDLE handle, u32 val);     //ok
gev_err_t gev_set_bs_gvcp_cap(GEV_HANDLE handle, u32 val);
gev_err_t gev_set_bs_strm_ch_cap(GEV_HANDLE handle, u32 val);
gev_err_t gev_set_bs_mnft_name(GEV_HANDLE handle, unsigned char *p_data, u8 size);
gev_err_t gev_set_bs_model_name(GEV_HANDLE handle, unsigned char *p_data, u8 size);
gev_err_t gev_set_bs_dev_ver(GEV_HANDLE handle, unsigned char *p_data, u8 size);
gev_err_t gev_set_bs_mnft_info(GEV_HANDLE handle, unsigned char *p_data, u8 size);
gev_err_t gev_set_bs_seri_num(GEV_HANDLE handle, unsigned char *p_data, u8 size);
gev_err_t gev_set_bs_fst_url(GEV_HANDLE handle, unsigned char *p_data, u16 size);
gev_err_t gev_set_bs_scnd_url(GEV_HANDLE handle, unsigned char *p_data, u16 size);
gev_err_t gev_set_bs_mnfest_tbl(GEV_HANDLE handle, unsigned char *p_data, u16 size);
gev_err_t gev_set_bs_pend_tmout(GEV_HANDLE handle, u32 val);
gev_err_t gev_set_bs_msg_ch_src_port(GEV_HANDLE handle, u16 val);
gev_err_t gev_set_bs_strm_ch_src_port(GEV_HANDLE handle, u16 val);
gev_err_t gev_read_bs_reg(GEV_HANDLE handle, u32 addr, u32 *p_val);  //OK

gev_err_t gev_set_ext_bs_reg_num(GEV_HANDLE handle, u16 bs_reg_num, u16 bs_reg_rng_num, u16 bs_reg_mem_num);
gev_err_t gev_add_bs_reg(GEV_HANDLE handle, gev_bs_reg_t *p_reg_info, u16 reg_num);
gev_err_t gev_add_bs_reg_rng(GEV_HANDLE handle, gev_bs_reg_rng_t *p_reg_rng_info, u16 reg_rng_num);
gev_err_t gev_add_bs_reg_mem(GEV_HANDLE handle, gev_bs_reg_mem_t *p_reg_mem_info, u16 reg_mem_num);

gev_err_t gev_send_evt_cmd(GEV_HANDLE handle, gev_evt_id_t evt_id);
gev_err_t gev_send_evtdt_cmd(GEV_HANDLE handle, gev_evt_id_t evt_id, u8 *p_evtdt, u16 evtdt_size);
gev_err_t gev_update_chunk_data(GEV_HANDLE handle, u32 chunk_id, u8 *p_data, u16 ofst, u16 size);

gev_err_t gev_reg_act_func(GEV_HANDLE handle, u8 num, gev_act_func_t act_func, u32 arg);   //easy 
gev_err_t gev_reg_bs_reg_width_write_func(GEV_HANDLE handle, gev_bs_reg_func_t write_func, u32 arg);  //easy
gev_err_t gev_reg_bs_reg_height_write_func(GEV_HANDLE handle, gev_bs_reg_func_t write_func, u32 arg);  //easy
gev_err_t gev_reg_bs_reg_ofstx_write_func(GEV_HANDLE handle, gev_bs_reg_func_t write_func, u32 arg);   //easy
gev_err_t gev_reg_bs_reg_ofsty_write_func(GEV_HANDLE handle, gev_bs_reg_func_t write_func, u32 arg);   //easy
gev_err_t gev_reg_bs_reg_acquisition_start_func(GEV_HANDLE handle, gev_bs_reg_func_t write_func, u32 arg);  //easy
gev_err_t gev_reg_bs_reg_acquisition_stop_func(GEV_HANDLE handle, gev_bs_reg_func_t write_func, u32 arg);   //easy
gev_err_t gev_reg_bs_reg_pixel_fromat_write_func(GEV_HANDLE handle, gev_bs_reg_func_t write_func, u32 arg);
gev_err_t gev_reg_bs_reg_payload_type_write_func(GEV_HANDLE handle, gev_bs_reg_func_t write_func, u32 arg);
gev_err_t gev_reg_bs_reg_field_count_write_func(GEV_HANDLE handle, gev_bs_reg_func_t write_func, u32 arg);
gev_err_t gev_reg_bs_reg_acquisition_mode_write_func(GEV_HANDLE handle, gev_bs_reg_func_t write_func, u32 arg);
gev_err_t gev_reg_bs_reg_acquisition_frame_count_write_func(GEV_HANDLE handle, gev_bs_reg_func_t write_func, u32 arg);
gev_err_t gev_reg_ipconf_done_func(GEV_HANDLE handle, gev_bs_reg_func_t done_func, u32 arg);
gev_err_t gev_set_bs_chunk_id(GEV_HANDLE handle, u8 num, u32 val);
gev_err_t gev_set_bs_chunk_length(GEV_HANDLE handle, u8 num, u32 val);
gev_err_t gev_get_scheduled_action_time_high(GEV_HANDLE handle, u32 *p_value);
gev_err_t gev_get_scheduled_action_time_low(GEV_HANDLE handle, u32 *p_value);
gev_err_t gev_set_multicast_ip(GEV_HANDLE handle, u32 val);
gev_err_t gev_set_multicast_ttl(GEV_HANDLE handle, u8 val);
gev_err_t gev_set_bs_action_signals_num(GEV_HANDLE handle, u8 val);

u32 read_macspire_gev_ip_reg(u32 offset);
u32 read_macspire_time_cap_drop_csr_ip_reg(u32 offset);
u32 read_macspire_ptp_clock_ip_reg(u32 offset);
u32 read_macspire_eth_tse_ip_reg(u32 offset);

gev_err_t gev_write_macspire_bs_reg(GEV_HANDLE handle, u32 addr, u32 data);

gev_err_t gev_set_frm_mem_param(GEV_HANDLE handle, u32 rng_start_addr, u32 rng_size, gev_frm_mem_param_t *p_param);
gev_err_t gev_get_frm_mem_param(GEV_HANDLE handle, u32 *p_rng_start_addr, u32 *p_size, gev_frm_mem_param_t *p_param);
gev_err_t gev_set_debug_mode(GEV_HANDLE handle, gev_debug_mode_t debug_mode);

gev_err_t gev_set_bs_timestamp_tick_high(GEV_HANDLE handle, u32 val);
gev_err_t gev_set_bs_timestamp_tick_low(GEV_HANDLE handle, u32 val);
gev_err_t gev_set_tx_clk_freq(GEV_HANDLE handle, u32 val);

gev_err_t gev_set_ref_timestamp_param(GEV_HANDLE handle, bool sel);
gev_err_t gev_get_ref_timestamp_param(GEV_HANDLE handle, bool *p_sel);
gev_err_t gev_set_avm_clk_freq(GEV_HANDLE handle, u32 val);

gev_err_t gev_set_ptp_init_val(GEV_HANDLE handle, ptp_param_t ptp_param);
gev_err_t gev_ptp_set_master_time(GEV_HANDLE handle, u32 low, u32 high);
gev_err_t gev_ptp_set_priority1(GEV_HANDLE handle, u8 num);
gev_err_t gev_ptp_set_priority2(GEV_HANDLE handle, u8 num);
gev_err_t gev_ptp_set_domain_number(GEV_HANDLE handle, u8 num);
gev_err_t gev_ptp_set_slave_only(GEV_HANDLE handle, bool flag);
gev_err_t gev_ptp_set_log_announce_interval(GEV_HANDLE handle, s8 num);
gev_err_t gev_ptp_set_log_sync_interval(GEV_HANDLE handle, s8 num);
gev_err_t gev_ptp_set_log_min_delay_req_interval(GEV_HANDLE handle, s8 num);
gev_err_t gev_ptp_set_announce_receipt_timeout(GEV_HANDLE handle, u8 num);

gev_err_t gev_set_local_time(GEV_HANDLE handle, u32 low_val, u32 high_val);

gev_err_t gev_set_gev_scps_packet_size(GEV_HANDLE handle, u32 value);
gev_err_t gev_set_scpd(GEV_HANDLE handle, u32 value);
gev_err_t gev_mcda_scda_check_process(GEV_HANDLE handle, bool val);
gev_err_t gev_accept_broadcast_writereg(GEV_HANDLE handle, bool val);
gev_err_t gev_dhcp_para_req_list_opt(GEV_HANDLE handle, bool val);
gev_err_t gev_accept_lla_false_access(GEV_HANDLE handle, bool val);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !define(__GEV_API_H__)
