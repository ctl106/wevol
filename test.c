#include <stdio.h>

#include "lib/levtx.h"

#include "..\wizlib\testlib.h"


#define APPLICATION_EVTX	".\test_data\Application.evtx"


int main()
{
	HANDLE handle = OpenBackupEventLog(NULL, APPLICATION_EVTX);
}
