#ifndef __HELPERS_H__
#define __HELPERS_H__

#include <tee_client_api.h>
#include <tee_api_types.h>

struct tee_attr_packed {
	uint32_t attr_id;
	uint32_t a;
	uint32_t b;
};

TEE_Result pack_attrs(const TEE_Attribute *attrs, uint32_t attr_count,
		      uint8_t **buf, size_t *blen);

#endif
