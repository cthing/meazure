/*
 * Copyright 2001, 2004, 2011 C Thing Software
 *
 * This file is part of Meazure.
 * 
 * Meazure is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * Meazure is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with Meazure.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "StdAfx.h"
#include "TimeStamp.h"
#include <time.h>
#include <string>
#pragma warning(disable: 4244 4127 4701)
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/local_time_adjustor.hpp>
#include <boost/date_time/c_local_time_adjustor.hpp>
#pragma warning(default: 4244 4127 4701)


CString MeaMakeTimeStamp(time_t t)
{
    CTime ts(t);

    return ts.FormatGmt(_T("%Y-%m-%dT%H:%M:%SZ"));
}



time_t MeaParseTimeStamp(const CString& timeStr)
{
    using namespace boost::posix_time;
    using namespace boost::gregorian;

    typedef boost::date_time::c_local_adjustor<ptime> local_adj;

    // The boost time/date routines work with STL strings.
    //
    std::string ts(timeStr);

    // The boost time/date routines don't want to see the
    // 'T' or 'Z' in delimited time strings so remove them.
    //
    int tpos = ts.find('T');
    int zpos = ts.find('Z');

    if (tpos != -1) {
        ts.replace(tpos, 1, 1, ' ');
    }
    if (zpos != -1) {
        ts.erase(zpos, 1);
    }

    // Parse the timestamp and convert from UTC to the local
    // time.
    //
    _tzset();
    ptime tlocal = local_adj::utc_to_local(time_from_string(ts));

    // Build a time structure.
    //
    struct tm tstruct;

    date tdate = tlocal.date();
    time_duration tdur = tlocal.time_of_day();

    tstruct.tm_hour  = tdur.hours();
    tstruct.tm_min   = tdur.minutes();
    tstruct.tm_sec   = tdur.seconds();

    tstruct.tm_year  = tdate.year() - 1900;
    tstruct.tm_mon   = tdate.month() - 1;
    tstruct.tm_mday  = tdate.day();
    tstruct.tm_isdst = -1;

    // Normalize the time struct, filling in the missing fields
    // and return the time in seconds since the Epoch.
    //
    return mktime(&tstruct);
}
