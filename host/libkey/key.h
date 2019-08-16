#ifndef __KEY_H__
#define __KEY_H__

#include <tee_client_api.h>
#include <session.h>
#include <ta_key.h>
#include <tee_api_types.h>

#define TEE_STORAGE_PRIVATE				0x00000001
#define TEE_STORAGE_PRIVATE_REE			0x80000000
#define TEE_STORAGE_PRIVATE_RPMB		0x80000100

#define TEE_AES_BLOCK_SIZE				16UL

#ifdef __cplusplus
extern "C" {
#endif

	typedef enum {
		PRIVATE = TEE_STORAGE_PRIVATE,
		REE = TEE_STORAGE_PRIVATE_REE,
		RPMB = TEE_STORAGE_PRIVATE_RPMB,
	} storageId;

	typedef struct _enum_object {
		uint8_t *id;
		uint32_t idSize;
	} eObj;

	typedef struct _enum_object_list {
		eObj *object;
		struct _enum_object_list *next;
	} eObjList;

	typedef void* OperHandle;

	TEEC_Result keyGenerate(oc *o,storageId sid,const char *keyFileName,uint32_t flags,uint32_t keySize,uint32_t *keyObj);
	TEEC_Result keyList(oc *o,storageId sid);
	TEEC_Result keyOpen(oc *o,storageId sid,const char *keyFileName,uint32_t flags,uint32_t *keyObj);
	TEEC_Result keyClose(oc *o,uint32_t keyObj);
	TEEC_Result keyCloseAndDelete(oc *o,uint32_t keyObj);
	TEEC_Result keyGetObjectBufferAttribute(oc *o,uint32_t keyObj,uint32_t attrId,void *buffer,size_t *bufferSize);
	TEEC_Result keyGetObjectValueAttribute(oc *o,uint32_t keyObj,uint32_t attrId,uint32_t *valueA,uint32_t *valueB);

	/*Enum*/
	TEEC_Result keyAllocEnum(oc *o, uint32_t *e);
	TEEC_Result keyResetEnum(oc *o, uint32_t e);
	TEEC_Result keyFreeEnum(oc *o, uint32_t e);
	TEEC_Result keyStartEnum(oc *o, uint32_t e,uint32_t storage_id);
	TEEC_Result keyNextEnum(oc *o, uint32_t e, void *obj_info,
							 size_t *info_size, void *id, uint32_t *id_size);
	TEEC_Result keyEnumObjectList(oc *o,storageId sid,eObjList **list,size_t *listSize);
	size_t keyFreeEnumObjectList(eObjList **list);

	TEEC_Result keyAllocOper(oc *o,uint32_t algo,uint32_t mode,uint32_t keyObj,OperHandle *operHandle);
	TEEC_Result keyFreeOper(oc *o,OperHandle operHandle);
	TEEC_Result keySetKeyOper(oc *o,OperHandle operHandle,uint32_t keyObj);
	TEEC_Result keySetKey2Oper(oc *o,OperHandle operHandle,uint32_t keyObj1,uint32_t keyObj2);
	TEEC_Result keyCipherInit(oc *o,OperHandle operHandle,const void* iv,uint32_t ivLen);
	TEEC_Result keyCipherUpdate(oc *o,OperHandle operHandle,const void *src,size_t srcLen,void *dst,size_t *dstLen);
	TEEC_Result keyCipherDoFinal(oc *o,OperHandle operHandle,const void *src,size_t srcLen,void *dst,size_t *dstLen);
	TEEC_Result keyAEInit(oc *o,OperHandle operHandle,const void* nonce,uint32_t nonceLen,
							uint32_t tagLen,uint32_t aadLen,uint32_t payloadLen);
	TEEC_Result keyAEUpdateAad(oc *o,OperHandle operHandle,const void* aad,uint32_t aadLen);
	TEEC_Result keyAEUpdate(oc *o,OperHandle operHandle,const void *src,size_t srcLen,void *dst,size_t *dstLen);
	TEEC_Result keyAEEncryptFinal(oc *o,OperHandle operHandle,const void *src,size_t srcLen,
							void *dst,size_t *dstLen,void *tag,size_t *tagLen);


#ifdef __cplusplus
}
#endif

#endif
