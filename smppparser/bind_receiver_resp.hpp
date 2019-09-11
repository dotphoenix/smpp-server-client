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
#ifndef __SMPP_BIND_RECEIVER_RESP_HPP__
#define __SMPP_BIND_RECEIVER_RESP_HPP__

/// @file bind_receiver_resp.hpp
/// @brief An SMPP bind_receiver_resp PDU.

#include "aux_types.hpp"
#include "header.hpp"

namespace Smpp {

    /// @class BindReceiverResp
    /// @brief Definition of an SMPP bind_receiver_resp, derived from a Response.
    class BindReceiverResp : public Response {
        SystemId system_id_;

        Smpp::Buffer buff_;

      public:
        /// bind_receiver_resp minimum length in octets.
        const static int MinLength = 17;
        
        /// @brief Default constructor.
        BindReceiverResp();
        
        /// @brief Constructor requiring all mandatory parameters.
        BindReceiverResp(
                const CommandStatus& commandStatus,
                const SequenceNumber& sequenceNumber,
                const SystemId& systemId);
       
        /// @brief Construct from a buffer.
        BindReceiverResp(const Smpp::Uint8* b);

        /// @brief Destructor - does nothing.
        ~BindReceiverResp();
        
        //
        // Mutating
        //
        
        /// @brief Sets the system id.
        /// @param p The system id.
        void system_id(const SystemId& p) {
            int diff = p.length() - system_id_.length();
            system_id_ = p;
            Header::update_length(diff);
        }
        
        /// @brief Sets the system id.
        /// @param p The system id.
        void system_id(const Smpp::Char* p) {
            int diff = strlen(p) - system_id_.length();
            system_id_ = p;
            Header::update_length(diff);
        }
        
        //
        // Accessing
        //
        
        /// @brief Accesses the system id.
        /// @return The system id.
        const SystemId& system_id() const { return system_id_; }
      
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

