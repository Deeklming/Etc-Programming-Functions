import time

def use_code():
    A = 0
    for i in range(1000000):
        A+=1

#sec 기본 측정
start = time.time()
use_code()
end = time.time()
print("time: ", end-start, "sec")

#sec 시스템 전체 측정(performance counter) - sleep() 포함
startperf = time.perf_counter()
use_code()
time.sleep(1)
endperf = time.perf_counter()
print("time_perf: ", endperf-startperf, "sec")

#sec 현재 프로세스 시스템 + CPU 시간 합계 측정 - sleep() 미포함
startproc = time.process_time()
use_code()
time.sleep(1)
endproc = time.process_time()
print("time_proc: ", endproc-startproc, "sec")

#sec 현재 쓰레드 시스템 + CPU 시간 합계 측정
startthread = time.thread_time()
use_code()
endthread = time.thread_time()
print("time_thread: ", endthread-startthread, "sec")


#ms 기본 측정
startms = time.time()
use_code()
endms = time.time()
print("time_ms: ", (endms-startms)*1000, "ms")

#ms 시스템 전체 측정(performance counter) - sleep() 포함
startperfms = time.perf_counter()
use_code()
time.sleep(1)
endperfms = time.perf_counter()
print("time_perf_ms: ", (endperfms-startperfms)*1000, "ms")

#ms 현재 프로세스 시스템 + CPU 시간 합계 측정 - sleep() 미포함
startprocms = time.process_time()
use_code()
time.sleep(1)
endprocms = time.process_time()
print("time_proc_ms: ", (endprocms-startprocms)*1000, "ms")

#ms 현재 쓰레드 시스템 + CPU 시간 합계 측정
startthreadms = time.thread_time()
use_code()
endthreadms = time.thread_time()
print("time_thread_ms: ", (endthreadms-startthreadms)*1000, "ms")


#ns 기본 측정
startns = time.time_ns()
use_code()
endns = time.time_ns()
print("time_ns: ", endns-startns, "ns")

#ns 시스템 전체 측정(performance counter) - sleep() 포함
startperfns = time.perf_counter_ns()
use_code()
time.sleep(1)
endperfns = time.perf_counter_ns()
print("time_perf_ns: ", endperfns-startperfns, "ns")

#ns 현재 프로세스 시스템 + CPU 시간 합계 측정 - sleep() 미포함
startprocns = time.process_time_ns()
use_code()
time.sleep(1)
endprocns = time.process_time_ns()
print("time_proc_ns: ", endprocns-startprocns, "ns")

#ns 현재 쓰레드 시스템 + CPU 시간 합계 측정
startthreadns = time.thread_time_ns()
use_code()
endthreadns = time.thread_time_ns()
print("time_thread_ns: ", endthreadns-startthreadns, "ns")
