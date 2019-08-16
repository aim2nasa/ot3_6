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

static TEE_Result ta_key_cmd_ae_init(uint32_t param_types, TEE_Param params[4])
{
	ASSERT_PARAM_TYPE(TEE_PARAM_TYPES
			  (TEE_PARAM_TYPE_VALUE_INPUT, TEE_PARAM_TYPE_MEMREF_INPUT,
			   TEE_PARAM_TYPE_VALUE_INPUT, TEE_PARAM_TYPE_NONE));

	TEE_Result res;
	TEE_OperationHandle op = VAL2HANDLE(params[0].value.a);

	DMSG("AEInit operHandle=%p,nonceLen=%u tagLen(bit)=%u AADLen=%u payloadLen=%u",
		op,params[1].memref.size,params[0].value.b*8,params[2].value.a,params[2].value.b);
	res = TEE_AEInit(op,
					 params[1].memref.buffer,params[1].memref.size,
					 params[0].value.b*8,	//tag length in bits
					 params[2].value.a,		//AAD length
					 params[2].value.b);	//payload length
	DMSG("AEInit result=0x%x",res);
	return res;
}

static TEE_Result ta_key_cmd_ae_update_aad(uint32_t param_types, TEE_Param params[4])
{
	TEE_OperationHandle op = VAL2HANDLE(params[0].value.a);

	ASSERT_PARAM_TYPE(TEE_PARAM_TYPES
			  (TEE_PARAM_TYPE_VALUE_INPUT,
			   TEE_PARAM_TYPE_MEMREF_INPUT, TEE_PARAM_TYPE_NONE,
			   TEE_PARAM_TYPE_NONE));

	TEE_AEUpdateAAD(op, params[1].memref.buffer, params[1].memref.size);
	DMSG("AEUpdateAAD operHandle=%p,AADLen=%u",op,params[1].memref.size);
	return TEE_SUCCESS;
}

static TEE_Result ta_key_cmd_ae_update(uint32_t param_types, TEE_Param params[4])
{
	ASSERT_PARAM_TYPE(TEE_PARAM_TYPES
			  (TEE_PARAM_TYPE_VALUE_INPUT,
			   TEE_PARAM_TYPE_MEMREF_INPUT,
			   TEE_PARAM_TYPE_MEMREF_OUTPUT, TEE_PARAM_TYPE_NONE));

	TEE_Result res;
	TEE_OperationHandle op = VAL2HANDLE(params[0].value.a);

	DMSG("AEUpdate operHandle=%p,srcLen=%u destLen=%u",
		op,params[1].memref.size,params[2].memref.size);
	res = TEE_AEUpdate(op, params[1].memref.buffer, params[1].memref.size,
					   params[2].memref.buffer, &params[2].memref.size);
	DMSG("AEUpdate result=0x%x, updated destLen=%u",res,params[2].memref.size);
	return res;
}

static TEE_Result ta_key_cmd_ae_encrypt_final(uint32_t param_types, TEE_Param params[4])
{
	ASSERT_PARAM_TYPE(TEE_PARAM_TYPES
			  (TEE_PARAM_TYPE_VALUE_INPUT,
			   TEE_PARAM_TYPE_MEMREF_INPUT,
			   TEE_PARAM_TYPE_MEMREF_OUTPUT,
			   TEE_PARAM_TYPE_MEMREF_OUTPUT));

	TEE_OperationHandle op = VAL2HANDLE(params[0].value.a);
	TEE_Result res = TEE_ERROR_GENERIC;

	DMSG("AEEncryptFinal operHandle=%p,srcLen=%u destLen=%u tagLen=%u",
		op,params[1].memref.size,params[2].memref.size,params[3].memref.size);
	res = TEE_AEEncryptFinal(op,
			params[1].memref.buffer, params[1].memref.size,
			params[2].memref.buffer, &params[2].memref.size,
			params[3].memref.buffer, &params[3].memref.size);
	DMSG("AEEncryptFinal result=0x%x, updated destLen=%u,tagLen=%u",
		res,params[2].memref.size,params[3].memref.size);
	return res;
}

static TEE_Result ta_key_cmd_ae_decrypt_final(uint32_t param_types, TEE_Param params[4])
{
	ASSERT_PARAM_TYPE(TEE_PARAM_TYPES
			  (TEE_PARAM_TYPE_VALUE_INPUT,
			   TEE_PARAM_TYPE_MEMREF_INPUT,
			   TEE_PARAM_TYPE_MEMREF_OUTPUT,
			   TEE_PARAM_TYPE_MEMREF_INPUT));

	TEE_OperationHandle op = VAL2HANDLE(params[0].value.a);
	TEE_Result res = TEE_ERROR_GENERIC;

	DMSG("AEDecryptFinal operHandle=%p,srcLen=%u destLen=%u tagLen=%u",
		op,params[1].memref.size,params[2].memref.size,params[3].memref.size);
	res = TEE_AEDecryptFinal(op,
			params[1].memref.buffer, params[1].memref.size,
			params[2].memref.buffer, &params[2].memref.size,
			params[3].memref.buffer, params[3].memref.size);
	DMSG("AEDecryptFinal result=0x%x, updated destLen=%u",res,params[2].memref.size);
	return res;
}
