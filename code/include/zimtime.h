#pragma once

#include <iostream>

namespace Zot
{

class ZimTime
{

public:

   ZimTime() : m_time(0) {}
   ZimTime(uint32 time) : m_time(time) { }
   ZimTime(const ZimTime &other) : m_time(other.m_time) {}
   ZimTime(bool bUpdate) : m_time(0) { if (bUpdate) update(); }
   ZimTime &update();
   uint32 get() const { return m_time; }
   void set(uint32 time) { m_time = time; }
   bool operator<(const ZimTime &rhs) const { return m_time < rhs.m_time; }
   bool operator<=(const ZimTime &rhs) const { return m_time <= rhs.m_time; }
   bool operator>(const ZimTime &rhs) const { return m_time > rhs.m_time; }
   bool operator>=(const ZimTime &rhs) const { return m_time >= rhs.m_time; }

   ZimTime operator+(const ZimTime &rhs) const { return ZimTime(m_time + rhs.m_time); }
   ZimTime operator-(const ZimTime &rhs) const { return ZimTime(m_time - rhs.m_time); }
   ZimTime operator+(int mstime) const { return ZimTime(m_time + mstime); }

   std::ostream &operator<<(std::ostream &os) const;

protected:

   uint32 m_time;

};

}
