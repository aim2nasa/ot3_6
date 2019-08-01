#ifndef __KEY_H__
#define __KEY_H__

#include <tee_client_api.h>
#include <session.h>
#include <ta_key.h>
#include <tee_api_types.h>

#define TEE_STORAGE_PRIVATE				0x00000001
#define TEE_STORAGE_PRIVATE_REE			0x80000000
#define TEE_STORAGE_PRIVATE_RPMB		0x80000100

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

	TEEC_Result keyGen(oc *o,storageId sid,const char *keyFileName,uint32_t flags,uint32_t keySize);
	TEEC_Result keyList(oc *o,storageId sid,eObjList **list);
	TEEC_Result keyAllocEnum(oc *o, uint32_t *e);
	TEEC_Result keyResetEnum(oc *o, uint32_t e);
	TEEC_Result keyFreeEnum(oc *o, uint32_t e);
	TEEC_Result keyStartEnum(oc *o, uint32_t e,uint32_t storage_id);
	TEEC_Result keyNextEnum(oc *o, uint32_t e, void *obj_info,
							 size_t *info_size, void *id, uint32_t *id_size);
	TEEC_Result keyEnumObjectList(oc *o,storageId sid,eObjList **list);
	int keyFreeEnumObjectList(eObjList *list);

#ifdef __cplusplus
}
#endif

#endif
