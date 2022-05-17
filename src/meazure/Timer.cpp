/*
 * Copyright 2001 C Thing Software
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

#include "pch.h"
#include "Timer.h"


MeaTimer::MeaTimer() :
    m_timerThread(nullptr),
    m_stopEvent(FALSE, TRUE),
    m_exitEvent(FALSE, TRUE),
    m_elapse(0),
    m_parent(nullptr),
    m_userData(0) {
    // The m_exitEvent indicates whether we are in the timer procedure. Since
    // we are just starting, we are not in the timer thread.
    //
    m_exitEvent.SetEvent();
}

MeaTimer::~MeaTimer() {
    try {
        // Don't leave the timer thread running or we will pull the object
        // instance out from under it.
        //
        Stop();

        m_timerThread = nullptr;
        m_parent = nullptr;
    } catch (...) {
        MeaAssert(false);
    }
}

void MeaTimer::Start(int elapse) {
    // The timer needs a parent object to send the timer expire event.
    //
    MeaAssert(m_parent != nullptr);

    // Only one thread at a time can set or read the timer thread ID.
    //
    m_critSect.Lock();

    m_elapse = elapse;

    // Only one timer thread is allowed to run at one time. In other
    // words, multiple calls to Start will not cause multiple timer
    // threads to pile up.
    //
    if (m_timerThread == nullptr) {
        m_timerThread = AfxBeginThread(MeaTimer::TimerProc, this);
    }

    m_critSect.Unlock();
}

void MeaTimer::Stop() {
    // Stop the timer thread.
    //
    m_stopEvent.PulseEvent();

    // Wait for the signal that the timer thread is done.
    //
    ::WaitForSingleObject(m_exitEvent, INFINITE);
}

UINT MeaTimer::TimerProc() {
    // Indicate that the timer thread has begun.
    //
    m_exitEvent.ResetEvent();

    //
    // Wait for the requested amount of time interruptible by stop event.
    //
    DWORD ret = ::WaitForSingleObject(m_stopEvent, m_elapse);

    // Allow another timer thread to start. This must happen
    // before posting the timer message in case the timer message
    // handler calls the timer Start method.
    //
    m_critSect.Lock();
    m_timerThread = nullptr;
    m_critSect.Unlock();

    // Tell the parent that the timer has expired if the wait
    // exited due to the elapsed time expiring rather than the
    // stop event being issued.
    //
    if (ret == WAIT_TIMEOUT) {        // Normal path
        m_parent->PostMessage(MeaHPTimerMsg, m_userData);
    }

    // Indicate that the timer thread is done.
    //
    m_exitEvent.SetEvent();

    return 0;
}
