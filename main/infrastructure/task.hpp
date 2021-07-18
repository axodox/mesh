#pragma once
#include <memory>
#include <functional>
#include "freertos/task.h"
#include <sdkconfig.h>

namespace mesh::infrastructure
{
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
    task(std::function<void()>&& action, uint32_t priority = CONFIG_PTHREAD_TASK_PRIO_DEFAULT, const char* name = CONFIG_PTHREAD_TASK_NAME_DEFAULT)
    {
      _action = std::make_unique<std::function<void()>>(std::move(action));
      //xTaskCreate(&task::worker, name, CONFIG_PTHREAD_TASK_STACK_SIZE_DEFAULT, _action.get(), priority, &_task_handle);
      xTaskCreatePinnedToCore(&task::worker, name, CONFIG_PTHREAD_TASK_STACK_SIZE_DEFAULT, _action.get(), priority, &_task_handle, 0);
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