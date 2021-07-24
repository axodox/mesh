#pragma once
#include <memory>
#include <functional>
#include "freertos/task.h"
#include <sdkconfig.h>

namespace mesh::infrastructure
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
  private:
    std::unique_ptr<std::function<void()>> _action;
    TaskHandle_t _task_handle;

    static void worker(void* data)
    {
      auto action = static_cast<std::function<void()>*>(data);
      (*action)();
    }

  public:
    task(std::function<void()>&& action, task_affinity affinity = task_affinity::none, task_priority priority = task_priority::normal, const char* name = "mesh_task")
    {
      _action = std::make_unique<std::function<void()>>(std::move(action));
      if(affinity == task_affinity::none)
      {
        xTaskCreate(&task::worker, name, CONFIG_PTHREAD_TASK_STACK_SIZE_DEFAULT, _action.get(), uint32_t(priority), &_task_handle);
      }
      else
      {
        xTaskCreatePinnedToCore(&task::worker, name, CONFIG_PTHREAD_TASK_STACK_SIZE_DEFAULT, _action.get(), uint32_t(priority), &_task_handle, int(affinity) - 1);
      }
    }

    ~task()
    {
      close();
    }

    void close()
    {
      if(!_action) return;

      vTaskDelete(_task_handle);
      _action.reset();
    }
  };
}