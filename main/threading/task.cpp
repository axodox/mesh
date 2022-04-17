#include "task.hpp"

using namespace mesh::infrastructure;

namespace mesh::threading
{
  thread_local task* task::_current_task = nullptr;

  task::task(std::function<void()> &&action, task_affinity affinity, task_priority priority, const char *name) :
    _action(std::move(action)),
    _task_handle(nullptr),
    _name(name)
  {
    _logger.log_message(log_severity::info, "Starting task %s...", _name);
    if (affinity == task_affinity::none)
    {
      xTaskCreate(&task::worker, _name, CONFIG_PTHREAD_TASK_STACK_SIZE_DEFAULT, this, uint32_t(priority), &_task_handle);
    }
    else
    {
      xTaskCreatePinnedToCore(&task::worker, _name, CONFIG_PTHREAD_TASK_STACK_SIZE_DEFAULT, this, uint32_t(priority), &_task_handle, int(affinity) - 1);
    }
  }

  task::~task()
  {
    close();
  }

  task* task::current()
  {
    return _current_task;
  }

  bool task::is_running() const
  {
    return _task_handle != nullptr;
  }

  void task::close()
  {
    if (!_task_handle) return;

    auto task_handle = _task_handle;
    _task_handle = nullptr;
    _action = nullptr;
    _logger.log_message(log_severity::info, "Waiting for task %s to finish...", _name);
    _finished.wait();    
    vTaskDelete(task_handle);
    _logger.log_message(log_severity::info, "Task %s closed.", _name);
  }

  void task::worker(void *data)
  {
    auto that = static_cast<task*>(data);

    _current_task = that;
    _logger.log_message(log_severity::info, "Task %s started.", that->_name);

    that->_action();

    _logger.log_message(log_severity::info, "Task %s finished.", that->_name);
    _current_task = nullptr;

    that->_finished.set();
  }
}