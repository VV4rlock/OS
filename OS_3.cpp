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
	T = new int[n+1];
	srand(GetTickCount());
	int check = 0;
	for (int i = 0; i < n; i++)
	{
		T[i] = rand() % BASE;
		check += T[i];
		cout << T[i]<<' ';
	}
	cout << endl;
	int MAX_THREADS = n/2+1;
	HANDLE  *hThreadArray = new HANDLE[MAX_THREADS];

	// Create MAX_THREADS worker threads.
	while (n > 2) {
		if (n & 1) { T[n] = 0; n = n / 2 + 1; }
		else n /= 2;
		for (int i = 0; i < n; i++)
		{
			hThreadArray[i] = CreateThread(0, 0, MyThreadFunction, (LPVOID)i, 0, 0);
		}
		WaitForMultipleObjects(n, hThreadArray, TRUE, INFINITE);
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
	T[0] += T[1];

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

