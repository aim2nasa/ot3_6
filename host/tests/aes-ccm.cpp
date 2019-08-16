#include <gtest/gtest.h>
#include <session.h>
#include <key.h>

//suppose 48bytes of plain text
#define TESTING_DATA_SIZE (TEE_AES_BLOCK_SIZE*3)
#define TESTING_TEXT "can't really explain it I haven't got the words"

class AesCcmTest : public ::testing::Test{
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

		nonce_ = NULL;
		keyObj_ = keySize_ = nonceLen_ = tagLen_ = aadLen_ = payloadLen_ = 0;
	}

	void SetUp(uint32_t keySize,uint32_t nonceLen,uint32_t tagLen,uint32_t aadLen,uint32_t payloadLen)
	{
		uint32_t flags = TEE_DATA_FLAG_ACCESS_WRITE_META|
					 TEE_DATA_FLAG_ACCESS_READ			|
					 TEE_DATA_FLAG_SHARE_READ			|
					 TEE_DATA_FLAG_ACCESS_WRITE			|
					 TEE_DATA_FLAG_SHARE_WRITE;

		ASSERT_EQ(keyGenerate(&o_,PRIVATE,"aesCcmKey",flags,keySize,&keyObj_),TEEC_SUCCESS);
		ASSERT_NE(keyObj_,0);

		keySize_ = keySize;
		nonceLen_ = nonceLen;
		tagLen_ = tagLen;
		aadLen_ = aadLen;
		payloadLen_ = payloadLen;

		ASSERT_GT(keySize_,0);
		ASSERT_GT(nonceLen_,0);
		ASSERT_GT(tagLen_,0);
		ASSERT_GT(aadLen_,0);
		ASSERT_GT(payloadLen_,0);

		nonce_ = new char[nonceLen_];
		memset(nonce_,0,nonceLen_);
	}

	void TearDown() override
	{
		delete [] nonce_;
		ASSERT_EQ(keyCloseAndDelete(&o_,keyObj_),TEEC_SUCCESS);
		closeSession(&o_);
		finalizeContext(&o_);
	}

	oc o_;
	TEEC_UUID uuid_;
	char plain_[TESTING_DATA_SIZE];
	char encod_[TESTING_DATA_SIZE];
	char decod_[TESTING_DATA_SIZE];
	char *nonce_;
	uint32_t keyObj_;
	uint32_t keySize_;
	uint32_t nonceLen_;
	uint32_t tagLen_;
	uint32_t aadLen_;
	uint32_t payloadLen_;
};

TEST_F(AesCcmTest,init)
{
	SetUp(256/*keySize*/,13/*nonceLen*/,4/*tagLen*/,16/*aadLen*/,TESTING_DATA_SIZE/*payloadLen*/);

	OperHandle op = TEE_HANDLE_NULL;
	ASSERT_EQ(keyAllocOper(&o_,TEE_ALG_AES_CCM,TEE_MODE_ENCRYPT,keyObj_,&op),TEEC_SUCCESS);
	ASSERT_NE(op,(void*)TEE_HANDLE_NULL);

	ASSERT_EQ(keySetKeyOper(&o_,op,keyObj_),TEEC_SUCCESS);

	ASSERT_EQ(keyAEInit(&o_,op,nonce_,nonceLen_,tagLen_,aadLen_,payloadLen_),TEEC_SUCCESS);

	std::string aad("This is aad");
	ASSERT_EQ(keyAEUpdateAad(&o_,op,aad.c_str(),aad.size()),TEEC_SUCCESS);

	ASSERT_EQ(keyFreeOper(&o_,op),TEEC_SUCCESS);
}

static void aeTest(oc *o,uint32_t algo,uint32_t mode,uint32_t keyObj,const void *nonce,size_t nonceLen,
				uint32_t tagLen,uint32_t payloadLen,std::string aad,
				const void *inpBuf,size_t inpBufLen,void *outBuf,size_t outBufLen)
{
	OperHandle operHandle = TEE_HANDLE_NULL;
	ASSERT_EQ(keyAllocOper(o,algo,mode,keyObj,&operHandle),TEEC_SUCCESS);
	ASSERT_NE(operHandle,(void*)TEE_HANDLE_NULL);

	ASSERT_EQ(keySetKeyOper(o,operHandle,keyObj),TEEC_SUCCESS);

	ASSERT_EQ(keyAEInit(o,operHandle,nonce,nonceLen,tagLen,aad.size(),payloadLen),TEEC_SUCCESS);

	ASSERT_GT(aad.size(),0);
	ASSERT_EQ(keyAEUpdateAad(o,operHandle,aad.c_str(),aad.size()),TEEC_SUCCESS);

	size_t outSize = outBufLen;
	int nI = 0;	//input buffer offset
	int nO = 0; //output buffer offset
	ASSERT_EQ(outSize,outBufLen);
	ASSERT_EQ(keyAEUpdate(o,operHandle,(char*)inpBuf+nI,10,(char*)outBuf+nO,&outSize),TEEC_SUCCESS);
	nO+= outSize;

	nI += 10;	//previous input buffer size
	outSize = outBufLen-nO;
	ASSERT_EQ(keyAEUpdate(o,operHandle,(char*)inpBuf+nI,10,(char*)outBuf+nO,&outSize),TEEC_SUCCESS);
	nO+=outSize;

	ASSERT_EQ(keyFreeOper(o,operHandle),TEEC_SUCCESS);
}

TEST_F(AesCcmTest,encoding)
{
	SetUp(256/*keySize*/,13/*nonceLen*/,4/*tagLen*/,16/*aadLen*/,TESTING_DATA_SIZE/*payloadLen*/);
	aeTest(&o_,TEE_ALG_AES_CCM,TEE_MODE_ENCRYPT,keyObj_,nonce_,nonceLen_,tagLen_,payloadLen_,"This is AAD Data",
			plain_,sizeof(plain_),encod_,sizeof(encod_));
}
