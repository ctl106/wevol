#include <stdio.h>

#include "check_main.h"

#include "evtx.h"
#include "evtx_test_data.h"


/*	utility declerations	*/
int test_build_evtx_record(const char *addr);


/*	tests	*/

START_TEST(test_good_evtx_header)
{
	EvtxHeader test_header;
	ck_assert_int_eq(
		build_evtx_header(good_evtx_header, &test_header),
		EXIT_SUCCESS
	);
}
END_TEST

START_TEST(test_bad_evtx_header)
{
	char header_data[sizeof(EvtxHeader)] = {0};
	EvtxHeader test_header;
	ck_assert_int_eq(
		build_evtx_header(header_data, &test_header),
		EXIT_FAILURE
	);
}
END_TEST

START_TEST(test_good_evtx_chunk)
{
	EvtxChunk test_chunk;
	ck_assert_int_eq(
		build_evtx_chunk(good_evtx_chunk, &test_chunk),
		EXIT_SUCCESS
	);
}
END_TEST

START_TEST(test_bad_evtx_chunk)
{
	char chunk_data[sizeof(EvtxChunk)] = {0};
	EvtxChunk test_chunk;
	ck_assert_int_eq(
		build_evtx_chunk(chunk_data, &test_chunk),
		EXIT_FAILURE
	);
}
END_TEST

START_TEST(test_good_evtx_record)
{
	ck_assert_int_eq(
		test_build_evtx_record(good_evtx_record),
		EXIT_SUCCESS
	);
}
END_TEST

START_TEST(test_bad_evtx_record_magic)
{
	ck_assert_int_eq(
		test_build_evtx_record(bad_evtx_magic_record),
		EXIT_FAILURE
	);
}
END_TEST

START_TEST(test_bad_evtx_record_length)
{
	ck_assert_int_eq(
		test_build_evtx_record(bad_evtx_length_record),
		EXIT_FAILURE
	);
}
END_TEST

START_TEST(test_find_chunk_0)
{
	const char *addr = chunk_addr(good_evtx_file, 0);
	printf("header size:\t%d\n", sizeof(EvtxHeader));
	printf("`good_evtx_file` addr:\t%p\t`addr`:\t%p\n", good_evtx_file, addr);
	char test_magic[sizeof(evtx_chunk_magic)];
	memcpy(test_magic, addr, sizeof(test_magic));

	uint64_t mag_int = *(uint64_t*)evtx_chunk_magic;
	uint64_t test_int = *(uint64_t*)test_magic;
	ck_assert_int_eq(mag_int, test_int);
	//ck_assert_bin_eq(evtx_chunk_magic, test_magic, sizeof(evtx_chunk_magic));
}
END_TEST


/*	utilities	*/

int test_build_evtx_record(const char *addr)
{
	int size;
	size = build_evtx_record(addr, NULL, 0);

	EvtxRecord *test_record;
	test_record = malloc(size);

	size = build_evtx_record(addr, test_record, size);
	if(size>=0)
		return EXIT_SUCCESS;
	else
		return EXIT_FAILURE;
}


/*	suite builder	*/

Suite *evtx_suite(void)
{
	Suite *suite = suite_create("Evtx");

	TCase *header = tcase_create("Header");
	tcase_add_test(header, test_good_evtx_header);
	tcase_add_test(header, test_bad_evtx_header);
	suite_add_tcase(suite, header);

	TCase *chunk = tcase_create("Chunk");
	tcase_add_test(chunk, test_good_evtx_chunk);
	tcase_add_test(chunk, test_bad_evtx_chunk);
	suite_add_tcase(suite, chunk);

	TCase *record = tcase_create("Record");
	tcase_add_test(record, test_good_evtx_record);
	tcase_add_test(record, test_bad_evtx_record_magic);
	tcase_add_test(record, test_bad_evtx_record_length);
	suite_add_tcase(suite, record);

	TCase *find_chunk = tcase_create("Find_File");
	tcase_add_test(find_chunk, test_find_chunk_0);
	suite_add_tcase(suite, find_chunk);

	return suite;
}
