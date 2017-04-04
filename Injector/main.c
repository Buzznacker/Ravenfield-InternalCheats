#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <TlHelp32.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>

#include "LoadLibraryR.h"
#include "memory.h"

//===============================================================================================//
//	A LOT OF THE STUFF ARE FROM VAPE CRACKED LAUNCHER.											 //
//===============================================================================================//

//===============================================================================================//
// https://stackoverflow.com/questions/744766/how-to-compare-ends-of-strings-in-c
int string_has_suffix(const WCHAR *str, const WCHAR *suf)
{
	const WCHAR *a = str + wcslen(str);
	const WCHAR *b = suf + wcslen(suf);

	while (a != str && b != suf) {
		if (*--a != *--b) break;
	}
	return b == suf && *a == *b;
}
//===============================================================================================//

//===============================================================================================//
#define GWOP_CTX_FREE(ptr)					\
	do {									\
		if( ptr ) {							\
			if( ptr->Windows )				\
				MemFree( ptr->Windows );		\
			MemFree( ptr );					\
			ptr = NULL;						\
		}									\
	} while( 0 );

typedef struct _GWOP_CTX {
	DWORD TargetProcess;
	HWND* Windows;
	SIZE_T WindowCount;
} GWOP_CTX, *PGWOP_CTX;

BOOL CALLBACK GetWindowsOfProcessProc(HWND hwnd, LPARAM lParam)
{
	PGWOP_CTX ctx;
	DWORD processId;

	ctx = (PGWOP_CTX)lParam;
	GetWindowThreadProcessId(hwnd, &processId);

	if (processId == ctx->TargetProcess)
	{
		HWND* newMemory = NULL;

		newMemory = (HWND*)MemReAlloc(ctx->Windows, (ctx->WindowCount + 1) * sizeof(HWND));
		if (NULL == newMemory)
			return FALSE;

		ctx->Windows = newMemory;
		ctx->Windows[ctx->WindowCount++] = hwnd;
	}

	return TRUE;
}

// Caller must free
PGWOP_CTX GetWindowsOfProcess(DWORD pid)
{
	PGWOP_CTX ctx;

	if ((ctx = (PGWOP_CTX)MemCalloc(sizeof(GWOP_CTX))))
	{
		ctx->TargetProcess = pid;
		EnumWindows(GetWindowsOfProcessProc, (LPARAM)ctx);
		return ctx;
	}

	return NULL;
}
//===============================================================================================//

//===============================================================================================//
#define FPBNAWT_CTX_FREE(ptr)				\
	do {									\
		if( ptr ) {							\
			if( ptr->Pids )					\
				MemFree( ptr->Pids );		\
			MemFree( ptr );					\
			ptr = NULL;						\
		}									\
	} while( 0 );

typedef struct _FPBNAWT_CTX
{
	PDWORD Pids;
	SIZE_T PidCount;
} FPBNAWT_CTX, *PFPBNAWT_CTX;

// Caller must free
PFPBNAWT_CTX FindProcessByNameAndWindowTitle(const wchar_t* name, const wchar_t* title, const BOOL matchTitleExact)
{
	HANDLE snapshot = INVALID_HANDLE_VALUE;
	PFPBNAWT_CTX fpbnawtCtx = NULL;

	if (INVALID_HANDLE_VALUE != (snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0)))
	{
		PROCESSENTRY32 entry = { 0 };
		PDWORD pids = NULL;
		SIZE_T pidCount = 0;

		entry.dwSize = sizeof(PROCESSENTRY32);

		if (TRUE == Process32First(snapshot, &entry))
		{
			while (TRUE == Process32Next(snapshot, &entry))
			{
				if (0 == wcscmp(entry.szExeFile, name))
				{
					PDWORD newMemory = NULL;

					newMemory = (PDWORD)MemReAlloc(pids, (pidCount + 1) * sizeof(DWORD));
					if (NULL == newMemory)
						break;

					pids = newMemory;
					pids[pidCount++] = entry.th32ProcessID;
				}
			}
		}

		if (NULL != pids)
		{
			fpbnawtCtx = (PFPBNAWT_CTX)MemCalloc(sizeof(FPBNAWT_CTX));

			if (NULL != fpbnawtCtx)
			{
				PGWOP_CTX gwopCtx;
				wchar_t textBuf[256]; // Make configurable?
				BOOL outOfMemory = FALSE;

				for (SIZE_T iii = 0; iii < pidCount; iii++)
				{
					gwopCtx = GetWindowsOfProcess(pids[iii]);

					if (gwopCtx == NULL)
						// Failed to allocate memory
						break;

					for (SIZE_T jjj = 0; jjj < gwopCtx->WindowCount; jjj++)
					{
						ZeroMemory(textBuf, sizeof(textBuf));

						if (GetWindowText(gwopCtx->Windows[jjj], textBuf, sizeof(textBuf)) > 0)
						{
							BOOL match = FALSE;

							if (matchTitleExact)
							{
								if (0 == wcscmp(textBuf, title))
								{
									match = TRUE;
								}
							}
							else
							{
								if (NULL != wcsstr(textBuf, title))
								{
									match = TRUE;
								}
							}

							if (TRUE == match)
							{
								PDWORD newMemory = NULL;

								newMemory = (PDWORD)MemReAlloc(fpbnawtCtx->Pids, (fpbnawtCtx->PidCount + 1) * sizeof(DWORD));
								if (NULL == newMemory) {
									outOfMemory = TRUE;
									break;
								}

								fpbnawtCtx->Pids = newMemory;
								fpbnawtCtx->Pids[fpbnawtCtx->PidCount++] = pids[iii];
								break;
							}
						}
					}

					GWOP_CTX_FREE(gwopCtx);

					if (outOfMemory)
						break;
				}
			}

			MemFree(pids);
			pids = NULL;
		}
	}

	return fpbnawtCtx;
}
//===============================================================================================//

//===============================================================================================//
#define FFIDMS_CTX_FREE(ptr)							\
	do {												\
		if( ptr ) {										\
			if( ptr->Paths )	 {							\
				for( int iii = 0;						\
					 iii < ptr->PathsCount;				\
					 iii++ ) {							\
					if( ptr->Paths[iii] )				\
						MemFree( ptr->Paths[iii] );		\
				}										\
				MemFree( ptr->Paths );					\
			}											\
			MemFree( ptr );								\
			ptr = NULL;									\
		}												\
	} while( 0 );

typedef struct _FFIDMS_CTX
{
	LPWSTR* Paths;
	SIZE_T PathsCount;
} FFIDMS_CTX, *PFFIDMS_CTX;

// Caller must free
PFFIDMS_CTX FindFilesInDirectoryWithSuffix(LPWSTR path, LPWSTR suffix)
{
	PFFIDMS_CTX ctx = NULL;

	if ((ctx = (PFFIDMS_CTX)MemCalloc(sizeof(FFIDMS_CTX))))
	{
		HANDLE findHandle;
		WIN32_FIND_DATA findData;
		WCHAR dirPath[MAX_PATH]; // Unicode API supports up to 32k chars, but oh well, MAX_PATH should be fine.
		SIZE_T pathLength;
		BOOL pathEndsWithBackslash;

		pathLength = wcslen(path);
		pathEndsWithBackslash = path[pathLength - 1] == L'\\';

		// Append "\*" for FindFirstFile
		ZeroMemory(dirPath, MAX_PATH);
		wcscpy(dirPath, path);
		wcscat(dirPath, L"\\*");

		if (INVALID_HANDLE_VALUE != (findHandle = FindFirstFile(dirPath, &findData)))
		{
			do
			{
				if (TRUE == string_has_suffix(findData.cFileName, suffix))
				{
					LPWSTR fullPath = NULL;
					SIZE_T fileNameLength;
					LPWSTR* newMemory = NULL;

					fileNameLength = wcslen(findData.cFileName);
					fullPath = (LPWSTR)MemCalloc((pathLength + fileNameLength + 2) * sizeof(WCHAR)); // null-terminator & possible "\"
					if (NULL == fullPath)
						break;

					// Rebuild full path
					wcscpy(fullPath, path);
					if (FALSE == pathEndsWithBackslash)
						wcscat(fullPath, L"\\");
					wcscat(fullPath, findData.cFileName);

					newMemory = (LPWSTR*)MemReAlloc(ctx->Paths, (ctx->PathsCount + 1) * sizeof(LPWSTR));
					if (NULL == newMemory)
						break;

					ctx->Paths = newMemory;
					ctx->Paths[ctx->PathsCount++] = fullPath;
				}
			} while (0 != FindNextFile(findHandle, &findData));

			FindClose(findHandle);
		}
	}

	return ctx;
}
//===============================================================================================//

//===============================================================================================//
// Caller must free
PFFIDMS_CTX GetDllInModuleDirectory(VOID)
{
	PFFIDMS_CTX ctx = NULL;
	LPWSTR modulePath = NULL;

	// Unicode API supports up to 32k chars, but oh well, MAX_PATH should be fine
	modulePath = (LPWSTR)MemCalloc((MAX_PATH + 1) * sizeof(WCHAR));

	if (NULL != modulePath)
	{
		if (GetModuleFileName(NULL, modulePath, MAX_PATH) > 0)
		{
			PWCHAR ch = wcsrchr(modulePath, L'\\');

			if (NULL != ch)
			{
				*ch = L'\0';
				ctx = FindFilesInDirectoryWithSuffix(modulePath, L".dll");
			}
		}

		MemFree(modulePath);
	}

	return ctx;
}
//===============================================================================================//

//===============================================================================================//
DWORD FindProcessId(wchar_t* processName)
{
	PROCESSENTRY32 processInfo;
	processInfo.dwSize = sizeof(processInfo);

	HANDLE processSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (processSnapshot == INVALID_HANDLE_VALUE)
		return 0;

	Process32First(processSnapshot, &processInfo);
	if (wcscmp(processName, processInfo.szExeFile) == NULL)
	{
		CloseHandle(processSnapshot);
		return processInfo.th32ProcessID;
	}

	while (Process32Next(processSnapshot, &processInfo))
	{
		if (wcscmp(processName, processInfo.szExeFile) == NULL)
		{
			CloseHandle(processSnapshot);
			return processInfo.th32ProcessID;
		}
	}

	CloseHandle(processSnapshot);
	return 0;
}
//===============================================================================================//

//===============================================================================================//
//    https://github.com/stephenfewer/ReflectiveDLLInjection/blob/master/inject/src/Inject.c	 //
//===============================================================================================//
BOOL InjectDynamicLib(DWORD pId, LPWSTR unityDllPath, LPWSTR cheatDllPath)
{
	HANDLE hFile = NULL;
	HANDLE hModule = NULL;
	HANDLE hProcess = NULL;
	HANDLE hToken = NULL;
	LPVOID remoteMemoryBuffer = NULL;
	LPVOID lpBuffer = NULL;
	DWORD dwLength = 0;
	DWORD dwBytesRead = 0;
	DWORD dwProcessId = 0;
	TOKEN_PRIVILEGES priv = { 0 };

	hFile = CreateFile(unityDllPath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return FALSE;


	dwLength = GetFileSize(hFile, NULL);
	if (dwLength == INVALID_FILE_SIZE || dwLength == 0)
		return FALSE;

	lpBuffer = HeapAlloc(GetProcessHeap(), 0, dwLength);
	if (!lpBuffer)
		return FALSE;

	if (ReadFile(hFile, lpBuffer, dwLength, &dwBytesRead, NULL) == FALSE)
		return FALSE;

	if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
	{
		priv.PrivilegeCount = 1;
		priv.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

		if (LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &priv.Privileges[0].Luid))
			AdjustTokenPrivileges(hToken, FALSE, &priv, 0, NULL, NULL);

		CloseHandle(hToken);
	}

	hProcess = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ, FALSE, pId);
	if (!hProcess)
		return FALSE;

	//Allocate memory to store our cheat dllpath inside the remote process.
	remoteMemoryBuffer = VirtualAllocEx(hProcess, NULL, wcslen(cheatDllPath) * sizeof(WCHAR), MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if (!remoteMemoryBuffer)
		return FALSE;

	//Write the path to the buffer.
	if (!WriteProcessMemory(hProcess, remoteMemoryBuffer, cheatDllPath, wcslen(cheatDllPath) * sizeof(WCHAR), NULL))
		return FALSE;

	//Inject and pass our allocated buffer as an argument
	hModule = LoadRemoteLibraryR(hProcess, lpBuffer, dwLength, remoteMemoryBuffer);
	if (!hModule)
		return FALSE;

	WaitForSingleObject(hModule, -1);

	if (lpBuffer)
		HeapFree(GetProcessHeap(), 0, lpBuffer);

	if (hProcess)
		CloseHandle(hProcess);

	return TRUE;
}

int wmain()
{
	PFFIDMS_CTX dllInPath = NULL;
	LPWSTR unityDllPath = NULL;
	LPWSTR cheatDllPath = NULL;
	SIZE_T dllPathLength = 0;

	dllInPath = GetDllInModuleDirectory();
	if (NULL == dllInPath)
	{
		printf("Failed to find any dlls in the current path\n");
		goto exit;
	}
	else if (dllInPath->PathsCount > 2)
	{
		printf("More than 2 Dlls have been found in the active directory. Make sure you only have 2 dll in the path \n");
		goto exit;
	}
	else if (dllInPath->PathsCount <= 1)
	{
		printf("Only 1 dll or none have been found in the active directory.\n");
		goto exit;
	}

	DWORD pId = FindProcessId(L"Ravenfield.exe");

	if (NULL == pId)
	{
		printf("Could not find process\n");
		goto exit;
	}

	printf("Injecting dll into process\n");

	cheatDllPath = dllInPath->Paths[0];
	unityDllPath = dllInPath->Paths[1];

	if (InjectDynamicLib(pId, unityDllPath, cheatDllPath))
		printf("Successfully injected dll\n");
	else
		printf("Error while injecting dll\n");

exit: 
	FFIDMS_CTX_FREE( dllInPath );
	printf("Press any key to exit...\n");
	_getch();
	return 1;
}