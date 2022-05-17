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


/// Mix-in class implementing the Singleton pattern. A Singleton
/// object is never created or destroyed explicitly. Instead, the
/// object is created when the Instance() method is called the first
/// time. The object is destroy when DestroyInstance() is called.
///
template <typename singleton_t>
class MeaSingleton_T {

public:
    /// The first time this method is called, a instance of the
    /// Singleton object is created and returned. Subsequent calls
    /// to this method return the same object instance. The method
    /// asserts if DestroyInstance() has been called.
    ///
    /// @return Singleton instance. The same instance is always
    ///         returned.
    ///
    static singleton_t& Instance() {
        static singleton_t instance { token{} };
        return instance;
    }

    /// Singletons have no copy semantics.
    ///
    MeaSingleton_T(const MeaSingleton_T<singleton_t>& src) = delete;

    /// Singletons have no assignment semantics.
    ///
    MeaSingleton_T<singleton_t>& operator=(const MeaSingleton_T<singleton_t>& src) = delete;

protected:
    /// Allows the derived class constructor to be public but restricts creation to this class.
    /// Avoids having to declare this class as a friend in the derived class.
    ///
    struct token {};

    /// Constructs a Singleton object instance. A Singleton is never
    /// created directly. Instead, the object is created the first
    /// time the Instance() method is called. Subsequent calls to
    /// Instance() return the same object instance.
    ///
    MeaSingleton_T() = default;
};
