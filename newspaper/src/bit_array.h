#ifndef __BIT_ARRAY_H
#define __BIT_ARRAY_H

int bitarray_copy(
	const unsigned char *src_org, 
	int src_offset, 
	int src_len, 
	unsigned char *dst_org, 
	int dst_offset
);

#endif
