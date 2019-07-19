#include "fs.h"
#include <stdlib.h>
#include <ta_storage.h>

#define TEEC_OPERATION_INITIALIZER {}

TEEC_UUID uuid = TA_STORAGE_UUID; 

TEEC_UUID * getUUID()
{
	return &uuid;
}

TEEC_Result fs_open(oc *o, void *id, uint32_t id_size,
			   uint32_t flags, uint32_t *obj, uint32_t storage_id)
{
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	TEEC_Result res = TEEC_ERROR_GENERIC;
	uint32_t org = 0;

	op.params[0].tmpref.buffer = id;
	op.params[0].tmpref.size = id_size;
	op.params[1].value.a = flags;
	op.params[1].value.b = 0;
	op.params[2].value.a = storage_id;

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INPUT,
					 TEEC_VALUE_INOUT, TEEC_VALUE_INPUT,
					 TEEC_NONE);

	res = TEEC_InvokeCommand(o->session, TA_STORAGE_CMD_OPEN, &op, &org);

	if (res == TEEC_SUCCESS)
		*obj = op.params[1].value.b;

	return res;
}

TEEC_Result fs_create(oc *o, void *id, uint32_t id_size,
			     uint32_t flags, uint32_t attr, void *data,
			     uint32_t data_size, uint32_t *obj,
			     uint32_t storage_id)
{
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	TEEC_Result res = TEEC_ERROR_GENERIC;
	uint32_t org = 0;

	op.params[0].tmpref.buffer = id;
	op.params[0].tmpref.size = id_size;
	op.params[1].value.a = flags;
	op.params[1].value.b = 0;
	op.params[2].value.a = attr;
	op.params[2].value.b = storage_id;
	op.params[3].tmpref.buffer = data;
	op.params[3].tmpref.size = data_size;

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INPUT,
					 TEEC_VALUE_INOUT, TEEC_VALUE_INPUT,
					 TEEC_MEMREF_TEMP_INPUT);

	res = TEEC_InvokeCommand(o->session, TA_STORAGE_CMD_CREATE, &op, &org);

	if (res == TEEC_SUCCESS)
		*obj = op.params[1].value.b;

	return res;
}

TEEC_Result fs_create_overwrite(oc *o, void *id,
				       uint32_t id_size, uint32_t storage_id)
{
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	TEEC_Result res = TEEC_ERROR_GENERIC;
	uint32_t org = 0;

	op.params[0].tmpref.buffer = id;
	op.params[0].tmpref.size = id_size;
	op.params[1].value.a = storage_id;

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INPUT,
					 TEEC_VALUE_INPUT, TEEC_NONE,
					 TEEC_NONE);

	res = TEEC_InvokeCommand(o->session, TA_STORAGE_CMD_CREATE_OVERWRITE, &op, &org);

	return res;
}

TEEC_Result fs_close(oc *o, uint32_t obj)
{
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t org = 0;

	op.params[0].value.a = obj;

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_NONE,
					 TEEC_NONE, TEEC_NONE);

	return TEEC_InvokeCommand(o->session, TA_STORAGE_CMD_CLOSE, &op, &org);
}

TEEC_Result fs_read(oc *o, uint32_t obj, void *data,
			   uint32_t data_size, uint32_t *count)
{
	TEEC_Result res = TEEC_ERROR_GENERIC;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t org = 0;

	op.params[0].tmpref.buffer = data;
	op.params[0].tmpref.size = data_size;
	op.params[1].value.a = obj;
	op.params[1].value.b = 0;

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_OUTPUT,
					 TEEC_VALUE_INOUT, TEEC_NONE,
					 TEEC_NONE);

	res = TEEC_InvokeCommand(o->session, TA_STORAGE_CMD_READ, &op, &org);

	if (res == TEEC_SUCCESS)
		*count = op.params[1].value.b;

	return res;
}

TEEC_Result fs_write(oc *o, uint32_t obj, void *data,
			    uint32_t data_size)
{
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t org = 0;

	op.params[0].tmpref.buffer = data;
	op.params[0].tmpref.size = data_size;
	op.params[1].value.a = obj;
	op.params[1].value.b = 0;

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INPUT,
					 TEEC_VALUE_INPUT, TEEC_NONE,
					 TEEC_NONE);

	return TEEC_InvokeCommand(o->session, TA_STORAGE_CMD_WRITE, &op, &org);
}

TEEC_Result fs_seek(oc *o, uint32_t obj, int32_t offset,
			   int32_t whence)
{
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t org = 0;

	op.params[0].value.a = obj;
	op.params[0].value.b = *(uint32_t *)&offset;
	op.params[1].value.a = whence;

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_VALUE_INOUT,
					 TEEC_NONE, TEEC_NONE);

	return TEEC_InvokeCommand(o->session, TA_STORAGE_CMD_SEEK, &op, &org);
}

TEEC_Result fs_unlink(oc *o, uint32_t obj)
{
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t org = 0;

	op.params[0].value.a = obj;

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_NONE,
					 TEEC_NONE, TEEC_NONE);

	return TEEC_InvokeCommand(o->session, TA_STORAGE_CMD_UNLINK, &op, &org);
}

TEEC_Result fs_trunc(oc *o, uint32_t obj, uint32_t len)
{
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t org = 0;

	op.params[0].value.a = obj;
	op.params[0].value.b = len;

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_NONE,
					 TEEC_NONE, TEEC_NONE);

	return TEEC_InvokeCommand(o->session, TA_STORAGE_CMD_TRUNC, &op, &org);
}

TEEC_Result fs_rename(oc *o, uint32_t obj, void *id,
			     uint32_t id_size)
{
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t org = 0;

	op.params[0].value.a = obj;
	op.params[1].tmpref.buffer = id;
	op.params[1].tmpref.size = id_size;

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,
					 TEEC_MEMREF_TEMP_INPUT, TEEC_NONE,
					 TEEC_NONE);

	return TEEC_InvokeCommand(o->session, TA_STORAGE_CMD_RENAME, &op, &org);
}

TEEC_Result fs_alloc_enum(oc *o, uint32_t *e)
{
	TEEC_Result res = TEEC_ERROR_GENERIC;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t org = 0;

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_OUTPUT, TEEC_NONE,
					 TEEC_NONE, TEEC_NONE);

	res = TEEC_InvokeCommand(o->session, TA_STORAGE_CMD_ALLOC_ENUM, &op, &org);

	if (res == TEEC_SUCCESS)
		*e = op.params[0].value.a;

	return res;
}

TEEC_Result fs_reset_enum(oc *o, uint32_t e)
{
	TEEC_Result res = TEEC_ERROR_GENERIC;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t org = 0;

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_NONE,
					 TEEC_NONE, TEEC_NONE);

	op.params[0].value.a = e;
	res = TEEC_InvokeCommand(o->session, TA_STORAGE_CMD_RESET_ENUM, &op, &org);

	return res;
}

TEEC_Result fs_free_enum(oc *o, uint32_t e)
{
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t org = 0;

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_NONE, TEEC_NONE,
					 TEEC_NONE);

	op.params[0].value.a = e;

	return TEEC_InvokeCommand(o->session, TA_STORAGE_CMD_FREE_ENUM, &op, &org);
}

TEEC_Result fs_start_enum(oc *o, uint32_t e,
				 uint32_t storage_id)
{
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t org = 0;

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_NONE,
					 TEEC_NONE, TEEC_NONE);

	op.params[0].value.a = e;
	op.params[0].value.b = storage_id;

	return TEEC_InvokeCommand(o->session, TA_STORAGE_CMD_START_ENUM, &op, &org);
}

TEEC_Result fs_next_enum(oc *o, uint32_t e, void *obj_info,
				size_t info_size, void *id, uint32_t id_size)
{
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t org = 0;

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_NONE,
					 TEEC_MEMREF_TEMP_OUTPUT, TEEC_NONE);
	if (obj_info && info_size)
		op.paramTypes |= (TEEC_MEMREF_TEMP_OUTPUT << 4);

	op.params[0].value.a = e;
	op.params[1].tmpref.buffer = obj_info;
	op.params[1].tmpref.size = info_size;
	op.params[2].tmpref.buffer = id;
	op.params[2].tmpref.size = id_size;

	return TEEC_InvokeCommand(o->session, TA_STORAGE_CMD_NEXT_ENUM, &op, &org);
}

TEEC_Result fs_restrict_usage(oc *o, uint32_t obj,
				     uint32_t obj_usage)
{
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t org = 0;

	op.params[0].value.a = obj;
	op.params[0].value.b = obj_usage;

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_NONE,
					 TEEC_NONE, TEEC_NONE);

	return TEEC_InvokeCommand(o->session, TA_STORAGE_CMD_RESTRICT_USAGE,
				  &op, &org);
}

TEEC_Result fs_alloc_obj(oc *o, uint32_t obj_type,
				     uint32_t max_key_size, uint32_t *obj)
{
	TEEC_Result res = TEEC_ERROR_GENERIC;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t org = 0;

	op.params[0].value.a = obj_type;
	op.params[0].value.b = max_key_size;

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_VALUE_OUTPUT,
					 TEEC_NONE, TEEC_NONE);

	res = TEEC_InvokeCommand(o->session, TA_STORAGE_CMD_ALLOC_OBJ, &op, &org);
	*obj = op.params[1].value.a;
	return res;
}

TEEC_Result fs_free_obj(oc *o, uint32_t obj)
{
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t org = 0;

	op.params[0].value.a = obj;

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_NONE,
					 TEEC_NONE, TEEC_NONE);

	return TEEC_InvokeCommand(o->session, TA_STORAGE_CMD_FREE_OBJ, &op, &org);
}

TEEC_Result fs_reset_obj(oc *o, uint32_t obj)
{
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t org = 0;

	op.params[0].value.a = obj;

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_NONE,
					 TEEC_NONE, TEEC_NONE);

	return TEEC_InvokeCommand(o->session, TA_STORAGE_CMD_RESET_OBJ, &op, &org);
}

TEEC_Result fs_get_obj_info(oc *o, uint32_t obj,
				void *obj_info, size_t info_size)
{
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t org = 0;

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,
					TEEC_MEMREF_TEMP_OUTPUT,
					TEEC_NONE, TEEC_NONE);

	op.params[0].value.a = obj;
	op.params[1].tmpref.buffer = obj_info;
	op.params[1].tmpref.size = info_size;

	return TEEC_InvokeCommand(o->session, TA_STORAGE_CMD_GET_OBJ_INFO, &op, &org);
}
