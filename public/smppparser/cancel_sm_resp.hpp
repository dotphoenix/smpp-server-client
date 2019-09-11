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
#ifndef __SMPP_CANCEL_SM_RESP_HPP__
#define __SMPP_CANCEL_SM_RESP_HPP__

/// @file cancel_sm_resp.hpp
/// @brief An SMPP cancel_sm_resp PDU.

#include "aux_types.hpp"
#include "header.hpp"

namespace Smpp {

    /// @class CancelSmResp
    /// @brief Definition of an SMPP cancel_sm_resp, derived from a Response.
    class CancelSmResp : public Response {
        Smpp::Buffer buff_;

      public:
        /// cancel_sm_resp minimum length in octets.
        const static int MinLength = 16;
        
        /// @brief Default constructor.
        CancelSmResp();
        
        /// @brief Constructor requiring all mandatory parameters.
        CancelSmResp(const CommandStatus& commandStatus,
                    const SequenceNumber& sequenceNumber);
       
        /// @brief Construct from a buffer.
        CancelSmResp(const Smpp::Uint8* b);

        /// @brief Destructor - does nothing.
        ~CancelSmResp();
        
        /// @brief Serialize the PDU.
        /// @note The length is taken from the command_length().
        /// @return The PDU as an octet array, suitable for outputting.
        const Smpp::Uint8* encode();

        /// @brief Populates the PDU from an array of octets.
        /// @param buff containing PDU as octet array.
        void decode(const Smpp::Uint8* buff);
    };

} // namespace Smpp

#endif

