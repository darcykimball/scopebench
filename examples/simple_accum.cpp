#include <algorithm>
#include <chrono>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <fstream>
#include <random>

#include <scopebench.hpp>


#ifndef BENCH
#define BENCH true
#endif // BENCH


using namespace std::literals;


// XXX: If you want to keep a global count, I'm not sure there's any way other
// than using a global variable (or a set of them).
std::map<
  std::string,
  std::chrono::duration<double>
  > total_times; 


// XXX: Pre-curried for conveniencre
auto accum_time = [](auto&& key) {
  return [&](auto&& duration) {
    if (!total_times.insert({key, duration}).second) {
      total_times[key] += duration;
    }
  };
};


double no_fib(std::vector<double> const& v, size_t s = 1) {
  auto resv = v;

  size_t i = 0;
  for (auto& e : resv) {
    e *= v[(i + s) % v.size()];
    ++i;
  }


  return std::accumulate(resv.cbegin(), resv.cend(), 0.0);
}

double not_fib_either(std::vector<double> const& v) {
  auto resv = v;

  scopebench::interval<BENCH> iv{accum_time("not_fib_either"s)};
  for (auto& e : resv) {
    e *= e;
  }


  return std::accumulate(resv.cbegin(), resv.cend(), 0.0);
}


double still_not_fib(std::vector<double> const& v) {
  double result;


  for (size_t s = 0; s < v.size(); ++s) {
    {
      scopebench::interval<BENCH> iv{accum_time("still_not_fib"s)};
              
      result += no_fib(v, s); 
    }
  }


  return result;
}


int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " n\n";
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


  // Do work and bench
  std::cout << std::boolalpha << "macro BENCH = " << BENCH << '\n';

  auto snf_result = still_not_fib(v);
  auto nfe_result = not_fib_either(v);
    
  std::cout << "snf_result = " << snf_result << '\n';
  std::cout << "nfe_result = " << nfe_result << '\n';


  std::cout << "accumulated times:\n";
  for (auto const& p : total_times) {
    std::cout << p.first << " took " << p.second.count() << "s\n";
  }


  return EXIT_SUCCESS;
}
