#pragma once
#include <functional>
#include <unordered_map>
#include <list>
#include <mutex>
#include <memory>
#include "esp_event.h"
#include "hasher.hpp"
#include "event_handler.hpp"
#include "logger.hpp"

namespace mesh::infrastructure
{
  typedef event_handler<void*> event_loop_handler_t;
  
  struct event_key
  {
    esp_event_base_t source;
    int32_t event;
  };

  struct event_loop_handler_collection
  {
    std::mutex mutex;
    esp_event_loop_handle_t loop;
    std::unordered_map<event_key, std::list<event_loop_handler_t>, trivial_hasher<event_key>, trivial_comparer<event_key>> items;
  };

  class event_loop_subscription
  {
    static constexpr logger _logger{"event_loop_subscription"};

  private:
    std::weak_ptr<event_loop_handler_collection> _handlers;
    event_key _key;
    std::list<event_loop_handler_t>::iterator _handler;

  public:
    event_loop_subscription() = default;
    
    event_loop_subscription(const std::shared_ptr<event_loop_handler_collection>& handlers, event_key key, const std::list<event_loop_handler_t>::iterator& handler);

    event_loop_subscription(event_loop_subscription&& other);
    event_loop_subscription(const event_loop_subscription&) = delete;

    const event_loop_subscription& operator=(event_loop_subscription&& other);

    ~event_loop_subscription();
  };
  
  class event_loop
  {
    friend class event_loop_subscription;
    static constexpr logger _logger{"event_loop"};

  public:
    event_loop(const char* name = nullptr);
    ~event_loop();

    event_loop_subscription subscribe(esp_event_base_t source, int32_t event, const event_loop_handler_t& handler);

  private:
    std::shared_ptr<event_loop_handler_collection> _handlers;

    static void on_event(void* that, esp_event_base_t source, int32_t event, void* arg);
  };
}