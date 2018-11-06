//Melek Bilgin Tamtürk - 230201053
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>

//Name given to the pipe
#define g_szPipeName "\\\\.\\Pipe\\MyNamedPipe"
#define BUFFER_SIZE 1024 

void writeToParent(HANDLE hPipe, char msg[BUFFER_SIZE]);
char *readFromParent(HANDLE hPipe);

int main(int argc, char* argv[]){
	HANDLE hPipe;
	//Connect to the server pipe using CreateFile()
	hPipe = CreateFile(
		g_szPipeName,   // pipe name 
		GENERIC_READ |  // read and write access 
		GENERIC_WRITE,
		0,              // no sharing 
		NULL,           // default security attributes
		OPEN_EXISTING,  // opens existing pipe 
		0,              // default attributes 
		NULL);          // no template file 

	if (INVALID_HANDLE_VALUE == hPipe){
		printf("\nError occurred while connecting"
			" to the server: %d", GetLastError());
		//One might want to check whether the server pipe is busy
		//This sample will error out if the server pipe is busy
		//Read on ERROR_PIPE_BUSY and WaitNamedPipe() for that
		return 1;  //Error
	}
	
	char msg[BUFFER_SIZE];
	
	int count = 1;
	while(count<6){
		//reading the bursting time of process
		int a = atoi(readFromParent(hPipe));
		sprintf(msg, "P%s started.\n", argv[1]);
		//writing to parrent that process started to work
		writeToParent(hPipe, msg);
		//
		Sleep(a);
		sprintf(msg, "P%s ended.\n", argv[1]);
		//writing to parrent that process ended 
		writeToParent(hPipe, msg);
		count++;
	}
	CloseHandle(hPipe);
	return 0; //Success
}

void writeToParent(HANDLE hPipe, char msg[BUFFER_SIZE]) {
	char szBuffer[BUFFER_SIZE];
	strcpy(szBuffer, msg );

	DWORD cbBytes;
	//Send the message to server
	BOOL bResult = WriteFile(
		hPipe,                // handle to pipe 
		szBuffer,             // buffer to write from 
		strlen(szBuffer) + 1,   // number of bytes to write, include the NULL
		&cbBytes,             // number of bytes written 
		NULL);                // not overlapped I/O 
	
	if ((!bResult) || (strlen(szBuffer) + 1 != cbBytes)) {
		printf("\nError occurred while writing"
			" to the server: %d", GetLastError());
		CloseHandle(hPipe);
	}

}
char *readFromParent(HANDLE hPipe) {
	//Read server response
	char szBuffer[BUFFER_SIZE];
	DWORD cbBytes;
	
	BOOL bResult = ReadFile(
		hPipe,                // handle to pipe 
		szBuffer,             // buffer to receive data 
		sizeof(szBuffer),     // size of buffer 
		&cbBytes,             // number of bytes read 
		NULL);                // not overlapped I/O 

	if ((!bResult) || (0 == cbBytes)) {
		printf("\nError occurred while reading"
			" from the server: %d", GetLastError());
		CloseHandle(hPipe);
	}
	return szBuffer;

}



