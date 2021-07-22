#include "file_io.hpp"
#include <stdio.h>

using namespace std;

namespace mesh::storage
{
  std::string file_io::read_all_text(const std::filesystem::path& path)
  {
    auto file = fopen(path.c_str(), "r");
    if(!file) return {};

    fseek(file, 0, SEEK_END);
    auto length = ftell(file);

    string buffer('\0', length);

    fseek(file, 0, SEEK_SET);
    fread(buffer.data(), 1, length, file);
    fclose(file);
    return buffer;
  }

  void file_io::write_all_text(const std::filesystem::path& path, const std::string& text)
  {
    auto file = fopen(path.c_str(), "w");
    if(!file) return;

    fwrite(text.data(), 1, text.length(), file);
    fclose(file);
  }
}