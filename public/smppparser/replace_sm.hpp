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
#ifndef __SMPP_REPLACE_SM_HPP__
#define __SMPP_REPLACE_SM_HPP__

/// @file replace_sm.hpp
/// @brief An SMPP replace_sm PDU.

#include "aux_types.hpp"
#include "header.hpp"

namespace Smpp {

    /// @class ReplaceSm
    /// @brief Definition of an SMPP replace_sm, derived from a Request.
    class ReplaceSm : public Request {
        MessageId message_id_;
        SmeAddress source_addr_;
        Time schedule_delivery_time_;
        Time validity_period_;
        RegisteredDelivery registered_delivery_;
        SmDefaultMsgId sm_default_msg_id_;
        ShortMessage short_message_;

        Smpp::Buffer buff_;

      public:
        /** replace_sm minimum length in octets */
        const static int MinLength = 25;
        
        /** @brief Default constructor. */
        ReplaceSm();
        
        /** @brief Constructor requiring all mandatory parameters. */
        ReplaceSm(
                const SequenceNumber& sequenceNumber,
                const MessageId& serviceType,
                const SmeAddress& sourceAddr,
                const Time& scheduleDeliveryTime,
                const Time& validityPeriod,
                const RegisteredDelivery& registeredDelivery,
                const SmDefaultMsgId& smDefaultMsgId,
                const ShortMessage& shortMessage);
       
        /// @brief Construct from a buffer.
        ReplaceSm(const Smpp::Uint8* b);

        /// @brief Destructor - does nothing.
        ~ReplaceSm();
        
        //
        // Mutating
        //
        
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
        
        /// @brief Set the schedule delivery time.
        /// @param p The new schedule delivery time.
        void schedule_delivery_time(const Time& p) {
            int diff = p.length() - schedule_delivery_time_.length();
            schedule_delivery_time_ = p;
            Header::update_length(diff);
        }
        
        /// @brief Set the schedule delivery time.
        /// @param p The new schedule delivery time.
        void schedule_delivery_time(const Smpp::Char* p) {
            int diff = strlen(p) - schedule_delivery_time_.length();
            schedule_delivery_time_ = p;
            Header::update_length(diff);
        }
        
        /// @brief Set the validity period.
        /// @param p The new validity period.
        void validity_period(const Time& p) {
            int diff = p.length() - validity_period_.length();
            validity_period_ = p;
            Header::update_length(diff);
        }

        /// @brief Set the validity period.
        /// @param p The new validity period.
        void validity_period(const Smpp::Char* p) {
            int diff = strlen(p) - validity_period_.length();
            validity_period_ = p;
            Header::update_length(diff);
        }

        /// @brief Set the registered delivery.
        /// @param p The new registered delivery.
        void registered_delivery(const RegisteredDelivery& p) {
            registered_delivery_ = p;
        }
        
        /// @brief Set the SM default message Id.
        /// @param p The new SM default message Id.
        void sm_default_msg_id(const SmDefaultMsgId& p) {
            sm_default_msg_id_ = p;
        }
        
        /// @brief Set the short message and sm length.
        /// @param p The new short message.
        /// @param l The new short message length.
        void short_message(const Smpp::Uint8* p, Smpp::Uint8 l) {
            int diff = l - short_message_.length();
            short_message_ = ShortMessage(p, l);
            Header::update_length(diff);
        }
        
        // 
        // Accessing
        // 
        
        /// @brief Access the message id.
        /// @return The message id.
        const MessageId& message_id() const { return message_id_; }
        
        /// @brief Access the source address.
        /// @return The source address.
        const SmeAddress& source_addr() const { return source_addr_; }
        
        /// @brief Access the schedule delivery time.
        /// @return The schedule delivery time.
        const Time& schedule_delivery_time() const {
            return schedule_delivery_time_;
        }
        
        /// @brief Access the validity period.
        /// @return The validity period.
        const Time& validity_period() const { return validity_period_; }
        
        /// @brief Access the registered delivery.
        /// @return The registered delivery.
        const RegisteredDelivery& registered_delivery() const {
            return registered_delivery_;
        }
        
        /// @brief Access the SM default message id.
        /// @return The SM default message id.
        const SmDefaultMsgId& sm_default_msg_id() const {
            return sm_default_msg_id_;
        }
        
        /// @brief Access the short message length.
        /// @return The short message length.
        const Smpp::String::size_type sm_length() const {
            return short_message_.length();
        }
        
        /// @brief Access the short message.
        /// @return The short message.
        const ShortMessage& short_message() const { return short_message_; }
      
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

