#pragma once
#include <functional>

namespace mesh::infrastructure
{
  template <typename... args_t>
  class event_handler : public std::function<void(args_t...)>
  {
  private:
    template <typename listener_t, typename handler_t>
    static auto bind_listener(listener_t&& listener, handler_t&& handler)
    {
      return[handler = std::forward<handler_t>(handler), listener = std::forward<listener_t>(listener)](auto&&... args){ return std::mem_fn(handler)(*listener, std::forward<decltype(args)>(args)...); };
    }

  public:
    template<class listener_t, class handler_t>
    event_handler(listener_t&& listener, handler_t&& handler) :
      std::function<void(args_t...)>(bind_listener(std::forward<decltype(listener)>(listener), std::forward<decltype(handler)>(handler)))
    { }

    using std::function<void(args_t...)>::function;
  };
}