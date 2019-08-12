static TEE_Result ta_key_cmd_cipher_init(uint32_t param_types, TEE_Param params[4])
{
	ASSERT_PARAM_TYPE(TEE_PARAM_TYPES
			  (TEE_PARAM_TYPE_VALUE_INPUT, TEE_PARAM_TYPE_MEMREF_INPUT,
			   TEE_PARAM_TYPE_NONE, TEE_PARAM_TYPE_NONE));
	
	TEE_CipherInit(VAL2HANDLE(params[0].value.a),params[1].memref.buffer,params[1].memref.size);
	DMSG("CipherInit operHandle=%p,IV Size=%u",VAL2HANDLE(params[0].value.a),params[1].memref.size);
	return TEE_SUCCESS;
}

static TEE_Result ta_key_cmd_cipher_update(uint32_t param_types, TEE_Param params[4])
{
	ASSERT_PARAM_TYPE(TEE_PARAM_TYPES
			  (TEE_PARAM_TYPE_VALUE_INPUT, TEE_PARAM_TYPE_MEMREF_INPUT,
			   TEE_PARAM_TYPE_MEMREF_OUTPUT, TEE_PARAM_TYPE_NONE));

	TEE_OperationHandle op = VAL2HANDLE(params[0].value.a);

	DMSG("CipherUpdate operHandle=%p,input buffer:%u, output buffer:%u",op,
			params[1].memref.size,params[2].memref.size);

	TEE_Result res;
	res = TEE_CipherUpdate(VAL2HANDLE(params[0].value.a),
			       params[1].memref.buffer,params[1].memref.size,
			       params[2].memref.buffer,&params[2].memref.size);
	DMSG("CipherUpdate result=0x%x,Updated output size:%d",res,params[2].memref.size);
	return res;
}

static TEE_Result ta_key_cmd_cipher_do_final(uint32_t param_types, TEE_Param params[4])
{
	ASSERT_PARAM_TYPE(TEE_PARAM_TYPES
			  (TEE_PARAM_TYPE_VALUE_INPUT, TEE_PARAM_TYPE_MEMREF_INPUT,
			   TEE_PARAM_TYPE_MEMREF_OUTPUT, TEE_PARAM_TYPE_NONE));

	TEE_OperationHandle op = VAL2HANDLE(params[0].value.a);

	DMSG("CipheDoFinal operHandle=%p,input buffer:%u, output buffer:%u",op,
			params[1].memref.size,params[2].memref.size);

	TEE_Result res;
	res = TEE_CipherDoFinal(op,
			       params[1].memref.buffer,params[1].memref.size,
			       params[2].memref.buffer,&params[2].memref.size);
	DMSG("CipherDoFinal result=0x%x,Updated output size:%d",res,params[2].memref.size);
	return res;
}
