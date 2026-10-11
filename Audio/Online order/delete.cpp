// // Type your code here, or load an example.
#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <atomic>
#include <chrono>

class test{
    public:
    void inc(int tim){
        for(int i=0;i<tim;++i){
            cnt++;
        }
    }
    int get()const{return cnt;}
    void reset(){cnt=0;}
    private:
      int cnt=0;

};
class atom{
    public:
    void inc(int tim){
        for(int i=0;i<tim;++i){
            cnt++;
        }
    }
    int get()const{return cnt.load();}
    void reset(){cnt.store(0);}
    private:
      std::atomic<int>cnt{0};

};
class mut{
public:
void inc(int n){
    for(int i=0;i<n;i++){
        std::lock_guard<std::mutex>lock(mtx);
        cnt++;  
    }

}
int get()const{std::lock_guard<std::mutex>lock(mtx);return cnt;}
void reset(){std::lock_guard<std::mutex>lock(mtx);cnt=0;}
private:
  int cnt=0;
  mutable std::mutex mtx;
};

template <typename CounterType> void demo(const char* name) {
    std::cout << "\n=== Testing " << name << " ===\n";
    
    CounterType counter;
    const int NUM_THREADS = 10;
    const int INCREMENTS_PER_THREAD = 1000000;
    const int EXPECTED = NUM_THREADS * INCREMENTS_PER_THREAD;
    
    // Run 5 times to show variability in the racy version
    for (int run = 0; run < 5; ++run) {
        counter.reset();
        
        auto start = std::chrono::high_resolution_clock::now();
        
        std::vector<std::thread> threads;
        for (int i = 0; i < NUM_THREADS; ++i) {
            threads.emplace_back(&CounterType::inc, &counter, INCREMENTS_PER_THREAD);
        }
        
        for (auto& t : threads) {
            t.join();
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        
        int result = counter.get();
        
        std::cout << "  Run " << (run + 1) << ": got " << result 
                  << " (expected " << EXPECTED << ") "
                  << "Time: " << duration.count() << "ms";
        
        if (result != EXPECTED) {
            std::cout << " RACE!";
        } else {
            std::cout << " ✓";
            // std::cout<<duration.dtype();
        }
        std::cout << "\n";
    }
}

int main() {
    std::cout << "========================================\n";
    std::cout << "C++ RACE CONDITION DEMONSTRATION\n";
    std::cout << "========================================\n";
    std::cout << "Spawning 10 threads, each incrementing 1,000,000 times.\n";
    std::cout << "Expected final count: 10,000,000\n";
    
    demo<test>("UnsafeCounter (NO SYNCHRONIZATION - RACE!)");
    demo<mut>("Mutexbased counter!!");
    demo<atom>("atomic based counter!!");
    return 0;

}