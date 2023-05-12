from threading import Thread
import timeit

def work(id, start, end, result):
    t=0
    for i in range(start, end):
        t+=i
    result.append(t)
    print(f"id{id} ok")
    return

starttime = timeit.default_timer()

li=[]
th1 = Thread(target=work, args=(1, 0, 200000000, li))
th2 = Thread(target=work, args=(2, 0, 200000000, li))
th1.start()
th2.start()
th1.join()
th2.join()

endtime = timeit.default_timer()
print(li)
print(f"실행시간: {endtime-starttime}sec")
