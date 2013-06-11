#ifndef __BIT_ARRAY_H
#define __BIT_ARRAY_H
#include "../config.h"

int bitarray_copy(
	const unsigned char *src_org,
	int src_offset,
	int src_len,
	unsigned char *dst_org,
	int dst_offset
);

#endif

/* vim: set expandtab ai ts=4 sw=4 nu:
*/
