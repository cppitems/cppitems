template <typename T> struct DebugType { using T::notexisting;};
// usage: DebugType<decltype(...)> error;

int main() {
  struct A {
    int a;
    void foo() {}
  };
  const A obj{5};
  const int &&a = 5;
  const double b = a + 5.0;
//   DebugType<decltype(a)> type_a;
//   DebugType<decltype(a + 5.0)> type_tmp;
//   DebugType<decltype(&A::foo)> type_A_foo_ptr;  
//   DebugType<decltype(A().a)> type_A_a;  
//   DebugType<decltype(A().foo())> type_A_foo_ret;
}