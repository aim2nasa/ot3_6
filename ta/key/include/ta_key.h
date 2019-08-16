#ifndef TA_KEY_H
#define TA_KEY_H

/* This UUID is generated with the ITU-T UUID generator at
   http://www.itu.int/ITU-T/asn1/uuid.html */
#define TA_KEY_UUID { 0xda5f2bcf, 0x1aed, 0x48c0, \
	{ 0xbf, 0x8e, 0x15, 0x5a, 0x9f, 0xbc, 0x35, 0xe7 } }

#define TA_KEY_CMD_GENERATE             1
#define TA_KEY_CMD_LIST					2
#define TA_KEY_CMD_ALLOC_ENUM			3
#define TA_KEY_CMD_FREE_ENUM			4
#define TA_KEY_CMD_RESET_ENUM			5
#define TA_KEY_CMD_START_ENUM			6
#define TA_KEY_CMD_NEXT_ENUM			7
#define TA_KEY_CMD_OPEN					8
#define TA_KEY_CMD_CLOSE				9
#define TA_KEY_CMD_CLOSE_AND_DELETE		10
#define TA_KEY_CMD_GET_OBJECT_BUFFER_ATTRIBUTE	11
#define TA_KEY_CMD_GET_OBJECT_VALUE_ATTRIBUTE	12
#define TA_KEY_CMD_ALLOC_OPER					13
#define TA_KEY_CMD_FREE_OPER					14
#define TA_KEY_CMD_SET_KEY_OPER					15
#define TA_KEY_CMD_CIPHER_INIT					16
#define TA_KEY_CMD_CIPHER_UPDATE				17
#define TA_KEY_CMD_CIPHER_DO_FINAL				18
#define TA_KEY_CMD_SET_KEY2_OPER				19
#define TA_KEY_CMD_AE_INIT						20

#endif /*TA_KEY_H */
