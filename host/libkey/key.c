#include "key.h"
#include <string.h>
#include <stdlib.h>

TEEC_Result keyGen(oc *o,storageId sid,const char *keyFileName,uint32_t flags,uint32_t keySize)
{
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;

	op.params[0].value.a = sid;
	op.params[1].tmpref.buffer = (char*)keyFileName;
	op.params[1].tmpref.size = strlen((const char*)keyFileName);
	op.params[2].value.a = flags;
	op.params[3].value.a = keySize;
	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,TEEC_MEMREF_TEMP_INPUT,TEEC_VALUE_INPUT,TEEC_VALUE_INPUT);

	return TEEC_InvokeCommand(o->session,TA_KEY_CMD_GENERATE,&op,&o->error);
}

TEEC_Result keyList(oc *o,storageId sid)
{
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;

	op.params[0].value.a = sid;
	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,TEEC_NONE,TEEC_NONE,TEEC_NONE);

	return TEEC_InvokeCommand(o->session,TA_KEY_CMD_LIST,&op,&o->error);
}

TEEC_Result keyAllocEnum(oc *o, uint32_t *e)
{
	TEEC_Result res = TEEC_ERROR_GENERIC;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t org = 0;

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_OUTPUT, TEEC_NONE,
					 TEEC_NONE, TEEC_NONE);

	res = TEEC_InvokeCommand(o->session, TA_KEY_CMD_ALLOC_ENUM, &op, &org);

	if (res == TEEC_SUCCESS)
		*e = op.params[0].value.a;

	return res;
}

TEEC_Result keyResetEnum(oc *o, uint32_t e)
{
	TEEC_Result res = TEEC_ERROR_GENERIC;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t org = 0;

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_NONE,
					 TEEC_NONE, TEEC_NONE);

	op.params[0].value.a = e;
	res = TEEC_InvokeCommand(o->session, TA_KEY_CMD_RESET_ENUM, &op, &org);

	return res;
}

TEEC_Result keyFreeEnum(oc *o, uint32_t e)
{
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t org = 0;

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_NONE, TEEC_NONE,
					 TEEC_NONE);

	op.params[0].value.a = e;

	return TEEC_InvokeCommand(o->session, TA_KEY_CMD_FREE_ENUM, &op, &org);
}

TEEC_Result keyStartEnum(oc *o, uint32_t e,uint32_t storage_id)
{
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t org = 0;

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_NONE,
					 TEEC_NONE, TEEC_NONE);

	op.params[0].value.a = e;
	op.params[0].value.b = storage_id;

	return TEEC_InvokeCommand(o->session, TA_KEY_CMD_START_ENUM, &op, &org);
}

TEEC_Result keyNextEnum(oc *o, uint32_t e, void *obj_info,
						 size_t *info_size, void *id, uint32_t *id_size)
{
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t org = 0;

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_NONE,
					 TEEC_MEMREF_TEMP_OUTPUT, TEEC_NONE);
	if (obj_info && info_size)
		op.paramTypes |= (TEEC_MEMREF_TEMP_OUTPUT << 4);

	op.params[0].value.a = e;
	op.params[1].tmpref.buffer = obj_info;
	op.params[1].tmpref.size = *info_size;
	op.params[2].tmpref.buffer = id;
	op.params[2].tmpref.size = *id_size;

	TEEC_Result res = TEEC_InvokeCommand(o->session, TA_KEY_CMD_NEXT_ENUM, &op, &org);
	if(res==TEEC_SUCCESS){
		*info_size = op.params[1].tmpref.size;
		*id_size = op.params[2].tmpref.size;
	}
	return res;
}

TEEC_Result keyEnumObjectList(oc *o,storageId sid,eObjList **list)
{
	TEEC_Result res;
	uint32_t enumHandle;
	size_t infoSize;
	uint32_t idSize;
	uint8_t info[256];
	uint8_t id[256+1];	//+1 to include null at the end of name
	eObjList *prev = NULL;

	*list = NULL;

	res = keyAllocEnum(o,&enumHandle);
        if(res!=TEEC_SUCCESS) return res;

	res = keyStartEnum(o,enumHandle,sid);
        if(res!=TEEC_SUCCESS) return res;

	memset((void*)info,0,sizeof(info));
	memset((void*)id,0,sizeof(id));
	infoSize = sizeof(info);
	idSize = sizeof(id);

	while(TEEC_SUCCESS==keyNextEnum(o,enumHandle,info,&infoSize,id,&idSize)){
		eObj *obj = NULL;
		eObjList *objectList = (eObjList*)malloc(sizeof(eObjList));

		if(*list==NULL) *list = objectList; 

		obj = (eObj*)malloc(sizeof(eObj));
		obj->idSize = idSize;
		obj->id = (uint8_t*)malloc(idSize);
		memcpy(obj->id,id,idSize);

		objectList->object = obj;
		objectList->next = NULL;
		if(prev) prev->next = objectList;

		prev = objectList;
		infoSize = sizeof(info);
		idSize = sizeof(id);
	}

	res = keyFreeEnum(o,enumHandle);
        if(res!=TEEC_SUCCESS) return res;

	return res;
}

size_t keyFreeEnumObjectList(eObjList *list)
{
	size_t nCount = 0;
	eObjList *cur = list;
	while(cur){
		eObjList *next = cur->next;
		free(cur->object->id);
		free(cur->object);
		free(cur);
		if(next) {
			cur = next;
		}else{
			cur = NULL;
		}
		nCount++;
	}
	return nCount;
}
