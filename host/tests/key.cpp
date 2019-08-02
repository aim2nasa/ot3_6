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

TEST(Key, list) { 
	oc o;
	TEEC_UUID uuid = TA_KEY_UUID; 
	ASSERT_EQ(initializeContext(NULL,&o),TEEC_SUCCESS);
	ASSERT_EQ(openSession(&o,&uuid,TEEC_LOGIN_PUBLIC,NULL,NULL),TEEC_SUCCESS);

	ASSERT_EQ(keyList(&o,PRIVATE/*storageId*/),TEEC_SUCCESS);

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

		std::cout<<"["<<i++<<"] "<<id<<std::endl;

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

	eObjList *list = NULL;
	ASSERT_EQ(keyEnumObjectList(&o,PRIVATE/*storageId*/,&list/*eObjList***/),TEEC_SUCCESS);
	display(list);

	keyFreeEnumObjectList(&list);
	ASSERT_EQ(list,(eObjList*)NULL);

	closeSession(&o);
	finalizeContext(&o);
}
