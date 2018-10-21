// OS_proc.cpp: определяет точку входа для консольного приложения.
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
	if(argc!=2){ // смотрим сколько аргументов из командной строки пришло, если не 2 то пока (первый - название проги)
		printf("Usage %s thread_count\n",argv[0]);
		return -1;
	}
	int proc_count=1;
	sscanf_s(argv[1], "%d", &proc_count); //считываем второй аргумент как число с плав.точкой
	HANDLE* hThreads = new HANDLE[proc_count]; // массив указателей на хендлеры(?) процессов

	STARTUPINFO si = { sizeof(si) };
	PROCESS_INFORMATION pi;
	//TCHAR CommandLine[] = L"OS_pi.exe -0.99 0.01 1000000";
	TCHAR CommandLine[256];
	printf("\n");
	double start; //хранит число с которого процесс начинает табулирование
	double step = 2.0 / iter_count * proc_count; //шаг, с которым будет происходить табулирование
	for (int i = 0; i < proc_count; i++) {
		start = -0.99999 + i*step; // вычисляем начало
		_snwprintf_s(CommandLine, 256, L"OS_pi.exe %lf %lf %d\n", start, step, 10000); //создаем строку для запуска процесса табулирования
		CreateProcess(NULL, CommandLine, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi); //создаем процесс
		hThreads[i] = pi.hProcess; //сохраняем хендлер процесса
	}
	WaitForMultipleObjects(proc_count,hThreads,TRUE,INFINITE); // ждем завершения всех процессов

	//объявляем переменные, которые будут хранить время
	FILETIME CreationTime; // время создания процесса
	FILETIME ExitTime;   // время выхода из работы процесса
	FILETIME KernelTime; // время, работы процесса в режиме ядра
	FILETIME UserTime;    // время, работы процесса в режиме пользователя
	SYSTEMTIME kt,ut;            //для перевода времени в нормальный вид
	//время работы складываеться из работы в режиме ядра и работы в режиме пользователя
	for (int i = 0; i<proc_count; i++)
	{
		GetProcessTimes(hThreads[i], &CreationTime, &ExitTime, &KernelTime, &UserTime); //получаем время
		FileTimeToSystemTime(&KernelTime,&kt); //нормальное время работы в режиме ядра
		FileTimeToSystemTime(&UserTime, &ut); //нормальное время работы в режиме пользователя
		printf("The %d process working during %02d s %04d ms\n",i, kt.wSecond+ut.wSecond, kt.wMilliseconds+ut.wMilliseconds); //выводим время работы каждого процесса
		CloseHandle(hThreads[i]); //закрываем хендлеры(так надо вроде))
	}
	system("PAUSE"); //чтобы консолька не закрылась
	delete[] hThreads;
	return 0;
}

