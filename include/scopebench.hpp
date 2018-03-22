#pragma once


#include <chrono>
#include <functional>
#include <type_traits>
#include <utility>


namespace scopebench {


template <
  bool WantBench = true, // Static flag for whether or not to turn benching on
  typename Clock = std::chrono::system_clock,
  typename Rep = double
  >
class interval {


  using time_point = std::chrono::time_point<Clock>;
  using duration = std::chrono::duration<Rep>;
 

  time_point _beginning; // Beginning of the interval this measures
  std::function<void(duration)> _callback; // Callback to invoke when done measuring


public:

  
  template <typename Callback>
  interval(Callback&& callback) :
    _beginning{Clock::now()},
    _callback{std::forward<Callback>(callback)}
  {
    static_assert(
      std::is_constructible<decltype(_callback), Callback>::value,
      "Callback type mismatch; not storable"
    );


    // XXX: Empty
  }


  ~interval() {
    auto end = Clock::now();
    auto measured_duration = end - _beginning;
    _callback(measured_duration);
  }


};


// Specialization for when benching is turned off
template <
  typename Clock,
  typename Rep
  >
class interval<false, Clock, Rep> {
public:
  

  // No-op ctor
  template <typename Unused>
  interval(Unused&&) {}


};


} // namespace scopebench
