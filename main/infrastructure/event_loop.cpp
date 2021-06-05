#include "event_loop.hpp"

#include <exception>
#include <thread>
#include <chrono>
#include "error.hpp"

using namespace std;
using namespace std::chrono;
using namespace std::this_thread;

namespace mesh::infrastructure
{
  event_loop_subscription::event_loop_subscription(const std::shared_ptr<event_loop_handler_collection> &handlers, event_key key, const std::list<event_loop_handler_t>::iterator &handler) : 
    _handlers(handlers),
    _key(key),
    _handler(handler)
  {
    _logger.log_message(log_severity::debug, "Event handler added.");
  }

  event_loop_subscription::event_loop_subscription(event_loop_subscription &&other)
  {
    *this = move(other);
  }

  const event_loop_subscription &event_loop_subscription::operator=(event_loop_subscription &&other)
  {
    swap(_handlers, other._handlers);
    swap(_key, other._key);
    swap(_handler, other._handler);
    return *this;
  }

  event_loop_subscription::~event_loop_subscription()
  {
    auto handler_collection = _handlers.lock();
    if (!handler_collection)
      return;

    lock_guard<mutex> lock(handler_collection->mutex);
    auto handlers = handler_collection->items[_key];
    handlers.erase(_handler);

    if (handlers.empty())
    {
      if(handler_collection->loop)
      {
        check_result(esp_event_handler_unregister_with(handler_collection->loop, _key.source, _key.event, &event_loop::on_event));
      }
      else
      {
        check_result(esp_event_handler_unregister(_key.source, _key.event, &event_loop::on_event));
      }
      _logger.log_message(log_severity::debug, "Event unsubscribed.");
    }

    _logger.log_message(log_severity::debug, "Event handler removed.");
  }

  event_loop::event_loop(const char *name)
  {
    _handlers = make_shared<event_loop_handler_collection>();

    auto is_default = !name;
    if (is_default)
    {
      check_result(esp_event_loop_create_default());
      _handlers->loop = nullptr;
    }
    else
    {
      esp_event_loop_args_t args = {
          .queue_size = CONFIG_ESP_SYSTEM_EVENT_QUEUE_SIZE,
          .task_name = name,
          .task_priority = ESP_TASKD_EVENT_PRIO,
          .task_stack_size = ESP_TASKD_EVENT_STACK,
          .task_core_id = 0};

      check_result(esp_event_loop_create(&args, &_handlers->loop));
    }
  }

  event_loop_subscription event_loop::subscribe(esp_event_base_t source, int32_t event, const event_loop_handler_t &handler)
  {
    lock_guard<mutex> lock(_handlers->mutex);

    event_key key{source, event};
    auto &handlers = _handlers->items[key];
    if (handlers.empty())
    {
      if(_handlers->loop)
      {
        check_result(esp_event_handler_register_with(_handlers->loop, source, event, &event_loop::on_event, this));
      }
      else
      {
        check_result(esp_event_handler_register(source, event, &event_loop::on_event, this));
      }
      _logger.log_message(log_severity::debug, "Event subscribed.");
    }

    auto it = handlers.insert(handlers.end(), handler);
    return event_loop_subscription(_handlers, key, it);
  }

  event_loop::~event_loop()
  {
    _handlers->items.clear();

    lock_guard<mutex> lock(_handlers->mutex);
    if(_handlers->loop)
    {
      esp_event_loop_delete(&_handlers->loop);
    }
  }

  void event_loop::on_event(void *that, esp_event_base_t source, int32_t event, void *arg)
  {
    _logger.log_message(log_severity::debug, "Event received.");
    auto loop = static_cast<event_loop *>(that);
    lock_guard<mutex> lock(loop->_handlers->mutex);

    event_key key{source, event};
    auto it = loop->_handlers->items.find(key);
    for (auto &handler : it->second)
    {
      try
      {
        handler(arg);
      }
      catch(const exception& e)
      {
        _logger.log_message(log_severity::fatal, e.what());
        _logger.log_message(log_severity::info, "Restarting in 5 seconds...");
        sleep_for(seconds(5));
        esp_restart();
      }
    }
  }
}