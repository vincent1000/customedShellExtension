#include <windows.h>
#include <stdio.h>
#include <shlobj_core.h>
#include "Logger.h"

static TCHAR g_PathLog[1024];
static bool isInitiated = false;

int LogWrite(const TCHAR* fmt, ...)
{
	if (!isInitiated)
	{
		PWSTR path;
		SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, NULL, &path);
		wcscpy_s(g_PathLog, path);
		wcscat_s(g_PathLog, L"\\logs\\ExplorerCMD\\");
		SHCreateDirectoryEx(NULL, g_PathLog, NULL);
		wcscat_s(g_PathLog, L"explorer-cmd.log");
		DeleteFile(g_PathLog);
		isInitiated = true;
	}

	FILE* fp;
	_wfopen_s(&fp, g_PathLog, L"a+");
	if (fp != 0)
	{
		// log headers
		SYSTEMTIME localTime;
		memset(&localTime, 0, sizeof(localTime));
		GetLocalTime(&localTime);
		fprintf(fp, "\n[%04d%02d%02d-%02d:%02d:%02d.%03d]",
			localTime.wYear, localTime.wMonth, localTime.wDay,
			localTime.wHour, localTime.wMinute, localTime.wSecond, localTime.wMilliseconds);

		// log body
		{ va_list ap; va_start(ap, fmt); vfwprintf(fp, fmt, ap); va_end(ap); }

		// close file
		fclose(fp);
	}
	return 0;
}
int FileWrite(char* func, int line)
{
    if (!isInitiated)
    {
        PWSTR path;
        SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, NULL, &path);
        wcscpy_s(g_PathLog, path);
        wcscat_s(g_PathLog, L"\\logs\\ExplorerCMD\\");
        SHCreateDirectoryEx(NULL, g_PathLog, NULL);
        wcscat_s(g_PathLog, L"explorer-cmd.log");
        DeleteFile(g_PathLog);
        isInitiated = true;
    }

    FILE* fp;
    _wfopen_s(&fp, g_PathLog, L"a+");
    if (fp != 0)
    {
        // log headers
        SYSTEMTIME localTime;
        memset(&localTime, 0, sizeof(localTime));
        GetLocalTime(&localTime);
        fprintf(fp, "\n%s,%d",func,line);
        // close file
        fclose(fp);
    }
    return 0;
}
