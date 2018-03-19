// OS_pi.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <windows.h>
#include <tchar.h>
#include <strsafe.h>
#include <algorithm>
#include <iostream>


DWORD WINAPI MyThreadFunction(LPVOID lpParam);

using namespace std;
HANDLE ghMutex;
double pi=0;

int main_func()
{
	
	pi = 0;
	int MAX_THREADS = 2;

	ghMutex = CreateMutex(
		NULL,              // default security attributes
		FALSE,             // initially not owned
		NULL);

	HANDLE  *hThreadArray = new HANDLE[MAX_THREADS];
	hThreadArray[0] = CreateThread(NULL, 0, MyThreadFunction, (LPVOID)0, 0, 0);
	hThreadArray[1] = CreateThread(NULL, 0, MyThreadFunction, (LPVOID)1, 0, 0);



	// Create MAX_THREADS worker threads.


	WaitForMultipleObjects(MAX_THREADS, hThreadArray, TRUE, INFINITE);
	printf("Pi=%.7f\n", 4 * pi);

	// Close all thread handles and free memory allocations.
	for (int i = 0; i<MAX_THREADS; i++)
	{
		CloseHandle(hThreadArray[i]);
	}
	delete[] hThreadArray;
	return 0;
}

int _tmain(){
	for(int i=0;i<100;i++)
		main_func();
	return 0;
}



DWORD WINAPI MyThreadFunction(LPVOID lpParam)
{
	int flag = (int)lpParam;
	double fact = 1.0;
	double threadsum = 0.0;
	if(!flag){
		for (int i = 0; i < 5000000; i++,fact=-fact) {
			threadsum += fact / (2 * i + 1);	
		}
	}
	else{
		for (int i = 5000000; i < 10000000; i++, fact=-fact) {
			threadsum += fact / (2 * i + 1);
		}
	}


	WaitForSingleObject(ghMutex, INFINITE);
	pi += threadsum;
	ReleaseMutex(ghMutex);

	return 0;
}






