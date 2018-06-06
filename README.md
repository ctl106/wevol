# wevol
###### Windows Event Viewer On Linux
------


### Purpose

Wevol is a C program for parsing and sorting through event records in the Windows Event .evtx files.

### Where We're At

Currently this project is at the beginning of its life. As such, not very much is currently implemented, and there is a fair bit of code that will likely be removed or modified.

### Current Goal

Currently I am working on implementing the event log API functions provided in Windows that I need within lib/levtx.*

------

### lib/levtx.h

````c
HANDLE OpenEventLog(const char *servername, const char *logname);
````
Returns a handle to the specified log on the specified server.

````c
HANDLE OpenBackupEventLog(const char *servername, const char *filename);
````
Returns a handle to the specified backup log file on the specified server

````c
int CloseEventLog(const HANDLE log);
````
Closes the handle returned by OpenEventLog() or OpenBackupEventLog()

````c
int BackupEventLog(const char *servername, const char *filename);
````
Creates a backup log file from the provided handle

````c
int ReadEventLog(
    const HANDLE log,
    const int readflags,
    const size_t offset, 
    void *buf,
    const size_t buf_size,
    size_t *bytes_read,
    size_t *bytes_needed
);
````
On each call, a new event record is stored in `*buf` so long as it is of sufficient size.
