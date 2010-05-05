#pragma once

#ifdef _WIN32

// windows implementation of gettimeofday
// found on http://www.suacommunity.com/dictionary/gettimeofday-entry.php
// and http://social.msdn.microsoft.com/Forums/en-US/vcgeneral/thread/430449b3-f6dd-4e18-84de-eebd26a8d668

struct timeval;
struct timezone;

int gettimeofday(struct timeval *tv, struct timezone *tz);

#endif
