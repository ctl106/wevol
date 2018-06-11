#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "lib/swap_endian.h"
#include "lib/winlog.h"

#include "test_data.h"

#include "..\wizlib\testlib.h"


#define APPLICATION_EVTX	".\test_data\Application.evtx"


void evtx_test_suite();
void test_evtx_header();
void test_good_evtx_header();
void test_bad_evtx_header();
void test_evtx_chunk();
void test_good_evtx_chunk();
void test_bad_evtx_chunk();
void test_evtx_record();
int test_build_evtx_record();
void test_good_evtx_record();
void test_bad_evtx_record_magic();
void test_bad_evtx_record_length();


// evtx test suite
void evtx_test_suite()
{
	test_evtx_header();
	test_evtx_chunk();
	test_evtx_record();
}

// evtx header tests
void test_evtx_header()
{
	test_good_evtx_header();
	test_bad_evtx_header();
}

void test_good_evtx_header()
{
	EvtxHeader test_header;
	assert(EXIT_SUCCESS == build_evtx_header(good_evtx_header, &test_header));
}

void test_bad_evtx_header()
{
	char header_data[sizeof(EvtxHeader)] = {0};
	EvtxHeader test_header;
	assert(EXIT_FAILURE == build_evtx_header(header_data, &test_header));
}

// evtx chunk tests
void test_evtx_chunk()
{
	test_good_evtx_chunk();
	test_bad_evtx_chunk();
}

void test_good_evtx_chunk()
{
	EvtxChunk test_chunk;
	assert(EXIT_SUCCESS == build_evtx_chunk(good_evtx_chunk, &test_chunk));
}

void test_bad_evtx_chunk()
{
	char chunk_data[sizeof(EvtxChunk)] = {0};
	EvtxChunk test_chunk;
	assert(EXIT_FAILURE == build_evtx_chunk(chunk_data, &test_chunk));
}

// evtx record tests
void test_evtx_record()
{
	test_good_evtx_record();
	test_bad_evtx_record_magic();
	test_bad_evtx_record_length();
}

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

void test_good_evtx_record()
{
	int good_evtx_record_eval = test_build_evtx_record(good_evtx_record);
	assert(EXIT_SUCCESS == good_evtx_record_eval);
}

void test_bad_evtx_record_magic()
{
	int bad_evtx_record_magic_eval = test_build_evtx_record(bad_evtx_magic_record);
	assert(EXIT_FAILURE == bad_evtx_record_magic_eval);
}

void test_bad_evtx_record_length()
{
	int bad_evtx_record_length_eval = test_build_evtx_record(bad_evtx_length_record);
	assert(EXIT_FAILURE == bad_evtx_record_length_eval);
}



int main()
{
	evtx_test_suite();
}
