#pragma once
#include <cstdint>
#include <cstring>
#include <span>

namespace mesh::storage
{
  class stream
  {
  public:
    virtual void write(std::span<const uint8_t> buffer) = 0;
    virtual void read(std::span<uint8_t> buffer) = 0;

    virtual size_t position() const;
    virtual void seek(size_t position);
    virtual size_t length() const;

    virtual ~stream() = default;

    template<typename T>
    void write(const T& value);

    template<typename T>
    void read(T& value);

    template<typename T>
    T read();

    virtual void flush();
  };

  template<typename T, typename TEnable = void>
  struct serializer { };

  template<typename T>
  void stream::write(const T& value)
  {
    serializer<T>::serialize(*this, value);
  }

  template<typename T>
  void stream::read(T& value)
  {
    serializer<T>::deserialize(*this, value);
  }

  template<typename T>
  T stream::read()
  {
    T value;
    serializer<T>::deserialize(*this, value);
    return value;
  }
}