//메인 스레드 외에 새로운 스레드를 CPU개수만큼 생성하고 우선순위 레벨을 변경
//OS가 Starvation(기아)상태를 막기 위해 낮은우선순위를 단계적으로 끌어올림
//Sleep(n)도 자동으로 스레드 일시정지, 재시작 효과가 있음
//스레드 자신의 할당된 CPU시간을 포기하고 우선순위가 같은 다른 스레드에 CPU시간을 할당하고 싶으면(빠른 컨텍스트 전환) Sleep(0)를 호출하면 됨
//최대 64개의 스레드를 한번에 종료 기다림을 하고싶으면 WaitForMultipleObjects()를 사용하면 됨
#include <windows.h>
#include <stdio.h>

//스레드 우선순위를 위한
DWORD WINAPI MyThread(LPVOID arg){//void형 포인터
    while(1);
    return 0;//함수 리턴으로 스레드 종료
}

//스레드 일시정지, 재시작, 종료 가다림을 위한
int sum = 0;
DWORD WINAPI MyThread2(LPVOID arg){
    int num = (int)(long long)arg;
    for(int i=0; i<=num; i++) sum += i;
    return 0;//함수 리턴으로 스레드 종료
}

//스레드 제어
int main(int argc, char *argv[]){
    //스레드 우선순위(priority) 변경
    printf("priority: %d ~ %d\n", THREAD_PRIORITY_IDLE, THREAD_PRIORITY_TIME_CRITICAL);
    
    SYSTEM_INFO si;//CPU 개수
    GetSystemInfo(&si);
    
    for(int i=0; i<(int)si.dwNumberOfProcessors; i++){//CPU 개수만큼 스레드 생성
        HANDLE hThread = CreateThread(NULL, 0, MyThread, NULL, 0, NULL);//스레드 생성
        SetThreadPriority(hThread, THREAD_PRIORITY_ABOVE_NORMAL);//우선순위 높게 설정
        CloseHandle(hThread);
    }

    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_BELOW_NORMAL);//우선순위 낮게 설정
    Sleep(1000);
    printf("run main thread!\n");//낮은 우선순위를 OS가 알아서 끌어올려 실행함
    
    //스레드 일시정지, 재시작, 종료 가다림
    int n = 100;
    HANDLE hThread2 = CreateThread(NULL, 0, MyThread2, (LPVOID)(long long)n, CREATE_SUSPENDED, NULL);//스레드 일시정지
    printf("before running thread sum result: %d\n", sum);
    ResumeThread(hThread2);//스레드 재시작
    WaitForSingleObject(hThread2, INFINITE);//스레드 종료 기다림, INFINITE는 무한정 ms
    printf("after running thread sum result: %d\n", sum);
    CloseHandle(hThread2);

    return 0;//함수 리턴으로 스레드 종료
}