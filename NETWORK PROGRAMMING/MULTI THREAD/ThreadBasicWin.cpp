#include <windows.h>//CreateThread(), CloseHandle()
#include <process.h>//_beginthreadex(), _endthreadex()
#include <stdio.h>

struct Point3D
{
	int x, y, z;
};

DWORD WINAPI MyThread1(LPVOID arg)//싱글 스레드용
{
	Sleep(1000);
	Point3D *pt = (Point3D *)arg;
	printf("MyThread1(): %lu - %d, %d, %d\n", GetCurrentThreadId(), pt->x, pt->y, pt->z);
	return 0;
}

unsigned __stdcall MyThread2(void *arg)//멀티 스레드용
{
	Sleep(1000);
	Point3D *pt = (Point3D *)arg;
	printf("MyThread2(): %lu - %d, %d, %d\n", GetCurrentThreadId(), pt->x, pt->y, pt->z);
    _endthreadex(0);//아래의 return과 같은 기능이라 생략 가능
	return 0;
}

DWORD WINAPI MyThread3(LPVOID arg)//스레드 우선순위 확인용
{
    unsigned long z=0;
	while(z!=MAXUINT32){
		z+=1;
	};
	return 0;
}

int sum = 0;
DWORD WINAPI MyThread4(LPVOID arg)//스레드 일시정지와 재시작용
{
    int n = (int)(long long)arg;
	for(int i=0; i<=n; i++) sum+=i;
	return 0;
}

int main(int argc, char *argv[])
{
    Point3D pt1 = { 10, 20, 30 };
	HANDLE hThread1 = CreateThread(NULL, 0, MyThread1, &pt1, 0, NULL);//첫 번째 스레드
	if (hThread1 == NULL) return 1;
	CloseHandle(hThread1);//핸들을 닫는다고 해서 스레드가 종료하는 것이 아님

	Point3D pt2 = { 40, 50, 60 };
	HANDLE hThread2 = (HANDLE)_beginthreadex(NULL, 0, MyThread2, &pt2, 0, NULL);//두 번째 스레드
	if (hThread2 == NULL) return 1;
	CloseHandle(hThread2);//빨리 닫을수록 좋음

	printf("main() Thread: %lu\n", GetCurrentThreadId());
	Sleep(2000);

    //스레드 우선순위 변경
    printf("Thread Priority Range: %d ~ %d\n", THREAD_PRIORITY_IDLE, THREAD_PRIORITY_TIME_CRITICAL);
    SYSTEM_INFO si;
    GetSystemInfo(&si);//CPU 개수 얻기위해
    for(int i=0; i<(int)si.dwNumberOfProcessors; i++){
        HANDLE hThread3 = CreateThread(NULL, 0, MyThread3, NULL, 0, NULL);
        SetThreadPriority(hThread3, THREAD_PRIORITY_ABOVE_NORMAL);//높은 우선순위 설정
        CloseHandle(hThread3);
    }
    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_BELOW_NORMAL);//낮은 우선순위 설정
    Sleep(1000);
    printf("main thread running...\n");//잠시 먹통될 수 있음

	//스레드 일시정지와 재시작
	int num = 100;
	HANDLE hThread4 = CreateThread(NULL, 0, MyThread4, (LPVOID)(long long)num, CREATE_SUSPENDED, NULL);
	printf("hThread4 running before: %d\n", sum);
	//SuspendThread(hThread4);//스레드 일시정지, 수동
	ResumeThread(hThread4);//스레드 재시작, 수동, SuspendThread()와 ResumeThread() 호출 횟수가 일치해야 함
	//Sleep(2000);//스레드 일시정지와 재시작이 자동으로 이루어짐
	WaitForSingleObject(hThread4, INFINITE);//단일 스레드 일시정지, INFINITE 옵션으로 무한정 대기
	//멀티스레드 대기하려면 WaitForMultipleObjects()를 사용해야 함
	printf("hThread4 running after: %d\n", sum);
	CloseHandle(hThread4);
	//컨텍스트 전환을 빠르게 하려면 Sleep(0);을 호출하면 됨
	return 0;
}
