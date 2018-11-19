#include "evt.h"

/*
typedef struct _EVENTLOG {
	PEvtxHeader	Header;
	PEvtxChunk	CurrentChunk;
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
*/
