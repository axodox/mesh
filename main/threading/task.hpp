#pragma once
#include <memory>
#include <functional>
#include "freertos/freertos.h"
#include "freertos/task.h"
#include <sdkconfig.h>

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
  public:
    task(std::function<void()>&& action, task_affinity affinity = task_affinity::none, task_priority priority = task_priority::normal, const char* name = "mesh_task");
    ~task();

    void close();

  private:
    std::unique_ptr<std::function<void()>> _action;
    TaskHandle_t _task_handle;

    static void worker(void* data);
  };
}