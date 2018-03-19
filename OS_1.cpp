// многочлены.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <windows.h>
#include <tchar.h>
#include <strsafe.h>
#include <iostream>
#include <ctime>

#define BASE 5+1
#define MIN(a,b) (((a) < (b)) ? (a) : (b) )
#define MAX(a,b) (((a) > (b)) ? (a) : (b) )

int *A, *B, *C;
int N, M;

using namespace std;
DWORD WINAPI MyThreadFunction(LPVOID lpParam);

int _tmain()
{
	srand(time(0));
	cout << "N M:";
	cin >> N>> M;
	A = new int[N+1];
	B = new int[M+1];
	C = new int[M + N + 1];
	cout << "First  array: ";
	for (int i = 0; i <= N; i++)
	{
		A[i] = rand() % BASE;
		cout << A[i] << ' ';
		C[i] = 0;
	}
	cout << endl<<"Second array: ";
	for (int i = 0; i <= M; i++)
	{
		B[i] = rand() % BASE;
		cout << B[i] << ' ';
		C[i + N] = 0;
	}
	cout << endl;

	int THREADS_COUNT = M + N + 1;
	HANDLE  *hThreadArray=new HANDLE[THREADS_COUNT];

	// Create MAX_THREADS worker threads.

	for (int i = 0; i < THREADS_COUNT; i++)
	{

		hThreadArray[i] = CreateThread(
			NULL,                   // default security attributes
			0,                      // use default stack size  
			MyThreadFunction,       // thread function name
			(LPVOID)i,          // argument to thread function 
			0,                      // use default creation flags 
			0);   // returns the thread identifier 
	}

	WaitForMultipleObjects(THREADS_COUNT, hThreadArray, TRUE, INFINITE);

	// Close all thread handles and free memory allocations.
	cout << "result array: ";
	for (int i = 0; i<THREADS_COUNT; i++)
	{
		CloseHandle(hThreadArray[i]);
		cout << C[i] << ' ';
	}
	cout << endl;

	delete[] A;
	delete[] B;
	delete[] C;
	delete[] hThreadArray;

	return 0;
}

DWORD WINAPI MyThreadFunction(LPVOID lpParam)
{
	int k = (int)lpParam;
	int min = MIN(k, N);
	for (int i = MAX(0, (k-M)); i <= min; i++)
		C[k] += A[i] * B[k - i];
	return 0;
}