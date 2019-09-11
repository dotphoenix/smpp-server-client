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
#ifndef __SMPP_OUTBIND_HPP__
#define __SMPP_OUTBIND_HPP__

/// @file outbind.hpp
/// @brief An SMPP outbind PDU.

#include "aux_types.hpp"
#include "header.hpp"

namespace Smpp {

    /// @class Outbind
    /// @brief Definition of an SMPP outbind, derived from a Request.
    class Outbind : public Request {
        SystemId system_id_;
        Password password_;

        Smpp::Buffer buff_;

      public:
        /** submit_sm minimum length in octets */
        const static int MinLength = 18;
        
        /** @brief Default constructor. */
        Outbind();
        
        /** @brief Constructor requiring all mandatory parameters. */
        Outbind(
                const SequenceNumber& sequenceNumber,
                const SystemId& systemId,
                const Password& password);
       
        /// @brief Construct from a buffer.
        Outbind(const Smpp::Uint8* b);

        /// @brief Destructor - does nothing.
        ~Outbind();
        
        //
        // Mutating
        //
        
        /// @brief Set the system id.
        /// @param p The new system id.
        void system_id(const SystemId& p) {
            int diff = p.length() - system_id_.length();
            system_id_ = p;
            Header::update_length(diff);
        }
        
        /// @brief Set the system id.
        /// @param p The new system id.
        void system_id(const Smpp::Char* p) {
            int diff = strlen(p) - system_id_.length();
            system_id_ = p;
            Header::update_length(diff);
        }
        
        /// @brief Set the password.
        /// @param p The new password.
        void password(const Password& p) {
            int diff = p.length() - password_.length();
            password_ = p;
            Header::update_length(diff);
        }
        
        /// @brief Set the password.
        /// @param p The new password.
        void password(const Smpp::Char* p) {
            int diff = strlen(p) - password_.length();
            password_ = p;
            Header::update_length(diff);
        }
        
        // 
        // Accessing
        // 
        
        /// @brief Access the system id.
        /// @return The system id.
        const SystemId& system_id() const { return system_id_; }
        
        /// @brief Access the password.
        /// @return The password.
        const Password& password() const { return password_; }
        
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

