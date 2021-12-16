1 // item status
# Parallelism and Concurrency
For shared-memory scenarios, the *OpenMP* `pragma`-based interface for C++ allows a straightforward "high-level" parallelization of many prominent use cases for parallelization (.e.g, nested for loops) and also provides mechanisms to implement synchronization between parallel running threads (e.g., critical regions or atomic updates). 
OpenMP implementations typically come along with a compiler and support a certain version of the OpenMP standard.
A prominent alternative is `TBB` which supports similar use cases but is shipped as a third-party library (i.e., the integration is not `pragma`-based).


**TBB**
```pmans
#include <tbb/parallel_for.h>
...
auto values = std::vector<double>(10000);
tbb::parallel_for(tbb::blocked_range<int>(0, values.size()),
                  [&](tbb::blocked_range<int> r) {
                    for (auto &&i : r) {
                      values[i] = 5;
                    }
                  });
```
**OpenMP**
```pmans
#include <omp.h>
...
auto values = std::vector<double>(10000);
#pragma omp for
  for (size_t n = 0; n < values.size(); ++n) {
    values[n] = 5;
  }
```


Currently, the C++ standard library provides support for threads and some synchronization primitives. Additionally, `std::atomic` provides a wrapper for atomic types with specialization for integers and pointers.

We will have a look at some important synchronization primitives to illustrate "how many batteries are included" in the standard library w.r.t. parallelism and concurrency.

## std::thread
Constructing a `std::thread` in C++ can look likes this, when using a callable which requires some arguments:
```pmans
      double c = 0;  
      auto /*f*/ callable /*x*/ = [&c](int /*f*/ a /*x*/, int /*f*/ b /*x*/) {
        std::cout << a + b << std::endl;
        --c; // (1) OK? 
        return a + b + c;
      };
      int arg1 = 1;
      int arg2 = 1;
      /*b*/ std::thread /*x*/ thread(/*f*/ callable /*x*/, /*f*/ arg1 /*x*/, /*f*/ arg2 /*x*/); 
      // how costly is creating a thread?
      /*b*/ std::thread /*x*/ thread2(/*f*/ callable /*x*/, /*f*/ arg1 /*x*/, /*f*/ arg2 /*x*/);      
      thread.join();
      thread2.join();
      // (2) c value?
``` 
Here, a function object obtained from a lambda expression is used.
After construction `thread` immediately invokes the callable using the provided arguments in a new thread of execution:
- local variables of "original-scope" are not accessible
- global variables are accessible

> Can variables be made available in the new thread when capturing them as references or by-value?
<!--
> - yes, capturing with lambda works ("lifting" into new thread context)
-->

The construction of a thread does not support passing references as constructor arguments, this is why the following is not immediately possible:
```pmans
      auto /*f*/ callable /*x*/ = []int /*f*/ &a /*x*/, int /*f*/ &b /*x*/) { ... };
      int arg1 = 2;
      int arg2 = 2;
      /*b*/ std::thread /*x*/ thread(/*f*/ callable /*x*/, /*f*/ arg1 /*x*/, /*f*/ arg2 /*x*/); // does not compile
```
> How to overcome this problem if we want to pass a reference (e.g., a large object to be manipulated by the thread)?
<!--
> ```pmans
> std::thread /*x*/ thread(/*f*/ callable /*x*/, /*f*/ std::ref(arg1) /*x*/, /*f*/ std::ref(arg2) /*x*/);
> ```
-->
As we have seen above, a std::thread requires an explicit `.join()` before the application ends. 
A lightweight wrapper can be used if desired to automatically join the thread when the variable is destructed:
```pmans
struct /*f*/ jthread /*x*/ {
   /*b*/ std::thread /*x*/ t;
  template <class... Args>
  explicit /*f*/ jthread /*x*/(Args &&... args) : t(std::forward<Args>(args)...) {}
  ~/*f*/ jthread /*x*/() { t.join(); }
};
```
Up to now we saw how to create threads which execute a provided callable but we did not really care about the returned value of the callable.

```
clang++ -std=c++17 thread.cpp -O3 -pthread && ./a.out
```

## std::future and std::promise
The approach to conveniently observe and obtain return values of callables executed in an another thread provided by the standard library are `std::promise` and `std::future`.
Let's see an example which does not even involve different threads:
```pmans
  auto /*f*/ promise /*x*/ = std::promise<int>(); // create promise: no future attached
  auto /*f*/ future /*x*/ = /*f*/ promise /*x*/.get_future(); // paired with future
  {
    auto status = /*f*/ future /*x*/./*b*/ wait_for /*x*/(std::chrono::milliseconds(1));
    assert(std::future_status::timeout == status);
  }
  promise./*b*/ set_value /*x*/(2); // promise fullfilled
  {
    auto status = /*f*/ future /*x*/./*b*/ wait_for /*x*/(std::chrono::milliseconds(1));
    assert(std::future_status::ready == status);
    /*f*/ future /*x*/./*b*/ wait /*x*/(); // blocking
    auto value = /*f*/ future /*x*/./*b*/ get /*x*/(); // get 2
  }
```
> What is the basic idea of std::promise/std::future pair, how might an implementation look like?
> - there is always a pair future/promise
> - promise side can set a value
> - future side can wait() and get()
> - typically: promise is set in different thread than future

Now let's see the same example when using a thread to "fulfill the promise":
```pmans
  auto /*f*/ promise /*x*/ = std::promise<int>();
  auto /*f*/ future /*x*/ = /*f*/ promise /*x*/./*b*/ get_future /*x*/();
  auto /*f*/ callable /*x*/ = [&promise]() {
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    /*f*/ promise /*x*/./*b*/ set_value /*x*/(4); // not a native return but similar
    // or better to have regular return here?
    return 4;
  };
  std::thread t(std::move(/*f*/ callable /*x*/)); // new thread -> promise set
  {
    auto status = /*f*/ future /*x*/./*b*/ wait_for /*x*/(std::chrono::milliseconds(1));
    assert(std::future_status::timeout == status);
  }
  {
    /*f*/ future /*x*/./*b*/ wait /*x*/();             // blocking
    auto value = /*f*/ future /*x*/./*b*/ get /*x*/(); // get 4
  }
  t.join();
```
> We can see that the callable had to be adopted (compared to having a regular `return` value). Is this desirable?
> - no, not desirable

A convenient approach to utilize "unmodified" callables with non-void return types with threads is `std::packaged_task`:
```pmans
  auto /*f*/ callable /*x*/ = []() {
    return 6;
  };
  auto /*f*/ task /*x*/ = /*b*/ std::packaged_task<int()> /*x*/i(std::move(/*f*/ callable /*x*/));
  auto /*f*/ future /*x*/ = /*f*/ task /*x*/.get_future(); // get future handle before moving in execution
  std::thread t(std::move(/*f*/ task /*x*/));
  {
    /*f*/ future /*x*/.wait();             // blocking
    auto value = /*f*/ future /*x*/.get(); // get 6
    std::cout << value << std::endl;
  }
  t.join();
```
We can see that a callable object can stay unmodified when executed by another thread.

```
clang++ -std=c++17 futures.cpp -O3 -pthread && ./a.out
clang++ -std=c++17 thread.cpp -O3 -pthread && ./a.out  # (last item)
```

## std::async
To even further simplify the triggering of an execution of a callable in a separate thread `std::async` can be used:
```pmans
  auto callable = [](int N, const std::string &str) {
    for (int i = 0; i < N; ++i)
      std::cout << str << std::endl;
      return 5;
  };
  int arg1 = 3;
  auto f1 = /*b*/ std::async /*x*/(callable, arg1, "default");
  auto f2 = /*b*/ std::async /*x*/(/*f*/ std::launch::deferred /*x*/,  callable, arg1, "deferred");
  auto f3 = /*b*/ std::async /*x*/(/*f*/ std::launch::async /*x*/, callable, arg1, "async");
  auto f4 = /*b*/ std::async /*x*/(/*f*/ std::launch::async /*x*/, callable, arg1, "async2");
  f4.wait();
  f3.wait();
  f1.wait();
  f2.wait();  
```
> What can we expect in terms of asynchronicity w.r.t to the different launch policies `async` and `deferred`?
<!--
> - deferred: might be lazy-evaluated
-->

Also `std::async` exhibits some properties which might be unexpected:

**Example #1**
```pmans
  { // (1a)
    auto future = /*b*/ std::async /*x*/(std::launch::async, callable, arg1, "async");
    func(std::move(future));
  } 
  { // (1b)
    auto future = /*b*/ std::async /*x*/(std::launch::async, callable, arg1, "async");
  }  
```
**Example #2**
```pmans
  { // (2)
    /*b*/ std::async /*x*/(callable, arg1, "is this ...");

    /*b*/ std::async /*x*/(callable, arg1, "... async?");
  }
```
> For the two examples above, will the two calls result in an overlapping execution of `callable` in two threads?
<!--
> - destructor of returned future is blocking until future is resolved/ready -> if not capturing -> blocking and wait in dtor of "rvalue-future"
> - `// ~future(){future.wait();}`
-->

```
clang++ -std=c++17 async.cpp -O3 -pthread && ./a.out
```

## Critical Sections (locking) 
For the probably most common synchronization task, i.e., protecting read or write access to a shared variable, the standard library provides `std::mutex` which is recommended to be used only in conjunction with a `std::unique_lock`  or `std::lock_guard`.
If a mutex would be used without a lock this can look like this:
```pmans
    /*b*/ std::mutex /*x*/ /*f*/ m /*x*/;
    std::vector<double> shared_data;
    auto manip = [&/*f*/ m /*x*/, &shared_data]() {
      /*f*/ m /*x*/./*b*/ lock /*x*/();
      // manipulate shared_data
      ...
      /*f*/ m /*x*/./*b*/ unlock /*x*/();
    };
    // this lambda could be running on different thread simult.
```
> Why is this usage error-prone?
<!--
> - manual unlock required (easy to forget in longer code)
-->

When using a `lock_guard` the example transforms to this:
```pmans
    std::mutex /*f*/ m /*x*/;
    std::vector<double> shared_data;
    auto manip = [&/*f*/ m /*x*/, &shared_data]() {
      /*b*/ std::lock_guard<std::mutex> /*x*/ /*f*/ lock /*x*/(/*f*/ m /*x*/);
      // manipulate shared_data
    };
```
In situations where is is required to acquire multiple mutexes before performing a manipulation, `unique_lock` can be utilized like this:
```pmans
    std::mutex /*f*/ m1 /*x*/;
    std::mutex /*f*/ m2 /*x*/;
    std::vector<double> shared_data1;
    std::vector<double> shared_data2;
    auto manip = [&/*f*/ m1 /*x*/, &/*f*/ m2 /*x*/, &shared_data1, &shared_data2]() {

      // proper "multi-lock"  
      std::unique_lock<std::mutex> /*b*/ dlock1 /*x*/(/*f*/ m1 /*x*/, std::defer_lock);
      std::unique_lock<std::mutex> /*b*/ dlock2 /*x*/(/*f*/ m2 /*x*/, std::defer_lock);
      std::lock(/*b*/ dlock1 /*x*/, /*b*/ dlock2 /*x*/); // locked in "atomic op, one sweep"
      // ... manipulate shared_data1 and shared_data2 together



    };

```
> Why is the snippet above preferable over a sequential locking using two `lock_guards`?
<!--
```pmans
func1(){
  ...
  // "not so good alternative"
  /*b*/ std::lock_guard<std::mutex> /*x*/ /*f*/ lock1 /*x*/(/*f*/ m1 /*x*/);    // t1 is here and lock1    
  /*b*/ std::lock_guard<std::mutex> /*x*/ /*f*/ lock2 /*x*/(/*f*/ m2 /*x*/);  
  // ... manipulate here      
  ...
}

func2(){
  ...
  /*b*/ std::lock_guard<std::mutex> /*x*/ /*f*/ lock2 /*x*/(/*f*/ m2 /*x*/);  // t2 is here and lock2
  /*b*/ std::lock_guard<std::mutex> /*x*/ /*f*/ lock1 /*x*/(/*f*/ m1 /*x*/); 
  // ... manipulate here    
  ...
}
```
-->

Locking is in no way a lightweight approach: only a single thread can execute the locked region and all other threads are blocked on entry.
Let's look at a performance comparison without even using more than one thread:
```pmans
    std::vector<int> vec(N, 1.0);
    int sum = 0;
    auto accumulate = [&sum, &vec]() {
      for (auto &&item : vec) {
        sum = sum + 1; // critical section: benchmark std::atomic vs lock_guard vs no synchronization
      }
    };
```

```
clang++ -std=c++17 serial_atomic_vs_lock.cpp -O3  && ./a.out 
```

## Atomic operations (std::atomic)
The standard library provides a wrapper for synchronizing access to entities which exhibit native support for atomic operations:
- integer types
- pointer types
```pmans
/*b*/ std::atomic /*x*/<int> /*f*/ a /*x*/(0);
/*f*/ a /*x*//*b*/ ++ /*x*/;            // (1a) perform atomic increment (specialization for int)
/*f*/ a /*x*/.fetch_add(1); // (1b) equivalent
/*f*/ a /*x*/ /*b*/ += /*x*/ 5;         // (2a) perform atomic addition (specialization for int)
/*f*/ a /*x*/.fetch_add(5); // (2b) equivalent
```

> Is the expression `a = a + 5;` below atomic as a whole?
```pmans
std::atomic<int> a(0);
a = a /*f*/ + /*x*/ 5;  // (3a)
...         // (3b) equivalent?
```

Atomics can be used for the variable which is shared directly, but they can also be used as index to non-atomic memory, see below for a simplistic example:
```pmans
int queue[N]:
std::atomic<size_t> front;
void push(int x) { 
    size_t unique_index = front.fetch_add(1);
    queue[unique_index] = x; // unique access here, no interference
}
```
Lock-free implementations of data structures follow this scheme, see link in the references.

Let's now move to an example where the synchronization is actually required because multiple threads are involved:
```pmans
struct Other {
  int a = 5;
  int b = 5;
}; // a+b is always 10;

struct Widget { 
  Other o;
  void /*b*/ mod1 /*x*/() {
      --o.a;
      ++o.b;
  }
  void /*b*/ mod2 /*x*/() {
      ++o.a;
      --o.b;
  }
  int /*b*/ inspect /*x*/() const { return o.a + o.b; }
};
```
> We will look at how a multi-threaded access of a `Other` through a `Widget` can be synchronized to guarantee the invariant of `Other`, namely `a+b==10`
```
clang++ -std=c++17 mutex_lock.cpp -O3 -pthread && ./a.out
```

## std::condition_variable
Another important synchronization primitive in the standard library is `std::condition_variable`: it allows to suspend the execution of threads and to notify a single or all of them if a condition becomes true. This can be used to avoid busy waiting of existing threads which have completed their tasks and shall be reused once new tasks are available.

> Why can it be attractive to reuse threads for subsequent tasks?
<!--
> - overhead when spawning a new one is >> than reusing
-->

The `std::condition_variable` is always used in combination with a lock, let's seen a minimal example to demonstrate it's usefulness:
```
clang++ -std=c++17 convar.cpp -O3 -pthread && ./a.out 
```




## Links
- std::thread https://en.cppreference.com/w/cpp/thread/thread
- std::promise https://en.cppreference.com/w/cpp/thread/promise
- std::future https://en.cppreference.com/w/cpp/thread/promise
- std::async https://en.cppreference.com/w/cpp/thread/async
- std::mutex https://en.cppreference.com/w/cpp/thread/mutex
- std::unique_lock https://en.cppreference.com/w/cpp/thread/unique_lock
- std::lock_guard https://en.cppreference.com/w/cpp/thread/lock_guard
- std::atomic https://en.cppreference.com/w/cpp/atomic/atomic
- Talk on `std::atomics` cppcon2017 https://www.youtube.com/watch?v=ZQFzMfHIxng 
