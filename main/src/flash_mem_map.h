//=====================================================================================
// Copyright (C) 2017-2021 Macnica Inc. All Rights Reserved.
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
//! @brief  Memory map of the target board.
//=====================================================================================

#pragma once

#include "../../main_bsp/system.h"

//------------------------------------------------------------------------------
// define and structures
//------------------------------------------------------------------------------
//
//  < flash memory assign >
//  +------------------------------------------+ SF_CTR_AVL_MEM_BASE
//  |                                          |
//  :                     :                    :
//  |                                          |
//  +------------------------------------------+ SF_CTR_AVL_MEM_BASE + FLASH_OFFSET_CURRENT_IP_CONFIGURATION
//  | "CURRENT_IP_CONFIGURATION"    (4 bytes)  |
//  +------------------------------------------+ SF_CTR_AVL_MEM_BASE + FLASH_OFFSET_USER_DEFINED_NAME
//  | "USER_DEFINED_NAME"           (16 bytes) |
//  +------------------------------------------+ SF_CTR_AVL_MEM_BASE + FLASH_OFFSET_PERSISTENT_IP_ADDRESS_0
//  | "PERSISTENT_IP_ADDRESS_0"     (4 bytes)  |
//  +------------------------------------------+ SF_CTR_AVL_MEM_BASE + FLASH_OFFSET_PERSISTENT_SUBNET_MASK_0
//  | "PERSISTENT_SUBNET_MASK_0"    (4 bytes)  |
//  +------------------------------------------+ SF_CTR_AVL_MEM_BASE + FLASH_OFFSET_PERSISTENT_GATEWAY_0
//  | "PERSISTENT_GATEWAY_0"        (4 bytes)  |
//  +------------------------------------------+ SF_CTR_AVL_MEM_BASE + FLASH_OFFSET_GVCP_CONFIGURATION
//  | "GVCP_CONFIGURATION"          (4 bytes)  |
//  +------------------------------------------+ SF_CTR_AVL_MEM_BASE + SF_CTR_AVL_MEM_SPAN
//
//! flash memory assign
#define FLASH_OFFSET_CURRENT_IP_CONFIGURATION     (FLASH_OFFSET_USER_DEFINED_NAME - 4)
#define FLASH_OFFSET_USER_DEFINED_NAME            (FLASH_OFFSET_PERSISTENT_IP_ADDRESS_0 - 16)
#define FLASH_OFFSET_PERSISTENT_IP_ADDRESS_0      (FLASH_OFFSET_PERSISTENT_SUBNET_MASK_0 - 4)
#define FLASH_OFFSET_PERSISTENT_SUBNET_MASK_0     (FLASH_OFFSET_PERSISTENT_GATEWAY_0 - 4)
#define FLASH_OFFSET_PERSISTENT_GATEWAY_0         (FLASH_OFFSET_GVCP_CONFIGURATION - 4)
#define FLASH_OFFSET_GVCP_CONFIGURATION           (SF_CTR_AVL_MEM_SPAN - 4)

//------------------------------------------------------------------------------
// global variables
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// prototypes
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// inline functions
//------------------------------------------------------------------------------
