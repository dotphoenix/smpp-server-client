/*
 * SMPP Encoder/Decoder
 * Copyright (C) 2006 redtaza@users.sourceforge.net
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */
#ifndef __SMPP_COMMAND_LENGTH_HPP__
#define __SMPP_COMMAND_LENGTH_HPP__

/// @file command_length.hpp

#include <cstring>
#include "aux_types.hpp"

namespace Smpp {

    class CommandLength {
        Smpp::Uint32 val_;
        CommandLength();
    public:
        
        CommandLength(const Smpp::Uint32& p) : val_(p) {}
        CommandLength& operator+=(int i) { val_ += i; return *this; }
            
        operator Smpp::Uint32() const { return val_; }

        static Smpp::Uint32 decode(const Smpp::Char* buff) {
            Smpp::Uint32 length;
            memcpy(&length, &buff[0], sizeof length);
            return Smpp::ntoh32(length);
        }
    };

} // namespace Smpp

#endif

