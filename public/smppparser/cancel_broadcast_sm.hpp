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
#ifndef __SMPP_CANCEL_BROADCAST_SM_HPP__
#define __SMPP_CANCEL_BROADCAST_SM_HPP__

/// @file cancel_broadcast_sm.hpp
/// @brief An SMPP cancel_broadcast_sm PDU.

#include "aux_types.hpp"
#include "header.hpp"

namespace Smpp {

    /// @class CancelBroadcastSm
    /// @brief Definition of an SMPP cancel_broadcast_sm, derived from a Request.
    class CancelBroadcastSm : public Request {
        ServiceType service_type_;
        MessageId message_id_;
        SmeAddress source_addr_;

        Smpp::Buffer buff_;

      public:
        /** cancel_broadcast_sm minimum length in octets */
        const static int MinLength = 21;
        
        /** @brief Default constructor. */
        CancelBroadcastSm();
        
        /** @brief Constructor requiring all mandatory parameters. */
        CancelBroadcastSm(
                const SequenceNumber& sequenceNumber,
                const ServiceType& serviceType,
                const MessageId& messageId,
                const SmeAddress& sourceAddr);
       
        /// @brief Construct from a buffer.
        CancelBroadcastSm(const Smpp::Uint8* b);

        /// @brief Destructor - does nothing.
        ~CancelBroadcastSm();
        
        //
        // Mutating
        //
        
        /// @brief Set the service type.
        /// @param p The new service type.
        void service_type(const ServiceType& p) {
            int diff = p.length() - service_type_.length();
            service_type_ = p;
            Header::update_length(diff);
        }
        
        /// @brief Set the service type.
        /// @param p The new service type.
        void service_type(const Smpp::Char* p) {
            int diff = strlen(p) - service_type_.length();
            service_type_ = p;
            Header::update_length(diff);
        }
        
        /// @brief Set the message id.
        /// @param p The new message id.
        void message_id(const MessageId& p) {
            int diff = p.length() - message_id_.length();
            message_id_ = p;
            Header::update_length(diff);
        }
        
        /// @brief Set the message id.
        /// @param p The new message id.
        void message_id(const Smpp::Char* p) {
            int diff = strlen(p) - message_id_.length();
            message_id_ = p;
            Header::update_length(diff);
        }
        
        /// @brief Set the source address using an SmeAddress object.
        /// @param p The new source address.
        void source_addr(const SmeAddress& p) {
            int diff = p.address().length() - source_addr_.address().length();
            source_addr_ = p;
            Header::update_length(diff);
        }

        /// @brief Set the source address using just the address.
        /// @param p The source address characters.
        void source_addr(const Address& p) {
            int diff = p.length() - source_addr_.address().length();
            source_addr_ = SmeAddress(Smpp::Ton(Smpp::Ton::Unknown),
                                       Smpp::Npi(Smpp::Npi::Unknown),
                                       p);
            Header::update_length(diff);
        }
        
        // 
        // Accessing
        // 
        
        /// @brief Access the service type.
        /// @return The service type.
        const ServiceType& service_type() const { return service_type_; }
        
        /// @brief Access the message id.
        /// @return The message id.
        const MessageId& message_id() const { return message_id_; }
        
        /// @brief Access the source address.
        /// @return The source address.
        const SmeAddress& source_addr() const { return source_addr_; }
        
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

