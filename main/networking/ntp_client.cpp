#include "ntp_client.hpp"
#include "infrastructure/error.hpp"

using namespace mesh::infrastructure;

namespace mesh::networking
{
  ntp_client::ntp_client(const char* server)
  {
    esp_sntp_setoperatingmode(SNTP_OPMODE_POLL);
    esp_sntp_setservername(0, server);
    esp_sntp_set_sync_mode(SNTP_SYNC_MODE_SMOOTH);
    esp_sntp_set_time_sync_notification_cb(&ntp_client::on_time_synchronized);
    esp_sntp_init();
  }

  bool ntp_client::is_in_sync() const
  {
    return sntp_get_sync_status() == SNTP_SYNC_STATUS_COMPLETED;
  }

  void ntp_client::set_timezone(const char* timezone)
  {
    _timezone = timezone;
  }

  void ntp_client::on_time_synchronized(struct timeval* /*time*/)
  {
    //Set time zone
    setenv("TZ", _timezone, 1);
    tzset();

    time_t utc_now;
    time(&utc_now);

    tm local_now;
    localtime_r(&utc_now, &local_now);

    char time_as_text[20];
    strftime(time_as_text, sizeof(time_as_text), "%F %T", &local_now);

    _logger.log_message(log_severity::info, "Time synchronized from network. The current time is: %s.", time_as_text);
  }
}