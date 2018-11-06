//Melek Bilgin Tamtürk - 230201053
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <tchar.h>
#include <time.h>

#define NO_OF_PROCESS 5
//Name given to the pipe
#define g_szPipeName "\\\\.\\Pipe\\MyNamedPipe"
#define BUFFER_SIZE 1024 

void readFromChild(HANDLE hPipe);
void writeToChild(HANDLE hPipe, char msg[BUFFER_SIZE]);
void insertionSort(float arr[]);
void orderIndexes(float temp[], float tn[], int indexes[]);
void cpuBurst(HANDLE hPipe[], float tn[], float tnTemp[], int tnExecutionOrder[]);
void calcNprintTn(float tn[], float tnTemp[], int tnExecutionOrder[]);


int main(int argc, char* argv[])
{
	STARTUPINFO si[NO_OF_PROCESS];
	PROCESS_INFORMATION pi[NO_OF_PROCESS];
	HANDLE processHandles[NO_OF_PROCESS];
	HANDLE hPipe[NO_OF_PROCESS];
	float tnTemp[NO_OF_PROCESS];
	float tn[NO_OF_PROCESS];
	int tnExecutionOrder[NO_OF_PROCESS];
	srand((unsigned int)time(NULL));


	char* lpCommandLine[NO_OF_PROCESS] = {  "Child.exe 1"
											,"Child.exe 2"
											,"Child.exe 3"
											,"Child.exe 4" 
											,"Child.exe 5" };
	int i = 0;
	for (i = 0; i < NO_OF_PROCESS; i++)
	{	//creating pipe
		hPipe[i] = CreateNamedPipe(
			g_szPipeName,             // pipe name 
			PIPE_ACCESS_DUPLEX,       // read/write access 
			PIPE_TYPE_MESSAGE |       // message type pipe 
			PIPE_READMODE_MESSAGE |   // message-read mode 
			PIPE_WAIT,                // blocking mode 
			PIPE_UNLIMITED_INSTANCES, // max. instances  
			BUFFER_SIZE,              // output buffer size 
			BUFFER_SIZE,              // input buffer size 
			NMPWAIT_USE_DEFAULT_WAIT, // client time-out 
			NULL);                    // default security attribute 

		if (INVALID_HANDLE_VALUE == hPipe[i]) {
			printf("\nError occurred while "
				"creating the pipe: %d", GetLastError());
			return 1;  //Error
		}
		
		SecureZeroMemory(&si[i], sizeof(STARTUPINFO));
		si[i].cb = sizeof(STARTUPINFO);
		SecureZeroMemory(&pi[i], sizeof(PROCESS_INFORMATION));

		//creating processes
		if (!CreateProcess(NULL,
			lpCommandLine[i],
			NULL,
			NULL,
			FALSE,
			NULL,//CREATE_NEW_CONSOLE
			NULL,
			NULL,
			&si[i],
			&pi[i])) {
			printf("unable to create process: %d\n", i);
			system("pause");
			ExitProcess(0);
		}
		//Wait for the client to connect
		BOOL bClientConnected = ConnectNamedPipe(hPipe[i], NULL);

		if (FALSE == bClientConnected)
		{
			printf("\nError occurred while connecting"
				" to the client: %d", GetLastError());
			CloseHandle(hPipe[i]);
			return 1;  //Error
		}
	}
	
	// Initiliazing tn values 
	tnTemp[0] = 300.0;
	tnTemp[1] = 220.0;
	tnTemp[2] = 180.0;
	tnTemp[3] = 45.0;
	tnTemp[4] = 255.0;

	//copying tnTemp to real tn
	memcpy(&tn, &tnTemp, sizeof tn);
	//sorting tnTemp 
	insertionSort(tnTemp);
	//creating an array to hold indexes to decide which will be executed first
	orderIndexes(tnTemp, tn, tnExecutionOrder);
	//starting cpu burst
	cpuBurst(hPipe, tn, tnTemp, tnExecutionOrder);
	
	WaitForMultipleObjects(NO_OF_PROCESS, processHandles, TRUE, INFINITE);
	for (i = 0; i < NO_OF_PROCESS; i++){
		CloseHandle(pi[i].hThread);
		CloseHandle(pi[i].hProcess);
		CloseHandle(hPipe[i]);
	}
	system("pause");
	return 1;
}

void readFromChild(HANDLE hPipe) {
	char szBuffer[BUFFER_SIZE];
	DWORD cbBytes;

	//We are connected to the client.
	//To communicate with the client 
	//we will use ReadFile()/WriteFile() 
	//on the pipe handle - hPipe

	//Read client message
	BOOL bResult = ReadFile(
		hPipe,                // handle to pipe 
		szBuffer,             // buffer to receive data 
		sizeof(szBuffer),     // size of buffer 
		&cbBytes,             // number of bytes read 
		NULL);                // not overlapped I/O 

	if ((!bResult) || (0 == cbBytes)) {
		printf("\nError occurred while reading "
			"from the client: %d", GetLastError());
		CloseHandle(hPipe); //Error
	}
	printf_s(szBuffer);
}

void writeToChild(HANDLE hPipe, char msg[BUFFER_SIZE]) {
	char szBuffer[BUFFER_SIZE];
	DWORD cbBytes;

	strncpy(szBuffer, msg, BUFFER_SIZE);

	//Reply to client
	BOOL bResult = WriteFile(
		hPipe,                // handle to pipe 
		szBuffer,             // buffer to write from 
		strlen(szBuffer) + 1,   // number of bytes to write, include the NULL 
		&cbBytes,             // number of bytes written 
		NULL);                // not overlapped I/O 

	if ((!bResult) || (strlen(szBuffer) + 1 != cbBytes)) {
		printf("\nError occurred while writing"
			" to the client: %d", GetLastError());
		CloseHandle(hPipe);//Error
	}
}

void insertionSort(float arr[])
{
	int i,j;
	float key;
	for (i = 1; i < NO_OF_PROCESS; i++){
		key = arr[i];
		j = i - 1;

		/* Move elements of arr[0..i-1], that are
		greater than key, to one position ahead
		of their current position */
		while (j >= 0 && arr[j] > key)
		{
			arr[j + 1] = arr[j];
			j = j - 1;
		}
		arr[j + 1] = key;
	}
}

void orderIndexes(float temp[], float tn[], int indexes[]) {
	for (int i = 0; i < NO_OF_PROCESS; i++) {
		for (int j = 0; j < NO_OF_PROCESS; j++) {
			if (temp[i] == tn[j]) {
				indexes[i] = j;
			}
		}
	}
}

void cpuBurst(HANDLE hPipe[], float tn[], float tnTemp[], int tnExecutionOrder[] ) {	
	int count = 1;
	char msg[BUFFER_SIZE];
	while (count < 6) {
		printf_s("Execution #%d\n", count);
		printf_s("Execution order: ");
		for (int j = 0; j < NO_OF_PROCESS; j++) {
			printf_s("P%d - ", tnExecutionOrder[j]+1);
		}
		printf_s("\n");
		for (int i = 0; i < NO_OF_PROCESS; i++){
			sprintf(msg, "%d", (int)tn[tnExecutionOrder[i]]);
			//writing the burst time of child into its pipe
			writeToChild(hPipe[tnExecutionOrder[i]], msg);
			// read from child two times because we expect 2 start-end messages
			readFromChild(hPipe[tnExecutionOrder[i]]);
			readFromChild(hPipe[tnExecutionOrder[i]]);
		}
		//new burst times calculating and printing
		calcNprintTn(tn, tnTemp, tnExecutionOrder);
		count++;
	}
}

void calcNprintTn(float tn[], float tnTemp[], int tnExecutionOrder[]) {
	float newTn[NO_OF_PROCESS];
	float a = 0.5;
	//calculating new burst times according to random actual lenght 
	for (int j = 0; j < NO_OF_PROCESS; j++) {
		tn[j] = (float)((rand() % 251) + 50); // actual length
		newTn[j] = (a*tn[j]) + ((1 - a)*tnTemp[j]); // estimated burst times
	}
	//printing values
	for (int i = 0; i < NO_OF_PROCESS; i++) {
		printf("For P%d\n", i+1);
		printf("Tn                : %f\n", tnTemp[i]);
		printf("tn(actual length) : %f\n", tn[i]);
		printf("Tn+1              : %f\n", newTn[i]);
	}
	printf("-----------------------------------------\n");
	//copying newTn to real tnTemp
	for  (int k= 0; k < NO_OF_PROCESS; k++){
		tnTemp[k] = newTn[k];
	}
	//sorting newTn 
	insertionSort(newTn);
	//creating an array to hold indexes to decide which will be executed first
	orderIndexes(newTn, tnTemp, tnExecutionOrder);

}