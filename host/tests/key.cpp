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
