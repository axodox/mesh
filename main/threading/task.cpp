#include "task.hpp"

namespace mesh::threading
{
  task::task(std::function<void()> &&action, task_affinity affinity, task_priority priority, const char *name)
  {
    _action = std::make_unique<std::function<void()>>(std::move(action));
    if (affinity == task_affinity::none)
    {
      xTaskCreate(&task::worker, name, CONFIG_PTHREAD_TASK_STACK_SIZE_DEFAULT, _action.get(), uint32_t(priority), &_task_handle);
    }
    else
    {
      xTaskCreatePinnedToCore(&task::worker, name, CONFIG_PTHREAD_TASK_STACK_SIZE_DEFAULT, _action.get(), uint32_t(priority), &_task_handle, int(affinity) - 1);
    }
  }

  task::~task()
  {
    close();
  }

  void task::close()
  {
    if (!_action)
      return;

    vTaskDelete(_task_handle);
    _action.reset();
  }

  void task::worker(void *data)
  {
    auto action = static_cast<std::function<void()> *>(data);
    (*action)();
  }
}