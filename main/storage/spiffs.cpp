#include "spiffs.hpp"

#include "infrastructure/error.hpp"
#include "esp_spiffs.h"

using namespace mesh::infrastructure;

namespace mesh::storage
{
  spiffs::spiffs(const char* root, size_t max_open_files)
  {
    _logger.log_message(log_severity::info, "Initializing SPIFFS...");
    esp_vfs_spiffs_conf_t conf = {
      .base_path = root,
      .partition_label = "storage",
      .max_files = max_open_files,
      .format_if_mount_failed = true
    };

    check_result(esp_vfs_spiffs_register(&conf));
    _partition_id = conf.partition_label;

    size_t total = 0, used = 0;
    check_result(esp_spiffs_info(conf.partition_label, &total, &used));

    _logger.log_message(log_severity::info, "SPIFFS initialized. Disk space: %d bytes total, %d bytes used.", total, used);
  }

  spiffs::~spiffs()
  {
    check_result(esp_vfs_spiffs_unregister(_partition_id));
  }
}