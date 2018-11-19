#include <stdlib.h>


#include "check_main.h"

int main(void)
{
	int status = EXIT_SUCCESS;

	SRunner *runner = srunner_create(evtx_suite());
	srunner_add_suite(runner, evt_suite());
	srunner_add_suite(runner, winlog_suite());

	srunner_run_all(runner, CK_NORMAL);
	int failed = srunner_ntests_failed(runner);
	if (failed) status = EXIT_FAILURE;

	srunner_free(runner);

    return status;
}
