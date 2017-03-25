/**
 *
 * Copyright (c) 2014 Pascal Gauthier.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
 *
 */

#ifndef DEXED_H_INCLUDED
#define DEXED_H_INCLUDED

void dexed_trace(const char *source, const char *fmt, ...);

#define DEXED_ID "0.9.4"

#ifdef DEBUG
    #define DEXED_VERSION DEXED_ID " DEBUG"
    #ifdef _MSC_VER
        #define TRACE(fmt, ...) dexed_trace(__FUNCTION__,fmt,##__VA_ARGS__)
    #else
        #define TRACE(fmt, ...) dexed_trace(__PRETTY_FUNCTION__,fmt,##__VA_ARGS__)
    #endif
#else
    #define DEXED_VERSION DEXED_ID
    #define TRACE(fmt, ...)
#endif

#endif  // DEXED_H_INCLUDED
