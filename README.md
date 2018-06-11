# wevol
###### Windows Event Viewer On Linux
------


### Purpose

Wevol is a digital forensics Linux program for reading Windows Eveng Log files.

### Dependancies
1. CMake - https://cmake.org/ - build system
2. Check - https://libcheck.github.io/check/ - unit testing framework


### Current State

Currently, I am creating the API and the tests for reading in evtx files.
I have also moved development to a seperate branch to keep main clean.

### Goals

1. Write and test API for decoding evtx log format.
2. Reimplament needed functions from Microsoft's Event Log API:
  * OpenBackupEventLog()
  * CloseEventLog()
  * ReadEventLog()
3. Create basic CLI with the following features:
  * Specify one or more evtx log files to view a record list from
  * Specify a record within a log to view the details of
4. Create an API for decoding the older evt log format.
5. Create an interactive CLI to enable easier exploring and searching of logs.
6. Create an optional GUI to make usage more convinient.

------

### API

#### evtx.h

##### build_evtx_header
````c
int build_evtx_header(const char *log, PEvtxHeader header);
````
###### Behavior
Takes a character array `log` containing the bytecode evtx file header and
fills in the members of the struct pointed to by `header` with the
corresponding values.

###### Returns
Returns EXIT_SUCCESS if a valid evtx file header was read into
`header`, else returns EXIT_FAILURE and does not modify `header`.

note:
EXIT_SUCCESS and EXIT_FAILURE are commonly defined as 0 and non-0, respectively.

##### build_evtx_chunk
````c
int build_evtx_chunk(const char *addr, PEvtxHeader chunk);
````
###### Behavior
Takes a character array `addr` containing the bytecode evtx chunk header and
fills in the members of the struct pointed to by `chunk` with the
corresponding values.

###### Returns
Returns EXIT_SUCCESS if a valid evtx chunk header was read into
`chunk`, else returns EXIT_FAILURE and does not modify `chunk`.

note:
EXIT_SUCCESS and EXIT_FAILURE are commonly defined as 0 and non-0, respectively.

##### build_evtx_record
````c
int build_evtx_record(const char *addr, PEvtxRecord record, size_t size);
````
###### Behavior
Takes a character array `addr` containing the bytecode evtx
record, and if the size of the record is <= `size`, fills in
the members of `record` with the appropriate values.

###### Returns
Returns < 0 if the contents of location `addr` is not a valid
evtx record, else returns the size of the record at `addr`.

