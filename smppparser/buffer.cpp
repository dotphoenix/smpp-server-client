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
/// @file buffer.cpp

#include "buffer.hpp"

/// @brief Creates a non allocated buffer.
Smpp::Buffer::Buffer() : data_(0), offset_(0)
{
}

/// @brief Creates a buffer of \c len length.
/// @param len Size of the buffer to create.
Smpp::Buffer::Buffer(Uint32 len) : data_(new Smpp::Uint8[len]), offset_(0)
{
}

/// @brief Destructor - destroys the buffer.
Smpp::Buffer::~Buffer() throw()
{
    if(data_)
        delete [] data_;
}

/// @brief Copies a submit_multi destination address to the buffer.
/// p submit_multi destination address base class.
void
Smpp::CopySubmitMultiAddresses::operator()(const MultiDestinationAddressBase* p)
{
    switch(p->Type()) {
        case 0x01:
          {
            b_ += static_cast<Uint8>(0x01);
            const SmeMultiAddress* addr =
                                static_cast<const SmeMultiAddress*>(p);
            b_ += addr->Value();
            break;
          }
        case 0x02:
          {
            b_ += static_cast<Uint8>(0x02);
            const DistributionListAddress* addr =
                        static_cast<const DistributionListAddress*>(p);
            b_ += addr->Value();
            break;
          }
        default:
            throw Smpp::Error("Invalid destination address");
            break;
    }
}

