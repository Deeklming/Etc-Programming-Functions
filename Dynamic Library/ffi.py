from ctypes import cdll
import timeit

li = cdll.LoadLibrary(".\\ffi_thread\\target\\release\\ffi_thread.dll")# dll 경로
starttime = timeit.default_timer()

li.process()

endtime = timeit.default_timer()
print(f"실행시간: {endtime-starttime}sec")