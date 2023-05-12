<h2>Dynamic Library</h2>
<h3>use - rust, python, c#</h4>

<h4>[rust 부분]</h4>

1. cargo new ffi_thread --lib 생성
2. cd ffi_thread && src/lib.rs 파일에 스레드 코드 작성
3. Cargo.toml 파일에 아래 코드 추가
```
    [lib]
    name="ffi_thread"
    crate-type=["dylib"]
```
4. cargo build --release 빌드

<h4>[python3 부분]</h4>

1. thread.py 파일 실행
2. ffi.py 파일에 rust로 만든 ffi_thread.dll 파일경로 설정 후 실행

<h4><결론></h4>
thread.py 파일의 작업량이 ffi_thread.dll보다 훨씬 적음에도 불구하고
실행속도 차이가 심각하게 많이 나오는 것을 알수있다.
</br>
</br>

**---추가---**
<h4>[c# 부분 - dotnet5.0 이상]</h4>

1. mkdir ffi && cd ffi
2. dotnet new console 생성
3. ffi.cs 파일에 있는 코드를 c# 메인파일에 작성
4. DllImport 부분에 ffi_thread.dll 경로 설정
```
[DllImport(@".\ffi_thread.dll")]
```
5. dotnet run 실행
