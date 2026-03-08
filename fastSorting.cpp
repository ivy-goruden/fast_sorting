#include <chrono>
#include <iostream>
#include <iterator>
#include <list>
#include <random>
#include <sys/resource.h>

std::ostream &operator<<(std::ostream &os, const std::list<int> &list) {
  os << "[";
  for (auto el : list) {
    os << el;
    os << ", ";
  }
  os << "]";
  return os;
}

std::list<int> fast_sorting(std::list<int> &list) {
  if (list.size() < 2) {
    return list;
  }
  int base = *list.begin();
  std::list<int> smaller;
  std::list<int> bigger;
  auto indx = std::next(list.begin());
  while (indx != list.end()) {
    auto next_it = std::next(indx);
    if (*indx < base) {
      smaller.splice(smaller.end(), list, indx);
    } else {
      bigger.splice(bigger.end(), list, indx);
    }
    indx = next_it;
  }
  bigger = fast_sorting(bigger);
  smaller = fast_sorting(smaller);
  smaller.splice(smaller.end(), list, list.begin());
  smaller.splice(smaller.end(), bigger);
  return smaller;
}

int main() {
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::mt19937 gen(seed);
  std::uniform_int_distribution<int> dist(1, 100);

  const int NUMBER = 20;

  std::list<int> check;
  for (int i = 0; i < NUMBER; i++) {
    int random_num = dist(gen);
    check.insert(check.end(), random_num);
  }
  std::cout << "ORIGINAL: " << check << "\n";
  auto start = std::chrono::high_resolution_clock::now();
  check = fast_sorting(check);
  auto end = std::chrono::high_resolution_clock::now();
  std::cout << "RESULT: " << check << "\n";
  std::chrono::duration<double, std::milli> duration_ms = end - start;
  std::cout << "Execution time: " << duration_ms.count() << " milliseconds\n";
  struct rusage usage;
  if (getrusage(RUSAGE_SELF, &usage) == 0) {
    std::cout << "Peak Memory: " << usage.ru_maxrss / 1024 << " KB"
              << std::endl;
  }

  return 0;
}