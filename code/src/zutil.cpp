#include "zot.h"
#include "zutil.h"

#ifdef _WIN32
#include <time.h>
#include <windows.h> 
#include <sys/timeb.h>
#include <sys/types.h>

#if defined(_MSC_VER) || defined(_MSC_EXTENSIONS)
   #define DELTA_EPOCH_IN_MICROSECS 11644473600000000Ui64;
#else
   #define DELTA_EPOCH_IN_MICROSECS  11644473600000000ULL
#endif

struct timezone 
{
   int  tz_minuteswest; /* minutes W of Greenwich */
   int  tz_dsttime;     /* type of dst correction */
};

#if 0
int gettimeofday(struct timeval *tv, struct timezone *tz)
{
   FILETIME ft;
   unsigned __int64 tmpres = 0;
   static int tzflag = 0;

   if (NULL != tv)
   {
      GetSystemTimeAsFileTime(&ft);

      tmpres |= ft.dwHighDateTime;
      tmpres <<= 32;
      tmpres |= ft.dwLowDateTime;

      ULARGE_INTEGER test = { ft.dwLowDateTime, ft.dwHighDateTime };

      /*converting file time to unix epoch*/
      tmpres -= DELTA_EPOCH_IN_MICROSECS;
      tmpres /= 10;  /*convert into microseconds*/
      tv->tv_sec = (long)(tmpres / 1000000UL);
      tv->tv_usec = (long)(tmpres % 1000000UL);
   }

   if (NULL != tz)
   {
      if (!tzflag)
      {
         _tzset();
         tzflag++;
      }

      long tztmp = 0;
      _get_timezone(&tztmp);
      tz->tz_minuteswest = tztmp / 60;
      _get_daylight(&tz->tz_dsttime);
   }

   return 0;
}
#else
// found this at:
// http://hpcgpu.codeplex.com/wikipage?title=execution

int gettimeofday(struct timeval *tv, struct timezone *tz)
{
#if 0 // another potential solution to this problem
   SYSTEMTIME sysTime;
   GetLocalTime(&sysTime);
#else
   struct _timeb timebuffer;
   _ftime_s(&timebuffer);

   tv->tv_sec = (long)timebuffer.time;
   tv->tv_usec = 1000 * timebuffer.millitm;
#endif
   return 0;
}
#endif
#endif

char *tolower(char *buf)
{
   for (char *p = buf; *p != '\0'; p++)
      *p = (char)tolower(*p);
   return buf;
}

char *toupper(char *buf)
{
   for (char *p = buf; *p != '\0'; p++)
      *p = (char)toupper(*p);
   return buf;
}
