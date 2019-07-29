#include "key.h"
#include <string.h>
#include <fs.h>
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

TEEC_Result keyEnumObjectList(oc *o,storageId sid,eObjList **list)
{
	TEEC_Result res;
	uint32_t enumHandle;
	size_t infoSize;
	uint32_t idSize;
	uint8_t info[256];
	uint8_t id[256+1];        //+1 to include null at the end of name

	eObjList *prev = NULL;
	*list = NULL;

	res = fs_alloc_enum(o,&enumHandle);
	if(res!=TEEC_SUCCESS) return res;

	res = fs_start_enum(o,enumHandle,sid);
	if(res!=TEEC_SUCCESS) return res;

	memset((void*)info,0,sizeof(info));
	memset((void*)id,0,sizeof(id));
	infoSize = sizeof(info);
	idSize = sizeof(id);

	while(TEEC_SUCCESS==fs_next_enum(o,enumHandle,info,infoSize,id,idSize)){
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
	res = fs_free_enum(o,enumHandle);
	if(res!=TEEC_SUCCESS) return res;
	return res;
}
