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
			  TEE_PARAM_TYPE_VALUE_INPUT,TEE_PARAM_TYPE_VALUE_OUTPUT));

	key_size = params[2].value.b;
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
		goto out2;
	}
	DMSG("keyInfo: %zd bytes",sizeof(TEE_ObjectInfo));

	DMSG("Input params[0], storage id: %d",params[0].value.a);

	keyFileNameSize = params[1].memref.size;
	keyFileName = TEE_Malloc(keyFileNameSize+1,0);
	if(!keyFileName){
		result = TEE_ERROR_OUT_OF_MEMORY;
		goto out2;
	}
	TEE_MemMove(keyFileName,params[1].memref.buffer,keyFileNameSize);
	keyFileName[keyFileNameSize]=0;
	DMSG("Input params[1], key filename: %s",keyFileName);

	if(keyFileNameSize==0) {
		EMSG("Bad parameter, keyFileNameSize is zero");
		result = TEE_ERROR_BAD_PARAMETERS;
		goto out3;
	}

	flags = params[2].value.a;
	DMSG("Input params[2], flags:0x%x",flags);
	if((result=TEE_CreatePersistentObject(params[0].value.a,
					      keyFileName,keyFileNameSize,
					      flags,transient_key,NULL,0,&persistent_key))!=TEE_SUCCESS){
		EMSG("Failed to create a persistent key: 0x%x", result);
		goto out3;
	}
	DMSG("%s persistent object(%p) flags:0x%x created",keyFileName,(void*)persistent_key,flags);
	params[3].value.a = (uintptr_t)persistent_key;

out3:
	TEE_Free(keyFileName);
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

static TEE_Result ta_key_cmd_close_and_delete(uint32_t param_types, TEE_Param params[4])
{
	ASSERT_PARAM_TYPE(TEE_PARAM_TYPES
			  (TEE_PARAM_TYPE_VALUE_INPUT,TEE_PARAM_TYPE_NONE,
			  TEE_PARAM_TYPE_NONE,TEE_PARAM_TYPE_NONE));

	DMSG("close and deleting object(%p)",(void*)(uintptr_t)params[0].value.a);
	TEE_CloseAndDeletePersistentObject1((TEE_ObjectHandle)(uintptr_t)params[0].value.a);
	return TEE_SUCCESS;
}

static TEE_Result ta_key_cmd_get_object_buffer_attribute(uint32_t param_types, TEE_Param params[4])
{
	ASSERT_PARAM_TYPE(TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_INPUT,TEE_PARAM_TYPE_MEMREF_OUTPUT,
									  TEE_PARAM_TYPE_NONE,TEE_PARAM_TYPE_NONE));

	TEE_ObjectHandle o = VAL2HANDLE(params[0].value.a);
	DMSG("asking getObjectAttribute objectHandle:%p,attribute:0x%x...",o,params[0].value.b);
	TEE_ObjectInfo oi;
	TEE_Result result = TEE_ERROR_GENERIC;
	result = TEE_GetObjectInfo1(o,&oi);
	DMSG("TEE_GetObjectInfo1=0x%x,objectInfo:0x%x",result,oi.objectUsage);

	DMSG("attributeID:0x%x",params[0].value.b);
	DMSG("TEE_ATTR_BIT_VALUE:0x%x",TEE_ATTR_BIT_VALUE);
	result = TEE_GetObjectBufferAttribute(o,params[0].value.b,params[1].memref.buffer,&params[1].memref.size);
	DMSG("TEE_GetObjectBufferAttribute=0x%x",result);
	return result;
}

static TEE_Result ta_key_cmd_get_object_value_attribute(uint32_t param_types,TEE_Param params[4])
{
	ASSERT_PARAM_TYPE(TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_INPUT,TEE_PARAM_TYPE_VALUE_OUTPUT,
									  TEE_PARAM_TYPE_NONE,TEE_PARAM_TYPE_NONE));

	TEE_ObjectHandle o = VAL2HANDLE(params[0].value.a);
	return TEE_GetObjectValueAttribute(o,params[0].value.b,&params[1].value.a,&params[1].value.b);
}

static TEE_Result algorithm(uint32_t *supAlgo,uint32_t askAlgo)
{
	switch(askAlgo){
	case TEE_ALG_AES_ECB_NOPAD:
		*supAlgo = TEE_ALG_AES_ECB_NOPAD;
		return TEE_SUCCESS;
	default:
		EMSG("Not supported algorhtim %u",askAlgo);
		return TEE_ERROR_BAD_PARAMETERS;
	}
}

static TEE_Result mode(uint32_t *supMode,uint32_t askMode)
{
	switch(askMode){
	case TEE_MODE_ENCRYPT:
		*supMode = TEE_MODE_ENCRYPT;
		return TEE_SUCCESS;
	case TEE_MODE_DECRYPT:
		*supMode = TEE_MODE_DECRYPT;
		return TEE_SUCCESS;
	default:
		EMSG("Not supported mode %u",askMode);
		return TEE_ERROR_BAD_PARAMETERS;
	}
}

#define AES_KEY_SIZE_128BIT	(128/8)
#define AES_KEY_SIZE_192BIT	(192/8)
#define AES_KEY_SIZE_256BIT	(256/8)

static TEE_Result aesKeySizeInByte(uint32_t *supSize,uint32_t askSize)
{
	switch(askSize){
	case AES_KEY_SIZE_128BIT:
		*supSize = AES_KEY_SIZE_128BIT;
		return TEE_SUCCESS;
	case AES_KEY_SIZE_192BIT:
		*supSize = AES_KEY_SIZE_192BIT;
		return TEE_SUCCESS;
	case AES_KEY_SIZE_256BIT:
		*supSize = AES_KEY_SIZE_256BIT;
		return TEE_SUCCESS;
	default:
		EMSG("Not supported size %ubytes",askSize);
		return TEE_ERROR_BAD_PARAMETERS;
	}
}

static TEE_Result aesInit(aesCipher *sess,uint32_t algo,uint32_t mod,
						  char *key,uint32_t keySize,char *iv,uint32_t ivSize)
{
	TEE_Result result = TEE_ERROR_GENERIC;

	if((result=algorithm(&sess->algo,algo))!=TEE_SUCCESS) goto out1;
	if((result=mode(&sess->mode,mod))!=TEE_SUCCESS) goto out1;
	if((result=aesKeySizeInByte(&sess->keySize,keySize))!=TEE_SUCCESS) goto out1;

	result = TEE_AllocateOperation(&sess->operHandle,sess->algo,sess->mode,sess->keySize*8);
	if(result!=TEE_SUCCESS) goto out1;
	DMSG("OperationHandle=%p",sess->operHandle);

	TEE_ObjectHandle keyHandle;	//Transient object to load the key
	result = TEE_AllocateTransientObject(TEE_TYPE_AES,sess->keySize*8,&keyHandle);
	if(result!=TEE_SUCCESS) goto out2;
	DMSG("ObjectHandle=%p",keyHandle);

	TEE_Attribute keyAttr;
	keyAttr.attributeID = TEE_ATTR_SECRET_VALUE;
	keyAttr.content.ref.buffer = (void*)key;
	keyAttr.content.ref.length = keySize;

	result = TEE_PopulateTransientObject(keyHandle,&keyAttr,1);
	if(result!=TEE_SUCCESS) goto out3;
	DMSG("PopulateTransientObject");

	result = TEE_SetOperationKey(sess->operHandle,keyHandle);
	if(result!=TEE_SUCCESS) goto out3;
	DMSG("SetOperationKey");

	TEE_CipherInit(sess->operHandle,iv,ivSize);
	DMSG("CipherInit");

	TEE_FreeTransientObject(keyHandle);
	return result;

out3:
	TEE_FreeTransientObject(keyHandle);
out2:
	TEE_FreeOperation(sess->operHandle);
out1:
	return result;
}

static TEE_Result ta_key_cmd_test(uint32_t param_types, TEE_Param params[4])
{
	ASSERT_PARAM_TYPE(TEE_PARAM_TYPES(TEE_PARAM_TYPE_NONE,TEE_PARAM_TYPE_NONE,
									  TEE_PARAM_TYPE_NONE,TEE_PARAM_TYPE_NONE));
	(void)params;
	DMSG("Start test");
	TEE_Result result = TEE_ERROR_GENERIC;

	char key[32]={0,};
	char iv[16]={1,};
	char plain[1024]={2,};
	char encrypt[1024]={3,};
	uint32_t encryptSize = sizeof(encrypt);

	aesCipher encSess;
	result = aesInit(&encSess,TEE_ALG_AES_ECB_NOPAD,TEE_MODE_ENCRYPT,key,sizeof(key),iv,sizeof(iv));
	if(result!=TEE_SUCCESS) goto out1;
	DMSG("aesInit ok");

	result = TEE_CipherUpdate(encSess.operHandle,plain,sizeof(plain),encrypt,&encryptSize);
	if(result!=TEE_SUCCESS) goto out2;
	DMSG("CipherUpdate outSize=%u",encryptSize);
	DMSG("encoding ok");

	if(memcmp(plain,encrypt,sizeof(plain))==0)
		DMSG("After encoding plain==encrypt");
	else
		DMSG("After encoding plain!=encrypt");

	aesCipher decSess;
	result = aesInit(&decSess,TEE_ALG_AES_ECB_NOPAD,TEE_MODE_DECRYPT,key,sizeof(key),iv,sizeof(iv));
	if(result!=TEE_SUCCESS) goto out2;
	DMSG("aesInit ok");

	char decrypt[1024]={4,};
	uint32_t decryptSize = sizeof(decrypt);
	result = TEE_CipherUpdate(decSess.operHandle,encrypt,sizeof(encrypt),decrypt,&decryptSize);
	if(result!=TEE_SUCCESS) goto out3;
	DMSG("CipherUpdate outSize=%u",decryptSize);
	DMSG("decoding ok");

	if(memcmp(decrypt,encrypt,sizeof(decrypt))==0)
		DMSG("After decoding decrypt==encrypt");
	else
		DMSG("After decoding decrypt!=encrypt");

	if(memcmp(plain,decrypt,sizeof(plain))==0)
		DMSG("compare plain==decrypt");
	else
		DMSG("compare plain!=decrypt");

out3:
	TEE_FreeOperation(decSess.operHandle);
out2:
	TEE_FreeOperation(encSess.operHandle);
out1:
	DMSG("End test");
	return result;
}

static TEE_Result allocOperation(aesCipher *sess,uint32_t algo,uint32_t mod,uint32_t keySize)
{
	TEE_Result result = TEE_ERROR_GENERIC;

	if(sess->operHandle!=TEE_HANDLE_NULL) TEE_FreeOperation(sess->operHandle);

	if((result=algorithm(&sess->algo,algo))!=TEE_SUCCESS) goto out1;
	if((result=mode(&sess->mode,mod))!=TEE_SUCCESS) goto out1;
	if((result=aesKeySizeInByte(&sess->keySize,keySize))!=TEE_SUCCESS) goto out1;

	result = TEE_AllocateOperation(&sess->operHandle,sess->algo,sess->mode,sess->keySize*8);
	if(result!=TEE_SUCCESS) goto out1;
	DMSG("aesCipher OperationHandle=%p",aesSess.operHandle);

out1:
	return result;
}

static TEE_Result ta_key_cmd_alloc_operation(uint32_t param_types, TEE_Param params[4])
{
	ASSERT_PARAM_TYPE(TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_INPUT,TEE_PARAM_TYPE_VALUE_INPUT,
									  TEE_PARAM_TYPE_NONE,TEE_PARAM_TYPE_NONE));

	return allocOperation(&aesSess,params[0].value.a,params[0].value.b,params[1].value.a);
}

static TEE_Result deallocOperation(aesCipher *sess)
{
	TEE_FreeOperation(sess->operHandle);
	sess->operHandle = TEE_HANDLE_NULL;
	return TEE_SUCCESS;
}

static TEE_Result ta_key_cmd_dealloc_operation(uint32_t param_types, TEE_Param params[4])
{
	ASSERT_PARAM_TYPE(TEE_PARAM_TYPES(TEE_PARAM_TYPE_NONE,TEE_PARAM_TYPE_NONE,
									  TEE_PARAM_TYPE_NONE,TEE_PARAM_TYPE_NONE));
	(void)params;
	return deallocOperation(&aesSess);
}

static TEE_Result setKey(aesCipher *sess,char *key,uint32_t keySize)
{
	TEE_Result result = TEE_ERROR_GENERIC;

	DMSG("setKey start");
	if(sess->keySize!=keySize) {
		result = TEE_ERROR_BAD_PARAMETERS;
		goto out1;
	} 

	TEE_ObjectHandle keyHandle;
	result = TEE_AllocateTransientObject(TEE_TYPE_AES,sess->keySize*8,&keyHandle);
	if(result!=TEE_SUCCESS) goto out1;
	DMSG("Allocate Key ObjectHandle=%p",keyHandle);

	TEE_Attribute keyAttr;
	keyAttr.attributeID = TEE_ATTR_SECRET_VALUE;
	keyAttr.content.ref.buffer = (void*)key;
	keyAttr.content.ref.length = keySize;

	result = TEE_PopulateTransientObject(keyHandle,&keyAttr,1);
	if(result!=TEE_SUCCESS) goto out2;
	DMSG("PopulateTransientObject key objectHandle=%p with keyAttribute=0x%x",keyHandle,keyAttr.attributeID);

	result = TEE_SetOperationKey(sess->operHandle,keyHandle);
	if(result!=TEE_SUCCESS) goto out2;
	DMSG("SetOperationKey operationHandle=%p, key ObjectHandle=%p",sess->operHandle,keyHandle);

out2:
	TEE_FreeTransientObject(keyHandle);
	DMSG("Free Key ObjectHandle=%p",keyHandle);
out1:
	DMSG("setKey end with error code=0x%x",result);
	return result;
}

static TEE_Result ta_key_cmd_set_key(uint32_t param_types, TEE_Param params[4])
{
	ASSERT_PARAM_TYPE(TEE_PARAM_TYPES(TEE_PARAM_TYPE_MEMREF_INPUT,TEE_PARAM_TYPE_NONE,
									  TEE_PARAM_TYPE_NONE,TEE_PARAM_TYPE_NONE));

	return setKey(&aesSess,params[0].memref.buffer,params[0].memref.size);
}

static TEE_Result setIV(aesCipher *sess,char *iv,uint32_t ivSize)
{
	TEE_CipherInit(sess->operHandle,iv,ivSize);
	return TEE_SUCCESS;
}

static TEE_Result ta_key_cmd_set_iv(uint32_t param_types, TEE_Param params[4])
{
	ASSERT_PARAM_TYPE(TEE_PARAM_TYPES(TEE_PARAM_TYPE_MEMREF_INPUT,TEE_PARAM_TYPE_NONE,
									  TEE_PARAM_TYPE_NONE,TEE_PARAM_TYPE_NONE));

	return setIV(&aesSess,params[0].memref.buffer,params[0].memref.size);
}
