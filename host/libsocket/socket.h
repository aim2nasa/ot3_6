#ifndef __SOCKET_H__
#define __SOCKET_H__

#include <tee_client_api.h>
#include <session.h>
#include <ta_socket.h>
#include <tee_api_types.h>

#ifdef __cplusplus
extern "C" {
#endif

	struct socket_handle {
		uint64_t buf[2];
		size_t blen;
	};

	TEE_Result socket_tcp_open(oc *o, uint32_t ip_vers,
					  const char *addr, uint16_t port,
					  struct socket_handle *handle,
					  uint32_t *error, uint32_t *ret_orig);
	TEE_Result socket_udp_open(oc *o, uint32_t ip_vers,
					  const char *addr, uint16_t port,
					  struct socket_handle *handle,
					  uint32_t *error, uint32_t *ret_orig);
	TEE_Result socket_send(oc *o,
				      struct socket_handle *handle,
				      const void *data, size_t *dlen,
				      uint32_t timeout, uint32_t *ret_orig);
	TEE_Result socket_recv(oc *o,
				      struct socket_handle *handle,
				      void *data, size_t *dlen,
				      uint32_t timeout, uint32_t *ret_orig);
	TEE_Result socket_get_error(oc *o,
				      struct socket_handle *handle,
				      uint32_t *proto_error, uint32_t *ret_orig);
	TEE_Result socket_close(oc *o,
				      struct socket_handle *handle, uint32_t *ret_orig);
	TEE_Result socket_ioctl(oc *o,
				      struct socket_handle *handle, uint32_t ioctl_cmd,
				      void *data, size_t *dlen, uint32_t *ret_orig);

#ifdef __cplusplus
}
#endif

#endif
