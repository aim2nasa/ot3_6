#ifndef __CRYPT_H__
#define __CRYPT_H__

#include <tee_client_api.h>
#include <session.h>
#include <ta_crypt.h>
#include <tee_api_types.h>

#ifdef __cplusplus
extern "C" {
#endif
	TEEC_Result ta_crypt_cmd_sha256(oc *o,
					const void *src, size_t src_len,void *dst, size_t *dst_len);
	TEEC_Result ta_crypt_cmd_aes256ecb_enc(oc *o,
					const void *src, size_t src_len,void *dst, size_t *dst_len);
	TEEC_Result ta_crypt_cmd_aes256ecb_dec(oc *o,
					const void *src, size_t src_len,void *dst, size_t *dst_len);

	TEEC_Result ta_crypt_cmd_reset_operation(oc *o,TEE_OperationHandle oph);
	TEEC_Result ta_crypt_cmd_copy_operation(oc *o,TEE_OperationHandle dst_oph,
					TEE_OperationHandle src_oph);
	TEEC_Result ta_crypt_cmd_digest_update(oc *o,TEE_OperationHandle oph,
					const void *chunk,size_t chunk_size);
	TEEC_Result ta_crypt_cmd_digest_do_final(oc *o,TEE_OperationHandle oph,
					const void *chunk,size_t chunk_len, void *hash,size_t *hash_len);
	TEE_Result ta_crypt_cmd_set_operation_key2(oc *o,TEE_OperationHandle oph,
					TEE_ObjectHandle key1,TEE_ObjectHandle key2);
	TEEC_Result ta_crypt_cmd_mac_init(oc *o,TEE_OperationHandle oph,
					const void *iv, size_t iv_len);
	TEEC_Result ta_crypt_cmd_mac_update(oc *o,TEE_OperationHandle oph,
					const void *chunk, size_t chunk_size);
	TEEC_Result ta_crypt_cmd_mac_final_compute(oc *o,TEE_OperationHandle oph,
					const void *chunk,size_t chunk_len,void *hash,size_t *hash_len);
	TEEC_Result ta_crypt_cmd_cipher_init(oc *o,TEE_OperationHandle oph,
					const void *iv, size_t iv_len);
	TEEC_Result ta_crypt_cmd_cipher_update(oc *o,TEE_OperationHandle oph,
					const void *src, size_t src_len,void *dst, size_t *dst_len);
	TEEC_Result ta_crypt_cmd_cipher_do_final(oc *o,TEE_OperationHandle oph,
					const void *src,size_t src_len,void *dst,size_t *dst_len);
	TEEC_Result ta_crypt_cmd_random_number_generate(oc *o,void *buf,size_t blen);
	TEEC_Result ta_crypt_cmd_ae_init(oc *o,TEE_OperationHandle oph,
					const void *nonce, size_t nonce_len,size_t tag_len, size_t aad_len,size_t payload_len);
	TEEC_Result ta_crypt_cmd_ae_update_aad(oc *o,TEE_OperationHandle oph,
					const void *aad, size_t aad_len);
	TEEC_Result ta_crypt_cmd_ae_update(oc *o,TEE_OperationHandle oph,
					const void *src,size_t src_len,void *dst,size_t *dst_len);
	TEEC_Result ta_crypt_cmd_ae_encrypt_final(oc *o,TEE_OperationHandle oph,
					const void *src,size_t src_len, void *dst,size_t *dst_len, void *tag,size_t *tag_len);
	TEEC_Result ta_crypt_cmd_ae_decrypt_final(oc *o,TEE_OperationHandle oph,
					const void *src, size_t src_len,void *dst, size_t *dst_len,const void *tag, size_t tag_len);
	TEEC_Result ta_crypt_cmd_asymmetric_operate(oc *o,TEE_OperationHandle oph,
					uint32_t cmd,const TEE_Attribute *params,uint32_t paramCount,const void *src,size_t src_len,
					void *dst,size_t *dst_len);
	TEEC_Result ta_crypt_cmd_asymmetric_encrypt(oc *o,TEE_OperationHandle oph,
					const TEE_Attribute *params,uint32_t paramCount,const void *src,size_t src_len,
					void *dst,size_t *dst_len);
	TEEC_Result ta_crypt_cmd_asymmetric_decrypt(oc *o,TEE_OperationHandle oph,
					const TEE_Attribute *params,uint32_t paramCount,const void *src,size_t src_len,
					void *dst,size_t *dst_len);
	TEEC_Result ta_crypt_cmd_asymmetric_sign(oc *o,TEE_OperationHandle oph,
					const TEE_Attribute *params,uint32_t paramCount,const void *digest,size_t digest_len,
					void *signature,size_t *signature_len);
	TEEC_Result ta_crypt_cmd_asymmetric_verify(oc *o,TEE_OperationHandle oph,
					const TEE_Attribute *params,uint32_t paramCount,const void *digest,size_t digest_len,
					const void *signature,size_t signature_len);
	TEEC_Result ta_crypt_cmd_get_object_value_attribute(oc *o,TEE_ObjectHandle oh,
					uint32_t attr_id,uint32_t *valuea,uint32_t *valueb);
	TEEC_Result ta_crypt_cmd_generate_key(oc *o,TEE_ObjectHandle oh,
					uint32_t key_size,const TEE_Attribute *params,uint32_t paramCount);

#ifdef __cplusplus
}
#endif

#endif
