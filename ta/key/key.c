static TEE_Result ta_key_cmd_generate(uint32_t param_types, TEE_Param params[4])
{
	TEE_Result result = TEE_SUCCESS;
	TEE_ObjectHandle transient_key = (TEE_ObjectHandle)NULL;
	TEE_ObjectHandle persistent_key = (TEE_ObjectHandle)NULL;
	size_t key_size;
	TEE_ObjectInfo keyInfo;
	char *keyFileName = 0;
	uint32_t flags;
	char *objectId = 0;
	size_t objectIdSize;

	(void)params;
	ASSERT_PARAM_TYPE(TEE_PARAM_TYPES
			  (TEE_PARAM_TYPE_VALUE_INPUT,TEE_PARAM_TYPE_MEMREF_INPUT,
			  TEE_PARAM_TYPE_VALUE_INPUT,TEE_PARAM_TYPE_VALUE_INPUT));

	key_size = params[3].value.a;
	DMSG("received key size: %zd bits",key_size);
	if(key_size!=128 && key_size!=192 && key_size!=256) {
		DMSG("key size: %zd bits, it must be one of 128,192,256.Otherwise not supported",key_size);
		return TEE_ERROR_NOT_SUPPORTED;
	}

	if((result=TEE_AllocateTransientObject(TEE_TYPE_AES,key_size,&transient_key))!=TEE_SUCCESS){
		EMSG("Failed to Allocate transient object handle : 0x%x",result);
		goto cleanup1;
	}
	DMSG("Allocated TransientObject: %p",(void*)transient_key);


	if((result=TEE_GenerateKey(transient_key,key_size,NULL,0))!=TEE_SUCCESS){
		EMSG("Failed to generate a transient key: 0x%x", result);
		goto cleanup2;
	}
	DMSG("Key(%zd bit) generated with TransientObject(%p)",key_size,(void*)transient_key);

	TEE_GetObjectInfo1(transient_key, &keyInfo);
	DMSG("keyInfo: %zd bytes",sizeof(TEE_ObjectInfo));

	DMSG("Input params[0], storage id: %d",params[0].value.a);

	objectIdSize = params[1].memref.size;
	objectId = TEE_Malloc(objectIdSize,0);
	if(!objectId)
		return TEE_ERROR_OUT_OF_MEMORY;

	if(objectIdSize>0) {
		TEE_MemMove(objectId,params[1].memref.buffer,objectIdSize);

		keyFileName = malloc(objectIdSize+1);
		memcpy(keyFileName,params[1].memref.buffer,objectIdSize);
		keyFileName[objectIdSize]=0;
		DMSG("Input params[1], key filename: %s",keyFileName);

		flags = params[2].value.a;
		DMSG("Input params[2], flags:0x%x",flags);
		if((result=TEE_CreatePersistentObject(params[0].value.a,
						      objectId,objectIdSize,
						      flags,transient_key,NULL,0,&persistent_key))!=TEE_SUCCESS){
			EMSG("Failed to create a persistent key: 0x%x", result);
			goto cleanup2;
		}
		DMSG("%s persistent object(%p) flags:0x%x created",keyFileName,(void*)persistent_key,flags);

		TEE_CloseObject(persistent_key);
	}else{
		DMSG("persistent object can't be created, no object name specified");
	}
cleanup2:
	TEE_FreeTransientObject(transient_key);
cleanup1:
	free(keyFileName);
	TEE_Free(objectId);
	return TEE_SUCCESS;
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
	if(res!=TEE_SUCCESS) return res;

	DMSG("storageID=0x%x",params[0].value.a);
	res = TEE_StartPersistentObjectEnumerator(oe,params[0].value.a);
	DMSG("TEE_StartPersistentObjectEnumerator=0x%x",res);
	if(res!=TEE_SUCCESS) return res;

	TEE_ObjectInfo obj;
	uint8_t objectID[TEE_OBJECT_ID_MAX_LEN];
	uint32_t objectIDLen;

	DMSG("TEE_GetNextPersistentObject start");
	while(TEE_SUCCESS==TEE_GetNextPersistentObject(oe,&obj,objectID,&objectIDLen)){
		objectID[objectIDLen]=0;
		DMSG("objectID:%s,objectIDLen=%u",objectID,objectIDLen);
	}
	DMSG("TEE_GetNextPersistentObject end");
	TEE_FreePersistentObjectEnumerator(oe);
	return TEE_SUCCESS;
}