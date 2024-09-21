/*-------------------------------------------------------------------------
 *
 * win32pread.c
 *	  Implementation of pread(2) for Windows.
 *
 * Portions Copyright (c) 1996-2022, PostgreSQL Global Development Group
 *
 * IDENTIFICATION
 *	  src/port/win32pread.c
 *
 *-------------------------------------------------------------------------
 */


#include "postgres.h"

#include <windows.h>

ssize_t
pread(int fd, void *buf/*要填充的buffer*/, size_t size/*可填充的buffer size*/, off_t offset/*要读取的位置*/)
{
	OVERLAPPED	overlapped = {0};
	HANDLE		handle;
	DWORD		result;

	handle = (HANDLE) _get_osfhandle(fd);
	if (handle == INVALID_HANDLE_VALUE)
	{
		errno = EBADF;
		return -1;
	}

	overlapped.Offset = offset;
	if (!ReadFile(handle, buf, size, &result, &overlapped))
	{
		if (GetLastError() == ERROR_HANDLE_EOF)
			return 0;

		_dosmaperr(GetLastError());
		return -1;
	}

	return result;
}
