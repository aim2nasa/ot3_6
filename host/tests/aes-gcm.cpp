#include "aesAeTest.h"

TEST_F(AesAeTest,GCM_init)
{
	SetUp(256/*keySize*/,13/*nonceLen*/,12/*tagLen*/,16/*aadLen*/,TESTING_DATA_SIZE/*payloadLen*/);

	OperHandle op = TEE_HANDLE_NULL;
	ASSERT_EQ(keyAllocOper(&o_,TEE_ALG_AES_GCM,TEE_MODE_ENCRYPT,keyObj_,&op),TEEC_SUCCESS);
	ASSERT_NE(op,(void*)TEE_HANDLE_NULL);

	ASSERT_EQ(keySetKeyOper(&o_,op,keyObj_),TEEC_SUCCESS);

	ASSERT_EQ(keyAEInit(&o_,op,nonce_,nonceLen_,tagLen_,aadLen_,payloadLen_),TEEC_SUCCESS);

	std::string aad("This is aad");
	ASSERT_EQ(keyAEUpdateAad(&o_,op,aad.c_str(),aad.size()),TEEC_SUCCESS);

	ASSERT_EQ(keyFreeOper(&o_,op),TEEC_SUCCESS);
}

TEST_F(AesAeTest,GCM_encoding)
{
	SetUp(256/*keySize*/,13/*nonceLen*/,12/*tagLen*/,16/*aadLen*/,TESTING_DATA_SIZE/*payloadLen*/);

#ifdef _DEBUG_TEST_
	std::cout<<"before encryption:";
	for(size_t i=0;i<sizeof(encod_);i++) std::cout<<std::hex<<(int)encod_[i]<<" ";
	std::cout<<std::endl;
#endif

	aeTest(&o_,TEE_ALG_AES_GCM,TEE_MODE_ENCRYPT,keyObj_,nonce_,nonceLen_,tagLen_,payloadLen_,"This is AAD Data",
			plain_,sizeof(plain_),encod_,sizeof(encod_),tag_);

#ifdef _DEBUG_TEST_
	std::cout<<"after encryption:";
	for(size_t i=0;i<sizeof(encod_);i++) std::cout<<std::hex<<(int)encod_[i]<<" ";
	std::cout<<std::endl;
#endif
}

TEST_F(AesAeTest,GCM_encDecVerify)
{
	SetUp(256/*keySize*/,13/*nonceLen*/,12/*tagLen*/,16/*aadLen*/,TESTING_DATA_SIZE/*payloadLen*/);
	aeTest(&o_,TEE_ALG_AES_GCM,TEE_MODE_ENCRYPT,keyObj_,nonce_,nonceLen_,tagLen_,payloadLen_,"This is AAD Data",
			plain_,sizeof(plain_),encod_,sizeof(encod_),tag_);
	aeTest(&o_,TEE_ALG_AES_GCM,TEE_MODE_DECRYPT,keyObj_,nonce_,nonceLen_,tagLen_,payloadLen_,"This is AAD Data",
			encod_,sizeof(encod_),decod_,sizeof(decod_),tag_);

	ASSERT_EQ(memcmp(plain_,decod_,sizeof(plain_)),0);

#ifdef _DEBUG_TEST_
	std::cout<<decod_<<std::endl;
#endif
}
