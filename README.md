# wevol
###### Windows Event Viewer On Linux
------


### Purpose

Wevol is a C program for parsing and sorting through event records in the Windows Event .evtx files.

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
