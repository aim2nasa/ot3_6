#ifndef __FS_H__
#define __FS_H__

#include <tee_client_api.h>
#include <session.h>
#include <ta_storage.h>

#ifdef __cplusplus
extern "C" {
#endif

	TEEC_Result fs_open(oc *o, void *id, uint32_t id_size,
						uint32_t flags, uint32_t *obj, uint32_t storage_id);
	TEEC_Result fs_create(oc *o, void *id, uint32_t id_size,
						uint32_t flags, uint32_t attr, void *data,
						uint32_t data_size, uint32_t *obj,
						uint32_t storage_id);
	TEEC_Result fs_create_overwrite(oc *o, void *id,uint32_t id_size, uint32_t storage_id);
	TEEC_Result fs_close(oc *o, uint32_t obj);
	TEEC_Result fs_read(oc *o, uint32_t obj, void *data,uint32_t data_size, uint32_t *count);
	TEEC_Result fs_write(oc *o, uint32_t obj, void *data,uint32_t data_size);
	TEEC_Result fs_seek(oc *o, uint32_t obj, int32_t offset,int32_t whence);
	TEEC_Result fs_unlink(oc *o, uint32_t obj);
	TEEC_Result fs_trunc(oc *o, uint32_t obj, uint32_t len);
	TEEC_Result fs_rename(oc *o, uint32_t obj, void *id,uint32_t id_size);
	TEEC_Result fs_alloc_enum(oc *o, uint32_t *e);
	TEEC_Result fs_reset_enum(oc *o, uint32_t e);
	TEEC_Result fs_free_enum(oc *o, uint32_t e);
	TEEC_Result fs_start_enum(oc *o, uint32_t e,uint32_t storage_id);
	TEEC_Result fs_next_enum(oc *o, uint32_t e, void *obj_info,
						size_t info_size, void *id, uint32_t id_size);
	TEEC_Result fs_restrict_usage(oc *o, uint32_t obj,uint32_t obj_usage);
	TEEC_Result fs_alloc_obj(oc *o, uint32_t obj_type,
						uint32_t max_key_size, uint32_t *obj);
	TEEC_Result fs_free_obj(oc *o, uint32_t obj);
	TEEC_Result fs_reset_obj(oc *o, uint32_t obj);
	TEEC_Result fs_get_obj_info(oc *o, uint32_t obj,
						void *obj_info, size_t info_size);

#ifdef __cplusplus
}
#endif

#endif
