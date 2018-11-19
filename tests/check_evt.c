#include "check_main.h"

#include <evt.h>

Suite *evt_suite(void)
{
	Suite *suite = suite_create("evtx");

	return suite;
}
