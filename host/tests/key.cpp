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

	ASSERT_EQ(keyGen(&o,PRIVATE/*storageId*/,"testKey"/*keyFileName*/,flags,256/*keySize*/),TEEC_SUCCESS);

	closeSession(&o);
	finalizeContext(&o);
}
