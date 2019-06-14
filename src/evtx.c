#include <inttypes.h>
#include <stdlib.h>
#include <string.h>	// do I really need this?

#include "evtx.h"

#ifdef DEBUG
#define DBG(format, ...)	fprintf(stderr, "%s(): %d: "format, __func__, __LINE__, ## __VA_ARGS__)
#else
#define DBG(format, ...)
#endif // DEBUG


const char evtx_header_magic[8] =	"ElfFile\0";
const char evtx_chunk_magic[8] = 	"ElfChnk\0";
const char evtx_record_magic[4] =	"**\0\0";


// private
#include <stdio.h>
static inline int get_record_length2(PEvtxRecord record)
{
	return *(uint32_t*)(((char*)record)+(record->length)-sizeof(record->length));
}

static int is_record_valid(PEvtxRecord record)
{
	if (memcmp(record->magic, evtx_record_magic, sizeof(record->magic)))
		return 0;
	else if(record->length != get_record_length2(record))
		return 0;
	else
		return 1;
}


// public
int build_evtx_header(const char *log, PEvtxHeader header)
{
	memcpy(header, log, EVTX_HEADER_SIZE);
	if(memcmp(header->magic, evtx_header_magic, sizeof(header->magic)))
		return EXIT_FAILURE;
	else
		return EXIT_SUCCESS;
}

int build_evtx_chunk(const char *addr, PEvtxChunk chunk)
{
	memcpy(chunk, addr, EVTX_CHUNK_HEADER_SIZE);//sizeof(EvtxChunk));
	if(memcmp(chunk->magic, evtx_chunk_magic, sizeof(chunk->magic)))
		return EXIT_FAILURE;
	else
		return EXIT_SUCCESS;
}

int build_evtx_record(const char *addr, PEvtxRecord record, size_t size)
{
	int length = ((PEvtxRecord)addr)->length;
	if (length <= size) {
		memcpy(record, addr, length);
		if(!is_record_valid(record))
			length = -1;
	}
	return length;
}

const char *chunk_addr(const char *log, size_t index)
{
	return log + EVTX_CHUNK_OFFSET + index*EVTX_CHUNK_SIZE;
}

const char *record_addr(PEvtxChunk chunk, size_t index);
