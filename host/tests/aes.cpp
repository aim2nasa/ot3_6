#include <gtest/gtest.h>
#include <session.h>
#include <key.h>

//suppose 48bytes of plain text
#define TESTING_DATA_SIZE (TEE_AES_BLOCK_SIZE*3)
#define TESTING_TEXT "can't really explain it I haven't got the words"

class AesTest : public ::testing::Test{
protected:
	void SetUp() override
	{
		uuid_ = TA_KEY_UUID;
		ASSERT_EQ(initializeContext(NULL,&o_),TEEC_SUCCESS);
		ASSERT_EQ(openSession(&o_,&uuid_,TEEC_LOGIN_PUBLIC,NULL,NULL),TEEC_SUCCESS);

		ASSERT_EQ(sizeof(TESTING_TEXT),TESTING_DATA_SIZE);
		memcpy(plain_,TESTING_TEXT,TESTING_DATA_SIZE);
		memset(encod_,0,TESTING_DATA_SIZE);
		memset(decod_,0,TESTING_DATA_SIZE);
		memset(iv_,0,TEE_AES_BLOCK_SIZE);

		uint32_t flags = TEE_DATA_FLAG_ACCESS_WRITE_META|
					 TEE_DATA_FLAG_ACCESS_READ			|
					 TEE_DATA_FLAG_SHARE_READ			|
					 TEE_DATA_FLAG_ACCESS_WRITE			|
					 TEE_DATA_FLAG_SHARE_WRITE;

		keyObj_ = 0;
		ASSERT_EQ(keyGenerate(&o_,PRIVATE,"aesTestKey",flags,256,&keyObj_),TEEC_SUCCESS);
	}

	void TearDown() override
	{
		ASSERT_EQ(keyCloseAndDelete(&o_,keyObj_),TEEC_SUCCESS);
		closeSession(&o_);
		finalizeContext(&o_);
	}

	oc o_;
	TEEC_UUID uuid_;
	uint32_t keyObj_;
	char plain_[TESTING_DATA_SIZE];
	char encod_[TESTING_DATA_SIZE];
	char decod_[TESTING_DATA_SIZE];
	char iv_[TEE_AES_BLOCK_SIZE];
};

static void cipherTest(oc *o,uint32_t algo,uint32_t mode,uint32_t keyObj,const void *iv,size_t ivLen,
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

static void encDecVerify(oc *o,uint32_t algo,uint32_t keyObj,const void *iv,size_t ivLen,
	const void *plain,size_t plainLen,void *encoded,size_t encodedLen,void *decoded,size_t decodedLen)
{
	memset(encoded,0,encodedLen);
	memset(decoded,0,decodedLen);
	cipherTest(o,algo,TEE_MODE_ENCRYPT,keyObj,iv,ivLen,plain,plainLen,encoded,encodedLen);
	cipherTest(o,algo,TEE_MODE_DECRYPT,keyObj,iv,ivLen,encoded,encodedLen,decoded,decodedLen);
	ASSERT_EQ(memcmp(plain,decoded,plainLen),0);
}

TEST_F(AesTest,TEE_ALG_AES_ECB_NOPAD)
{
	encDecVerify(&o_,TEE_ALG_AES_ECB_NOPAD,keyObj_,NULL,0,plain_,sizeof(plain_),
					encod_,sizeof(encod_),decod_,sizeof(decod_));
}

TEST_F(AesTest,TEE_ALG_AES_CBC_NOPAD)
{
	encDecVerify(&o_,TEE_ALG_AES_CBC_NOPAD,keyObj_,iv_,sizeof(iv_),plain_,sizeof(plain_),
					encod_,sizeof(encod_),decod_,sizeof(decod_));
}

TEST_F(AesTest,TEE_ALG_AES_CTR)
{
	encDecVerify(&o_,TEE_ALG_AES_CTR,keyObj_,iv_,sizeof(iv_),plain_,sizeof(plain_),
					encod_,sizeof(encod_),decod_,sizeof(decod_));
}
