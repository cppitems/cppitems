template <class T> class shared_ptr {
  struct ControlBlock {
    unsigned count = 1;
  };
  ControlBlock *cb;
  T *ptr;

  void increment() {
    if (cb)
      ++cb->count;
  }
  void decrement() {
    if (cb && --cb->count == 0) {
      delete ptr;
      delete cb;
    }
  }

public:
  shared_ptr(T *ptr) : cb(new ControlBlock()), ptr(ptr) {}
  shared_ptr(const shared_ptr &other) : cb(other.cb), ptr(other.ptr) {
    //
  }
  shared_ptr(shared_ptr &&other) : cb(other.cb), ptr(other.ptr) {
    //
  }
  ~shared_ptr() { 
    // 
  }
  shared_ptr &operator=(const shared_ptr &other) {
    //
  }

  T *operator->() const { return ptr; }
  T &operator*() const { return *ptr; }
};
template <typename T, typename... ARGS>
shared_ptr<T> make_shared(ARGS &&... args) {
  return shared_ptr<T>(new T(std::forward<ARGS>(args)...));
}
