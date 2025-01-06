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
//! @brief  This header file includes bootstrap register definitions.
//=====================================================================================

#pragma once

#if !defined(__BOOTSTRAP_REG_H__)
#define __BOOTSTRAP_REG_H__

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

//------------------------------------------------------------------------------
// define and structures
//------------------------------------------------------------------------------
#define GVBS_ACTION_CMD_INTERVAL 0x10

// Bootstrap register
#define GVBS_VERSION_OFFSET                        0x00000000
#define GVBS_VERSION_MINOR_MASK                    0x0000ffff
#define GVBS_VERSION_MINOR_POS                     0
#define GVBS_VERSION_MAJOR_MASK                    0xffff0000
#define GVBS_VERSION_MAJOR_POS                     16

#define GVBS_DEVICE_MODE_OFFSET                    0x00000004
#define GVBS_DEVICE_MODE_CHARACTER_SET_MASK        0x000000ff
#define GVBS_DEVICE_MODE_CHARACTER_SET_POS         0
#define GVBS_DEVICE_MODE_CHARACTER_SET_UTF8        1
#define GVBS_DEVICE_MODE_CHARACTER_SET_ASCII       2
#define GVBS_DEVICE_MODE_ENDIAN_POS                31

#define GVBS_DEVICE_MAC_ADDRESS_HIGH_OFFSET        0x00000008
#define GVBS_DEVICE_MAC_ADDRESS_LOW_OFFSET         0x0000000c

#define GVBS_SUPPORTED_IP_CONFIGURATION_OFFSET     0x00000010
#define GVBS_CURRENT_IP_CONFIGURATION_OFFSET       0x00000014
#define GVBS_IP_CONFIGURATION_PERSISTENT           (1 << 0)
#define GVBS_IP_CONFIGURATION_DHCP                 (1 << 1)
#define GVBS_IP_CONFIGURATION_LLA                  (1 << 2)
#define GVBS_IP_CONFIGURATION_PG                   (1 << 30)
#define GVBS_IP_CONFIGURATION_PR                   (1 << 31)
#define GVBS_IP_CONFIGURATION_FAIL                 0x80000000

#define GVBS_CURRENT_IP_ADDRESS_OFFSET             0x00000024
#define GVBS_CURRENT_SUBNET_MASK_OFFSET            0x00000034
#define GVBS_CURRENT_GATEWAY_OFFSET                0x00000044

#define GVBS_MANUFACTURER_NAME_OFFSET              0x00000048
#define GVBS_MANUFACTURER_NAME_SIZE                32
#define GVBS_MODEL_NAME_OFFSET                     0x00000068
#define GVBS_MODEL_NAME_SIZE                       32
#define GVBS_DEVICE_VERSION_OFFSET                 0x00000088
#define GVBS_DEVICE_VERSION_SIZE                   32
#define GVBS_MANUFACTURER_INFORMATIONS_OFFSET      0x000000a8
#define GVBS_MANUFACTURER_INFORMATIONS_SIZE        48
#define GVBS_SERIAL_NUMBER_OFFSET                  0x000000d8 // [Optional]
#define GVBS_SERIAL_NUMBER_SIZE                    16
#define GVBS_USER_DEFINED_NAME_OFFSET              0x000000e8 // [Optional]
#define GVBS_USER_DEFINED_NAME_SIZE                16

#define GVBS_XML_URL_0_OFFSET                      0x00000200
#define GVBS_XML_URL_1_OFFSET                      0x00000400
#define GVBS_XML_URL_SIZE                          512

#define GVBS_NUMBER_OF_NETWORK_INTERFACES_OFFSET   0x00000600

#define GVBS_PERSISTENT_IP_ADDRESS_0_OFFSET        0x0000064c // [Optional]
#define GVBS_PERSISTENT_SUBNET_MASK_0_OFFSET       0x0000065c // [Optional]
#define GVBS_PERSISTENT_GATEWAY_0_OFFSET           0x0000066c // [Optional]

//#define GVBS_LINK_SPEED_0_OFFSET                   0x00000670 // [Optional]

#define GVBS_NUMBER_OF_MESSAGE_CHANNELS_OFFSET     0x00000900 // 0~1
#define GVBS_NUMBER_OF_STREAM_CHANNELS_OFFSET      0x00000904 // 1~512
#define GVBS_NUMBER_OF_ACTION_SIGNALS_OFFSET       0x00000908 // [Optional] 0~128

#define GVBS_ACTION_DEVICE_KEY_OFFSET              0x0000090c // [Optional]
#define GVBS_NUMBER_OF_ACTIVE_LINKS_OFFSET         0x00000910 //

#define GVBS_GVSP_CAPABILITY_OFFSET                0x0000092c // [Optional]
#define GVBS_GVSP_CAPABILITY_SUPPORT_SCSP          (1 << 31)
#define GVBS_GVSP_CAPABILITY_SUPPORT_16BIT         (1 << 30)

#define GVBS_MESSAGE_CHANNEL_CAPABILITY_OFFSET     0x00000930 // [Optional]

#define GVBS_GVCP_CAPABILITY_OFFSET                0x00000934
// This register reports the bootstrap device is supported by the control register and channel GVCP arbitrary commands.
// Some of these properties that are supported when setting register GVCP (address 0x0954) is enabled through.
#define GVBS_GVCP_CAPABILITY_CONCATENATION         (1 << 0)   // Support multiple operation in a single message
#define GVBS_GVCP_CAPABILITY_WRITE_MEMORY          (1 << 1)   // Support WRITEMEM_CMD & WRITEMEM_ACK
#define GVBS_GVCP_CAPABILITY_PACKET_RESEND         (1 << 2)   // Support PACKETRESEND_CMD
#define GVBS_GVCP_CAPABILITY_EVENT                 (1 << 3)   // Support EVENT_CMD & EVENT_ACK
#define GVBS_GVCP_CAPABILITY_EVENT_DATA            (1 << 4)   // Support EVENTDATA_CMD & EVENTDATA_ACK
#define GVBS_GVCP_CAPABILITY_PENDING               (1 << 5)   // Support PENDING_ACK
#define GVBS_GVCP_CAPABILITY_ACTION                (1 << 6)   // Support ACTION_CMD & ACTION_ACK
#define GVBS_GVCP_CAPABILITY_SCHEDULED_ACTION      (1 << 17)  // Support Scheduled Action Commands
#define GVBS_GVCP_CAPABILITY_ES2                   (1 << 18)  // Support extended_status_code_for_2_0(ES2)
#define GVBS_GVCP_CAPABILITY_IEEE1588              (1 << 19)  // Support IEEE1588
#define GVBS_GVCP_CAPABILITY_UNCONDITIONAL_ACTION  (1 << 20)  // Support unconditional_ACTION
#define GVBS_GVCP_CAPABILITY_PAS                   (1 << 21)  // Support primary_application_switchover(PAS)
#define GVBS_GVCP_CAPABILITY_EXTENSTION_STATUS     (1 << 22)  // GEV_STATUS_PACKET_RESEND, GEV_STATUS_WRONG_CONFIG, GEV_STATUS_PACKET_NOT_YET_AVAILABLE,
															  // GEV_STATUS_PACKET_AND_PREV_REMOVED_FROM_MEMORY, GEV_STATUS_PACKET_REMOVED_FROM_MEMORY
#define GVBS_GVCP_CAPABILITY_CAN_WRITE_DISCOVERY_ACK_DELAY (1 << 23) // Application can write discovery acknowledge delay register if this is set
#define GVBS_GVCP_CAPABILITY_DISCOVERY_ACK_DELAY   (1 << 24)  // Support discovery acknowledge delay register
#define GVBS_GVCP_CAPABILITY_TEST_DATA             (1 << 25)  // Test packet is filled with data from the generator LFSR.
#define GVBS_GVCP_CAPABILITY_MANIFEST_TABLE        (1 << 26)  // Manifest tables are supported. When an application must be supported and taken an XML device description file
															  // with the manifest table.
#define GVBS_GVCP_CAPABILITY_CCP_APPLICATION_PORT_AND_IP  (1 << 27) // Manifest tables are supported. When an application must be supported and taken an XML device description file
																	// with the manifest table.
#define GVBS_GVCP_CAPABILITY_LINK_SPEED            (1 << 28)  // Support link speed register
#define GVBS_GVCP_CAPABILITY_HEARTBEAT_DISABLED    (1 << 29)  // The heartbeat can be disabled
#define GVBS_GVCP_CAPABILITY_SERIAL_NUMBER         (1 << 30)  // Support serial number register
#define GVBS_GVCP_CAPABILITY_USER_DEFINED_NAME     (1 << 31)  // Support user-defined name

#define GVBS_HEARTBEAT_TIMEOUT_OFFSET              0x00000938 // Heartbeat timeout in millisecond
#define GVBS_TIMESTAMP_TICK_FREQUENCY_HIGH_OFFSET  0x0000093c // [Optional]
#define GVBS_TIMESTAMP_TICK_FREQUENCY_LOW_OFFSET   0x00000940 // [Optional]
#define GVBS_TIMESTAMP_CONTROL_OFFSET              0x00000944 // [Optional]
#define GVBS_TIMESTAMP_CONTROL_RESET               (1 << 0)   // Reset timestamp latched value (0x948/0x94c)
#define GVBS_TIMESTAMP_CONTROL_LATCH               (1 << 1)   // Latch current timestamp in 0x948/0x94c
#define GVBS_TIMESTAMP_LATCHED_VALUE_HIGH_OFFSET   0x00000948 // [Optional]
#define GVBS_TIMESTAMP_LATCHED_VALUE_LOW_OFFSET    0x0000094c // [Optional]

//#define GVBS_DISCOVERY_ACK_DELAY_OFFSET            0x00000950 // [Optional]
// [CR27-41d] If you register late discovery writable ACK, its value must be stored in a persistent non-volatile memory has to be reused when the next reset or power into.
// [CR27-42d] If you register late discovery is supported ACK, the factory default setting of this register up to1000ms (1 second) must be less than or equal to.
#define GVBS_GVCP_CONFIGURATION_OFFSET             0x00000954 // [Optional]
// This optional register to provide additional controls GVCP.
// Additional capability features of these registers GVCP (address 0x0934) must be shown to.
// For example, when this performance is supported, you can use when you disable the heartbeat. Also useful for debugging.
#define GVBS_GVCP_CONFIGURATION_HEARTBEAT_DISABLE    (1 << 0)   // Disable the heartbeat
#define GVBS_GVCP_CONFIGURATION_PENDING              (1 << 1)   // Enable pending acknowledge
#define GVBS_GVCP_CONFIGURATION_EXTENSTION_STATUS    (1 << 2)   // Enable extended_status_code_for_1_1
#define GVBS_GVCP_CONFIGURATION_UNCONDITIONAL_ACTION (1 << 3)   // Enable unconditional action
#define GVBS_GVCP_CONFIGURATION_ES2                  (1 << 18)  // Enable extended_status_code_for_2_0
#define GVBS_GVCP_CONFIGURATION_IEEE1588             (1 << 19)  // Enable IEEE1588

#define GVBS_PENDING_TIMEOUT_OFFSET                0x00000958 // [Optional]

#define GVBS_GVSP_CONFIGURATION_OFFSET             0x00000960 // [Optional]
#define GVBS_GVSP_CONFIGURATION_64BIT_BLK          (1 << 30)

#define GVBS_PHYSICAL_LINK_CAPABILITY_OFFSET       0x00000964 //
#define GVBS_PHYSICAL_LINK_CONFIGURATION_OFFSET    0x00000968 //
#define GVBS_PHYSICAL_LINK_CONFIGURATION_MASK      0x00000003 //

#define GVBS_IEEE_1588_STATUS_OFFSET               0x0000096c //
#define GVBS_IEEE_1588_STATUS_MASK                 0x0000000f //

#define GVBS_SCH_ACT_COM_QUEUE_SIZE_OFFSET         0x00000970 //

#define GVBS_CONTROL_CHANNEL_PRIVILEGE_OFFSET      0x00000a00 // CCP
// This register is used to grant privileges to the application.
// It is possible to control a device that only one application.
// This application can write to the register of the device.
// Other applications can read the registers of the device only when the application does not have the privilege of control rights.
#define GVBS_CONTROL_CHANNEL_PRIVILEGE_EXCLUSIVE   (1 << 0)   // exclusive access
#define GVBS_CONTROL_CHANNEL_PRIVILEGE_CONTROL     (1 << 1)   // control access
#define GVBS_CONTROL_CHANNEL_PRIVILEGE_CSE         (1 << 2)   // control switchover enable
#define GVBS_CONTROL_CHANNEL_PRIVILEGE_CSK_MASK    0xFFFF0000 // control switchover key mask

#define GVBS_PRIMARY_APPLICATION_PORT_OFFSET       0x00000a04 // [Optional]
// UDP source port of the main application.
// This value must be zero if no key applications on the device. (CCP register is equal to 0)
#define GVBS_PRIMARY_APPLICATION_IP_ADDRESS_OFFSET 0x00000a14 // [Optional]
// IPv4 address key applications. MUST be a unicast address.
// This value must be zero if no key applications on the device. (CCP register is equal to 0)

#define GVBS_MCP_OFFSET                            0x00000b00 // Message Control Port [Optional]
// When supported, the application shall activate the channel by writing messages to host_port nonzero field. Otherwise, the closed channel.
#define GVBS_MCP_HOST_PORT_MASK                    0x0000ffff // A message must be sent to the device from the port. A message channel is set to close this value to 0.
#define GVBS_MCP_NETWORK_INTERFACE_INDEX_MASK      0x000f0000 // Always 0

#define GVBS_MCDA_OFFSET                           0x00000b10 // Message Channel Destination Address [Optional]
#define GVBS_MCTT_OFFSET                           0x00000b14 // Message Channel Transmission Time (ms) [Optional]
#define GVBS_MCRC_OFFSET                           0x00000b18 // Message Channel Retry Count [Optional]
#define GVBS_MCSP_OFFSET                           0x00000b1c // Message Channel Source Port [Optional]


//
// Stream Channel Registers
//
#define GVBS_SCP_OFFSET                            0x00000d00 // Stream Channel Port
#define GBBS_SCP_DIRECTION                         (1 << 31)  // the direction of stream channel
#define GVBS_SCP_HOST_PORT_MASK                    0x0000ffff // port that the device must send a data stream. The stream channel and close to zero value.
#define GVBS_SCP_NETWORK_INTERFACE_INDEX_MASK      0x000f0000 // The index of the network interface (3-0). A particular stream is hard coded to a specific network interface.
															  // The devices in this field may not be the program. This is a read-only.

#define GVBS_SCPS_OFFSET                           0x00000d04 // Stream Channel Packet Size
#define GVBS_SCPS_PACKET_SIZE_MASK                 0x0000ffff
#define GVBS_SCPS_PACKET_BIG_ENDIAN                (1 << 29)
#define GVBS_SCPS_DO_NOT_FRAGMENT                  (1 << 30)
#define GVBS_SCPS_EMIT_TEST_PACKET                 (1 << 31)
// When this bit is set, the device emits a single test packet size that is specified in bits 16-31.
// TheIP header "don't fragment" bit must be set for this test packet.
// If the device supports the LFSRgenerator load test packet to be filled with data by a polynomial.
// Otherwise, load data "don't care" becomes.

#define GVBS_SCPD_OFFSET                           0x00000d08 // Stream Channel Packet Delay

// This delay is the same precision instruments utilize the time stamp in order to ensure the accuracy of the normal packet delays.
// This instrument is used very frequently. Internal packet delay is very small.
// If the timestamp is not supported, this register is not effective.

#define GVBS_SCDA_OFFSET                           0x00000d18 // Stream Channel Destination Address
// IPv4 destination address stream channel. The destination address is multicast or unicast address.

#define GVBS_SCSP_OFFSET                           0x00000d1c // Stream Channel Source Port [Optional]
// This optional register indicating the source UDP port for a particular stream channel
// The purpose of this information is to take measures to ensure that the device back to host streaming traffic from the host application.
// Expected usage of the device sends a source port from UDP packet to port the application receives a stream of false, which seem to be
// required by the application rather than a stream of unsolicited traffic by doing so.
//
// [CR27-45d] If supported by the device, SCPSx must return a nonzero value for the source UDP port of the stream channel is not zero when SCSPx.
//            During the control session, as long as the non-zero constant is valid.
// [CR27-46d] If supported by the device, if not zero SCDAx SCSPx not focus on the device and SCPx SCSPx port (if it is open if the stream channel)
// and ignore the UDP traffic coming from the address and port combination that is listed in must be.

// 这是一个可选的寄存器，用于指示特定流信道的源UDP端口 
// 这项信息的目的在于采取措施，确保设备回传到主机的流媒体流量是由主机应用程序发起的。 
// 设备的预期使用方式是发送一个常数，该常数对于设备来说是有效的。 
// [CR27-46d] 如果设备支持，当SCDAx不为零时，不需要关注设备的SCSPx端口（如果是流信道开启的话），并且必须忽略来自SCPx SCSPx端口（如果它是开放的流信道）的地址和端口组合的UDP流量。 
// 如果SCSPx不为零，则SCPSx SCSPx的组合不应聚焦于设备，并且必须忽略列出的地址和端口组合的UDP流量。

#define GVBS_SCC_OFFSET                            0x00000d20 // Stream Channel Capability [Optional]
// Any of these registers gives a list of specific capability in a particular stream channel.
#define GVBS_SCC_EXTENDED_CHUNK_DATA               (1 << 0)
#define GVBS_SCC_EXTENDED_UNCONDITIONAL_STREAMING  (1 << 1)
#define GVBS_SCC_ALL_IN_TRANSMISSION               (1 << 2)
#define GVBS_SCC_PACKET_RESEND_DESTINATION_OPTION  (1 << 3)
#define GVBS_SCC_MULTI_ZONE                        (1 << 4)
#define GVBS_SCC_IP_REASSEMBLY                     (1 << 30)
#define GVBS_SCC_BIG_ENDIAN                        (1 << 31)
// This indicates that the stream channel to support big and little endian.
// Stream channel to keep in mind that all must support the little endian.

#define GVBS_SCCFG_OFFSET                          0x00000d24 // Stream Channel Configuration [Optional]


// For Stream Channel Registers (n = 0 ~ 511)
#define GVBS_SCx_OFFSET(reg, n)                    (reg + 0x40 * n)
// For example:
//    SCP1  -> GVBS_SCx_OFFSET(GVBS_SCP_OFFSET, 1)
//    SCSP2 -> GVBS_SCx_OFFSET(GVBS_SCSP_OFFSET, 2)

#define GVBS_MANIFEST_TABLE_OFFSET                 0x00009000 // [Optional]
#define GVBS_MANIFEST_TABLE_ENTRY_SIZE             8
#define GVBS_MANIFEST_TABLE_ENTRY_MAX              64

#define GVBS_ACTION_GROUP_KEY_0_OFFSET             0x00009800 // [Optional]
#define GVBS_ACTION_GROUP_MASK_0_OFFSET            0x00009804 // [Optional]

#define GVBS_MANUFACTURER_SPECIFIC_SPACE_OFFSET    0x0000A000
#define GVBS_WIDTH_OFFSET                          0x0000A000
#define GVBS_HEIGHT_OFFSET                         0x0000A004
#define GVBS_OFFSET_X_OFFSET                       0x0000A008
#define GVBS_OFFSET_Y_OFFSET                       0x0000A00C

#define GVBS_ACQUISITION_MODE_OFFSET               0x0000A010
#define GVBS_ACQUISITION_MODE_CONTINUOUS           0
#define GVBS_ACQUISITION_MODE_SINGLE               1
#define GVBS_ACQUISITION_MODE_MULTI                3

#define GVBS_ACQUISITION_START_OFFSET              0x0000A014
#define GVBS_ACQUISITION_STOP_OFFSET               0x0000A018
#define GVBS_PIXEL_FORMAT_OFFSET                   0x0000A01C

#define GVBS_PAYLOAD_TYPE_OFFSET                   0x0000A020
#define GVBS_FIELD_COUNT_OFFSET                    0x0000A024
#define GVBS_PAYLOAD_SIZE_OFFSET                   0x0000A028
#define GVBS_GEV_SCSP_PACKET_SIZE_OFFSET           0x0000A02C

#define GVBS_ACQUISITION_FRAME_COUNT_OFFSET        0x0000A030
#define GVBS_RESEND_WAIT_FINAL                     0x0000A034

#define GVBS_RESEND_FRAMES_OFFSET                  0x0000A040
#define GVBS_RAW_SIZE_OFFSET                       0x0000A044
#define GVBS_DEBUG_MODE_OFFSET                     0x0000A048
#define GVBS_DEBUG_MODE_1                          (1 << 0)
#define GVBS_DEBUG_MODE_2                          (1 << 1)
#define GVBS_DEBUG_MODE_3                          (1 << 2)
#define GVBS_DEBUG_MODE_MASK                       0x00000007
#define GVBS_DEBUG_MODE_PATTERN                    (1 << 4)
#define GVBS_DEBUG_MODE_PATTERN_MASK               0x00000030
#define GVBS_DEBUG_MODE_TIMING                     (1 << 6)

// chunk data structure
#define GVBS_CHUNK_NUM_MAX                         0x00000008
#define GVBS_CHUNK_DATA_BLOCK_SIZE                 0x00000010

#define GVBS_CHUNK_DATA_ENABLE_OFFSET              0x00000000
#define GVBS_CHUNK_DATA_ID_OFFSET                  0x00000004
#define GVBS_CHUNK_DATA_SIZE_OFFSET                0x00000008

// chunk data bootstrap register
#define GVBS_EXTENDED_CHUNK_ENABLE_OFFSET          0x0000A100
#define GVSB_CHUNK_BASE_DATA_ID_OFFSET             0x0000A104

#define GVBS_CHUNK_DATA_START                      0x0000A200
#define GVBS_CHUNK_DATA0_ENABLE_OFFSET             0x0000A200
#define GVBS_CHUNK_DATA0_ID_OFFSET                 0x0000A204
#define GVBS_CHUNK_DATA0_SIZE_OFFSET               0x0000A208

#define GVBS_CHUNK_DATA1_ENABLE_OFFSET             0x0000A210
#define GVBS_CHUNK_DATA1_ID_OFFSET                 0x0000A214
#define GVBS_CHUNK_DATA1_SIZE_OFFSET               0x0000A218

#define GVBS_CHUNK_DATA2_ENABLE_OFFSET             0x0000A220
#define GVBS_CHUNK_DATA2_ID_OFFSET                 0x0000A224
#define GVBS_CHUNK_DATA2_SIZE_OFFSET               0x0000A228

#define GVBS_CHUNK_DATA3_ENABLE_OFFSET             0x0000A230
#define GVBS_CHUNK_DATA3_ID_OFFSET                 0x0000A234
#define GVBS_CHUNK_DATA3_SIZE_OFFSET               0x0000A238

#define GVBS_CHUNK_DATA4_ENABLE_OFFSET             0x0000A240
#define GVBS_CHUNK_DATA4_ID_OFFSET                 0x0000A244
#define GVBS_CHUNK_DATA4_SIZE_OFFSET               0x0000A248

#define GVBS_CHUNK_DATA5_ENABLE_OFFSET             0x0000A250
#define GVBS_CHUNK_DATA5_ID_OFFSET                 0x0000A254
#define GVBS_CHUNK_DATA5_SIZE_OFFSET               0x0000A258

#define GVBS_CHUNK_DATA6_ENABLE_OFFSET             0x0000A260
#define GVBS_CHUNK_DATA6_ID_OFFSET                 0x0000A264
#define GVBS_CHUNK_DATA6_SIZE_OFFSET               0x0000A268

#define GVBS_CHUNK_DATA7_ENABLE_OFFSET             0x0000A270
#define GVBS_CHUNK_DATA7_ID_OFFSET                 0x0000A274
#define GVBS_CHUNK_DATA7_SIZE_OFFSET               0x0000A278

#define GVBS_USER_REGISTER_START                   0x0000C000

#define GVBS_MIN_RAW_SIZE                          0x00000004
#define GVBS_MAX_RAW_SIZE                          0x20000000

#define GVBS_INTER_OUT_MODE_OFFSET                 0x0000A300
#define GVBS_INTER_OUT_MODE_OFF                    0x00000000
#define GVBS_INTER_OUT_MODE_ON                     0x00000001
#define GVBS_INTER_OUT_VAL_OFFSET                  0x0000A304

#define GVBS_TRIG_OUTPUT_MODE_OFFSET               0x0000A308
#define GVBS_TRIG_OUTPUT_MODE_OFF                  0x00000000
#define GVBS_TRIG_OUTPUT_MODE_ON                   0x00000001
#define GVBS_TRIG_OUTPUT_START_OFFSET              0x0000A30C
#define GVBS_TRIG_OUTPUT_TRIG                      0x00000001

//------------------------------------------------------------------------------
// global variables
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// prototypes
//------------------------------------------------------------------------------
u16 initialize_bootstrap_regs(void);
void write_bootstrap_reg(u32 offset, u32 value);   //OK
u32 read_bootstrap_reg(u32 offset);                //OK
void copy_to_bootstrap_reg(u32 offset, char *p_data, u32 size);  //OK
void copy_from_bootstrap_reg(u32 offset, char *p_data, u32 size);  //OK
void copy_to_bootstrap_reg_big_little(u32 offset, char *p_data, u32 size);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !defined(__BOOTSTRAP_REG_H__)
