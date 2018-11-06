// Melek Bilgin Tamtürk 230201053
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <tchar.h>


#define NO_OF_PROCESS 7
int main(int argc, char* argv[])
{
	STARTUPINFO si[NO_OF_PROCESS];
	PROCESS_INFORMATION pi[NO_OF_PROCESS];
	HANDLE processHandles[NO_OF_PROCESS];

	char* lpCommandLine[NO_OF_PROCESS] = { "childp.exe 1",
		"childp.exe 2",
		"childp.exe 3",
		"childp.exe 4",
		"childp.exe 5",
		"childp.exe 6",
		"childp.exe 7" };
	int i = 0;
	for (i = 0; i < NO_OF_PROCESS; i++)
	{
		SecureZeroMemory(&si[i], sizeof(STARTUPINFO));
		si[i].cb = sizeof(STARTUPINFO);
		SecureZeroMemory(&pi[i], sizeof(PROCESS_INFORMATION));

		if (!CreateProcess(NULL,
			lpCommandLine[i],
			NULL,
			NULL,
			FALSE,
			CREATE_NEW_CONSOLE,
			NULL,
			NULL,
			&si[i],
			&pi[i]))
		{
			printf("unable to create process: %d\n", i);
			system("pause");
			ExitProcess(0);
		}
		else
		{
			processHandles[i] = pi[i].hProcess;
		}
	}
	printf("Parent process has created.\n");

	WaitForMultipleObjects(NO_OF_PROCESS, processHandles, TRUE, INFINITE);


	for (i = 0; i < NO_OF_PROCESS; i++)
	{
		CloseHandle(pi[i].hThread);
		CloseHandle(pi[i].hProcess);
	}
	return 1;
}


//C:\\WINDOWS\\system32\\mspaint.exe