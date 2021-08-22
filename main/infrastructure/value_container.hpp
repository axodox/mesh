#pragma once
#include <typeinfo>
#include <typeindex>
#include <memory>
#include <type_traits>

namespace mesh::infrastructure
{
class value_container_storage_base
  {
  protected:
    virtual void* data() = 0;

  public:
    virtual std::type_index type() const = 0;

    template<typename value_t>
    value_t* get()
    {
      if(std::type_index(typeid(value_t)) == type())
      {
        return static_cast<value_t*>(data());
      }
      else
      {
        return nullptr;
      }
    }

    template<typename value_t, std::enable_if_t<std::is_move_assignable_v<value_t>, bool> = true>
    bool set(value_t&& value)
    {
      if(std::type_index(typeid(value_t)) == type())
      {
        *static_cast<value_t*>(data()) = std::move(value);
        return true;
      }
      else
      {
        return false;
      }
    }

    template<typename value_t, std::enable_if_t<std::negation_v<std::is_move_assignable<value_t>>, bool> = true>
    bool set(const value_t& value)
    {
      if(std::type_index(typeid(value_t)) == type())
      {
        *static_cast<value_t*>(value()) = value;
        return true;
      }
      else
      {
        return false;
      }
    }
  };

  template<typename value_t>
  class value_container_storage : public value_container_storage_base
  {
    virtual std::type_index type() const override
    {
      return typeid(value_t);
    }

    virtual void* data() override
    {
      return &_value;
    }
    
    value_t _value;
  };

  class value_container
  {
  public:
    template<typename value_t, std::enable_if_t<std::negation_v<std::is_move_assignable<value_t>>, bool> = true>
    void set(const value_t& value)
    {
      if(!_storage || !_storage->set(value))
      {
        _storage = std::make_unique<value_container_storage<value_t>>();
        _storage->set(value);
      }
    }

    template<typename value_t, std::enable_if_t<std::is_move_assignable_v<value_t>, bool> = true>
    void set(value_t&& value)
    {
      if(!_storage || !_storage->set(std::move(value)))
      {
        _storage = std::make_unique<value_container_storage<value_t>>();
        _storage->set(std::move(value));
      }
    }

    template<typename value_t>
    value_t* get()
    {
      if(_storage)
      {
        return _storage->get<value_t>();
      }
      else
      {
        return nullptr;
      }
    }

    template<typename value_t, std::enable_if_t<std::is_move_assignable_v<value_t>, bool> = true>
    static value_container create(value_t&& value)
    {
      value_container container;
      container.set(std::move(value));
      return container;
    }

    template<typename value_t, std::enable_if_t<std::negation_v<std::is_move_assignable<value_t>>, bool> = true>
    static value_container create(const value_t& value)
    {
      value_container container;
      container.set(value);
      return container;
    }

    operator bool() const
    {
      return _storage != nullptr;
    }

  private:
    std::unique_ptr<value_container_storage_base> _storage;
  };
}