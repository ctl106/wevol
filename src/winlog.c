#include "winlog.h"


// external API method definitions

HANDLE OpenBackupEventLog(LPCSTR lpUNCServerName, LPCSTR lpFileName)
{
	return NULL;
}

BOOL CloseEventLog(HANDLE hEventLog)
{
	return 1;
}

BOOL ReadEventLog(
	HANDLE hEventLog,
	WORD dwReadFlags,
	WORD dwRecordOffset,
	LPVOID lpBuffer,
	DWORD nNumberOfBytesToRead,
	DWORD *pnBytesRead,
	DWORD *pnMinNumberOfBytesNeeded
	)
{
	/*
	when setting bytes_needed, set errno to something related to
	M$'s ERROR_INSUFFICIENT_BUFFER IF unsuccessful execution
	*/
	return 1;
}
