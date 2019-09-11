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
#ifndef __SMPP_QUERY_BROADCAST_SM_RESP_HPP__
#define __SMPP_QUERY_BROADCAST_SM_RESP_HPP__

/// @file query_broadcast_sm_resp.hpp
/// @brief An SMPP query_broadcast_sm_resp PDU.

#include "aux_types.hpp"
#include "header.hpp"

namespace Smpp {

    /// @class QueryBroadcastSmResp
    /// @brief Definition of an SMPP query_broadcast_sm_resp, derived from a Response.
    class QueryBroadcastSmResp : public Response {
        MessageId message_id_;
        MessageState message_state_;
       
        // true if the BroadcastAreaIdentifier is set from the default
        // constructor, in which case it should be replaced.
        bool defaultBroadcastAreaIdentifier_;

        Smpp::Buffer buff_;

      public:
        /// query_broadcast_sm_resp minimum length in octets.
        const static int MinLength = 18; // not including mandatory TLVs
        
        /// @brief Default constructor.
        QueryBroadcastSmResp();
        
        /// @brief Constructor requiring all mandatory parameters.
        QueryBroadcastSmResp(const CommandStatus& commandStatus,
                     const SequenceNumber& sequenceNumber,
                     const MessageId& messageId,
                     const MessageState& messageState,
                     const BroadcastAreaIdentifier& broadcastAreaIdentifier,
                     const BroadcastAreaSuccess& broadcastAreaSuccess);
       
        /// @brief Construct from a buffer.
        QueryBroadcastSmResp(const Smpp::Uint8* b);

        /// @brief Destructor - does nothing.
        ~QueryBroadcastSmResp();
        
        //
        // Mutating
        //
        
        /// @brief Sets the message id.
        /// @param p The message id.
        void message_id(const MessageId& p) {
            int diff = p.length() - message_id_.length();
            message_id_ = p;
            Header::update_length(diff);
        }
        
        /// @brief Sets the message id.
        /// @param p The message id.
        void message_id(const Smpp::Char* p) {
            int diff = strlen(p) - message_id_.length();
            message_id_ = p;
            Header::update_length(diff);
        }
       
        /// @brief Set the message state.
        /// @param p The new message state.
        void message_state(const MessageState& p) {
            message_state_ = p;
        }
       
        /// @brief Set the broadcast area identifier.
        /// @param p The new broadcast area identifier.
        void broadcast_area_identifier(const BroadcastAreaIdentifier& p) {
            if(defaultBroadcastAreaIdentifier_) {
                remove_tlv(Tlv::broadcast_area_identifier);
                defaultBroadcastAreaIdentifier_ = false;
            }
            this->insert_before_tlv(new Tlv(p), Tlv::broadcast_area_success);
        }
       
        /// @brief Set the broadcast area success.
        /// @param p The new broadcast area success.
        void broadcast_area_success(const BroadcastAreaSuccess& p) {
            remove_tlv(Tlv::broadcast_area_success);
            this->insert_after_tlv(new Tlv(p), Tlv::broadcast_area_identifier);
        }
 
        //
        // Accessing
        //
 
        /// @brief Accesses the message id.
        /// @return The message id.
        const MessageId& message_id() const { return message_id_; }
     
        /// @brief Access the message state.
        /// @return The message state.
        const MessageState& message_state() const { return message_state_; }

        /// @brief Access the broadcast area identifiers.
        /// @return List of broadcast area identifiers pointers.
        const TlvList broadcast_area_identifier() const {
            return find_tlv_list(Tlv::broadcast_area_identifier);
        }

        /// @brief Access the broadcast area success.
        /// @return The broadcast area success.
        const BroadcastAreaSuccess& broadcast_area_success() const {
            const Tlv* t = find_tlv(Tlv::broadcast_area_success);
            if(!t)
                throw Smpp::Error("Missing broadcast_area_success");
            return *static_cast<const BroadcastAreaSuccess*>(t);
        }
        
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

