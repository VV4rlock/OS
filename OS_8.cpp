// OS_8.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <windows.h>
#include <queue>
#include <stack>
#include <iostream>

using namespace std;

DWORD WINAPI client(LPVOID lpParam);
DWORD WINAPI barber(LPVOID lpParam);

queue<int> clientTable;
queue<int> barberTable;

HANDLE blockClientTable;
HANDLE blockBarberTable;

int clientsCount;
int haircutsCount;
int barbersCount;
int placesCount;

HANDLE chairReady;
HANDLE * ticketIsReadyForHaircut;
HANDLE* haircutEnded;
HANDLE* clientHasGone;
HANDLE onBarberTable;

int main()
{
	setlocale(LC_ALL, "Russian");
	cout << "Введите количесво клиентов, стрижек, парикмахеров, мест на диване:\n";
	cin >> clientsCount>> haircutsCount>> barbersCount>>placesCount;
	blockBarberTable = CreateMutex(0,0,0);
	blockClientTable = CreateMutex(0,0,0);

	chairReady = CreateSemaphore(0, barbersCount, barbersCount, 0);
	onBarberTable = CreateSemaphore(0, 0, barbersCount, 0);

	ticketIsReadyForHaircut = new HANDLE[barbersCount+placesCount];
	haircutEnded= new HANDLE[barbersCount + placesCount];
	clientHasGone = new HANDLE[barbersCount + placesCount];
	for(int i = 0;i<barbersCount+placesCount;i++){
		haircutEnded[i]= CreateSemaphore(0, 0, 1, 0);
		ticketIsReadyForHaircut[i] = CreateSemaphore(0, 0, 1, 0);
		clientHasGone[i]= CreateSemaphore(0, 0, 1, 0);
		clientTable.push(i);
	}

	HANDLE * barbers = new HANDLE[barbersCount];
	for(int i=0;i<barbersCount;i++){
		barbers[i] = CreateThread(0, 0, barber, (LPVOID)i, 0, 0);
	}
	HANDLE * clients = new HANDLE[clientsCount];
	for (int i = 0; i<clientsCount; i++) {
		clients[i] = CreateThread(0, 0, client, (LPVOID)i, 0, 0);
	}
	WaitForMultipleObjects(clientsCount, clients, TRUE, INFINITE);

	for (int i = 0; i<barbersCount; i++) {
		CloseHandle(barbers[i]);
	}
	for (int i = 0; i<barbersCount; i++) {
		CloseHandle(clients[i]);
	}
	for (int i = 0; i<barbersCount + placesCount; i++) {
		CloseHandle(haircutEnded[i]);
		CloseHandle(ticketIsReadyForHaircut[i]);
		CloseHandle(clientHasGone[i]);
	}
	CloseHandle(blockBarberTable);
	CloseHandle(blockClientTable);
	CloseHandle(chairReady);
	CloseHandle(onBarberTable);

	delete[] clientHasGone;
	delete[] ticketIsReadyForHaircut;
	delete[] haircutEnded;
	delete[] barbers;
	delete[] clients;
    return 0;
}

DWORD WINAPI client(LPVOID lpParam) {
	int id = (int)lpParam;
	srand(GetTickCount() + id);
	int ticket,h=0;
	while(h<haircutsCount){
		WaitForSingleObject(blockClientTable, INFINITE);
		if (!clientTable.empty())
		{
			ticket = clientTable.front();
			clientTable.pop();
			ReleaseMutex(blockClientTable);
			printf("Посетитель %d зашел в парикмахерскую и взял номерок %d\n", id, ticket);

			WaitForSingleObject(chairReady, INFINITE);

			WaitForSingleObject(blockBarberTable, INFINITE);
			barberTable.push(ticket);
			ReleaseSemaphore(onBarberTable,1,0);
			printf("Посетитель %d садится в кресло парикмахера\n", id);
			ReleaseMutex(blockBarberTable);
			

			ReleaseSemaphore(ticketIsReadyForHaircut[ticket],1,0);

			WaitForSingleObject(haircutEnded[ticket],INFINITE);

			printf("Посетитель %d постригся и уходит из парикмахерской\n", id);
			ReleaseSemaphore(clientHasGone[ticket], 1, 0);
			ReleaseSemaphore(chairReady, 1, 0);
			h++;
		}
		else
		{
			printf("Свободных мест нет, посетитель %d уходит из парикмахерской\n", id);
			ReleaseMutex(blockClientTable);
		}
		Sleep((rand() % 5 + 1) * 1000);
	}
	return 0;
}

DWORD WINAPI barber(LPVOID lpParam){
	int id = (int)lpParam;
	srand(GetTickCount()-id);
	int ticket;
	while(true){
		WaitForSingleObject(onBarberTable, INFINITE);

		WaitForSingleObject(blockBarberTable, INFINITE);
		ticket = barberTable.front();
		barberTable.pop();
		ReleaseMutex(blockBarberTable);

		WaitForSingleObject(ticketIsReadyForHaircut[ticket], INFINITE);
		printf("Парикмахер %d стрижет клиента с номерком %d\n", id, ticket);
		Sleep((rand() % 3 + 1) * 1000);
		ReleaseSemaphore(haircutEnded[ticket], 1, 0);

		WaitForSingleObject(blockClientTable, INFINITE);
		clientTable.push(ticket);
		ReleaseMutex(blockClientTable);

		WaitForSingleObject(clientHasGone[ticket], INFINITE);
		printf("Парикмахер %d готов принять следующего посетителя\n", id, ticket);
	}
	return 0;
}