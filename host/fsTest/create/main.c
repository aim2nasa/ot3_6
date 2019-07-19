#include <err.h>
#include <stdio.h>
#include <fs.h>
#include <tee_api_defines.h>

static uint8_t objectID[] = { 0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,
			      0x08,0x09,0x10,0x11,0x12,0x13,0x14,0x15 };
static uint8_t data[] = { 0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,0x1a,0x1b };

int main(int argc, char *argv[])
{
	TEEC_Result res;
	fs f;
	uint32_t obj;
	uint32_t storage_id = TEE_STORAGE_PRIVATE;

	printf("initializeContext...\n");
	res = initializeContext(NULL,&f);
	if(res!=TEEC_SUCCESS)
		errx(1,"initializeContext failed with code 0x%x",res);
	printf("initializeContext ok\n");

	printf("openSession...\n");
	res = openSession(&f,TEEC_LOGIN_PUBLIC,NULL,NULL);
	if(res!=TEEC_SUCCESS)
		errx(1,"openSession failed with code 0x%x origin 0x%x",res,f.error);
	printf("openSession ok\n");

	printf("Creating in TA...\n");
	res = fs_create(&f,objectID,sizeof(objectID),
					TEE_DATA_FLAG_ACCESS_WRITE|
					TEE_DATA_FLAG_ACCESS_WRITE_META,
					0,data,sizeof(data),&obj,storage_id);
	if(res!=TEEC_SUCCESS)
		errx(1,"fs_create failed with code 0x%x",res);
	printf("Created in TA, obj=0x%x\n",obj);

	printf("Closing...\n");
	res = fs_close(&f,obj);
	if(res!=TEEC_SUCCESS)
		errx(1,"fs_close failed with code 0x%x",res);
	printf("Closed\n");

	printf("Opening...\n");
	res = fs_open(&f,objectID,sizeof(objectID),TEE_DATA_FLAG_ACCESS_WRITE_META,&obj,storage_id);
	if(res!=TEEC_SUCCESS)
		errx(1,"fs_open failed with code 0x%x",res);
	printf("Opened\n");

	printf("Closing...\n");
	res = fs_close(&f,obj);
	if(res!=TEEC_SUCCESS)
		errx(1,"fs_close failed with code 0x%x",res);
	printf("Closed\n");

	printf("Opening...\n");
	res = fs_open(&f,objectID,sizeof(objectID),TEE_DATA_FLAG_ACCESS_WRITE_META,&obj,storage_id);
	if(res!=TEEC_SUCCESS)
		errx(1,"fs_open failed with code 0x%x",res);
	printf("Opened\n");

	printf("Unlink...\n");
	res = fs_unlink(&f,obj);
	if(res!=TEEC_SUCCESS)
		errx(1,"fs_unlink failed with code 0x%x",res);
	printf("Unlink done\n");

	printf("closeSession...\n");
	closeSession(&f);
	printf("closeSession ok\n");

	printf("finalizeContext...\n");
	finalizeContext(&f);
	printf("finalizeContext ok\n");

	printf("create test end\n");
	return 0;
}
