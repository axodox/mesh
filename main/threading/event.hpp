#pragma once
#include <chrono>
#include <mutex>
#include <condition_variable>

namespace mesh::threading
{
  class event
  {
  public:
    void set();
    void reset();

    bool wait(std::chrono::steady_clock::duration timeout = {}, bool reset = true);

 private:
    std::mutex _mutex;
    std::condition_variable _condition;
    bool _signal = false;
  };
}