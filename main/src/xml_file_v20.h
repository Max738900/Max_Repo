//=====================================================================================
// Copyright (C) 2014-2021 Macnica Inc. All Rights Reserved.
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
//! @brief
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

//------------------------------------------------------------------------------
// define and structures
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// global variables
//------------------------------------------------------------------------------
u8 g_first_url_v20[]  = "local:BetelmaGiCs_GEVv20_Refernece.xml;200000;2934B";
u8 g_second_url_v20[] = "local:///BetelmaGiCs_GEVv20_Refernece.xml;200000;2934B?SchemaVersion=1.1.0";
u8 g_xml_file_v20_p1[] =
"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n"                                                    \
"<RegisterDescription\r\n"                                                                          \
	" ModelName=\"Macnica_Camera\"\r\n"                                                             \
	" VendorName=\"Macnica\"\r\n"                                                                   \
	" ToolTip=\"Macnica Camera Reference XML\"\r\n"                                                 \
	" StandardNameSpace=\"GEV\"\r\n"                                                                \
	" SchemaMajorVersion=\"1\"\r\n"                                                                 \
	" SchemaMinorVersion=\"1\"\r\n"                                                                 \
	" SchemaSubMinorVersion=\"0\"\r\n"                                                              \
	" MajorVersion=\"3\"\r\n"                                                                       \
	" MinorVersion=\"0\"\r\n"                                                                       \
	" SubMinorVersion=\"1\"\r\n"                                                                    \
	" ProductGuid=\"B27EB5D2-A2D6-4E75-8BB8-5FA0B8D6A7F0\"\r\n"                                     \
	" VersionGuid=\"039276cf-8fa0-475b-b263-86f454a928eb\"\r\n"                                     \
	" xmlns=\"http://www.genicam.org/GenApi/Version_1_1\"\r\n"                                      \
	" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">\r\n"                                 \

	" <Group Comment=\"RootCategory\">\r\n"                                                         \
	"	<Category Name=\"Root\" NameSpace=\"Standard\">\r\n"                                        \
	"		<pFeature>DeviceControl</pFeature>\r\n"                                                 \
	"		<pFeature>ImageFormatControl</pFeature>\r\n"                                            \
	"		<pFeature>AcquisitionControl</pFeature>\r\n"                                            \
	"		<pFeature>TransportLayerControl</pFeature>\r\n"                                         \
	"		<pFeature>ActionControl</pFeature>\r\n"                                                 \
	"		<pFeature>EventControl</pFeature>\r\n"                                                  \
	"		<pFeature>ChunkDataControl</pFeature>\r\n"                                              \
	"		<pFeature>MacnicaOriginal</pFeature>\r\n"                                               \
	"	</Category>\r\n"                                                                            \
	" </Group>\r\n"                                                                                 \
	" <Group Comment=\"SubCategories\">\r\n"                                                        \
	"	<Category Name=\"DeviceControl\" NameSpace=\"Standard\">\r\n"                               \
	"		<ToolTip>Category for device information and control.</ToolTip>\r\n"                    \
	"		<Description>Category for device information and control.</Description>\r\n"            \
	"		<DisplayName>Device Control</DisplayName>\r\n"                                          \
	"		<pFeature>DeviceVendorName</pFeature>\r\n"                                              \
	"		<pFeature>DeviceModelName</pFeature>\r\n"                                               \
	"		<pFeature>DeviceVersion</pFeature>\r\n"                                                 \
	"		<pFeature>DeviceManufacturerInfo</pFeature>\r\n"                                        \
	"		<pFeature>DeviceSerialNumber</pFeature>\r\n"                                            \
	"		<pFeature>DeviceUserID</pFeature>\r\n"                                                  \
	"		<pFeature>DeviceReset</pFeature>\r\n"                                                   \
	"	</Category>\r\n"                                                                            \
	"	<Category Name=\"ImageFormatControl\" NameSpace=\"Standard\">\r\n"                          \
	"		<ToolTip>Category for Image Format Control features.</ToolTip>\r\n"                     \
	"		<Description>Category for Image Format Control features.</Description>\r\n"             \
	"		<DisplayName>Image Format Control</DisplayName>\r\n"                                    \
	"		<pFeature>Width</pFeature>\r\n"                                                         \
	"		<pFeature>Height</pFeature>\r\n"                                                        \
	"		<pFeature>OffsetX</pFeature>\r\n"                                                       \
	"		<pFeature>OffsetY</pFeature>\r\n"                                                       \
	"		<pFeature>PixelFormat</pFeature>\r\n"                                                   \
	"		<pFeature>SensorWidth</pFeature>\r\n"                                                   \
	"		<pFeature>SensorHeight</pFeature>\r\n"                                                  \
	" 	</Category>\r\n"                                                                            \
	" 	<Category Name=\"AcquisitionControl\" NameSpace=\"Standard\">\r\n"                          \
	"		<ToolTip>Category for the acquisition and trigger control features.</ToolTip>\r\n"      \
	"		<Description>Category for the acquisition and trigger control features.</Description>\r\n"   \
	"		<DisplayName>Acquisition Control</DisplayName>\r\n"                                     \
	"		<pFeature>AcquisitionMode</pFeature>\r\n"                                               \
	"		<pFeature>AcquisitionStart</pFeature>\r\n"                                              \
	"		<pFeature>AcquisitionStop</pFeature>\r\n"                                               \
	"		<pFeature>AcquisitionFrameCount</pFeature>\r\n"                                         \
	"	</Category>\r\n"                                                                            \
	"	<Category Name=\"TransportLayerControl\" NameSpace=\"Standard\">\r\n"                       \
	"		<ToolTip>Category that contains the transport Layer control features.</ToolTip>\r\n"    \
	"		<Description>Category that contains the transport Layer control features.</Description>\r\n" \
	"		<DisplayName>Transport Layer Control</DisplayName>\r\n"                                 \
	"		<pFeature>PayloadSize</pFeature>\r\n"                                                   \
	"		<pFeature>GigEVision</pFeature>\r\n"                                                    \
	"	</Category>\r\n"                                                                            \
	"	<Category Name=\"GigEVision\" NameSpace=\"Standard\">\r\n"                                  \
	"		<ToolTip>Category that contains the features pertaining to the GigE Vision transport layer of the device.</ToolTip>\r\n"            \
	"		<Description>Category that contains the features pertaining to the GigE Vision transport layer of the device.</Description>\r\n"    \
	"		<DisplayName>GigE Vision</DisplayName>\r\n"                                             \
	"		<pFeature>GevVersionMajor</pFeature>\r\n"                                               \
	"		<pFeature>GevVersionMinor</pFeature>\r\n"                                               \
	"		<pFeature>GevDeviceModeIsBigEndian</pFeature>\r\n"                                      \
	"		<pFeature>GevDeviceClass</pFeature>\r\n"                                                \
	"		<pFeature>GevDeviceModeCharacterSet</pFeature>\r\n"                                     \
	"		<pFeature>GevInterfaceSelector</pFeature>\r\n"                                          \
	"		<pFeature>GevMACAddress</pFeature>\r\n"                                                 \
	"		<pFeature>GevPAUSEFrameReception</pFeature>\r\n"                                        \
	"		<pFeature>GevPAUSEFrameTransmission</pFeature>\r\n"                                     \
	"		<pFeature>GevCurrentIPConfigurationLLA</pFeature>\r\n"                                  \
	"		<pFeature>GevCurrentIPConfigurationDHCP</pFeature>\r\n"                                 \
	"		<pFeature>GevCurrentIPConfigurationPersistentIP</pFeature>\r\n"                         \
	"		<pFeature>GevCurrentIPAddress</pFeature>\r\n"                                           \
	"		<pFeature>GevCurrentSubnetMask</pFeature>\r\n"                                          \
	"		<pFeature>GevCurrentDefaultGateway</pFeature>\r\n"                                      \
	"		<pFeature>GevFirstURL</pFeature>\r\n"                                                   \
	"		<pFeature>GevSecondURL</pFeature>\r\n"                                                  \
	"		<pFeature>GevNumberOfInterfaces</pFeature>\r\n"                                         \
	"		<pFeature>GevPersistentIPAddress</pFeature>\r\n"                                        \
	"		<pFeature>GevPersistentSubnetMask</pFeature>\r\n"                                       \
	"		<pFeature>GevPersistentDefaultGateway</pFeature>\r\n"                                   \
	"		<pFeature>GevMessageChannelCount</pFeature>\r\n"                                        \
	"		<pFeature>GevStreamChannelCount</pFeature>\r\n"                                         \
	"		<pFeature>GevActiveLinkCount</pFeature>\r\n"                                            \
	"		<pFeature>GevSupportedOptionSelector</pFeature>\r\n"                                    \
	"		<pFeature>GevHeartbeatTimeout</pFeature>\r\n"                                           \
	"		<pFeature>GevTimestampTickFrequency</pFeature>\r\n"                                     \
	"		<pFeature>GevTimestampControlLatch</pFeature>\r\n"                                      \
	"		<pFeature>GevTimestampControlReset</pFeature>\r\n"                                      \
	"		<pFeature>GevTimestampValue</pFeature>\r\n"                                             \
	"		<pFeature>GevIEEE1588</pFeature>\r\n"                                                   \
	"		<pFeature>GevGVCPExtendedStatusCodesSelector</pFeature>\r\n"                            \
	"		<pFeature>GevGVCPExtendedStatusCodes</pFeature>\r\n"                                    \
	"		<pFeature>GevGVCPExtendedStatusCodeVersion2_0</pFeature>\r\n"                           \
	"		<pFeature>GevGVCPExtendedStatusCodeVersion1_1</pFeature>\r\n"                           \
	"		<pFeature>GevGVCPPendingAck</pFeature>\r\n"                                             \
	"		<pFeature>GevGVCPHeartbeatDisable</pFeature>\r\n"                                       \
	"		<pFeature>GevGVCPPendingTimeout</pFeature>\r\n"                                         \
	"		<pFeature>GevPrimaryApplicationSwitchoverKey</pFeature>\r\n"                            \
	"		<pFeature>GevGVSPExtendedIDMode</pFeature>\r\n"                                         \
	"		<pFeature>GevCurrentPhysicalLinkConfiguration</pFeature>\r\n"                           \
	"		<pFeature>GevPhysicalLinkConfiguration</pFeature>\r\n"                                  \
	"		<pFeature>GevIEEE1588Status</pFeature>\r\n"                                             \
	"		<pFeature>GevCCP</pFeature>\r\n"                                                        \
	"		<pFeature>GevPrimaryApplicationSocket</pFeature>\r\n"                                   \
	"		<pFeature>GevPrimaryApplicationIPAddress</pFeature>\r\n"                                \
	"		<pFeature>GevMCPHostPort</pFeature>\r\n"                                                \
	"		<pFeature>GevMCDA</pFeature>\r\n"                                                       \
	"		<pFeature>GevMCTT</pFeature>\r\n"                                                       \
	"		<pFeature>GevMCRC</pFeature>\r\n"                                                       \
	"		<pFeature>GevMCSP</pFeature>\r\n"                                                       \
	"		<pFeature>GevStreamChannelSelector</pFeature>\r\n"                                      \
	"		<pFeature>GevSCPDirection</pFeature>\r\n"                                               \
	"		<pFeature>GevSCPInterfaceIndex</pFeature>\r\n"                                          \
	"		<pFeature>GevSCPHostPort</pFeature>\r\n"                                                \
	"		<pFeature>GevSCPSFireTestPacket</pFeature>\r\n"                                         \
	"		<pFeature>GevSCPSDoNotFragment</pFeature>\r\n"                                          \
	"		<pFeature>GevSCPSBigEndian</pFeature>\r\n"                                              \
	"		<pFeature>GevSCPSPacketSize</pFeature>\r\n"                                             \
	"		<pFeature>GevSCPD</pFeature>\r\n"                                                       \
	"		<pFeature>GevSCDA</pFeature>\r\n"                                                       \
	"		<pFeature>GevSCSP</pFeature>\r\n"                                                       \
	"		<pFeature>GevSCCFGPacketResendDestination</pFeature>\r\n"                               \
	"		<pFeature>GevSCCFGAllInTransmission</pFeature>\r\n"                                     \
	"		<pFeature>GevSCCFGUnconditionalStreaming</pFeature>\r\n"                                \
	"		<pFeature>GevSCCFGExtendedChunkData</pFeature>\r\n"                                     \
	"	</Category>\r\n"                                                                            \
	"	<Category Name=\"ActionControl\" NameSpace=\"Standard\">\r\n"                               \
	"		<ToolTip>Category that contains the Action control features.</ToolTip>\r\n"             \
	"		<Description>Category that contains the Action control features.</Description>\r\n"     \
	"		<DisplayName>Action Control</DisplayName>\r\n"                                          \
	"		<pFeature>ActionDeviceKey</pFeature>\r\n"                                               \
	"		<pFeature>ActionUnconditionalMode</pFeature>\r\n"                                       \
	"		<pFeature>ActionQueueSize</pFeature>\r\n"                                               \
	"		<pFeature>ActionSelector</pFeature>\r\n"                                                \
	"		<pFeature>ActionGroupMask</pFeature>\r\n"                                               \
	"		<pFeature>ActionGroupKey</pFeature>\r\n"                                                \
	"	</Category>\r\n"                                                                            \
	"	<Category Name=\"MacnicaOriginal\" NameSpace=\"Standard\">\r\n"                             \
	"		<ToolTip>Category that contains the Macnica original features.</ToolTip>\r\n"           \
	"		<Description>Category that contains the Macnica original features.</Description>\r\n"   \
	"		<DisplayName>Macnica original</DisplayName>\r\n"                                        \
	"		<pFeature>GevPAUSEFrameReceptionCap</pFeature>\r\n"                                     \
	"		<pFeature>GevPAUSEFrameTransmissionCap</pFeature>\r\n"                                  \
	"		<pFeature>GevCurrentIPConfigurationLLACap</pFeature>\r\n"                               \
	"		<pFeature>GevCurrentIPConfigurationDHCPCap</pFeature>\r\n"                              \
	"		<pFeature>GevCurrentIPConfigurationPersistentIPCap</pFeature>\r\n"                      \
	"		<pFeature>GevNumberOfActionSignals</pFeature>\r\n"                                      \
	"		<pFeature>GevSCSPxSupported</pFeature>\r\n"                                             \
	"		<pFeature>GevLegacy16bitBlockIDSupported</pFeature>\r\n"                                \
	"		<pFeature>GevMCSPxSupported</pFeature>\r\n"                                             \
	"		<pFeature>GevPhysicalLinkConfigurationCap</pFeature>\r\n"                               \
	"		<pFeature>PayloadType</pFeature>\r\n"                                                   \
	"		<pFeature>FieldCount</pFeature>\r\n"                                                    \
	"		<pFeature>ResendWaitFinal</pFeature>\r\n"                                               \
	"		<pFeature>ResendFrame</pFeature>\r\n"                                                   \
	"		<pFeature>PayloadRawSize</pFeature>\r\n"                                                \
	"		<pFeature>DebugMode</pFeature>\r\n"                                                     \
	"		<pFeature>IntervalOutputMode</pFeature>\r\n"                                            \
	"		<pFeature>IntervalOutputCountValue</pFeature>\r\n"                                      \
	"		<pFeature>TriggerOutputMode</pFeature>\r\n"                                             \
	"		<pFeature>TriggerOutputStart</pFeature>\r\n"                                            \
	"		<pFeature>TimestampSelect</pFeature>\r\n"                                               \
	"		<pFeature>PTPSetPriority1</pFeature>\r\n"                                               \
	"		<pFeature>PTPSetPriority2</pFeature>\r\n"                                               \
	"		<pFeature>PTPSetDomainNumber</pFeature>\r\n"                                            \
	"		<pFeature>PTPSetSlaveOnly</pFeature>\r\n"                                               \
	"		<pFeature>PTPSetLogAnnounceInterval</pFeature>\r\n"                                     \
	"		<pFeature>PTPSetLogSyncInterval</pFeature>\r\n"                                         \
	"		<pFeature>PTPSetLogMinDelayReqInterval</pFeature>\r\n"                                  \
	"		<pFeature>PTPSetAnnounceReceiptTimeout</pFeature>\r\n"                                  \
	"		<pFeature>SetTimestampInitialValue</pFeature>\r\n"                                      \
	"		<pFeature>TimestampInitialValueHigh</pFeature>\r\n"                                     \
	"		<pFeature>TimestampInitialValueLow</pFeature>\r\n"                                      \
	"	</Category>\r\n"                                                                            \
	" </Group>\r\n"                                                                                 \
	" <Group Comment=\"DeviceControl\">\r\n"                                                        \
	" <String Name=\"DeviceVendorName\" NameSpace=\"Standard\">\r\n"                                \
	"	<ToolTip>Name of the manufacturer of the device.</ToolTip>\r\n"                             \
	"	<Description>Name of the manufacturer of the device.</Description>\r\n"                     \
	"	<DisplayName>Device Vendor Name</DisplayName>\r\n"                                          \
	"	<Visibility>Beginner</Visibility>\r\n"                                                      \
	"	<ImposedAccessMode>RO</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GEV_BOOTSTRAP_DeviceVendorName</pValue>\r\n"                                        \
	"</String>\r\n"                                                                                 \
	"<String Name=\"DeviceModelName\" NameSpace=\"Standard\">\r\n"                                  \
	"	<ToolTip>Model of the device.</ToolTip>\r\n"                                                \
	"	<Description>Model of the device.</Description>\r\n"                                        \
	"	<DisplayName>Device Model Name</DisplayName>\r\n"                                           \
	"	<Visibility>Beginner</Visibility>\r\n"                                                      \
	"	<ImposedAccessMode>RO</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GEV_BOOTSTRAP_DeviceModelName</pValue>\r\n"                                         \
	"</String>\r\n"                                                                                 \
	"<String Name=\"DeviceVersion\" NameSpace=\"Standard\">\r\n"                                    \
	"	<ToolTip>Version of the device.</ToolTip>\r\n"                                              \
	"	<Description>Version of the device.</Description>\r\n"                                      \
	"	<DisplayName>Device Version</DisplayName>\r\n"                                              \
	"	<Visibility>Beginner</Visibility>\r\n"                                                      \
	"	<ImposedAccessMode>RO</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GEV_BOOTSTRAP_DeviceVersion</pValue>\r\n"                                           \
	"</String>\r\n"                                                                                 \
	"<String Name=\"DeviceManufacturerInfo\" NameSpace=\"Standard\">\r\n"                           \
	"	<ToolTip>Manufacturer information about the device.</ToolTip>\r\n"                          \
	"	<Description>Manufacturer information about the device.</Description>\r\n"                  \
	"	<DisplayName>Device Manufacturer Info</DisplayName>\r\n"                                    \
	"	<Visibility>Beginner</Visibility>\r\n"                                                      \
	"	<ImposedAccessMode>RO</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GEV_BOOTSTRAP_DeviceManufacturerInfo</pValue>\r\n"                                  \
	"</String>\r\n"                                                                                 \
	"<String Name=\"DeviceSerialNumber\" NameSpace=\"Standard\">\r\n"                               \
	"	<ToolTip>Device`s serial number.</ToolTip>\r\n"                                            \
	"	<Description>Device`s serial number. This string is a unique identifier of the device.</Description>\r\n"   \
	"	<DisplayName>Device Serial Number</DisplayName>\r\n"                                        \
	"	<Visibility>Expert</Visibility>\r\n"                                                        \
	"	<ImposedAccessMode>RO</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GEV_BOOTSTRAP_DeviceSerialNumber</pValue>\r\n"                                      \
	"</String>\r\n"                                                                                 \
	"<String Name=\"DeviceUserID\" NameSpace=\"Standard\">\r\n"                                     \
	"	<ToolTip>Device`s  user ID.</ToolTip>\r\n"                                                  \
	"	<Description>Device`s  user ID. This string is a unique identifier of the device.</Description>\r\n"   \
	"	<DisplayName>Device User ID</DisplayName>\r\n"                                              \
	"	<Visibility>Expert</Visibility>\r\n"                                                        \
	"	<ImposedAccessMode>RW</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GEV_BOOTSTRAP_DeviceUserID</pValue>\r\n"                                            \
	"</String>\r\n"                                                                                 \
	"<Command Name=\"DeviceReset\" NameSpace=\"Standard\">\r\n"                                     \
	"	<ToolTip>Resets the device to its power up state.</ToolTip>\r\n"                            \
	"	<Description>Resets the device to its power up state. After reset, the device must be rediscovered.</Description>\r\n"   \
	"	<DisplayName>Device Reset</DisplayName>\r\n"                                                \
	"	<Visibility>Guru</Visibility>\r\n"                                                          \
	"	<ImposedAccessMode>WO</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GEV_BOOTSTRAP_DeviceReset</pValue>\r\n"                                             \
	"	<CommandValue>0x1</CommandValue>\r\n"                                                       \
	"</Command>\r\n"                                                                                \
	"</Group>\r\n"                                                                                  \
	" <Group Comment=\"ImageFormatControl\">\r\n"                                                   \
	"<Integer Name=\"Width\" NameSpace=\"Standard\">\r\n"                                           \
	"	<ToolTip>Width of the image provided by the device (in pixels).</ToolTip>\r\n"              \
	"	<Description>Width of the image provided by the device (in pixels).</Description>\r\n"      \
	"	<DisplayName>Width</DisplayName>\r\n"                                                       \
	"	<Visibility>Beginner</Visibility>\r\n"                                                      \
	"	<pIsLocked>TLParamsLocked</pIsLocked>\r\n"                                                  \
	"	<ImposedAccessMode>RW</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GEV_BOOTSTRAP_Width</pValue>\r\n"                                                   \
	"	<Min>4</Min>\r\n"                                                                           \
	"	<Max>8191</Max>\r\n"                                                                        \
	"</Integer>\r\n"                                                                                \
	"<Integer Name=\"Height\" NameSpace=\"Standard\">\r\n"                                          \
	"	<ToolTip>Height of the image provided by the device (in pixels).</ToolTip>\r\n"             \
	"	<Description>Height of the image provided by the device (in pixels).</Description>\r\n"     \
	"	<DisplayName>Height</DisplayName>\r\n"                                                      \
	"	<Visibility>Beginner</Visibility>\r\n"                                                      \
	"	<pIsLocked>TLParamsLocked</pIsLocked>\r\n"                                                  \
	"	<ImposedAccessMode>RW</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GEV_BOOTSTRAP_Height</pValue>\r\n"                                                  \
	"	<Min>1</Min>\r\n"                                                                           \
	"	<Max>8191</Max>\r\n"                                                                        \
	"</Integer>\r\n"                                                                                \
	"<Integer Name=\"OffsetX\" NameSpace=\"Standard\">\r\n"                                         \
	"	<ToolTip>Horizontal offset from the origin to the region of interest (in pixels).</ToolTip>\r\n"           \
	"	<Description>Horizontal offset from the origin to the region of interest (in pixels).</Description>\r\n"   \
	"	<DisplayName>Offset X</DisplayName>\r\n"                                                    \
	"	<Visibility>Beginner</Visibility>\r\n"                                                      \
	"	<pIsLocked>TLParamsLocked</pIsLocked>\r\n"                                                  \
	"	<ImposedAccessMode>RW</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GEV_BOOTSTRAP_OffsetX</pValue>\r\n"                                                 \
	"</Integer>\r\n"                                                                                \
	"<Integer Name=\"OffsetY\" NameSpace=\"Standard\">\r\n"                                         \
	"	<ToolTip>Vertical offset from the origin to the region of interest (in pixels).</ToolTip>\r\n"             \
	"	<Description>Vertical offset from the origin to the region of interest (in pixels).</Description>\r\n"     \
	"	<DisplayName>Offset Y</DisplayName>\r\n"                                                    \
	"	<Visibility>Beginner</Visibility>\r\n"                                                      \
	"	<pIsLocked>TLParamsLocked</pIsLocked>\r\n"                                                  \
	"	<ImposedAccessMode>RW</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GEV_BOOTSTRAP_OffsetY</pValue>\r\n"                                                 \
	"</Integer>\r\n"                                                                                \
	"<Enumeration Name=\"PixelFormat\" NameSpace=\"Standard\">\r\n"                                 \
	"	<ToolTip>Format of the pixels provided by the device.</ToolTip>\r\n"                        \
	"	<Description>Format of the pixels provided by the device. It represents all the information provided by PixelCoding, PixelSize, PixelColorFilter combined in a single feature.</Description>\r\n"                                                           \
	"	<DisplayName>Pixel Format</DisplayName>\r\n"                                                \
	"	<Visibility>Beginner</Visibility>\r\n"                                                      \
	"	<pIsLocked>TLParamsLocked</pIsLocked>\r\n"                                                  \
	"	<ImposedAccessMode>RW</ImposedAccessMode>\r\n"                                              \
	"	<EnumEntry Name=\"RGB8\" NameSpace=\"Standard\">\r\n"                                       \
	"		<ToolTip>Red, Green, Blue 8 bit.</ToolTip>\r\n"                                         \
	"		<Description>Red, Green, Blue 8 bit</Description>\r\n"                                  \
	"		<DisplayName>RGB8</DisplayName>\r\n"                                                    \
	"		<Value>0x02180014</Value>\r\n"                                                          \
	"	</EnumEntry>\r\n"                                                                           \
	"	<EnumEntry Name=\"BGR8\" NameSpace=\"Standard\">\r\n"                                       \
	"		<ToolTip>Blue, Green, Red 8 bit.</ToolTip>\r\n"                                         \
	"		<Description>Blue, Green, Red 8 bit</Description>\r\n"                                  \
	"		<DisplayName>BGR8</DisplayName>\r\n"                                                    \
	"		<Value>0x02180015</Value>\r\n"                                                          \
	"	</EnumEntry>\r\n"                                                                           \
	"	<pValue>GEV_BOOTSTRAP_PixelFormat</pValue>\r\n"                                             \
	"</Enumeration>\r\n"                                                                            \
	"<Integer Name=\"SensorWidth\" NameSpace=\"Standard\">\r\n"                                     \
	"	<ToolTip>Effective width of the sensor in pixels.</ToolTip>\r\n"                            \
	"	<Description>Effective width of the sensor in pixels.</Description>\r\n"                    \
	"	<DisplayName>Sensor Width</DisplayName>\r\n"                                                \
	"	<Visibility>Expert</Visibility>\r\n"                                                        \
	"	<pIsLocked>TLParamsLocked</pIsLocked>\r\n"                                                  \
	"	<ImposedAccessMode>RW</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GEV_BOOTSTRAP_SensorWidth</pValue>\r\n"                                             \
	"</Integer>\r\n"                                                                                \
	"<Integer Name=\"SensorHeight\" NameSpace=\"Standard\">\r\n"                                    \
	"	<ToolTip>Effective height of the sensor in pixels.</ToolTip>\r\n"                           \
	"	<Description>Effective height of the sensor in pixels.</Description>\r\n"                   \
	"	<DisplayName>Sensor Height</DisplayName>\r\n"                                               \
	"	<Visibility>Expert</Visibility>\r\n"                                                        \
	"	<pIsLocked>TLParamsLocked</pIsLocked>\r\n"                                                  \
	"	<ImposedAccessMode>RW</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GEV_BOOTSTRAP_SensorHeight</pValue>\r\n"                                            \
	"</Integer>\r\n"                                                                                \
	"</Group>\r\n"                                                                                  \

	"<Group Comment=\"AcquisitionControl\">\r\n"                                                    \
	"<Enumeration Name=\"AcquisitionMode\" NameSpace=\"Standard\">\r\n"                             \
	"	<ToolTip>Sets the acquisition mode of the device.</ToolTip>\r\n"                            \
	"	<Description>Sets the acquisition mode of the device. It defines mainly the number of frames to capture during an acquisition and the way the acquisition stops.</Description>\r\n"                                                                         \
	"	<DisplayName>Acquisition Mode</DisplayName>\r\n"                                            \
	"	<Visibility>Beginner</Visibility>\r\n"                                                      \
	"	<pIsLocked>TLParamsLocked</pIsLocked>\r\n"                                                  \
	"	<ImposedAccessMode>RW</ImposedAccessMode>\r\n"                                              \
	"	<EnumEntry Name=\"SingleFrame\" NameSpace=\"Standard\">\r\n"                                \
	"		<ToolTip>One frame is captured.</ToolTip>\r\n"                                          \
	"		<Description>One frame is captured.</Description>\r\n"                                  \
	"		<DisplayName>Single Frame</DisplayName>\r\n"                                            \
	"		<Value>1</Value>\r\n"                                                                   \
	"	</EnumEntry>\r\n"                                                                           \
	"	<EnumEntry Name=\"MultiFrame\" NameSpace=\"Standard\">\r\n"                                 \
	"		<ToolTip>The number of frames specified by AcquisitionFrameCount is captured.</ToolTip>\r\n"          \
	"		<Description>The number of frames specified by AcquisitionFrameCount is captured.</Description>\r\n"  \
	"		<DisplayName>Multi Frame</DisplayName>\r\n"                                             \
	"		<Value>3</Value>\r\n"                                                                   \
	"	</EnumEntry>\r\n"                                                                           \
	"	<EnumEntry Name=\"Continuous\" NameSpace=\"Standard\">\r\n"                                 \
	"		<ToolTip>Frames are captured continuously until stopped with the AcquisitionStop command.</ToolTip>\r\n"            \
	"		<Description>Frames are captured continuously until stopped with the AcquisitionStop command.</Description>\r\n"    \
	"		<DisplayName>Continuous</DisplayName>\r\n"                                              \
	"		<Value>0</Value>\r\n"                                                                   \
	"	</EnumEntry>\r\n"                                                                           \
	"	<pValue>GEV_BOOTSTRAP_AcquisitionMode</pValue>\r\n"                                         \
	"</Enumeration>\r\n"                                                                            \
	"<Command Name=\"AcquisitionStart\" NameSpace=\"Standard\">\r\n"                                \
	"	<ToolTip>Starts the Acquisition of the device.</ToolTip>\r\n"                               \
	"	<Description>Starts the Acquisition of the device. The number of frames captured is specified by AcquisitionMode.</Description>\r\n"    \
	"	<DisplayName>Acquisition Start</DisplayName>\r\n"                                           \
	"	<Visibility>Beginner</Visibility>\r\n"                                                      \
	"	<ImposedAccessMode>WO</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GEV_BOOTSTRAP_AcquisitionStart</pValue>\r\n"                                        \
	"	<CommandValue>1</CommandValue>\r\n"                                                         \
	"</Command>\r\n"                                                                                \
	"<Command Name=\"AcquisitionStop\" NameSpace=\"Standard\">\r\n"                                 \
	"	<ToolTip>Stops the Acquisition of the device at the end of the current Frame.</ToolTip>\r\n"    \
	"	<Description>Stops the Acquisition of the device at the end of the current Frame. It is mainly used when AcquisitionMode is Continuous but can be used in any acquisition mode.</Description>\r\n"                                                          \
	"	<DisplayName>Acquisition Stop</DisplayName>\r\n"                                            \
	"	<Visibility>Beginner</Visibility>\r\n"                                                      \
	"	<ImposedAccessMode>WO</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GEV_BOOTSTRAP_AcquisitionStop</pValue>\r\n"                                         \
	"	<CommandValue>1</CommandValue>\r\n"                                                         \
	"</Command>\r\n"                                                                                \
	"<Integer Name=\"AcquisitionFrameCount\" NameSpace=\"Standard\">\r\n"                           \
	"	<ToolTip>Number of frames to acquire in MultiFrame Acquisition mode.</ToolTip>\r\n"         \
	"	<Description>Number of frames to acquire in MultiFrame Acquisition mode.</Description>\r\n" \
	"	<DisplayName>Acquisition Frame Count</DisplayName>\r\n"                                     \
	"	<Visibility>Beginner</Visibility>\r\n"                                                      \
	"	<ImposedAccessMode>RW</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GEV_BOOTSTRAP_AcquisitionFrameCount</pValue>\r\n"                                   \
	"</Integer>\r\n"                                                                                \
	"</Group>\r\n"                                                                                  \

	"<Group Comment=\"TransportLayerControl\">\r\n"                                                 \
	"<Integer Name=\"PayloadSize\" NameSpace=\"Standard\">\r\n"                                     \
	"	<ToolTip>Provides the number of bytes transferred for each image or chunk on the stream channel.</ToolTip>\r\n"    \
	"	<Description>Provides the number of bytes transferred for each image or chunk on the stream channel. This includes any end-of-line, end-of-frame statistics or other stamp data. This is the total size of data payload for a data block.</Description>\r\n" \
	"	<DisplayName>Payload Size</DisplayName>\r\n"                                                \
	"	<Visibility>Expert</Visibility>\r\n"                                                        \
	"	<ImposedAccessMode>RO</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GEV_BOOTSTRAP_PayloadSize</pValue>\r\n"                                             \
	"	<Unit>B</Unit>\r\n"                                                                         \
	"</Integer>\r\n"                                                                                \
	"</Group>\r\n"                                                                                  \

	"<Group Comment=\"GigEVision\">\r\n"                                                            \
	"<Integer Name=\"GevVersionMajor\" NameSpace=\"Standard\">\r\n"                                 \
	"	<ToolTip>Major version of the specification.</ToolTip>\r\n"                                 \
	"	<Description>Major version of the specification.</Description>\r\n"                         \
	"	<DisplayName>Gev Version Major</DisplayName>\r\n"                                           \
	"	<Visibility>Expert</Visibility>\r\n"                                                        \
	"	<ImposedAccessMode>RO</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GEV_BOOTSTRAP_Version_MAJORVERSION</pValue>\r\n"                                    \
	"</Integer>\r\n"                                                                                \
	"<Integer Name=\"GevVersionMinor\" NameSpace=\"Standard\">\r\n"                                 \
	"	<ToolTip>Minor version of the specification.</ToolTip>\r\n"                                 \
	"	<Description>Minor version of the specification.</Description>\r\n"                         \
	"	<DisplayName>Gev Version Minor</DisplayName>\r\n"                                           \
	"	<Visibility>Expert</Visibility>\r\n"                                                        \
	"	<ImposedAccessMode>RO</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GEV_BOOTSTRAP_Version_MINORVERSION</pValue>\r\n"                                    \
	"</Integer>\r\n"                                                                                \
	"<Boolean Name=\"GevDeviceModeIsBigEndian\" NameSpace=\"Standard\">\r\n"                        \
	"	<ToolTip>Endianess of the device registers.</ToolTip>\r\n"                                  \
	"	<Description>Endianess of the device registers.</Description>\r\n"                          \
	"	<DisplayName>Gev Device Mode Is Big Endian</DisplayName>\r\n"                               \
	"	<Visibility>Guru</Visibility>\r\n"                                                          \
	"	<ImposedAccessMode>RO</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GEV_BOOTSTRAP_DeviceMode_ENDIANESS</pValue>\r\n"                                    \
	"	<OnValue>1</OnValue>\r\n"                                                                   \
	"	<OffValue>0</OffValue>\r\n"                                                                 \
	"</Boolean>\r\n"                                                                                \
	"<Enumeration Name=\"GevDeviceClass\" NameSpace=\"Standard\">\r\n"                              \
	"	<ToolTip>Returns the class of the device.</ToolTip>\r\n"                                    \
	"	<Description>Returns the class of the device.</Description>\r\n"                            \
	"	<DisplayName>Gev Device Class</DisplayName>\r\n"                                            \
	"	<Visibility>Guru</Visibility>\r\n"                                                          \
	"	<ImposedAccessMode>RO</ImposedAccessMode>\r\n"                                              \
	"	<EnumEntry Name=\"Transmitter\" NameSpace=\"Standard\">\r\n"                                \
	"		<ToolTip>Transmitter</ToolTip>\r\n"                                                     \
	"		<Description>Transmitter</Description>\r\n"                                             \
	"		<DisplayName>Transmitter</DisplayName>\r\n"                                             \
	"		<Value>0</Value>\r\n"                                                                   \
	"	</EnumEntry>\r\n"                                                                           \
	"	<EnumEntry Name=\"Receiver\" NameSpace=\"Standard\">\r\n"                                   \
	"		<ToolTip>Receiver</ToolTip>\r\n"                                                        \
	"		<Description>Receiver</Description>\r\n"                                                \
	"		<DisplayName>Receiver</DisplayName>\r\n"                                                \
	"		<Value>1</Value>\r\n"                                                                   \
	"	</EnumEntry>\r\n"                                                                           \
	"	<EnumEntry Name=\"Transceiver\" NameSpace=\"Standard\">\r\n"                                \
	"		<ToolTip>Transceiver</ToolTip>\r\n"                                                     \
	"		<Description>Transceiver</Description>\r\n"                                             \
	"		<DisplayName>Transceiver</DisplayName>\r\n"                                             \
	"		<Value>2</Value>\r\n"                                                                   \
	"	</EnumEntry>\r\n"                                                                           \
	"	<EnumEntry Name=\"Peripheral\" NameSpace=\"Standard\">\r\n"                                 \
	"		<ToolTip>Peripheral</ToolTip>\r\n"                                                      \
	"		<Description>Peripheral</Description>\r\n"                                              \
	"		<DisplayName>Peripheral</DisplayName>\r\n"                                              \
	"		<Value>3</Value>\r\n"                                                                   \
	"	</EnumEntry>\r\n"                                                                           \
	"	<pValue>GEV_BOOTSTRAP_DeviceMode_DEVICECLASS</pValue>\r\n"                                  \
	"</Enumeration>\r\n"                                                                            \
	"<Enumeration Name=\"GevDeviceModeCharacterSet\" NameSpace=\"Standard\">\r\n"                   \
	"	<ToolTip>Character set used by all the strings of the bootstrap registers.</ToolTip>\r\n"   \
	"	<Description>Character set used by all the strings of the bootstrap registers.</Description>\r\n"   \
	"	<DisplayName>Gev Device Mode Character Set</DisplayName>\r\n"                               \
	"	<Visibility>Guru</Visibility>\r\n"                                                          \
	"	<ImposedAccessMode>RO</ImposedAccessMode>\r\n"                                              \
	"	<EnumEntry Name=\"UTF8\" NameSpace=\"Standard\">\r\n"                                       \
	"		<ToolTip>UTF 8</ToolTip>\r\n"                                                           \
	"		<Description>UTF 8</Description>\r\n"                                                   \
	"		<DisplayName>UTF 8</DisplayName>\r\n"                                                   \
	"		<Value>1</Value>\r\n"                                                                   \
	"	</EnumEntry>\r\n"                                                                           \
	"	<EnumEntry Name=\"ASCII\" NameSpace=\"Standard\">\r\n"                                      \
	"		<ToolTip>ASCII</ToolTip>\r\n"                                                           \
	"		<Description>ASCII</Description>\r\n"                                                   \
	"		<DisplayName>ASCII</DisplayName>\r\n"                                                   \
	"		<Value>2</Value>\r\n"                                                                   \
	"	</EnumEntry>\r\n"                                                                           \
	"	<pValue>GEV_BOOTSTRAP_DeviceMode_CHARACTERSET</pValue>\r\n"                                 \
	"</Enumeration>\r\n"                                                                            \

	"<Integer Name=\"GevInterfaceSelector\" NameSpace=\"Standard\">\r\n"                            \
	"	<ToolTip>Selects which logical link to control.</ToolTip>\r\n"                              \
	"	<Description>Selects which logical link to control.</Description>\r\n"                      \
	"	<DisplayName>Gev Interface Selector</DisplayName>\r\n"                                      \
	"	<Visibility>Beginner</Visibility>\r\n"                                                      \
	"	<ImposedAccessMode>RW</ImposedAccessMode>\r\n"                                              \
	"	<Value>0</Value>\r\n"                                                                       \
	"	<Min>0</Min>\r\n"                                                                           \
	"	<Max>0</Max>\r\n"                                                                           \
	"	<pSelected>GevMACAddress</pSelected>\r\n"                                                   \
	"	<pSelected>GevPAUSEFrameReceptionCap</pSelected>\r\n"                                       \
	"	<pSelected>GevPAUSEFrameTransmissionCap</pSelected>\r\n"                                    \
	"	<pSelected>GevCurrentIPConfigurationLLACap</pSelected>\r\n"                                 \
	"	<pSelected>GevCurrentIPConfigurationDHCPCap</pSelected>\r\n"                                \
	"	<pSelected>GevCurrentIPConfigurationPersistentIPCap</pSelected>\r\n"                        \
	"	<pSelected>GevPAUSEFrameReception</pSelected>\r\n"                                          \
	"	<pSelected>GevPAUSEFrameTransmission</pSelected>\r\n"                                       \
	"	<pSelected>GevCurrentIPConfigurationLLA</pSelected>\r\n"                                    \
	"	<pSelected>GevCurrentIPConfigurationDHCP</pSelected>\r\n"                                   \
	"	<pSelected>GevCurrentIPConfigurationPersistentIP</pSelected>\r\n"                           \
	"	<pSelected>GevCurrentIPAddress</pSelected>\r\n"                                             \
	"	<pSelected>GevCurrentSubnetMask</pSelected>\r\n"                                            \
	"	<pSelected>GevCurrentDefaultGateway</pSelected>\r\n"                                        \
	"	<pSelected>GevPersistentIPAddress</pSelected>\r\n"                                          \
	"	<pSelected>GevPersistentSubnetMask</pSelected>\r\n"                                         \
	"	<pSelected>GevPersistentDefaultGateway</pSelected>\r\n"                                     \
	"</Integer>\r\n"                                                                                \
	"<Integer Name=\"GevMACAddress\" NameSpace=\"Standard\">\r\n"                                   \
	"	<ToolTip>MAC address of the logical link.</ToolTip>\r\n"                                    \
	"	<Description>MAC address of the logical link.</Description>\r\n"                            \
	"	<DisplayName>Gev MAC Address</DisplayName>\r\n"                                             \
	"	<Visibility>Beginner</Visibility>\r\n"                                                      \
	"	<ImposedAccessMode>RO</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GevMACAddress_SwissKnife</pValue>\r\n"                                              \
	"</Integer>\r\n"                                                                                \
	"<Boolean Name=\"GevPAUSEFrameReceptionCap\" NameSpace=\"Standard\">\r\n"                       \
	"	<ToolTip>Status whether incoming PAUSE Frames are handled on the given logical link.</ToolTip>\r\n"          \
	"	<Description>Status whether incoming PAUSE Frames are handled on the given logical link.</Description>\r\n"  \
	"	<DisplayName>Gev PAUSE Frame Reception Capability</DisplayName>\r\n"                        \
	"	<Visibility>Expert</Visibility>\r\n"                                                        \
	"	<ImposedAccessMode>RO</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GEV_BOOTSTRAP_PAUSEFrameReceptionCap</pValue>\r\n"                                  \
	"	<OnValue>1</OnValue>\r\n"                                                                   \
	"	<OffValue>0</OffValue>\r\n"                                                                 \
	"</Boolean>\r\n"                                                                                \
	"<Boolean Name=\"GevPAUSEFrameTransmissionCap\" NameSpace=\"Standard\">\r\n"                    \
	"	<ToolTip>Status whether PAUSE Frames can be generated on the given logical link.</ToolTip>\r\n"              \
	"	<Description>Status whether PAUSE Frames can be generated on the given logical link.</Description>\r\n"      \
	"	<DisplayName>Gev PAUSE Frame Transmission Capability</DisplayName>\r\n"                     \
	"	<Visibility>Expert</Visibility>\r\n"                                                        \
	"	<ImposedAccessMode>RO</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GEV_BOOTSTRAP_PAUSEFrameTransmissionCap</pValue>\r\n"                               \
	"	<OnValue>1</OnValue>\r\n"                                                                   \
	"	<OffValue>0</OffValue>\r\n"                                                                 \
	"</Boolean>\r\n"                                                                                \
	"<Boolean Name=\"GevCurrentIPConfigurationLLACap\" NameSpace=\"Standard\">\r\n"                 \
	"	<ToolTip>Status whether the Link Local Address IP configuration scheme is activated on the given logical link.</ToolTip>\r\n"           \
	"	<Description>Status whether the Link Local Address IP configuration scheme is activated on the given logical link.</Description>\r\n"   \
	"	<DisplayName>Gev Current IP Configuration LLA Capability</DisplayName>\r\n"                 \
	"	<Visibility>Beginner</Visibility>\r\n"                                                      \
	"	<ImposedAccessMode>RO</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GEV_BOOTSTRAP_CurrentIPConfigurationLLACap</pValue>\r\n"                            \
	"	<OnValue>1</OnValue>\r\n"                                                                   \
	"	<OffValue>0</OffValue>\r\n"                                                                 \
	"</Boolean>\r\n"                                                                                \
	"<Boolean Name=\"GevCurrentIPConfigurationDHCPCap\" NameSpace=\"Standard\">\r\n"                \
	"	<ToolTip>Status whether the DHCP IP configuration scheme is activated on the given logical link.</ToolTip>\r\n"            \
	"	<Description>Status whether the DHCP IP configuration scheme is activated on the given logical link.</Description>\r\n"    \
	"	<DisplayName>Gev Current IP Configuration DHCP Capability</DisplayName>\r\n"                \
	"	<Visibility>Beginner</Visibility>\r\n"                                                      \
	"	<ImposedAccessMode>RO</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GEV_BOOTSTRAP_CurrentIPConfigurationDHCPCap</pValue>\r\n"                           \
	"	<OnValue>1</OnValue>\r\n"                                                                   \
	"	<OffValue>0</OffValue>\r\n"                                                                 \
	"</Boolean>\r\n"                                                                                \
	"<Boolean Name=\"GevCurrentIPConfigurationPersistentIPCap\" NameSpace=\"Standard\">\r\n"        \
	"	<ToolTip>Status whether the PersistentIP configuration scheme is activated on the given logical link.</ToolTip>\r\n"          \
	"	<Description>Status whether the PersistentIP configuration scheme is activated on the given logical link.</Description>\r\n"  \
	"	<DisplayName>Gev Current IP Configuration Persistent IP Capability</DisplayName>\r\n"       \
	"	<Visibility>Beginner</Visibility>\r\n"                                                      \
	"	<ImposedAccessMode>RO</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GEV_BOOTSTRAP_CurrentIPConfigurationPersistentIPCap</pValue>\r\n"                   \
	"	<OnValue>1</OnValue>\r\n"                                                                   \
	"	<OffValue>0</OffValue>\r\n"                                                                 \
	"</Boolean>\r\n"                                                                                \
	"<Boolean Name=\"GevPAUSEFrameReception\" NameSpace=\"Standard\">\r\n"                          \
	"	<ToolTip>Controls whether incoming PAUSE Frames are handled on the given logical link.</ToolTip>\r\n"            \
	"	<Description>Controls whether incoming PAUSE Frames are handled on the given logical link.</Description>\r\n"    \
	"	<DisplayName>Gev PAUSE Frame Reception</DisplayName>\r\n"                                   \
	"	<Visibility>Expert</Visibility>\r\n"                                                        \
	"	<ImposedAccessMode>RO</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GEV_BOOTSTRAP_PAUSEFrameReception</pValue>\r\n"                                     \
	"	<OnValue>1</OnValue>\r\n"                                                                   \
	"	<OffValue>0</OffValue>\r\n"                                                                 \
	"</Boolean>\r\n"                                                                                \
	"<Boolean Name=\"GevPAUSEFrameTransmission\" NameSpace=\"Standard\">\r\n"                       \
	"	<ToolTip>Controls whether PAUSE Frames can be generated on the given logical link.</ToolTip>\r\n"                \
	"	<Description>Controls whether PAUSE Frames can be generated on the given logical link.</Description>\r\n"        \
	"	<DisplayName>Gev PAUSE Frame Transmission</DisplayName>\r\n"                                \
	"	<Visibility>Expert</Visibility>\r\n"                                                        \
	"	<ImposedAccessMode>RO</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GEV_BOOTSTRAP_PAUSEFrameTransmission</pValue>\r\n"                                  \
	"	<OnValue>1</OnValue>\r\n"                                                                   \
	"	<OffValue>0</OffValue>\r\n"                                                                 \
	"</Boolean>\r\n"                                                                                \
	"<Boolean Name=\"GevCurrentIPConfigurationLLA\" NameSpace=\"Standard\">\r\n"                    \
	"	<ToolTip>Controls whether the Link Local Address IP configuration scheme is activated on the given logical link.</ToolTip>\r\n"          \
	"	<Description>Controls whether the Link Local Address IP configuration scheme is activated on the given logical link.</Description>\r\n"  \
	"	<DisplayName>Gev Current IP Configuration LLA</DisplayName>\r\n"                            \
	"	<Visibility>Beginner</Visibility>\r\n"                                                      \
	"	<ImposedAccessMode>RW</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GEV_BOOTSTRAP_CurrentIPConfigurationLLA</pValue>\r\n"                               \
	"	<OnValue>1</OnValue>\r\n"                                                                   \
	"	<OffValue>0</OffValue>\r\n"                                                                 \
	"</Boolean>\r\n"                                                                                \
	"<Boolean Name=\"GevCurrentIPConfigurationDHCP\" NameSpace=\"Standard\">\r\n"                   \
	"	<ToolTip>Controls whether the DHCP IP configuration scheme is activated on the given logical link.</ToolTip>\r\n"            \
	"	<Description>Controls whether the DHCP IP configuration scheme is activated on the given logical link.</Description>\r\n"    \
	"	<DisplayName>Gev Current IP Configuration DHCP</DisplayName>\r\n"                           \
	"	<Visibility>Beginner</Visibility>\r\n"                                                      \
	"	<ImposedAccessMode>RW</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GEV_BOOTSTRAP_CurrentIPConfigurationDHCP</pValue>\r\n"                              \
	"	<OnValue>1</OnValue>\r\n"                                                                   \
	"	<OffValue>0</OffValue>\r\n"                                                                 \
	"</Boolean>\r\n"                                                                                \
	"<Boolean Name=\"GevCurrentIPConfigurationPersistentIP\" NameSpace=\"Standard\">\r\n"           \
	"	<ToolTip>Controls whether the PersistentIP configuration scheme is activated on the given logical link.</ToolTip>\r\n"         \
	"	<Description>Controls whether the PersistentIP configuration scheme is activated on the given logical link.</Description>\r\n" \
	"	<DisplayName>Gev Current IP Configuration Persistent IP</DisplayName>\r\n"                  \
	"	<Visibility>Beginner</Visibility>\r\n"                                                      \
	"	<ImposedAccessMode>RW</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GEV_BOOTSTRAP_CurrentIPConfigurationPersistentIP</pValue>\r\n"                      \
	"	<OnValue>1</OnValue>\r\n"                                                                   \
	"	<OffValue>0</OffValue>\r\n"                                                                 \
	"</Boolean>\r\n"                                                                                \
	"<Integer Name=\"GevCurrentIPAddress\" NameSpace=\"Standard\">\r\n"                             \
	"	<ToolTip>Reports the IP address for the given locical link.</ToolTip>\r\n"                  \
	"	<Description>Reports the IP address for the given locical link.</Description>\r\n"          \
	"	<DisplayName>Gev Current IP Address</DisplayName>\r\n"                                      \
	"	<Visibility>Beginner</Visibility>\r\n"                                                      \
	"	<ImposedAccessMode>RO</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GEV_BOOTSTRAP_CurrentIPAddress_0</pValue>\r\n"                                      \
	"</Integer>\r\n"                                                                                \
	"<Integer Name=\"GevCurrentSubnetMask\" NameSpace=\"Standard\">\r\n"                            \
	"	<ToolTip>Reports the subnet mask of the given logical link.</ToolTip>\r\n"                  \
	"	<Description>Reports the subnet mask of the given logical link.</Description>\r\n"          \
	"	<DisplayName>Gev Current Subnet Mask</DisplayName>\r\n"                                     \
	"	<Visibility>Beginner</Visibility>\r\n"                                                      \
	"	<ImposedAccessMode>RO</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GEV_BOOTSTRAP_CurrentSubnetMask_0</pValue>\r\n"                                     \
	"</Integer>\r\n"                                                                                \
	"<Integer Name=\"GevCurrentDefaultGateway\" NameSpace=\"Standard\">\r\n"                        \
	"	<ToolTip>Reports the default gateway IP address to be used on the given logical link.</ToolTip>\r\n"           \
	"	<Description>Reports the default gateway IP address to be used on the given logical link.</Description>\r\n"   \
	"	<DisplayName>Gev Current Default Gateway</DisplayName>\r\n"                                 \
	"	<Visibility>Beginner</Visibility>\r\n"                                                      \
	"	<ImposedAccessMode>RO</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GEV_BOOTSTRAP_CurrentGateway_0</pValue>\r\n"                                        \
	"</Integer>\r\n"                                                                                \
	"<String Name=\"GevFirstURL\" NameSpace=\"Standard\">\r\n"                                      \
	"	<ToolTip>Indicates the first URL to the GenICam XML device description file.</ToolTip>\r\n" \
	"	<Description>Indicates the first URL to the GenICam XML device description file. The First URL is used as the first choice by the application to retrieve the GenICam XML device description file.</Description>\r\n"                                       \
	"	<DisplayName>Gev First URL</DisplayName>\r\n"                                               \
	"	<Visibility>Guru</Visibility>\r\n"                                                          \
	"	<ImposedAccessMode>RO</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GEV_BOOTSTRAP_FirstURL</pValue>\r\n"                                                \
	"</String>\r\n"                                                                                 \
	"<String Name=\"GevSecondURL\" NameSpace=\"Standard\">\r\n"                                     \
	"	<ToolTip>Indicates the second URL to the GenICam XML device description file.</ToolTip>\r\n" \
	"	<Description>Indicates the second URL to the GenICam XML device description file. This URL is an alternative if the application was unsuccessful to retrieve the device description file using the first URL.</Description>\r\n"                            \
	"	<DisplayName>Gev Second URL</DisplayName>\r\n"                                              \
	"	<Visibility>Guru</Visibility>\r\n"                                                          \
	"	<ImposedAccessMode>RO</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GEV_BOOTSTRAP_SecondURL</pValue>\r\n"                                               \
	"</String>\r\n"                                                                                 \
	"<Integer Name=\"GevNumberOfInterfaces\" NameSpace=\"Standard\">\r\n"                           \
	"	<ToolTip>Indicates the number of logical links supported by this device.</ToolTip>\r\n"     \
	"	<Description>Indicates the number of logical links supported by this device.</Description>\r\n" \
	"	<DisplayName>Gev Number Of Interfaces</DisplayName>\r\n"                                    \
	"	<Visibility>Expert</Visibility>\r\n"                                                        \
	"	<ImposedAccessMode>RO</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GEV_BOOTSTRAP_NumberOfInterfaces</pValue>\r\n"                                      \
	"</Integer>\r\n"                                                                                \
	"<Integer Name=\"GevPersistentIPAddress\" NameSpace=\"Standard\">\r\n"                          \
	"	<ToolTip>Controls the Persistent IP address for this logical link.</ToolTip>\r\n"           \
	"	<Description>Controls the Persistent IP address for this logical link. It is only used when the device boots with the Persistent IP configuration scheme.</Description>\r\n"                                                                                \
	"	<DisplayName>Gev Persistent IP Address</DisplayName>\r\n"                                   \
	"	<Visibility>Beginner</Visibility>\r\n"                                                      \
	"	<ImposedAccessMode>RW</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GEV_BOOTSTRAP_PersistentIPAddress_0</pValue>\r\n"                                   \
	"</Integer>\r\n"                                                                                \
	"<Integer Name=\"GevPersistentSubnetMask\" NameSpace=\"Standard\">\r\n"                         \
	"	<ToolTip>Controls the Persistent subnet mask associated with the Persistent IP address on this logical link.</ToolTip>\r\n"      \
	"	<Description>Controls the Persistent subnet mask associated with the Persistent IP address on this logical link. It is only used when the device boots with the Persistent IP configuration scheme.</Description>\r\n"                                      \
	"	<DisplayName>Gev Persistent Subnet Mask</DisplayName>\r\n"                                  \
	"	<Visibility>Beginner</Visibility>\r\n"                                                      \
	"	<ImposedAccessMode>RW</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GEV_BOOTSTRAP_PersistentSubnetMask_0</pValue>\r\n"                                  \
	"</Integer>\r\n"                                                                                \
	"<Integer Name=\"GevPersistentDefaultGateway\" NameSpace=\"Standard\">\r\n"                     \
	"	<ToolTip>Controls the persistent default gateway for this logical link.</ToolTip>\r\n"      \
	"	<Description>Controls the persistent default gateway for this logical link. It is only used when the device boots with the Persistent IP configuration scheme.Visibility</Description>\r\n"                                                                 \
	"	<DisplayName>Gev Persistent Default Gateway</DisplayName>\r\n"                              \
	"	<Visibility>Beginner</Visibility>\r\n"                                                      \
	"	<ImposedAccessMode>RW</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GEV_BOOTSTRAP_PersistentDefaultGateway_0</pValue>\r\n"                              \
	"</Integer>\r\n"                                                                                \
	"<Integer Name=\"GevMessageChannelCount\" NameSpace=\"Standard\">\r\n"                          \
	"	<ToolTip>Indicates the number of message channels supported by this device.</ToolTip>\r\n"  \
	"	<Description>Indicates the number of message channels supported by this device.</Description>\r\n" \
	"	<DisplayName>Gev Message Channel Count</DisplayName>\r\n"                                   \
	"	<Visibility>Expert</Visibility>\r\n"                                                        \
	"	<ImposedAccessMode>RO</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GEV_BOOTSTRAP_MessageChannelCount</pValue>\r\n"                                     \
	"</Integer>\r\n"                                                                                \
	"<Integer Name=\"GevStreamChannelCount\" NameSpace=\"Standard\">\r\n"                           \
	"	<ToolTip>Indicates the number of stream channels supported by this device.</ToolTip>\r\n"   \
	"	<Description>Indicates the number of stream channels supported by this device.</Description>\r\n"  \
	"	<DisplayName>Gev Stream Channel Count</DisplayName>\r\n"                                    \
	"	<Visibility>Expert</Visibility>\r\n"                                                        \
	"	<ImposedAccessMode>RO</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GEV_BOOTSTRAP_StreamChannelCount</pValue>\r\n"                                      \
	"</Integer>\r\n"                                                                                \
	"<Integer Name=\"GevActiveLinkCount\" NameSpace=\"Standard\">\r\n"                              \
	"	<ToolTip>Indicates the current number of active logical links.</ToolTip>\r\n"               \
	"	<Description>Indicates the current number of active logical links.</Description>\r\n"       \
	"	<DisplayName>Gev Active Link Count</DisplayName>\r\n"                                       \
	"	<Visibility>Expert</Visibility>\r\n"                                                        \
	"	<ImposedAccessMode>RO</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GEV_BOOTSTRAP_GevActiveLinkCount</pValue>\r\n"                                      \
	"</Integer>\r\n"                                                                                \

	"<Enumeration Name=\"GevSupportedOptionSelector\" NameSpace=\"Standard\">\r\n"                  \
	"	<ToolTip>Selects the GEV option to interrogate for existing support.</ToolTip>\r\n"         \
	"	<Description>Selects the GEV option to interrogate for existing support.</Description>\r\n" \
	"	<DisplayName>Gev Supported Option Selector</DisplayName>\r\n"                               \
	"	<Visibility>Expert</Visibility>\r\n"                                                        \
	"	<ImposedAccessMode>RW</ImposedAccessMode>\r\n"                                              \
	"	<EnumEntry Name=\"UserDefinedName\" NameSpace=\"Standard\">\r\n"                            \
	"		<ToolTip>User Defined Name</ToolTip>\r\n"                                               \
	"		<Description>User Defined Name</Description>\r\n"                                       \
	"		<DisplayName>User Defined Name</DisplayName>\r\n"                                       \
	"		<Value>0</Value>\r\n"                                                                   \
	"	</EnumEntry>\r\n"                                                                           \
	"	<EnumEntry Name=\"SerialNumber\" NameSpace=\"Standard\">\r\n"                               \
	"		<ToolTip>Serial Number</ToolTip>\r\n"                                                   \
	"		<Description>Serial Number</Description>\r\n"                                           \
	"		<DisplayName>Serial Number</DisplayName>\r\n"                                           \
	"		<Value>1</Value>\r\n"                                                                   \
	"	</EnumEntry>\r\n"                                                                           \
	"	<EnumEntry Name=\"HeartbeatDisable\" NameSpace=\"Standard\">\r\n"                           \
	"		<ToolTip>Heartbeat Disable</ToolTip>\r\n"                                               \
	"		<Description>Heartbeat Disable</Description>\r\n"                                       \
	"		<DisplayName>Heartbeat Disable</DisplayName>\r\n"                                       \
	"		<Value>2</Value>\r\n"                                                                   \
	"	</EnumEntry>\r\n"                                                                           \
	"	<EnumEntry Name=\"LinkSpeed\" NameSpace=\"Standard\">\r\n"                                  \
	"		<ToolTip>Link Speed</ToolTip>\r\n"                                                      \
	"		<Description>Link Speed</Description>\r\n"                                              \
	"		<DisplayName>Link Speed</DisplayName>\r\n"                                              \
	"		<Value>3</Value>\r\n"                                                                   \
	"	</EnumEntry>\r\n"                                                                           \
	"	<EnumEntry Name=\"CCPApplicationSocket\" NameSpace=\"Standard\">\r\n"                       \
	"		<ToolTip>CCP Application Socket</ToolTip>\r\n"                                          \
	"		<Description>CCP Application Socket</Description>\r\n"                                  \
	"		<DisplayName>CCP Application Socket</DisplayName>\r\n"                                  \
	"		<Value>4</Value>\r\n"                                                                   \
	"	</EnumEntry>\r\n"                                                                           \
	"	<EnumEntry Name=\"ManifestTable\" NameSpace=\"Standard\">\r\n"                              \
	"		<ToolTip>Manifest Table</ToolTip>\r\n"                                                  \
	"		<Description>Manifest Table</Description>\r\n"                                          \
	"		<DisplayName>Manifest Table</DisplayName>\r\n"                                          \
	"		<Value>5</Value>\r\n"                                                                   \
	"	</EnumEntry>\r\n"                                                                           \
	"	<EnumEntry Name=\"TestData\" NameSpace=\"Standard\">\r\n"                                   \
	"		<ToolTip>Test Data</ToolTip>\r\n"                                                       \
	"		<Description>Test Data</Description>\r\n"                                               \
	"		<DisplayName>Test Data</DisplayName>\r\n"                                               \
	"		<Value>6</Value>\r\n"                                                                   \
	"	</EnumEntry>\r\n"                                                                           \
	"	<EnumEntry Name=\"DiscoveryAckDelayWritable\" NameSpace=\"Standard\">\r\n"                  \
	"		<ToolTip>Discovery Ack Delay Writable</ToolTip>\r\n"                                    \
	"		<Description>Discovery Ack Delay Writable</Description>\r\n"                            \
	"		<DisplayName>Discovery Ack Delay Writable</DisplayName>\r\n"                            \
	"		<Value>7</Value>\r\n"                                                                   \
	"	</EnumEntry>\r\n"                                                                           \
	"	<EnumEntry Name=\"DiscoveryAckDelay\" NameSpace=\"Standard\">\r\n"                          \
	"		<ToolTip>Discovery Ack Delay</ToolTip>\r\n"                                             \
	"		<Description>Discovery Ack Delay</Description>\r\n"                                     \
	"		<DisplayName>Discovery Ack Delay</DisplayName>\r\n"                                     \
	"		<Value>8</Value>\r\n"                                                                   \
	"	</EnumEntry>\r\n"                                                                           \
	"	<EnumEntry Name=\"ExtendedStatusCodes\" NameSpace=\"Standard\">\r\n"                        \
	"		<ToolTip>Extended Status Codes</ToolTip>\r\n"                                           \
	"		<Description>Extended Status Codes</Description>\r\n"                                   \
	"		<DisplayName>Extended Status Codes</DisplayName>\r\n"                                   \
	"		<Value>9</Value>\r\n"                                                                   \
	"	</EnumEntry>\r\n"                                                                           \
	"	<EnumEntry Name=\"PrimaryApplicationSwitchover\" NameSpace=\"Standard\">\r\n"               \
	"		<ToolTip>Primary Application Switchover</ToolTip>\r\n"                                  \
	"		<Description>Primary Application Switchover</Description>\r\n"                          \
	"		<DisplayName>Primary Application Switchover</DisplayName>\r\n"                          \
	"		<Value>10</Value>\r\n"                                                                  \
	"	</EnumEntry>\r\n"                                                                           \
	"	<EnumEntry Name=\"UnconditionalAction\" NameSpace=\"Standard\">\r\n"                        \
	"		<ToolTip>Unconditional Action</ToolTip>\r\n"                                            \
	"		<Description>Unconditional Action</Description>\r\n"                                    \
	"		<DisplayName>Unconditional Action</DisplayName>\r\n"                                    \
	"		<Value>11</Value>\r\n"                                                                  \
	"	</EnumEntry>\r\n"                                                                           \
	"	<EnumEntry Name=\"IEEE1588\" NameSpace=\"Standard\">\r\n"                                   \
	"		<ToolTip>IEEE 1588</ToolTip>\r\n"                                                       \
	"		<Description>IEEE 1588</Description>\r\n"                                               \
	"		<DisplayName>IEEE 1588</DisplayName>\r\n"                                               \
	"		<Value>12</Value>\r\n"                                                                  \
	"	</EnumEntry>\r\n"                                                                           \
	"	<EnumEntry Name=\"ExtendedStatusCodesVersion2_0\" NameSpace=\"Standard\">\r\n"              \
	"		<ToolTip>ExtendedStatusCodesVersion2_0</ToolTip>\r\n"                                   \
	"		<Description>ExtendedStatusCodesVersion2_0</Description>\r\n"                           \
	"		<DisplayName>ExtendedStatusCodesVersion2_0</DisplayName>\r\n"                           \
	"		<Value>13</Value>\r\n"                                                                  \
	"	</EnumEntry>\r\n"                                                                           \
	"	<EnumEntry Name=\"ScheduledAction\" NameSpace=\"Standard\">\r\n"                            \
	"		<ToolTip>Scheduled Action</ToolTip>\r\n"                                                \
	"		<Description>Scheduled Action</Description>\r\n"                                        \
	"		<DisplayName>Scheduled Action</DisplayName>\r\n"                                        \
	"		<Value>14</Value>\r\n"                                                                  \
	"	</EnumEntry>\r\n"                                                                           \
	"	<EnumEntry Name=\"Action\" NameSpace=\"Standard\">\r\n"                                     \
	"		<ToolTip>Action</ToolTip>\r\n"                                                          \
	"		<Description>Action</Description>\r\n"                                                  \
	"		<DisplayName>Action</DisplayName>\r\n"                                                  \
	"		<Value>25</Value>\r\n"                                                                  \
	"	</EnumEntry>\r\n"                                                                           \
	"	<EnumEntry Name=\"PendingAck\" NameSpace=\"Standard\">\r\n"                                 \
	"		<ToolTip>Pending Ack</ToolTip>\r\n"                                                     \
	"		<Description>Pending Ack</Description>\r\n"                                             \
	"		<DisplayName>Pending Ack</DisplayName>\r\n"                                             \
	"		<Value>26</Value>\r\n"                                                                  \
	"	</EnumEntry>\r\n"                                                                           \
	"	<EnumEntry Name=\"EventData\" NameSpace=\"Standard\">\r\n"                                  \
	"		<ToolTip>Event Data</ToolTip>\r\n"                                                      \
	"		<Description>Event Data</Description>\r\n"                                              \
	"		<DisplayName>Event Data</DisplayName>\r\n"                                              \
	"		<Value>27</Value>\r\n"                                                                  \
	"	</EnumEntry>\r\n"                                                                           \
	"	<EnumEntry Name=\"Event\" NameSpace=\"Standard\">\r\n"                                      \
	"		<ToolTip>Event</ToolTip>\r\n"                                                           \
	"		<Description>Event</Description>\r\n"                                                   \
	"		<DisplayName>Event</DisplayName>\r\n"                                                   \
	"		<Value>28</Value>\r\n"                                                                  \
	"	</EnumEntry>\r\n"                                                                           \
	"	<EnumEntry Name=\"PacketResend\" NameSpace=\"Standard\">\r\n"                               \
	"		<ToolTip>Packet Resend</ToolTip>\r\n"                                                   \
	"		<Description>Packet Resend</Description>\r\n"                                           \
	"		<DisplayName>Packet Resend</DisplayName>\r\n"                                           \
	"		<Value>29</Value>\r\n"                                                                  \
	"	</EnumEntry>\r\n"                                                                           \
	"	<EnumEntry Name=\"WriteMem\" NameSpace=\"Standard\">\r\n"                                   \
	"		<ToolTip>Write Mem</ToolTip>\r\n"                                                       \
	"		<Description>Write Mem</Description>\r\n"                                               \
	"		<DisplayName>Write Mem</DisplayName>\r\n"                                               \
	"		<Value>30</Value>\r\n"                                                                  \
	"	</EnumEntry>\r\n"                                                                           \
	"	<EnumEntry Name=\"CommandsConcatenation\" NameSpace=\"Standard\">\r\n"                      \
	"		<ToolTip>Commands Concatenation</ToolTip>\r\n"                                          \
	"		<Description>Commands Concatenation</Description>\r\n"                                  \
	"		<DisplayName>Commands Concatenation</DisplayName>\r\n"                                  \
	"		<Value>31</Value>\r\n"                                                                  \
	"	</EnumEntry>\r\n"                                                                           \
	"	<EnumEntry Name=\"StreamChannel0BigAndLittleEndian\" NameSpace=\"Standard\">\r\n"           \
	"		<ToolTip>Stream Channel 0 Big And Little Endian</ToolTip>\r\n"                          \
	"		<Description>Stream Channel 0 Big And Little Endian</Description>\r\n"                  \
	"		<DisplayName>Stream Channel 0 Big And Little Endian</DisplayName>\r\n"                  \
	"		<Value>40</Value>\r\n"                                                                  \
	"	</EnumEntry>\r\n"                                                                           \
	"	<EnumEntry Name=\"StreamChannel0IPReassembly\" NameSpace=\"Standard\">\r\n"                 \
	"		<ToolTip>Stream Channel 0 IP Reassembly</ToolTip>\r\n"                                  \
	"		<Description>Stream Channel 0 IP Reassembly</Description>\r\n"                          \
	"		<DisplayName>Stream Channel 0 IP Reassembly</DisplayName>\r\n"                          \
	"		<Value>41</Value>\r\n"                                                                  \
	"	</EnumEntry>\r\n"                                                                           \
	"	<EnumEntry Name=\"StreamChannel0MultiZone\" NameSpace=\"Standard\">\r\n"                    \
	"		<ToolTip>Stream Channel 0 Multi Zone</ToolTip>\r\n"                                     \
	"		<Description>Stream Channel 0 Multi Zone</Description>\r\n"                             \
	"		<DisplayName>Stream Channel 0 Multi Zone</DisplayName>\r\n"                             \
	"		<Value>67</Value>\r\n"                                                                  \
	"	</EnumEntry>\r\n"                                                                           \

	"	<EnumEntry Name=\"StreamChannel0PacketResendDestination\" NameSpace=\"Standard\">\r\n"      \
	"		<ToolTip>Stream Channel 0 Packet Resend Destination</ToolTip>\r\n"                      \
	"		<Description>Stream Channel 0 Packet Resend Destination</Description>\r\n"              \
	"		<DisplayName>Stream Channel 0 Packet Resend Destination</DisplayName>\r\n"              \
	"		<Value>68</Value>\r\n"                                                                  \
	"	</EnumEntry>\r\n"                                                                           \
	"	<EnumEntry Name=\"StreamChannel0AllInTransmission\" NameSpace=\"Standard\">\r\n"            \
	"		<ToolTip>Stream Channel 0 All In Transmission</ToolTip>\r\n"                            \
	"		<Description>Stream Channel 0 All In Transmission</Description>\r\n"                    \
	"		<DisplayName>Stream Channel 0 All In Transmission</DisplayName>\r\n"                    \
	"		<Value>69</Value>\r\n"                                                                  \
	"	</EnumEntry>\r\n"                                                                           \
	"	<EnumEntry Name=\"StreamChannel0UnconditionalStreaming\" NameSpace=\"Standard\">\r\n"       \
	"		<ToolTip>Stream Channel 0 Unconditional Streaming</ToolTip>\r\n"                        \
	"		<Description>Stream Channel 0 Unconditional Streaming</Description>\r\n"                \
	"		<DisplayName>Stream Channel 0 Unconditional Streaminge</DisplayName>\r\n"               \
	"		<Value>70</Value>\r\n"                                                                  \
	"	</EnumEntry>\r\n"                                                                           \
	"	<EnumEntry Name=\"StreamChannel0ExtendedChunkData\" NameSpace=\"Standard\">\r\n"            \
	"		<ToolTip>Stream Channel 0 Extended Chunk Data</ToolTip>\r\n"                            \
	"		<Description>Stream Channel 0 Extended Chunk Data</Description>\r\n"                    \
	"		<DisplayName>Stream Channel 0 Extended Chunk Data</DisplayName>\r\n"                    \
	"		<Value>71</Value>\r\n"                                                                  \
	"	</EnumEntry>\r\n"                                                                           \
	"	<pValue>GevSupportedOptionSelectorValue</pValue>\r\n"                                       \
	"	<pSelected>GevSupportedOption</pSelected>\r\n"                                              \
	"</Enumeration>\r\n"                                                                            \
	"<Integer Name=\"GevSupportedOptionSelectorValue\">\r\n"                                        \
	"	<Value>0</Value>\r\n"                                                                       \
	"</Integer>\r\n"                                                                                \
	"<Boolean Name=\"GevSupportedOption\" NameSpace=\"Standard\">\r\n"                              \
	"	<ToolTip>Returns if the selected GEV option is supported.</ToolTip>\r\n"                    \
	"	<Description>Returns if the selected GEV option is supported.</Description>\r\n"            \
	"	<DisplayName>Gev Supported Option</DisplayName>\r\n"                                        \
	"	<Visibility>Expert</Visibility>\r\n"                                                        \
	"	<ImposedAccessMode>RO</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GevSupportedOptionValue_SwissKnife</pValue>\r\n"                                    \
	"</Boolean>\r\n"                                                                                \
	"<IntSwissKnife Name=\"GevSupportedOptionValue_SwissKnife\">\r\n"                               \
	"	<pVariable Name=\"SEL\">GevSupportedOptionSelectorValue</pVariable>\r\n"                    \
	"	<pVariable Name=\"UN\" >GEV_BOOTSTRAP_GVCPCapability_userdefinedname</pVariable>\r\n"       \
	"	<pVariable Name=\"SN\" >GEV_BOOTSTRAP_GVCPCapability_serialnumber</pVariable>\r\n"          \
	"	<pVariable Name=\"HD\" >GEV_BOOTSTRAP_GVCPCapability_heartbeatdisable</pVariable>\r\n"      \
	"	<pVariable Name=\"LS\" >GEV_BOOTSTRAP_GVCPCapability_linkspeed</pVariable>\r\n"             \
	"	<pVariable Name=\"CAP\">GEV_BOOTSTRAP_GVCPCapability_CCP_applicationportIP</pVariable>\r\n" \
	"	<pVariable Name=\"MT\" >GEV_BOOTSTRAP_GVCPCapability_manifesttable</pVariable>\r\n"         \
	"	<pVariable Name=\"TD\" >GEV_BOOTSTRAP_GVCPCapability_testdata</pVariable>\r\n"              \
	"	<pVariable Name=\"DD\" >GEV_BOOTSTRAP_GVCPCapability_discoveryACKdelay</pVariable>\r\n"     \
	"	<pVariable Name=\"WD\" >GEV_BOOTSTRAP_GVCPCapability_writablediscoveryACKdelay</pVariable>\r\n"     \
	"	<pVariable Name=\"ES\" >GEV_BOOTSTRAP_GVCPCapability_extendedstatuscodefor11</pVariable>\r\n"       \
	"	<pVariable Name=\"PAS\">GEV_BOOTSTRAP_GVCPCapability_primaryapplicationswitchover</pVariable>\r\n"  \
	"	<pVariable Name=\"UA\" >GEV_BOOTSTRAP_GVCPCapability_unconditionalACTION</pVariable>\r\n"   \
	"	<pVariable Name=\"PTP\">GEV_BOOTSTRAP_GVCPCapability_IEEE1588support</pVariable>\r\n"       \
	"	<pVariable Name=\"ES2\">GEV_BOOTSTRAP_GVCPCapability_extendedstatuscodefor20</pVariable>\r\n"       \
	"	<pVariable Name=\"SAC\">GEV_BOOTSTRAP_GVCPCapability_scheduledactioncommand</pVariable>\r\n"        \
	"	<pVariable Name=\"A\"  >GEV_BOOTSTRAP_GVCPCapability_ACTION</pVariable>\r\n"                \
	"	<pVariable Name=\"PA\" >GEV_BOOTSTRAP_GVCPCapability_PENDINGACK</pVariable>\r\n"            \
	"	<pVariable Name=\"ED\" >GEV_BOOTSTRAP_GVCPCapability_EVENTDATA</pVariable>\r\n"             \
	"	<pVariable Name=\"E\"  >GEV_BOOTSTRAP_GVCPCapability_EVENT</pVariable>\r\n"                 \
	"	<pVariable Name=\"PR\" >GEV_BOOTSTRAP_GVCPCapability_PACKETRESEND</pVariable>\r\n"          \
	"	<pVariable Name=\"W\"  >GEV_BOOTSTRAP_GVCPCapability_WRITEMEM</pVariable>\r\n"              \
	"	<pVariable Name=\"C\"  >GEV_BOOTSTRAP_GVCPCapability_concatenation</pVariable>\r\n"         \
	"	<pVariable Name=\"BE\" >GEV_BOOTSTRAP_StreamChannel0BigAndLittleEndian</pVariable>\r\n"     \
	"	<pVariable Name=\"R\"  >GEV_BOOTSTRAP_StreamChannel0IPReassembly</pVariable>\r\n"           \
	"	<pVariable Name=\"MZ\" >GEV_BOOTSTRAP_StreamChannel0MultiZonen</pVariable>\r\n"             \
	"	<pVariable Name=\"PRD\" >GEV_BOOTSTRAP_StreamChannel0PacketResendDestination</pVariable>\r\n"      \
	"	<pVariable Name=\"AIT\" >GEV_BOOTSTRAP_StreamChannel0AllInTransmission</pVariable>\r\n"     \
	"	<pVariable Name=\"US\"  >GEV_BOOTSTRAP_StreamChannel0UnconditionalStreaming</pVariable>\r\n"       \
	"	<pVariable Name=\"EC\"  >GEV_BOOTSTRAP_StreamChannel0ExtendedChunkData</pVariable>\r\n"     \
	"	<Formula>SEL =  0 ? UN  :\r\n"                                                              \
	"			(SEL =  1 ? SN  :\r\n"                                                              \
	"			(SEL =  2 ? HD  :\r\n"                                                              \
	"			(SEL =  3 ? LS  :\r\n"                                                              \
	"			(SEL =  4 ? CAP :\r\n"                                                              \
	"			(SEL =  5 ? MT  :\r\n"                                                              \
	"			(SEL =  6 ? TD  :\r\n"                                                              \
	"			(SEL =  7 ? DD  :\r\n"                                                              \
	"			(SEL =  8 ? WD  :\r\n"                                                              \
	"			(SEL =  9 ? ES  :\r\n"                                                              \
	"			(SEL = 10 ? PAS :\r\n"                                                              \
	"			(SEL = 11 ? UA  :\r\n"                                                              \
	"			(SEL = 12 ? PTP :\r\n"                                                              \
	"			(SEL = 13 ? ES2 :\r\n"                                                              \
	"			(SEL = 14 ? SAC :\r\n"                                                              \
	"			(SEL = 25 ? A   :\r\n"                                                              \
	"			(SEL = 26 ? PA  :\r\n"                                                              \
	"			(SEL = 27 ? ED  :\r\n"                                                              \
	"			(SEL = 28 ? E   :\r\n"                                                              \
	"			(SEL = 29 ? PR  :\r\n"                                                              \
	"			(SEL = 30 ? W   :\r\n"                                                              \
	"			(SEL = 31 ? C   :\r\n"                                                              \
	"			(SEL = 40 ? BE  :\r\n"                                                              \
	"			(SEL = 41 ? R   :\r\n"                                                              \
	"			(SEL = 67 ? MZ  :\r\n"                                                              \
	"			(SEL = 68 ? PRD :\r\n"                                                              \
	"			(SEL = 69 ? AIT :\r\n"                                                              \
	"			(SEL = 70 ? US  :\r\n"                                                              \
	"			(SEL = 71 ? EC  : 0 ))))))))))))))))))))))))))))</Formula>\r\n"                     \
	"</IntSwissKnife>\r\n"                                                                          \
	"<Integer Name=\"GevHeartbeatTimeout\" NameSpace=\"Standard\">\r\n"                             \
	"	<ToolTip>Controls the current heartbeat timeout in milliseconds.</ToolTip>\r\n"             \
	"	<Description>Controls the current heartbeat timeout in milliseconds.</Description>\r\n"     \
	"	<DisplayName>Gev Heartbeat Timeout</DisplayName>\r\n"                                       \
	"	<Visibility>Guru</Visibility>\r\n"                                                          \
	"	<ImposedAccessMode>RW</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GEV_BOOTSTRAP_HeartbeatTimeout</pValue>\r\n"                                        \
	"	<Min>3000</Min>\r\n"                                                                        \
	"	<Unit>ms</Unit>\r\n"                                                                        \
	"</Integer>\r\n"                                                                                \
	"<Integer Name=\"GevTimestampTickFrequency\" NameSpace=\"Standard\">\r\n"                       \
	"	<ToolTip>Indicates the number of timestamp ticks in 1 second (frequency in Hz).</ToolTip>\r\n"    \
	"	<Description>Indicates the number of timestamp ticks in 1 second (frequency in Hz). If IEEE 1588 is used, this feature must return 1,000,000,000 (1 GHz).</Description>\r\n"                                                                                \
	"	<DisplayName>Gev Timestamp Tick Frequency</DisplayName>\r\n"                                \
	"	<Visibility>Expert</Visibility>\r\n"                                                        \
	"	<ImposedAccessMode>RO</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GevTimestampTickFrequency_SwissKnife</pValue>\r\n"                                  \
	"	<Unit>Hz</Unit>\r\n"                                                                        \
	"</Integer>\r\n"                                                                                \
	"<Command Name=\"GevTimestampControlLatch\" NameSpace=\"Standard\">\r\n"                        \
	"	<ToolTip>Latches the current timestamp counter into GevTimestampValue.</ToolTip>\r\n"       \
	"	<Description>Latches the current timestamp counter into GevTimestampValue.</Description>\r\n"      \
	"	<DisplayName>Gev Timestamp Control Latch</DisplayName>\r\n"                                 \
	"	<Visibility>Expert</Visibility>\r\n"                                                        \
	"	<ImposedAccessMode>WO</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GEV_BOOTSTRAP_TimestampControl</pValue>\r\n"                                        \
	"	<CommandValue>2</CommandValue>\r\n"                                                         \
	"</Command>\r\n"                                                                                \
	"<Command Name=\"GevTimestampControlReset\" NameSpace=\"Standard\">\r\n"                        \
	"	<ToolTip>Resets the timestamp counter to 0.</ToolTip>\r\n"                                  \
	"	<Description>Resets the timestamp counter to 0. This feature is not available or as no effect when IEEE 1588 is used.</Description>\r\n"  \
	"	<DisplayName>Gev Timestamp Control Reset</DisplayName>\r\n"                                 \
	"	<Visibility>Expert</Visibility>\r\n"                                                        \
	"	<ImposedAccessMode>WO</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GEV_BOOTSTRAP_TimestampControl</pValue>\r\n"                                        \
	"	<CommandValue>1</CommandValue>\r\n"                                                         \
	"</Command>\r\n"                                                                                \
	"<Integer Name=\"GevTimestampValue\" NameSpace=\"Standard\">\r\n"                               \
	"	<ToolTip>Returns the latched 64-bit value of the timestamp counter.</ToolTip>\r\n"          \
	"	<Description>Returns the latched 64-bit value of the timestamp counter.</Description>\r\n"  \
	"	<DisplayName>Gev Timestamp Value</DisplayName>\r\n"                                         \
	"	<Visibility>Expert</Visibility>\r\n"                                                        \
	"	<ImposedAccessMode>RO</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GevTimestampValue_SwissKnife</pValue>\r\n"                                          \
	"</Integer>\r\n"                                                                                \

	"<Boolean Name=\"GevIEEE1588\" NameSpace=\"Standard\">\r\n"                                     \
	"	<ToolTip>Enables the IEEE 1588 Precision Time Protocol to control the timestamp register.</ToolTip>\r\n"          \
	"	<Description>Enables the IEEE 1588 Precision Time Protocol to control the timestamp register.</Description>\r\n"  \
	"	<DisplayName>Gev IEEE 1588</DisplayName>\r\n"                                               \
	"	<Visibility>Expert</Visibility>\r\n"                                                        \
	"	<ImposedAccessMode>RW</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GEV_BOOTSTRAP_IEEE1588</pValue>\r\n"                                                \
	"</Boolean>\r\n"                                                                                \
	"<Enumeration Name=\"GevGVCPExtendedStatusCodesSelector\" NameSpace=\"Standard\">\r\n"          \
	"	<ToolTip>Selects the GigE Vision version to control extended status codes for.</ToolTip>\r\n"           \
	"	<Description>Selects the GigE Vision version to control extended status codes for.</Description>\r\n"   \
	"	<DisplayName>Gev GVCP Extended Status Codes Selector</DisplayName>\r\n"                     \
	"	<Visibility>Guru</Visibility>\r\n"                                                          \
	"	<ImposedAccessMode>RW</ImposedAccessMode>\r\n"                                              \
	"	<EnumEntry Name=\"Version1_1\" NameSpace=\"Standard\">\r\n"                                 \
	"		<ToolTip>Version1_1</ToolTip>\r\n"                                                      \
	"		<Description>Version1_1</Description>\r\n"                                              \
	"		<DisplayName>Version1_1</DisplayName>\r\n"                                              \
	"		<Value>0</Value>\r\n"                                                                   \
	"	</EnumEntry>\r\n"                                                                           \
	"	<EnumEntry Name=\"Version2_0\" NameSpace=\"Standard\">\r\n"                                 \
	"		<ToolTip>Version2_0</ToolTip>\r\n"                                                      \
	"		<Description>Version2_0</Description>\r\n"                                              \
	"		<DisplayName>Version2_0</DisplayName>\r\n"                                              \
	"		<Value>1</Value>\r\n"                                                                   \
	"	</EnumEntry>\r\n"                                                                           \
	"	<pValue>GevGVCPExtendedStatusCodesSelectorValue</pValue>\r\n"                               \
	"	<pSelected>GevGVCPExtendedStatusCodes</pSelected>\r\n"                                      \
	"</Enumeration>\r\n"                                                                            \
	"<Integer Name=\"GevGVCPExtendedStatusCodesSelectorValue\">\r\n"                                \
	"	<Value>0</Value>\r\n"                                                                       \
	"</Integer>\r\n"                                                                                \
	"<Boolean Name=\"GevGVCPExtendedStatusCodes\" NameSpace=\"Standard\">\r\n"                      \
	"	<ToolTip>Enables the generation of extended status codes.</ToolTip>\r\n"                    \
	"	<Description>Enables the generation of extended status codes.</Description>\r\n"            \
	"	<DisplayName>Gev GVCP Extended Status Codes</DisplayName>\r\n"                              \
	"	<Visibility>Guru</Visibility>\r\n"                                                          \
	"	<ImposedAccessMode>RW</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GevGVCPExtendedStatusCodesValue_SwissKnife</pValue>\r\n"                            \
	"	<OnValue>1</OnValue>\r\n"                                                                   \
	"	<OffValue>0</OffValue>\r\n"                                                                 \
	"</Boolean>\r\n"                                                                                \
	"<IntSwissKnife Name=\"GevGVCPExtendedStatusCodesValue_SwissKnife\">\r\n"                       \
	"	<pVariable Name=\"SEL\" >GevGVCPExtendedStatusCodesSelectorValue</pVariable>\r\n"           \
	"	<pVariable Name=\"V11\" >GEV_BOOTSTRAP_GVCPExtendedStatusCodeVersion1_1</pVariable>\r\n"    \
	"	<pVariable Name=\"V20\" >GEV_BOOTSTRAP_GVCPExtendedStatusCodeVersion2_0</pVariable>\r\n"    \
	"	<Formula>SEL = 0 ? V11 :\r\n"                                                               \
	"			(SEL = 1 ? V20 : 0 )</Formula>\r\n"                                                 \
	"</IntSwissKnife>\r\n"                                                                          \
	"<Boolean Name=\"GevGVCPExtendedStatusCodeVersion2_0\" NameSpace=\"Standard\">\r\n"             \
	"	<ToolTip>Enables the generation of extended status codes for 2.0.</ToolTip>\r\n"            \
	"	<Description>Enables the generation of extended status codes for 2.0.</Description>\r\n"    \
	"	<DisplayName>Gev GVCP Extended Status Codes 2.0</DisplayName>\r\n"                          \
	"	<Visibility>Guru</Visibility>\r\n"                                                          \
	"	<ImposedAccessMode>RW</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GEV_BOOTSTRAP_GVCPExtendedStatusCodeVersion2_0</pValue>\r\n"                        \
	"	<OnValue>1</OnValue>\r\n"                                                                   \
	"	<OffValue>0</OffValue>\r\n"                                                                 \
	"</Boolean>\r\n"                                                                                \
	"<Boolean Name=\"GevGVCPExtendedStatusCodeVersion1_1\" NameSpace=\"Standard\">\r\n"             \
	"	<ToolTip>Enables the generation of extended status codes for 1.1.</ToolTip>\r\n"            \
	"	<Description>Enables the generation of extended status codes for 1.1.</Description>\r\n"    \
	"	<DisplayName>Gev GVCP Extended Status Codes 1.1</DisplayName>\r\n"                          \
	"	<Visibility>Guru</Visibility>\r\n"                                                          \
	"	<ImposedAccessMode>RW</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GEV_BOOTSTRAP_GVCPExtendedStatusCodeVersion1_1</pValue>\r\n"                        \
	"	<OnValue>1</OnValue>\r\n"                                                                   \
	"	<OffValue>0</OffValue>\r\n"                                                                 \
	"</Boolean>\r\n"                                                                                \
	"<Boolean Name=\"GevGVCPPendingAck\" NameSpace=\"Standard\">\r\n"                               \
	"	<ToolTip>Enables the generation of PENDING_ACK.</ToolTip>\r\n"                              \
	"	<Description>Enables the generation of PENDING_ACK.</Description>\r\n"                      \
	"	<DisplayName>Gev GVCP Pending Ack</DisplayName>\r\n"                                        \
	"	<Visibility>Guru</Visibility>\r\n"                                                          \
	"	<ImposedAccessMode>RW</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GEV_BOOTSTRAP_GVCPPendingAck</pValue>\r\n"                                          \
	"	<OnValue>1</OnValue>\r\n"                                                                   \
	"	<OffValue>0</OffValue>\r\n"                                                                 \
	"</Boolean>\r\n"                                                                                \
	"<Boolean Name=\"GevGVCPHeartbeatDisable\" NameSpace=\"Standard\">\r\n"                         \
	"	<ToolTip>Disables the GVCP heartbeat.</ToolTip>\r\n"                                        \
	"	<Description>Disables the GVCP heartbeat.</Description>\r\n"                                \
	"	<DisplayName>Gev GVCP Heartbeat Disable</DisplayName>\r\n"                                  \
	"	<Visibility>Expert</Visibility>\r\n"                                                        \
	"	<ImposedAccessMode>RO</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GEV_BOOTSTRAP_GVCPHeartbeatDisable</pValue>\r\n"                                    \
	"	<OnValue>1</OnValue>\r\n"                                                                   \
	"	<OffValue>0</OffValue>\r\n"                                                                 \
	"</Boolean>\r\n"                                                                                \
	"<Integer Name=\"GevGVCPPendingTimeout\" NameSpace=\"Standard\">\r\n"                           \
	"	<ToolTip>Indicates the longest GVCP command execution time before a device returns a PENDING_ACK.</ToolTip>\r\n"          \
	"	<Description>Indicates the longest GVCP command execution time before a device returns a PENDING_ACK.</Description>\r\n"  \
	"	<DisplayName>Gev GVCP Pending Timeout</DisplayName>\r\n"                                    \
	"	<Visibility>Guru</Visibility>\r\n"                                                          \
	"	<ImposedAccessMode>RO</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GEV_BOOTSTRAP_GVCPPendingTimeout</pValue>\r\n"                                      \
	"</Integer>\r\n"                                                                                \
	"<Integer Name=\"GevPrimaryApplicationSwitchoverKey\" NameSpace=\"Standard\">\r\n"              \
	"	<ToolTip>Controls the key to use to authenticate primary application switchover requests.</ToolTip>\r\n"         \
	"	<Description>Controls the key to use to authenticate primary application switchover requests.</Description>\r\n" \
	"	<DisplayName>Gev Primary Application Switchover Key</DisplayName>\r\n"                      \
	"	<Visibility>Guru</Visibility>\r\n"                                                          \
	"	<ImposedAccessMode>WO</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GEV_BOOTSTRAP_PrimaryApplicationSwitchoverKey</pValue>\r\n"                         \
	"</Integer>\r\n"                                                                                \
	"<Enumeration Name=\"GevGVSPExtendedIDMode\" NameSpace=\"Standard\">\r\n"                       \
	"	<ToolTip>Enables the extended IDs mode.</ToolTip>\r\n"                                      \
	"	<Description>Enables the extended IDs mode.</Description>\r\n"                              \
	"	<DisplayName>Gev GVSP Extended ID Mode</DisplayName>\r\n"                                   \
	"	<Visibility>Expert</Visibility>\r\n"                                                        \
	"	<ImposedAccessMode>RW</ImposedAccessMode>\r\n"                                              \
	"	<EnumEntry Name=\"Off\" NameSpace=\"Standard\">\r\n"                                        \
	"		<ToolTip>Off</ToolTip>\r\n"                                                             \
	"		<Description>Off</Description>\r\n"                                                     \
	"		<DisplayName>Off</DisplayName>\r\n"                                                     \
	"		<Value>0</Value>\r\n"                                                                   \
	"	</EnumEntry>\r\n"                                                                           \
	"	<EnumEntry Name=\"On\" NameSpace=\"Standard\">\r\n"                                         \
	"		<ToolTip>On</ToolTip>\r\n"                                                              \
	"		<Description>On</Description>\r\n"                                                      \
	"		<DisplayName>On</DisplayName>\r\n"                                                      \
	"		<Value>1</Value>\r\n"                                                                   \
	"	</EnumEntry>\r\n"                                                                           \
	"	<pValue>GEV_BOOTSTRAP_GVSPExtendedIDMode</pValue>\r\n"                                      \
	"</Enumeration>\r\n"                                                                            \

	"<Enumeration Name=\"GevCurrentPhysicalLinkConfiguration\" NameSpace=\"Standard\">\r\n"         \
	"	<ToolTip>Indicates the current physical link configuration of the device.</ToolTip>\r\n"    \
	"	<Description>Indicates the current physical link configuration of the device.</Description>\r\n"       \
	"	<DisplayName>Gev Current Physical Link Configuration</DisplayName>\r\n"                     \
	"	<Visibility>Expert</Visibility>\r\n"                                                        \
	"	<ImposedAccessMode>RO</ImposedAccessMode>\r\n"                                              \
	"	<EnumEntry Name=\"SingleLink\" NameSpace=\"Standard\">\r\n"                                 \
	"		<ToolTip>Single Link</ToolTip>\r\n"                                                     \
	"		<Description>Single Link</Description>\r\n"                                             \
	"		<DisplayName>Single Link</DisplayName>\r\n"                                             \
	"		<Value>0</Value>\r\n"                                                                   \
	"	</EnumEntry>\r\n"                                                                           \
	"	<EnumEntry Name=\"MultiLink\" NameSpace=\"Standard\">\r\n"                                  \
	"		<ToolTip>Multi Link</ToolTip>\r\n"                                                      \
	"		<Description>Multi Link</Description>\r\n"                                              \
	"		<DisplayName>Multi Link</DisplayName>\r\n"                                              \
	"		<Value>1</Value>\r\n"                                                                   \
	"	</EnumEntry>\r\n"                                                                           \
	"	<EnumEntry Name=\"StaticLAG\" NameSpace=\"Standard\">\r\n"                                  \
	"		<ToolTip>Static LAG</ToolTip>\r\n"                                                      \
	"		<Description>Static LAG</Description>\r\n"                                              \
	"		<DisplayName>Static LAG</DisplayName>\r\n"                                              \
	"		<Value>2</Value>\r\n"                                                                   \
	"	</EnumEntry>\r\n"                                                                           \
	"	<EnumEntry Name=\"DynamicLAG\" NameSpace=\"Standard\">\r\n"                                 \
	"		<ToolTip>Dynamic LAG</ToolTip>\r\n"                                                     \
	"		<Description>Dynamic LAG</Description>\r\n"                                             \
	"		<DisplayName>Dynamic LAG</DisplayName>\r\n"                                             \
	"		<Value>3</Value>\r\n"                                                                   \
	"	</EnumEntry>\r\n"                                                                           \
	"	<pValue>GEV_BOOTSTRAP_CurrentPhysicalLinkConfiguration</pValue>\r\n"                        \
	"</Enumeration>\r\n"                                                                            \
	"<Enumeration Name=\"GevPhysicalLinkConfiguration\" NameSpace=\"Standard\">\r\n"                \
	"	<ToolTip>Indicates the physical link configuration of the device.</ToolTip>\r\n"            \
	"	<Description>Indicates the physical link configuration of the device.</Description>\r\n"    \
	"	<DisplayName>Gev Physical Link Configuration</DisplayName>\r\n"                             \
	"	<Visibility>Expert</Visibility>\r\n"                                                        \
	"	<ImposedAccessMode>RW</ImposedAccessMode>\r\n"                                              \
	"	<EnumEntry Name=\"SingleLink\" NameSpace=\"Standard\">\r\n"                                 \
	"		<ToolTip>Single Link</ToolTip>\r\n"                                                     \
	"		<Description>Single Link</Description>\r\n"                                             \
	"		<DisplayName>Single Link</DisplayName>\r\n"                                             \
	"		<Value>0</Value>\r\n"                                                                   \
	"	</EnumEntry>\r\n"                                                                           \
	"	<EnumEntry Name=\"MultiLink\" NameSpace=\"Standard\">\r\n"                                  \
	"		<ToolTip>Multi Link</ToolTip>\r\n"                                                      \
	"		<Description>Multi Link</Description>\r\n"                                              \
	"		<DisplayName>Multi Link</DisplayName>\r\n"                                              \
	"		<Value>1</Value>\r\n"                                                                   \
	"	</EnumEntry>\r\n"                                                                           \
	"	<EnumEntry Name=\"StaticLAG\" NameSpace=\"Standard\">\r\n"                                  \
	"		<ToolTip>Static LAG</ToolTip>\r\n"                                                      \
	"		<Description>Static LAG</Description>\r\n"                                              \
	"		<DisplayName>Static LAG</DisplayName>\r\n"                                              \
	"		<Value>2</Value>\r\n"                                                                   \
	"	</EnumEntry>\r\n"                                                                           \
	"	<EnumEntry Name=\"DynamicLAG\" NameSpace=\"Standard\">\r\n"                                 \
	"		<ToolTip>Dynamic LAG</ToolTip>\r\n"                                                     \
	"		<Description>Dynamic LAG</Description>\r\n"                                             \
	"		<DisplayName>Dynamic LAG</DisplayName>\r\n"                                             \
	"		<Value>3</Value>\r\n"                                                                   \
	"	</EnumEntry>\r\n"                                                                           \
	"	<pValue>GEV_BOOTSTRAP_PhysicalLinkConfiguration</pValue>\r\n"                               \
	"</Enumeration>\r\n"                                                                            \

	"<Enumeration Name=\"GevIEEE1588Status\" NameSpace=\"Standard\">\r\n"                           \
	"	<ToolTip>Provides the state of the IEEE 1588 clock.</ToolTip>\r\n"                          \
	"	<Description>Provides the state of the IEEE 1588 clock.</Description>\r\n"                  \
	"	<DisplayName>Gev IEEE 1588 Status</DisplayName>\r\n"                                        \
	"	<Visibility>Expert</Visibility>\r\n"                                                        \
	"	<ImposedAccessMode>RO</ImposedAccessMode>\r\n"                                              \
	"	<EnumEntry Name=\"Initializing\" NameSpace=\"Standard\">\r\n"                               \
	"		<ToolTip>Initializing</ToolTip>\r\n"                                                    \
	"		<Description>Initializing</Description>\r\n"                                            \
	"		<DisplayName>Initializing</DisplayName>\r\n"                                            \
	"		<Value>1</Value>\r\n"                                                                   \
	"	</EnumEntry>\r\n"                                                                           \
	"	<EnumEntry Name=\"Faulty\" NameSpace=\"Standard\">\r\n"                                     \
	"		<ToolTip>Faulty</ToolTip>\r\n"                                                          \
	"		<Description>Faulty</Description>\r\n"                                                  \
	"		<DisplayName>Faulty</DisplayName>\r\n"                                                  \
	"		<Value>2</Value>\r\n"                                                                   \
	"	</EnumEntry>\r\n"                                                                           \
	"	<EnumEntry Name=\"Disabled\" NameSpace=\"Standard\">\r\n"                                   \
	"		<ToolTip>Disabled</ToolTip>\r\n"                                                        \
	"		<Description>Disabled</Description>\r\n"                                                \
	"		<DisplayName>Disabled</DisplayName>\r\n"                                                \
	"		<Value>3</Value>\r\n"                                                                   \
	"	</EnumEntry>\r\n"                                                                           \
	"	<EnumEntry Name=\"Listening\" NameSpace=\"Standard\">\r\n"                                  \
	"		<ToolTip>Listening</ToolTip>\r\n"                                                       \
	"		<Description>Listening</Description>\r\n"                                               \
	"		<DisplayName>Listening</DisplayName>\r\n"                                               \
	"		<Value>4</Value>\r\n"                                                                   \
	"	</EnumEntry>\r\n"                                                                           \
	"	<EnumEntry Name=\"PreMaster\" NameSpace=\"Standard\">\r\n"                                  \
	"		<ToolTip>Pre Master</ToolTip>\r\n"                                                      \
	"		<Description>Pre Master</Description>\r\n"                                              \
	"		<DisplayName>Pre Master</DisplayName>\r\n"                                              \
	"		<Value>5</Value>\r\n"                                                                   \
	"	</EnumEntry>\r\n"                                                                           \
	"	<EnumEntry Name=\"Master\" NameSpace=\"Standard\">\r\n"                                     \
	"		<ToolTip>Master</ToolTip>\r\n"                                                          \
	"		<Description>Master</Description>\r\n"                                                  \
	"		<DisplayName>Master</DisplayName>\r\n"                                                  \
	"		<Value>6</Value>\r\n"                                                                   \
	"	</EnumEntry>\r\n"                                                                           \
	"	<EnumEntry Name=\"Passive\" NameSpace=\"Standard\">\r\n"                                    \
	"		<ToolTip>Passive</ToolTip>\r\n"                                                         \
	"		<Description>Passive</Description>\r\n"                                                 \
	"		<DisplayName>Passive</DisplayName>\r\n"                                                 \
	"		<Value>7</Value>\r\n"                                                                   \
	"	</EnumEntry>\r\n"                                                                           \
	"	<EnumEntry Name=\"Uncalibrated\" NameSpace=\"Standard\">\r\n"                               \
	"		<ToolTip>Uncalibrated</ToolTip>\r\n"                                                    \
	"		<Description>Uncalibrated</Description>\r\n"                                            \
	"		<DisplayName>Uncalibrated</DisplayName>\r\n"                                            \
	"		<Value>8</Value>\r\n"                                                                   \
	"	</EnumEntry>\r\n"                                                                           \
	"	<EnumEntry Name=\"Slave\" NameSpace=\"Standard\">\r\n"                                      \
	"		<ToolTip>Slave</ToolTip>\r\n"                                                           \
	"		<Description>Slave</Description>\r\n"                                                   \
	"		<DisplayName>Slave</DisplayName>\r\n"                                                   \
	"		<Value>9</Value>\r\n"                                                                   \
	"	</EnumEntry>\r\n"                                                                           \
	"	<pValue>GEV_BOOTSTRAP_IEEE1588Status</pValue>\r\n"                                          \
	"</Enumeration>\r\n"                                                                            \

	"<Enumeration Name=\"GevCCP\" NameSpace=\"Standard\">\r\n"                                      \
	"	<ToolTip>Controls the device access privilege of an application.</ToolTip>\r\n"             \
	"	<Description>Controls the device access privilege of an application.</Description>\r\n"     \
	"	<DisplayName>Gev CCP</DisplayName>\r\n"                                                     \
	"	<Visibility>Guru</Visibility>\r\n"                                                          \
	"	<ImposedAccessMode>RW</ImposedAccessMode>\r\n"                                              \
	"	<EnumEntry Name=\"OpenAccess\" NameSpace=\"Standard\">\r\n"                                 \
	"		<ToolTip>Open Access</ToolTip>\r\n"                                                     \
	"		<Description>Open Access</Description>\r\n"                                             \
	"		<DisplayName>Open Access</DisplayName>\r\n"                                             \
	"		<Value>0</Value>\r\n"                                                                   \
	"	</EnumEntry>\r\n"                                                                           \
	"	<EnumEntry Name=\"ExclusiveAccess\" NameSpace=\"Standard\">\r\n"                            \
	"		<ToolTip>Exclusive Access</ToolTip>\r\n"                                                \
	"		<Description>Exclusive Access</Description>\r\n"                                        \
	"		<DisplayName>Exclusive Access</DisplayName>\r\n"                                        \
	"		<Value>1</Value>\r\n"                                                                   \
	"	</EnumEntry>\r\n"                                                                           \
	"	<EnumEntry Name=\"ControlAccess\" NameSpace=\"Standard\">\r\n"                              \
	"		<ToolTip>Control Access</ToolTip>\r\n"                                                  \
	"		<Description>Control Access</Description>\r\n"                                          \
	"		<DisplayName>Control Access</DisplayName>\r\n"                                          \
	"		<Value>2</Value>\r\n"                                                                   \
	"	</EnumEntry>\r\n"                                                                           \
	"	<EnumEntry Name=\"ControlAccessSwitchoverActive\" NameSpace=\"Standard\">\r\n"              \
	"		<ToolTip>Control Access Switchover Active</ToolTip>\r\n"                                \
	"		<Description>Control Access Switchover Active</Description>\r\n"                        \
	"		<DisplayName>Control Access Switchover Active</DisplayName>\r\n"                        \
	"		<Value>4</Value>\r\n"                                                                   \
	"	</EnumEntry>\r\n"                                                                           \
	"	<pValue>GEV_BOOTSTRAP_CCP</pValue>\r\n"                                                     \
	"</Enumeration>\r\n"                                                                            \
	"<Integer Name=\"GevPrimaryApplicationSocket\" NameSpace=\"Standard\">\r\n"                     \
	"	<ToolTip>Returns the UDP source port of the primary application.</ToolTip>\r\n"             \
	"	<Description>Returns the UDP source port of the primary application.</Description>\r\n"     \
	"	<DisplayName>Gev Primary Application Socket</DisplayName>\r\n"                              \
	"	<Visibility>Guru</Visibility>\r\n"                                                          \
	"	<ImposedAccessMode>RO</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GEV_BOOTSTRAP_PrimaryApplicationSocket</pValue>\r\n"                                \
	"</Integer>\r\n"                                                                                \
	"<Integer Name=\"GevPrimaryApplicationIPAddress\" NameSpace=\"Standard\">\r\n"                  \
	"	<ToolTip>Returns the address of the primary application.</ToolTip>\r\n"                     \
	"	<Description>Returns the address of the primary application.</Description>\r\n"             \
	"	<DisplayName>Gev Primary Application IP Address</DisplayName>\r\n"                          \
	"	<Visibility>Guru</Visibility>\r\n"                                                          \
	"	<ImposedAccessMode>RO</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GEV_BOOTSTRAP_PrimaryApplicationIPAddress</pValue>\r\n"                             \
	"</Integer>\r\n"                                                                                \
	"<Integer Name=\"GevMCPHostPort\" NameSpace=\"Standard\">\r\n"                                  \
	"	<ToolTip>Controls the port to which the device must send messages.</ToolTip>\r\n"           \
	"	<Description>Controls the port to which the device must send messages. Setting this value to 0 closes the message channel.</Description>\r\n"  \
	"	<DisplayName>Gev MCP Host Port</DisplayName>\r\n"                                           \
	"	<Visibility>Guru</Visibility>\r\n"                                                          \
	"	<ImposedAccessMode>RO</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GEV_BOOTSTRAP_MCPHostPort</pValue>\r\n"                                             \
	"</Integer>\r\n"                                                                                \
	"<Integer Name=\"GevMCDA\" NameSpace=\"Standard\">\r\n"                                         \
	"	<ToolTip>Controls the destination IP address for the message channel.</ToolTip>\r\n"        \
	"	<Description>Controls the destination IP address for the message channel.</Description>\r\n"      \
	"	<DisplayName>Gev MCDA</DisplayName>\r\n"                                                    \
	"	<Visibility>Guru</Visibility>\r\n"                                                          \
	"	<ImposedAccessMode>RW</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GEV_BOOTSTRAP_MCDA</pValue>\r\n"                                                    \
	"</Integer>\r\n"                                                                                \
	"<Integer Name=\"GevMCTT\" NameSpace=\"Standard\">\r\n"                                         \
	"	<ToolTip>Provides the transmission timeout value in milliseconds.</ToolTip>\r\n"            \
	"	<Description>Provides the transmission timeout value in milliseconds.</Description>\r\n"    \
	"	<DisplayName>Gev MCTT</DisplayName>\r\n"                                                    \
	"	<Visibility>Guru</Visibility>\r\n"                                                          \
	"	<ImposedAccessMode>RW</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GEV_BOOTSTRAP_MCTT</pValue>\r\n"                                                    \
	"	<Unit>ms</Unit>\r\n"                                                                        \
	"</Integer>\r\n"                                                                                \
	"<Integer Name=\"GevMCRC\" NameSpace=\"Standard\">\r\n"                                         \
	"	<ToolTip>Controls the number of retransmissions allowed when a message channel message times out.</ToolTip>\r\n"           \
	"	<Description>Controls the number of retransmissions allowed when a message channel message times out.</Description>\r\n"   \
	"	<DisplayName>Gev MCRC</DisplayName>\r\n"                                                    \
	"	<Visibility>Guru</Visibility>\r\n"                                                          \
	"	<ImposedAccessMode>RW</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GEV_BOOTSTRAP_MCRC</pValue>\r\n"                                                    \
	"</Integer>\r\n"                                                                                \
	"<Integer Name=\"GevMCSP\" NameSpace=\"Standard\">\r\n"                                         \
	"	<ToolTip>This feature indicates the source port for the message channel.</ToolTip>\r\n"     \
	"	<Description>This feature indicates the source port for the message channel.</Description>\r\n"   \
	"	<DisplayName>Gev MCSP</DisplayName>\r\n"                                                    \
	"	<Visibility>Guru</Visibility>\r\n"                                                          \
	"	<ImposedAccessMode>RO</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GEV_BOOTSTRAP_MCSP</pValue>\r\n"                                                    \
	"</Integer>\r\n"                                                                                \
	"<Integer Name=\"GevStreamChannelSelector\" NameSpace=\"Standard\">\r\n"                        \
	"	<ToolTip>Selects the stream channel to control.</ToolTip>\r\n"                              \
	"	<Description>Selects the stream channel to control.</Description>\r\n"                      \
	"	<DisplayName>Gev Stream Channel Selector</DisplayName>\r\n"                                 \
	"	<Visibility>Expert</Visibility>\r\n"                                                        \
	"	<ImposedAccessMode>RW</ImposedAccessMode>\r\n"                                              \
	"	<Value>0</Value>\r\n"                                                                       \
	"	<Min>0</Min>\r\n"                                                                           \
	"	<Max>0</Max>\r\n"                                                                           \
	"	<pSelected>GevSCPDirection</pSelected>\r\n"                                                 \
	"	<pSelected>GevSCPInterfaceIndex</pSelected>\r\n"                                            \
	"	<pSelected>GevSCPHostPort</pSelected>\r\n"                                                  \
	"	<pSelected>GevSCPSFireTestPacket</pSelected>\r\n"                                           \
	"	<pSelected>GevSCPSDoNotFragment</pSelected>\r\n"                                            \
	"	<pSelected>GevSCPSBigEndian</pSelected>\r\n"                                                \
	"	<pSelected>GevSCPSPacketSize</pSelected>\r\n"                                               \
	"	<pSelected>GevSCPD</pSelected>\r\n"                                                         \
	"	<pSelected>GevSCDA</pSelected>\r\n"                                                         \
	"	<pSelected>GevSCSP</pSelected>\r\n"                                                         \
	"	<pSelected>GevSCCFGPacketResendDestination</pSelected>\r\n"                                 \
	"	<pSelected>GevSCCFGAllInTransmission</pSelected>\r\n"                                       \
	"	<pSelected>GevSCCFGUnconditionalStreaming</pSelected>\r\n"                                  \
	"	<pSelected>GevSCCFGExtendedChunkData</pSelected>\r\n"                                       \
	"</Integer>\r\n"                                                                                \
	"<Enumeration Name=\"GevSCPDirection\" NameSpace=\"Standard\">\r\n"                             \
	"	<ToolTip>Reports the direction of the stream channel.</ToolTip>\r\n"                        \
	"	<Description>Reports the direction of the stream channel.</Description>\r\n"                \
	"	<DisplayName>Gev SCP Direction</DisplayName>\r\n"                                           \
	"	<Visibility>Guru</Visibility>\r\n"                                                          \
	"	<ImposedAccessMode>RO</ImposedAccessMode>\r\n"                                              \
	"	<EnumEntry Name=\"Transmitter\" NameSpace=\"Standard\">\r\n"                                \
	"		<ToolTip>Transmitter</ToolTip>\r\n"                                                     \
	"		<Description>Transmitter</Description>\r\n"                                             \
	"		<DisplayName>Transmitter</DisplayName>\r\n"                                             \
	"		<Value>0</Value>\r\n"                                                                   \
	"	</EnumEntry>\r\n"                                                                           \
	"	<EnumEntry Name=\"Receiver\" NameSpace=\"Standard\">\r\n"                                   \
	"		<ToolTip>Receiver</ToolTip>\r\n"                                                        \
	"		<Description>Receiver</Description>\r\n"                                                \
	"		<DisplayName>Receiver</DisplayName>\r\n"                                                \
	"		<Value>1</Value>\r\n"                                                                   \
	"	</EnumEntry>\r\n"                                                                           \
	"	<pValue>GEV_BOOTSTRAP_SCPDirection</pValue>\r\n"                                            \
	"</Enumeration>\r\n"                                                                            \

	"<Integer Name=\"GevSCPInterfaceIndex\" NameSpace=\"Standard\">\r\n"                            \
	"	<ToolTip>Index of the logical link to use.</ToolTip>\r\n"                                   \
	"	<Description>Index of the logical link to use.</Description>\r\n"                           \
	"	<DisplayName>Gev SCP Interface Index</DisplayName>\r\n"                                     \
	"	<Visibility>Guru</Visibility>\r\n"                                                          \
	"	<ImposedAccessMode>RO</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GEV_BOOTSTRAP_SCPInterfaceIndex</pValue>\r\n"                                       \
	"</Integer>\r\n"                                                                                \
	"<Integer Name=\"GevSCPHostPort\" NameSpace=\"Standard\">\r\n"                                  \
	"	<ToolTip>Controls the port of the selected channel to which a GVSP transmitter must send data stream or the port from which a GVSP receiver may receive data stream.</ToolTip>\r\n"                                                                         \
	"	<Description>Controls the port of the selected channel to which a GVSP transmitter must send data stream or the port from which a GVSP receiver may receive data stream. Setting this value to 0 closes the stream channel.</Description>\r\n"              \
	"	<DisplayName>Gev SCP Host Port</DisplayName>\r\n"                                           \
	"	<Visibility>Guru</Visibility>\r\n"                                                          \
	"	<ImposedAccessMode>RW</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GEV_BOOTSTRAP_SCPHostPort</pValue>\r\n"                                             \
	"</Integer>\r\n"                                                                                \
	"<Boolean Name=\"GevSCPSFireTestPacket\" NameSpace=\"Standard\">\r\n"                           \
	"	<ToolTip>Sends a test packet.</ToolTip>\r\n"                                                \
	"	<Description>Sends a test packet. When this feature is set, the device will fire one test packet.</Description>\r\n"      \
	"	<DisplayName>Gev SCPS Fire Test Packet</DisplayName>\r\n"                                   \
	"	<Visibility>Guru</Visibility>\r\n"                                                          \
	"	<ImposedAccessMode>RW</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GEV_BOOTSTRAP_SCPSFireTestPacket</pValue>\r\n"                                      \
	"</Boolean>\r\n"                                                                                \
	"<Boolean Name=\"GevSCPSDoNotFragment\" NameSpace=\"Standard\">\r\n"                            \
	"	<ToolTip>The state of this feature is copied into the \"do not fragment\" bit of IP header of each stream packet.</ToolTip>\r\n"    \
	"	<Description>The state of this feature is copied into the \"do not fragment\" bit of IP header of each stream packet. It can be used by the application to prevent IP fragmentation of packets on the stream channel.</Description>\r\n"                    \
	"	<DisplayName>Gev SCPS Do Not Fragment</DisplayName>\r\n"                                    \
	"	<Visibility>Guru</Visibility>\r\n"                                                          \
	"	<ImposedAccessMode>RO</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GEV_BOOTSTRAP_SCPSDoNotFragment</pValue>\r\n"                                       \
	"</Boolean>\r\n"                                                                                \
	"<Boolean Name=\"GevSCPSBigEndian\" NameSpace=\"Standard\">\r\n"                                \
	"	<ToolTip>Endianess of multi-byte pixel data for this stream.</ToolTip>\r\n"                 \
	"	<Description>Endianess of multi-byte pixel data for this stream.</Description>\r\n"         \
	"	<DisplayName>Gev SCPS Big Endian</DisplayName>\r\n"                                         \
	"	<Visibility>Guru</Visibility>\r\n"                                                          \
	"	<ImposedAccessMode>RW</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GEV_BOOTSTRAP_SCPSBigEndian</pValue>\r\n"                                           \
	"</Boolean>\r\n"                                                                                \
	"<Integer Name=\"GevSCPSPacketSize\" NameSpace=\"Standard\">\r\n"                               \
	"	<ToolTip>Specifies the stream packet size, in bytes, to send on the selected channel for a GVSP transmitter or specifies the maximum packet size supported by a GVSP receiver.</ToolTip>\r\n"                                                               \
	"	<Description>Specifies the stream packet size, in bytes, to send on the selected channel for a GVSP transmitter or specifies the maximum packet size supported by a GVSP receiver.</Description>\r\n"                                                       \
	"	<DisplayName>Gev SCPS Packet Size</DisplayName>\r\n"                                        \
	"	<Visibility>Expert</Visibility>\r\n"                                                        \
	"	<ImposedAccessMode>RW</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GEV_BOOTSTRAP_SCPSPacketSize</pValue>\r\n"                                          \
	"	<Min>576</Min>\r\n"                                                                           \
	"	<Max>8240</Max>\r\n"                                                                        \
	"	<Inc>4</Inc>\r\n"                                                                           \
	"	<Unit>B</Unit>\r\n"                                                                         \
	"</Integer>\r\n"                                                                                \
	"<Integer Name=\"GevSCPD\" NameSpace=\"Standard\">\r\n"                                         \
	"	<ToolTip>Controls the delay (in timestamp counter unit) to insert between each packet for this stream channel.</ToolTip>\r\n"     \
	"	<Description>Controls the delay (in timestamp counter unit) to insert between each packet for this stream channel. This can be used as a crude flow-control mechanism if the application or the network infrastructure cannot keep up with the packets coming from the device.</Description>\r\n" \
	"	<DisplayName>Gev SCPD</DisplayName>\r\n"                                                    \
	"	<Visibility>Expert</Visibility>\r\n"                                                        \
	"	<ImposedAccessMode>RW</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GEV_BOOTSTRAP_SCPD</pValue>\r\n"                                                    \
	"</Integer>\r\n"                                                                                \
	"<Integer Name=\"GevSCDA\" NameSpace=\"Standard\">\r\n"                                         \
	"	<ToolTip>Controls the destination IP address of the selected stream channel to which a GVSP transmitter must send data stream or the destination IP address from which a GVSP receiver may receive data stream.</ToolTip>\r\n"                              \
	"	<Description>Controls the destination IP address of the selected stream channel to which a GVSP transmitter must send data stream or the destination IP address from which a GVSP receiver may receive data stream.</Description>\r\n"                      \
	"	<DisplayName>Gev SCDA</DisplayName>\r\n"                                                    \
	"	<Visibility>Guru</Visibility>\r\n"                                                          \
	"	<ImposedAccessMode>RW</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GEV_BOOTSTRAP_SCDA</pValue>\r\n"                                                    \
	"</Integer>\r\n"                                                                                \
	"<Integer Name=\"GevSCSP\" NameSpace=\"Standard\">\r\n"                                         \
	"	<ToolTip>Indicates the source port of the stream channel.</ToolTip>\r\n"                    \
	"	<Description>Indicates the source port of the stream channel.</Description>\r\n"            \
	"	<DisplayName>Gev SCSP</DisplayName>\r\n"                                                    \
	"	<Visibility>Guru</Visibility>\r\n"                                                          \
	"	<ImposedAccessMode>RO</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GEV_BOOTSTRAP_SCSP</pValue>\r\n"                                                    \
	"</Integer>\r\n"                                                                                \
	"<Boolean Name=\"GevSCCFGPacketResendDestination\" NameSpace=\"Standard\">\r\n"                 \
	"	<ToolTip>Enables the alternate IP destination for stream packets resent due to a packet resend request.</ToolTip>\r\n"                        \
	"		<Description>Enables the alternate IP destination for stream packets resent due to a packet resend request. When True, the source IP address provided in the packet resend command packet is used. When False, the value set in the GevSCDA[GevStreamChannelSelector] feature is used.</Description>\r\n" \
	"	<DisplayName>Gev SCCFG Packet Resend Destination</DisplayName>\r\n"                         \
	"	<Visibility>Guru</Visibility>\r\n"                                                          \
	"	<ImposedAccessMode>RW</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GEV_BOOTSTRAP_SCCFGPacketResendDestination</pValue>\r\n"                            \
	"</Boolean>\r\n"                                                                                \
	"<Boolean Name=\"GevSCCFGAllInTransmission\" NameSpace=\"Standard\">\r\n"                       \
	"	<ToolTip>Enables the selected GVSP transmitter to use the single packet per data block All-in Transmission mode.</ToolTip>\r\n"         \
	"	<Description>Enables the selected GVSP transmitter to use the single packet per data block All-in Transmission mode.</Description>\r\n" \
	"	<DisplayName>Gev SCCFG All In Transmission</DisplayName>\r\n"                               \
	"	<Visibility>Guru</Visibility>\r\n"                                                          \
	"	<ImposedAccessMode>RO</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GEV_BOOTSTRAP_SCCFGAllInTransmission</pValue>\r\n"                                  \
	"</Boolean>\r\n"                                                                                \
	"<Boolean Name=\"GevSCCFGUnconditionalStreaming\" NameSpace=\"Standard\">\r\n"                  \
	"	<ToolTip>Enables the camera to continue to stream, for this stream channel, if its control channel is closed or regardless of the reception of any ICMP messages (such as destination unreachable messages).</ToolTip>\r\n"                                 \
	"	<Description>Enables the camera to continue to stream, for this stream channel, if its control channel is closed or regardless of the reception of any ICMP messages (such as destination unreachable messages).</Description>\r\n"                         \
	"	<DisplayName>Gev SCCFG Unconditional Streaming</DisplayName>\r\n"                           \
	"	<Visibility>Guru</Visibility>\r\n"                                                          \
	"	<ImposedAccessMode>RO</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GEV_BOOTSTRAP_SCCFGUnconditionalStreaming</pValue>\r\n"                             \
	"</Boolean>\r\n"                                                                                \
	"<Boolean Name=\"GevSCCFGExtendedChunkData\" NameSpace=\"Standard\">\r\n"                       \
	"	<ToolTip>Enables cameras to use the extended chunk data payload type for this stream channel.</ToolTip>\r\n"          \
	"	<Description>Enables cameras to use the extended chunk data payload type for this stream channel.</Description>\r\n"  \
	"	<DisplayName>Gev SCCFG Extended Chunk Data</DisplayName>\r\n"                               \
	"	<Visibility>Guru</Visibility>\r\n"                                                          \
	"	<ImposedAccessMode>RO</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GEV_BOOTSTRAP_SCCFGExtendedChunkData</pValue>\r\n"                                  \
	"</Boolean>\r\n"                                                                                \
	"</Group>\r\n"                                                                                  \

	"<Group Comment=\"ActionControl\">\r\n"                                                         \
	"<Integer Name=\"ActionDeviceKey\" NameSpace=\"Standard\">\r\n"                                 \
	"	<ToolTip>Provides the device key that allows the device to check the validity of action commands.</ToolTip>\r\n"         \
	"	<Description>Provides the device key that allows the device to check the validity of action commands. The device internal assertion of an action signal is only authorized if the ActionDeviceKey and the action device key value in the protocol message are equal.</Description>\r\n"  \
	"	<DisplayName>Action Device Key</DisplayName>\r\n"                                           \
	"	<Visibility>Guru</Visibility>\r\n"                                                          \
	"	<ImposedAccessMode>WO</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GEV_BOOTSTRAP_ActionDeviceKey</pValue>\r\n"                                         \
	"</Integer>\r\n"                                                                                \
	"<Enumeration Name=\"ActionUnconditionalMode\" NameSpace=\"Standard\">\r\n"                     \
	"	<ToolTip>Enables the unconditional action command mode where action commands are processed even when the primary control channel is closed.</ToolTip>\r\n"         \
	"	<Description>Enables the unconditional action command mode where action commands are processed even when the primary control channel is closed.</Description>\r\n" \
	"	<DisplayName>Action Unconditional Mode</DisplayName>\r\n"                                   \
	"	<Visibility>Guru</Visibility>\r\n"                                                          \
	"	<ImposedAccessMode>RO</ImposedAccessMode>\r\n"                                              \
	"	<EnumEntry Name=\"Off\" NameSpace=\"Standard\">\r\n"                                        \
	"		<ToolTip>Unconditional mode is disabled.</ToolTip>\r\n"                                 \
	"		<Description>Unconditional mode is disabled.</Description>\r\n"                         \
	"		<DisplayName>Off</DisplayName>\r\n"                                                     \
	"		<Value>0</Value>\r\n"                                                                   \
	"	</EnumEntry>\r\n"                                                                           \
	"	<EnumEntry Name=\"On\" NameSpace=\"Standard\">\r\n"                                         \
	"		<ToolTip>Unconditional mode is enabled.</ToolTip>\r\n"                                  \
	"		<Description>Unconditional mode is enabled.</Description>\r\n"                          \
	"		<DisplayName>On</DisplayName>\r\n"                                                      \
	"		<Value>1</Value>\r\n"                                                                   \
	"	</EnumEntry>\r\n"                                                                           \
	"	<pValue>GEV_BOOTSTRAP_ActionUnconditionalMode</pValue>\r\n"                                 \
	"</Enumeration>\r\n"                                                                            \
	"<Integer Name=\"ActionQueueSize\" NameSpace=\"Standard\">\r\n"                                 \
	"	<ToolTip>Indicates the size of the scheduled action commands queue.</ToolTip>\r\n"          \
	"	<Description>Indicates the size of the scheduled action commands queue. This number represents the maximum number of scheduled action commands that can be pending at a given point in time.</Description>\r\n"                                             \
	"	<DisplayName>Action Queue Size</DisplayName>\r\n"                                           \
	"	<Visibility>Guru</Visibility>\r\n"                                                          \
	"	<ImposedAccessMode>RO</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GEV_BOOTSTRAP_ActionQueueSize</pValue>\r\n"                                         \
	"</Integer>\r\n"                                                                                \
	"<Integer Name=\"ActionSelector\" NameSpace=\"Standard\">\r\n"                                  \
	"	<ToolTip>Selects to which Action Signal further Action settings apply.</ToolTip>\r\n"       \
	"	<Description>Selects to which Action Signal further Action settings apply.</Description>\r\n"     \
	"	<DisplayName>Action Selector</DisplayName>\r\n"                                             \
	"	<Visibility>Guru</Visibility>\r\n"                                                          \
	"	<ImposedAccessMode>RW</ImposedAccessMode>\r\n"                                              \
	"	<pValue>ActionSelectorValue</pValue>\r\n"                                                   \
	"	<pSelected>ActionGroupKey</pSelected>\r\n"                                                  \
	"	<pSelected>ActionGroupMask</pSelected>\r\n"                                                 \
	"</Integer>\r\n"                                                                                \
	"<Integer Name=\"ActionSelectorValue\">\r\n"                                                    \
	"	<Value>0</Value>\r\n"                                                                       \
	"	<Min>0</Min>\r\n"                                                                           \
	"	<pMax>ActionSelectorValueMax</pMax>\r\n"                                                    \
	"</Integer>\r\n"                                                                                \
	"<IntSwissKnife Name=\"ActionSelectorValueMax\">\r\n"                                           \
	"	<ToolTip>Action Selector Value Max</ToolTip>\r\n"                                           \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<ImposedAccessMode>RO</ImposedAccessMode>\r\n"                                              \
	"	<pVariable Name=\"N\">GEV_BOOTSTRAP_NumberOfActionSignals</pVariable>\r\n"                  \
	"	<Formula>N = 0 ? 0 : N-1</Formula>\r\n"                                                     \
	"</IntSwissKnife>\r\n"                                                                          \
	"<Integer Name=\"ActionGroupKey\" NameSpace=\"Standard\">\r\n"                                  \
	"	<ToolTip>Provides the key that the device will use to validate the action on reception of the action protocol message.</ToolTip>\r\n"         \
	"	<Description>Provides the key that the device will use to validate the action on reception of the action protocol message.</Description>\r\n" \
	"	<DisplayName>Action Group Key</DisplayName>\r\n"                                            \
	"	<Visibility>Guru</Visibility>\r\n"                                                          \
	"	<ImposedAccessMode>RW</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GEV_BOOTSTRAP_ActionGroupKey</pValue>\r\n"                                          \
	"</Integer>\r\n"                                                                                \
	"<Integer Name=\"ActionGroupMask\" NameSpace=\"Standard\">\r\n"                                 \
	"	<ToolTip>Provides the mask that the device will use to validate the action on reception of the action protocol message.</ToolTip>\r\n"         \
	"	<Description>Provides the mask that the device will use to validate the action on reception of the action protocol message.</Description>\r\n" \
	"	<DisplayName>Action Group Mask</DisplayName>\r\n"                                           \
	"	<Visibility>Guru</Visibility>\r\n"                                                          \
	"	<ImposedAccessMode>RW</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GEV_BOOTSTRAP_ActionGroupMask</pValue>\r\n"                                         \
	"</Integer>\r\n"                                                                                \
	"</Group>\r\n"                                                                                  \

	"<Group Comment=\"EventControl\">\r\n"                                                          \
	"<Category Name=\"EventControl\" NameSpace=\"Standard\">\r\n"                                   \
	"	<ToolTip>Category that contains Event control features.</ToolTip>\r\n"                      \
	"	<Description>This category that contains Event control features.</Description>\r\n"         \
	"</Category>\r\n"                                                                               \
	"<IntReg Name=\"Event1EventIDReg\">\r\n"                                                        \
	"	<Description>Event1 Event ID.</Description>\r\n"                                            \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0x0002</Address>\r\n"                                                              \
	"	<Length>2</Length>\r\n"                                                                     \
	"	<AccessMode>RO</AccessMode>\r\n"                                                            \
	"	<pPort>Event1</pPort>\r\n"                                                                  \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</IntReg>\r\n"                                                                                 \
	"<IntReg Name=\"Event1StreamChannelIdReg\">\r\n"                                                \
	"	<Description>Event1 Stream Channel ID.</Description>\r\n"                                   \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0x0004</Address>\r\n"                                                              \
	"	<Length>2</Length>\r\n"                                                                     \
	"	<AccessMode>RO</AccessMode>\r\n"                                                            \
	"	<pPort>Event1</pPort>\r\n"                                                                  \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</IntReg>\r\n"                                                                                 \
	"<Port Name=\"Event1\">\r\n"                                                                    \
	"	<EventID>1234</EventID>\r\n"                                                                \
	"</Port>\r\n"                                                                                   \

	"<IntReg Name=\"EventDataValue1\">\r\n"                                                         \
	"	<Address>0x10</Address>\r\n"                                                                \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RO</AccessMode>\r\n"                                                            \
	"	<pPort>EventData1</pPort>\r\n"                                                              \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>LittleEndian</Endianess>\r\n"                                                    \
	"</IntReg>\r\n"                                                                                 \
	"<IntReg Name=\"EventDataValue2\">\r\n"                                                         \
	"	<Address>0x14</Address>\r\n"                                                                \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RO</AccessMode>\r\n"                                                            \
	"	<pPort>EventData1</pPort>\r\n"                                                              \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>LittleEndian</Endianess>\r\n"                                                    \
	"</IntReg>\r\n"                                                                                 \
	"<Port Name=\"EventData1\">\r\n"                                                                \
	"	<EventID>5678</EventID>\r\n"                                                                \
	"</Port>\r\n"                                                                                   \
	"</Group>\r\n"                                                                                  \

	"<Group Comment=\"ChunkDataControl\">\r\n"                                                      \
	"<Category Name=\"ChunkDataControl\" NameSpace=\"Standard\">\r\n"                               \
	"	<ToolTip>Category that contains the Chunk Data control features.</ToolTip>\r\n"             \
	"	<Description>Category that contains the Chunk Data control features.</Description>\r\n"     \
	"	<DisplayName>Chunk Data Control</DisplayName>\r\n"                                          \
	"	<Visibility>Expert</Visibility>\r\n"                                                        \
	"	<pFeature>ChunkModeActive</pFeature>\r\n"                                                   \
	"	<pFeature>ChunkSelector</pFeature>\r\n"                                                     \
	"	<pFeature>ChunkEnable</pFeature>\r\n"                                                       \
	"</Category>\r\n"                                                                               \
	"<Boolean Name=\"ChunkModeActive\" NameSpace=\"Standard\">\r\n"                                 \
	"	<ToolTip>Activates the inclusion of Chunk data in the payload of the image.</ToolTip>\r\n"  \
	"	<Description>Activates the inclusion of Chunk data in the payload of the image.</Description>\r\n"     \
	"	<DisplayName>Chunk Mode Active</DisplayName>\r\n"                                           \
	"	<Visibility>Expert</Visibility>\r\n"                                                        \
	"	<ImposedAccessMode>RW</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GEV_BOOTSTRAP_ChunkModeActive</pValue>\r\n"                                         \
	"</Boolean>\r\n"                                                                                \
	"<IntReg Name=\"GEV_BOOTSTRAP_ChunkModeActive\" NameSpace=\"Custom\">\r\n"                      \
	"	<Description>Chunk Mode Active.</Description>\r\n"                                          \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0xA100</Address>\r\n"                                                              \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RW</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</IntReg>\r\n"                                                                                 \
	"<Enumeration Name=\"ImageChunkIDSet\" NameSpace=\"Standard\">\r\n"                             \
	"	<ToolTip>Set Image Chunk ID.</ToolTip>\r\n"                                                 \
	"	<Description>Set Image Chunk ID</Description>\r\n"                                          \
	"	<DisplayName>Image Chunk ID Set</DisplayName>\r\n"                                          \
	"	<Visibility>Expert</Visibility>\r\n"                                                        \
	"	<ImposedAccessMode>RW</ImposedAccessMode>\r\n"                                              \
	"	<EnumEntry Name=\"ImageChunk\" NameSpace=\"Standard\">\r\n"                                 \
	"		<ToolTip>ImageChunk</ToolTip>\r\n"                                                      \
	"		<Description>ImageChunk</Description>\r\n"                                              \
	"		<DisplayName>ImageChunk</DisplayName>\r\n"                                              \
	"		<Value>0x01233210</Value>\r\n"                                                          \
	"	</EnumEntry>\r\n"                                                                           \
	"	<EnumEntry Name=\"None\" NameSpace=\"Standard\">\r\n"                                       \
	"		<ToolTip>None</ToolTip>\r\n"                                                            \
	"		<Description>None</Description>\r\n"                                                    \
	"		<DisplayName>None</DisplayName>\r\n"                                                    \
	"		<Value>0x00000000</Value>\r\n"                                                          \
	"	</EnumEntry>\r\n"                                                                           \
	"	<pValue>GEV_BOOTSTRAP_ImageChunkID</pValue>\r\n"                                            \
	"</Enumeration>\r\n"                                                                            \

	"<IntReg Name=\"GEV_BOOTSTRAP_ImageChunkID\" NameSpace=\"Custom\">\r\n"                         \
	"	<Description>Set Image Chunk ID.</Description>\r\n"                                         \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0xA104</Address>\r\n"                                                              \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RW</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</IntReg>\r\n"                                                                                 \
	"<Enumeration Name=\"ChunkSelector\" NameSpace=\"Standard\">\r\n"                               \
	"	<ToolTip>Selects which Chunk to enable or control.</ToolTip>\r\n"                           \
	"	<Description>Selects which Chunk to enable or control.</Description>\r\n"                   \
	"	<DisplayName>Chunk Selector</DisplayName>\r\n"                                              \
	"	<Visibility>Expert</Visibility>\r\n"                                                        \
	"	<ImposedAccessMode>RW</ImposedAccessMode>\r\n"                                              \
	"	<EnumEntry Name=\"Chunk0\" NameSpace=\"Standard\">\r\n"                                     \
	"		<ToolTip>Chunk0</ToolTip>\r\n"                                                          \
	"		<Description>Chunk0</Description>\r\n"                                                  \
	"		<DisplayName>Chunk0</DisplayName>\r\n"                                                  \
	"		<Value>0</Value>\r\n"                                                                   \
	"	</EnumEntry>\r\n"                                                                           \
	"	<EnumEntry Name=\"Chunk1\" NameSpace=\"Standard\">\r\n"                                     \
	"		<ToolTip>Chunk1</ToolTip>\r\n"                                                          \
	"		<Description>Chunk1</Description>\r\n"                                                  \
	"		<DisplayName>Chunk1</DisplayName>\r\n"                                                  \
	"		<Value>1</Value>\r\n"                                                                   \
	"	</EnumEntry>\r\n"                                                                           \
	"	<EnumEntry Name=\"Chunk2\" NameSpace=\"Standard\">\r\n"                                     \
	"		<ToolTip>Chunk2</ToolTip>\r\n"                                                          \
	"		<Description>Chunk2</Description>\r\n"                                                  \
	"		<DisplayName>Chunk2</DisplayName>\r\n"                                                  \
	"		<Value>2</Value>\r\n"                                                                   \
	"	</EnumEntry>\r\n"                                                                           \
	"	<EnumEntry Name=\"Chunk3\" NameSpace=\"Standard\">\r\n"                                     \
	"		<ToolTip>Chunk3</ToolTip>\r\n"                                                          \
	"		<Description>Chunk3</Description>\r\n"                                                  \
	"		<DisplayName>Chunk3</DisplayName>\r\n"                                                  \
	"		<Value>3</Value>\r\n"                                                                   \
	"	</EnumEntry>\r\n"                                                                           \
	"	<EnumEntry Name=\"Chunk4\" NameSpace=\"Standard\">\r\n"                                     \
	"		<ToolTip>Chunk4</ToolTip>\r\n"                                                          \
	"		<Description>Chunk4</Description>\r\n"                                                  \
	"		<DisplayName>Chunk4</DisplayName>\r\n"                                                  \
	"		<Value>4</Value>\r\n"                                                                   \
	"	</EnumEntry>\r\n"                                                                           \
	"	<EnumEntry Name=\"Chunk5\" NameSpace=\"Standard\">\r\n"                                     \
	"		<ToolTip>Chunk5</ToolTip>\r\n"                                                          \
	"		<Description>Chunk5</Description>\r\n"                                                  \
	"		<DisplayName>Chunk5</DisplayName>\r\n"                                                  \
	"		<Value>5</Value>\r\n"                                                                   \
	"	</EnumEntry>\r\n"                                                                           \
	"	<EnumEntry Name=\"Chunk6\" NameSpace=\"Standard\">\r\n"                                     \
	"		<ToolTip>Chunk6</ToolTip>\r\n"                                                          \
	"		<Description>Chunk6</Description>\r\n"                                                  \
	"		<DisplayName>Chunk6</DisplayName>\r\n"                                                  \
	"		<Value>6</Value>\r\n"                                                                   \
	"	</EnumEntry>\r\n"                                                                           \
	"	<EnumEntry Name=\"Chunk7\" NameSpace=\"Standard\">\r\n"                                     \
	"		<ToolTip>Chunk7</ToolTip>\r\n"                                                          \
	"		<Description>Chunk7</Description>\r\n"                                                  \
	"		<DisplayName>Chunk7</DisplayName>\r\n"                                                  \
	"		<Value>7</Value>\r\n"                                                                   \
	"	</EnumEntry>\r\n"                                                                           \
	"	<pValue>ChunkSelectorValue</pValue>\r\n"                                                    \
	"	<pSelected>ChunkEnable</pSelected>\r\n"                                                     \
	"</Enumeration>\r\n"                                                                            \
	"<Integer Name=\"ChunkSelectorValue\">\r\n"                                                     \
	"	<Value>0</Value>\r\n"                                                                       \
	"</Integer>\r\n"                                                                                \
	"<Boolean Name=\"ChunkEnable\" NameSpace=\"Standard\">\r\n"                                     \
	"	<ToolTip>Enables the inclusion of the selected Chunk data in the payload of the image.</ToolTip>\r\n"           \
	"	<Description>Enables the inclusion of the selected Chunk data in the payload of the image.</Description>\r\n"   \
	"	<DisplayName>Chunk Enable</DisplayName>\r\n"                                                \
	"	<Visibility>Expert</Visibility>\r\n"                                                        \
	"	<ImposedAccessMode>RW</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GEV_BOOTSTRAP_ChunkEnable</pValue>\r\n"                                             \
	"</Boolean>\r\n"                                                                                \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_ChunkEnable\" NameSpace=\"Custom\">\r\n"                    \
	"	<Description>Enables the inclusion of the selected Chunk data in the payload of the image.</Description>\r\n"   \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<pAddress>ChunkEnableAddress</pAddress>\r\n"                                                \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RW</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>31</LSB>\r\n"                                                                          \
	"	<MSB>31</MSB>\r\n"                                                                          \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<IntSwissKnife Name=\"ChunkEnableAddress\">\r\n"                                               \
	"	<pVariable Name=\"INDEX\">ChunkSelectorValue</pVariable>\r\n"                               \
	"	<pVariable Name=\"BASEADDR\">ChunkEnableBaseAddress</pVariable>\r\n"                        \
	"	<Formula>BASEADDR+((INDEX)*16)</Formula>\r\n"                                               \
	"</IntSwissKnife>\r\n"                                                                          \
	"<Integer Name=\"ChunkEnableBaseAddress\" NameSpace=\"Custom\">\r\n"                            \
	"	<Value>0xA200</Value>\r\n"                                                                  \
	"</Integer>\r\n"                                                                                \

	"<Enumeration Name=\"ChunkIDSet\" NameSpace=\"Standard\">\r\n"                                  \
	"	<ToolTip>Set Chunk ID.</ToolTip>\r\n"                                                       \
	"	<Description>Set Chunk ID</Description>\r\n"                                                \
	"	<DisplayName>Chunk ID Set</DisplayName>\r\n"                                                \
	"	<Visibility>Expert</Visibility>\r\n"                                                        \
	"	<ImposedAccessMode>RW</ImposedAccessMode>\r\n"                                              \
	"	<EnumEntry Name=\"Chunk0\" NameSpace=\"Standard\">\r\n"                                     \
	"		<ToolTip>Chunk0</ToolTip>\r\n"                                                          \
	"		<Description>Chunk0</Description>\r\n"                                                  \
	"		<DisplayName>Chunk0</DisplayName>\r\n"                                                  \
	"		<Value>0x12344321</Value>\r\n"                                                          \
	"	</EnumEntry>\r\n"                                                                           \
	"	<EnumEntry Name=\"Chunk1\" NameSpace=\"Standard\">\r\n"                                     \
	"		<ToolTip>Chunk1</ToolTip>\r\n"                                                          \
	"		<Description>Chunk1</Description>\r\n"                                                  \
	"		<DisplayName>Chunk1</DisplayName>\r\n"                                                  \
	"		<Value>0x23455432</Value>\r\n"                                                          \
	"	</EnumEntry>\r\n"                                                                           \
	"	<EnumEntry Name=\"Chunk2\" NameSpace=\"Standard\">\r\n"                                     \
	"		<ToolTip>Chunk2</ToolTip>\r\n"                                                          \
	"		<Description>Chunk2</Description>\r\n"                                                  \
	"		<DisplayName>Chunk2</DisplayName>\r\n"                                                  \
	"		<Value>0x34566543</Value>\r\n"                                                          \
	"	</EnumEntry>\r\n"                                                                           \
	"	<EnumEntry Name=\"Chunk3\" NameSpace=\"Standard\">\r\n"                                     \
	"		<ToolTip>Chunk3</ToolTip>\r\n"                                                          \
	"		<Description>Chunk3</Description>\r\n"                                                  \
	"		<DisplayName>Chunk3</DisplayName>\r\n"                                                  \
	"		<Value>0x45677654</Value>\r\n"                                                          \
	"	</EnumEntry>\r\n"                                                                           \
	"	<EnumEntry Name=\"Chunk4\" NameSpace=\"Standard\">\r\n"                                     \
	"		<ToolTip>Chunk4</ToolTip>\r\n"                                                          \
	"		<Description>Chunk4</Description>\r\n"                                                  \
	"		<DisplayName>Chunk4</DisplayName>\r\n"                                                  \
	"		<Value>0x56788765</Value>\r\n"                                                          \
	"	</EnumEntry>\r\n"                                                                           \
	"	<EnumEntry Name=\"Chunk5\" NameSpace=\"Standard\">\r\n"                                     \
	"		<ToolTip>Chunk5</ToolTip>\r\n"                                                          \
	"		<Description>Chunk5</Description>\r\n"                                                  \
	"		<DisplayName>Chunk5</DisplayName>\r\n"                                                  \
	"		<Value>0x67899876</Value>\r\n"                                                          \
	"	</EnumEntry>\r\n"                                                                           \
	"	<EnumEntry Name=\"Chunk6\" NameSpace=\"Standard\">\r\n"                                     \
	"		<ToolTip>Chunk6</ToolTip>\r\n"                                                          \
	"		<Description>Chunk6</Description>\r\n"                                                  \
	"		<DisplayName>Chunk6</DisplayName>\r\n"                                                  \
	"		<Value>0x789AA987</Value>\r\n"                                                          \
	"	</EnumEntry>\r\n"                                                                           \
	"	<EnumEntry Name=\"Chunk7\" NameSpace=\"Standard\">\r\n"                                     \
	"		<ToolTip>Chunk7</ToolTip>\r\n"                                                          \
	"		<Description>Chunk7</Description>\r\n"                                                  \
	"		<DisplayName>Chunk7</DisplayName>\r\n"                                                  \
	"		<Value>0x89ABBA98</Value>\r\n"                                                          \
	"	</EnumEntry>\r\n"                                                                           \
	"	<EnumEntry Name=\"None\" NameSpace=\"Standard\">\r\n"                                       \
	"		<ToolTip>None</ToolTip>\r\n"                                                            \
	"		<Description>None</Description>\r\n"                                                    \
	"		<DisplayName>None</DisplayName>\r\n"                                                    \
	"		<Value>0x00000000</Value>\r\n"                                                          \
	"	</EnumEntry>\r\n"                                                                           \
	"	<pValue>GEV_BOOTSTRAP_ChunkID</pValue>\r\n"                                                 \
	"</Enumeration>\r\n"                                                                            \
	"<IntReg Name=\"GEV_BOOTSTRAP_ChunkID\" NameSpace=\"Custom\">\r\n"                              \
	"	<Description>Set Chunk ID.</Description>\r\n"                                               \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<pAddress>ChunkIDAddress</pAddress>\r\n"                                                    \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RW</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</IntReg>\r\n"                                                                                 \
	"<IntSwissKnife Name=\"ChunkIDAddress\">\r\n"                                                   \
	"	<pVariable Name=\"INDEX\">ChunkSelectorValue</pVariable>\r\n"                               \
	"	<pVariable Name=\"BASEADDR\">ChunkIDBaseAddress</pVariable>\r\n"                            \
	"	<Formula>BASEADDR+((INDEX)*16)</Formula>\r\n"                                               \
	"</IntSwissKnife>\r\n"                                                                          \
	"<Integer Name=\"ChunkIDBaseAddress\" NameSpace=\"Custom\">\r\n"                                \
	"	<Value>0xA204</Value>\r\n"                                                                  \
	"</Integer>\r\n"                                                                                \
	"<Enumeration Name=\"ChunkSizeSet\" NameSpace=\"Standard\">\r\n"                                \
	"	<ToolTip>Set Chunk Size.</ToolTip>\r\n"                                                     \
	"	<Description>Set Chunk Size</Description>\r\n"                                              \
	"	<DisplayName>Chunk Size Set</DisplayName>\r\n"                                              \
	"	<Visibility>Expert</Visibility>\r\n"                                                        \
	"	<ImposedAccessMode>RW</ImposedAccessMode>\r\n"                                              \
	"	<EnumEntry Name=\"Chunk0\" NameSpace=\"Standard\">\r\n"                                     \
	"		<ToolTip>Chunk0</ToolTip>\r\n"                                                          \
	"		<Description>Chunk0</Description>\r\n"                                                  \
	"		<DisplayName>Chunk0</DisplayName>\r\n"                                                  \
	"		<Value>0x4</Value>\r\n"                                                                 \
	"	</EnumEntry>\r\n"                                                                           \
	"	<EnumEntry Name=\"Chunk1\" NameSpace=\"Standard\">\r\n"                                     \
	"		<ToolTip>Chunk1</ToolTip>\r\n"                                                          \
	"		<Description>Chunk1</Description>\r\n"                                                  \
	"		<DisplayName>Chunk1</DisplayName>\r\n"                                                  \
	"		<Value>0x4</Value>\r\n"                                                                 \
	"	</EnumEntry>\r\n"                                                                           \
	"	<EnumEntry Name=\"Chunk2\" NameSpace=\"Standard\">\r\n"                                     \
	"		<ToolTip>Chunk2</ToolTip>\r\n"                                                          \
	"		<Description>Chunk2</Description>\r\n"                                                  \
	"		<DisplayName>Chunk2</DisplayName>\r\n"                                                  \
	"		<Value>0x4</Value>\r\n"                                                                 \
	"	</EnumEntry>\r\n"                                                                           \
	"	<EnumEntry Name=\"Chunk3\" NameSpace=\"Standard\">\r\n"                                     \
	"		<ToolTip>Chunk3</ToolTip>\r\n"                                                          \
	"		<Description>Chunk3</Description>\r\n"                                                  \
	"		<DisplayName>Chunk3</DisplayName>\r\n"                                                  \
	"		<Value>0x4</Value>\r\n"                                                                 \
	"	</EnumEntry>\r\n"                                                                           \
	"	<EnumEntry Name=\"Chunk4\" NameSpace=\"Standard\">\r\n"                                     \
	"		<ToolTip>Chunk4</ToolTip>\r\n"                                                          \
	"		<Description>Chunk4</Description>\r\n"                                                  \
	"		<DisplayName>Chunk4</DisplayName>\r\n"                                                  \
	"		<Value>0x4</Value>\r\n"                                                                 \
	"	</EnumEntry>\r\n"                                                                           \
	"	<EnumEntry Name=\"Chunk5\" NameSpace=\"Standard\">\r\n"                                     \
	"		<ToolTip>Chunk5</ToolTip>\r\n"                                                          \
	"		<Description>Chunk5</Description>\r\n"                                                  \
	"		<DisplayName>Chunk5</DisplayName>\r\n"                                                  \
	"		<Value>0x4</Value>\r\n"                                                                 \
	"	</EnumEntry>\r\n"                                                                           \
	"	<EnumEntry Name=\"Chunk6\" NameSpace=\"Standard\">\r\n"                                     \
	"		<ToolTip>Chunk6</ToolTip>\r\n"                                                          \
	"		<Description>Chunk6</Description>\r\n"                                                  \
	"		<DisplayName>Chunk6</DisplayName>\r\n"                                                  \
	"		<Value>0xC0</Value>\r\n"                                                                \
	"	</EnumEntry>\r\n"                                                                           \
	"	<EnumEntry Name=\"Chunk7\" NameSpace=\"Standard\">\r\n"                                     \
	"		<ToolTip>Chunk7</ToolTip>\r\n"                                                          \
	"		<Description>Chunk7</Description>\r\n"                                                  \
	"		<DisplayName>Chunk7</DisplayName>\r\n"                                                  \
	"		<Value>0xD8</Value>\r\n"                                                                \
	"	</EnumEntry>\r\n"                                                                           \
	"	<EnumEntry Name=\"None\" NameSpace=\"Standard\">\r\n"                                       \
	"		<ToolTip>None</ToolTip>\r\n"                                                            \
	"		<Description>None</Description>\r\n"                                                    \
	"		<DisplayName>None</DisplayName>\r\n"                                                    \
	"		<Value>0x0000</Value>\r\n"                                                              \
	"	</EnumEntry>\r\n"                                                                           \
	"	<pValue>GEV_BOOTSTRAP_ChunkSize</pValue>\r\n"                                               \
	"</Enumeration>\r\n"                                                                            \
	" <IntReg Name=\"GEV_BOOTSTRAP_ChunkSize\" NameSpace=\"Custom\">\r\n"                           \
	"	<Description>Set Chunk Size.</Description>\r\n"                                             \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<pAddress>ChunkSizeAddress</pAddress>\r\n"                                                  \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RW</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</IntReg>\r\n"                                                                                 \

	"<IntSwissKnife Name=\"ChunkSizeAddress\">\r\n"                                                 \
	"	<pVariable Name=\"INDEX\">ChunkSelectorValue</pVariable>\r\n"                               \
	"	<pVariable Name=\"BASEADDR\">ChunkSizeBaseAddress</pVariable>\r\n"                          \
	"	<Formula>BASEADDR+((INDEX)*16)</Formula>\r\n"                                               \
	"</IntSwissKnife>\r\n"                                                                          \
	"<Integer Name=\"ChunkSizeBaseAddress\" NameSpace=\"Custom\">\r\n"                              \
	"	<Value>0xA208</Value>\r\n"                                                                  \
	"</Integer>\r\n"                                                                                \
	"<IntReg Name=\"Chunk0Value\">\r\n"                                                             \
	"	<Address>0x00</Address>\r\n"                                                                \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RO</AccessMode>\r\n"                                                            \
	"	<pPort>Chunk0</pPort>\r\n"                                                                  \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>LittleEndian</Endianess>\r\n"                                                    \
	"</IntReg>\r\n"                                                                                 \
	"<IntReg Name=\"Chunk1Value\">\r\n"                                                             \
	"	<Address>0x00</Address>\r\n"                                                                \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RO</AccessMode>\r\n"                                                            \
	"	<pPort>Chunk1</pPort>\r\n"                                                                  \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>LittleEndian</Endianess>\r\n"                                                    \
	"</IntReg>\r\n"                                                                                 \
	"<IntReg Name=\"Chunk2Value\">\r\n"                                                             \
	"	<Address>0x00</Address>\r\n"                                                                \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RO</AccessMode>\r\n"                                                            \
	"	<pPort>Chunk2</pPort>\r\n"                                                                  \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>LittleEndian</Endianess>\r\n"                                                    \
	"</IntReg>\r\n"                                                                                 \
	"<IntReg Name=\"Chunk3Value\">\r\n"                                                             \
	"	<Address>0x00</Address>\r\n"                                                                \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RO</AccessMode>\r\n"                                                            \
	"	<pPort>Chunk3</pPort>\r\n"                                                                  \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"<Endianess>LittleEndian</Endianess>\r\n"                                                       \
	"</IntReg>\r\n"                                                                                 \
	"<IntReg Name=\"Chunk4Value\">\r\n"                                                             \
	"	<Address>0x00</Address>\r\n"                                                                \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RO</AccessMode>\r\n"                                                            \
	"	<pPort>Chunk4</pPort>\r\n"                                                                  \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"<Endianess>LittleEndian</Endianess>\r\n"                                                       \
	"</IntReg>\r\n"                                                                                 \
	"<IntReg Name=\"Chunk5Value\">\r\n"                                                             \
	"	<Address>0x00</Address>\r\n"                                                                \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RO</AccessMode>\r\n"                                                            \
	"	<pPort>Chunk5</pPort>\r\n"                                                                  \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"<Endianess>LittleEndian</Endianess>\r\n"                                                       \
	"</IntReg>\r\n"                                                                                 \
	"<IntReg Name=\"Chunk6Value\">\r\n"                                                             \
	"	<Address>0x00</Address>\r\n"                                                                \
	"	<Length>192</Length>\r\n"                                                                   \
	"	<AccessMode>RO</AccessMode>\r\n"                                                            \
	"	<pPort>Chunk6</pPort>\r\n"                                                                  \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"<Endianess>LittleEndian</Endianess>\r\n"                                                       \
	"</IntReg>\r\n"                                                                                 \
	"<IntReg Name=\"Chunk7Value\">\r\n"                                                             \
	"	<Address>0x00</Address>\r\n"                                                                \
	"	<Length>216</Length>\r\n"                                                                   \
	"	<AccessMode>RO</AccessMode>\r\n"                                                            \
	"	<pPort>Chunk7</pPort>\r\n"                                                                  \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"<Endianess>LittleEndian</Endianess>\r\n"                                                       \
	"</IntReg>\r\n"                                                                                 \
	"<Port Name=\"Chunk0\">\r\n"                                                                    \
	"	<ChunkID>12344321</ChunkID>\r\n"                                                            \
	"</Port>\r\n"                                                                                   \
	"<Port Name=\"Chunk1\">\r\n"                                                                    \
	"	<ChunkID>23455432</ChunkID>\r\n"                                                            \
	"</Port>\r\n"                                                                                   \
	"<Port Name=\"Chunk2\">\r\n"                                                                    \
	"	<ChunkID>34566543</ChunkID>\r\n"                                                            \
	"</Port>\r\n"                                                                                   \
	"<Port Name=\"Chunk3\">\r\n"                                                                    \
	"	<ChunkID>45677654</ChunkID>\r\n"                                                            \
	"</Port>\r\n"                                                                                   \
	"<Port Name=\"Chunk4\">\r\n"                                                                    \
	"	<ChunkID>56788765</ChunkID>\r\n"                                                            \
	"</Port>\r\n"                                                                                   \
	"<Port Name=\"Chunk5\">\r\n"                                                                    \
	"	<ChunkID>67899876</ChunkID>\r\n"                                                            \
	"</Port>\r\n"                                                                                   \
	"<Port Name=\"Chunk6\">\r\n"                                                                    \
	"	<ChunkID>789AA987</ChunkID>\r\n"                                                            \
	"</Port>\r\n"                                                                                   \
	"<Port Name=\"Chunk7\">\r\n"                                                                    \
	"	<ChunkID>89ABBA98</ChunkID>\r\n"                                                            \
	"</Port>\r\n"                                                                                   \
	"</Group>\r\n"                                                                                  \

	"<Group Comment=\"MacnicaOriginal\">\r\n"                                                       \
	"<Integer Name=\"GevNumberOfActionSignals\" NameSpace=\"Standard\">\r\n"                        \
	"	<ToolTip>Indicates the number of the action signals.</ToolTip>\r\n"                         \
	"	<Description>Indicates the number of the action signals.</Description>\r\n"                 \
	"	<DisplayName>Number Of Action Siglnals</DisplayName>\r\n"                                   \
	"	<Visibility>Expert</Visibility>\r\n"                                                        \
	"	<ImposedAccessMode>RO</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GEV_BOOTSTRAP_NumberOfActionSignals</pValue>\r\n"                                   \
	"</Integer>\r\n"                                                                                \
	"<Integer Name=\"GevSCSPxSupported\" NameSpace=\"Standard\">\r\n"                               \
	"	<ToolTip>Indicates the stream channel source port is supported.</ToolTip>\r\n"              \
	"	<Description>Indicates the stream channel source port is supported.</Description>\r\n"      \
	"	<DisplayName>SCSP Supported</DisplayName>\r\n"                                              \
	"	<Visibility>Expert</Visibility>\r\n"                                                        \
	"	<ImposedAccessMode>RO</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GEV_BOOTSTRAP_SCSPxSupported</pValue>\r\n"                                          \
	"</Integer>\r\n"                                                                                \
	"<Boolean Name=\"GevLegacy16bitBlockIDSupported\" NameSpace=\"Standard\">\r\n"                  \
	"	<ToolTip>Indicates the Legacy 16bit Block ID is supported.</ToolTip>\r\n"                   \
	"	<Description>Indicates the Legacy 16bit Block ID is supported.</Description>\r\n"           \
	"	<DisplayName>Legacy 16bit Block ID Supported</DisplayName>\r\n"                             \
	"	<Visibility>Expert</Visibility>\r\n"                                                        \
	"	<ImposedAccessMode>RO</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GEV_BOOTSTRAP_Legacy16bitBlockIDSupported</pValue>\r\n"                             \
	"</Boolean>\r\n"                                                                                \
	"<Integer Name=\"GevMCSPxSupported\" NameSpace=\"Standard\">\r\n"                               \
	"	<ToolTip>Indicates the message channel source port is supported.</ToolTip>\r\n"             \
	"	<Description>Indicates the message channel source port is supported.</Description>\r\n"     \
	"	<DisplayName>MCSP Supported</DisplayName>\r\n"                                              \
	"	<Visibility>Expert</Visibility>\r\n"                                                        \
	"	<ImposedAccessMode>RO</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GEV_BOOTSTRAP_MCSPxSupported</pValue>\r\n"                                          \
	"</Integer>\r\n"                                                                                \
	"<Enumeration Name=\"GevPhysicalLinkConfigurationCap\" NameSpace=\"Standard\">\r\n"             \
	"	<ToolTip>Indicates the supported physical link configuration of the device.</ToolTip>\r\n"  \
	"	<Description>Indicates the supported physical link configuration of the device.</Description>\r\n"      \
	"	<DisplayName>Gev Supported Physical Link Configuration</DisplayName>\r\n"                   \
	"	<Visibility>Expert</Visibility>\r\n"                                                        \
	"	<ImposedAccessMode>RO</ImposedAccessMode>\r\n"                                              \
	"	<EnumEntry Name=\"SingleLink\" NameSpace=\"Standard\">\r\n"                                 \
	"		<ToolTip>Single Link</ToolTip>\r\n"                                                     \
	"		<Description>Single Link</Description>\r\n"                                             \
	"		<DisplayName>Single Link</DisplayName>\r\n"                                             \
	"		<Value>1</Value>\r\n"                                                                   \
	"	</EnumEntry>\r\n"                                                                           \
	"	<EnumEntry Name=\"MultiLink\" NameSpace=\"Standard\">\r\n"                                  \
	"		<ToolTip>Multi Link</ToolTip>\r\n"                                                      \
	"		<Description>Multi Link</Description>\r\n"                                              \
	"		<DisplayName>Multi Link</DisplayName>\r\n"                                              \
	"		<Value>2</Value>\r\n"                                                                   \
	"	</EnumEntry>\r\n"                                                                           \
	"	<EnumEntry Name=\"StaticLAG\" NameSpace=\"Standard\">\r\n"                                  \
	"		<ToolTip>Static LAG</ToolTip>\r\n"                                                      \
	"		<Description>Static LAG</Description>\r\n"                                              \
	"		<DisplayName>Static LAG</DisplayName>\r\n"                                              \
	"		<Value>4</Value>\r\n"                                                                   \
	"	</EnumEntry>\r\n"                                                                           \
	"	<EnumEntry Name=\"DynamicLAG\" NameSpace=\"Standard\">\r\n"                                 \
	"		<ToolTip>Dynamic LAG</ToolTip>\r\n"                                                     \
	"		<Description>Dynamic LAG</Description>\r\n"                                             \
	"		<DisplayName>Dynamic LAG</DisplayName>\r\n"                                             \
	"		<Value>8</Value>\r\n"                                                                   \
	"	</EnumEntry>\r\n"                                                                           \
	"	<pValue>GEV_BOOTSTRAP_PhysicalLinkConfigurationCap</pValue>\r\n"                            \
	"</Enumeration>\r\n"                                                                            \
	"<Integer Name=\"PayloadType\" NameSpace=\"Standard\">\r\n"                                     \
	"	<ToolTip>Configuration of the payload type of the device.</ToolTip>\r\n"                    \
	"	<Description>Configuration of the payload type of the device.</Description>\r\n"            \
	"	<DisplayName>Payload Type</DisplayName>\r\n"                                                \
	"	<Visibility>Expert</Visibility>\r\n"                                                        \
	"	<ImposedAccessMode>RO</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GEV_BOOTSTRAP_PayloadType</pValue>\r\n"                                             \
	"</Integer>\r\n"                                                                                \
	"<Integer Name=\"FieldCount\" NameSpace=\"Standard\">\r\n"                                      \
	"	<ToolTip>Configuration of the field count of the device.</ToolTip>\r\n"                     \
	"	<Description>Configuration of the field count of the device.</Description>\r\n"             \
	"	<DisplayName>Field Count</DisplayName>\r\n"                                                 \
	"	<Visibility>Expert</Visibility>\r\n"                                                        \
	"	<ImposedAccessMode>RW</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GEV_BOOTSTRAP_FieldCount</pValue>\r\n"                                              \
	"</Integer>\r\n"                                                                                \
	"<Integer Name=\"ResendWaitFinal\" NameSpace=\"Standard\">\r\n"                                 \
	"	<ToolTip>Configuration of the waiting time from final frame.</ToolTip>\r\n"                 \
	"	<Description>Configuration of the waiting time from final frame.</Description>\r\n"         \
	"	<DisplayName>Resend Wait Final</DisplayName>\r\n"                                           \
	"	<Visibility>Expert</Visibility>\r\n"                                                        \
	"	<ImposedAccessMode>RW</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GEV_BOOTSTRAP_ResendWaitFinal</pValue>\r\n"                                         \
	"</Integer>\r\n"                                                                                \
	"<Integer Name=\"ResendFrame\" NameSpace=\"Standard\">\r\n"                                     \
	"	<ToolTip>Indicate the number of the resend count.</ToolTip>\r\n"                            \
	"	<Description>Indicate the number of the resend count.</Description>\r\n"                    \
	"	<DisplayName>Resend Frame</DisplayName>\r\n"                                                \
	"	<Visibility>Expert</Visibility>\r\n"                                                        \
	"	<ImposedAccessMode>RO</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GEV_BOOTSTRAP_ResendFrame</pValue>\r\n"                                             \
	"</Integer>\r\n"                                                                                \
	"<Integer Name=\"PayloadRawSize\" NameSpace=\"Standard\">\r\n"                                  \
	"	<ToolTip>Configuration of the payload raw size.</ToolTip>\r\n"                              \
	"	<Description>Configuration of the payload raw size.</Description>\r\n"                      \
	"	<DisplayName>Payload Raw Size</DisplayName>\r\n"                                            \
	"	<Visibility>Expert</Visibility>\r\n"                                                        \
	"	<ImposedAccessMode>RO</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GEV_BOOTSTRAP_PayloadRawSize</pValue>\r\n"                                          \
	"</Integer>\r\n"                                                                                \
	"<Integer Name=\"DebugMode\" NameSpace=\"Standard\">\r\n"                                       \
	"	<ToolTip>Configuration of the debug mode.</ToolTip>\r\n"                                    \
	"	<Description>Configuration of the debug mode.</Description>\r\n"                            \
	"	<DisplayName>Debug Mode</DisplayName>\r\n"                                                  \
	"	<Visibility>Expert</Visibility>\r\n"                                                        \
	"	<ImposedAccessMode>RW</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GEV_BOOTSTRAP_DebugMode</pValue>\r\n"                                               \
	"</Integer>\r\n"                                                                                \
	"<Boolean Name=\"IntervalOutputMode\" NameSpace=\"Standard\">\r\n"                              \
	"	<ToolTip>Interval output mode ON-OFF.</ToolTip>\r\n"                                        \
	"	<Description>Interval output mode ON-OFF.</Description>\r\n"                                \
	"	<DisplayName>IntervalOutputMode</DisplayName>\r\n"                                          \
	"	<Visibility>Expert</Visibility>\r\n"                                                        \
	"	<ImposedAccessMode>RW</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GEV_BOOTSTRAP_IntervalOutputMode</pValue>\r\n"                                      \
	"</Boolean>\r\n"                                                                                \
	"<Integer Name=\"IntervalOutputCountValue\" NameSpace=\"Standard\">\r\n"                        \
	"	<ToolTip>Interval count value between output frames.</ToolTip>\r\n"                         \
	"	<Description>Interval count value between output frames.</Description>\r\n"                 \
	"	<DisplayName>IntervalOutputCountValue</DisplayName>\r\n"                                    \
	"	<Visibility>Expert</Visibility>\r\n"                                                        \
	"	<ImposedAccessMode>RW</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GEV_BOOTSTRAP_IntervalOutputCountValue</pValue>\r\n"                                \
	"</Integer>\r\n"                                                                                \
	"<Boolean Name=\"TriggerOutputMode\" NameSpace=\"Standard\">\r\n"                               \
	"	<ToolTip>Trigger output mode ON-OFF.</ToolTip>\r\n"                                         \
	"	<Description>Trigger output mode ON-OFF.</Description>\r\n"                                 \
	"	<DisplayName>TriggerOutputMode</DisplayName>\r\n"                                           \
	"	<Visibility>Expert</Visibility>\r\n"                                                        \
	"	<ImposedAccessMode>RW</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GEV_BOOTSTRAP_TriggerOutputMode</pValue>\r\n"                                       \
	"</Boolean>\r\n"                                                                                \
	"<Command Name=\"TriggerOutputStart\" NameSpace=\"Standard\">\r\n"                              \
	"	<ToolTip>Trigger of output frames.</ToolTip>\r\n"                                           \
	"	<Description>Trigger of output frames.</Description>\r\n"                                   \
	"	<DisplayName>TriggerOutputStart</DisplayName>\r\n"                                          \
	"	<Visibility>Expert</Visibility>\r\n"                                                        \
	"	<ImposedAccessMode>WO</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GEV_BOOTSTRAP_TriggerOutputStart</pValue>\r\n"                                      \
	"	<CommandValue>1</CommandValue>\r\n"                                                         \
	"</Command>\r\n"                                                                                \
	"<Boolean Name=\"TimestampSelect\" NameSpace=\"Standard\">\r\n"                                 \
	"	<ToolTip>Timestamp source selection(0: internal, 1: external).</ToolTip>\r\n"               \
	"	<Description>Timestamp source selection(0: internal, 1: external).</Description>\r\n"       \
	"	<DisplayName>TimestampSelect</DisplayName>\r\n"                                             \
	"	<Visibility>Expert</Visibility>\r\n"                                                        \
	"	<ImposedAccessMode>RW</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GEV_BOOTSTRAP_TimestampSelect</pValue>\r\n"                                         \
	"</Boolean>\r\n"                                                                                \
	"<Integer Name=\"PTPSetPriority1\" NameSpace=\"Standard\">\r\n"                                 \
	"	<ToolTip>PTP proprity1 setting.</ToolTip>\r\n"                                              \
	"	<Description>PTP proprity1 setting.</Description>\r\n"                                      \
	"	<DisplayName>PTPSetPriority1</DisplayName>\r\n"                                             \
	"	<Visibility>Expert</Visibility>\r\n"                                                        \
	"	<ImposedAccessMode>RW</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GEV_BOOTSTRAP_PTPSetPriority1</pValue>\r\n"                                         \
	"	<Min>0</Min>\r\n"                                                                           \
	"	<Max>255</Max>\r\n"                                                                         \
	"</Integer>\r\n"                                                                                \
	"<Integer Name=\"PTPSetPriority2\" NameSpace=\"Standard\">\r\n"                                 \
	"	<ToolTip>PTP proprity2 setting.</ToolTip>\r\n"                                              \
	"	<Description>PTP proprity2 setting.</Description>\r\n"                                      \
	"	<DisplayName>PTPSetPriority2</DisplayName>\r\n"                                             \
	"	<Visibility>Expert</Visibility>\r\n"                                                        \
	"	<ImposedAccessMode>RW</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GEV_BOOTSTRAP_PTPSetPriority2</pValue>\r\n"                                         \
	"	<Min>0</Min>\r\n"                                                                           \
	"	<Max>255</Max>\r\n"                                                                         \
	"</Integer>\r\n"                                                                                \
	"<Integer Name=\"PTPSetDomainNumber\" NameSpace=\"Standard\">\r\n"                              \
	"	<ToolTip>PTP set domain number.</ToolTip>\r\n"                                              \
	"	<Description>PTP set domain number.</Description>\r\n"                                      \
	"	<DisplayName>PTPSetDomainNumber</DisplayName>\r\n"                                          \
	"	<Visibility>Expert</Visibility>\r\n"                                                        \
	"	<ImposedAccessMode>RW</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GEV_BOOTSTRAP_PTPSetDomainNumber</pValue>\r\n"                                      \
	"	<Min>0</Min>\r\n"                                                                           \
	"	<Max>255</Max>\r\n"                                                                         \
	"</Integer>\r\n"                                                                                \
	"<Boolean Name=\"PTPSetSlaveOnly\" NameSpace=\"Standard\">\r\n"                                 \
	"	<ToolTip>PTP set slave only.</ToolTip>\r\n"                                                 \
	"	<Description>PTP set slave only.</Description>\r\n"                                         \
	"	<DisplayName>PTPSetSlaveOnly</DisplayName>\r\n"                                             \
	"	<Visibility>Expert</Visibility>\r\n"                                                        \
	"	<ImposedAccessMode>RW</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GEV_BOOTSTRAP_PTPSetSlaveOnly</pValue>\r\n"                                         \
	"</Boolean>\r\n"                                                                                \
	"<Integer Name=\"PTPSetLogAnnounceInterval\" NameSpace=\"Standard\">\r\n"                       \
	"	<ToolTip>PTP set announce interval.</ToolTip>\r\n"                                          \
	"	<Description>PTP set announce interval.</Description>\r\n"                                  \
	"	<DisplayName>PTPSetLogAnnounceInterval</DisplayName>\r\n"                                   \
	"	<Visibility>Expert</Visibility>\r\n"                                                        \
	"	<ImposedAccessMode>RW</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GEV_BOOTSTRAP_PTPSetLogAnnounceInterval</pValue>\r\n"                               \
	"	<Min>0</Min>\r\n"                                                                           \
	"	<Max>4</Max>\r\n"                                                                           \
	"</Integer>\r\n"                                                                                \
	"<Integer Name=\"PTPSetLogSyncInterval\" NameSpace=\"Standard\">\r\n"                           \
	"	<ToolTip>PTP set sync interval.</ToolTip>\r\n"                                              \
	"	<Description>PTP set sync interval.</Description>\r\n"                                      \
	"	<DisplayName>PTPSetLogSyncInterval</DisplayName>\r\n"                                       \
	"	<Visibility>Expert</Visibility>\r\n"                                                        \
	"	<ImposedAccessMode>RW</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GEV_BOOTSTRAP_PTPSetLogSyncInterval</pValue>\r\n"                                   \
	"	<Min>-1</Min>\r\n"                                                                          \
	"	<Max>1</Max>\r\n"                                                                           \
	"</Integer>\r\n"                                                                                \
	"<Integer Name=\"PTPSetLogMinDelayReqInterval\" NameSpace=\"Standard\">\r\n"                    \
	"	<ToolTip>PTP set minimum delay request interval.</ToolTip>\r\n"                             \
	"	<Description>PTP set minimum delay request interval.</Description>\r\n"                     \
	"	<DisplayName>PTPSetLogMinDelayReqInterval</DisplayName>\r\n"                                \
	"	<Visibility>Expert</Visibility>\r\n"                                                        \
	"	<ImposedAccessMode>RW</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GEV_BOOTSTRAP_PTPSetLogMinDelayReqInterval</pValue>\r\n"                            \
	"	<Min>0</Min>\r\n"                                                                           \
	"	<Max>5</Max>\r\n"                                                                           \
	"</Integer>\r\n"                                                                                \
	"<Integer Name=\"PTPSetAnnounceReceiptTimeout\" NameSpace=\"Standard\">\r\n"                    \
	"	<ToolTip>PTP set announce receipt timeout.</ToolTip>\r\n"                                   \
	"	<Description>PTP set announce receipt timeout.</Description>\r\n"                           \
	"	<DisplayName>PTPSetAnnounceReceiptTimeout</DisplayName>\r\n"                                \
	"	<Visibility>Expert</Visibility>\r\n"                                                        \
	"	<ImposedAccessMode>RW</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GEV_BOOTSTRAP_PTPSetAnnounceReceiptTimeout</pValue>\r\n"                            \
	"	<Min>2</Min>\r\n"                                                                           \
	"	<Max>10</Max>\r\n"                                                                          \
	"</Integer>\r\n"                                                                                \
	"<Command Name=\"SetTimestampInitialValue\" NameSpace=\"Standard\">\r\n"                        \
	"	<ToolTip>Initializing timestamp value.</ToolTip>\r\n"                                       \
	"	<Description>Initializing timestamp value.</Description>\r\n"                               \
	"	<DisplayName>SetTimestampInitialValue</DisplayName>\r\n"                                    \
	"	<Visibility>Expert</Visibility>\r\n"                                                        \
	"	<ImposedAccessMode>WO</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GEV_BOOTSTRAP_SetTimestampInitialValue</pValue>\r\n"                                \
	"	<CommandValue>1</CommandValue>\r\n"                                                         \
	"</Command>\r\n"                                                                                \
	"<Integer Name=\"TimestampInitialValueHigh\" NameSpace=\"Standard\">\r\n"                       \
	"	<ToolTip>Set 64-bit initial value of the timestamp counter.</ToolTip>\r\n"                  \
	"	<Description>Set 64-bit initial value of the timestamp counter.</Description>\r\n"          \
	"	<DisplayName>TimestampInitialValue</DisplayName>\r\n"                                       \
	"	<Visibility>Expert</Visibility>\r\n"                                                        \
	"	<ImposedAccessMode>WO</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GEV_BOOTSTRAP_TimestampInitialValueHigh</pValue>\r\n"                               \
	"</Integer>\r\n"                                                                                \
	"<Integer Name=\"TimestampInitialValueLow\" NameSpace=\"Standard\">\r\n"                        \
	"	<ToolTip>Set 64-bit initial value of the timestamp counter.</ToolTip>\r\n"                  \
	"	<Description>Set 64-bit initial value of the timestamp counter.</Description>\r\n"          \
	"	<DisplayName>TimestampInitialValue</DisplayName>\r\n"                                       \
	"	<Visibility>Expert</Visibility>\r\n"                                                        \
	"	<ImposedAccessMode>WO</ImposedAccessMode>\r\n"                                              \
	"	<pValue>GEV_BOOTSTRAP_TimestampInitialValueLow</pValue>\r\n"                                \
	"</Integer>\r\n"                                                                                \
	"</Group>\r\n"                                                                                  \

	"<Group Comment=\"SpecialFeatures\">\r\n"                                                       \
	"<Integer Name=\"TLParamsLocked\" NameSpace=\"Standard\">\r\n"                                  \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Value>0</Value>\r\n"                                                                       \
	"	<Min>0</Min>\r\n"                                                                           \
	"	<Max>1</Max>\r\n"                                                                           \
	"</Integer>\r\n"                                                                                \
	"</Group>\r\n"                                                                                  \

	"<Group Comment=\"GEV_BOOTSTRAP\">\r\n"                                                         \
	"<StringReg Name=\"GEV_BOOTSTRAP_DeviceVendorName\" NameSpace=\"Standard\">\r\n"                \
	"	<ToolTip>Indicates the name of the device's vendor</ToolTip>\r\n"                           \
	"	<Description>This is a read only element. It is a text description that indicates the name of the device's vendor.</Description>\r\n"   \
	"	<DisplayName>Vendor Name</DisplayName>\r\n"                                                 \
	"	<Address>0x48</Address>\r\n"                                                                \
	"	<Length>32</Length>\r\n"                                                                    \
	"	<AccessMode>RO</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"</StringReg>\r\n"                                                                              \
	"<StringReg Name=\"GEV_BOOTSTRAP_DeviceModelName\" NameSpace=\"Standard\">\r\n"                 \
	"	<ToolTip>Indicates the model name of the device</ToolTip>\r\n"                              \
	"	<Description>This is a read only element. It is a text description that indicates the model name of the device.</Description>\r\n"      \
	"	<DisplayName>Model Name</DisplayName>\r\n"                                                  \
	"	<Address>0x68</Address>\r\n"                                                                \
	"	<Length>32</Length>\r\n"                                                                    \
	"	<AccessMode>RO</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"</StringReg>\r\n"                                                                              \
	"<StringReg Name=\"GEV_BOOTSTRAP_DeviceVersion\" NameSpace=\"Standard\">\r\n"                   \
	"	<ToolTip>Indicates the version of the device</ToolTip>\r\n"                                 \
	"	<Description>This is a read only element. It is a string that indicates the version of the device.</Description>\r\n"         \
	"	<DisplayName>Device Version</DisplayName>\r\n"                                              \
	"	<Address>0x88</Address>\r\n"                                                                \
	"	<Length>32</Length>\r\n"                                                                    \
	"	<AccessMode>RO</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"</StringReg>\r\n"                                                                              \
	"<StringReg Name=\"GEV_BOOTSTRAP_DeviceManufacturerInfo\" NameSpace=\"Standard\">\r\n"          \
	"	<ToolTip>Provides additional information from the vendor about the device</ToolTip>\r\n"    \
	"	<Description>This is a read only element. It is a string that provides additional information from the vendor about the camera.</Description>\r\n"  \
	"	<DisplayName>Manufacturer Info</DisplayName>\r\n"                                           \
	"	<Address>0xA8</Address>\r\n"                                                                \
	"	<Length>48</Length>\r\n"                                                                    \
	"	<AccessMode>RO</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"</StringReg>\r\n"                                                                              \
	"<StringReg Name=\"GEV_BOOTSTRAP_DeviceSerialNumber\" NameSpace=\"Standard\">\r\n"              \
	"	<ToolTip>Indicates the version of the device</ToolTip>\r\n"                                 \
	"	<Description>This is a read only element. It is a string that indicates the version of the device.</Description>\r\n"   \
	"	<DisplayName>Device Version</DisplayName>\r\n"                                              \
	"	<Address>0xD8</Address>\r\n"                                                                \
	"	<Length>16</Length>\r\n"                                                                    \
	"	<AccessMode>RO</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"</StringReg>\r\n"                                                                              \
	"<StringReg Name=\"GEV_BOOTSTRAP_DeviceUserID\" NameSpace=\"Standard\">\r\n"                    \
	"	<ToolTip>Device`s  user ID.</ToolTip>\r\n"                                                  \
	"	<Description>Device`s  user ID. This string is a unique identifier of the device.</Description>\r\n"    \
	"	<DisplayName>Device User ID</DisplayName>\r\n"                                              \
	"	<Address>0xE8</Address>\r\n"                                                                \
	"	<Length>16</Length>\r\n"                                                                    \
	"	<AccessMode>RW</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"</StringReg>\r\n"                                                                              \
	"<IntReg Name=\"GEV_BOOTSTRAP_DeviceReset\" NameSpace=\"Custom\">\r\n"                          \
	"	<ToolTip>DeviceReset</ToolTip>\r\n"                                                         \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0xD000</Address>\r\n"                                                              \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>WO</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>LittleEndian</Endianess>\r\n"                                                    \
	"	<Representation>Boolean</Representation>\r\n"                                               \
	"</IntReg>\r\n"                                                                                 \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_Width\" NameSpace=\"Custom\">\r\n"                          \
	"	<Description>Width of the image provided by the device (in pixels).</Description>\r\n"      \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0xA000</Address>\r\n"                                                              \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RW</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>31</LSB>\r\n"                                                                          \
	"	<MSB>19</MSB>\r\n"                                                                          \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_Height\" NameSpace=\"Custom\">\r\n"                         \
	"	<Description>Height of the image provided by the device (in pixels).</Description>\r\n"     \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0xA004</Address>\r\n"                                                              \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RW</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>31</LSB>\r\n"                                                                          \
	"	<MSB>19</MSB>\r\n"                                                                          \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_OffsetX\" NameSpace=\"Custom\">\r\n"                        \
	"	<Description>Horizontal offset from the origin to the region of interest (in pixels).</Description>\r\n"        \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0xA008</Address>\r\n"                                                              \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RW</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>31</LSB>\r\n"                                                                          \
	"	<MSB>19</MSB>\r\n"                                                                          \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_OffsetY\" NameSpace=\"Custom\">\r\n"                        \
	"	<Description>Vertical offset from the origin to the region of interest (in pixels).</Description>\r\n"       \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0xA00C</Address>\r\n"                                                              \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RW</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>31</LSB>\r\n"                                                                          \
	"	<MSB>19</MSB>\r\n"                                                                          \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<IntReg Name=\"GEV_BOOTSTRAP_PixelFormat\" NameSpace=\"Custom\">\r\n"                          \
	"	<Description>Format of the pixels provided by the device. It represents all the information provided by PixelCoding, PixelSize, PixelColorFilter combined in a single feature.</Description>\r\n"                                                           \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0xA01C</Address>\r\n"                                                              \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RW</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</IntReg>\r\n"                                                                                 \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_SensorWidth\" NameSpace=\"Custom\">\r\n"                    \
	"	<Description>Effective width of the sensor in pixels.</Description>\r\n"                    \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0xC000</Address>\r\n"                                                              \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RW</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>31</LSB>\r\n"                                                                          \
	"	<MSB>19</MSB>\r\n"                                                                          \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_SensorHeight\" NameSpace=\"Custom\">\r\n"                   \
	"	<Description>Height of the image provided by the device (in pixels).</Description>\r\n"     \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0xC004</Address>\r\n"                                                              \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RW</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>31</LSB>\r\n"                                                                          \
	"	<MSB>19</MSB>\r\n"                                                                          \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_AcquisitionMode\" NameSpace=\"Custom\">\r\n"                \
	"	<Description>Sets the acquisition mode of the device. It defines mainly the number of frames to capture during an acquisition and the way the acquisition stops.</Description>\r\n"                                                                         \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0xA010</Address>\r\n"                                                              \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RW</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>31</LSB>\r\n"                                                                          \
	"	<MSB>30</MSB>\r\n"                                                                          \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<IntReg Name=\"GEV_BOOTSTRAP_AcquisitionStart\" NameSpace=\"Custom\">\r\n"                     \
	"	<Description>Starts the Acquisition of the device. The number of frames captured is specified by AcquisitionMode.</Description>\r\n"   \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0xA014</Address>\r\n"                                                              \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>WO</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</IntReg>\r\n"                                                                                 \
	"<IntReg Name=\"GEV_BOOTSTRAP_AcquisitionStop\" NameSpace=\"Custom\">\r\n"                      \
	"	<Description>Stop the Acquisition of the device. The number of frames captured is specified by AcquisitionMode.</Description>\r\n"    \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0xA018</Address>\r\n"                                                              \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>WO</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</IntReg>\r\n"                                                                                 \
	"<IntReg Name=\"GEV_BOOTSTRAP_AcquisitionFrameCount\" NameSpace=\"Custom\">\r\n"                \
	"	<Description>Number of frames to acquire in MultiFrame Acquisition mode.</Description>\r\n" \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0xA030</Address>\r\n"                                                              \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RW</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</IntReg>\r\n"                                                                                 \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_PayloadSize\" NameSpace=\"Custom\">\r\n"                    \
	"	<Description>Provides the number of bytes transferred for each image or chunk on the stream channel. This includes any end-of-line, end-of-frame statistics or other stamp data. This is the total size of data payload for a data block.</Description>\r\n" \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0xA028</Address>\r\n"                                                              \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RO</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>31</LSB>\r\n"                                                                          \
	"	<MSB>2</MSB>\r\n"                                                                           \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_Version_MAJORVERSION\" NameSpace=\"Custom\">\r\n"           \
	"	<Description>Major version of the specification.</Description>\r\n"                         \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0x0</Address>\r\n"                                                                 \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RO</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>15</LSB>\r\n"                                                                          \
	"	<MSB>0</MSB>\r\n"                                                                           \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_Version_MINORVERSION\" NameSpace=\"Custom\">\r\n"           \
	"	<Description>Minor version of the specification.</Description>\r\n"                         \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0x0</Address>\r\n"                                                                 \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RO</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>31</LSB>\r\n"                                                                          \
	"	<MSB>16</MSB>\r\n"                                                                          \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_DeviceMode_ENDIANESS\" NameSpace=\"Custom\">\r\n"           \
	"	<Description>Endianess of the device registers.</Description>\r\n"                          \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0x4</Address>\r\n"                                                                 \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RO</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>0</LSB>\r\n"                                                                           \
	"	<MSB>0</MSB>\r\n"                                                                           \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_DeviceMode_DEVICECLASS\" NameSpace=\"Custom\">\r\n"         \
	"	<Description>Returns the class of the device.</Description>\r\n"                            \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0x4</Address>\r\n"                                                                 \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RO</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>3</LSB>\r\n"                                                                           \
	"	<MSB>1</MSB>\r\n"                                                                           \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_DeviceMode_CHARACTERSET\" NameSpace=\"Custom\">\r\n"        \
	"	<Description>Character set used by all the strings of the bootstrap registers.</Description>\r\n"    \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0x4</Address>\r\n"                                                                 \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RO</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>31</LSB>\r\n"                                                                          \
	"	<MSB>24</MSB>\r\n"                                                                          \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<IntSwissKnife Name=\"GevMACAddress_SwissKnife\">\r\n"                                         \
	"	<pVariable Name=\"HIGH\">GEV_BOOTSTRAP_DeviceMacAddressHigh_0</pVariable>\r\n"              \
	"	<pVariable Name=\"LOW\">GEV_BOOTSTRAP_DeviceMacAddressLow_0</pVariable>\r\n"                \
	"	<Formula>(HIGH&lt;&lt; 32) | LOW</Formula>\r\n"                                             \
	"</IntSwissKnife>\r\n"                                                                          \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_DeviceMacAddressHigh_0\" NameSpace=\"Custom\">\r\n"         \
	"	<Description>MAC address of the logical link high</Description>\r\n"                        \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0x8</Address>\r\n"                                                                 \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RO</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>31</LSB>\r\n"                                                                          \
	"	<MSB>16</MSB>\r\n"                                                                          \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_DeviceMacAddressLow_0\" NameSpace=\"Custom\">\r\n"          \
	"	<Description>MAC address of the logical link low</Description>\r\n"                         \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0xc</Address>\r\n"                                                                 \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RO</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>31</LSB>\r\n"                                                                          \
	"	<MSB>0</MSB>\r\n"                                                                           \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_PAUSEFrameReceptionCap\" NameSpace=\"Custom\">\r\n"         \
	"	<Description>Status whether incoming PAUSE Frames are handled on the given logical link.</Description>\r\n"  \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0x10</Address>\r\n"                                                                \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RO</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>0</LSB>\r\n"                                                                           \
	"	<MSB>0</MSB>\r\n"                                                                           \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_PAUSEFrameTransmissionCap\" NameSpace=\"Custom\">\r\n"      \
	"	<Description>Status whether PAUSE Frames can be generated on the given logical link.</Description>\r\n"    \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0x10</Address>\r\n"                                                                \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RO</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>1</LSB>\r\n"                                                                           \
	"	<MSB>1</MSB>\r\n"                                                                           \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_CurrentIPConfigurationLLACap\" NameSpace=\"Custom\">\r\n"   \
	"	<Description>Status whether the Link Local Address IP configuration scheme is activated on the given logical link.</Description>\r\n"   \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0x10</Address>\r\n"                                                                \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RO</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>29</LSB>\r\n"                                                                          \
	"	<MSB>29</MSB>\r\n"                                                                          \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_CurrentIPConfigurationDHCPCap\" NameSpace=\"Custom\">\r\n"  \
	"	<Description>Status whether the DHCP IP configuration scheme is activated on the given logical link.</Description>\r\n"   \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0x10</Address>\r\n"                                                                \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RO</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>30</LSB>\r\n"                                                                          \
	"	<MSB>30</MSB>\r\n"                                                                          \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_CurrentIPConfigurationPersistentIPCap\" NameSpace=\"Custom\">\r\n"                           \
	"	<Description>Status whether the PersistentIP configuration scheme is activated on the given logical link.</Description>\r\n" \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0x10</Address>\r\n"                                                                \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RO</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>31</LSB>\r\n"                                                                          \
	"	<MSB>31</MSB>\r\n"                                                                          \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_PAUSEFrameReception\" NameSpace=\"Custom\">\r\n"            \
	"	<Description>Controls whether incoming PAUSE Frames are handled on the given logical link.</Description>\r\n"  \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0x14</Address>\r\n"                                                                \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RO</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>0</LSB>\r\n"                                                                           \
	"	<MSB>0</MSB>\r\n"                                                                           \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_PAUSEFrameTransmission\" NameSpace=\"Custom\">\r\n"         \
	"	<Description>Controls whether PAUSE Frames can be generated on the given logical link.</Description>\r\n"   \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0x14</Address>\r\n"                                                                \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RO</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>1</LSB>\r\n"                                                                           \
	"	<MSB>1</MSB>\r\n"                                                                           \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_CurrentIPConfigurationLLA\" NameSpace=\"Custom\">\r\n"      \
	"	<Description>Controls whether the Link Local Address IP configuration scheme is activated on the given logical link.</Description>\r\n"  \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0x14</Address>\r\n"                                                                \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RW</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>29</LSB>\r\n"                                                                          \
	"	<MSB>29</MSB>\r\n"                                                                          \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_CurrentIPConfigurationDHCP\" NameSpace=\"Custom\">\r\n"     \
	"	<Description>Controls whether the DHCP IP configuration scheme is activated on the given logical link.</Description>\r\n"  \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0x14</Address>\r\n"                                                                \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RW</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>30</LSB>\r\n"                                                                          \
	"	<MSB>30</MSB>\r\n"                                                                          \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_CurrentIPConfigurationPersistentIP\" NameSpace=\"Custom\">\r\n"                                \
	"	<Description>Controls whether the PersistentIP configuration scheme is activated on the given logical link.</Description>\r\n" \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0x14</Address>\r\n"                                                                \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RW</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>31</LSB>\r\n"                                                                          \
	"	<MSB>31</MSB>\r\n"                                                                          \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_CurrentIPAddress_0\" NameSpace=\"Custom\">\r\n"             \
	"	<Description>Current IP Address of this device</Description>\r\n"                           \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0x24</Address>\r\n"                                                                \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RO</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>31</LSB>\r\n"                                                                          \
	"	<MSB>0</MSB>\r\n"                                                                           \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_CurrentSubnetMask_0\" NameSpace=\"Custom\">\r\n"            \
	"	<Description>Current Subnet Mask of this device</Description>\r\n"                          \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0x34</Address>\r\n"                                                                \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RO</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>31</LSB>\r\n"                                                                          \
	"	<MSB>0</MSB>\r\n"                                                                           \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_CurrentGateway_0\" NameSpace=\"Custom\">\r\n"               \
	"	<Description>Current Subnet Mask of this device</Description>\r\n"                          \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0x44</Address>\r\n"                                                                \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RO</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>31</LSB>\r\n"                                                                          \
	"	<MSB>0</MSB>\r\n"                                                                           \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<StringReg Name=\"GEV_BOOTSTRAP_FirstURL\">\r\n"                                               \
	"	<ToolTip>Indicates the first URL to the XML device description file</ToolTip>\r\n"          \
	"	<Description>This is a read only element. It indicates the first URL to the XML device description file.</Description>\r\n"   \
	"	<DisplayName>First URL</DisplayName>\r\n"                                                   \
	"	<Visibility>Guru</Visibility>\r\n"                                                          \
	"	<Address>0x200</Address>\r\n"                                                               \
	"	<Length>512</Length>\r\n"                                                                   \
	"	<AccessMode>RO</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"</StringReg>\r\n"                                                                              \
	"<StringReg Name=\"GEV_BOOTSTRAP_SecondURL\">\r\n"                                              \
	"	<ToolTip>Indicates the second URL to the XML device description file</ToolTip>\r\n"         \
	"	<Description>This is a read only element. It indicates the second URL to the XML device description file.</Description>\r\n"   \
	"	<DisplayName>Second URL</DisplayName>\r\n"                                                  \
	"	<Visibility>Guru</Visibility>\r\n"                                                          \
	"	<Address>0x400</Address>\r\n"                                                               \
	"	<Length>512</Length>\r\n"                                                                   \
	"	<AccessMode>RO</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"</StringReg>\r\n"                                                                              \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_NumberOfInterfaces\" NameSpace=\"Custom\">\r\n"             \
	"	<Description>Indicates the number of physical network interfaces on this device. A device must have at least one network interface.</Description>\r\n" \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0x600</Address>\r\n"                                                               \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RO</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>31</LSB>\r\n"                                                                          \
	"	<MSB>29</MSB>\r\n"                                                                          \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_PersistentIPAddress_0\" NameSpace=\"Custom\">\r\n"          \
	"	<Description>Persistent IP Address of this device</Description>\r\n"                        \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0x64c</Address>\r\n"                                                               \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RW</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>31</LSB>\r\n"                                                                          \
	"	<MSB>0</MSB>\r\n"                                                                           \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_PersistentSubnetMask_0\" NameSpace=\"Custom\">\r\n"         \
	"	<Description>Persistent Subnet Mask of this device</Description>\r\n"                       \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0x65c</Address>\r\n"                                                               \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RW</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>31</LSB>\r\n"                                                                          \
	"	<MSB>0</MSB>\r\n"                                                                           \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_PersistentDefaultGateway_0\" NameSpace=\"Custom\">\r\n"     \
	"	<Description>Persistent Subnet Mask of this device</Description>\r\n"                       \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0x66c</Address>\r\n"                                                               \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RW</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>31</LSB>\r\n"                                                                          \
	"	<MSB>0</MSB>\r\n"                                                                           \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_MessageChannelCount\" NameSpace=\"Custom\">\r\n"            \
	"	<Description>Indicates the number of message channels supported by this device. It can take two values: 0 or 1.</Description>\r\n"   \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0x900</Address>\r\n"                                                               \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RO</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>31</LSB>\r\n"                                                                          \
	"	<MSB>0</MSB>\r\n"                                                                           \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_StreamChannelCount\" NameSpace=\"Custom\">\r\n"             \
	"	<Description>Indicates the number of stream channels supported by this device. It can take any value from 1 to 512.</Description>\r\n"   \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0x904</Address>\r\n"                                                               \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RO</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>31</LSB>\r\n"                                                                          \
	"	<MSB>0</MSB>\r\n"                                                                           \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_GevActiveLinkCount\" NameSpace=\"Custom\">\r\n"             \
	"	<Description>Indicates the current number of active logical links.</Description>\r\n"       \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0x910</Address>\r\n"                                                               \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RO</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>31</LSB>\r\n"                                                                          \
	"	<MSB>28</MSB>\r\n"                                                                          \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_SupportedOptionSelector\" NameSpace=\"Custom\">\r\n"        \
	"	<Description>This feature indicates the source port for the message channel.</Description>\r\n"  \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0x934</Address>\r\n"                                                               \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RW</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>31</LSB>\r\n"                                                                          \
	"	<MSB>0</MSB>\r\n"                                                                           \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_GVCPCapability_userdefinedname\" NameSpace=\"Custom\">\r\n" \
	"	<Description>User Defined Name</Description>\r\n"                                           \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0x934</Address>\r\n"                                                               \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RO</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>0</LSB>\r\n"                                                                           \
	"	<MSB>0</MSB>\r\n"                                                                           \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_GVCPCapability_serialnumber\" NameSpace=\"Custom\">\r\n"    \
	"	<Description>Serial Number</Description>\r\n"                                               \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0x934</Address>\r\n"                                                               \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RO</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>1</LSB>\r\n"                                                                           \
	"	<MSB>1</MSB>\r\n"                                                                           \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_GVCPCapability_heartbeatdisable\" NameSpace=\"Custom\">\r\n" \
	"	<Description>heartbeat disable</Description>\r\n"                                           \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0x934</Address>\r\n"                                                               \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RO</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>2</LSB>\r\n"                                                                           \
	"	<MSB>2</MSB>\r\n"                                                                           \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_GVCPCapability_linkspeed\" NameSpace=\"Custom\">\r\n"       \
	"	<Description>link speed</Description>\r\n"                                                  \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0x934</Address>\r\n"                                                               \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RO</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>3</LSB>\r\n"                                                                           \
	"	<MSB>3</MSB>\r\n"                                                                           \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_GVCPCapability_CCP_applicationportIP\" NameSpace=\"Custom\">\r\n"  \
	"	<Description>CCP application port IP</Description>\r\n"                                     \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0x934</Address>\r\n"                                                               \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RO</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>4</LSB>\r\n"                                                                           \
	"	<MSB>4</MSB>\r\n"                                                                           \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_GVCPCapability_manifesttable\" NameSpace=\"Custom\">\r\n"   \
	"	<Description>manifest table</Description>\r\n"                                              \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0x934</Address>\r\n"                                                               \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RO</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>5</LSB>\r\n"                                                                           \
	"	<MSB>5</MSB>\r\n"                                                                           \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_GVCPCapability_testdata\" NameSpace=\"Custom\">\r\n"        \
	"	<Description>test data</Description>\r\n"                                                   \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0x934</Address>\r\n"                                                               \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RO</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>6</LSB>\r\n"                                                                           \
	"	<MSB>6</MSB>\r\n"                                                                           \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_GVCPCapability_discoveryACKdelay\" NameSpace=\"Custom\">\r\n" \
	"	<Description>discovery ACK delay</Description>\r\n"                                         \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0x934</Address>\r\n"                                                               \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RO</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>7</LSB>\r\n"                                                                           \
	"	<MSB>7</MSB>\r\n"                                                                           \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_GVCPCapability_writablediscoveryACKdelay\" NameSpace=\"Custom\">\r\n"    \
	"	<Description>writable discovery ACK delay</Description>\r\n"                                \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0x934</Address>\r\n"                                                               \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RO</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>8</LSB>\r\n"                                                                           \
	"	<MSB>8</MSB>\r\n"                                                                           \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_GVCPCapability_extendedstatuscodefor11\" NameSpace=\"Custom\">\r\n"   \
	"	<Description>extended status code for 1.1</Description>\r\n"                                \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0x934</Address>\r\n"                                                               \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RO</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>9</LSB>\r\n"                                                                           \
	"	<MSB>9</MSB>\r\n"                                                                           \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_GVCPCapability_primaryapplicationswitchover\" NameSpace=\"Custom\">\r\n"  \
	"	<Description>primary application switchover</Description>\r\n"                              \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0x934</Address>\r\n"                                                               \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RO</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>10</LSB>\r\n"                                                                          \
	"	<MSB>10</MSB>\r\n"                                                                          \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_GVCPCapability_unconditionalACTION\" NameSpace=\"Custom\">\r\n"  \
	"	<Description>unconditional ACTION</Description>\r\n"                                        \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0x934</Address>\r\n"                                                               \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RO</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>11</LSB>\r\n"                                                                          \
	"	<MSB>11</MSB>\r\n"                                                                          \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_GVCPCapability_IEEE1588support\" NameSpace=\"Custom\">\r\n" \
	"	<Description>IEEE1588 support</Description>\r\n"                                            \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0x934</Address>\r\n"                                                               \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RO</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>12</LSB>\r\n"                                                                          \
	"	<MSB>12</MSB>\r\n"                                                                          \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_GVCPCapability_extendedstatuscodefor20\" NameSpace=\"Custom\">\r\n"  \
	"	<Description>extended status code for 2.0</Description>\r\n"                                \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0x934</Address>\r\n"                                                               \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RO</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>13</LSB>\r\n"                                                                          \
	"	<MSB>13</MSB>\r\n"                                                                          \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_GVCPCapability_scheduledactioncommand\" NameSpace=\"Custom\">\r\n"   \
	"	<Description>scheduled action command</Description>\r\n"                                    \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0x934</Address>\r\n"                                                               \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RO</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>14</LSB>\r\n"                                                                          \
	"	<MSB>14</MSB>\r\n"                                                                          \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_GVCPCapability_ACTION\" NameSpace=\"Custom\">\r\n"          \
	"	<Description>ACTION</Description>\r\n"                                                      \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0x934</Address>\r\n"                                                               \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RO</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>25</LSB>\r\n"                                                                          \
	"	<MSB>25</MSB>\r\n"                                                                          \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_GVCPCapability_PENDINGACK\" NameSpace=\"Custom\">\r\n"      \
	"	<Description>PENDING ACK</Description>\r\n"                                                 \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0x934</Address>\r\n"                                                               \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RO</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>26</LSB>\r\n"                                                                          \
	"	<MSB>26</MSB>\r\n"                                                                          \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_GVCPCapability_EVENTDATA\" NameSpace=\"Custom\">\r\n"       \
	"	<Description>EVENTDATA</Description>\r\n"                                                   \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0x934</Address>\r\n"                                                               \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RO</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>27</LSB>\r\n"                                                                          \
	"	<MSB>27</MSB>\r\n"                                                                          \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_GVCPCapability_EVENT\" NameSpace=\"Custom\">\r\n"           \
	"	<Description>EVENT</Description>\r\n"                                                       \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0x934</Address>\r\n"                                                               \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RO</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>28</LSB>\r\n"                                                                          \
	"	<MSB>28</MSB>\r\n"                                                                          \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_GVCPCapability_PACKETRESEND\" NameSpace=\"Custom\">\r\n"    \
	"	<Description>PACKETRESEND</Description>\r\n"                                                \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0x934</Address>\r\n"                                                               \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RO</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>29</LSB>\r\n"                                                                          \
	"	<MSB>29</MSB>\r\n"                                                                          \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_GVCPCapability_WRITEMEM\" NameSpace=\"Custom\">\r\n"        \
	"	<Description>WRITEMEM</Description>\r\n"                                                    \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0x934</Address>\r\n"                                                               \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RO</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>30</LSB>\r\n"                                                                          \
	"	<MSB>30</MSB>\r\n"                                                                          \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_GVCPCapability_concatenation\" NameSpace=\"Custom\">\r\n"   \
	"	<Description>concatenation</Description>\r\n"                                               \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0x934</Address>\r\n"                                                               \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RO</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>31</LSB>\r\n"                                                                          \
	"	<MSB>31</MSB>\r\n"                                                                          \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_HeartbeatTimeout\" NameSpace=\"Custom\">\r\n"               \
	"	<Description>Controls the current heartbeat timeout in milliseconds.</Description>\r\n"     \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0x938</Address>\r\n"                                                               \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RW</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>31</LSB>\r\n"                                                                          \
	"	<MSB>0</MSB>\r\n"                                                                           \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<IntSwissKnife Name=\"GevTimestampTickFrequency_SwissKnife\">\r\n"                             \
	"	<pVariable Name=\"HIGH\">GEV_BOOTSTRAP_TimestampTickFrequencyHigh</pVariable>\r\n"          \
	"	<pVariable Name=\"LOW\">GEV_BOOTSTRAP_TimestampTickFrequencyLow</pVariable>\r\n"            \
	"	<Formula>(HIGH&lt;&lt; 32) | LOW</Formula>\r\n"                                             \
	"</IntSwissKnife>\r\n"                                                                          \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_TimestampTickFrequencyHigh\" NameSpace=\"Custom\">\r\n"     \
	"	<Description>Indicates the number of timestamp ticks in 1 second (frequency in Hz).</Description>\r\n"   \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0x93C</Address>\r\n"                                                               \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RO</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>31</LSB>\r\n"                                                                          \
	"	<MSB>0</MSB>\r\n"                                                                           \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_TimestampTickFrequencyLow\" NameSpace=\"Custom\">\r\n"      \
	"	<Description>Indicates the number of timestamp ticks in 1 second (frequency in Hz).</Description>\r\n"   \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0x940</Address>\r\n"                                                               \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RO</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>31</LSB>\r\n"                                                                          \
	"	<MSB>0</MSB>\r\n"                                                                           \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<IntReg Name=\"GEV_BOOTSTRAP_TimestampControl\" NameSpace=\"Custom\">\r\n"                     \
	"	<Description>Timestamp control register.</Description>\r\n"                                 \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0x944</Address>\r\n"                                                               \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>WO</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</IntReg>\r\n"                                                                                 \
	"<IntSwissKnife Name=\"GevTimestampValue_SwissKnife\">\r\n"                                     \
	"	<pVariable Name=\"HIGH\">GEV_BOOTSTRAP_TimestampValuHigh</pVariable>\r\n"                   \
	"	<pVariable Name=\"LOW\">GEV_BOOTSTRAP_TimestampValuLow</pVariable>\r\n"                     \
	"	<Formula>(HIGH&lt;&lt; 32) | LOW</Formula>\r\n"                                             \
	"</IntSwissKnife>\r\n"                                                                          \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_TimestampValuHigh\" NameSpace=\"Custom\">\r\n"              \
	"	<Description>Returns the latched 64-bit value of the timestamp counter.</Description>\r\n"  \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0x948</Address>\r\n"                                                               \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RO</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>31</LSB>\r\n"                                                                          \
	"	<MSB>0</MSB>\r\n"                                                                           \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_TimestampValuLow\" NameSpace=\"Custom\">\r\n"               \
	"	<Description>Returns the latched 64-bit value of the timestamp counter.</Description>\r\n"  \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0x94C</Address>\r\n"                                                               \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RO</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>31</LSB>\r\n"                                                                          \
	"	<MSB>0</MSB>\r\n"                                                                           \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_IEEE1588\" NameSpace=\"Custom\">\r\n"                       \
	"	<Description>Enables the IEEE 1588 Precision Time Protocol to control the timestamp register.</Description>\r\n"  \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0x954</Address>\r\n"                                                               \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RW</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>12</LSB>\r\n"                                                                          \
	"	<MSB>12</MSB>\r\n"                                                                          \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_GVCPExtendedStatusCodeVersion1_1\" NameSpace=\"Custom\">\r\n" \
	"	<Description>Enables the generation of extended status codes.</Description>\r\n"            \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0x954</Address>\r\n"                                                               \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RW</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>29</LSB>\r\n"                                                                          \
	"	<MSB>29</MSB>\r\n"                                                                          \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_GVCPExtendedStatusCodeVersion2_0\" NameSpace=\"Custom\">\r\n" \
	"	<Description>Enables the generation of extended status codes.</Description>\r\n"            \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0x954</Address>\r\n"                                                               \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RW</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>13</LSB>\r\n"                                                                          \
	"	<MSB>13</MSB>\r\n"                                                                          \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_GVCPPendingAck\" NameSpace=\"Custom\">\r\n"                 \
	"	<Description>Enables the generation of PENDING_ACK.</Description>\r\n"                      \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0x954</Address>\r\n"                                                               \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RW</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>30</LSB>\r\n"                                                                          \
	"	<MSB>30</MSB>\r\n"                                                                          \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_GVCPHeartbeatDisable\" NameSpace=\"Custom\">\r\n"           \
	"	<Description>Disables the GVCP heartbeat.</Description>\r\n"                                \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0x954</Address>\r\n"                                                               \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RO</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>31</LSB>\r\n"                                                                          \
	"	<MSB>31</MSB>\r\n"                                                                          \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<IntReg Name=\"GEV_BOOTSTRAP_GVCPPendingTimeout\" NameSpace=\"Custom\">\r\n"                   \
	"	<Description>Indicates the longest GVCP command execution time before a device returns a PENDING_ACK.</Description>\r\n"   \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0x958</Address>\r\n"                                                               \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RO</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</IntReg>\r\n"                                                                                 \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_PrimaryApplicationSwitchoverKey\" NameSpace=\"Custom\">\r\n" \
	"	<Description>Controls the key to use to authenticate primary application switchover requests</Description>\r\n"   \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0x95C</Address>\r\n"                                                               \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>WO</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>31</LSB>\r\n"                                                                          \
	"	<MSB>16</MSB>\r\n"                                                                          \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_GVSPExtendedIDMode\" NameSpace=\"Custom\">\r\n"             \
	"	<Description>Enables the extended IDs mode.</Description>\r\n"                              \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0x960</Address>\r\n"                                                               \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RW</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>1</LSB>\r\n"                                                                           \
	"	<MSB>1</MSB>\r\n"                                                                           \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_CurrentPhysicalLinkConfiguration\" NameSpace=\"Custom\">\r\n"     \
	"	<Description>Indicates the current physical link configuration of the device.</Description>\r\n"  \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0x4</Address>\r\n"                                                                 \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RO</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>7</LSB>\r\n"                                                                           \
	"	<MSB>4</MSB>\r\n"                                                                           \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_PhysicalLinkConfiguration\" NameSpace=\"Custom\">\r\n"      \
	"	<Description>Indicates the physical link configuration of the device.</Description>\r\n"    \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0x968</Address>\r\n"                                                               \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RW</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>31</LSB>\r\n"                                                                          \
	"	<MSB>30</MSB>\r\n"                                                                          \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_IEEE1588Status\" NameSpace=\"Custom\">\r\n"                 \
	"	<Description>Provides the state of the IEEE 1588 clock.</Description>\r\n"                  \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0x96C</Address>\r\n"                                                               \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RO</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>31</LSB>\r\n"                                                                          \
	"	<MSB>28</MSB>\r\n"                                                                          \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_CCP\" NameSpace=\"Custom\">\r\n"                            \
	"	<Description>Controls the device access privilege of an application.</Description>\r\n"     \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0xA00</Address>\r\n"                                                               \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RW</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>31</LSB>\r\n"                                                                          \
	"	<MSB>29</MSB>\r\n"                                                                          \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_PrimaryApplicationSocket\" NameSpace=\"Custom\">\r\n"       \
	"	<Description>Returns the UDP source port of the primary application.</Description>\r\n"     \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0xA04</Address>\r\n"                                                               \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RO</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>31</LSB>\r\n"                                                                          \
	"	<MSB>16</MSB>\r\n"                                                                          \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<IntReg Name=\"GEV_BOOTSTRAP_PrimaryApplicationIPAddress\" NameSpace=\"Custom\">\r\n"          \
	"	<Description>Returns the address of the primary application.</Description>\r\n"             \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0xA14</Address>\r\n"                                                               \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RO</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</IntReg>\r\n"                                                                                 \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_MCPHostPort\" NameSpace=\"Custom\">\r\n"                    \
	"	<Description>Controls the port to which the device must send messages. Setting this value to 0 closes the message channel.</Description>\r\n"  \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0xB00</Address>\r\n"                                                               \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RO</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>31</LSB>\r\n"                                                                          \
	"	<MSB>16</MSB>\r\n"                                                                          \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<IntReg Name=\"GEV_BOOTSTRAP_MCDA\" NameSpace=\"Custom\">\r\n"                                 \
	"	<Description>Controls the destination IP address for the message channel.</Description>\r\n" \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0xB10</Address>\r\n"                                                               \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RW</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</IntReg>\r\n"                                                                                 \
	"<IntReg Name=\"GEV_BOOTSTRAP_MCTT\" NameSpace=\"Custom\">\r\n"                                 \
	"	<Description>Provides the transmission timeout value in milliseconds.</Description>\r\n"    \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0xB14</Address>\r\n"                                                               \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RW</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</IntReg>\r\n"                                                                                 \
	"<IntReg Name=\"GEV_BOOTSTRAP_MCRC\" NameSpace=\"Custom\">\r\n"                                            \
	"	<Description>Controls the number of retransmissions allowed when a message channel message times out.</Description>\r\n"  \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0xB18</Address>\r\n"                                                               \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RW</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</IntReg>\r\n"                                                                                 \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_MCSP\" NameSpace=\"Custom\">\r\n"                           \
	"	<Description>This feature indicates the source port for the message channel.</Description>\r\n"  \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0xB1C</Address>\r\n"                                                               \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RO</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>31</LSB>\r\n"                                                                          \
	"	<MSB>16</MSB>\r\n"                                                                          \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_SCPDirection\" NameSpace=\"Custom\">\r\n"                   \
	"	<Description>Reports the direction of the stream channel.</Description>\r\n"                \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0xD00</Address>\r\n"                                                               \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RO</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>0</LSB>\r\n"                                                                           \
	"	<MSB>0</MSB>\r\n"                                                                           \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_SCPInterfaceIndex\" NameSpace=\"Custom\">\r\n"              \
	"	<Description>Index of the logical link to use.</Description>\r\n"                           \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0xD00</Address>\r\n"                                                               \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RO</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>15</LSB>\r\n"                                                                          \
	"	<MSB>12</MSB>\r\n"                                                                          \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_SCPHostPort\" NameSpace=\"Custom\">\r\n"                    \
	"	<Description>Controls the port of the selected channel to which a GVSP transmitter must send data stream or the port from which a GVSP receiver may receive data stream. Setting this value to 0 closes the stream channel.</Description>\r\n"              \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0xD00</Address>\r\n"                                                               \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RW</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>31</LSB>\r\n"                                                                          \
	"	<MSB>16</MSB>\r\n"                                                                          \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_SCPSFireTestPacket\" NameSpace=\"Custom\">\r\n"             \
	"	<Description>Sends a test packet. When this feature is set, the device will fire one test packet.</Description>\r\n"  \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0xD04</Address>\r\n"                                                               \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RW</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>0</LSB>\r\n"                                                                           \
	"	<MSB>0</MSB>\r\n"                                                                           \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_SCPSDoNotFragment\" NameSpace=\"Custom\">\r\n"              \
	"	<Description>The state of this feature is copied into the \"do not fragment\" bit of IP header of each stream packet. It can be used by the application to prevent IP fragmentation of packets on the stream channel.</Description>\r\n"                    \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0xD04</Address>\r\n"                                                               \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RO</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>1</LSB>\r\n"                                                                           \
	"	<MSB>1</MSB>\r\n"                                                                           \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_SCPSBigEndian\" NameSpace=\"Custom\">\r\n"                  \
	"	<Description>Endianess of multi-byte pixel data for this stream.</Description>\r\n"         \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0xD04</Address>\r\n"                                                               \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RW</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>2</LSB>\r\n"                                                                           \
	"	<MSB>2</MSB>\r\n"                                                                           \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_SCPSPacketSize\" NameSpace=\"Custom\">\r\n"                 \
	"	<Description>Specifies the stream packet size, in bytes, to send on the selected channel for a GVSP transmitter or specifies the maximum packet size supported by a GVSP receiver.</Description>\r\n"                                                       \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0xD04</Address>\r\n"                                                               \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RW</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>31</LSB>\r\n"                                                                          \
	"	<MSB>16</MSB>\r\n"                                                                          \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<IntReg Name=\"GEV_BOOTSTRAP_SCPD\" NameSpace=\"Custom\">\r\n"                                 \
	"	<Description>Controls the delay (in timestamp counter unit) to insert between each packet for this stream channel. This can be used as a crude flow-control mechanism if the application or the network infrastructure cannot keep up with the packets coming from the device.</Description>\r\n"  \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0xD08</Address>\r\n"                                                               \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RW</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</IntReg>\r\n"                                                                                 \
	"<IntReg Name=\"GEV_BOOTSTRAP_SCDA\" NameSpace=\"Custom\">\r\n"                                 \
	"	<Description>Indicates the source port of the stream channel.</Description>\r\n"            \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0xD18</Address>\r\n"                                                               \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RW</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</IntReg>\r\n"                                                                                 \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_SCSP\" NameSpace=\"Custom\">\r\n"                           \
	"	<Description>Indicates the source port of the stream channel.</Description>\r\n"            \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0xD1C</Address>\r\n"                                                               \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RO</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>31</LSB>\r\n"                                                                          \
	"	<MSB>16</MSB>\r\n"                                                                          \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_StreamChannel0BigAndLittleEndian\" NameSpace=\"Custom\">\r\n" \
	"	<Description>Stream Channel 0 Big And Little Endian</Description>\r\n"                      \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0xD20</Address>\r\n"                                                               \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RO</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>0</LSB>\r\n"                                                                           \
	"	<MSB>0</MSB>\r\n"                                                                           \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_StreamChannel0IPReassembly\" NameSpace=\"Custom\">\r\n"     \
	"	<Description>Stream Channel 0 IP Reassembly</Description>\r\n"                              \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0xD20</Address>\r\n"                                                               \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RO</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>1</LSB>\r\n"                                                                           \
	"	<MSB>1</MSB>\r\n"                                                                           \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_StreamChannel0MultiZonen\" NameSpace=\"Custom\">\r\n"       \
	"	<Description>Stream Channel 0 Multi Zonen</Description>\r\n"                                \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0xD20</Address>\r\n"                                                               \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RO</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>27</LSB>\r\n"                                                                          \
	"	<MSB>27</MSB>\r\n"                                                                          \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_StreamChannel0PacketResendDestination\" NameSpace=\"Custom\">\r\n"  \
	"	<Description>Stream Channel 0 Packet Resend Destination</Description>\r\n"                  \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0xD20</Address>\r\n"                                                               \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RO</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>28</LSB>\r\n"                                                                          \
	"	<MSB>28</MSB>\r\n"                                                                          \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \

	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_StreamChannel0AllInTransmission\" NameSpace=\"Custom\">\r\n" \
	"	<Description>Stream Channel 0 All In Transmission</Description>\r\n"                        \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0xD20</Address>\r\n"                                                               \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RO</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>29</LSB>\r\n"                                                                          \
	"	<MSB>29</MSB>\r\n"                                                                          \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_StreamChannel0UnconditionalStreaming\" NameSpace=\"Custom\">\r\n"   \
	"	<Description>Stream Channel 0 Unconditional Streaming</Description>\r\n"                    \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0xD20</Address>\r\n"                                                               \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RO</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>30</LSB>\r\n"                                                                          \
	"	<MSB>30</MSB>\r\n"                                                                          \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_StreamChannel0ExtendedChunkData\" NameSpace=\"Custom\">\r\n" \
	"	<Description>Stream Channel 0 Extended Chunk Data</Description>\r\n"                        \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0xD20</Address>\r\n"                                                               \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RO</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>31</LSB>\r\n"                                                                          \
	"	<MSB>31</MSB>\r\n"                                                                          \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_SCCFGPacketResendDestination\" NameSpace=\"Custom\">\r\n"   \
	"	<Description>Enables the alternate IP destination for stream packets resent due to a packet resend request. When True, the source IP address provided in the packet resend command packet is used. When False, the value set in the GevSCDA[GevStreamChannelSelector] feature is used.</Description>\r\n" \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0xD24</Address>\r\n"                                                               \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RW</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>28</LSB>\r\n"                                                                          \
	"	<MSB>28</MSB>\r\n"                                                                          \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_SCCFGAllInTransmission\" NameSpace=\"Custom\">\r\n"         \
	"	<Description>Enables the selected GVSP transmitter to use the single packet per data block All-in Transmission mode.</Description>\r\n"  \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0xD24</Address>\r\n"                                                               \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RO</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>29</LSB>\r\n"                                                                          \
	"	<MSB>29</MSB>\r\n"                                                                          \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_SCCFGUnconditionalStreaming\" NameSpace=\"Custom\">\r\n"    \
	"	<Description>Enables the camera to continue to stream, for this stream channel, if its control channel is closed or regardless of the reception of any ICMP messages (such as destination unreachable messages).</Description>\r\n"                         \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0xD24</Address>\r\n"                                                               \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RO</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>30</LSB>\r\n"                                                                          \
	"	<MSB>30</MSB>\r\n"                                                                          \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_SCCFGExtendedChunkData\" NameSpace=\"Custom\">\r\n"         \
	"	<Description>Enables cameras to use the extended chunk data payload type for this stream channel.</Description>\r\n"   \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0xD24</Address>\r\n"                                                               \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RO</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>31</LSB>\r\n"                                                                          \
	"	<MSB>31</MSB>\r\n"                                                                          \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<IntReg Name=\"GEV_BOOTSTRAP_ActionDeviceKey\" NameSpace=\"Custom\">\r\n"                      \
	"	<Description>Provides the device key that allows the device to check the validity of action commands.</Description>\r\n"  \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0x90C</Address>\r\n"                                                               \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>WO</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</IntReg>\r\n"                                                                                 \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_ActionUnconditionalMode\" NameSpace=\"Custom\">\r\n"        \
	"	<Description>Enables the unconditional action command mode where action commands are processed even when the primary control channel is closed.</Description>\r\n" \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0x954</Address>\r\n"                                                               \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RO</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>28</LSB>\r\n"                                                                          \
	"	<MSB>28</MSB>\r\n"                                                                          \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<IntReg Name=\"GEV_BOOTSTRAP_ActionQueueSize\" NameSpace=\"Custom\">\r\n"                      \
	"	<Description>Indicates the size of the scheduled action commands queue.</Description>\r\n"  \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0x970</Address>\r\n"                                                               \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RO</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</IntReg>\r\n"                                                                                 \
	"<IntReg Name=\"GEV_BOOTSTRAP_ActionGroupKey\" NameSpace=\"Custom\">\r\n"                       \
	"	<Description>Provides the key that the device will use to validate the action on reception of the action protocol message.</Description>\r\n" \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<pAddress>ActionGroupKeyAddress</pAddress>\r\n"                                             \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RW</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</IntReg>\r\n"                                                                                 \
	"<IntSwissKnife Name=\"ActionGroupKeyAddress\">\r\n"                                            \
	"	<pVariable Name=\"INDEX\">ActionSelectorValue</pVariable>\r\n"                              \
	"	<pVariable Name=\"BASEADDR\">ActionGroupKeyBaseAddress</pVariable>\r\n"                     \
	"	<Formula>BASEADDR+((INDEX)*16)</Formula>\r\n"                                               \
	"</IntSwissKnife>\r\n"                                                                          \
	"<Integer Name=\"ActionGroupKeyBaseAddress\" NameSpace=\"Custom\">\r\n"                         \
	"	<Value>0x9800</Value>\r\n"                                                                  \
	"</Integer>\r\n"                                                                                \
	"<IntReg Name=\"GEV_BOOTSTRAP_ActionGroupMask\" NameSpace=\"Custom\">\r\n"                      \
	"	<Description>Provides the mask that the device will use to validate the action on reception of the action protocol message.</Description>\r\n"  \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<pAddress>ActionGroupMaskAddress</pAddress>\r\n"                                            \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RW</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</IntReg>\r\n"                                                                                 \
	"<IntSwissKnife Name=\"ActionGroupMaskAddress\">\r\n"                                           \
	"	<pVariable Name=\"INDEX\">ActionSelectorValue</pVariable>\r\n"                              \
	"	<pVariable Name=\"BASEADDR\">ActionGroupMaskBaseAddress</pVariable>\r\n"                    \
	"	<Formula>BASEADDR+((INDEX)*16)</Formula>\r\n"                                               \
	"</IntSwissKnife>\r\n"                                                                          \
	"<Integer Name=\"ActionGroupMaskBaseAddress\" NameSpace=\"Custom\">\r\n"                        \
	"	<Value>0x9804</Value>\r\n"                                                                  \
	"</Integer>\r\n"                                                                                \

"	<IntReg Name=\"GEV_BOOTSTRAP_NumberOfActionSignals\" NameSpace=\"Custom\">\r\n"                 \
	"	<Description>Indicates the number of the action signals.</Description>\r\n"                 \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0x908</Address>\r\n"                                                               \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RO</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</IntReg>\r\n"                                                                                 \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_SCSPxSupported\" NameSpace=\"Custom\">\r\n"                 \
	"	<Description>Indicates the stream channel source port is supported.</Description>\r\n"      \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0x92C</Address>\r\n"                                                               \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RO</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>0</LSB>\r\n"                                                                           \
	"	<MSB>0</MSB>\r\n"                                                                           \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_Legacy16bitBlockIDSupported\" NameSpace=\"Custom\">\r\n"    \
	"	<Description>Indicates the Legacy 16bit Block ID is supported.</Description>\r\n"           \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0x92C</Address>\r\n"                                                               \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RO</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>1</LSB>\r\n"                                                                           \
	"	<MSB>1</MSB>\r\n"                                                                           \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_MCSPxSupported\" NameSpace=\"Custom\">\r\n"                 \
	"	<Description>Indicates the message channel source port is supported.</Description>\r\n"     \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0x930</Address>\r\n"                                                               \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RO</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>0</LSB>\r\n"                                                                           \
	"	<MSB>0</MSB>\r\n"                                                                           \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_PhysicalLinkConfigurationCap\" NameSpace=\"Custom\">\r\n"   \
	"	<Description>Indicates the supported physical link configuration of the device.</Description>\r\n"  \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0x964</Address>\r\n"                                                               \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RO</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>31</LSB>\r\n"                                                                          \
	"	<MSB>28</MSB>\r\n"                                                                          \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \

"	<MaskedIntReg Name=\"GEV_BOOTSTRAP_PayloadType\" NameSpace=\"Custom\">\r\n"                     \
	"	<Description>Configuration of the payload type of the device.</Description>\r\n"            \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0xA020</Address>\r\n"                                                              \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RO</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>31</LSB>\r\n"                                                                          \
	"	<MSB>16</MSB>\r\n"                                                                          \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_FieldCount\" NameSpace=\"Custom\">\r\n"                     \
	"	<Description>Configuration of the field count of the device.</Description>\r\n"             \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0xA024</Address>\r\n"                                                              \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RW</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>31</LSB>\r\n"                                                                          \
	"	<MSB>31</MSB>\r\n"                                                                          \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<IntReg Name=\"GEV_BOOTSTRAP_ResendWaitFinal\" NameSpace=\"Custom\">\r\n"                      \
	"	<Description>Configuration of the waiting time from final frame.</Description>\r\n"         \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0xA034</Address>\r\n"                                                              \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RW</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</IntReg>\r\n"                                                                                 \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_ResendFrame\" NameSpace=\"Custom\">\r\n"                    \
	"	<Description>Indicate the number of the resend count.</Description>\r\n"                    \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0xA040</Address>\r\n"                                                              \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RO</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>31</LSB>\r\n"                                                                          \
	"	<MSB>26</MSB>\r\n"                                                                          \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_PayloadRawSize\" NameSpace=\"Custom\">\r\n"                 \
	"	<Description>Configuration of the payload raw size.</Description>\r\n"                      \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0xA044</Address>\r\n"                                                              \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RO</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>31</LSB>\r\n"                                                                          \
	"	<MSB>17</MSB>\r\n"                                                                          \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<IntReg Name=\"GEV_BOOTSTRAP_DebugMode\" NameSpace=\"Custom\">\r\n"                            \
	"	<Description>Configuration of the debug mode.</Description>\r\n"                            \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0xA048</Address>\r\n"                                                              \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RW</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</IntReg>\r\n"                                                                                 \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_IntervalOutputMode\" NameSpace=\"Custom\">\r\n"             \
	"	<Description>Interval output mode ON-OFF.</Description>\r\n"                                \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0xA300</Address>\r\n"                                                              \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RW</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>31</LSB>\r\n"                                                                          \
	"	<MSB>31</MSB>\r\n"                                                                          \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<IntReg Name=\"GEV_BOOTSTRAP_IntervalOutputCountValue\" NameSpace=\"Custom\">\r\n"             \
	"	<Description>Interval count value between output frames.</Description>\r\n"                 \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0xA304</Address>\r\n"                                                              \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RW</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</IntReg>\r\n"                                                                                 \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_TriggerOutputMode\" NameSpace=\"Custom\">\r\n"              \
	"	<Description>Trigger output mode ON-OFF.</Description>\r\n"                                 \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0xA308</Address>\r\n"                                                              \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RW</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>31</LSB>\r\n"                                                                          \
	"	<MSB>31</MSB>\r\n"                                                                          \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<IntReg Name=\"GEV_BOOTSTRAP_TriggerOutputStart\" NameSpace=\"Custom\">\r\n"                   \
	"	<Description>Trigger output mode ON-OFF.</Description>\r\n"                                 \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0xA30C</Address>\r\n"                                                              \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>WO</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</IntReg>\r\n"                                                                                 \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_TimestampSelect\" NameSpace=\"Custom\">\r\n"                \
	"	<Description>Timestamp source selection(0: internal, 1: external).</Description>\r\n"       \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0xC010</Address>\r\n"                                                              \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RW</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>31</LSB>\r\n"                                                                          \
	"	<MSB>31</MSB>\r\n"                                                                          \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<IntReg Name=\"GEV_BOOTSTRAP_PTPSetPriority1\" NameSpace=\"Custom\">\r\n"                      \
	"	<Description>PTP proprity1 setting.</Description>\r\n"                                      \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0xC020</Address>\r\n"                                                              \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RW</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</IntReg>\r\n"                                                                                 \
	"<IntReg Name=\"GEV_BOOTSTRAP_PTPSetPriority2\" NameSpace=\"Custom\">\r\n"                      \
	"	<Description>PTP proprity2 setting.</Description>\r\n"                                      \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0xC024</Address>\r\n"                                                              \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RW</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</IntReg>\r\n"                                                                                 \
	"<IntReg Name=\"GEV_BOOTSTRAP_PTPSetDomainNumber\" NameSpace=\"Custom\">\r\n"                   \
	"	<Description>PTP set domain number.</Description>\r\n"                                      \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0xC028</Address>\r\n"                                                              \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RW</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</IntReg>\r\n"                                                                                 \
	"<MaskedIntReg Name=\"GEV_BOOTSTRAP_PTPSetSlaveOnly\" NameSpace=\"Custom\">\r\n"                \
	"	<Description>PTP set slave only.</Description>\r\n"                                         \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0xC02C</Address>\r\n"                                                              \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RW</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<LSB>31</LSB>\r\n"                                                                          \
	"	<MSB>31</MSB>\r\n"                                                                          \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</MaskedIntReg>\r\n"                                                                           \
	"<IntReg Name=\"GEV_BOOTSTRAP_PTPSetLogAnnounceInterval\" NameSpace=\"Custom\">\r\n"            \
	"	<Description>PTP set announce interval.</Description>\r\n"                                  \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0xC030</Address>\r\n"                                                              \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RW</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<Sign>Signed</Sign>\r\n"                                                                    \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</IntReg>\r\n"                                                                                 \
	"<IntReg Name=\"GEV_BOOTSTRAP_PTPSetLogSyncInterval\" NameSpace=\"Custom\">\r\n"                \
	"	<Description>PTP set sync interval.</Description>\r\n"                                      \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0xC034</Address>\r\n"                                                              \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RW</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<Sign>Signed</Sign>\r\n"                                                                    \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</IntReg>\r\n"                                                                                 \
	"<IntReg Name=\"GEV_BOOTSTRAP_PTPSetLogMinDelayReqInterval\" NameSpace=\"Custom\">\r\n"         \
	"	<Description>PTP set minimum delay request interval.</Description>\r\n"                     \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0xC038</Address>\r\n"                                                              \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RW</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<Sign>Signed</Sign>\r\n"                                                                    \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</IntReg>\r\n"                                                                                 \
	"<IntReg Name=\"GEV_BOOTSTRAP_PTPSetAnnounceReceiptTimeout\" NameSpace=\"Custom\">\r\n"         \
	"	<Description>PTP set announce receipt timeout.</Description>\r\n"                           \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0xC03C</Address>\r\n"                                                              \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>RW</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</IntReg>\r\n"                                                                                 \
	"<IntReg Name=\"GEV_BOOTSTRAP_SetTimestampInitialValue\" NameSpace=\"Custom\">\r\n"             \
	"	<Description>Initializing timestamp value.</Description>\r\n"                               \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0xC040</Address>\r\n"                                                              \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>WO</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</IntReg>\r\n"                                                                                 \
	"<IntReg Name=\"GEV_BOOTSTRAP_TimestampInitialValueHigh\" NameSpace=\"Custom\">\r\n"            \
	"	<Description>Set 64-bit initial value of the timestamp counter.</Description>\r\n"          \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0xC044</Address>\r\n"                                                              \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>WO</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</IntReg>\r\n"                                                                                 \
	"<IntReg Name=\"GEV_BOOTSTRAP_TimestampInitialValueLow\" NameSpace=\"Custom\">\r\n"             \
	"	<Description>Set 64-bit initial value of the timestamp counter.</Description>\r\n"          \
	"	<Visibility>Invisible</Visibility>\r\n"                                                     \
	"	<Address>0xC048</Address>\r\n"                                                              \
	"	<Length>4</Length>\r\n"                                                                     \
	"	<AccessMode>WO</AccessMode>\r\n"                                                            \
	"	<pPort>Device</pPort>\r\n"                                                                  \
	"	<Cachable>NoCache</Cachable>\r\n"                                                           \
	"	<Sign>Unsigned</Sign>\r\n"                                                                  \
	"	<Endianess>BigEndian</Endianess>\r\n"                                                       \
	"</IntReg>\r\n"                                                                                 \
	"</Group>\r\n"                                                                                  \

"<Port Name=\"Device\">\r\n"                                                                        \
"	<ToolTip>Port giving access to the device.</ToolTip>\r\n"                                       \
"	<SwapEndianess>Yes</SwapEndianess>\r\n"                                                         \
"</Port>\r\n"                                                                                       \
"</RegisterDescription>\r\n";


//------------------------------------------------------------------------------
// prototypes
//------------------------------------------------------------------------------
