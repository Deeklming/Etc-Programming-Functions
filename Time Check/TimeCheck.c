#include <stdio.h>
#include <time.h>

//linux 전용
//-.-;;

//windows 전용
#include <Windows.h>


void use_code(){
    int A = 0;
    for(int i=0; i<1000000; ++i){
        for(int j=0; j<1000; ++j){
            A++;
            A--;
        }
    }
}

void on_linux(){
    //sec clock() 기본 측정 - CPU 클럭 수 측정, CPU가 점유하지 않은 sleep() 등 제외
    clock_t cstart, cend;
    cstart = clock();
    use_code();
    cend = clock();
    printf("linux_clock_t_sec: %lf sec\n", (double)(cend - cstart)/CLOCKS_PER_SEC);

    //ms clock() 기본 측정 - CPU 클럭 수 측정, CPU가 점유하지 않은 sleep() 등 제외
    clock_t cmstart, cmend;
    cmstart = clock();
    use_code();
    cmend = clock();
    printf("linux_clock_t_ms: %lf ms\n", (double)(cmend - cmstart));

    //clock_gettime() 측정
    //CLOCK_REALTIME - 시스템 시간이용해서 시스템 시간에 문제가 생기면 정확하지 않음
    //CLOCK_MONOTONIC - 특정 시점(부팅 이후 시간)부터 시간측정, 시스템 관리자가 초기화 가능
    //CLOCK_BOOTTIME - MONOTONIC에 시스템 절전 대기 시간 포함
    //CLOCK_PROCESS_PROCESS_CPUTIME_ID - 프로세스당 모든 쓰레드들의 CPU 시간
    //CLOCK_THREAD_CPUTIME_ID - 특정 쓰레드 CPU 시간
    struct timespec tsstart, tsend;
    clock_gettime(CLOCK_MONOTONIC, &tsstart);
    use_code();
    clock_gettime(CLOCK_MONOTONIC, &tsend);
    printf("linux_clock_gettime: %ld sec, %ld ns, %ld ms\n",
        tsend.tv_sec-tsstart.tv_sec, tsend.tv_nsec-tsstart.tv_nsec, (tsend.tv_nsec-tsstart.tv_nsec)/1000);
}

void on_windows(){
    //sec clock() 기본 측정 - 전체 동작 시간 측정
    clock_t cstart, cend;
    cstart = clock();
    use_code();
    cend = clock();
    printf("windows_clock_t_sec: %lf sec\n", (double)(cend - cstart)/CLOCKS_PER_SEC);

    //ms clock() 기본 측정 - 전체 동작 시간 측정
    clock_t cmstart, cmend;
    cmstart = clock();
    use_code();
    cmend = clock();
    printf("windows_clock_t_ms: %lf ms\n", (double)(cmend - cmstart));

    //sec QueryPerformance 측정
    LARGE_INTEGER Start, End, Frequency;
    QueryPerformanceFrequency(&Frequency);
	QueryPerformanceCounter(&Start);
    use_code();
    QueryPerformanceCounter(&End);
    double Time = (double)(End.QuadPart - Start.QuadPart) / Frequency.QuadPart;
    printf("windows_time_sec : %lf (sec)\n", Time);
}

int main(){
    //sec 기본 측정
    time_t tstart, tend;
    tstart = time(NULL);
    use_code();
    tend = time(NULL);
    printf("basic_time_t_sec: %lf sec\n", (double)(tend - tstart));

    on_linux();
    on_windows();

    return 0;
}