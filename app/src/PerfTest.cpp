#include <iostream>
#include <algorithm>
#include <atomic>
#include <vector>


#define ALWAYS_INLINE inline __attribute__((always_inline))

struct CpuId
{
  uint32_t eax;
  uint32_t ebx;
  uint32_t ecx;
  uint32_t edx;
};

struct Counter
{
  static ALWAYS_INLINE uint64_t rdtsc()
  {
      uint64_t eax = 0;
      uint64_t edx = 0;
      asm volatile( "rdtsc" : "=a"(eax), "=d"(edx) );
      return (edx << 32) + eax;
  }

  static ALWAYS_INLINE uint64_t rdtscp()
  {
      uint64_t eax = 0;
      uint64_t edx = 0;
      asm volatile( "rdtsc" : "=a"(eax), "=d"(edx) :: "ecx" );
      return (edx << 32) + eax;
  }

  static ALWAYS_INLINE CpuId getCpuId(uint32_t leaf, uint32_t subleaf = 0)
  {
      CpuId r;
      __asm__ __volatile__("cpuid"
        : "=a"(r.eax),
          "=b"(r.ebx),
          "=c"(r.ecx),
          "=d"(r.edx)
        : "a"(leaf),
          "c" (subleaf)
        : );
      return r;
  }

};

std::atomic<uint64_t> my_atomic1(0);
std::atomic<uint64_t> my_atomic2(0);
//volatile uint64_t my_atomic1(0);
//volatile uint64_t my_atomic2(0);

struct A {
    explicit A(size_t _a, const char* const _s) : a(_a), s(_s) {}
    //explicit A(size_t _a) : a(_a) {}
    //explicit A(size_t _a, char _suffix = 0) : a(_a), suffix(_suffix) {}
    //char suffix;
    size_t a;
    std::string s;
};


void f() {
    throw int(0);
}

int main()
{
    size_t count = 10000;
    static std::vector<size_t> result;
    result.reserve(count);

    for (size_t i = 0; i < count; ++i) {
        std::vector<A> a;
        a.reserve(1000);

        asm volatile ("" ::: "memory");
        uint64_t p1 = Counter::rdtscp();
        asm volatile ("" ::: "memory");

        for (size_t j = 0; j < 1000; ++j) {
            //a.emplace_back(j, 'A');
            //a.emplace_back(j, "Alexey Egorov45");
            //my_atomic1.fetch_add(1);
            //my_atomic1.fetch_add(1);
            //++my_atomic1;
            //++my_atomic1;
            //++my_atomic1;
            //sched_yield();
            try {
                f();
            }
            catch(int i) {

            }
        }

        asm volatile ("" ::: "memory");
        uint64_t p2 = Counter::rdtscp();
        asm volatile ("" ::: "memory");

        result.push_back(p2 - p1);
    }

    std::sort(result.begin(), result.end());
    std::cout << my_atomic1 << " " << my_atomic2 << std::endl;
    std::cout <<
        result[0] << " " <<
        result[100] << " " <<
        result[500] << " " <<
        result[1000] << " " <<
         result[2000] << " " <<
         result[3000] << " " <<
         result[4000] << " " <<
         result[5000] << " " <<
         result[6000] << " " <<
         result[7000] << " " <<
         result[8000] << " " <<
         result[9000] << " " <<
         result[9500] << " " <<
         result[9900] << " " <<
         result[9999] << std::endl;

    //int* p; //= new int{0};
    //std::cout << *p << std::endl;
    //delete p;

    //asm volatile ("" ::: "memory");
    //uint64_t s = Counter::rdtscp();
    //p = new int{1};
    //Counter::rdtscp();
    //Counter::getCpuId(0);
    //for (uint64_t j = 0; j < 1000000000; ++j)
    //{
    //    ++i;
    //}
    //asm volatile ("lfence" ::: "memory");
    //asm volatile ("sfence" ::: "memory");
    //Counter::rdtsc();

    //uint64_t e = Counter::rdtscp();
    //asm volatile ("" ::: "memory");
    //std::cout << e - s << std::endl;
    //std::cout << *p << std::endl;

    return 0;
}
