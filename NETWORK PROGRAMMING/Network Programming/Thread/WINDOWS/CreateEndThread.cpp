#include <windows.h>
#include <stdio.h>
#include <process.h> //_beginthreadex(), _endthreadex()

struct Point3D{
    int x, y, z;
};

DWORD WINAPI MyThread(LPVOID arg){//void형 포인터
    Sleep(1000);
    Point3D *pt = (Point3D*)arg;
    printf("MyThread %lu: %d, %d, %d\n", GetCurrentThreadId(), pt->x, pt->y, pt->z);
    return 0;//함수 리턴으로 스레드 종료
}

unsigned __stdcall MyThread2(void *arg){//void형 포인터
    Sleep(1000);
    Point3D *pt = (Point3D*)arg;
    printf("MyThread2 %lu: %d, %d, %d\n", GetCurrentThreadId(), pt->x, pt->y, pt->z);
    _endthreadex(0);//스레드 종료, 아래 리턴문과 같은 효과로 선택해서 사용
    return 0;//함수 리턴으로 스레드 종료
}

int main(int argc, char *argv[]){
    //단일 스레드 환경에서만 가능
    Point3D pt1 = { 1,2,3 };
    HANDLE hThread1 = CreateThread(NULL, 0, MyThread, &pt1, 0, NULL);//스레드 생성
    if(hThread1==NULL) return 1;
    CloseHandle(hThread1);//핸들을 닫는 것이고 스레드 종료가 아님

    Point3D pt2 = { 4,5,6 };
    HANDLE hThread2 = CreateThread(NULL, 0, MyThread, &pt2, 0, NULL);
    if(hThread2==NULL) return 1;
    CloseHandle(hThread2);

    //멀티 스레드 환경에서 가능, 주로 이 방식을 사용함
    Point3D pt3 = { 7,8,9 };
    HANDLE hThread3 = (HANDLE)_beginthreadex(NULL, 0, MyThread2, &pt3, 0, NULL);
    if(hThread3==NULL) return 1;
    CloseHandle(hThread3);

    printf("main thread id: %lu\n", GetCurrentThreadId());
    Sleep(2000);
    return 0;//함수 리턴으로 스레드 종료
}