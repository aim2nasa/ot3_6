#include "crypt.h"
#include "helpers.h"
#include <assert.h>
#include <stdlib.h>

TEEC_Result ta_crypt_cmd_aes256ecb_enc(oc *o,
					const void *src, size_t src_len,void *dst, size_t *dst_len)
{
	TEEC_Result res = TEEC_ERROR_GENERIC;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t ret_orig = 0;

	op.params[0].tmpref.buffer = (void *)src;
	op.params[0].tmpref.size = src_len;

	op.params[1].tmpref.buffer = dst;
	op.params[1].tmpref.size = *dst_len;

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INPUT,
									 TEEC_MEMREF_TEMP_OUTPUT,
									 TEEC_NONE,TEEC_NONE);

	res = TEEC_InvokeCommand(o->session, TA_CRYPT_CMD_AES256ECB_ENC, &op, &ret_orig);
	if (res == TEEC_SUCCESS)
		*dst_len = op.params[1].tmpref.size;

	return res;
}

TEEC_Result ta_crypt_cmd_aes256ecb_dec(oc *o,
					const void *src, size_t src_len,void *dst, size_t *dst_len)
{
	TEEC_Result res = TEEC_ERROR_GENERIC;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t ret_orig = 0;

	op.params[0].tmpref.buffer = (void *)src;
	op.params[0].tmpref.size = src_len;

	op.params[1].tmpref.buffer = dst;
	op.params[1].tmpref.size = *dst_len;

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INPUT,
									 TEEC_MEMREF_TEMP_OUTPUT,
									 TEEC_NONE,TEEC_NONE);

	res = TEEC_InvokeCommand(o->session, TA_CRYPT_CMD_AES256ECB_DEC, &op, &ret_orig);
	if (res == TEEC_SUCCESS)
		*dst_len = op.params[1].tmpref.size;

	return res;
}

TEEC_Result ta_crypt_cmd_reset_operation(oc *o, TEE_OperationHandle oph)
{
	TEEC_Result res = TEEC_ERROR_GENERIC;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t ret_orig = 0;

	assert((uintptr_t)oph <= UINT32_MAX);
	op.params[0].value.a = (uint32_t)(uintptr_t)oph;
	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_NONE, TEEC_NONE,
					 TEEC_NONE);
	res = TEEC_InvokeCommand(o->session, TA_CRYPT_CMD_RESET_OPERATION, &op,
				 &ret_orig);
	return res;
}

TEEC_Result ta_crypt_cmd_copy_operation(oc *o, TEE_OperationHandle dst_oph,TEE_OperationHandle src_oph)
{
	TEEC_Result res = TEEC_ERROR_GENERIC;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t ret_orig = 0;

	assert((uintptr_t)dst_oph <= UINT32_MAX);
	op.params[0].value.a = (uint32_t)(uintptr_t)dst_oph;

	assert((uintptr_t)src_oph <= UINT32_MAX);
	op.params[0].value.b = (uint32_t)(uintptr_t)src_oph;
	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_NONE, TEEC_NONE,
					 TEEC_NONE);

	res = TEEC_InvokeCommand(o->session, TA_CRYPT_CMD_COPY_OPERATION, &op,
				 &ret_orig);
	return res;
}

TEEC_Result ta_crypt_cmd_digest_update(oc *o, TEE_OperationHandle oph,const void *chunk,size_t chunk_size)
{
	TEEC_Result res = TEEC_ERROR_GENERIC;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t ret_orig = 0;

	assert((uintptr_t)oph <= UINT32_MAX);
	op.params[0].value.a = (uint32_t)(uintptr_t)oph;
	op.params[1].tmpref.buffer = (void *)chunk;
	op.params[1].tmpref.size = chunk_size;

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,
					 TEEC_MEMREF_TEMP_INPUT, TEEC_NONE,
					 TEEC_NONE);

	res = TEEC_InvokeCommand(o->session, TA_CRYPT_CMD_DIGEST_UPDATE, &op, &ret_orig);
	return res;
}

TEEC_Result ta_crypt_cmd_digest_do_final(oc *o, TEE_OperationHandle oph,
						const void *chunk,
						size_t chunk_len, void *hash,
						size_t *hash_len)
{
	TEEC_Result res = TEEC_ERROR_GENERIC;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t ret_orig = 0;

	assert((uintptr_t)oph <= UINT32_MAX);
	op.params[0].value.a = (uint32_t)(uintptr_t)oph;

	op.params[1].tmpref.buffer = (void *)chunk;
	op.params[1].tmpref.size = chunk_len;

	op.params[2].tmpref.buffer = (void *)hash;
	op.params[2].tmpref.size = *hash_len;

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,
					 TEEC_MEMREF_TEMP_INPUT,
					 TEEC_MEMREF_TEMP_OUTPUT, TEEC_NONE);

	res = TEEC_InvokeCommand(o->session, TA_CRYPT_CMD_DIGEST_DO_FINAL, &op,
				 &ret_orig);

	if (res == TEEC_SUCCESS)
		*hash_len = op.params[2].tmpref.size;

	return res;
}

TEE_Result ta_crypt_cmd_set_operation_key2(oc *o, TEE_OperationHandle oph,
						  TEE_ObjectHandle key1,
						  TEE_ObjectHandle key2)
{
	TEEC_Result res = TEEC_ERROR_GENERIC;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t ret_orig = 0;

	assert((uintptr_t)oph <= UINT32_MAX);
	op.params[0].value.a = (uint32_t)(uintptr_t)oph;

	assert((uintptr_t)key1 <= UINT32_MAX);
	op.params[0].value.b = (uint32_t)(uintptr_t)key1;

	assert((uintptr_t)key2 <= UINT32_MAX);
	op.params[1].value.a = (uint32_t)(uintptr_t)key2;
	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_VALUE_INPUT,
					 TEEC_NONE, TEEC_NONE);

	res = TEEC_InvokeCommand(o->session, TA_CRYPT_CMD_SET_OPERATION_KEY2, &op,
				 &ret_orig);

	return res;
}

TEEC_Result ta_crypt_cmd_mac_init(oc *o, TEE_OperationHandle oph,const void *iv, size_t iv_len)
{
	TEEC_Result res = TEEC_ERROR_GENERIC;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t ret_orig = 0;

	assert((uintptr_t)oph <= UINT32_MAX);
	op.params[0].value.a = (uint32_t)(uintptr_t)oph;

	if (iv != NULL) {
		op.params[1].tmpref.buffer = (void *)iv;
		op.params[1].tmpref.size = iv_len;
		op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,
						 TEEC_MEMREF_TEMP_INPUT,
						 TEEC_NONE, TEEC_NONE);
	} else {
		op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_NONE,
						 TEEC_NONE, TEEC_NONE);
	}

	res = TEEC_InvokeCommand(o->session, TA_CRYPT_CMD_MAC_INIT, &op, &ret_orig);
	return res;
}

TEEC_Result ta_crypt_cmd_mac_update(oc *o, TEE_OperationHandle oph,
					   const void *chunk, size_t chunk_size)
{
	TEEC_Result res = TEEC_ERROR_GENERIC;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t ret_orig = 0;

	assert((uintptr_t)oph <= UINT32_MAX);
	op.params[0].value.a = (uint32_t)(uintptr_t)oph;

	op.params[1].tmpref.buffer = (void *)chunk;
	op.params[1].tmpref.size = chunk_size;

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,
					 TEEC_MEMREF_TEMP_INPUT, TEEC_NONE,
					 TEEC_NONE);

	res = TEEC_InvokeCommand(o->session, TA_CRYPT_CMD_MAC_UPDATE, &op, &ret_orig);
	return res;
}

TEEC_Result ta_crypt_cmd_mac_final_compute(oc *o, TEE_OperationHandle oph,
						  const void *chunk,
						  size_t chunk_len,
						  void *hash,
						  size_t *hash_len)
{
	TEEC_Result res = TEEC_ERROR_GENERIC;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t ret_orig = 0;

	assert((uintptr_t)oph <= UINT32_MAX);
	op.params[0].value.a = (uint32_t)(uintptr_t)oph;

	op.params[1].tmpref.buffer = (void *)chunk;
	op.params[1].tmpref.size = chunk_len;

	op.params[2].tmpref.buffer = (void *)hash;
	op.params[2].tmpref.size = *hash_len;

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,
					 TEEC_MEMREF_TEMP_INPUT,
					 TEEC_MEMREF_TEMP_OUTPUT, TEEC_NONE);

	res = TEEC_InvokeCommand(o->session, TA_CRYPT_CMD_MAC_FINAL_COMPUTE, &op,
				 &ret_orig);

	if (res == TEEC_SUCCESS)
		*hash_len = op.params[2].tmpref.size;

	return res;
}

TEEC_Result ta_crypt_cmd_cipher_init(oc *o, TEE_OperationHandle oph,
					    const void *iv, size_t iv_len)
{
	TEEC_Result res = TEEC_ERROR_GENERIC;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t ret_orig = 0;

	assert((uintptr_t)oph <= UINT32_MAX);
	op.params[0].value.a = (uint32_t)(uintptr_t)oph;

	if (iv != NULL) {
		op.params[1].tmpref.buffer = (void *)iv;
		op.params[1].tmpref.size = iv_len;

		op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,
						 TEEC_MEMREF_TEMP_INPUT,
						 TEEC_NONE, TEEC_NONE);
	} else {
		op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_NONE,
						 TEEC_NONE, TEEC_NONE);
	}

	res = TEEC_InvokeCommand(o->session, TA_CRYPT_CMD_CIPHER_INIT, &op, &ret_orig);
	return res;
}

TEEC_Result ta_crypt_cmd_cipher_update(oc *o, TEE_OperationHandle oph,
					      const void *src, size_t src_len,
					      void *dst, size_t *dst_len)
{
	TEEC_Result res = TEEC_ERROR_GENERIC;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t ret_orig = 0;

	assert((uintptr_t)oph <= UINT32_MAX);
	op.params[0].value.a = (uint32_t)(uintptr_t)oph;

	op.params[1].tmpref.buffer = (void *)src;
	op.params[1].tmpref.size = src_len;

	op.params[2].tmpref.buffer = dst;
	op.params[2].tmpref.size = *dst_len;

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,
					 TEEC_MEMREF_TEMP_INPUT,
					 TEEC_MEMREF_TEMP_OUTPUT, TEEC_NONE);

	res = TEEC_InvokeCommand(o->session, TA_CRYPT_CMD_CIPHER_UPDATE, &op, &ret_orig);
	if (res == TEEC_SUCCESS)
		*dst_len = op.params[2].tmpref.size;

	return res;
}

TEEC_Result ta_crypt_cmd_cipher_do_final(oc *o, TEE_OperationHandle oph,
						const void *src,
						size_t src_len,
						void *dst,
						size_t *dst_len)
{
	TEEC_Result res = TEEC_ERROR_GENERIC;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t ret_orig = 0;

	assert((uintptr_t)oph <= UINT32_MAX);
	op.params[0].value.a = (uint32_t)(uintptr_t)oph;

	op.params[1].tmpref.buffer = (void *)src;
	op.params[1].tmpref.size = src_len;

	op.params[2].tmpref.buffer = (void *)dst;
	op.params[2].tmpref.size = *dst_len;

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,
					 TEEC_MEMREF_TEMP_INPUT,
					 TEEC_MEMREF_TEMP_OUTPUT, TEEC_NONE);

	res = TEEC_InvokeCommand(o->session, TA_CRYPT_CMD_CIPHER_DO_FINAL, &op,
				 &ret_orig);

	if (res == TEEC_SUCCESS)
		*dst_len = op.params[2].tmpref.size;

	return res;
}

TEEC_Result ta_crypt_cmd_random_number_generate(oc *o, void *buf,size_t blen)
{
	TEEC_Result res = TEEC_ERROR_GENERIC;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t ret_orig = 0;

	op.params[0].tmpref.buffer = buf;
	op.params[0].tmpref.size = blen;

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_OUTPUT, TEEC_NONE,
					 TEEC_NONE, TEEC_NONE);

	res = TEEC_InvokeCommand(o->session, TA_CRYPT_CMD_RANDOM_NUMBER_GENEREATE, &op,
				 &ret_orig);
	return res;
}

TEEC_Result ta_crypt_cmd_ae_init(oc *o, TEE_OperationHandle oph,
					const void *nonce, size_t nonce_len,
					size_t tag_len, size_t aad_len,
					size_t payload_len)
{
	TEEC_Result res = TEEC_ERROR_GENERIC;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t ret_orig = 0;

	assert((uintptr_t)oph <= UINT32_MAX);
	op.params[0].value.a = (uint32_t)(uintptr_t)oph;
	op.params[0].value.b = tag_len;

	op.params[1].tmpref.buffer = (void *)nonce;
	op.params[1].tmpref.size = nonce_len;

	op.params[2].value.a = aad_len;
	op.params[2].value.b = payload_len;

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,
					 TEEC_MEMREF_TEMP_INPUT,
					 TEEC_VALUE_INPUT, TEEC_NONE);

	res = TEEC_InvokeCommand(o->session, TA_CRYPT_CMD_AE_INIT, &op, &ret_orig);
	return res;
}

TEEC_Result ta_crypt_cmd_ae_update_aad(oc *o, TEE_OperationHandle oph,
					      const void *aad, size_t aad_len)
{
	TEEC_Result res = TEEC_ERROR_GENERIC;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t ret_orig = 0;

	assert((uintptr_t)oph <= UINT32_MAX);
	op.params[0].value.a = (uint32_t)(uintptr_t)oph;

	op.params[1].tmpref.buffer = (void *)aad;
	op.params[1].tmpref.size = aad_len;

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,
					 TEEC_MEMREF_TEMP_INPUT, TEEC_NONE,
					 TEEC_NONE);

	res = TEEC_InvokeCommand(o->session, TA_CRYPT_CMD_AE_UPDATE_AAD, &op, &ret_orig);
	return res;
}

TEEC_Result ta_crypt_cmd_ae_update(oc *o, TEE_OperationHandle oph,
					  const void *src,
					  size_t src_len,
					  void *dst,
					  size_t *dst_len)
{
	TEEC_Result res = TEEC_ERROR_GENERIC;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t ret_orig = 0;

	assert((uintptr_t)oph <= UINT32_MAX);
	op.params[0].value.a = (uint32_t)(uintptr_t)oph;

	op.params[1].tmpref.buffer = (void *)src;
	op.params[1].tmpref.size = src_len;

	op.params[2].tmpref.buffer = (void *)dst;
	op.params[2].tmpref.size = *dst_len;

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,
					 TEEC_MEMREF_TEMP_INPUT,
					 TEEC_MEMREF_TEMP_OUTPUT, TEEC_NONE);

	res = TEEC_InvokeCommand(o->session, TA_CRYPT_CMD_AE_UPDATE, &op, &ret_orig);

	if (res == TEEC_SUCCESS)
		*dst_len = op.params[2].tmpref.size;

	return res;
}

TEEC_Result ta_crypt_cmd_ae_encrypt_final(oc *o, TEE_OperationHandle oph,
						 const void *src,
						 size_t src_len, void *dst,
						 size_t *dst_len, void *tag,
						 size_t *tag_len)
{
	TEEC_Result res = TEEC_ERROR_GENERIC;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t ret_orig = 0;

	assert((uintptr_t)oph <= UINT32_MAX);
	op.params[0].value.a = (uint32_t)(uintptr_t)oph;

	op.params[1].tmpref.buffer = (void *)src;
	op.params[1].tmpref.size = src_len;

	op.params[2].tmpref.buffer = (void *)dst;
	op.params[2].tmpref.size = *dst_len;

	op.params[3].tmpref.buffer = (void *)tag;
	op.params[3].tmpref.size = *tag_len;

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,
					 TEEC_MEMREF_TEMP_INPUT,
					 TEEC_MEMREF_TEMP_OUTPUT,
					 TEEC_MEMREF_TEMP_OUTPUT);

	res = TEEC_InvokeCommand(o->session, TA_CRYPT_CMD_AE_ENCRYPT_FINAL, &op,
				 &ret_orig);

	if (res == TEEC_SUCCESS) {
		*dst_len = op.params[2].tmpref.size;
		*tag_len = op.params[3].tmpref.size;
	}

	return res;
}

TEEC_Result ta_crypt_cmd_ae_decrypt_final(oc *o, TEE_OperationHandle oph,
						 const void *src, size_t src_len,
						 void *dst, size_t *dst_len,
						 const void *tag, size_t tag_len)
{
	TEEC_Result res = TEEC_ERROR_GENERIC;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t ret_orig = 0;

	assert((uintptr_t)oph <= UINT32_MAX);
	op.params[0].value.a = (uint32_t)(uintptr_t)oph;

	op.params[1].tmpref.buffer = (void *)src;
	op.params[1].tmpref.size = src_len;

	op.params[2].tmpref.buffer = dst;
	op.params[2].tmpref.size = *dst_len;

	op.params[3].tmpref.buffer = (void *)tag;
	op.params[3].tmpref.size = tag_len;

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,
					 TEEC_MEMREF_TEMP_INPUT,
					 TEEC_MEMREF_TEMP_OUTPUT,
					 TEEC_MEMREF_TEMP_INPUT);

	res = TEEC_InvokeCommand(o->session, TA_CRYPT_CMD_AE_DECRYPT_FINAL, &op,
				 &ret_orig);

	if (res == TEEC_SUCCESS)
		*dst_len = op.params[2].tmpref.size;

	return res;
}

TEEC_Result ta_crypt_cmd_asymmetric_operate(oc *o, TEE_OperationHandle oph,
						   uint32_t cmd,
						   const TEE_Attribute *params,
						   uint32_t paramCount,
						   const void *src,
						   size_t src_len,
						   void *dst,
						   size_t *dst_len)
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

	op.params[1].tmpref.buffer = buf;
	op.params[1].tmpref.size = blen;

	op.params[2].tmpref.buffer = (void *)src;
	op.params[2].tmpref.size = src_len;

	op.params[3].tmpref.buffer = dst;
	op.params[3].tmpref.size = *dst_len;

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,
					 TEEC_MEMREF_TEMP_INPUT,
					 TEEC_MEMREF_TEMP_INPUT,
					 TEEC_MEMREF_TEMP_OUTPUT);

	res = TEEC_InvokeCommand(o->session, cmd, &op, &ret_orig);

	if (res == TEEC_SUCCESS)
		*dst_len = op.params[3].tmpref.size;

	free(buf);
	return res;
}

TEEC_Result ta_crypt_cmd_asymmetric_encrypt(oc *o, TEE_OperationHandle oph,
						   const TEE_Attribute *params,
						   uint32_t paramCount,
						   const void *src,
						   size_t src_len,
						   void *dst,
						   size_t *dst_len)
{
	return ta_crypt_cmd_asymmetric_operate(o, oph,
					       TA_CRYPT_CMD_ASYMMETRIC_ENCRYPT,
					       params, paramCount,
					       src, src_len, dst, dst_len);
}

TEEC_Result ta_crypt_cmd_asymmetric_decrypt(oc *o, TEE_OperationHandle oph,
						   const TEE_Attribute *params,
						   uint32_t paramCount,
						   const void *src,
						   size_t src_len,
						   void *dst,
						   size_t *dst_len)
{
	return ta_crypt_cmd_asymmetric_operate(o, oph,
					       TA_CRYPT_CMD_ASYMMETRIC_DECRYPT,
					       params, paramCount,
					       src, src_len, dst, dst_len);
}

TEEC_Result ta_crypt_cmd_asymmetric_sign(oc *o, TEE_OperationHandle oph,
						const TEE_Attribute *params,
						uint32_t paramCount,
						const void *digest,
						size_t digest_len,
						void *signature,
						size_t *signature_len)
{
	return ta_crypt_cmd_asymmetric_operate(o, oph,
			TA_CRYPT_CMD_ASYMMETRIC_SIGN_DIGEST, params, paramCount,
			digest, digest_len, signature, signature_len);
}

TEEC_Result ta_crypt_cmd_asymmetric_verify(oc *o, TEE_OperationHandle oph,
						  const TEE_Attribute *params,
						  uint32_t paramCount,
						  const void *digest,
						  size_t digest_len,
						  const void *signature,
						  size_t signature_len)
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

	op.params[1].tmpref.buffer = buf;
	op.params[1].tmpref.size = blen;

	op.params[2].tmpref.buffer = (void *)digest;
	op.params[2].tmpref.size = digest_len;

	op.params[3].tmpref.buffer = (void *)signature;
	op.params[3].tmpref.size = signature_len;

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,
					 TEEC_MEMREF_TEMP_INPUT,
					 TEEC_MEMREF_TEMP_INPUT,
					 TEEC_MEMREF_TEMP_INPUT);

	res = TEEC_InvokeCommand(o->session, TA_CRYPT_CMD_ASYMMETRIC_VERIFY_DIGEST,
				 &op, &ret_orig);

	free(buf);
	return res;
}

TEEC_Result ta_crypt_cmd_get_object_value_attribute(oc *o, TEE_ObjectHandle oh,
							   uint32_t attr_id,
							   uint32_t *valuea,
							   uint32_t *valueb)
{
	TEEC_Result res = TEEC_ERROR_GENERIC;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t ret_orig = 0;

	assert((uintptr_t)oh <= UINT32_MAX);
	op.params[0].value.a = (uint32_t)(uintptr_t)oh;
	op.params[0].value.b = attr_id;
	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_VALUE_OUTPUT,
					 TEEC_NONE, TEEC_NONE);

	res = TEEC_InvokeCommand(o->session, TA_CRYPT_CMD_GET_OBJECT_VALUE_ATTRIBUTE,
				 &op, &ret_orig);

	if (res == TEEC_SUCCESS) {
		*valuea = op.params[1].value.a;
		*valueb = op.params[1].value.b;
	}

	return res;
}

TEEC_Result ta_crypt_cmd_generate_key(oc *o, TEE_ObjectHandle oh,
					     uint32_t key_size,
					     const TEE_Attribute *params,
					     uint32_t paramCount)
{
	TEEC_Result res = TEEC_ERROR_GENERIC;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t ret_orig = 0;
	uint8_t *buf = NULL;
	size_t blen = 0;

	res = pack_attrs(params, paramCount, &buf, &blen);
	if (res!=TEEC_SUCCESS)
		return res;

	assert((uintptr_t)oh <= UINT32_MAX);
	op.params[0].value.a = (uint32_t)(uintptr_t)oh;
	op.params[0].value.b = key_size;

	op.params[1].tmpref.buffer = buf;
	op.params[1].tmpref.size = blen;

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,
					 TEEC_MEMREF_TEMP_INPUT, TEEC_NONE,
					 TEEC_NONE);

	res = TEEC_InvokeCommand(o->session, TA_CRYPT_CMD_GENERATE_KEY, &op, &ret_orig);

	free(buf);
	return res;
}

