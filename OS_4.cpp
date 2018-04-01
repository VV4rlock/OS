// OS_4.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <iostream>
#include <windows.h>


#define BASE 5+1

DWORD WINAPI MyThreadFunction(LPVOID lpParam);
int** A;
int *B,*C;
HANDLE* mutexArr;
int m, n, q,m_q,n_q;

using namespace std;
int main() // input m n q
{
	srand(GetTickCount());
	cout << "input m n q: ";
	cin >> m >> n >> q;
	if(m%q!=0 || n%q!=0 || m<1 || n<1)
	{
		cout << "Invalid inpunt!\n";
		return 1;
	}
	m_q = m / q; 
	n_q = n / q;

	A = new int*[m];
	C = new int[m];
	cout << "A:\n";
	for(int i=0;i<m;i++)
	{
		A[i] = new int[n];
		for(int j=0;j<n;j++)
		{
			A[i][j] = rand() % BASE;
			cout << A[i][j] << ' ';
		}
		C[i] = 0;
		cout << '\n';
	}

	cout << "\nB: ";
	B = new int[n];
	for(int i=0;i<n;i++)
	{	
		B[i] = rand() % BASE;
		cout << B[i] << ' ';
	}
	cout << "\n\n\n";

	mutexArr = new HANDLE[q];
	for (int i = 0; i < q; i++)
		mutexArr[i] = CreateMutex(0,0,0);

	int MAX_THREADS = q*q;
	HANDLE  *hThreadArray = new HANDLE[MAX_THREADS];
	for (int i = 0; i<MAX_THREADS; i++)
	{
		hThreadArray[i] = CreateThread(0, 0, MyThreadFunction, (LPVOID)i, 0, 0);
	}
	WaitForMultipleObjects(MAX_THREADS, hThreadArray, TRUE, INFINITE);
	for (int i = 0; i < MAX_THREADS; i++)
		CloseHandle(hThreadArray[i]);


	cout << "\nresult: ";
	for (int i = 0; i < m; i++)
		cout << C[i] << ' ';
	cout << '\n';


	delete[] hThreadArray;
	delete[] mutexArr;
	delete[] C;
	delete[] B;
	for (int i = 0; i < m; i++)
		delete[] A[i];
	delete[] A;

    return 0;
}

DWORD WINAPI MyThreadFunction(LPVOID lpParam)
{
	int index = (int)lpParam; //0...q*q
	int block_row = index / q;
	int block_column = index%q;
	char buf[255];
	char* ptr = buf;
	int * vector = new int[m_q];
	
	int beg_row = block_row * m_q;
	int beg_cul = block_column * n_q;
	ptr+=sprintf_s(ptr,255,"Processing block (%d,%d)\n\tvector: ", block_row, block_column);
	for (int i = 0; i < m_q; i++)
	{
		
		vector[i] = 0;
		for (int r = 0; r < n_q; r++)
			vector[i] += A[i + beg_row][r + beg_cul] * B[r + beg_cul];
		ptr += sprintf_s(ptr,255-(ptr-buf), "%d ", vector[i]);
	}
	WaitForSingleObject(mutexArr[block_row], INFINITE);
	for (int i = 0; i < m_q; i++) {
		C[i + m_q * block_row] += vector[i]; //critical section
	}
	
	ReleaseMutex(mutexArr[block_row]);
	printf("%s\n",buf);
	delete[] vector;

	return 0;
}

