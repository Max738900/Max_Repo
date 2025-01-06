/*
 * system.h - SOPC Builder system and BSP software package information
 *
 * Machine generated for CPU 'nios2_gen2' in SOPC Builder design 'gev_qsys'
 * SOPC Builder design path: ../../gev_qsys.sopcinfo
 *
 * Generated: Fri Apr 09 02:29:24 JST 2021
 */

/*
 * DO NOT MODIFY THIS FILE
 *
 * Changing this file will have subtle consequences
 * which will almost certainly lead to a nonfunctioning
 * system. If you do modify this file, be aware that your
 * changes will be overwritten and lost when this file
 * is generated again.
 *
 * DO NOT MODIFY THIS FILE
 */

/*
 * License Agreement
 *
 * Copyright (c) 2008
 * Altera Corporation, San Jose, California, USA.
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * This agreement shall be governed in all respects by the laws of the State
 * of California and by the laws of the United States of America.
 */

#ifndef __SYSTEM_H_
#define __SYSTEM_H_

/* Include definitions from linker script generator */
#include "linker.h"


/*
 * CPU configuration
 *
 */

#define ALT_CPU_ARCHITECTURE "altera_nios2_gen2"
#define ALT_CPU_BIG_ENDIAN 0
#define ALT_CPU_BREAK_ADDR 0x04020820
#define ALT_CPU_CPU_ARCH_NIOS2_R1
#define ALT_CPU_CPU_FREQ 100000000u
#define ALT_CPU_CPU_ID_SIZE 1
#define ALT_CPU_CPU_ID_VALUE 0x00000000
#define ALT_CPU_CPU_IMPLEMENTATION "fast"
#define ALT_CPU_DATA_ADDR_WIDTH 0x1e
#define ALT_CPU_DCACHE_BYPASS_MASK 0x80000000
#define ALT_CPU_DCACHE_LINE_SIZE 32
#define ALT_CPU_DCACHE_LINE_SIZE_LOG2 5
#define ALT_CPU_DCACHE_SIZE 2048
#define ALT_CPU_EXCEPTION_ADDR 0x04018020
#define ALT_CPU_FLASH_ACCELERATOR_LINES 0
#define ALT_CPU_FLASH_ACCELERATOR_LINE_SIZE 0
#define ALT_CPU_FLUSHDA_SUPPORTED
#define ALT_CPU_FREQ 100000000
#define ALT_CPU_HARDWARE_DIVIDE_PRESENT 0
#define ALT_CPU_HARDWARE_MULTIPLY_PRESENT 1
#define ALT_CPU_HARDWARE_MULX_PRESENT 0
#define ALT_CPU_HAS_DEBUG_CORE 1
#define ALT_CPU_HAS_DEBUG_STUB
#define ALT_CPU_HAS_EXTRA_EXCEPTION_INFO
#define ALT_CPU_HAS_ILLEGAL_INSTRUCTION_EXCEPTION
#define ALT_CPU_HAS_JMPI_INSTRUCTION
#define ALT_CPU_ICACHE_LINE_SIZE 32
#define ALT_CPU_ICACHE_LINE_SIZE_LOG2 5
#define ALT_CPU_ICACHE_SIZE 4096
#define ALT_CPU_INITDA_SUPPORTED
#define ALT_CPU_INST_ADDR_WIDTH 0x1e
#define ALT_CPU_NAME "nios2_gen2"
#define ALT_CPU_NUM_OF_SHADOW_REG_SETS 0
#define ALT_CPU_OCI_VERSION 1
#define ALT_CPU_RESET_ADDR 0x00600000


/*
 * CPU configuration (with legacy prefix - don't use these anymore)
 *
 */

#define NIOS2_BIG_ENDIAN 0
#define NIOS2_BREAK_ADDR 0x04020820
#define NIOS2_CPU_ARCH_NIOS2_R1
#define NIOS2_CPU_FREQ 100000000u
#define NIOS2_CPU_ID_SIZE 1
#define NIOS2_CPU_ID_VALUE 0x00000000
#define NIOS2_CPU_IMPLEMENTATION "fast"
#define NIOS2_DATA_ADDR_WIDTH 0x1e
#define NIOS2_DCACHE_BYPASS_MASK 0x80000000
#define NIOS2_DCACHE_LINE_SIZE 32
#define NIOS2_DCACHE_LINE_SIZE_LOG2 5
#define NIOS2_DCACHE_SIZE 2048
#define NIOS2_EXCEPTION_ADDR 0x04018020
#define NIOS2_FLASH_ACCELERATOR_LINES 0
#define NIOS2_FLASH_ACCELERATOR_LINE_SIZE 0
#define NIOS2_FLUSHDA_SUPPORTED
#define NIOS2_HARDWARE_DIVIDE_PRESENT 0
#define NIOS2_HARDWARE_MULTIPLY_PRESENT 1
#define NIOS2_HARDWARE_MULX_PRESENT 0
#define NIOS2_HAS_DEBUG_CORE 1
#define NIOS2_HAS_DEBUG_STUB
#define NIOS2_HAS_EXTRA_EXCEPTION_INFO
#define NIOS2_HAS_ILLEGAL_INSTRUCTION_EXCEPTION
#define NIOS2_HAS_JMPI_INSTRUCTION
#define NIOS2_ICACHE_LINE_SIZE 32
#define NIOS2_ICACHE_LINE_SIZE_LOG2 5
#define NIOS2_ICACHE_SIZE 4096
#define NIOS2_INITDA_SUPPORTED
#define NIOS2_INST_ADDR_WIDTH 0x1e
#define NIOS2_NUM_OF_SHADOW_REG_SETS 0
#define NIOS2_OCI_VERSION 1
#define NIOS2_RESET_ADDR 0x00600000


/*
 * Define for each module class mastered by the CPU
 *
 */

#define __ALTERA_AVALON_JTAG_UART
#define __ALTERA_AVALON_ONCHIP_MEMORY2
#define __ALTERA_AVALON_PIO
#define __ALTERA_AVALON_TIMER
#define __ALTERA_EPCQ_CONTROLLER
#define __ALTERA_ETH_TSE
#define __ALTERA_MEM_IF_DDR3_EMIF
#define __ALTERA_NIOS2_GEN2
#define __GEV_CORE_TOP
#define __PTP_CLOCK
#define __TIME_CAP_DROP


/*
 * System configuration
 *
 */

#define ALT_DEVICE_FAMILY "Cyclone V"
#define ALT_ENHANCED_INTERRUPT_API_PRESENT
#define ALT_IRQ_BASE NULL
#define ALT_LOG_PORT "/dev/null"
#define ALT_LOG_PORT_BASE 0x0
#define ALT_LOG_PORT_DEV null
#define ALT_LOG_PORT_TYPE ""
#define ALT_NUM_EXTERNAL_INTERRUPT_CONTROLLERS 0
#define ALT_NUM_INTERNAL_INTERRUPT_CONTROLLERS 1
#define ALT_NUM_INTERRUPT_CONTROLLERS 1
#define ALT_STDERR "/dev/jtag_uart"
#define ALT_STDERR_BASE 0x4022880
#define ALT_STDERR_DEV jtag_uart
#define ALT_STDERR_IS_JTAG_UART
#define ALT_STDERR_PRESENT
#define ALT_STDERR_TYPE "altera_avalon_jtag_uart"
#define ALT_STDIN "/dev/jtag_uart"
#define ALT_STDIN_BASE 0x4022880
#define ALT_STDIN_DEV jtag_uart
#define ALT_STDIN_IS_JTAG_UART
#define ALT_STDIN_PRESENT
#define ALT_STDIN_TYPE "altera_avalon_jtag_uart"
#define ALT_STDOUT "/dev/jtag_uart"
#define ALT_STDOUT_BASE 0x4022880
#define ALT_STDOUT_DEV jtag_uart
#define ALT_STDOUT_IS_JTAG_UART
#define ALT_STDOUT_PRESENT
#define ALT_STDOUT_TYPE "altera_avalon_jtag_uart"
#define ALT_SYSTEM_NAME "gev_qsys"


/*
 * emac configuration
 *
 */

#define ALT_MODULE_CLASS_emac altera_eth_tse
#define EMAC_BASE 0x4021000
#define EMAC_ENABLE_MACLITE 0
#define EMAC_FIFO_WIDTH 32
#define EMAC_IRQ -1
#define EMAC_IRQ_INTERRUPT_CONTROLLER_ID -1
#define EMAC_IS_MULTICHANNEL_MAC 0
#define EMAC_MACLITE_GIGE 0
#define EMAC_MDIO_SHARED 0
#define EMAC_NAME "/dev/emac"
#define EMAC_NUMBER_OF_CHANNEL 1
#define EMAC_NUMBER_OF_MAC_MDIO_SHARED 1
#define EMAC_PCS 0
#define EMAC_PCS_ID 0
#define EMAC_PCS_SGMII 1
#define EMAC_RECEIVE_FIFO_DEPTH 1024
#define EMAC_REGISTER_SHARED 0
#define EMAC_RGMII 0
#define EMAC_SPAN 1024
#define EMAC_TRANSMIT_FIFO_DEPTH 64
#define EMAC_TYPE "altera_eth_tse"
#define EMAC_USE_MDIO 1


/*
 * emif configuration
 *
 */

#define ALT_MODULE_CLASS_emif altera_mem_if_ddr3_emif
#define EMIF_BASE 0x20000000
#define EMIF_IRQ -1
#define EMIF_IRQ_INTERRUPT_CONTROLLER_ID -1
#define EMIF_NAME "/dev/emif"
#define EMIF_SPAN 268435456
#define EMIF_TYPE "altera_mem_if_ddr3_emif"


/*
 * gev_hwip_avs0_fpga_reg configuration
 *
 */

#define ALT_MODULE_CLASS_gev_hwip_avs0_fpga_reg gev_core_top
#define GEV_HWIP_AVS0_FPGA_REG_BASE 0x4022400
#define GEV_HWIP_AVS0_FPGA_REG_IRQ 4
#define GEV_HWIP_AVS0_FPGA_REG_IRQ_INTERRUPT_CONTROLLER_ID 0
#define GEV_HWIP_AVS0_FPGA_REG_NAME "/dev/gev_hwip_avs0_fpga_reg"
#define GEV_HWIP_AVS0_FPGA_REG_SPAN 512
#define GEV_HWIP_AVS0_FPGA_REG_TYPE "gev_core_top"


/*
 * gev_hwip_avs1_tx_gvsp configuration
 *
 */

#define ALT_MODULE_CLASS_gev_hwip_avs1_tx_gvsp gev_core_top
#define GEV_HWIP_AVS1_TX_GVSP_BASE 0x4022000
#define GEV_HWIP_AVS1_TX_GVSP_IRQ -1
#define GEV_HWIP_AVS1_TX_GVSP_IRQ_INTERRUPT_CONTROLLER_ID -1
#define GEV_HWIP_AVS1_TX_GVSP_NAME "/dev/gev_hwip_avs1_tx_gvsp"
#define GEV_HWIP_AVS1_TX_GVSP_SPAN 1024
#define GEV_HWIP_AVS1_TX_GVSP_TYPE "gev_core_top"


/*
 * gev_hwip_avs2_tx_gvcp configuration
 *
 */

#define ALT_MODULE_CLASS_gev_hwip_avs2_tx_gvcp gev_core_top
#define GEV_HWIP_AVS2_TX_GVCP_BASE 0x4021c00
#define GEV_HWIP_AVS2_TX_GVCP_IRQ -1
#define GEV_HWIP_AVS2_TX_GVCP_IRQ_INTERRUPT_CONTROLLER_ID -1
#define GEV_HWIP_AVS2_TX_GVCP_NAME "/dev/gev_hwip_avs2_tx_gvcp"
#define GEV_HWIP_AVS2_TX_GVCP_SPAN 1024
#define GEV_HWIP_AVS2_TX_GVCP_TYPE "gev_core_top"


/*
 * gev_hwip_avs3_rx_packet configuration
 *
 */

#define ALT_MODULE_CLASS_gev_hwip_avs3_rx_packet gev_core_top
#define GEV_HWIP_AVS3_RX_PACKET_BASE 0x4021800
#define GEV_HWIP_AVS3_RX_PACKET_IRQ -1
#define GEV_HWIP_AVS3_RX_PACKET_IRQ_INTERRUPT_CONTROLLER_ID -1
#define GEV_HWIP_AVS3_RX_PACKET_NAME "/dev/gev_hwip_avs3_rx_packet"
#define GEV_HWIP_AVS3_RX_PACKET_SPAN 1024
#define GEV_HWIP_AVS3_RX_PACKET_TYPE "gev_core_top"


/*
 * gev_hwip_avs4_tx_chunk configuration
 *
 */

#define ALT_MODULE_CLASS_gev_hwip_avs4_tx_chunk gev_core_top
#define GEV_HWIP_AVS4_TX_CHUNK_BASE 0x4021400
#define GEV_HWIP_AVS4_TX_CHUNK_IRQ -1
#define GEV_HWIP_AVS4_TX_CHUNK_IRQ_INTERRUPT_CONTROLLER_ID -1
#define GEV_HWIP_AVS4_TX_CHUNK_NAME "/dev/gev_hwip_avs4_tx_chunk"
#define GEV_HWIP_AVS4_TX_CHUNK_SPAN 1024
#define GEV_HWIP_AVS4_TX_CHUNK_TYPE "gev_core_top"


/*
 * gev_pio_i_dip_sw configuration
 *
 */

#define ALT_MODULE_CLASS_gev_pio_i_dip_sw altera_avalon_pio
#define GEV_PIO_I_DIP_SW_BASE 0x2000030
#define GEV_PIO_I_DIP_SW_BIT_CLEARING_EDGE_REGISTER 0
#define GEV_PIO_I_DIP_SW_BIT_MODIFYING_OUTPUT_REGISTER 0
#define GEV_PIO_I_DIP_SW_CAPTURE 0
#define GEV_PIO_I_DIP_SW_DATA_WIDTH 4
#define GEV_PIO_I_DIP_SW_DO_TEST_BENCH_WIRING 0
#define GEV_PIO_I_DIP_SW_DRIVEN_SIM_VALUE 0
#define GEV_PIO_I_DIP_SW_EDGE_TYPE "NONE"
#define GEV_PIO_I_DIP_SW_FREQ 25000000
#define GEV_PIO_I_DIP_SW_HAS_IN 1
#define GEV_PIO_I_DIP_SW_HAS_OUT 0
#define GEV_PIO_I_DIP_SW_HAS_TRI 0
#define GEV_PIO_I_DIP_SW_IRQ -1
#define GEV_PIO_I_DIP_SW_IRQ_INTERRUPT_CONTROLLER_ID -1
#define GEV_PIO_I_DIP_SW_IRQ_TYPE "NONE"
#define GEV_PIO_I_DIP_SW_NAME "/dev/gev_pio_i_dip_sw"
#define GEV_PIO_I_DIP_SW_RESET_VALUE 0
#define GEV_PIO_I_DIP_SW_SPAN 16
#define GEV_PIO_I_DIP_SW_TYPE "altera_avalon_pio"


/*
 * gev_pio_i_push_sw configuration
 *
 */

#define ALT_MODULE_CLASS_gev_pio_i_push_sw altera_avalon_pio
#define GEV_PIO_I_PUSH_SW_BASE 0x2000020
#define GEV_PIO_I_PUSH_SW_BIT_CLEARING_EDGE_REGISTER 0
#define GEV_PIO_I_PUSH_SW_BIT_MODIFYING_OUTPUT_REGISTER 0
#define GEV_PIO_I_PUSH_SW_CAPTURE 0
#define GEV_PIO_I_PUSH_SW_DATA_WIDTH 2
#define GEV_PIO_I_PUSH_SW_DO_TEST_BENCH_WIRING 0
#define GEV_PIO_I_PUSH_SW_DRIVEN_SIM_VALUE 0
#define GEV_PIO_I_PUSH_SW_EDGE_TYPE "NONE"
#define GEV_PIO_I_PUSH_SW_FREQ 25000000
#define GEV_PIO_I_PUSH_SW_HAS_IN 1
#define GEV_PIO_I_PUSH_SW_HAS_OUT 0
#define GEV_PIO_I_PUSH_SW_HAS_TRI 0
#define GEV_PIO_I_PUSH_SW_IRQ -1
#define GEV_PIO_I_PUSH_SW_IRQ_INTERRUPT_CONTROLLER_ID -1
#define GEV_PIO_I_PUSH_SW_IRQ_TYPE "NONE"
#define GEV_PIO_I_PUSH_SW_NAME "/dev/gev_pio_i_push_sw"
#define GEV_PIO_I_PUSH_SW_RESET_VALUE 0
#define GEV_PIO_I_PUSH_SW_SPAN 16
#define GEV_PIO_I_PUSH_SW_TYPE "altera_avalon_pio"


/*
 * gev_pio_o_height configuration
 *
 */

#define ALT_MODULE_CLASS_gev_pio_o_height altera_avalon_pio
#define GEV_PIO_O_HEIGHT_BASE 0x2000080
#define GEV_PIO_O_HEIGHT_BIT_CLEARING_EDGE_REGISTER 0
#define GEV_PIO_O_HEIGHT_BIT_MODIFYING_OUTPUT_REGISTER 0
#define GEV_PIO_O_HEIGHT_CAPTURE 0
#define GEV_PIO_O_HEIGHT_DATA_WIDTH 32
#define GEV_PIO_O_HEIGHT_DO_TEST_BENCH_WIRING 0
#define GEV_PIO_O_HEIGHT_DRIVEN_SIM_VALUE 0
#define GEV_PIO_O_HEIGHT_EDGE_TYPE "NONE"
#define GEV_PIO_O_HEIGHT_FREQ 25000000
#define GEV_PIO_O_HEIGHT_HAS_IN 0
#define GEV_PIO_O_HEIGHT_HAS_OUT 1
#define GEV_PIO_O_HEIGHT_HAS_TRI 0
#define GEV_PIO_O_HEIGHT_IRQ -1
#define GEV_PIO_O_HEIGHT_IRQ_INTERRUPT_CONTROLLER_ID -1
#define GEV_PIO_O_HEIGHT_IRQ_TYPE "NONE"
#define GEV_PIO_O_HEIGHT_NAME "/dev/gev_pio_o_height"
#define GEV_PIO_O_HEIGHT_RESET_VALUE 0
#define GEV_PIO_O_HEIGHT_SPAN 16
#define GEV_PIO_O_HEIGHT_TYPE "altera_avalon_pio"


/*
 * gev_pio_o_height_max configuration
 *
 */

#define ALT_MODULE_CLASS_gev_pio_o_height_max altera_avalon_pio
#define GEV_PIO_O_HEIGHT_MAX_BASE 0x2000070
#define GEV_PIO_O_HEIGHT_MAX_BIT_CLEARING_EDGE_REGISTER 0
#define GEV_PIO_O_HEIGHT_MAX_BIT_MODIFYING_OUTPUT_REGISTER 0
#define GEV_PIO_O_HEIGHT_MAX_CAPTURE 0
#define GEV_PIO_O_HEIGHT_MAX_DATA_WIDTH 32
#define GEV_PIO_O_HEIGHT_MAX_DO_TEST_BENCH_WIRING 0
#define GEV_PIO_O_HEIGHT_MAX_DRIVEN_SIM_VALUE 0
#define GEV_PIO_O_HEIGHT_MAX_EDGE_TYPE "NONE"
#define GEV_PIO_O_HEIGHT_MAX_FREQ 25000000
#define GEV_PIO_O_HEIGHT_MAX_HAS_IN 0
#define GEV_PIO_O_HEIGHT_MAX_HAS_OUT 1
#define GEV_PIO_O_HEIGHT_MAX_HAS_TRI 0
#define GEV_PIO_O_HEIGHT_MAX_IRQ -1
#define GEV_PIO_O_HEIGHT_MAX_IRQ_INTERRUPT_CONTROLLER_ID -1
#define GEV_PIO_O_HEIGHT_MAX_IRQ_TYPE "NONE"
#define GEV_PIO_O_HEIGHT_MAX_NAME "/dev/gev_pio_o_height_max"
#define GEV_PIO_O_HEIGHT_MAX_RESET_VALUE 0
#define GEV_PIO_O_HEIGHT_MAX_SPAN 16
#define GEV_PIO_O_HEIGHT_MAX_TYPE "altera_avalon_pio"


/*
 * gev_pio_o_offset_x configuration
 *
 */

#define ALT_MODULE_CLASS_gev_pio_o_offset_x altera_avalon_pio
#define GEV_PIO_O_OFFSET_X_BASE 0x2000090
#define GEV_PIO_O_OFFSET_X_BIT_CLEARING_EDGE_REGISTER 0
#define GEV_PIO_O_OFFSET_X_BIT_MODIFYING_OUTPUT_REGISTER 0
#define GEV_PIO_O_OFFSET_X_CAPTURE 0
#define GEV_PIO_O_OFFSET_X_DATA_WIDTH 32
#define GEV_PIO_O_OFFSET_X_DO_TEST_BENCH_WIRING 0
#define GEV_PIO_O_OFFSET_X_DRIVEN_SIM_VALUE 0
#define GEV_PIO_O_OFFSET_X_EDGE_TYPE "NONE"
#define GEV_PIO_O_OFFSET_X_FREQ 25000000
#define GEV_PIO_O_OFFSET_X_HAS_IN 0
#define GEV_PIO_O_OFFSET_X_HAS_OUT 1
#define GEV_PIO_O_OFFSET_X_HAS_TRI 0
#define GEV_PIO_O_OFFSET_X_IRQ -1
#define GEV_PIO_O_OFFSET_X_IRQ_INTERRUPT_CONTROLLER_ID -1
#define GEV_PIO_O_OFFSET_X_IRQ_TYPE "NONE"
#define GEV_PIO_O_OFFSET_X_NAME "/dev/gev_pio_o_offset_x"
#define GEV_PIO_O_OFFSET_X_RESET_VALUE 0
#define GEV_PIO_O_OFFSET_X_SPAN 16
#define GEV_PIO_O_OFFSET_X_TYPE "altera_avalon_pio"


/*
 * gev_pio_o_offset_y configuration
 *
 */

#define ALT_MODULE_CLASS_gev_pio_o_offset_y altera_avalon_pio
#define GEV_PIO_O_OFFSET_Y_BASE 0x2000060
#define GEV_PIO_O_OFFSET_Y_BIT_CLEARING_EDGE_REGISTER 0
#define GEV_PIO_O_OFFSET_Y_BIT_MODIFYING_OUTPUT_REGISTER 0
#define GEV_PIO_O_OFFSET_Y_CAPTURE 0
#define GEV_PIO_O_OFFSET_Y_DATA_WIDTH 32
#define GEV_PIO_O_OFFSET_Y_DO_TEST_BENCH_WIRING 0
#define GEV_PIO_O_OFFSET_Y_DRIVEN_SIM_VALUE 0
#define GEV_PIO_O_OFFSET_Y_EDGE_TYPE "NONE"
#define GEV_PIO_O_OFFSET_Y_FREQ 25000000
#define GEV_PIO_O_OFFSET_Y_HAS_IN 0
#define GEV_PIO_O_OFFSET_Y_HAS_OUT 1
#define GEV_PIO_O_OFFSET_Y_HAS_TRI 0
#define GEV_PIO_O_OFFSET_Y_IRQ -1
#define GEV_PIO_O_OFFSET_Y_IRQ_INTERRUPT_CONTROLLER_ID -1
#define GEV_PIO_O_OFFSET_Y_IRQ_TYPE "NONE"
#define GEV_PIO_O_OFFSET_Y_NAME "/dev/gev_pio_o_offset_y"
#define GEV_PIO_O_OFFSET_Y_RESET_VALUE 0
#define GEV_PIO_O_OFFSET_Y_SPAN 16
#define GEV_PIO_O_OFFSET_Y_TYPE "altera_avalon_pio"


/*
 * gev_pio_o_payload_type configuration
 *
 */

#define ALT_MODULE_CLASS_gev_pio_o_payload_type altera_avalon_pio
#define GEV_PIO_O_PAYLOAD_TYPE_BASE 0x2000040
#define GEV_PIO_O_PAYLOAD_TYPE_BIT_CLEARING_EDGE_REGISTER 0
#define GEV_PIO_O_PAYLOAD_TYPE_BIT_MODIFYING_OUTPUT_REGISTER 0
#define GEV_PIO_O_PAYLOAD_TYPE_CAPTURE 0
#define GEV_PIO_O_PAYLOAD_TYPE_DATA_WIDTH 16
#define GEV_PIO_O_PAYLOAD_TYPE_DO_TEST_BENCH_WIRING 0
#define GEV_PIO_O_PAYLOAD_TYPE_DRIVEN_SIM_VALUE 0
#define GEV_PIO_O_PAYLOAD_TYPE_EDGE_TYPE "NONE"
#define GEV_PIO_O_PAYLOAD_TYPE_FREQ 25000000
#define GEV_PIO_O_PAYLOAD_TYPE_HAS_IN 0
#define GEV_PIO_O_PAYLOAD_TYPE_HAS_OUT 1
#define GEV_PIO_O_PAYLOAD_TYPE_HAS_TRI 0
#define GEV_PIO_O_PAYLOAD_TYPE_IRQ -1
#define GEV_PIO_O_PAYLOAD_TYPE_IRQ_INTERRUPT_CONTROLLER_ID -1
#define GEV_PIO_O_PAYLOAD_TYPE_IRQ_TYPE "NONE"
#define GEV_PIO_O_PAYLOAD_TYPE_NAME "/dev/gev_pio_o_payload_type"
#define GEV_PIO_O_PAYLOAD_TYPE_RESET_VALUE 0
#define GEV_PIO_O_PAYLOAD_TYPE_SPAN 16
#define GEV_PIO_O_PAYLOAD_TYPE_TYPE "altera_avalon_pio"


/*
 * gev_pio_o_pixel_format configuration
 *
 */

#define ALT_MODULE_CLASS_gev_pio_o_pixel_format altera_avalon_pio
#define GEV_PIO_O_PIXEL_FORMAT_BASE 0x2000050
#define GEV_PIO_O_PIXEL_FORMAT_BIT_CLEARING_EDGE_REGISTER 0
#define GEV_PIO_O_PIXEL_FORMAT_BIT_MODIFYING_OUTPUT_REGISTER 0
#define GEV_PIO_O_PIXEL_FORMAT_CAPTURE 0
#define GEV_PIO_O_PIXEL_FORMAT_DATA_WIDTH 32
#define GEV_PIO_O_PIXEL_FORMAT_DO_TEST_BENCH_WIRING 0
#define GEV_PIO_O_PIXEL_FORMAT_DRIVEN_SIM_VALUE 0
#define GEV_PIO_O_PIXEL_FORMAT_EDGE_TYPE "NONE"
#define GEV_PIO_O_PIXEL_FORMAT_FREQ 25000000
#define GEV_PIO_O_PIXEL_FORMAT_HAS_IN 0
#define GEV_PIO_O_PIXEL_FORMAT_HAS_OUT 1
#define GEV_PIO_O_PIXEL_FORMAT_HAS_TRI 0
#define GEV_PIO_O_PIXEL_FORMAT_IRQ -1
#define GEV_PIO_O_PIXEL_FORMAT_IRQ_INTERRUPT_CONTROLLER_ID -1
#define GEV_PIO_O_PIXEL_FORMAT_IRQ_TYPE "NONE"
#define GEV_PIO_O_PIXEL_FORMAT_NAME "/dev/gev_pio_o_pixel_format"
#define GEV_PIO_O_PIXEL_FORMAT_RESET_VALUE 0
#define GEV_PIO_O_PIXEL_FORMAT_SPAN 16
#define GEV_PIO_O_PIXEL_FORMAT_TYPE "altera_avalon_pio"


/*
 * gev_pio_o_width configuration
 *
 */

#define ALT_MODULE_CLASS_gev_pio_o_width altera_avalon_pio
#define GEV_PIO_O_WIDTH_BASE 0x20000b0
#define GEV_PIO_O_WIDTH_BIT_CLEARING_EDGE_REGISTER 0
#define GEV_PIO_O_WIDTH_BIT_MODIFYING_OUTPUT_REGISTER 0
#define GEV_PIO_O_WIDTH_CAPTURE 0
#define GEV_PIO_O_WIDTH_DATA_WIDTH 32
#define GEV_PIO_O_WIDTH_DO_TEST_BENCH_WIRING 0
#define GEV_PIO_O_WIDTH_DRIVEN_SIM_VALUE 0
#define GEV_PIO_O_WIDTH_EDGE_TYPE "NONE"
#define GEV_PIO_O_WIDTH_FREQ 25000000
#define GEV_PIO_O_WIDTH_HAS_IN 0
#define GEV_PIO_O_WIDTH_HAS_OUT 1
#define GEV_PIO_O_WIDTH_HAS_TRI 0
#define GEV_PIO_O_WIDTH_IRQ -1
#define GEV_PIO_O_WIDTH_IRQ_INTERRUPT_CONTROLLER_ID -1
#define GEV_PIO_O_WIDTH_IRQ_TYPE "NONE"
#define GEV_PIO_O_WIDTH_NAME "/dev/gev_pio_o_width"
#define GEV_PIO_O_WIDTH_RESET_VALUE 0
#define GEV_PIO_O_WIDTH_SPAN 16
#define GEV_PIO_O_WIDTH_TYPE "altera_avalon_pio"


/*
 * gev_pio_o_width_max configuration
 *
 */

#define ALT_MODULE_CLASS_gev_pio_o_width_max altera_avalon_pio
#define GEV_PIO_O_WIDTH_MAX_BASE 0x20000a0
#define GEV_PIO_O_WIDTH_MAX_BIT_CLEARING_EDGE_REGISTER 0
#define GEV_PIO_O_WIDTH_MAX_BIT_MODIFYING_OUTPUT_REGISTER 0
#define GEV_PIO_O_WIDTH_MAX_CAPTURE 0
#define GEV_PIO_O_WIDTH_MAX_DATA_WIDTH 32
#define GEV_PIO_O_WIDTH_MAX_DO_TEST_BENCH_WIRING 0
#define GEV_PIO_O_WIDTH_MAX_DRIVEN_SIM_VALUE 0
#define GEV_PIO_O_WIDTH_MAX_EDGE_TYPE "NONE"
#define GEV_PIO_O_WIDTH_MAX_FREQ 25000000
#define GEV_PIO_O_WIDTH_MAX_HAS_IN 0
#define GEV_PIO_O_WIDTH_MAX_HAS_OUT 1
#define GEV_PIO_O_WIDTH_MAX_HAS_TRI 0
#define GEV_PIO_O_WIDTH_MAX_IRQ -1
#define GEV_PIO_O_WIDTH_MAX_IRQ_INTERRUPT_CONTROLLER_ID -1
#define GEV_PIO_O_WIDTH_MAX_IRQ_TYPE "NONE"
#define GEV_PIO_O_WIDTH_MAX_NAME "/dev/gev_pio_o_width_max"
#define GEV_PIO_O_WIDTH_MAX_RESET_VALUE 0
#define GEV_PIO_O_WIDTH_MAX_SPAN 16
#define GEV_PIO_O_WIDTH_MAX_TYPE "altera_avalon_pio"


/*
 * hal configuration
 *
 */

#define ALT_INCLUDE_INSTRUCTION_RELATED_EXCEPTION_API
#define ALT_MAX_FD 32
#define ALT_SYS_CLK SYS_TIMER
#define ALT_TIMESTAMP_CLK none


/*
 * jtag_uart configuration
 *
 */

#define ALT_MODULE_CLASS_jtag_uart altera_avalon_jtag_uart
#define JTAG_UART_BASE 0x4022880
#define JTAG_UART_IRQ 0
#define JTAG_UART_IRQ_INTERRUPT_CONTROLLER_ID 0
#define JTAG_UART_NAME "/dev/jtag_uart"
#define JTAG_UART_READ_DEPTH 64
#define JTAG_UART_READ_THRESHOLD 8
#define JTAG_UART_SPAN 8
#define JTAG_UART_TYPE "altera_avalon_jtag_uart"
#define JTAG_UART_WRITE_DEPTH 64
#define JTAG_UART_WRITE_THRESHOLD 8


/*
 * onchip_mem configuration
 *
 */

#define ALT_MODULE_CLASS_onchip_mem altera_avalon_onchip_memory2
#define ONCHIP_MEM_ALLOW_IN_SYSTEM_MEMORY_CONTENT_EDITOR 0
#define ONCHIP_MEM_ALLOW_MRAM_SIM_CONTENTS_ONLY_FILE 0
#define ONCHIP_MEM_BASE 0x4018000
#define ONCHIP_MEM_CONTENTS_INFO ""
#define ONCHIP_MEM_DUAL_PORT 0
#define ONCHIP_MEM_GUI_RAM_BLOCK_TYPE "AUTO"
#define ONCHIP_MEM_INIT_CONTENTS_FILE "gev_qsys_onchip_mem"
#define ONCHIP_MEM_INIT_MEM_CONTENT 1
#define ONCHIP_MEM_INSTANCE_ID "NONE"
#define ONCHIP_MEM_IRQ -1
#define ONCHIP_MEM_IRQ_INTERRUPT_CONTROLLER_ID -1
#define ONCHIP_MEM_NAME "/dev/onchip_mem"
#define ONCHIP_MEM_NON_DEFAULT_INIT_FILE_ENABLED 0
#define ONCHIP_MEM_RAM_BLOCK_TYPE "AUTO"
#define ONCHIP_MEM_READ_DURING_WRITE_MODE "DONT_CARE"
#define ONCHIP_MEM_SINGLE_CLOCK_OP 0
#define ONCHIP_MEM_SIZE_MULTIPLE 1
#define ONCHIP_MEM_SIZE_VALUE 32768
#define ONCHIP_MEM_SPAN 32768
#define ONCHIP_MEM_TYPE "altera_avalon_onchip_memory2"
#define ONCHIP_MEM_WRITABLE 1


/*
 * ptp_clock_hwip configuration
 *
 */

#define ALT_MODULE_CLASS_ptp_clock_hwip PTP_CLOCK
#define PTP_CLOCK_HWIP_BASE 0x4022600
#define PTP_CLOCK_HWIP_IRQ 8
#define PTP_CLOCK_HWIP_IRQ_INTERRUPT_CONTROLLER_ID 0
#define PTP_CLOCK_HWIP_NAME "/dev/ptp_clock_hwip"
#define PTP_CLOCK_HWIP_SPAN 256
#define PTP_CLOCK_HWIP_TYPE "PTP_CLOCK"


/*
 * ptp_ts_cap_hwip_csr configuration
 *
 */

#define ALT_MODULE_CLASS_ptp_ts_cap_hwip_csr time_cap_drop
#define PTP_TS_CAP_HWIP_CSR_BASE 0x4022700
#define PTP_TS_CAP_HWIP_CSR_IRQ 7
#define PTP_TS_CAP_HWIP_CSR_IRQ_INTERRUPT_CONTROLLER_ID 0
#define PTP_TS_CAP_HWIP_CSR_NAME "/dev/ptp_ts_cap_hwip_csr"
#define PTP_TS_CAP_HWIP_CSR_SPAN 256
#define PTP_TS_CAP_HWIP_CSR_TYPE "time_cap_drop"


/*
 * ptp_ts_cap_hwip_mem configuration
 *
 */

#define ALT_MODULE_CLASS_ptp_ts_cap_hwip_mem time_cap_drop
#define PTP_TS_CAP_HWIP_MEM_BASE 0x4000000
#define PTP_TS_CAP_HWIP_MEM_IRQ 6
#define PTP_TS_CAP_HWIP_MEM_IRQ_INTERRUPT_CONTROLLER_ID 0
#define PTP_TS_CAP_HWIP_MEM_NAME "/dev/ptp_ts_cap_hwip_mem"
#define PTP_TS_CAP_HWIP_MEM_SPAN 65536
#define PTP_TS_CAP_HWIP_MEM_TYPE "time_cap_drop"


/*
 * sf_ctr_avl_csr configuration
 *
 */

#define ALT_MODULE_CLASS_sf_ctr_avl_csr altera_epcq_controller
#define SF_CTR_AVL_CSR_BASE 0x2000000
#define SF_CTR_AVL_CSR_FLASH_TYPE "EPCQ256"
#define SF_CTR_AVL_CSR_IRQ -1
#define SF_CTR_AVL_CSR_IRQ_INTERRUPT_CONTROLLER_ID -1
#define SF_CTR_AVL_CSR_IS_EPCS 0
#define SF_CTR_AVL_CSR_NAME "/dev/sf_ctr_avl_csr"
#define SF_CTR_AVL_CSR_NUMBER_OF_SECTORS 512
#define SF_CTR_AVL_CSR_PAGE_SIZE 256
#define SF_CTR_AVL_CSR_SECTOR_SIZE 65536
#define SF_CTR_AVL_CSR_SPAN 32
#define SF_CTR_AVL_CSR_SUBSECTOR_SIZE 4096
#define SF_CTR_AVL_CSR_TYPE "altera_epcq_controller"


/*
 * sf_ctr_avl_mem configuration
 *
 */

#define ALT_MODULE_CLASS_sf_ctr_avl_mem altera_epcq_controller
#define SF_CTR_AVL_MEM_BASE 0x0
#define SF_CTR_AVL_MEM_FLASH_TYPE "EPCQ256"
#define SF_CTR_AVL_MEM_IRQ -1
#define SF_CTR_AVL_MEM_IRQ_INTERRUPT_CONTROLLER_ID -1
#define SF_CTR_AVL_MEM_IS_EPCS 0
#define SF_CTR_AVL_MEM_NAME "/dev/sf_ctr_avl_mem"
#define SF_CTR_AVL_MEM_NUMBER_OF_SECTORS 512
#define SF_CTR_AVL_MEM_PAGE_SIZE 256
#define SF_CTR_AVL_MEM_SECTOR_SIZE 65536
#define SF_CTR_AVL_MEM_SPAN 33554432
#define SF_CTR_AVL_MEM_SUBSECTOR_SIZE 4096
#define SF_CTR_AVL_MEM_TYPE "altera_epcq_controller"


/*
 * sys_timer configuration
 *
 */

#define ALT_MODULE_CLASS_sys_timer altera_avalon_timer
#define SYS_TIMER_ALWAYS_RUN 0
#define SYS_TIMER_BASE 0x4022840
#define SYS_TIMER_COUNTER_SIZE 32
#define SYS_TIMER_FIXED_PERIOD 0
#define SYS_TIMER_FREQ 100000000
#define SYS_TIMER_IRQ 1
#define SYS_TIMER_IRQ_INTERRUPT_CONTROLLER_ID 0
#define SYS_TIMER_LOAD_VALUE 99999
#define SYS_TIMER_MULT 0.001
#define SYS_TIMER_NAME "/dev/sys_timer"
#define SYS_TIMER_PERIOD 1
#define SYS_TIMER_PERIOD_UNITS "ms"
#define SYS_TIMER_RESET_OUTPUT 0
#define SYS_TIMER_SNAPSHOT 1
#define SYS_TIMER_SPAN 32
#define SYS_TIMER_TICKS_PER_SEC 1000
#define SYS_TIMER_TIMEOUT_PULSE_OUTPUT 0
#define SYS_TIMER_TYPE "altera_avalon_timer"


/*
 * timer_gev configuration
 *
 */

#define ALT_MODULE_CLASS_timer_gev altera_avalon_timer
#define TIMER_GEV_ALWAYS_RUN 0
#define TIMER_GEV_BASE 0x4022860
#define TIMER_GEV_COUNTER_SIZE 32
#define TIMER_GEV_FIXED_PERIOD 0
#define TIMER_GEV_FREQ 100000000
#define TIMER_GEV_IRQ 2
#define TIMER_GEV_IRQ_INTERRUPT_CONTROLLER_ID 0
#define TIMER_GEV_LOAD_VALUE 99999
#define TIMER_GEV_MULT 0.001
#define TIMER_GEV_NAME "/dev/timer_gev"
#define TIMER_GEV_PERIOD 1
#define TIMER_GEV_PERIOD_UNITS "ms"
#define TIMER_GEV_RESET_OUTPUT 0
#define TIMER_GEV_SNAPSHOT 1
#define TIMER_GEV_SPAN 32
#define TIMER_GEV_TICKS_PER_SEC 1000
#define TIMER_GEV_TIMEOUT_PULSE_OUTPUT 0
#define TIMER_GEV_TYPE "altera_avalon_timer"

#endif /* __SYSTEM_H_ */
