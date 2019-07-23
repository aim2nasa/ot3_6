#include "helpers.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <session.h>
#include <ta_crypt.h>

/* Round up the even multiple of size, size has to be a multiple of 2 */
#define ROUNDUP(v, size) (((v) + (size - 1)) & ~(size - 1))

TEE_Result pack_attrs(const TEE_Attribute *attrs, uint32_t attr_count,
		      uint8_t **buf, size_t *blen)
{
	struct tee_attr_packed *a = NULL;
	uint8_t *b = NULL;
	size_t bl = 0;
	size_t n = 0;

	*buf = NULL;
	*blen = 0;
	if (attr_count == 0)
		return TEE_SUCCESS;

	bl = sizeof(uint32_t) + sizeof(struct tee_attr_packed) * attr_count;
	for (n = 0; n < attr_count; n++) {
		if ((attrs[n].attributeID & TEE_ATTR_BIT_VALUE) != 0)
			continue; /* Only memrefs need to be updated */

		if (!attrs[n].content.ref.buffer)
			continue;

		/* Make room for padding */
		bl += ROUNDUP(attrs[n].content.ref.length, 4);
	}

	b = calloc(1, bl);
	if (!b)
		return TEE_ERROR_OUT_OF_MEMORY;

	*buf = b;
	*blen = bl;

	*(uint32_t *)(void *)b = attr_count;
	b += sizeof(uint32_t);
	a = (struct tee_attr_packed *)(void *)b;
	b += sizeof(struct tee_attr_packed) * attr_count;

	for (n = 0; n < attr_count; n++) {
		a[n].attr_id = attrs[n].attributeID;
		if (attrs[n].attributeID & TEE_ATTR_BIT_VALUE) {
			a[n].a = attrs[n].content.value.a;
			a[n].b = attrs[n].content.value.b;
			continue;
		}

		a[n].b = attrs[n].content.ref.length;

		if (!attrs[n].content.ref.buffer) {
			a[n].a = 0;
			continue;
		}

		memcpy(b, attrs[n].content.ref.buffer,
		       attrs[n].content.ref.length);

		/* Make buffer pointer relative to *buf */
		a[n].a = (uint32_t)(uintptr_t)(b - *buf);

		/* Round up to good alignment */
		b += ROUNDUP(attrs[n].content.ref.length, 4);
	}

	return TEE_SUCCESS;
}

TEEC_Result ta_crypt_cmd_allocate_operation(oc *o, TEE_OperationHandle *oph,
					    uint32_t algo, uint32_t mode, uint32_t max_key_size)
{
	TEEC_Result res = TEEC_ERROR_GENERIC;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t ret_orig = 0;

	op.params[0].value.a = 0;
	op.params[0].value.b = algo;
	op.params[1].value.a = mode;
	op.params[1].value.b = max_key_size;

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INOUT, TEEC_VALUE_INPUT,
					 TEEC_NONE, TEEC_NONE);

	res = TEEC_InvokeCommand(o->session, TA_CRYPT_CMD_ALLOCATE_OPERATION, &op,
				 &ret_orig);

	if (res == TEEC_SUCCESS)
		*oph = (TEE_OperationHandle)(uintptr_t)op.params[0].value.a;

	return res;
}

TEEC_Result ta_crypt_cmd_allocate_transient_object(oc *o, TEE_ObjectType obj_type,
						   uint32_t max_obj_size, TEE_ObjectHandle *oh)
{
	TEEC_Result res = TEEC_ERROR_GENERIC;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t ret_orig = 0;

	op.params[0].value.a = obj_type;
	op.params[0].value.b = max_obj_size;

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_VALUE_OUTPUT,
					 TEEC_NONE, TEEC_NONE);

	res = TEEC_InvokeCommand(o->session, TA_CRYPT_CMD_ALLOCATE_TRANSIENT_OBJECT, &op,
				 &ret_orig);

	if (res == TEEC_SUCCESS)
		*oh = (TEE_ObjectHandle)(uintptr_t)op.params[1].value.a;

	return res;
}

TEEC_Result ta_crypt_cmd_populate_transient_object(oc *o, TEE_ObjectHandle oh,
						   const TEE_Attribute *attrs, uint32_t attr_count)
{
	TEEC_Result res = TEEC_ERROR_GENERIC;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t ret_orig = 0;
	uint8_t *buf = NULL;
	size_t blen = 0;

	res = pack_attrs(attrs, attr_count, &buf, &blen);
	if (res!=TEEC_SUCCESS)
		return res;

	assert((uintptr_t)oh <= UINT32_MAX);
	op.params[0].value.a = (uint32_t)(uintptr_t)oh;

	op.params[1].tmpref.buffer = buf;
	op.params[1].tmpref.size = blen;

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,
					 TEEC_MEMREF_TEMP_INPUT, TEEC_NONE,
					 TEEC_NONE);

	res = TEEC_InvokeCommand(o->session, TA_CRYPT_CMD_POPULATE_TRANSIENT_OBJECT, &op,
				 &ret_orig);

	free(buf);
	return res;
}

TEE_Result ta_crypt_cmd_set_operation_key(oc *o, TEE_OperationHandle oph, TEE_ObjectHandle key)
{
	TEEC_Result res = TEEC_ERROR_GENERIC;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t ret_orig = 0;

	assert((uintptr_t)oph <= UINT32_MAX);
	op.params[0].value.a = (uint32_t)(uintptr_t)oph;

	assert((uintptr_t)key <= UINT32_MAX);
	op.params[0].value.b = (uint32_t)(uintptr_t)key;

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_NONE, TEEC_NONE,
					 TEEC_NONE);

	res = TEEC_InvokeCommand(o->session, TA_CRYPT_CMD_SET_OPERATION_KEY, &op,
				 &ret_orig);
	return res;
}

TEEC_Result ta_crypt_cmd_free_transient_object(oc *o, TEE_ObjectHandle oh)
{
	TEEC_Result res = TEEC_ERROR_GENERIC;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t ret_orig = 0;

	assert((uintptr_t)oh <= UINT32_MAX);
	op.params[0].value.a = (uint32_t)(uintptr_t)oh;
	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_NONE, TEEC_NONE,
					 TEEC_NONE);

	res = TEEC_InvokeCommand(o->session, TA_CRYPT_CMD_FREE_TRANSIENT_OBJECT, &op,
				 &ret_orig);
	return res;
}

TEEC_Result ta_crypt_cmd_derive_key(oc *o,TEE_OperationHandle oph, TEE_ObjectHandle oh,
					const TEE_Attribute *params, uint32_t paramCount)
{
	TEEC_Result res = TEEC_ERROR_GENERIC;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t ret_orig = 0;
	uint8_t *buf = NULL;
	size_t blen = 0;

	res = pack_attrs(params, paramCount, &buf, &blen);

	if (res!=TEEC_SUCCESS)
		return res;

	assert((uintptr_t)oph <= UINT32_MAX);
	op.params[0].value.a = (uint32_t)(uintptr_t)oph;

	assert((uintptr_t)oh <= UINT32_MAX);
	op.params[0].value.b = (uint32_t)(uintptr_t)oh;

	op.params[1].tmpref.buffer = buf;
	op.params[1].tmpref.size = blen;

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,
					 TEEC_MEMREF_TEMP_INPUT, TEEC_NONE,
					 TEEC_NONE);

	res = TEEC_InvokeCommand(o->session, TA_CRYPT_CMD_DERIVE_KEY, &op, &ret_orig);

	free(buf);
	return res;
}

TEEC_Result ta_crypt_cmd_get_object_buffer_attribute(oc * o, TEE_ObjectHandle oh,
					uint32_t attr_id, void *buf, size_t *blen)
{
	TEEC_Result res = TEEC_ERROR_GENERIC;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t ret_orig = 0;

	assert((uintptr_t)oh <= UINT32_MAX);
	op.params[0].value.a = (uint32_t)(uintptr_t)oh;
	op.params[0].value.b = attr_id;

	op.params[1].tmpref.buffer = buf;
	op.params[1].tmpref.size = *blen;

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,
					 TEEC_MEMREF_TEMP_OUTPUT, TEEC_NONE,
					 TEEC_NONE);

	res = TEEC_InvokeCommand(o->session, TA_CRYPT_CMD_GET_OBJECT_BUFFER_ATTRIBUTE,
				 &op, &ret_orig);

	if (res == TEEC_SUCCESS)
		*blen = op.params[1].tmpref.size;

	return res;
}

TEEC_Result ta_crypt_cmd_free_operation(oc *o, TEE_OperationHandle oph)
{
	TEEC_Result res = TEEC_ERROR_GENERIC;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t ret_orig = 0;

	op.params[0].value.a = (uint32_t)(uintptr_t)oph;

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_NONE, TEEC_NONE,
					 TEEC_NONE);

	res = TEEC_InvokeCommand(o->session, TA_CRYPT_CMD_FREE_OPERATION, &op,
				 &ret_orig);
	return res;
}
