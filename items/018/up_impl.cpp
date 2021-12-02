template <typename T> class unique_ptr {
  T *ptr = nullptr;

public:
  unique_ptr(T *p) : ptr(p) {}
  ~unique_ptr() { delete ptr; }
  T *get() { return ptr; }
  T &operator*() { return *get(); }
  T *operator->() { return get(); }
  ...
};
template <typename T, typename... ARGS>
unique_ptr<T> make_unique(ARGS &&... args) {
  return unique_ptr<T>(new T(std::forward<ARGS>(args)...));
}