// OS7.cpp: ���������� ����� ����� ��� ����������� ����������.
//

#include "stdafx.h"
#include <iostream>
#include <algorithm>
#include <windows.h>

using namespace std;

HANDLE* sticks;
HANDLE chairs;

DWORD WINAPI philosopher_thread(LPVOID);
int n, k;

int main()
{
	setlocale(LC_ALL, "Russian");
	cin>>n>>k;
	chairs = CreateSemaphore(NULL, n - 1, n - 1, NULL);
	HANDLE * philosophers = new HANDLE[n];
	sticks = new HANDLE[n];
	for (int i = 0; i<n; i++) {
		sticks[i] = CreateMutex(NULL, FALSE, NULL);
	}
	for(int i=0;i<n;i++){
		philosophers[i] = CreateThread(NULL, 0, philosopher_thread, (LPVOID)i, 0, NULL);
	}

	WaitForMultipleObjects(n, philosophers, TRUE, INFINITE);
	for (int i = 0; i < n; i++)
	{
		CloseHandle(philosophers[i]);
		CloseHandle(sticks[i]);
	}

	CloseHandle(chairs);
	delete[] philosophers;
	delete[] sticks;

    return 0;
}

DWORD WINAPI philosopher_thread(LPVOID lpParam){
	int number = (int)lpParam;
	srand(GetTickCount()+number);
	int left = number;
	int right = (number + 1) % n;
	int time = 0;
	for(int din=0;din<k;din++){
		time= rand() % 5 + 1;
		printf("������� %d ���������� %d ������\n",number,time);
		Sleep(time*1000);
		
		WaitForSingleObject(chairs, INFINITE);
		//printf("������� %d ��� �� ��������� ����\n", number, time);
		
		WaitForSingleObject(sticks[left], INFINITE);
		printf("������� %d ���� ����� �������\n", number);

		WaitForSingleObject(sticks[right], INFINITE);
		printf("������� %d ���� ������ �������\n", number);

		time = rand() % 3 + 1;
		printf("������� %d ������� %d ������\n", number,time);
		Sleep(time * 1000);

		ReleaseMutex(sticks[left]);
	//	printf("������� %d ������� ����� �������\n", number);
	
		ReleaseMutex(sticks[right]);
	//	printf("������� %d ������� ������ �������\n", number);

	//	printf("������� %d ����� ��-�� �����\n", number);
		ReleaseSemaphore(chairs, 1, NULL);
	}
	return 0;
}