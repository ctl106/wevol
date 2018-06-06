/*

I'd like to give credit and thanks to the following source(s) for helping me
decode evtx file into its representative structs
header:	http://computer.forensikblog.de/en/2007/07/evtx-file-header.html
chunk:	http://computer.forensikblog.de/en/2007/07/evtx-chunk-header.html
record:	http://computer.forensikblog.de/en/2007/07/evtx-event-record.html

*/

#include <string.h>

#include "levtx.h"


#define HEADER_SIZE	4096	// bytes size of header, though not all is used
#define CHUNK_SIZE	65536	// bytes size of chunks, though not all is used

#define EVTX_HEADER_MAGIC	0x456c6646696c6500	// translates as "ElfFile\0x00"
#define EVTX_CHUNK_MAGIC	0x456c6643686e6b00	// translates as "ElfChnk\0x00"

#define SWAP_ENDIAN_16(x)	\
(							\
	((x>>8) & 0x00ff)	|	\
	((x<<8) & 0xff00)		\
)

#define SWAP_ENDIAN_32(x)		\
(								\
	((x>>24) & 0x000000ff)	|	\
	((x>>8) & 0x0000ff00)	|	\
	((x<<8) & 0x00ff0000)		|	\
	((x<<24) & 0xff000000)		\
)


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

// just to make my "unsigned char" lines shorter...
#ifndef _BYTE_
typedef unsigned char BYTE;
typedef BYTE *PBYTE;
#define _BYTE_
#endif // _BYTE_

typedef struct _LogFileHeader {
	char 		Magic[8];	// valid evtx headers should have EVTX_HEADER_MAGIC
	int64_t		OldestChunk;
	int64_t		CurrentChunkNum;
	int64_t		NextRecordNum;
	uint32_t	HeaderPart1Len;	// always 0x80 in current evtx version
	uint16_t	MinorVersion;	// always 1 (why?)
	uint16_t	MajorVersion;	// always 3 (why?)
	uint16_t	HeaderSize;		// always 0x1000 in current evtx version
	uint16_t	ChunkCount;
	char		unknown[76];	// Unknown usage bytes [0x2x, 0x78)
	uint32_t	Flags;
	uint32_t	Checksum;
} LogFileHeader,*PLogFileHeader;

typedef struct _LogFileChunk {
	char		Magic[8];		// valid evtx chunks should have EVTX_CHUNK_MAGIC
	int64_t		NumLogRecFirst;
	int64_t		NumLogRecLast;
	int64_t		NumFileRecFirst;
	int64_t		NumFileRecLast;
	uint32_t	OfsTables;		// always 0x080; StringTable offset?
	uint32_t	OfsRecLast;
	uint32_t	OfsRecNext;
	uint32_t	DataCRC;
	char		unknown[68];	// unknown usage bytes [0x038, 0x07c)
	uint32_t	HeaderCRC;
	uint32_t	StringTable[64];
	uint32_t	TemplateTable[32];
	BYTE		records[65024];	// 64 kB/chunk * 1024 B/kB - 0x200
} LogFileChunk,*PLogFileChunk;

//	This struct should only be used as a pointer or as dynamically allocated
//	memory due to the need for additional storage space after the last member
typedef struct _LogFileRecord {
	char		Magic[4];	// one of the following: 0x2a2a0000
	uint32_t	Length;
	int64_t		NumLogRecord;
	FILETIME	TimeCreated;
	/*
		// undefined members
	BYTE		BinXmlStream[];
	uint32_t	Length;	// duplicate of Length
	*/
} LogFileRecord,*PLogFileRecord;

typedef struct _EVENTLOG {
	PLogFileHeader	Header;
	PLogFileChunk	CurrentChunk;
	size_t			Offset;	// offset from Records of current record being read
	size_t			RecordsLength;	// how long the Records member is
	BYTE			*Records;	// an buffer of EVENTLOGRECORD records
} EVENTLOG,*PEVENTLOG;


// forward declare private "methods" for interacting with EVENTLOG instances

// returns the size of current record and copies the record into buf IF it fits
static size_t copy_current_record(PEVENTLOG log, PEVENTLOGRECORD buf, size_t size);
// sets Offset to 0 and returns 0 if log contains any records, else non-0
static int find_first_record(PEVENTLOG log);
// modifies Offset and returns 0 if log contains any records, else non-0
static int find_last_record(PEVENTLOG log);
// modifies Offset and returns 0 if another was found, else non-0 if at end
static int find_next_record(PEVENTLOG log);
// modifies Offset and returns 0 if another was found, else non-0 if at start
static int find_prev_record(PEVENTLOG log);
// takes a pointer to memory and offset, then returns current + leading length
static inline size_t add_leading_length(PBYTE mem, size_t current)
{
	return current + ((PEVENTLOGRECORD)(mem+current))->Length;
}
// takes a pointer to memory and offset, then returns current - trailing length
static inline size_t sub_trailing_length(PBYTE mem, size_t current)
{
	return current - *(uint32_t*)(mem + current - sizeof(uint32_t));
}


// binary file parsing function

static void build_header(PBYTE log, PLogFileHeader header);
static void build_chunk(PBYTE addr, PLogFileChunk chunk);
// if ((PLogFileRecord)addr)->Length is <= size, build record. Return ->Length
static size_t build_record(PBYTE addr, PLogFileRecord record, size_t size);
static PBYTE chunk_addr(PBYTE log, size_t index);
static PBYTE record_addr(PLogFileChunk chunk, size_t index);


// external API method definitions

HANDLE OpenBackupEventLog(const char *servername, const char *filename)
{
		return NULL;
}

int ReadEventLog(
	const HANDLE log,
	const int readflags,
	const size_t offset,
	void *buf,
	const size_t buf_size,
	size_t *bytes_read,
	size_t *bytes_needed
	)
{
	/*
	when setting bytes_needed, set errno to something related to
	M$'s ERROR_INSUFFICIENT_BUFFER IF unsuccessful execution
	*/
	return 1;
}

// definitions of private EVENTLOG methods

// returns the size of current record and copies the record into buf IF it fits
static size_t copy_current_record(PEVENTLOG log, PEVENTLOGRECORD buf, size_t size)
{
	PEVENTLOGRECORD record = (PEVENTLOGRECORD)(log->Records+log->Offset);
	if(record->Length <= size)
		memcpy(buf, record, record->Length);
	return record->Length;
};

// sets Offset to 0 and returns 0 if log contains any records, else non-0
static int find_first_record(PEVENTLOG log)
{
	if (log->RecordsLength > 0) {
		log->Offset = 0;
		return 0;
	} else
		return 1;
}

// modifies Offset and returns 0 if log contains any records, else non-0
static int find_last_record(PEVENTLOG log)
{
	if(log->RecordsLength > 0) {
		log->Offset = sub_trailing_length(log->Records, log->RecordsLength);
		return 0;
	} else
		return 1;
}

// modifies Offset and returns 0 if a next was found, else non-0 if at end
static int find_next_record(PEVENTLOG log)
{
	size_t next_offset = add_leading_length(log->Records, log->Offset);
	if (next_offset < log->RecordsLength) {
		log->Offset = next_offset;
		return 0;
	} else
		return 1;
}

// modifies Offset and returns 0 if a previous was found,else non-0 if at start
static int find_prev_record(PEVENTLOG log)
{
	if(log->Offset > 0) {
		log->Offset = sub_trailing_length(log->Records, log->Offset);
		return 0;
	} else
		return 1;
}


// binary file parsing function

static void build_header(PBYTE log, PLogFileHeader header)
{
	memcpy(header, log, sizeof(LogFileHeader));
}

static void build_chunk(PBYTE addr, PLogFileChunk chunk)
{
	memcpy(chunk, addr, sizeof(LogFileChunk));
}

static size_t build_record(PBYTE addr, PLogFileRecord record, size_t size)
{
	size_t length = (size_t)SWAP_ENDIAN_32(((PLogFileRecord)addr)->Length);
	if (length <= size)
		memcpy(record, addr, length);
	return length;
}

static PBYTE chunk_addr(PBYTE log, size_t index)
{
	return log + sizeof(LogFileHeader) + index*sizeof(LogFileChunk);
}

static PBYTE record_addr(PLogFileChunk chunk, size_t index);

