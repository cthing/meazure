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

/// @file
/// @brief Header file for a high priority timer.

#pragma once


#include <afxmt.h>
#include <meazure/Messages.h>
#include <cassert>


/// Implements a timer that issues timing messages at a higher priority
/// than the standard windows timer that issues WM_TIMER messages.
///
class MeaTimer {

public:
    /// Instantiates a timer. To use the timer, the Create method
    /// must be called.
    ///
    MeaTimer();

    /// Destroys a timer.
    ///
    virtual ~MeaTimer();

    /// Registers the specified parent window to receive the timer
    /// messages. The timer will send the parent a MeaHPTimerMsg
    /// message.
    ///
    /// @param parent   [in] Parent window to receive the MeaHPTimerMsg message.
    /// @param userData [in] Caller defined data.
    ///
    void Create(CWnd* parent, WPARAM userData = 0) {
        assert(parent != nullptr);
        m_parent = parent;
        m_userData = userData;
    }

    /// Sets the timer to the specified interval and starts it running.
    ///
    /// @param elapse   [in] Time interval in milliseconds.
    ///
    void Start(int elapse);

    /// Cancels the current timer if it is set.
    ///
    void Stop();

private:
    /// Called when the timer expires. Simply calls the instance
    /// method of the same name.
    ///
    /// @param pParam   [i] Timer instance pointer.
    ///
    /// @return Zero indicating success.
    ///
    static UINT TimerProc(LPVOID pParam) {
        return static_cast<MeaTimer*>(pParam)->TimerProc();
    }

    /// Called when the timer expires. Sends the MeaHPTimerMsg message
    /// to the parent window.
    ///
    /// @return Zero indicating success.
    ///
    UINT TimerProc();


    CWinThread* m_timerThread;      ///< Thread in which the timer runs.
    CCriticalSection m_critSect;    ///< Critical section guard.
    CEvent m_stopEvent;             ///< Used internally to indicate the timer has been stopped.
    CEvent m_exitEvent;             ///< Used internally to indicate the timer has completed all processing.
    int m_elapse;                   ///< Timer period, in milliseconds.
    CWnd* m_parent;                 ///< Window to receive the timer expire message.
    WPARAM m_userData;              ///< Caller defined data.
};
