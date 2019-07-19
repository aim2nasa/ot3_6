#ifndef __SESSION_BASE_H__
#define __SESSION_BASE_H__

#include <tee_client_api.h>

#define TEEC_OPERATION_INITIALIZER {}

typedef struct _opteeContext {
	TEEC_Context *ctx;
	TEEC_Session *session;
	uint32_t error;
} oc;

#ifdef __cplusplus
extern "C" {
#endif

	TEEC_Result initializeContext(const char *name,oc *o);
	TEEC_Result openSession(oc *o,TEEC_UUID *uuid,uint32_t connectionMethod,
						const void *connectionData,TEEC_Operation *operation);
	void closeSession(oc *o);
	void finalizeContext(oc *o);

#ifdef __cplusplus
}
#endif

#endif
