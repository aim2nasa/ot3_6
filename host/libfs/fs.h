#ifndef __FS_H__
#define __FS_H__

#include <tee_client_api.h>

#ifdef __cplusplus
extern "C" {
#endif

	typedef struct _optee_fs_context {
		TEEC_Context *ctx;
		TEEC_Session *session;
		uint32_t error;
	} fs;


	TEEC_Result initializeContext(const char *name,fs *f);
	TEEC_Result openSession(fs *f,uint32_t connectionMethod,
						const void *connectionData,TEEC_Operation *operation);


	TEEC_Result fs_open(fs *f, void *id, uint32_t id_size,
						uint32_t flags, uint32_t *obj, uint32_t storage_id);
	TEEC_Result fs_create(fs *f, void *id, uint32_t id_size,
						uint32_t flags, uint32_t attr, void *data,
						uint32_t data_size, uint32_t *obj,
						uint32_t storage_id);
	TEEC_Result fs_create_overwrite(fs *f, void *id,uint32_t id_size, uint32_t storage_id);
	TEEC_Result fs_close(fs *f, uint32_t obj);
	TEEC_Result fs_read(fs *f, uint32_t obj, void *data,uint32_t data_size, uint32_t *count);
	TEEC_Result fs_write(fs *f, uint32_t obj, void *data,uint32_t data_size);
	TEEC_Result fs_seek(fs *f, uint32_t obj, int32_t offset,int32_t whence);
	TEEC_Result fs_unlink(fs *f, uint32_t obj);
	TEEC_Result fs_trunc(fs *f, uint32_t obj, uint32_t len);
	TEEC_Result fs_rename(fs *f, uint32_t obj, void *id,uint32_t id_size);
	TEEC_Result fs_alloc_enum(fs *f, uint32_t *e);
	TEEC_Result fs_reset_enum(fs *f, uint32_t e);
	TEEC_Result fs_free_enum(fs *f, uint32_t e);
	TEEC_Result fs_start_enum(fs *f, uint32_t e,uint32_t storage_id);
	TEEC_Result fs_next_enum(fs *f, uint32_t e, void *obj_info,
						size_t info_size, void *id, uint32_t id_size);
	TEEC_Result fs_restrict_usage(fs *f, uint32_t obj,uint32_t obj_usage);
	TEEC_Result fs_alloc_obj(fs *f, uint32_t obj_type,
						uint32_t max_key_size, uint32_t *obj);
	TEEC_Result fs_free_obj(fs *f, uint32_t obj);
	TEEC_Result fs_reset_obj(fs *f, uint32_t obj);
	TEEC_Result fs_get_obj_info(fs *f, uint32_t obj,
						void *obj_info, size_t info_size);

	void closeSession(fs *f);
	void finalizeContext(fs *f);

#ifdef __cplusplus
}
#endif

#endif
