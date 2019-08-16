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
	void SetUp() override
	{
		uuid_ = TA_KEY_UUID;
		ASSERT_EQ(initializeContext(NULL,&o_),TEEC_SUCCESS);
		ASSERT_EQ(openSession(&o_,&uuid_,TEEC_LOGIN_PUBLIC,NULL,NULL),TEEC_SUCCESS);

		ASSERT_EQ(sizeof(TESTING_TEXT),TESTING_DATA_SIZE);
		memcpy(plain_,TESTING_TEXT,TESTING_DATA_SIZE);
		memset(encod_,0,TESTING_DATA_SIZE);
		memset(decod_,0,TESTING_DATA_SIZE);

		nonce_ = tag_ = NULL;
		keyObj_ = keySize_ = nonceLen_ = tagLen_ = aadLen_ = payloadLen_ = 0;
	}

	void SetUp(uint32_t keySize,uint32_t nonceLen,uint32_t tagLen,uint32_t aadLen,uint32_t payloadLen)
	{
		uint32_t flags = TEE_DATA_FLAG_ACCESS_WRITE_META|
					 TEE_DATA_FLAG_ACCESS_READ			|
					 TEE_DATA_FLAG_SHARE_READ			|
					 TEE_DATA_FLAG_ACCESS_WRITE			|
					 TEE_DATA_FLAG_SHARE_WRITE;

		ASSERT_EQ(keyGenerate(&o_,PRIVATE,"aeKey",flags,keySize,&keyObj_),TEEC_SUCCESS);
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

		tag_ = new char[tagLen_];
		memset(tag_,0,tagLen_);
	}

	void TearDown() override
	{
		delete [] tag_;
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
	char *tag_;
	uint32_t keyObj_;
	uint32_t keySize_;
	uint32_t nonceLen_;
	uint32_t tagLen_;
	uint32_t aadLen_;
	uint32_t payloadLen_;
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
