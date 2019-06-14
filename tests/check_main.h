#ifndef CHECK_MAIN_H_INCLUDED
#define CHECK_MAIN_H_INCLUDED


#include <stdlib.h>

#include <check.h>

#include "check_assertions.h"


Suite *evtx_suite(void);
Suite *evt_suite(void);
Suite *winlog_suite(void);


#endif	// CHECK_MAIN_H_INCLUDED
