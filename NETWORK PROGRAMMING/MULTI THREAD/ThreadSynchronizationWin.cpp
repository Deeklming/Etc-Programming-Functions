#include <windows.h>
#include <stdio.h>

#define MAXCNT 100000000
int count = 0;

DWORD WINAPI MyThread1(LPVOID arg){//동기화 없이
    for(int i=0; i<MAXCNT; i++) count += 2;
    return 0;
}
DWORD WINAPI MyThread2(LPVOID arg){//동기화 없이
    for(int i=0; i<MAXCNT; i++) count -= 2;
    return 0;
}

CRITICAL_SECTION cs;//임계 영역 동기화
DWORD WINAPI MyThread3(LPVOID arg){//Critical Section 동기화
    for(int i=0; i<MAXCNT; i++){
        EnterCriticalSection(&cs);//임계 영역 진입
        count += 2;
        LeaveCriticalSection(&cs);//임계 영역 나오기
    }
    return 0;
}
DWORD WINAPI MyThread4(LPVOID arg){//Critical Section은 스레드 처리 순서 못 정함
    for(int i=0; i<MAXCNT; i++){
        EnterCriticalSection(&cs);//임계 영역 진입
        count -= 2;
        LeaveCriticalSection(&cs);//임계 영역 나오기
    }
    return 0;
}


//Event 동기화용
#define BUF_SIZE 10
HANDLE hWriteEvent;//자동 리셋 이벤트용
HANDLE hReadEvent;//자동 리셋 이벤트용
int buf[BUF_SIZE];

DWORD WINAPI WriteThreadEvent(LPVOID arg){
    DWORD retval;
    for(int i=1; i<=50; i++){
        //읽기 완료 대기
        retval = WaitForSingleObject(hReadEvent, INFINITE);//읽기 이벤트가 신호 상태가 되기를 기다림
        if(retval != WAIT_OBJECT_0) break;

        //공유 버퍼에 데이터 저장
        for(int j=0; j<BUF_SIZE; j++) buf[j] = i;

        //쓰기 완료 알림
        SetEvent(hWriteEvent);//쓰기 이벤트를 신호 상태로 만들어 읽기 스레드 중 하나를 대기 상태에서 깨움
    }
    return 0;
}

DWORD WINAPI ReadThreadEvent(LPVOID arg){
    DWORD retval;
    while(1){
        //쓰기 완료 대기
        retval = WaitForSingleObject(hWriteEvent, INFINITE);//쓰기 이벤트가 신호 상태가 되기를 기다림
        if(retval != WAIT_OBJECT_0) break;

        //읽은 데이터 출력 후 버퍼 초기화
        printf("Thread %4lu\t", GetCurrentThreadId());
        for(int i=0; i<BUF_SIZE; i++) printf("%3d ", buf[i]);
        printf("\n");
        memset(buf, 0, sizeof(buf));//오류 확인용

        //읽기 완료 알림
        SetEvent(hReadEvent);//읽기 이벤트를 신호 상태로 만들어 쓰기 스레드를 대기 상태에서 깨움
    }
    return 0;
}

int main(int argc, char* argv[]){
    //동기화 없는 스레드 생성
    HANDLE hThread[2];
    hThread[0] = CreateThread(NULL, 0, MyThread1, NULL, 0, NULL);
    hThread[1] = CreateThread(NULL, 0, MyThread2, NULL, 0, NULL);

    //스레드 종료 대기
    WaitForMultipleObjects(2, hThread, TRUE, INFINITE);

    //동기화 하지 않고 출력
    printf("count = %d\n", count);


    count = 0;
    //임계 영역 초기화
    InitializeCriticalSection(&cs);

    //Critical Section 동기화를 사용할 스레드 생성
    HANDLE hThreadSync[2];
    hThreadSync[0] = CreateThread(NULL, 0, MyThread3, NULL, 0, NULL);
    hThreadSync[1] = CreateThread(NULL, 0, MyThread4, NULL, 0, NULL);

    //스레드 종료 대기
    WaitForMultipleObjects(2, hThreadSync, TRUE, INFINITE);

    //임계 영역 삭제
    DeleteCriticalSection(&cs);

    //동기화 된 출력
    printf("sync count = %d\n", count);


    //Event 동기화
    //이벤트 생성
    hWriteEvent = CreateEvent(NULL, FALSE, FALSE, NULL);//비신호 상태로 시작
    hReadEvent = CreateEvent(NULL, FALSE, FALSE, NULL);//비신호 상태로 시작

    //Event 동기화용 스레드 생성
    HANDLE hThreadEvent[3];
    hThreadEvent[0] = CreateThread(NULL, 0, WriteThreadEvent, NULL, 0, NULL);//쓰기용 스레드
    hThreadEvent[1] = CreateThread(NULL, 0, ReadThreadEvent, NULL, 0, NULL);//읽기용 스레드
    hThreadEvent[2] = CreateThread(NULL, 0, ReadThreadEvent, NULL, 0, NULL);//읽기용 스레드

    //읽기 완료 알림
    SetEvent(hReadEvent);//읽기 이벤트를 비신호 상태에서 신호 상태로 변경
    //BOOL ResetEvent(HANDLE hEvent); 는 신호 상태를 비신호 상태로 변경

    //스레드 종료 대기
    WaitForMultipleObjects(3, hThreadEvent, TRUE, INFINITE);
    
    //이벤트 제거
    CloseHandle(hWriteEvent);
    CloseHandle(hReadEvent);
    return 0;
}