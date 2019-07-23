#include <gtest/gtest.h>
#include <session.h>
#include <crypt.h>
 
TEST(Crypt, aes256ecb_encDec) { 
	oc o;
	TEEC_UUID uuid = TA_CRYPT_UUID; 
	ASSERT_EQ(initializeContext(NULL,&o),TEEC_SUCCESS);
	ASSERT_EQ(openSession(&o,&uuid,TEEC_LOGIN_PUBLIC,NULL,NULL),TEEC_SUCCESS);

	uint8_t inp[16]={1,2,3,4};
	ASSERT_EQ(sizeof(inp),16);
	uint8_t out[16];
	ASSERT_EQ(sizeof(out),16);

	size_t len = sizeof(out);
	ASSERT_EQ(ta_crypt_cmd_aes256ecb_enc(&o,inp,sizeof(inp),out,&len),TEEC_SUCCESS);
	std::cout<<"len="<<len<<std::endl;

	ASSERT_NE(memcmp(inp,out,len),0);
	
	uint8_t tmp[16]={0};
	size_t tmpLen = sizeof(tmp);
	ASSERT_EQ(ta_crypt_cmd_aes256ecb_dec(&o,out,len,tmp,&tmpLen),TEEC_SUCCESS);
	std::cout<<"tmpLen="<<tmpLen<<std::endl;

	ASSERT_EQ(tmpLen,sizeof(inp));
	ASSERT_EQ(memcmp(inp,tmp,tmpLen),0);

	closeSession(&o);
	finalizeContext(&o);
}

TEST(Crypt, sha256) {
	oc o;
	TEEC_UUID uuid = TA_CRYPT_UUID;
	ASSERT_EQ(initializeContext(NULL,&o),TEEC_SUCCESS);
	ASSERT_EQ(openSession(&o,&uuid,TEEC_LOGIN_PUBLIC,NULL,NULL),TEEC_SUCCESS);

	const uint8_t sha256_in[] = { 'a', 'b', 'c' };
	const uint8_t sha256_out[] = {
		0xba, 0x78, 0x16, 0xbf, 0x8f, 0x01, 0xcf, 0xea,
		0x41, 0x41, 0x40, 0xde, 0x5d, 0xae, 0x22, 0x23,
		0xb0, 0x03, 0x61, 0xa3, 0x96, 0x17, 0x7a, 0x9c,
		0xb4, 0x10, 0xff, 0x61, 0xf2, 0x00, 0x15, 0xad
	};
	uint8_t out[32] = { };

	size_t len = sizeof(out);
	ASSERT_EQ(ta_crypt_cmd_sha256(&o,sha256_in,sizeof(sha256_in),out,&len),TEEC_SUCCESS);
	std::cout<<"len="<<len<<std::endl;

	ASSERT_EQ(memcmp(sha256_out,out,len),0);

	closeSession(&o);
	finalizeContext(&o);
}
