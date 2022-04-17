#include <stdio.h>
#include "file_io.hpp"

using namespace std;

namespace mesh::storage
{
  std::string file_io::read_all_text(const char* path)
  {
    auto file = fopen(path, "r");
    if(!file) return {};

    fseek(file, 0, SEEK_END);
    auto length = ftell(file);

    string buffer(length + 1, '\0');

    fseek(file, 0, SEEK_SET);
    fread(buffer.data(), 1, length, file);
    fclose(file);

    return buffer;
  }

  void file_io::write_all_text(const char* path, const std::string& text)
  {
    auto file = fopen(path, "w");
    if(!file) return;

    fwrite(text.data(), 1, text.length(), file);
    fclose(file);
  }
}