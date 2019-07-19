#include "session.h"
#include <stdlib.h>

TEEC_Result initializeContext(const char *name,oc *o)
{
	o->ctx = (TEEC_Context*)malloc(sizeof(TEEC_Context));
	return TEEC_InitializeContext(name,o->ctx);
}

TEEC_Result openSession(oc *o,TEEC_UUID *uuid,uint32_t connectionMethod,
			const void *connectionData,TEEC_Operation *operation)
{
	o->session = (TEEC_Session*)malloc(sizeof(TEEC_Session));
	return TEEC_OpenSession(o->ctx,o->session,uuid,connectionMethod,
		connectionData,operation,&o->error);
}

void closeSession(oc *o)
{
	TEEC_CloseSession(o->session);
	free(o->session);
}

void finalizeContext(oc *o)
{
	TEEC_FinalizeContext(o->ctx);
	free(o->ctx);
}
