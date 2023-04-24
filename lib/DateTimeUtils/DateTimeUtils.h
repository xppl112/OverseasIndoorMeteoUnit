#include <stdint.h>
#include <Time.h>

class DateTimeUtils
{
public:
  static String formatFromTimestamp(unsigned long timestamp, uint8_t hoursOffset, bool showDate = true, bool showTime = true);
  static int getHourFromTimestamp(unsigned long timestamp, uint8_t hoursOffset);

private:
  static tm* getLocalTime(unsigned long timestamp, uint8_t hoursOffset);
};