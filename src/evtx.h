/*

I'd like to give credit and thanks to the following source(s) for helping me
decode evtx file into its representative structs
header:	http://computer.forensikblog.de/en/2007/07/evtx-file-header.html
chunk:	http://computer.forensikblog.de/en/2007/07/evtx-chunk-header.html
record:	http://computer.forensikblog.de/en/2007/07/evtx-event-record.html

*/

#ifndef EVTX_H_INCLUDED
#define EVTX_H_INCLUDED

#include <stddef.h>	// for size_t
#include <stdint.h>	// for uint16_t and uint32_t for Win WORD and DWORD types

//#define EVTX_HEADER_MAGIC	"ElfFile\0"	//0x456c6646696c6500	// translates as "ElfFile\0x00"
const char evtx_header_magic[8];
//#define EVTX_CHUNK_MAGIC	0x456c6643686e6b00	// translates as "ElfChnk\0x00"
const char evtx_chunk_magic[8];
//#define EVTX_RECORD_MAGIC	0x2a2a0000
const char evtx_record_magic[4];


#define EVTX_HEADER_SIZE	1024	// bytes size of header
#define EVTX_CHUNK_OFFSET 4096	// the offset of the first chunk
#define EVTX_CHUNK_HEADER_SIZE	512
#define EVTX_CHUNK_SIZE	65536	// bytes size of chunks, though not all is used

/*
time datatype used by Windows...
needed for a struct definition, so not externally visible, and since value
is pulled from a file, no imports would be warranted.
*/
#ifndef _FILETIME_
typedef struct _FILETIME {
	uint32_t dwLowDateTime;
	uint32_t dwHighDateTime;
} FILETIME,*PFILETIME,*LPFILETIME;
#define _FILETIME_
#endif // _FILETIME_

typedef struct _EvtxHeader {
	char		magic[8];	// valid evtx headers should have evtx_header_magic
	int64_t		oldest_chunk;
	int64_t		current_chunk;
	int64_t		next_record;
	uint32_t	length;	// always 168
	uint16_t	major_version;	// always 1(?)
	uint16_t	minor_version;	// always 3(?)
	uint16_t	size;	// always 0x1000
	uint16_t	chunk_count;
	char		unknown[76];	// Unknown usage bytes [0x2x, 0x78)
	uint32_t	flags;
	uint32_t	checksum;	// CRC32 of first 120 bytes
} EvtxHeader,*PEvtxHeader;

typedef struct _EvtxChunk {
	char		magic[8];		// valid evtx chunks should have evtx_chunk_magic
	int64_t		first_log_record;
	int64_t		last_log_record;
	int64_t		first_file_record;	// will differ if log spans multiple files
	int64_t		last_file_record;
	uint32_t	string_table_offset;		// always 0x080; StringTable offset?
	uint32_t	last_record_offset;
	uint32_t	free_space_offset;
	uint32_t	data_checksum;	// CRC32 of record data
	char		unknown[68];	// unknown usage bytes [0x038, 0x07c)
	uint32_t	checksum;	// CRC32 of bytes 0-119 and 128-511
	uint32_t	string_table[64];
	uint32_t	template_table[32];
	char		records[65024];	// 64 kB/chunk * 1024 B/kB - 0x200
} EvtxChunk,*PEvtxChunk;

//	This struct should only be used as a pointer or as dynamically allocated
//	memory due to the need for additional storage space after the last member
typedef struct _EvtxRecord {
	char		magic[4];	// valid evtx records should have evtx_record_magic
	uint32_t	length;
	int64_t		id;
	FILETIME	time;
	/*
		// undefined members
	BYTE		BinXmlStream[];
	uint32_t	Length;	// duplicate of Length
	*/
} EvtxRecord,*PEvtxRecord;


int build_evtx_header(const char *log, PEvtxHeader header);
int build_evtx_chunk(const char *addr, PEvtxChunk chunk);
// if ((PEvtxRecord)addr)->Length is <= size, build record. Return ->Length
int build_evtx_record(const char *addr, PEvtxRecord record, size_t size);

const char *chunk_addr(const char *log, size_t index);
//const char *record_addr(PEvtxChunk chunk, size_t index);

#endif // EVTX_H_INCLUDED
