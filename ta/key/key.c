static TEE_Result ta_key_cmd_generate(uint32_t param_types, TEE_Param params[4])
{
	TEE_Result result = TEE_ERROR_GENERIC;
	TEE_ObjectHandle transient_key = (TEE_ObjectHandle)NULL;
	TEE_ObjectHandle persistent_key = (TEE_ObjectHandle)NULL;
	size_t key_size;
	TEE_ObjectInfo keyInfo;
	char *keyFileName = 0;
	size_t keyFileNameSize = 0;
	uint32_t flags;

	(void)params;
	ASSERT_PARAM_TYPE(TEE_PARAM_TYPES
			  (TEE_PARAM_TYPE_VALUE_INPUT,TEE_PARAM_TYPE_MEMREF_INPUT,
			  TEE_PARAM_TYPE_VALUE_INPUT,TEE_PARAM_TYPE_VALUE_INPUT));

	key_size = params[3].value.a;
	DMSG("received key size: %zd bits",key_size);
	if(key_size!=128 && key_size!=192 && key_size!=256) {
		DMSG("key size: %zd bits, it must be one of 128,192,256.Otherwise not supported",key_size);
		result = TEE_ERROR_NOT_SUPPORTED;
		goto out1;
	}

	if((result=TEE_AllocateTransientObject(TEE_TYPE_AES,key_size,&transient_key))!=TEE_SUCCESS){
		EMSG("Failed to Allocate transient object handle : 0x%x",result);
		goto out1;
	}
	DMSG("Allocated TransientObject: %p",(void*)transient_key);

	if((result=TEE_GenerateKey(transient_key,key_size,NULL,0))!=TEE_SUCCESS){
		EMSG("Failed to generate a transient key: 0x%x", result);
		goto out2;
	}
	DMSG("Key(%zd bit) generated with TransientObject(%p)",key_size,(void*)transient_key);

	if((result=TEE_GetObjectInfo1(transient_key, &keyInfo))!=TEE_SUCCESS){
		EMSG("Failed to GetObjectInfo1: 0x%x", result);
		goto out3;
	}
	DMSG("keyInfo: %zd bytes",sizeof(TEE_ObjectInfo));

	DMSG("Input params[0], storage id: %d",params[0].value.a);

	keyFileNameSize = params[1].memref.size;
	keyFileName = TEE_Malloc(keyFileNameSize+1,0);
	if(!keyFileName){
		result = TEE_ERROR_OUT_OF_MEMORY;
		goto out3;
	}
	TEE_MemMove(keyFileName,params[1].memref.buffer,keyFileNameSize);
	keyFileName[keyFileNameSize]=0;
	DMSG("Input params[1], key filename: %s",keyFileName);

	if(keyFileNameSize==0) {
		EMSG("Bad parameter, keyFileNameSize is zero");
		result = TEE_ERROR_BAD_PARAMETERS;
		goto out4;
	}

	flags = params[2].value.a;
	DMSG("Input params[2], flags:0x%x",flags);
	if((result=TEE_CreatePersistentObject(params[0].value.a,
					      keyFileName,keyFileNameSize,
					      flags,transient_key,NULL,0,&persistent_key))!=TEE_SUCCESS){
		EMSG("Failed to create a persistent key: 0x%x", result);
		goto out4;
	}
	DMSG("%s persistent object(%p) flags:0x%x created",keyFileName,(void*)persistent_key,flags);

out4:
	TEE_Free(keyFileName);
out3:
	TEE_CloseObject(persistent_key);
out2:
	TEE_FreeTransientObject(transient_key);
out1:
	return result;
}

static TEE_Result ta_key_cmd_list(uint32_t param_types, TEE_Param params[4])
{
	(void)param_types;
	ASSERT_PARAM_TYPE(TEE_PARAM_TYPES
			  (TEE_PARAM_TYPE_VALUE_INPUT,TEE_PARAM_TYPE_NONE,
			  TEE_PARAM_TYPE_NONE,TEE_PARAM_TYPE_NONE));
	DMSG("has been called");

	TEE_Result res = TEE_ERROR_GENERIC;
	TEE_ObjectEnumHandle oe = TEE_HANDLE_NULL;
	res = TEE_AllocatePersistentObjectEnumerator(&oe);
	DMSG("TEE_AllocatePersistentObjectEnumerator=0x%x,objectEnumHandle=%p",res,oe);
	if(res!=TEE_SUCCESS) goto out1; 

	DMSG("storageID=0x%x",params[0].value.a);
	res = TEE_StartPersistentObjectEnumerator(oe,params[0].value.a);
	DMSG("TEE_StartPersistentObjectEnumerator=0x%x",res);
	if(res!=TEE_SUCCESS) goto out2;

	TEE_ObjectInfo obj;
	uint8_t objectID[TEE_OBJECT_ID_MAX_LEN];
	uint32_t objectIDLen;

	DMSG("TEE_GetNextPersistentObject start");
	while(TEE_SUCCESS==TEE_GetNextPersistentObject(oe,&obj,objectID,&objectIDLen)){
		objectID[objectIDLen]=0;
		DMSG("objectID:%s,objectIDLen=%u",objectID,objectIDLen);
	}
	DMSG("TEE_GetNextPersistentObject end");

out2:
	TEE_FreePersistentObjectEnumerator(oe);
out1:
	return res;
}

static TEE_Result ta_key_cmd_open(uint32_t param_types, TEE_Param params[4])
{
	TEE_Result result = TEE_SUCCESS;
	TEE_ObjectHandle key = (TEE_ObjectHandle)NULL;
	char *keyFileName = 0;
	size_t keyFileNameSize = 0;
	uint32_t flags;

	ASSERT_PARAM_TYPE(TEE_PARAM_TYPES
			  (TEE_PARAM_TYPE_VALUE_INPUT,TEE_PARAM_TYPE_MEMREF_INPUT,
			   TEE_PARAM_TYPE_VALUE_INPUT,TEE_PARAM_TYPE_VALUE_OUTPUT));

	DMSG("Input params[0], storage id: %d",params[0].value.a);

	keyFileNameSize = params[1].memref.size;
	keyFileName = TEE_Malloc(keyFileNameSize+1,0);
	if(!keyFileName){
		result = TEE_ERROR_OUT_OF_MEMORY;
		goto out1;
	}
	TEE_MemMove(keyFileName,params[1].memref.buffer,keyFileNameSize);
	keyFileName[keyFileNameSize]=0;
	DMSG("Input params[1], key filename: %s",keyFileName);

	flags = params[2].value.a;
	DMSG("Input params[2], flags:0x%x",flags);
	if((result=TEE_OpenPersistentObject(params[0].value.a,
					    keyFileName,keyFileNameSize,
					    flags,&key))!=TEE_SUCCESS){
		EMSG("Failed to open a persistent key: 0x%x", result);
		goto out2;
	}
	params[3].value.a = (uintptr_t)key;
	DMSG("%s persistent object(%p) flags:0x%x opened",keyFileName,(void*)key,flags);

out2:
	TEE_Free(keyFileName);
out1:
	return result;
}

static TEE_Result ta_key_cmd_close(uint32_t param_types, TEE_Param params[4])
{
	ASSERT_PARAM_TYPE(TEE_PARAM_TYPES
			  (TEE_PARAM_TYPE_VALUE_INPUT,TEE_PARAM_TYPE_NONE,
			  TEE_PARAM_TYPE_NONE,TEE_PARAM_TYPE_NONE));

	DMSG("closing object(%p)",(void*)(uintptr_t)params[0].value.a);
	TEE_CloseObject((TEE_ObjectHandle)(uintptr_t)params[0].value.a);
	return TEE_SUCCESS;
}
