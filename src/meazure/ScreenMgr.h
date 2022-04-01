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
/// @brief Header file for the display screen manager. 

#pragma once

#include "Singleton.h"
#include <map>
#include "Profile.h"
#include "Utils.h"


/// Provides information on the display monitor(s). One of the primary
/// jobs of this manager is to properly handle multiple display monitor
/// environments.
///
class MeaScreenMgr : public MeaSingleton_T<MeaScreenMgr>
{
private:
    /// Represents a single display screen.
    ///
    class Screen
    {
    public:
        /// Constructs a display screen object.
        ///
        /// @param mgr      [in] Parent manager.
        /// @param rect     [in] Rectangle representing the screen, in pixels.
        ///                 In multiple monitor environments, the screen rectangle
        ///                 can have negative coordinates.
        ///
        Screen(MeaScreenMgr& mgr, LPCRECT rect);
        
        /// Destroys a display screen object.
        ///
        virtual ~Screen();


        /// Returns the screen rectangle. Note that in multiple
        /// monitor environments, the screen rectangle can have
        /// negative coordinates.
        ///
        /// @return Screen rectangle, in pixels.
        ///
        const CRect&    GetRect() const { return m_rect; }
        
        /// Determines the center of the screen.
        ///
        /// @return Center point of the screen.
        ///
        const CPoint&   GetCenter() const { return m_center; }


        /// Sets the resolution of the screen, in pixels per inch.
        ///
        /// @param useManualRes     [in] Specifies whether the manually calibrated
        ///                         resolution should be used (true) or the operating
        ///                         system resolution (false).
        /// @param manualRes        [in] If non-NULL, specifies the manually calibrated
        ///                         resolution.
        ///
        void            SetScreenRes(bool useManualRes, const FSIZE *manualRes = NULL);

        /// Obtains the currently set manual screen resolution, if set.
        ///
        /// @param useManualRes     [out] Indicates whether the manually calibrated
        ///                         resolution is used (true) or the operating
        ///                         system resolution (false).
        /// @param manualRes        [out] Manually calibrated resolution, in pixels
        ///                         per inch.
        ///
        void            GetScreenRes(bool& useManualRes, FSIZE& manualRes) const {
            useManualRes = m_useManualRes;
            manualRes = m_manualRes;
        }

        /// Obtains the current screen resolution. If the resolution has been
        /// manually calibrated, it is returned. Otherwise the operating system
        /// reported resolution is returned.
        ///
        /// @return Screen resolution, in pixels per inch.
        ///
        const FSIZE&    GetScreenRes() const { return m_currentRes; }
        
        /// Indicates if the screen resolution has been manually set.
        ///
        /// @return <b>true</b> if the screen resolution has been set manually.
        ///
        bool            IsManualRes() const { return m_useManualRes; }


        /// In multiple monitor environments, one of the monitors is
        /// designated as the primary. This method designates the screen
        /// as the primary.
        ///
        /// @param primary      [in] Specify <b>true</b> if this screen
        ///                     is the primary.
        ///
        void            SetPrimary(bool primary) { m_primary = primary; }

        /// Indicates if this is the primary screen.
        ///
        /// @return <b>true</b> if this is the primary screen.
        ///
        bool            IsPrimary() const { return m_primary; }


        /// Specifies whether the resolution calibration has been performed
        /// in inches or centimeters.
        ///
        /// @param calInInches  [in] <b>true</b> if the calibration has been
        ///                     performed in inches.
        ///
        void            SetCalInInches(bool calInInches) { m_calInInches = calInInches; }
        
        /// Indicates whether the resolution calibration has been performed
        /// in inches or centimeters.
        ///
        /// @return <b>true</b> if the calibration has been performed in inches.
        ///
        bool            GetCalInInches() const { return m_calInInches; }


        /// Sets a descriptive name for the screen.
        ///
        /// @param name     [in] Name for the screen.
        ///
        void            SetName(LPCTSTR name) { m_name = name; }
        
        /// Returns the descriptive name for the screen.
        ///
        /// @return Name for the screen or the empty string if
        ///         there is no name.
        ///
        CString         GetName() const { return m_name; }

    private:
        MeaScreenMgr&   m_mgr;      ///< Parent manager.
        CRect           m_rect;     ///< Screen rectangle.
        CPoint          m_center;   ///< Center point of the screen.

        FSIZE   m_currentRes;       ///< Current screen resolution, pixels per inch.
        FSIZE   m_manualRes;        ///< Manually calibrated resolution, pixels per inch.
        bool    m_useManualRes;     ///< Indicates if manually calibrated resolution is used.
        bool    m_calInInches;      ///< Indicates if calibration in inches or centimeters.
        bool    m_primary;          ///< Indicates if this is the primary screen.
        CString m_name;             ///< Descriptive name for the screen.
    };


    typedef std::map<HMONITOR, Screen*> Screens;    ///< Maps a monitor handle to a screen object.

public:
    typedef Screens::const_iterator ScreenIter;     ///< Constant iterator over the screens.

    /// Used by the STL collections for ordering comparisons.
    ///
    struct less {
        /// Compares two screen iterators.
        /// @param lhs  [in] Left hand side of the comparison.
        /// @param rhs  [in] Right hand side of the comparison.
        /// @return <b>true</b> if lhs < rhs.
        bool operator()(const ScreenIter& lhs, const ScreenIter& rhs) const
        {
            return (*lhs).first < (*rhs).first;
        }
    };

    
    static const bool kDefUseManualRes;     ///< Use manual resolution by default.
    static const bool kDefCalInInches;      ///< Calibrate in inches by default.


    /// Persists the state of the manager to the specified profile object.
    ///
    /// @param profile  [in] Profile object into which the manager state
    ///                 is persisted.
    ///
    void            SaveProfile(MeaProfile& profile) const;
    
    /// Restore the state of the manager from the specified profile object.
    ///
    /// @param profile  [in] Profile object from which the manager state
    ///                 is restored.
    ///
    void            LoadProfile(MeaProfile& profile);


    /// Resets the screen manager to its default state.
    ///
    void            MasterReset() const;


    /// Returns the number of display screens attached to the system.
    ///
    /// @return Number of display screens attached to the system.
    ///
    int             GetNumScreens() const { return static_cast<int>(m_screens.size()); }


    /// Obtains an iterator over each screen attached to the system.
    /// @return Constant iterator over all screens.
    ScreenIter      GetScreenIter() const { return m_screens.begin(); }

    /// Obtains an iterator pointed at the screen that contains the
    /// specified window.
    ///
    /// @param wnd      [in] Window whose screen is desired.
    ///
    /// @return Constant iterator pointed at screen containing the
    ///         specified window.
    ///
    ScreenIter      GetScreenIter(const CWnd* wnd) const;

    /// Obtains an iterator pointed at the screen that contains the
    /// specified point.
    ///
    /// @param point    [in] Point whose screen is desired.
    ///
    /// @return Constant iterator pointed at screen containing the
    ///         specified point.
    ///
    ScreenIter      GetScreenIter(const POINT& point) const;

    /// Obtains an iterator pointed at the screen that contains the
    /// specified rectangle.
    ///
    /// @param rect     [in] Rectangle whose screen is desired.
    ///
    /// @return Constant iterator pointed at screen containing the
    ///         specified rectangle.
    ///
    ScreenIter      GetScreenIter(const RECT& rect) const;

    /// Indicates if the screen iterator is at the end of the
    /// screen collection.
    ///
    /// @param iter     [in] Screen iterator to test.
    ///
    /// @return <b>true</b> if the specified iterator is pointed to
    ///         the end of the collection of screens.
    ///
    bool            AtEnd(const ScreenIter& iter) const { return iter == m_screens.end(); }


    /// Determines the center of the screen that contains the
    /// app's main window.
    ///
    /// @return Center point of the screen containing the app's
    ///         main window.
    ///
    const CPoint&   GetCenter() const;

    /// Returns a point guaranteed to be located off any screen.
    ///
    /// @return Point located off any display screen.
    ///
    CPoint          GetOffScreen() const;
    
    /// Returns the virtual screen rectangle. The virtual rectangle
    /// is the rectangle containing all display screen attached to
    /// the system.
    ///
    /// @return Rectangle encompassing all display screens attached
    ///         to the system, in pixels.
    ///
    const CRect&    GetVirtualRect() const { return m_virtualRect; }

    /// Returns the screen rectangle for the screen pointed to by
    /// the specified iterator.
    ///
    /// @param iter     [in] Screen iterator pointed at the screen
    ///                 whose rectangle is desired.
    ///
    /// @return Screen rectangle, in pixels.
    ///
    const CRect&    GetScreenRect(const ScreenIter& iter) const { return (*iter).second->GetRect(); }

    /// Returns the manually set screen resolution for the screen
    /// pointed to by the specified iterator.
    ///
    /// @param iter         [in] Screen iterator pointed at the screen
    ///                     whose manual resolution is desired.
    /// @param useManualRes [out] <b>true</b> if the resolution was set
    ///                     manually.
    /// @param manualRes    [out] Manually set resolution, in pixels per inch.
    ///
    void            GetScreenRes(const ScreenIter& iter, bool& useManualRes, FSIZE& manualRes) const {
                        (*iter).second->GetScreenRes(useManualRes, manualRes);
    }

    /// Returns the resolution for the screen pointed to by the specified iterator.
    ///
    /// @param iter     [in] Screen iterator pointed at the screen whose
    ///                 resolution is desired.
    ///
    /// @return Screen resolution, in pixels per inch. If the resolution has been
    ///         set manually, it is returned. Otherwise, the operating system
    ///         reported resolution is returned.
    ///
    const FSIZE&    GetScreenRes(const ScreenIter& iter) const { return (*iter).second->GetScreenRes(); }

    /// Indicates it the resolution for the specified screen has been set manually.
    ///
    /// @param iter     [in] Screen iterator pointed at the screen whose
    ///                 resolution setting is being queried.
    ///
    /// @return <b>true</b> if the screen's resolution was set manually.
    ///
    bool            IsManualRes(const ScreenIter& iter) const { return (*iter).second->IsManualRes(); }
    
    /// Indicates whether any screen is using the operating system reported
    /// resolution. In other words, are there any screen that require calibration.
    ///
    /// @return <b>true</b> if there are any screens requiring manual calibration.
    ///
    bool            AnyOSRes() const {
                        for (ScreenIter iter = m_screens.begin(); iter != m_screens.end(); ++iter) {
                            if (!(*iter).second->IsManualRes()) {
                                return true;
                            }
                        }
                        return false;
    }

    /// Sets the resolution for the screen pointed to by the specified iterator.
    ///
    /// @param iter             [in] Iterator pointing to the screen whose resolution is
    ///                         to be set.
    /// @param useManualRes     [in] <b>true</b> if the resolution is manually calibrated.
    /// @param manualRes        [in] Manually calibrated screen resolution, in pixels.
    ///
    void            SetScreenRes(const ScreenIter& iter, bool useManualRes, const FSIZE *manualRes = NULL) const {
                        (*iter).second->SetScreenRes(useManualRes, manualRes);
    }

    /// Indicates if the screen pointed to by the specified iterator is the primary.
    ///
    /// @return <b>true</b> if the specified screen is the primary.
    ///
    bool            IsPrimary(const ScreenIter& iter) const { return (*iter).second->IsPrimary(); }
    
    /// Returns the name for the specified screen.
    ///
    /// @param iter     [in] Iterator pointing to the screen whose name is desired.
    ///
    /// @return Name of the specified screen.
    ///
    CString         GetScreenName(const ScreenIter& iter) const { return (*iter).second->GetName(); }

    /// Indicates if the specified screen was calibrated in inches or centimeters.
    ///
    /// @param iter     [in] Points at the screen whose calibration units are desired.
    ///
    /// @return <b>true</b> if the resolution was calibrated in inches.
    ///
    bool            GetCalInInches(const ScreenIter& iter) const { return (*iter).second->GetCalInInches(); }
    
    /// Specifies the calibration units for the specified screen.
    ///
    /// @param iter         [in] Points at the screen whose calibration units are to be set.
    /// @param calInInches  [in] <b>true</b> if the resolution is calibrated in inches.
    ///
    void            SetCalInInches(const ScreenIter& iter, bool calInInches) const {
                        (*iter).second->SetCalInInches(calInInches);
    }

    /// Indicates whether the virtual screen rectangle has changed
    /// since the last time the application was run. If the virtual screen
    /// size has changed, re-calibration is recommended.
    ///
    /// @return <b>true</b> if the virtual screen rectangle has changed.
    ///
    bool            SizeChanged() const { return m_sizeChanged; }


    /// Ensures that the specified window rectangle is visible on
    /// the nearest screen. If the window rectangle is already visible,
    /// the rectangle is not changed.
    ///
    /// @param windowRect   [in] Window rectangle that is to be tested
    ///                     for visibility. The rectangle is altered if
    ///                     necessary to ensure at least a part of the
    ///                     window is visible.
    ///
    /// @return New rectangle that is guaranteed to be visible.
    ///
    CRect           EnsureVisible(const RECT& windowRect) const;

    /// Ensures that the specified point is on a screen.
    ///
    /// @param pt       [in] Point to check.
    ///
    /// @return New point based on the specified point that is guaranteed
    ///         to be on a screen.
    ///
    CPoint          LimitPosition(const CPoint& pt) const;

    /// Obtains the screen resolution reported by the operating system.
    ///
    /// @return Resolution in pixels per inch.
    ///
    FSIZE           GetOSScreenRes() const;

private:
    MEA_SINGLETON_DECL(MeaScreenMgr);       ///< Managers are singletons.

    /// Purposely undefined.
    MeaScreenMgr(const MeaScreenMgr&);

    /// Purposely undefined.
    MeaScreenMgr& operator=(const MeaScreenMgr&);


    /// Called by the constructor via EnumDisplayMonitors.
    /// Instantiates a Screen object for each display monitor
    /// on the system.
    ///
    /// @param hMonitor     [in] Monitor handle.
    /// @param hdcMonitor   [in] Not used.
    /// @param monitorRect  [in] Rectangle representing the
    ///                     monitors logical dimensions.
    /// @param userData     [in] this pointer.
    ///
    /// @return TRUE to continue enumeration of monitors.
    ///
    static BOOL CALLBACK CreateScreens(HMONITOR hMonitor, HDC hdcMonitor,
                                       LPRECT monitorRect, LPARAM userData);

    /// Obtains the screen containing the specified point.
    ///
    /// @param point        [in] Point whose screen is desired.
    ///
    /// @return Screen containing the specified point or NULL if
    ///         the point is not contained by any screen.
    ///
    Screen*     GetScreen(const POINT& point) const;

    Screens     m_screens;          ///< Display monitors.
    CRect       m_virtualRect;      ///< Bounding box of all screen rectangles.
    bool        m_sizeChanged;      ///< Virtual screen rectangle changed since last run.
};

