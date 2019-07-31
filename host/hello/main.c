#include <err.h>
#include <stdio.h>
#include <tee_client_api.h>
#include <hello_ta.h>
#include <string.h>

int main(int argc, char *argv[])
{
	TEEC_Result res;
	TEEC_Context ctx;
	TEEC_Session sess;
	TEEC_UUID uuid = TA_HELLO_UUID;
	uint32_t err_origin;

	printf("TEEC_InitializeContext...\n");
	res = TEEC_InitializeContext(NULL,&ctx);
	if(res!=TEEC_SUCCESS)
		errx(1,"TEEC_InitializeContext failed with code 0x%x",res);
	printf("TEEC_InitializeContext ok\n");

	printf("TEEC_OpenSession...\n");
	res = TEEC_OpenSession(&ctx,&sess,&uuid,TEEC_LOGIN_PUBLIC,NULL,NULL,&err_origin);
	if(res!=TEEC_SUCCESS)
		errx(1,"TEEC_OpenSession failed with code 0x%x origin 0x%x",res,err_origin);
	printf("TEEC_OpenSession ok\n");

	printf("Invoking TA...\n");
	char output[50]={ 'A' };
	memset(output,0x41,sizeof(output));
	printf("init output=%s\n",(char*)output);
	TEEC_Operation op;
	op.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INPUT,TEEC_MEMREF_TEMP_OUTPUT,
									 TEEC_NONE,TEEC_NONE);
	op.params[0].tmpref.buffer = "Hello";
	op.params[0].tmpref.size = 5;
	op.params[1].tmpref.buffer = &output;
	op.params[1].tmpref.size = sizeof(output);

	printf("1====> op.params[1].tmpref.buffer=%p\n",op.params[1].tmpref.buffer);

	res = TEEC_InvokeCommand(&sess,TA_HELLO_CMD,&op,&err_origin);
	if(res!=TEEC_SUCCESS)
		errx(1,"TEEC_InvokeCommand failed with code 0x%x origin 0x%x",res,err_origin);
	printf("TA Invoked\n");
	printf("2====> op.params[1].tmpref.buffer=%p\n",op.params[1].tmpref.buffer);

	printf("output size= %lu\n",op.params[1].tmpref.size);
	output[op.params[1].tmpref.size]=0;	//make NULL
	printf("output=%s\n",(char*)output);

	printf("TEEC_FinalizeContext...\n");
	TEEC_FinalizeContext(&ctx);
	printf("TEEC_FinalizeContext ok\n");

	printf("Hello end\n");
	return 0;
}
