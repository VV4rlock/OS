// OS_proc.cpp: ���������� ����� ����� ��� ����������� ����������.
//

#include "stdafx.h"
#include "windows.h"
#include <time.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int iter_count = 100000;

int main(int argc, char** argv)
{
	if(argc!=2){ // ������� ������� ���������� �� ��������� ������ ������, ���� �� 2 �� ���� (������ - �������� �����)
		printf("Usage %s thread_count\n",argv[0]);
		return -1;
	}
	int proc_count=1;
	sscanf_s(argv[1], "%d", &proc_count); //��������� ������ �������� ��� ����� � ����.������
	HANDLE* hThreads = new HANDLE[proc_count]; // ������ ���������� �� ��������(?) ���������

	STARTUPINFO si = { sizeof(si) };
	PROCESS_INFORMATION pi;
	//TCHAR CommandLine[] = L"OS_pi.exe -0.99 0.01 1000000";
	TCHAR CommandLine[256];
	printf("\n");
	double start; //������ ����� � �������� ������� �������� �������������
	double step = 2.0 / iter_count * proc_count; //���, � ������� ����� ����������� �������������
	for (int i = 0; i < proc_count; i++) {
		start = -0.99999 + i*step; // ��������� ������
		_snwprintf_s(CommandLine, 256, L"OS_pi.exe %lf %lf %d\n", start, step, 10000); //������� ������ ��� ������� �������� �������������
		CreateProcess(NULL, CommandLine, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi); //������� �������
		hThreads[i] = pi.hProcess; //��������� ������� ��������
	}
	WaitForMultipleObjects(proc_count,hThreads,TRUE,INFINITE); // ���� ���������� ���� ���������

	//��������� ����������, ������� ����� ������� �����
	FILETIME CreationTime; // ����� �������� ��������
	FILETIME ExitTime;   // ����� ������ �� ������ ��������
	FILETIME KernelTime; // �����, ������ �������� � ������ ����
	FILETIME UserTime;    // �����, ������ �������� � ������ ������������
	SYSTEMTIME kt,ut;            //��� �������� ������� � ���������� ���
	//����� ������ ������������� �� ������ � ������ ���� � ������ � ������ ������������
	for (int i = 0; i<proc_count; i++)
	{
		GetProcessTimes(hThreads[i], &CreationTime, &ExitTime, &KernelTime, &UserTime); //�������� �����
		FileTimeToSystemTime(&KernelTime,&kt); //���������� ����� ������ � ������ ����
		FileTimeToSystemTime(&UserTime, &ut); //���������� ����� ������ � ������ ������������
		printf("The %d process working during %02d s %04d ms\n",i, kt.wSecond+ut.wSecond, kt.wMilliseconds+ut.wMilliseconds); //������� ����� ������ ������� ��������
		CloseHandle(hThreads[i]); //��������� ��������(��� ���� �����))
	}
	system("PAUSE"); //����� ��������� �� ���������
	delete[] hThreads;
	return 0;
}

