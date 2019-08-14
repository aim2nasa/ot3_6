#include <gtest/gtest.h>
#include <session.h>
#include <key.h>
 
TEST(Key, generation) { 
	oc o;
	TEEC_UUID uuid = TA_KEY_UUID; 
	ASSERT_EQ(initializeContext(NULL,&o),TEEC_SUCCESS);
	ASSERT_EQ(openSession(&o,&uuid,TEEC_LOGIN_PUBLIC,NULL,NULL),TEEC_SUCCESS);

	uint32_t flags = TEE_DATA_FLAG_ACCESS_WRITE_META	|
					 TEE_DATA_FLAG_ACCESS_READ			|
					 TEE_DATA_FLAG_SHARE_READ			|
					 TEE_DATA_FLAG_ACCESS_WRITE			|
					 TEE_DATA_FLAG_SHARE_WRITE;

	uint32_t keyObj = 0;
	ASSERT_EQ(keyGenerate(&o,PRIVATE,"testKey",flags,256,&keyObj),TEEC_SUCCESS);
	ASSERT_NE(keyObj,0);
	ASSERT_EQ(keyCloseAndDelete(&o,keyObj),TEEC_SUCCESS);

	closeSession(&o);
	finalizeContext(&o);
}

TEST(Key, list) { 
	oc o;
	TEEC_UUID uuid = TA_KEY_UUID; 
	ASSERT_EQ(initializeContext(NULL,&o),TEEC_SUCCESS);
	ASSERT_EQ(openSession(&o,&uuid,TEEC_LOGIN_PUBLIC,NULL,NULL),TEEC_SUCCESS);

	ASSERT_EQ(keyList(&o,PRIVATE),TEEC_ERROR_ITEM_NOT_FOUND);

	closeSession(&o);
	finalizeContext(&o);
}

void display(eObjList *list)
{
	int i=0;
	char id[TEE_OBJECT_ID_MAX_LEN+1]={0};
	eObjList *cur=list;

	while(cur){
		memcpy(id,cur->object->id,cur->object->idSize);
		id[cur->object->idSize]=0;

		#ifdef _DEBUG_TEST_
		std::cout<<"["<<i++<<"] "<<id<<std::endl;
		#endif

		if(cur->next)
			cur = cur->next;
		else
			cur = NULL;
	}
}

TEST(Key, eObjList) { 
	oc o;
	TEEC_UUID uuid = TA_KEY_UUID; 
	ASSERT_EQ(initializeContext(NULL,&o),TEEC_SUCCESS);
	ASSERT_EQ(openSession(&o,&uuid,TEEC_LOGIN_PUBLIC,NULL,NULL),TEEC_SUCCESS);

	uint32_t flags = TEE_DATA_FLAG_ACCESS_WRITE_META	|
					 TEE_DATA_FLAG_ACCESS_READ			|
					 TEE_DATA_FLAG_SHARE_READ			|
					 TEE_DATA_FLAG_ACCESS_WRITE			|
					 TEE_DATA_FLAG_SHARE_WRITE;
	uint32_t keyObj = 0;
	ASSERT_EQ(keyGenerate(&o,PRIVATE,"eObjListTestKey",flags,256,&keyObj),TEEC_SUCCESS);

	eObjList *list = NULL;
	size_t listSize=0;
	ASSERT_EQ(keyEnumObjectList(&o,PRIVATE,&list,&listSize),TEEC_SUCCESS);
	display(list);

	ASSERT_EQ(listSize,1);
	ASSERT_EQ(listSize,keyFreeEnumObjectList(&list));
	ASSERT_EQ(list,(eObjList*)NULL);

	ASSERT_NE(keyObj,0);
	ASSERT_EQ(keyCloseAndDelete(&o,keyObj),TEEC_SUCCESS);

	closeSession(&o);
	finalizeContext(&o);
}

TEST(Key, openClose) {
	oc o;
	TEEC_UUID uuid = TA_KEY_UUID;
	ASSERT_EQ(initializeContext(NULL,&o),TEEC_SUCCESS);
	ASSERT_EQ(openSession(&o,&uuid,TEEC_LOGIN_PUBLIC,NULL,NULL),TEEC_SUCCESS);

	uint32_t flags = TEE_DATA_FLAG_ACCESS_WRITE_META	|
					 TEE_DATA_FLAG_ACCESS_READ			|
					 TEE_DATA_FLAG_SHARE_READ			|
					 TEE_DATA_FLAG_ACCESS_WRITE			|
					 TEE_DATA_FLAG_SHARE_WRITE;

	uint32_t keyObj1 = 0;
	ASSERT_EQ(keyGenerate(&o,PRIVATE,"openTest",flags,256,&keyObj1),TEEC_SUCCESS);
	ASSERT_NE(keyObj1,0);
	ASSERT_EQ(keyClose(&o,keyObj1),TEEC_SUCCESS);

	uint32_t keyObj2 = 0;
	ASSERT_EQ(keyOpen(&o,PRIVATE,"openTest",flags,&keyObj2),TEEC_SUCCESS);
	ASSERT_NE(keyObj2,0);
	ASSERT_EQ(keyCloseAndDelete(&o,keyObj2),TEEC_SUCCESS);

	closeSession(&o);
	finalizeContext(&o);
}

TEST(Key, getObjectBufferAttribute) {
	oc o;
	TEEC_UUID uuid = TA_KEY_UUID;
	ASSERT_EQ(initializeContext(NULL,&o),TEEC_SUCCESS);
	ASSERT_EQ(openSession(&o,&uuid,TEEC_LOGIN_PUBLIC,NULL,NULL),TEEC_SUCCESS);

	uint32_t flags = TEE_DATA_FLAG_ACCESS_WRITE_META	|
					 TEE_DATA_FLAG_ACCESS_READ			|
					 TEE_DATA_FLAG_SHARE_READ			|
					 TEE_DATA_FLAG_ACCESS_WRITE			|
					 TEE_DATA_FLAG_SHARE_WRITE;

	uint32_t keyObj = 0;
	ASSERT_EQ(keyGenerate(&o,PRIVATE,"getObjBufAttrTest",flags,256,&keyObj),TEEC_SUCCESS);
	ASSERT_NE(keyObj,0);

	uint8_t keyBuffer[1024] = {0};
	size_t keySize = sizeof(keyBuffer);
	ASSERT_EQ(keyGetObjectBufferAttribute(&o,keyObj,TEE_ATTR_SECRET_VALUE,keyBuffer,&keySize),TEEC_SUCCESS);

	ASSERT_EQ(keySize*8,256);
	#ifdef _DEBUG_TEST_
	for(size_t i=0;i<keySize;i++) std::cout<<std::hex<<(int)keyBuffer[i]<<" ";
	std::cout<<std::endl;
	#endif

	ASSERT_EQ(keyCloseAndDelete(&o,keyObj),TEEC_SUCCESS);

	closeSession(&o);
	finalizeContext(&o);
}

void cipherTest(oc *o,uint32_t algo,uint32_t mode,uint32_t keyObj,const void *iv,size_t ivLen,
				const void *inpBuf,size_t inpBufLen,void *outBuf,size_t outBufLen)
{
	OperHandle operHandle = TEE_HANDLE_NULL;
	ASSERT_EQ(keyAllocOper(o,algo,mode,keyObj,&operHandle),TEEC_SUCCESS);
	ASSERT_NE(operHandle,(void*)TEE_HANDLE_NULL);
	ASSERT_EQ(keySetKeyOper(o,operHandle,keyObj),TEEC_SUCCESS);

	ASSERT_EQ(keyCipherInit(o,operHandle,iv,ivLen),TEEC_SUCCESS);

	size_t outSize = outBufLen;
	int nI = 0;	//input buffer offset
	int nO = 0; //output buffer offset
	ASSERT_EQ(outSize,outBufLen);
	ASSERT_GE(outSize,16);	//To avoid TEE_ERROR_SHORT_BUFFER
	ASSERT_EQ(keyCipherUpdate(o,operHandle,(char*)inpBuf+nI,10,(char*)outBuf+nO,&outSize),TEEC_SUCCESS);
	if(algo==TEE_ALG_AES_CTR){
		ASSERT_EQ(outSize,10);
	}else{
		ASSERT_EQ(outSize,0);	//accumulated 10, less than 16
	}
	nO+= outSize;

	nI += 10;	//previous input buffer size
	outSize = outBufLen;
	ASSERT_GE(outSize,16);	//To avoid TEE_ERROR_SHORT_BUFFER
	ASSERT_EQ(keyCipherUpdate(o,operHandle,(char*)inpBuf+nI,10,(char*)outBuf+nO,&outSize),TEEC_SUCCESS);
	if(algo==TEE_ALG_AES_CTR){
		ASSERT_EQ(outSize,10);
	}else{
		ASSERT_EQ(outSize,16);	//accumuated 10+10-16=4
	}
	nO+=outSize;

	nI += 10;	//previous input buffer size
	outSize = outBufLen-16;//16 bytes written so far
	ASSERT_GE(outSize,16);	//To avoid TEE_ERROR_SHORT_BUFFER
	ASSERT_EQ(keyCipherUpdate(o,operHandle,(char*)inpBuf+nI,10,(char*)outBuf+nO,&outSize),TEEC_SUCCESS);
	if(algo==TEE_ALG_AES_CTR){
		ASSERT_EQ(outSize,10);
	}else{
		ASSERT_EQ(outSize,0);	//accumulated 4+10=14, less then 16
	}
	nO+=outSize;

	nI += 10;	//previous input buffer size
	outSize = outBufLen-16;//16 bytes written so far
	ASSERT_GE(outSize,16);	//To avoid TEE_ERROR_SHORT_BUFFER
	ASSERT_EQ(keyCipherUpdate(o,operHandle,(char*)inpBuf+nI,10,(char*)outBuf+nO,&outSize),TEEC_SUCCESS);
	if(algo==TEE_ALG_AES_CTR){
		ASSERT_EQ(outSize,10);
	}else{
		ASSERT_EQ(outSize,16);	//accumulated 14+10-16=8
	}
	nO+=outSize;

	nI += 10;	//previous input buffer size
	outSize = outBufLen-32;//32 bytes written so far
	ASSERT_GE(outSize,16);	//To avoid TEE_ERROR_SHORT_BUFFER
	ASSERT_EQ(keyCipherDoFinal(o,operHandle,(char*)inpBuf+nI,8,(char*)outBuf+nO,&outSize),TEEC_SUCCESS);
	if(algo==TEE_ALG_AES_CTR){
		ASSERT_EQ(outSize,8);
	}else{
		ASSERT_EQ(outSize,16);	//accumulated 8+8=16-16=0
	}
	nO+=outSize;

	ASSERT_EQ(nO,16*3);

	nI += 8;	//previous input buffer size
	ASSERT_EQ(nI,16*3);

	ASSERT_EQ(keyFreeOper(o,operHandle),TEEC_SUCCESS);
}

void encDecVerify(oc *o,uint32_t algo,uint32_t keyObj,const void *iv,size_t ivLen,const void *plain,size_t plainLen,
				  void *encoded,size_t encodedLen,void *decoded,size_t decodedLen)
{
	memset(encoded,0,encodedLen);
	memset(decoded,0,decodedLen);
	cipherTest(o,algo,TEE_MODE_ENCRYPT,keyObj,iv,ivLen,plain,plainLen,encoded,encodedLen);
	cipherTest(o,algo,TEE_MODE_DECRYPT,keyObj,iv,ivLen,encoded,encodedLen,decoded,decodedLen);
	ASSERT_EQ(memcmp(plain,decoded,plainLen),0);
}

TEST(Key, encDec) {
	oc o;
	TEEC_UUID uuid = TA_KEY_UUID;
	ASSERT_EQ(initializeContext(NULL,&o),TEEC_SUCCESS);
	ASSERT_EQ(openSession(&o,&uuid,TEEC_LOGIN_PUBLIC,NULL,NULL),TEEC_SUCCESS);

	uint32_t flags = TEE_DATA_FLAG_ACCESS_WRITE_META	|
					 TEE_DATA_FLAG_ACCESS_READ			|
					 TEE_DATA_FLAG_SHARE_READ			|
					 TEE_DATA_FLAG_ACCESS_WRITE			|
					 TEE_DATA_FLAG_SHARE_WRITE;

	uint32_t keyObj = 0;
	ASSERT_EQ(keyGenerate(&o,PRIVATE,"encDecTest",flags,256,&keyObj),TEEC_SUCCESS);
	ASSERT_NE(keyObj,0);

	ASSERT_EQ(TEE_AES_BLOCK_SIZE,16);
	char plain[]={"can't really explain it I haven't got the words"};
	ASSERT_EQ(sizeof(plain),16*3);
	char encoded[TEE_AES_BLOCK_SIZE*3]={0,};
	ASSERT_EQ(sizeof(encoded),16*3);
	ASSERT_EQ(sizeof(encoded),sizeof(plain));
	char decoded[TEE_AES_BLOCK_SIZE*3]={0,};
	ASSERT_EQ(sizeof(decoded),16*3);
	char iv[TEE_AES_BLOCK_SIZE]={0,};
	ASSERT_EQ(sizeof(iv),16);

	encDecVerify(&o,TEE_ALG_AES_ECB_NOPAD,keyObj,NULL,0,plain,sizeof(plain),
					encoded,sizeof(encoded),decoded,sizeof(decoded));
	encDecVerify(&o,TEE_ALG_AES_CBC_NOPAD,keyObj,iv,sizeof(iv),plain,sizeof(plain),
					encoded,sizeof(encoded),decoded,sizeof(decoded));
	encDecVerify(&o,TEE_ALG_AES_CTR,keyObj,iv,sizeof(iv),plain,sizeof(plain),
					encoded,sizeof(encoded),decoded,sizeof(decoded));

	ASSERT_EQ(keyCloseAndDelete(&o,keyObj),TEEC_SUCCESS);
	closeSession(&o);
	finalizeContext(&o);
}
