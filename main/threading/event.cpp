#include "event.hpp"

using namespace std;

namespace mesh::threading
{
  void event::set()
  {
    {
      lock_guard<mutex> lock(_mutex);
      _signal = true;
    }
    
    _condition.notify_all();
  }

  void event::reset()
  {
    lock_guard<mutex> lock(_mutex);
    _signal = false;
  }

  bool event::wait(std::chrono::steady_clock::duration timeout, bool reset)
  {
    unique_lock<mutex> lock(_mutex);
    bool result;
    if(timeout != std::chrono::steady_clock::duration{})
    {
      result = _condition.wait_for(lock, timeout, [&] { return _signal; });
    }
    else
    {
      _condition.wait(lock, [&] { return _signal; });
      result = true;
    }

    _signal = false;
    return result;
  }
}