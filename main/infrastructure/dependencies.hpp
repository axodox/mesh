#pragma once
#include <memory>
#include <functional>
#include <mutex>
#include <type_traits>
#include <unordered_map>
#include <typeindex>
#include <exception>

namespace mesh::infrastructure
{
  enum class dependency_lifetime
  {
    singleton,
    transient
  };

  class dependency_container
  {
  private:
    struct dependency_registration_base 
    {
      virtual ~dependency_registration_base() = default;
    };

    template<typename T>
    class dependency_registration : public dependency_registration_base
    {
    private:
      dependency_lifetime _lifetime;
      std::shared_ptr<T> _value;
      std::function<std::shared_ptr<T>()> _factory;
      std::mutex _mutex;

    public:
      dependency_registration(dependency_lifetime lifetime) :
        _lifetime(lifetime)
      { 
        if constexpr (std::is_default_constructible<T>::value)
        {
          _factory = [] { return std::make_shared<T>(); };
        }
        else
        {
          _factory = []() -> std::shared_ptr<T> { throw std::logic_error("Cannot construct abstract type."); };
        }
      }

      dependency_registration(dependency_lifetime lifetime, std::function<std::shared_ptr<T>()>&& factory) :
        _lifetime(lifetime),
        _factory{ std::move(factory) }
      { }

      std::shared_ptr<T> get()
      {
        switch (_lifetime)
        {
        case dependency_lifetime::singleton:
        {
          std::lock_guard<std::mutex> lock(_mutex);

          if (!_value)
          {
            _value = _factory();
          }

          return _value;
        }
        case dependency_lifetime::transient:
        {
          return _factory();
        }
        default:
          throw std::logic_error("Unknown dependency lifetime.");
        }
      }
    };

    std::unordered_map<std::type_index, std::unique_ptr<dependency_registration_base>> _registrations;

    std::mutex _mutex;

  public:
    template<typename T>
    std::shared_ptr<T> resolve()
    {
      std::type_index index{ typeid(T) };

      dependency_registration<T>* registration;
      {
        std::lock_guard<std::mutex> lock(_mutex);

        auto& item = _registrations[index];
        if (!item)
        {
          item = std::make_unique<dependency_registration<T>>(dependency_lifetime::singleton);
        }

        registration = static_cast<dependency_registration<T>*>(item.get());
      }

      return registration->get();
    }

    template<typename T>
    bool add(dependency_lifetime lifetime)
    {
      std::type_index index{ typeid(T) };

      std::lock_guard<std::mutex> lock(_mutex);
      return _registrations.emplace(index, std::make_unique<dependency_registration<T>>(lifetime)).second;
    }

    template<typename T>
    bool add(dependency_lifetime lifetime, std::function<std::shared_ptr<T>()>&& factory)
    {
      std::type_index index{ typeid(T) };

      std::lock_guard<std::mutex> lock(_mutex);
      return _registrations.emplace(index, std::make_unique<dependency_registration<T>>(lifetime, std::move(factory))).second;
    }

    template<typename T, typename U>
    bool add()
    {
      return add<T>(dependency_lifetime::singleton, std::function<std::shared_ptr<T>()>([this] { return resolve<U>(); }));
    }

    template<typename T, typename... TArgs>
    bool add(TArgs&&... arguments)
    {
      std::type_index index{ typeid(T) };

      std::lock_guard<std::mutex> lock(_mutex);
      return _registrations.emplace(index, std::make_unique<dependency_registration<T>>(dependency_lifetime::singleton, [...args = std::forward<TArgs>(arguments)] { return std::make_shared<T>(args...); })).second;
    }
  };

  extern dependency_container dependencies;
}

