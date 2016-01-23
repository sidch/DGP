//============================================================================
//
// DGP: Digital Geometry Processing toolkit
// Copyright (C) 2016, Siddhartha Chaudhuri
//
// This software is covered by a BSD license. Portions derived from other
// works are covered by their respective licenses. For full licensing
// information see the LICENSE.txt file.
//
//============================================================================

#include "Log.hpp"
#include "Common.hpp"
#include <ctime>
#include <iomanip>
#include <sstream>

namespace DGP {
namespace LogInternal {

Spinlock lock;

std::string
stripPathFromFilename(std::string const & fullPath)
{
  std::size_t lastSlash;
  lastSlash = fullPath.find_last_of("/\\");
  return (lastSlash >= fullPath.length()) ? fullPath : fullPath.substr(lastSlash + 1);
}

std::string
currentDateTimeToString()
{
  std::time_t rawTime;
  std::tm const * timeInfo;

  std::time(&rawTime);  // get raw system time
  timeInfo = std::localtime(&rawTime);  // break it down into components w.r.t. the current locale
  std::ostringstream os;
  os << 1900 + timeInfo->tm_year << '-'
     << std::setw(2) << std::right << std::setfill('0') << 1 + timeInfo->tm_mon << '-'
     << std::setw(2) << std::right << std::setfill('0') << timeInfo->tm_mday << ' '
     << std::setw(2) << std::right << std::setfill('0') << timeInfo->tm_hour << ':'
     << std::setw(2) << std::right << std::setfill('0') << timeInfo->tm_min  << ':'
     << std::setw(2) << std::right << std::setfill('0') << timeInfo->tm_sec;
  return os.str();
}

} // namespace LogInternal
} // namespace DGP
