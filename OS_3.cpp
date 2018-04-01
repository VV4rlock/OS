// OS_3.cpp: определяет точку входа для консольного приложения.
//


#include "stdafx.h"
#include <windows.h>
#include <tchar.h>
#include <strsafe.h>
#include <algorithm>
#include <iostream>

#define BASE 5+1;


DWORD WINAPI MyThreadFunction(LPVOID lpParam);

using namespace std;



int* T;
int n,N;

int _tmain()
{
	cin >> n;
	N = n;
	T = new int[n];
	srand(GetTickCount());
	int check = 0;
	for (int i = 0; i < n; i++)
	{
		T[i] = rand() % BASE;
		check += T[i];
		cout << T[i]<<' ';
	}
	cout << endl;
	int MAX_THREADS = (n >> 1);
	HANDLE  *hThreadArray = new HANDLE[MAX_THREADS];

	// Create MAX_THREADS worker threads.

	while (n > 1) {
		int start = n & 1?1:0;
		n = n >> 1;
		
		for (int i=0; i < n; i++)
		{
			hThreadArray[i] = CreateThread(0, 0, MyThreadFunction, (LPVOID)(i+start), 0, 0);
		}
		WaitForMultipleObjects(n, hThreadArray, TRUE, INFINITE);
		if (start) n++;
		for (int i = 0; i < n; i++)
		{
			CloseHandle(hThreadArray[i]);
		}
		
		for (int i = 0; i < n; i++)
		{
			cout << T[i] << ' ';
		}
		cout << endl;
	}
	//T[0] += T[1];

	cout<<"right: "<<check <<" result: "<<T[0]<< endl;
	delete[] hThreadArray;
	delete[] T;
	return 0;
}


DWORD WINAPI MyThreadFunction(LPVOID lpParam)
{
	int i = (int)lpParam;
	T[i] += T[i + n];
	return 0;
}

