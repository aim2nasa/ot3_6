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
#include "cipher.c"

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
	case TA_KEY_CMD_OPEN:
		IMSG("TA_KEY_CMD_OPEN command");
		return ta_key_cmd_open(nParamTypes, pParams);
	case TA_KEY_CMD_CLOSE:
		IMSG("TA_KEY_CMD_CLOSE command");
		return ta_key_cmd_close(nParamTypes, pParams);
	case TA_KEY_CMD_CLOSE_AND_DELETE:
		IMSG("TA_KEY_CMD_CLOSE_AND_DELETE command");
		return ta_key_cmd_close_and_delete(nParamTypes, pParams);
	case TA_KEY_CMD_GET_OBJECT_BUFFER_ATTRIBUTE:
		IMSG("TA_KEY_CMD_GET_OBJECT_BUFFER_ATTRIBUTE command");
		return ta_key_cmd_get_object_buffer_attribute(nParamTypes, pParams);
	case TA_KEY_CMD_GET_OBJECT_VALUE_ATTRIBUTE:
		IMSG("TA_KEY_CMD_GET_OBJECT_VALUE_ATTRIBUTE command");
		return ta_key_cmd_get_object_value_attribute(nParamTypes, pParams);
	case TA_KEY_CMD_ALLOC_OPER:
		IMSG("TA_KEY_CMD_ALLOC_OPER command");
		return ta_key_cmd_alloc_oper(nParamTypes, pParams);
	case TA_KEY_CMD_FREE_OPER:
		IMSG("TA_KEY_CMD_FREE_OPER command");
		return ta_key_cmd_free_oper(nParamTypes, pParams);
	case TA_KEY_CMD_SET_KEY_OPER:
		IMSG("TA_KEY_CMD_SET_KEY_OPER command");
		return ta_key_cmd_set_key_oper(nParamTypes, pParams);
	case TA_KEY_CMD_CIPHER_INIT:
		IMSG("TA_KEY_CMD_CIPHER_INIT command");
		return ta_key_cmd_cipher_init(nParamTypes, pParams);
	case TA_KEY_CMD_CIPHER_UPDATE:
		IMSG("TA_KEY_CMD_CIPHER_UPDATE command");
		return ta_key_cmd_cipher_update(nParamTypes, pParams);
	default:
		IMSG("TEE_ERROR_NOT_SUPPORTED");
		return TEE_ERROR_NOT_SUPPORTED;
	}
}
