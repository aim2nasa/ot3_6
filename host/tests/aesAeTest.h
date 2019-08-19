#ifndef __AES_AE_TEST_H__
#define __AES_AE_TEST_H__

#include <gtest/gtest.h>
#include <session.h>
#include <key.h>

//suppose 48bytes of plain text
#define TESTING_DATA_SIZE (TEE_AES_BLOCK_SIZE*3)
#define TESTING_TEXT "can't really explain it I haven't got the words"

class AesAeTest : public ::testing::Test{
protected:
	AesAeTest():keyObj_(0),keySize_(0){}

	void SetUp() override
	{
		uuid_ = TA_KEY_UUID;
		ASSERT_EQ(initializeContext(NULL,&o_),TEEC_SUCCESS);
		ASSERT_EQ(openSession(&o_,&uuid_,TEEC_LOGIN_PUBLIC,NULL,NULL),TEEC_SUCCESS);
	}

	void TearDown() override
	{
		closeSession(&o_);
		finalizeContext(&o_);
	}

	void generateKey(uint32_t keySize)
	{
		uint32_t flags = TEE_DATA_FLAG_ACCESS_WRITE_META|
					 TEE_DATA_FLAG_ACCESS_READ			|
					 TEE_DATA_FLAG_SHARE_READ			|
					 TEE_DATA_FLAG_ACCESS_WRITE			|
					 TEE_DATA_FLAG_SHARE_WRITE;

		ASSERT_EQ(keyGenerate(&o_,PRIVATE,"aeKey",flags,keySize,&keyObj_),TEEC_SUCCESS);
		ASSERT_NE(keyObj_,0);
		keySize_ = keySize;
		ASSERT_GT(keySize_,0);
	}

	void deleteKey()
	{
		ASSERT_EQ(keyCloseAndDelete(&o_,keyObj_),TEEC_SUCCESS);
		keyObj_ = 0;
		keySize_ = 0;
	}

	void initParams(const char *nonce,uint32_t nonceLen,
					const char *tag,uint32_t tagLen,
					const char *aad,uint32_t aadLen,
					const char *payload,uint32_t payloadLen)
	{
		ASSERT_NE(keyObj_,0);
		ASSERT_GT(keySize_,0);

		ASSERT_EQ(sizeof(TESTING_TEXT),TESTING_DATA_SIZE);
		memcpy(plain_,TESTING_TEXT,TESTING_DATA_SIZE);
		memset(encod_,0,TESTING_DATA_SIZE);
		memset(decod_,0,TESTING_DATA_SIZE);

		nonceLen_ = nonceLen;
		tagLen_ = tagLen;
		aadLen_ = aadLen;
		payloadLen_ = payloadLen;

		nonce_ = new char[nonceLen_];
		tag_ = new char[tagLen_];
		aad_ = new char[aadLen_];
		payload_ = new char[payloadLen_];

		memcpy(nonce_,nonce,nonceLen_);
		memcpy(tag_,tag,tagLen_);
		memcpy(aad_,aad,aadLen_);
		memcpy(payload_,payload,payloadLen_);

		ASSERT_GT(nonceLen_,0);
		ASSERT_GT(tagLen_,0);
		ASSERT_GT(aadLen_,0);
		ASSERT_GT(payloadLen_,0);
	}

	void clearParams()
	{
		delete [] payload_;
		delete [] aad_;
		delete [] tag_;
		delete [] nonce_;

		nonceLen_=tagLen_=aadLen_=payloadLen_=0;
		nonce_=tag_=aad_=payload_=NULL;
	}

	oc o_;
	TEEC_UUID uuid_;
	char plain_[TESTING_DATA_SIZE];
	char encod_[TESTING_DATA_SIZE];
	char decod_[TESTING_DATA_SIZE];
	char *nonce_;
	char *tag_;
	char *aad_;
	char *payload_;
	uint32_t nonceLen_;
	uint32_t tagLen_;
	uint32_t aadLen_;
	uint32_t payloadLen_;
	uint32_t keyObj_;
	uint32_t keySize_;
};

static void aeTest(oc *o,uint32_t algo,uint32_t mode,uint32_t keyObj,const void *nonce,size_t nonceLen,
				uint32_t tagLen,uint32_t payloadLen,std::string aad,
				const void *inpBuf,size_t inpBufLen,void *outBuf,size_t outBufLen,void *tag)
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

	nI += 10;	//previous input buffer size
	outSize = outBufLen-nO;
	ASSERT_EQ(keyAEUpdate(o,operHandle,(char*)inpBuf+nI,10,(char*)outBuf+nO,&outSize),TEEC_SUCCESS);
	nO+=outSize;

	nI += 10;	//previous input buffer size
	outSize = outBufLen-nO;
	ASSERT_EQ(keyAEUpdate(o,operHandle,(char*)inpBuf+nI,10,(char*)outBuf+nO,&outSize),TEEC_SUCCESS);
	nO+=outSize;

	nI += 10;	//previous input buffer size
	outSize = outBufLen-nO;
	size_t outTagLen = tagLen;
	if(mode==TEE_MODE_ENCRYPT){
		ASSERT_EQ(keyAEEncryptFinal(o,operHandle,(char*)inpBuf+nI,8,(char*)outBuf+nO,&outSize,(char*)tag,&outTagLen),TEEC_SUCCESS);
	}else if(mode==TEE_MODE_DECRYPT){
		ASSERT_EQ(keyAEDecryptFinal(o,operHandle,(char*)inpBuf+nI,8,(char*)outBuf+nO,&outSize,(char*)tag,tagLen),TEEC_SUCCESS);
	}else{
		ASSERT_TRUE(0);	//in this case, wrong mode is given
	}

	nO+=outSize;

	ASSERT_EQ(nO,16*3);

	nI += 8;	//previous input buffer size
	ASSERT_EQ(nI,16*3);

	ASSERT_EQ(keyFreeOper(o,operHandle),TEEC_SUCCESS);
}

#endif
