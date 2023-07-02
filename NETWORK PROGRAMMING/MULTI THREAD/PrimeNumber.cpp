#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <memory>
#include <mutex>

using namespace std;

const int MaxCount = 800000;//소수 찾는 최대 수
const int ThreadCount = 4;//사용 스레드 개수

bool IsPrimeNumber(int num){//소수 확인
    if(num==1) return false;
    if(num==2 || num==3) return true;
    for(int i=2; i<num-1; ++i){
        if((num%i)==0) return false;
    }
    return true;
}

void Pr(const vector<int>& primes){//찾은 소수들 출력
    for(int i: primes) cout<<i<<endl;
}

int main(){
    int num=1;
    recursive_mutex num_mutex;

    vector<int> primes;
    recursive_mutex primes_mutex;

    auto start = chrono::system_clock::now();

    vector<shared_ptr<thread>> threads;//작동할 워커 스레드

    for(int i=0; i<ThreadCount; ++i){
        shared_ptr<thread> thread3(new thread([&](){
            while(true){//각 스레드의 메인 함수, 값을 가져올 수 있으면 루프를 돔
                int n;
                {
                    lock_guard<recursive_mutex> num_lock(num_mutex);//자동으로 unlock() 해줌
                    n=num;
                    num++;
                }
                if(n>=MaxCount) break;
                if(IsPrimeNumber(n)){
                    lock_guard<recursive_mutex> primes_lock(primes_mutex);
                    primes.push_back(n);
                }
            }
        }));
        threads.push_back(thread3);//일단 스레드 객체를 가지고 있음
    }

    for(auto thread: threads) thread->join();//모든 스레드가 끝날 때까지 기다림

    auto end = chrono::system_clock::now();

    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    cout<<"times: "<<duration<<"(milliseconds)"<<endl;
    Pr(primes);
}