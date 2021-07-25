#pragma once
#include <string>
#include <filesystem>

namespace mesh::storage
{
  class file_io
  {
  public:
    static std::string read_all_text(const char* path);

    static void write_all_text(const char* path, const std::string& text);
  };
}