#include "winlog.h"


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
