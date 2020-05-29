#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <locale.h>

#define N 2

static int turn; 
static int interested[N]; 

void enter_region(int thread) 
{
	int other; 
	other = 1 - thread; 
	interested[thread] = TRUE; 
	turn = thread; 
	while (turn == thread && interested[other] == TRUE); 
}

void leave_region(int thread) 
{
	interested[thread] = FALSE; 
}

static volatile long variable;

DWORD WINAPI Thread(PVOID pvContext)
{
	for (int i = 0; i < 100000; i++)
	{
		enter_region((int)pvContext);
		variable++;
		leave_region((int)pvContext);
	}
	return 0;
}

int main(void)
{
	setlocale(LC_ALL, "Russian");
	HANDLE hArr[N];
	HANDLE hCrProcess;
	int i, ThreadCounter = 0;
	hCrProcess = GetCurrentProcess(); 
	SetPriorityClass(hCrProcess, NORMAL_PRIORITY_CLASS); 
	SetProcessPriorityBoost(hCrProcess, TRUE); 

	for (i = 0; i < N; i++)
		{
			hArr[i] = CreateThread(NULL, 0, Thread, (PVOID)i, 0, NULL);
			ThreadCounter++;
			///////

			if(ThreadCounter == 1) 
			{ 
				printf("Первый поток создан\n");
				printf("'Время'(Кол-во циклов) нахождения - %d\n", variable);
			}
			
			if (ThreadCounter == 2) 
			{ 
				printf("Второй поток создан\n"); 
				printf("'Время'(Кол-во циклов) нахождения - %d\n", variable );
			}
			
			///////
			if (NULL == hArr[i])
				{
					printf("Ошибка создания потока!!!");
					printf("Нажмите любую кнопку");
					_getch();
				}
			///////
		}

	WaitForMultipleObjects(i, hArr, TRUE, INFINITE);

	for (i = 0; i < N; i++)
		{
			CloseHandle(hArr[i]); 
		}

	printf("Количество потоков - %d\n", ThreadCounter);
	printf("Нажмите любую кнопку");
	_getch();
	return 0;
}