#include <pthread.h>
#include <stdio.h>
#include <string.h>

#define MAXCNT 100000000
int count = 0;//멀티 스레드시 전역 변수나 정적 변수를 사용할 때 앞에 __thread를 붙여주면 스레드별 따로 존재하게 되어 메모리 공유 문제가 생기지 않음

void *MyThread1(void *arg){//동기화 없이
    for(int i=0; i<MAXCNT; i++) count += 2;
    return 0;
}
void *MyThread2(void *arg){//동기화 없이
    for(int i=0; i<MAXCNT; i++) count -= 2;
    return 0;
}

pthread_mutex_t mutex;//뮤텍스 동기화용
void *MyThread3(void *arg){//Mutex 동기화
    for(int i=0; i<MAXCNT; i++){
        pthread_mutex_lock(&mutex);//뮤텍스 진입
        count += 2;
        pthread_mutex_unlock(&mutex);//뮤텍스 나오기
    }
    return 0;
}
void *MyThread4(void *arg){//Mutex 동기화
    for(int i=0; i<MAXCNT; i++){
        pthread_mutex_lock(&mutex);//뮤텍스 진입
        count -= 2;
        pthread_mutex_unlock(&mutex);//뮤텍스 나오기
    }
    return 0;
}


//Condition Variable 동기화용
#define BUF_SIZE 10
pthread_cond_t writeCond;
pthread_cond_t readCond;
pthread_mutex_t writeMutex;//조건 변수는 뮤텍스와 함께 사용해야 함
pthread_mutex_t readMutex;
int writeDone = 0;//조건 변수만으로 스레드 한 개만 깨운다는 보장이 없어서 쓰기 완료 변수 선언
int readDone = 0;//조건 변수만으로 스레드 한 개만 깨운다는 보장이 없어서 읽기 완료 변수 선언
int buf[BUF_SIZE];

void *WriteThreadCV(void *arg){
    for(int i=1; i<=50; i++){
        //읽기 완료 대기
        pthread_mutex_lock(&readMutex);
		while(readDone==0) pthread_cond_wait(&readCond, &readMutex);//pthread_cond_wait로 읽기 완료를 기다림
		readDone = 0;//잠시 1이었음을 감지 못하게 만듬
		pthread_mutex_unlock(&readMutex);

        //공유 버퍼에 데이터 저장
        for(int j=0; j<BUF_SIZE; j++) buf[j] = i;

        //쓰기 완료 알림
		pthread_mutex_lock(&writeMutex);
		writeDone = 1;
		pthread_mutex_unlock(&writeMutex);
        pthread_cond_signal(&writeCond);//pthread_cond_signal로 한 개 이상의 스레드에 쓰기 완료를 알림
    }
    return 0;
}

void *ReadThreadCV(void *arg){
    while(1){
        //쓰기 완료 대기
    	pthread_mutex_lock(&writeMutex);
		while(writeDone==0) pthread_cond_wait(&writeCond, &writeMutex);//pthread_cond_wait로 쓰기 완료를 기다림
		writeDone = 0;//잠시 1이었음을 감지 못하게 만듬
		pthread_mutex_unlock(&writeMutex);

        //읽은 데이터 출력 후 버퍼 초기화
        printf("Thread %4d\t", (int)pthread_self());
        for(int i=0; i<BUF_SIZE; i++) printf("%3d ", buf[i]);
        printf("\n");
        memset(buf, 0, sizeof(buf));//오류 확인용

        //읽기 완료 알림
        pthread_mutex_lock(&readMutex);
		readDone = 1;
		pthread_mutex_unlock(&readMutex);
		pthread_cond_signal(&readCond);//pthread_cond_signal로 한 개 이상의 스레드에 읽기 완료를 알림
    }
    return 0;
}

int main(int argc, char* argv[]){
    //동기화 없는 스레드 생성
    pthread_t td1[2];
    pthread_create(&td1[0], NULL, MyThread1, NULL);
	pthread_create(&td1[1], NULL, MyThread2, NULL);
    
    //스레드 종료 대기
    pthread_join(td1[0], NULL);
	pthread_join(td1[1], NULL);
	
    //동기화 하지 않고 출력
    printf("count = %d\n", count);


	count = 0;
    //뮤텍스 초기화
    pthread_mutex_init(&mutex, NULL);
    
    //Mutex 동기화를 사용할 스레드 생성
    pthread_t td2[2];
    pthread_create(&td2[0], NULL, MyThread3, NULL);
	pthread_create(&td2[1], NULL, MyThread4, NULL);

    //스레드 종료 대기
    pthread_join(td2[0], NULL);
	pthread_join(td2[1], NULL);

    //뮤텍스 삭제
    pthread_mutex_destroy(&mutex);

    //동기화 된 출력
    printf("sync count = %d\n", count);


    //Condition Variable 동기화
    //조건 변수와 뮤텍스 생성
    pthread_cond_init(&writeCond, NULL);//비신호 상태로 시작
    pthread_cond_init(&readCond, NULL);//비신호 상태로 시작
	pthread_mutex_init(&writeMutex, NULL);
	pthread_mutex_init(&readMutex, NULL);

    //Condition Variable 동기화용 스레드 생성
    pthread_t td3[3];
    pthread_create(&td3[0], NULL, WriteThreadCV, NULL);//쓰기용 스레드
	pthread_create(&td3[1], NULL, ReadThreadCV, NULL);//읽기용 스레드
	pthread_create(&td3[2], NULL, ReadThreadCV, NULL);//읽기용 스레드

    //읽기 완료 알림
    pthread_mutex_lock(&readMutex);
	readDone = 1;
	pthread_mutex_unlock(&readMutex);
	pthread_cond_signal(&readCond);//pthread_cond_signal은 한 개 이상의 스레드에 읽기 완료를 알림
    //하나가 아닌 다수에게 신호를 보내려면 pthread_cond_broadcast()를 사용하면 됨

    //스레드 종료 대기
    pthread_join(td3[0], NULL);
	pthread_join(td3[1], NULL);
	pthread_join(td3[2], NULL);
    
    //조건 변수와 뮤텍스 제거
    pthread_cond_destroy(&writeCond);
	pthread_cond_destroy(&readCond);
	pthread_mutex_destroy(&writeMutex);
	pthread_mutex_destroy(&readMutex);
    return 0;
}
