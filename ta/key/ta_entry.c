#include <tee_ta_api.h>
#include <ta_key.h>
#include <common.h>
#include <trace.h>
#include <tee_api.h>
#include <stdlib.h>
#include <string.h>
#include <ta_storage.h>

#include "key.c"
#include "enum.c"

static TEE_UUID stor_uuid = TA_STORAGE_UUID;
static TEE_TASessionHandle stor_session = TEE_HANDLE_NULL;

/*
 * Trusted Application Entry Points
 */

/* Called each time a new instance is created */
TEE_Result TA_CreateEntryPoint(void)
{
	DMSG("has been called");
	return TEE_SUCCESS;
}

/* Called each time an instance is destroyed */
void TA_DestroyEntryPoint(void)
{
	DMSG("has been called");
}

/* Called each time a session is opened */
TEE_Result TA_OpenSessionEntryPoint(uint32_t nParamTypes,
				    TEE_Param pParams[4],
				    void **ppSessionContext)
{
	TEE_Result res = TEE_ERROR_GENERIC;
	TEE_Param params[4]={};
	uint32_t origin = 0;
	uint32_t types = TEE_PARAM_TYPES(TEE_PARAM_TYPE_NONE,TEE_PARAM_TYPE_NONE,
									 TEE_PARAM_TYPE_NONE,TEE_PARAM_TYPE_NONE);

	(void)nParamTypes;
	(void)pParams;
	(void)ppSessionContext;

	res = TEE_OpenTASession(&stor_uuid,0,types,params,&stor_session,&origin);
	DMSG("TEE_OpenTASession=0x%x",res);
	DMSG("has been called");
	return TEE_SUCCESS;
}

/* Called each time a session is closed */
void TA_CloseSessionEntryPoint(void *pSessionContext)
{
	(void)pSessionContext;
	TEE_CloseTASession(stor_session);
	DMSG("has been called");
}

/* Called when a command is invoked */
TEE_Result TA_InvokeCommandEntryPoint(void *pSessionContext,
				      uint32_t nCommandID, uint32_t nParamTypes,
				      TEE_Param pParams[4])
{
	(void)pSessionContext;

	switch (nCommandID) {
	case TA_KEY_CMD_GENERATE:
		IMSG("TA_KEY_CMD_GENERATE command");
		return ta_key_cmd_generate(nParamTypes, pParams);
	case TA_KEY_CMD_LIST:
		IMSG("TA_KEY_CMD_LIST command");
		return ta_key_cmd_list(nParamTypes, pParams);
	case TA_KEY_CMD_ALLOC_ENUM:
		IMSG("TA_KEY_CMD_ALLOC_ENUM command");
		return ta_key_cmd_alloc_enum(nParamTypes, pParams);
	case TA_KEY_CMD_FREE_ENUM:
		IMSG("TA_KEY_CMD_FREE_ENUM command");
		return ta_key_cmd_free_enum(nParamTypes, pParams);
	case TA_KEY_CMD_RESET_ENUM:
		IMSG("TA_KEY_CMD_RESET_ENUM command");
		return ta_key_cmd_reset_enum(nParamTypes, pParams);
	case TA_KEY_CMD_START_ENUM:
		IMSG("TA_KEY_CMD_START_ENUM command");
		return ta_key_cmd_start_enum(nParamTypes, pParams);
	case TA_KEY_CMD_NEXT_ENUM:
		IMSG("TA_KEY_CMD_NEXT_ENUM command");
		return ta_key_cmd_next_enum(nParamTypes, pParams);
	default:
		IMSG("TEE_ERROR_BAD_PARAMETERS");
		return TEE_ERROR_BAD_PARAMETERS;
	}
}
