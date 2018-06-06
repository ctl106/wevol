#include <stdio.h>

#include "lib/levtx.h"

#include "..\wizlib\testlib.h"


#define APPLICATION_EVTX	".\test_data\Application.evtx"


int main()
{
	HANDLE handle = OpenBackupEventLog(NULL, APPLICATION_EVTX);

	int result;
	size_t buf_len = 1024;
	size_t read = 1;
	char *buf = NULL;
	do {
		buf = realloc(buf, buf_len);
		result = ReadEventLog(
			handle,
			EVENTLOG_SEQUENTIAL_READ|EVENTLOG_FORWARDS_READ,
			0,
			buf,
			buf_len,
			&read,
			&buf_len
			);
	} while(!result);

	free(buf);
}
