// OS2.cpp: определяет точку входа для консольного приложения.

#include "stdafx.h"
#include <windows.h>
#include <tchar.h>
#include <strsafe.h>
#include <algorithm>
#include <iostream>


DWORD WINAPI MyThreadFunction(LPVOID lpParam);
void merge(int* from, int indexOfElementInFrom, int* secondArr, int sizeOfSecondArr);
int binSearch(int value, int* arr, int size);
using namespace std;



int* A,* B,* C;
int m, n;

int _tmain()
{
	cin >> n >> m;
	int* T = new int[n + m];
	C = new int[m + n];
	for (int i = 0; i < n + m; i++)
		T[i] = i + 1;
	srand(GetTickCount());
	random_shuffle(T, T + n + m);
	A = T;
	B = T + n;
	sort(A, A + n);
	sort(B, B + m);
	cout << "Array A: ";
	for (int i = 0; i < n; i++)
		cout << A[i] << ' ';
	cout << endl<<"Array B: ";
	for (int i = 0; i < m ; i++)
		cout << B[i] << ' ';
	cout << endl;
	//cout << binSearch(14, A,n);

	int MAX_THREADS = m + n;
	HANDLE  *hThreadArray=new HANDLE[MAX_THREADS];

	// Create MAX_THREADS worker threads.

	for (int i = 0; i<MAX_THREADS; i++)
	{
		hThreadArray[i] = CreateThread(
			NULL,                   // default security attributes
			0,                      // use default stack size  
			MyThreadFunction,       // thread function name
			(LPVOID)i,          // argument to thread function 
			0,                      // use default creation flags 
			0);					// returns the thread identifier 
	}

	WaitForMultipleObjects(MAX_THREADS, hThreadArray, TRUE, INFINITE);

	// Close all thread handles and free memory allocations.
	cout<< "Result:  ";
	for (int i = 0; i<MAX_THREADS; i++)
	{
		CloseHandle(hThreadArray[i]);
		cout << C[i] << ' ';
	}
	cout << endl;
	delete[] hThreadArray;
	delete[] T;
	delete[] C;
	return 0;
}



int binSearch(int value,int* arr, int size){ //возвращает правый элемент
	int first = 0,last=size;
	for (int mid = size / 2; first < last; mid = first/2 + last/2)
		if (value < arr[mid])
			last = mid;
		else
			first = mid+1;
	return last;
}

void merge(int* from, int indexOfElementInFrom, int* secondArr, int sizeOfSecondArr){
	C[
			indexOfElementInFrom +
				binSearch(from[indexOfElementInFrom], secondArr, sizeOfSecondArr)
		] = from[indexOfElementInFrom];
}


DWORD WINAPI MyThreadFunction(LPVOID lpParam)
{
	int i = (int)lpParam;
	if (i < n) { 
		printf("Merging A[%d]\n",i); 
		merge(A, i, B, m); 
	}
	else {
		printf("Merging B[%d]\n", i-n); 
		merge(B, i - n, A, n);
	}
	return 0;
}