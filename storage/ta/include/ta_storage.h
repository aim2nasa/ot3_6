/*
 * Copyright (c) 2014, STMicroelectronics International N.V.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __TA_STORAGE_H
#define __TA_STORAGE_H

#define TA_STORAGE_UUID { 0xd18e20ef, 0x01fe, 0x4e9d, \
	{ 0xa4, 0x69, 0x69, 0xe3, 0x22, 0x7e, 0xe6, 0xe9 } }
#define TA_STORAGE2_UUID { 0x731e279e, 0xaafb, 0x4575, \
	{ 0xa7, 0x71, 0x38, 0xca, 0xa6, 0xf0, 0xcc, 0xa6 } }

#define TA_STORAGE_CMD_OPEN			0
#define TA_STORAGE_CMD_CLOSE			1
#define TA_STORAGE_CMD_READ			2
#define TA_STORAGE_CMD_WRITE			3
#define TA_STORAGE_CMD_CREATE			4
#define TA_STORAGE_CMD_SEEK			5
#define TA_STORAGE_CMD_UNLINK			6
#define TA_STORAGE_CMD_RENAME			7
#define TA_STORAGE_CMD_TRUNC			8
#define TA_STORAGE_CMD_ALLOC_ENUM		9
#define TA_STORAGE_CMD_FREE_ENUM		10
#define TA_STORAGE_CMD_RESET_ENUM		11
#define TA_STORAGE_CMD_START_ENUM		12
#define TA_STORAGE_CMD_NEXT_ENUM		13
#define TA_STORAGE_CMD_CREATE_OVERWRITE		14
#define TA_STORAGE_CMD_KEY_IN_PERSISTENT	15
#define TA_STORAGE_CMD_LOOP			16
#define TA_STORAGE_CMD_RESTRICT_USAGE		17
#define TA_STORAGE_CMD_ALLOC_OBJ		18
#define TA_STORAGE_CMD_FREE_OBJ			19
#define TA_STORAGE_CMD_RESET_OBJ		20
#define TA_STORAGE_CMD_GET_OBJ_INFO		21
#define TA_STORAGE_CMD_OPEN_ID_IN_SHM		22
#define TA_STORAGE_CMD_CREATE_ID_IN_SHM		23
#define TA_STORAGE_CMD_CREATEOVER_ID_IN_SHM	24
#define TA_STORAGE_CMD_RENAME_ID_IN_SHM		25

#endif /*__TA_STORAGE_H*/
