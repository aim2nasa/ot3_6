static TEE_Result ta_key_cmd_alloc_enum(uint32_t param_types, TEE_Param params[4])
{
	TEE_Result res = TEE_ERROR_GENERIC;
	TEE_ObjectEnumHandle oe = TEE_HANDLE_NULL;

	ASSERT_PARAM_TYPE(TEE_PARAM_TYPES
			  (TEE_PARAM_TYPE_VALUE_OUTPUT, TEE_PARAM_TYPE_NONE,
			   TEE_PARAM_TYPE_NONE, TEE_PARAM_TYPE_NONE));

	res = TEE_AllocatePersistentObjectEnumerator(&oe);
	params[0].value.a = (uintptr_t)oe;
	return res;
}

static TEE_Result ta_key_cmd_free_enum(uint32_t param_types, TEE_Param params[4])
{
	TEE_ObjectEnumHandle oe = VAL2HANDLE(params[0].value.a);

	ASSERT_PARAM_TYPE(TEE_PARAM_TYPES
			  (TEE_PARAM_TYPE_VALUE_INPUT, TEE_PARAM_TYPE_NONE,
			   TEE_PARAM_TYPE_NONE, TEE_PARAM_TYPE_NONE));

	TEE_FreePersistentObjectEnumerator(oe);
	return TEE_SUCCESS;
}

static TEE_Result ta_key_cmd_reset_enum(uint32_t param_types, TEE_Param params[4])
{
	TEE_ObjectEnumHandle oe = VAL2HANDLE(params[0].value.a);

	ASSERT_PARAM_TYPE(TEE_PARAM_TYPES
			  (TEE_PARAM_TYPE_VALUE_INPUT, TEE_PARAM_TYPE_NONE,
			   TEE_PARAM_TYPE_NONE, TEE_PARAM_TYPE_NONE));

	TEE_ResetPersistentObjectEnumerator(oe);
	return TEE_SUCCESS;
}

static TEE_Result ta_key_cmd_start_enum(uint32_t param_types, TEE_Param params[4])
{
	TEE_ObjectEnumHandle oe = VAL2HANDLE(params[0].value.a);

	ASSERT_PARAM_TYPE(TEE_PARAM_TYPES
			  (TEE_PARAM_TYPE_VALUE_INPUT, TEE_PARAM_TYPE_NONE,
			   TEE_PARAM_TYPE_NONE, TEE_PARAM_TYPE_NONE));

	return TEE_StartPersistentObjectEnumerator(oe, params[0].value.b);
}

static TEE_Result ta_key_cmd_next_enum(uint32_t param_types, TEE_Param params[4])
{
	TEE_ObjectEnumHandle oe = VAL2HANDLE(params[0].value.a);
	TEE_ObjectInfo *obj = NULL;

	if (TEE_PARAM_TYPE_GET(param_types, 0) != TEE_PARAM_TYPE_VALUE_INPUT)
		return TEE_ERROR_BAD_PARAMETERS;
	if (TEE_PARAM_TYPE_GET(param_types, 2) != TEE_PARAM_TYPE_MEMREF_OUTPUT)
		return TEE_ERROR_BAD_PARAMETERS;
	if (TEE_PARAM_TYPE_GET(param_types, 3) != TEE_PARAM_TYPE_NONE)
		return TEE_ERROR_BAD_PARAMETERS;

	if (TEE_PARAM_TYPE_GET(param_types, 1) == TEE_PARAM_TYPE_NONE)
		obj = NULL;
	else if (TEE_PARAM_TYPE_GET(param_types, 1) ==
		 TEE_PARAM_TYPE_MEMREF_OUTPUT) {
		if (params[1].memref.size < sizeof(TEE_ObjectInfo)) {
			params[1].memref.size = sizeof(TEE_ObjectInfo);
			return TEE_ERROR_SHORT_BUFFER;
		}
		params[1].memref.size = sizeof(TEE_ObjectInfo);
		obj = (TEE_ObjectInfo *)params[1].memref.buffer;
	} else
		return TEE_ERROR_BAD_PARAMETERS;

	if (params[2].memref.size < TEE_OBJECT_ID_MAX_LEN)
		return TEE_ERROR_SHORT_BUFFER;

	return TEE_GetNextPersistentObject(oe, obj,
					   params[2].memref.buffer,
					   &params[2].memref.size);
}
