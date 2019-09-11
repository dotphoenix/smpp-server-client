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
#ifndef __SMPP_ALERT_NOTIFICATION_HPP__
#define __SMPP_ALERT_NOTIFICATION_HPP__

/// @file alert_notification.hpp
/// @brief An SMPP alert_notification PDU.

#include "aux_types.hpp"
#include "header.hpp"

namespace Smpp {

    /// @class AlertNotification
    /// @brief Definition of an SMPP alert_notification, derived from a Request.
    class AlertNotification : public Request {
        SmeAddress source_addr_;
        SmeAddress esme_addr_;

        Smpp::Buffer buff_;

      public:
        /** alert_notification minimum length in octets */
        const static int MinLength = 22;
        
        /** @brief Default constructor. */
        AlertNotification();
        
        /** @brief Constructor requiring all mandatory parameters. */
        AlertNotification(
                const SequenceNumber& sequenceNumber,
                const SmeAddress& sourceAddr,
                const SmeAddress& esmeAddr);
       
        /// @brief Construct from a buffer.
        AlertNotification(const Smpp::Uint8* b);

        /// @brief Destructor - does nothing.
        ~AlertNotification();
        
        //
        // Mutating
        //
        
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
        
        /// @brief Set the esme address using an SmeAddress object.
        /// @param p The new esme address.
        void esme_addr(const SmeAddress& p) {
            int diff =
                p.address().length() - esme_addr_.address().length();
            esme_addr_ = p;
            Header::update_length(diff);
        }
        
        /// @brief Set the esme address using the separate parameters.
        /// @param t The esme address TON.
        /// @param n The esme address NPI.
        /// @param p The esme address characters.
        void destination_addr(const Ton& t, const Npi& n, const Address& p) {
            int diff = p.length() - esme_addr_.address().length();
            esme_addr_ = SmeAddress(t, n, p);
            Header::update_length(diff);
        }

        // 
        // Accessing
        // 
        
        /// @brief Access the source address.
        /// @return The source address.
        const SmeAddress& source_addr() const { return source_addr_; }
        
        /// @brief Access the esme address.
        /// @return The esme address.
        const SmeAddress& esme_addr() const { return esme_addr_; }
        
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

