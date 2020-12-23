#include "list.hpp"

#include <algorithm>
#include <numeric>
#include <chrono>
#include <iostream>

template <class Container>
void fillFibonaci(Container &cont,
                  typename Container::value_type numberOfElements) {
  typename Container::value_type lastElement = 1, newElement = 1;
  cont.push_back(lastElement);
  cont.push_back(newElement);
  for (unsigned i = 0; i < numberOfElements; ++i) {
    auto dummy = newElement;
    newElement = lastElement + newElement;
    cont.push_back(newElement);
    lastElement = dummy;
  }
}

template <class ForwardIt> void fillRandom(ForwardIt first, ForwardIt last, unsigned max) {
  while(first != last) {
    *first = rand() % max;
    ++first;
  }
}

template <class Container> void rangeLoopOutput(Container &cont) {
  std::cout << "Range-based output: " << std::endl;
  for (auto &it : cont) {
    std::cout << it << std::endl;
  }
}

template <class Container> void explicitLoopOutput(Container &cont) {
  std::cout << "Explicit Loop output: " << std::endl;
  for (auto it = cont.begin(); it != cont.end(); ++it) {
    std::cout << *it << std::endl;
  }
}

template <class Container>
void singleElementOutput(const Container &cont, unsigned index) {
  auto it = cont.begin();
  for (unsigned i = 0; i < index; ++i) {
    ++it;
  }
  std::cout << "Element " << index << ": " << *it << std::endl;
}

template <class Container> void reverseOutput(Container &cont) {
  std::cout << "Reverse output:" << std::endl;
  auto it = --cont.cend(); // end() must be decrementable!
  for (; it != cont.cbegin(); --it) {
    std::cout << *it << std::endl;
  }
  std::cout << *it << std::endl;
}

template <class Container> void findFirstInRandom(Container &cont) {
  Container randomCont(10000, 0);
  fillRandom(randomCont.begin(), randomCont.end(), 1000);

  // find the first fibonacci number in our sequence of random data
  auto it = std::find_first_of(randomCont.begin(), randomCont.end(),
                               cont.begin(), cont.end());
  if (it == randomCont.end()) {
    std::cout << "No fibonacci numbers found in random sequence!" << std::endl;
  } else {
    std::cout << "Element " << std::distance(randomCont.begin(), it)
              << " is a Fibonacci number: " << *it << std::endl;
  }
}

int main() {
  srand((unsigned)time(NULL)); // seed random number generator

  // check if iterator functionality is implemented
  {
    list<double> fibList;

    fillFibonaci(fibList, 10);

    rangeLoopOutput(fibList);

    explicitLoopOutput(fibList);

    reverseOutput(fibList);

    singleElementOutput(fibList, 5);

    findFirstInRandom(fibList);
  }

  // check if iterator really only allows const access
  {
    const list<int> myList(10, 42);
    
    static_assert(std::is_const<decltype(myList.begin())::value_type>::value);
    static_assert(std::is_const<decltype(myList.end())::value_type>::value);
    static_assert(std::is_const<std::remove_reference<decltype(*(myList.begin()))>::type>::value);
  }

  // test requirement for empty list
  list<int> emptyList(12, 3);
  emptyList.clear();
  if (emptyList.begin() != emptyList.end()) {
    std::cout
        << "ERROR: list::begin and list::end are not equal for empty list!"
        << std::endl;
  }

  return 0;
}