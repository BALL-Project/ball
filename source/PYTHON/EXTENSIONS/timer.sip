// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: timer.sip,v 1.1 2004/07/10 15:15:01 amoll Exp $
//

class Timer
{
%TypeHeaderCode
	#include <BALL/SYSTEM/timer.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End
  public:
//  void dump(::std::ostream& s = ::std::cout, Size depth = 0L) const;
//  Timer& operator = (const Timer& timer);
  Timer();
  Timer(Timer&);
  bool isRunning() const;
  bool isValid() const;
  bool operator != (const Timer&) const;
  bool operator < (const Timer&) const;
  bool operator <= (const Timer&) const;
  bool operator == (const Timer&) const;
  bool operator > (const Timer&) const;
  bool operator >= (const Timer&) const;
  bool start();
  bool stop();
  double getCPUTime() const;
  double getClockTime() const;
  double getSystemTime() const;
  double getUserTime() const;
  void clear();
  void reset();
  ~Timer();
};
