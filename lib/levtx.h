/*

A set of functions for interacting with Windows evtx format event log files.

In order for the behavior to be as expected with anyone having used the Windows
evtx API, I will have the functions return the same values and take the same
inputs.

Unfortunately, functions returning int values will be returning non-zero on
success instead of the standard 0, and the inverse on failure, as this is the
way Microsoft implemented the Windows versions of these functions.

As I don't want to make this needlessly complicated by implementing the system
of error numbers and the relevant getters and setters Windows uses, I will set
errno from <errno.h> on error so that the normal strerror can be used to
retrieve a meaningful string rendition of the error.

Champagne Lewis - champagne.t.lewis@gmail.com

*/


#ifndef LEVTX_H_INCLUDED
#define LEVTX_H_INCLUDED


#include <stdint.h>	// for uint16_t and uint32_t for Win WORD and DWORD types


// I need to look up what exactly M$ says this value is.
// It is found in the header and every entry in a given event log
#ifndef ELF_LOG_SIGNATURE
#define ELF_LOG_SIGNATURE	0x654c664c			// "eLfL"; can't find in evtx files...
	// this is in little endian format, whereas it would be big endian if in file?
#endif // ELF_LOG_SIGNATURE

// readflags values to OR together and pass to ReadEventLog()
#define EVENTLOG_SEEK_READ			0b00	// 0x0002 in Win; read from offset
#define EVENTLOG_SEQUENTIAL_READ	0b01	// 0x0001 in Win; read from either end
#define EVENTLOG_FORWARDS_READ		0b00	// 0x0004 in Win; read forwards IF seq
#define EVENTLOG_BACKWARDS_READ		0b10	// 0x0008 in Win; read backwards IF seq

// EVENGLOGRECORD.EventType values
#define EVENTLOG_SUCCESS			0x0000
#define EVENTLOG_ERROR_TYPE			0x0001
#define EVENTLOG_WARNING_TYPE		0x0002
#define EVENTLOG_INFORMATION_TYPE	0x0004
#define EVENTLOG_AUDIT_SUCCESS		0x0008
#define EVENTLOG_AUDIT_FAILURE		0x0010


// using the type name used by Windows for these functions
// for levtx, this will likely point to a struct holding a representation
// of the XML interpreted from the evtx file
#ifndef _HANDLE_DEFINED
#define _HANDLE_DEFINED
typedef void *HANDLE;
#endif // _HANDLE_DEFINED


typedef struct _EVENTLOGRECORD {
	uint32_t Length;	// total length of struct, INCLUDING undefined members
	uint32_t Reserved;	// always set to ELF_LOG_SIGNATURE
	uint32_t RecordNumber;	// index of the record, used for seek reading
	uint32_t TimeGenerated;	// when entry was submitted
	uint32_t TimeWritten;	// when entry was received by the service
	uint32_t EventID;	// UID defined by source; used to find event message
	uint16_t EventType;	// an int defining the event type
	uint16_t NumStrings;	// number of strings found at StringOffset
	uint16_t EventCategory;	// category of event; meaning depends on source...
	uint16_t ReservedFlags;	// Reserved (?)
	uint32_t ClosingRecordNumber;	// Reserved (?)
	uint32_t StringOffset;	// offset of the description strings within record
	uint32_t UserSidLength;	// size of UserSid member in bytes; can be 0
	uint32_t UserSidOffset;	// offset of the UserSID within record
	uint32_t DataLength;	// size of event-specific data at DataOffset
	uint32_t DataOffset;	// offset of the Data member within record
	/*
	// undefined "members", for the sake of reference
	wchar_t SourceName[];	// variable-length string that specifies the name of the source
	wchar_t ComputerName[];	// name of the computer that generated event; may include padding to align UserSID
	SID UserSid;	// IDs active user at time event was logged; may be empty if length is 0
		// I WILL NOT implement SID here. This can be done elsewhere...
	wchar_t Strings[];
	unsigned char Data[];
	char Pad[];	// aligns Length on DWORD boundery, I guess
	uint32_t Length;	// duplicate of above length
	*/
} EVENTLOGRECORD,*PEVENTLOGRECORD;


// log file access

// this function will try to find events logs in %windir%/System32/winevt/Logs
// of the server specified, tries to find a local Windows system directory
HANDLE OpenEventLog(	// https://msdn.microsoft.com/en-us/library/aa363672
	const char *servername,	// name of server. If NULL, local computer is used
	const char *logname	// name of log file; if not found, use Application log
	);

// this function will behave like a normal file open command, looking for file
// "filename" on the specified server, else on the current machine
HANDLE OpenBackupEventLog(	// https://msdn.microsoft.com/en-us/library/aa363671
	const char *servername,	// same as OpenEventLog
	const char *filename	// similar to OpenEventLog, except full file path
	);

int CloseEventLog(	// https://msdn.microsoft.com/en-us/library/aa363639
	HANDLE log	// handle to event log to be closed
	);

int BackupEventLog(	// https://msdn.microsoft.com/en-us/library/aa363635
	const HANDLE  log,	// handle to open event log
	const char *filename	// absolute or relative path to backup file
	);


// log event access

int ReadEventLog(	// https://msdn.microsoft.com/en-us/library/aa363674
	const HANDLE log,	// handle to log to be read
	const int readflags,	// how to read events from log
	const size_t offset,	// event index to start reading at if seek read
	void *buf,	// receives >=1 EVENTLOGRECORD structs; can't be NULL, per M$
	const size_t buf_size,	// bytes size of buff; <=0x7ffff, per M$
	size_t *bytes_read,	// stores the number of bytes stored in buf
	size_t *bytes_needed	// bytes needed if buf is too small
		// in M$, this is valid on return 0 and error ERROR_INSUFFICIENT_BUFFER
		// I will update this with appropriate errno value at some point...
	);

#endif // LEVTX_H_INCLUDED
