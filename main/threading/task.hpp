#pragma once
#include <memory>
#include <functional>
#include <sdkconfig.h>

#include "freertos/freertos.h"
#include "freertos/task.h"
#include "infrastructure/logger.hpp"
#include "threading/event.hpp"

namespace mesh::threading
{
  enum class task_affinity : uint8_t
  {
    none,
    core_0,
    core_1
  };

  enum class task_priority : uint32_t
  {
    minimum = 0,
    normal = CONFIG_PTHREAD_TASK_PRIO_DEFAULT,
    maximum = configMAX_PRIORITIES
  };

  class task
  {
    static constexpr infrastructure::logger _logger{"task"};

  public:
    task(std::function<void()>&& action, task_affinity affinity = task_affinity::none, task_priority priority = task_priority::normal, const char* name = "mesh_task");
    ~task();

    static task* current();
    bool is_running() const;
    void close();

  private:
    static thread_local task* _current_task;
    std::function<void()> _action;
    TaskHandle_t _task_handle;
    const char* _name;
    event _finished;

    static void worker(void* data);
  };
}