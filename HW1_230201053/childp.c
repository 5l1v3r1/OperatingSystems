// Melek Bilgin Tamtürk 230201053
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>

typedef struct {
	int threadNo;
	int milk;
	int biscuit;
	int chips;
	int coke;
	FILE *file;
	int err;
	int message;
}THREAD_PARAMETERS;

DWORD WINAPI threadWork(LPVOID parameters);


int main(int argc, char* argv[])
{
	HANDLE* handles;
	THREAD_PARAMETERS* lpParameter;
	int* threadID;
	int i = 0;
	int threadCount = 4;
	int totalMilk = 0;
	int totalBiscuit = 0;
	int totalChips = 0;
	int totalCoke = 0;

	//check arguments for error
	if (argc != 2) {
		printf("error in child process...now exiting %d\n", argv[0]);
		system("pause");
		exit(0);
	}
	//read thread count from arguments
	//threadCount = 4;

	//allocate memory for every parameters needed
	handles = malloc(sizeof(HANDLE)* threadCount);
	lpParameter = malloc(sizeof(THREAD_PARAMETERS)* threadCount);
	threadID = malloc(sizeof(int)* threadCount);
	//for each thread
	for (i = 0; i < threadCount; i++) {
		//initialize parameters
		lpParameter[i].milk = 0;
		lpParameter[i].biscuit = 0;
		lpParameter[i].chips = 0;
		lpParameter[i].coke = 0;
		lpParameter[i].threadNo = i + 1;
		lpParameter[i].err = fopen_s(&lpParameter[i].file, "market.txt", "r", "ccs = encoding");
		lpParameter[i].message = atoi(argv[1]);
		
		handles[i] = CreateThread(NULL, 0, threadWork, &lpParameter[i], 0, &threadID[i]);
		//check errors in creation
		if (handles[i] == INVALID_HANDLE_VALUE) {
			printf("error when creating thread\n");
			system("pause");
			exit(0);
		}
	}
	WaitForMultipleObjects(threadCount, handles, TRUE, INFINITE);

	for (i = 0; i < threadCount; i++) {
		totalMilk = totalMilk + lpParameter[i].milk;
		totalBiscuit = totalBiscuit + lpParameter[i].biscuit;
		totalChips = totalChips + lpParameter[i].chips;
		totalCoke = totalCoke + lpParameter[i].coke;
	}
	printf("Here is the total numbers of each item for day %d\n", lpParameter[0].message);
	printf("Milk : %d\n", totalMilk);
	printf("Biscuit : %d\n", totalBiscuit);
	printf("Chips : %d\n", totalChips);
	printf("Coke : %d\n", totalCoke);


	free(handles);
	free(lpParameter);
	free(threadID);

	system("pause");
	return 1;
}

DWORD WINAPI threadWork(LPVOID parameters)
{
	THREAD_PARAMETERS* param = (THREAD_PARAMETERS*)parameters;
	char line[200];
	if (param->err == 0) {
		char *milk = "MILK";
		char *biscuit = "BISCUIT";
		char *chips = "CHIPS";
		char *coke = "COKE";
		//reading file
		while (fgets(line, sizeof(line), param->file)!= NULL) {
			//process message and line controlling
			if ((param->message == 1) && (strcmp(line, "#START DAY 1#\n") == 0)) {
				while (strcmp(line, "#END DAY 1#\n") != 0) {
					if ((param->threadNo == 1) && (strstr(&line, milk) != NULL)) {
						param->milk = param->milk + 1; //counting milk
					}
					else if ((param->threadNo == 2) && (strstr(&line, biscuit) != NULL)) {
						param->biscuit = param->biscuit + 1; //counting biscuit
					}
					else if ((param->threadNo == 3) && (strstr(&line, chips) != NULL)) {
						param->chips = param->chips + 1; //counting chips
					}
					else if ((param->threadNo == 4) && (strstr(&line, coke) != NULL)) {
						param->coke = param->coke + 1; //counting coke
					}
					fgets(line, sizeof(line), param->file);
				}
			}
			//process message and line controlling
			if ((param->message == 2) && (strcmp(line, "#START DAY 2#\n") == 0)) {
				while (strcmp(line, "#END DAY 2#\n") != 0) {
					if ((param->threadNo == 1) && (strstr(&line, milk) != NULL)) {
						param->milk = param->milk + 1; //counting milk
					}
					else if ((param->threadNo == 2) && (strstr(&line, biscuit) != NULL)) {
						param->biscuit = param->biscuit + 1; //counting biscuit
					}
					else if ((param->threadNo == 3) && (strstr(&line, chips) != NULL)) {
						param->chips = param->chips + 1; //counting chips
					}
					else if ((param->threadNo == 4) && (strstr(&line, coke) != NULL)) {
						param->coke = param->coke + 1; //counting coke
					}
					fgets(line, sizeof(line), param->file);
				}
			}
			//process message and line controlling 
			if ((param->message == 3) && (strcmp(line, "#START DAY 3#\n") == 0)) {
				while (strcmp(line, "#END DAY 3#\n") != 0) {
					if ((param->threadNo == 1) && (strstr(&line, milk) != NULL)) {
						param->milk = param->milk + 1; //counting milk
					}
					else if ((param->threadNo == 2) && (strstr(&line, biscuit) != NULL)) {
						param->biscuit = param->biscuit + 1; //counting biscuit
					}
					else if ((param->threadNo == 3) && (strstr(&line, chips) != NULL)) {
						param->chips = param->chips + 1; //counting chips
					}
					else if ((param->threadNo == 4) && (strstr(&line, coke) != NULL)) {
						param->coke = param->coke + 1; //counting coke
					}
					fgets(line, sizeof(line), param->file);
				}
			}
			//process message and line controlling
			if ((param->message == 4) && (strcmp(line, "#START DAY 4#\n") == 0)) {
				while (strcmp(line, "#END DAY 4#\n") != 0) {
					if ((param->threadNo == 1) && (strstr(&line, milk) != NULL)) {
						param->milk = param->milk + 1; //counting milk
					}
					else if ((param->threadNo == 2) && (strstr(&line, biscuit) != NULL)) {
						param->biscuit = param->biscuit + 1; //counting biscuit
					}
					else if ((param->threadNo == 3) && (strstr(&line, chips) != NULL)) {
						param->chips = param->chips + 1; //counting chips
					}
					else if ((param->threadNo == 4) && (strstr(&line, coke) != NULL)) {
						param->coke = param->coke + 1; //counting coke
					}
					fgets(line, sizeof(line), param->file);
				}
			}
			//process message and line controlling
			if ((param->message == 5) && (strcmp(line, "#START DAY 5#\n") == 0)) {
				while (strcmp(line, "#END DAY 5#\n") != 0) {
					if ((param->threadNo == 1) && (strstr(&line, milk) != NULL)) {
						param->milk = param->milk + 1; //counting milk
					}
					else if ((param->threadNo == 2) && (strstr(&line, biscuit) != NULL)) {
						param->biscuit = param->biscuit + 1; //counting biscuit
					}
					else if ((param->threadNo == 3) && (strstr(&line, chips) != NULL)) {
						param->chips = param->chips + 1; //counting chips
					}
					else if ((param->threadNo == 4) && (strstr(&line, coke) != NULL)) {
						param->coke = param->coke + 1; //counting coke
					}
					fgets(line, sizeof(line), param->file);
				}
			}
			//process message and line controlling
			if ((param->message == 6) && (strcmp(line, "#START DAY 6#\n") == 0)) {
				while (strcmp(line, "#END DAY 6#\n") != 0) {
					if ((param->threadNo == 1) && (strstr(&line, milk) != NULL)) {
						param->milk = param->milk + 1; //counting milk
					}
					else if ((param->threadNo == 2) && (strstr(&line, biscuit) != NULL)) {
						param->biscuit = param->biscuit + 1; //counting biscuit
					}
					else if ((param->threadNo == 3) && (strstr(&line, chips) != NULL)) {
						param->chips = param->chips + 1; //counting chips
					}
					else if ((param->threadNo == 4) && (strstr(&line, coke) != NULL)) {
						param->coke = param->coke + 1; //counting coke
					}
					fgets(line, sizeof(line), param->file);
				}
			}
			//process message and line controlling
			if ((param->message == 7) && (strcmp(line, "#START DAY 7#\n") == 0)) {
				while (strcmp(line, "#END DAY 7#") != 0) {
					if ((param->threadNo == 1) && (strstr(&line, milk) != NULL)) {
						param->milk = param->milk + 1; //counting milk
					}
					else if ((param->threadNo == 2) && (strstr(&line, biscuit) != NULL)) {
						param->biscuit = param->biscuit + 1; //counting biscuit
					}
					else if ((param->threadNo == 3) && (strstr(&line, chips) != NULL)) {
						param->chips = param->chips + 1; //counting chips
					}
					else if ((param->threadNo == 4) && (strstr(&line, coke) != NULL)) {
						param->coke = param->coke + 1; //counting coke
					}
					fgets(line, sizeof(line), param->file);
				}
			}
		}
		fclose(param->file);
	}
		else {
			printf("Could not open the file.");
		}
	

	//system("pause");
	return 1;
}




