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
/// @brief Mix-in class for the Singleton pattern.

#pragma once

#include "MeaAssert.h"


/// Defines a Singleton object. This macro must be called from the cpp file.
///
/// @param singleton_t  Type of the singleton object.
///
#define MEA_SINGLETON_DEF(singleton_t)                              \
    singleton_t* MeaSingleton_T<singleton_t>::m_instance { nullptr }; \
    bool MeaSingleton_T<singleton_t>::m_destroyed { false };

/// Declares a Singleton object. This macro must be called from
/// the class declaration in the header file.
///
/// @param singleton_t  Type of the singleton object.
///
#define MEA_SINGLETON_DECL(singleton_t)             \
    private:                                        \
        friend class MeaSingleton_T<singleton_t>;   \
        singleton_t();                              \
        virtual ~singleton_t();


/// Mix-in class implementing the Singleton pattern. A Singleton
/// object is never created or destroyed explicitly. Instead, the
/// object is created when the Instance() method is called the first
/// time. The object is destroy when DestroyInstance() is called.
///
template <typename singleton_t>
class MeaSingleton_T {

public:
    /// Destructor for the Singleton. This method is called
    /// indirectly by the DestroyInstance() method. The delete
    /// operator is never directly applied to a Singleton object
    /// by the user of this class.
    ///
    virtual ~MeaSingleton_T() {}

    /// The first time this method is called, a instance of the
    /// Singleton object is created and returned. Subsequent calls
    /// to this method return the same object instance. The method
    /// asserts if DestroyInstance() has been called.
    ///
    /// @return Singleton instance. The same instance is always
    ///         returned.
    ///
    static singleton_t& Instance() {
        MeaAssert(!m_destroyed);
        if (m_instance == nullptr) {
            m_instance = new singleton_t();
        }
        return *m_instance;
    }

    /// When the Singleton object is no longer needed, it must
    /// be destroyed using this method. The Instance() method must
    /// not be called after this method has been called or an
    /// assertion will be raised.
    ///
    static void DestroyInstance() {
        delete m_instance;
        m_instance = nullptr;
        m_destroyed = true;
    }

    /// Indicates whether the Singleton object has been destroyed.
    /// Once a Singleton instance has been destroyed, it is illegal
    /// to call the Instance() method.
    ///
    /// @return <b>true</b> if the Singleton object has been destroyed.
    ///
    static bool IsDestroyed() { return m_destroyed; }

protected:
    /// Constructs a Singleton object instance. A Singleton is never
    /// created directly. Instead, the object is created the first
    /// time the Instance() method is called. Subsequent calls to
    /// Instance() return the same object instance.
    ///
    MeaSingleton_T() {}

private:
    /// Singletons have no copy semantics. Therefore, this method is
    /// purposely left undefined to trigger a compile/link error if an
    /// attempt is ever made to use it.
    ///
    MeaSingleton_T(const MeaSingleton_T<singleton_t>& src);

    /// Singletons have no assignment semantics. Therefore, this method
    /// is purposely left undefined to trigger a compile/link error if
    /// an attempt is ever made to use it.
    ///
    MeaSingleton_T<singleton_t>& operator=(const MeaSingleton_T<singleton_t>& src);


    static singleton_t* m_instance;     ///< Instance of the Singleton object.
    static bool m_destroyed;            ///< Has the Singleton object been destroyed.
};
