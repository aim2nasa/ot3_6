static TEE_Result ta_key_cmd_cipher_init(uint32_t param_types, TEE_Param params[4])
{
	ASSERT_PARAM_TYPE(TEE_PARAM_TYPES
			  (TEE_PARAM_TYPE_VALUE_INPUT, TEE_PARAM_TYPE_MEMREF_INPUT,
			   TEE_PARAM_TYPE_NONE, TEE_PARAM_TYPE_NONE));
	
	TEE_CipherInit(VAL2HANDLE(params[0].value.a),params[1].memref.buffer,params[1].memref.size);
	DMSG("CipherInit operHandle=%p,IV Size=%u",VAL2HANDLE(params[0].value.a),params[1].memref.size);
	return TEE_SUCCESS;
}
