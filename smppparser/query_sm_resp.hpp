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
#ifndef __SMPP_QUERY_SM_RESP_HPP__
#define __SMPP_QUERY_SM_RESP_HPP__

/// @file query_sm_resp.hpp
/// @brief An SMPP query_sm_resp PDU.

#include "aux_types.hpp"
#include "header.hpp"

namespace Smpp {

    /// @class QuerySmResp
    /// @brief Definition of an SMPP query_sm_resp, derived from a Response.
    class QuerySmResp : public Response {
        MessageId message_id_;
        Smpp::Time final_date_;
        MessageState message_state_;
        ErrorCode error_code_;

        Smpp::Buffer buff_;

      public:
        /// query_sm_resp minimum length in octets.
        const static int MinLength = 20;
        
        /// @brief Default constructor.
        QuerySmResp();
        
        /// @brief Constructor requiring all mandatory parameters.
        QuerySmResp(const CommandStatus& commandStatus,
                     const SequenceNumber& sequenceNumber,
                     const MessageId& messageId,
                     const Smpp::Time& finalDate,
                     const MessageState& messageState,
                     const ErrorCode& errorCode);
       
        /// @brief Construct from a buffer.
        QuerySmResp(const Smpp::Uint8* b);

        /// @brief Destructor - does nothing.
        ~QuerySmResp();
        
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
       
        /// @brief Set the final date.
        /// @param p The new final date.
        void final_date(const Time& p) {
            int diff = p.length() - final_date_.length();
            final_date_ = p;
            Header::update_length(diff);
        }
        
        /// @brief Set the final date.
        /// @param p The new final date.
        void final_date(const Smpp::Char* p) {
            int diff = strlen(p) - final_date_.length();
            final_date_ = p;
            Header::update_length(diff);
        }
   
        /// @brief Set the message state.
        /// @param p The new message state.
        void message_state(const MessageState& p) {
            message_state_ = p;
        }
        
        /// @brief Set the error code.
        /// @param p The new error code.
        void error_code(const ErrorCode& p) {
            error_code_ = p;
        }
        
        //
        // Accessing
        //
        
        /// @brief Accesses the message id.
        /// @return The message id.
        const MessageId& message_id() const { return message_id_; }
     
        /// @brief Access the final date.
        /// @return The final date.
        const Time& final_date() const { return final_date_; }
       
        /// @brief Access the message state.
        /// @return The message state.
        const MessageState& message_state() const { return message_state_; }
        
        /// @brief Access the error code.
        /// @return The error code.
        const ErrorCode& error_code() const { return error_code_; }
        
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

