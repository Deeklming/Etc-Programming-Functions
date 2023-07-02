#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

struct Point3D
{
	int x, y, z;
};

void *MyThread1(void *arg)//싱글 스레드용
{
	sleep(1);
	Point3D *pt = (Point3D *)arg;
	printf("MyThread1(): thread ID<%lu> - %d, %d, %d\n", pthread_self(), pt->x, pt->y, pt->z);//pthread_self()는 현재 스레드ID 리턴하는 시스템 콜
	return NULL;//스레드 종료 방법 중 하나
}

void *MyThread2(void *arg)//우선순위를 변경하기 때문에 관리자 권한으로 실행해야 함
{
	//스레드 우선순위 높게 설정
    errno = 0;//우선순위 변경시 항상 해줘야 함
	int retval = nice(-20);//nice가 낮을수록 우선순위가 높아짐
	if(retval<0 && errno!=0){
		perror("nice() in MyThread2");
		exit(1);
	}
	
	while(1){
		write(1, ".", 1);
	}
	return 0;
}

int sum = 0;
void *MyThread3(void *arg)//스레드 종료 기다림용
{
    int n = (int)(long long)arg;
	for(int i=1; i<=n; i++) sum+=i;
	return 0;
}

int main(int argc, char *argv[])
{
	//스레드 사용
	int retval;

	pthread_t td1;
    Point3D pt1 = { 10, 20, 30 };
	retval = pthread_create(&td1, NULL, MyThread1, &pt1);//첫 번째 스레드
	if (retval != 0) return 1;
	
	pthread_t td2;
    Point3D pt2 = { 40, 50, 60 };
	retval = pthread_create(&td2, NULL, MyThread1, &pt2);//두 번째 스레드
	if (retval != 0) return 1;

	printf("main() Thread: %lu\n", pthread_self());
	sleep(2);

	//스레드 종료 기다림
	int num = 100;
	pthread_t td4;
	pthread_create(&td4, NULL, MyThread3, (void *)(long long)num);
	
	printf("hThread4 running before: %d\n", sum);
	pthread_join(td4, NULL);//다른 스레드 종료 기다림
	printf("hThread4 running after: %d\n", sum);
	//스스로 스레드 일시 정지와 재시작은 sleep():초, usleep():마이크로초, nanosleep():나노초 함수가 있음

    //스레드 우선순위 변경
    int numOfProcessors = sysconf(_SC_NPROCESSORS_ONLN);//CPU 개수 리턴

	for(int i=0; i<numOfProcessors; i++){//CPU 개수만큼 스레드 생성
		pthread_t td3;
		pthread_create(&td3, NULL, MyThread2, NULL);
	}

	errno = 0;//nice를 사용하기 전 전역 변수 errno를 미리 초기화 해야 함
	retval = nice(19);//nice의 유일한 인수 inc값은 -20 ~ +19까지만 설정 가능
	if(retval<0 && errno!=0){
		perror("nice() in main");
		exit(1);
	}

	sleep(1);
	while(1){
		write(1, "*", 1);//시스템 콜 함수: write(1은 표준(화면) 출력, 출력할 내용, 출력할 바이트 수);
	}
	return 0;
}
