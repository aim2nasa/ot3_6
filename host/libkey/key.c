#include "key.h"
#include <string.h>

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

TEEC_Result keyList(oc *o,storageId sid,eObjList **list)
{
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;

	op.params[0].value.a = sid;
	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,TEEC_NONE,TEEC_NONE,TEEC_NONE);

	return TEEC_InvokeCommand(o->session,TA_KEY_CMD_LIST,&op,&o->error);
}
