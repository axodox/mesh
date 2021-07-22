#pragma once
#include <string>
#include <filesystem>

namespace mesh::storage
{
  class file_io
  {
  public:
    static std::string read_all_text(const std::filesystem::path& path);

    static void write_all_text(const std::filesystem::path& path, const std::string& text);
  };
}