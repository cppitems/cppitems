#include <memory>
#include <vector>
template <typename T,
          template <typename ...> class CONTAINER = std::vector>
struct Widget {
//   std::vector<T, ...> ctnr;
  CONTAINER<T> ctnr;
};

// specialization for T=double
template <template <typename, typename> class CONTAINER>
struct Widget<double, CONTAINER> {
  CONTAINER<double, std::allocator<double>> ctnr;
};

int main() { Widget<double, std::vector> w{}; }
