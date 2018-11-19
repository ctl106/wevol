#ifndef WINLOG_H_INCLUDED
#define WINLOG_H_INCLUDED

#include <stddef.h>	// for size_t
#include <stdint.h>	// for uint16_t and uint32_t for Win WORD and DWORD types

#include "evtx.h"


// I need to look up what exactly M$ says this value is.
// It is found in the header and every entry in a given event log
#ifndef ELF_LOG_SIGNATURE
#define ELF_LOG_SIGNATURE	0x654c664c			// "eLfL"; can't find in evtx files...
	// this is in little endian format, whereas it would be big endian if in file?
#endif // ELF_LOG_SIGNATURE

// readflags values to OR together and pass to ReadEventLog()
#define EVENTLOG_SEQUENTIAL_READ	0b00	// 0x0001 in Win; read from either end
#define EVENTLOG_SEEK_READ			0b01	// 0x0002 in Win; read from offset
#define EVENTLOG_FORWARDS_READ		0b00	// 0x0004 in Win; read forwards IF seq
#define EVENTLOG_BACKWARDS_READ		0b10	// 0x0008 in Win; read backwards IF seq

// EVENGLOGRECORD.EventType values
#define EVENTLOG_SUCCESS			0x0000
#define EVENTLOG_ERROR_TYPE			0x0001
#define EVENTLOG_WARNING_TYPE		0x0002
#define EVENTLOG_INFORMATION_TYPE	0x0004
#define EVENTLOG_AUDIT_SUCCESS		0x0008
#define EVENTLOG_AUDIT_FAILURE		0x0010


/*
 using the type name used by Windows for these functions
 all typedefs based on how Microsoft defines the types:
 https://docs.microsoft.com/en-us/windows/desktop/winprog/windows-data-types
*/

#ifndef _BOOL_DEFINED
#define _BOOL_DEFINED
typedef int BOOL;
#endif // _BOOL_DEFINED

#ifndef TRUE
#define TRUE 1
#endif // TRUE

#ifndef FALSE
#define FALSE 0
#endif // FALSE

#ifndef _BYTE_DEFINED
#define _BYTE_DEFINED
typedef unsigned char BYTE;
#endif // _BYTE_DEFINED

#ifndef _CHAR_DEFINED
#define _CHAR_DEFINED
typedef char CHAR;
#endif // _CHAR_DEFINED

#ifndef _DWORD_DEFINED
#define _DWORD_DEFINED
typedef uint32_t DWORD;
#endif // _DWORD_DEFINED

// for levtx, this will likely point to a struct holding a representation
// of the XML interpreted from the evtx file
#ifndef _HANDLE_DEFINED
#define _HANDLE_DEFINED
typedef void *HANDLE;
#endif // _HANDLE_DEFINED

#ifndef _LPCSTR_DEFINED
#define _LPCSTR_DEFINED
typedef const char *LPCSTR;
#endif // _LPCSTR_DEFINED

#ifndef _LPVOID_DEFINED
typedef void *LPVOID;
#endif // _LPVOID_DEFINED

#ifndef _WCHAR_DEFINED
#define _WCHAR_DEFINED
typedef wchar_t WCHAR;
#endif // _WCHAR_DEFINED

#ifndef _WORD_DEFINED
#define _WORD_DEFINED
typedef uint16_t WORD;
#endif // _WORD_DEFINED

#ifndef _SID_IDENTIFIER_AUTHORITY_DEFINED
#define _SID_IDENTIFIER_AUTHORITY_DEFINED
// https://docs.microsoft.com/en-us/windows/desktop/api/winnt/ns-winnt-_sid_identifier_authority
typedef struct _SID_IDENTIFIER_AUTHORITY {
	BYTE Value[6];
} SID_IDENTIFIER_AUTHORITY, *PSID_IDENTIFIER_AUTHORITY;
#endif // _SID_IDENTIFIER_AUTHORITY_DEFINED

#ifndef _SID_DEFINED
#define _SID_DEFINED
// https://docs.microsoft.com/en-us/windows/desktop/api/winnt/ns-winnt-_sid
typedef struct _SID {
  BYTE                     Revision;
  BYTE                     SubAuthorityCount;
  SID_IDENTIFIER_AUTHORITY IdentifierAuthority;
  DWORD                    *SubAuthority;
  /*
   docs a little unclear on how SubAuthority should work...
   it says *SubAuthority[] or SubAuthority[ANYSIZE_ARRAY], implying the value
   at that part of the struct could be a pointer to an array OR could be
   element 0 of the array... not sure how this will be determined.
  */
} SID, *PISID;
#endif // _SID_DEFINED

#ifndef _EVENTLOGRECORD_DEFINED
#define _EVENTLOGRECORD_DEFINED
// https://docs.microsoft.com/en-us/windows/desktop/api/winnt/ns-winnt-_eventlogrecord
typedef struct _EVENTLOGRECORD {
	DWORD Length;	// total length of struct, INCLUDING undefined members
	DWORD Reserved;	// always set to ELF_LOG_SIGNATURE
	DWORD RecordNumber;	// index of the record, used for seek reading
	DWORD TimeGenerated;	// when entry was submitted
	DWORD TimeWritten;	// when entry was received by the service
	DWORD EventID;	// UID defined by source; used to find event message
	WORD EventType;	// an int defining the event type
	WORD NumStrings;	// number of strings found at StringOffset
	WORD EventCategory;	// category of event; meaning depends on source...
	WORD ReservedFlags;	// Reserved (?)
	DWORD ClosingRecordNumber;	// Reserved (?)
	DWORD StringOffset;	// offset of the description strings within record
	DWORD UserSidLength;	// size of UserSid member in bytes; can be 0
	DWORD UserSidOffset;	// offset of the UserSID within record
	DWORD DataLength;	// size of event-specific data at DataOffset
	DWORD DataOffset;	// offset of the Data member within record
	/*
	// undefined "members", for the sake of reference
	WCHAR SourceName[];	// variable-length string that specifies the name of the source
	WCHAR ComputerName[];	// name of the computer that generated event; may include padding to align UserSID
	SID UserSid;	// IDs active user at time event was logged; may be empty if length is 0
	WCHAR Strings[];
	BYTE Data[];
	CHAR Pad[];	// aligns Length on DWORD boundery, I guess
	DWORD Length;	// duplicate of above length
	*/
} EVENTLOGRECORD,*PEVENTLOGRECORD;
#endif // _EVENTLOGRECORD_DEFINED


// log file access

// this function will behave like a normal file open command, looking for file
// "filename" on the specified server, else on the current machine

// https://msdn.microsoft.com/en-us/library/aa363671
HANDLE OpenBackupEventLog(
	LPCSTR lpUNCServerName,	// same as OpenEventLog
	LPCSTR lpFileName	// similar to OpenEventLog, except full file path
	);

// https://msdn.microsoft.com/en-us/library/aa363639
BOOL CloseEventLog(
	HANDLE hEventLog	// handle to event log to be closed
	);

// log event access

// https://msdn.microsoft.com/en-us/library/aa363674
BOOL ReadEventLog(
	HANDLE hEventLog,	// handle to log to be read
	WORD dwReadFlags,	// how to read events from log
	WORD dwRecordOffset,	// event index to start reading at if seek read
	LPVOID lpBuffer,	// receives >=1 EVENTLOGRECORD structs; can't be NULL, per M$
	DWORD nNumberOfBytesToRead,	// bytes size of buff; <=0x7ffff, per M$
	DWORD *pnBytesRead,	// stores the number of bytes stored in buf
	DWORD *pnMinNumberOfBytesNeeded	// bytes needed if buf is too small
		// in M$, this is valid on return 0 and error ERROR_INSUFFICIENT_BUFFER
		// I will update this with appropriate errno value at some point...
	);

#endif // WINLOG_H_INCLUDED
