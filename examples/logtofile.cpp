#include <algorithm>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <fstream>
#include <random>

#include <scopebench.hpp>


#ifndef BENCH
#define BENCH true
#endif // BENCH


double no_fib(std::vector<double> const& v, size_t s = 1) {
  auto resv = v;

  size_t i = 0;
  for (auto& e : resv) {
    e *= v[(i + s) % v.size()];
    ++i;
  }


  return std::accumulate(resv.cbegin(), resv.cend(), 0.0);
}


int main(int argc, char** argv) {
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0] << " n log_filename\n";
    return EXIT_FAILURE;
  }


  size_t n;
  if (!(std::istringstream{argv[1]} >> n)) {
    std::cerr << "Arg must be non-negative integer\n";
    return EXIT_FAILURE;
  }


  // Make some random input
  std::vector<double> v(n, 0.0);

  std::random_device rd;
  std::mt19937 gen{rd()};
  std::generate(v.begin(), v.end(),
    [&gen]() { return std::generate_canonical<double, 32>(gen); });


  // Setup logging action
  auto const& log_filename = argv[2];
  std::ofstream log_file{log_filename};

  if (!log_file) {
    std::cerr << "Couldn't open log file for output\n";
    return EXIT_FAILURE;
  }

  auto log = [&log_file](auto duration) {
    log_file << "Operation took " << duration.count() << '\n';
  };


  // Do work and bench
  double result{0.0};
  
  
  std::cout << std::boolalpha << "macro BENCH = " << BENCH << '\n';

  {
    scopebench::interval<BENCH> iv{log};
    result = no_fib(v);
  }
    
  std::cout << "result = " << result << '\n';


  return EXIT_SUCCESS;
}
