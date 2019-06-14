# wevol
###### Windows Event Viewer On Linux
------


### Purpose

Wevol is a digital forensics Linux program for reading Windows Eveng Log files.

### Dependancies
1. CMake - https://cmake.org/ - build system
2. Check - https://libcheck.github.io/check/ - unit testing framework

### Building This Project

In order to keep the project tree clean and prevent accidental polution resulting from in-source builds, out-of-source build is enforced by the `CMakeLists.txt` file.

To build this project, after setting up dependancies and cloning this project, follow these instructions:

1. Create a `build` directory in the project root if one does not already exist
````shell
mkdir build
````
2. Change directories into the `build` directory
````shell
cd build
````
3. Call `cmake`, specifying the `generator-name` option corresponding to the IDE and version of `make` that you use. In my example, I am using CodeBlocks with Unix style `makefile`
````shell
cmake .. -G "CodeBlocks - Unix Makefiles"
````
4. If you are using a supported IDE and selected the relevent `generator-name` option, a project file will have been created, allowing you to edit the files in your IDE without having to create a project from scratch. Otherwise, you may have to create your own project here or edit the files via a traditional text editor.

5. Once you are ready to compile the code, if using a supported IDE, you should be able to select the `build` button from within the IDE. Otherwise, you should be able to build the project from the command-line using the version of `make` corresponding to the `generator-name` selected previously:
````shell
make
````

### Testing This Project

There are two different ways to test this project:

1. Using the `test` target of the makefile
2. Directly calling the `check_main` binary

All instructions in this section assume that `build` is still the current working directory.

##### Using The `test` Makefile Target

This option, at least when used through the GNU `make` utility, 
sees this project as one single test, and thus is only useful for determining if the entire project passes or fails. It can be used as follows:
````shell
make test
````

##### Calling the `check_main` Binary

This option provides more information, listing the suites to be tested and reporting the actual quantity of successes and failures from each suite. It can be used as follows:
````shell
./tests/check_main
````

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

