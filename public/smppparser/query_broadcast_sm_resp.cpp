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
/// @file query_broadcast_sm_resp.cpp
/// @brief Declares a query_broadcast_sm_resp

#include "query_broadcast_sm_resp.hpp"

/// @brief Default constructor.
/// note Sets all values to their defaults.
Smpp::QueryBroadcastSmResp::QueryBroadcastSmResp() :
    Response(CommandLength(MinLength), 
    CommandId(CommandId::QueryBroadcastSmResp),
    CommandStatus(CommandStatus::ESME_ROK),
    SequenceNumber::Min),
    defaultBroadcastAreaIdentifier_(true)
{
    // The command_length is automatically updated
    insert_tlv(BroadcastAreaIdentifier(0x00, 0, 0));
    insert_tlv(BroadcastAreaSuccess(255 /* Unavailable */));
}

/// @brief Constructor that requires values for all the mandatory parameters.
/// @param commandStatus The command status to use.
/// @param sequenceNumber The sequence number to use.
/// @param messageId The message id to use.
/// @param messageState The message state to use.
/// @param broadcastAreaIdentifier The broadcast_area_identifier.
/// @param broadcastAreaSuccess The broadcast_area_success.
Smpp::QueryBroadcastSmResp::QueryBroadcastSmResp(
        const CommandStatus& commandStatus,
        const SequenceNumber& sequenceNumber,
        const MessageId& messageId,
        const MessageState& messageState,
        const BroadcastAreaIdentifier& broadcastAreaIdentifier,
        const BroadcastAreaSuccess& broadcastAreaSuccess) :
    Response(CommandLength(MinLength), 
             CommandId(CommandId::QueryBroadcastSmResp), 
             commandStatus,
             sequenceNumber),
    message_id_(messageId),
    message_state_(messageState),
    defaultBroadcastAreaIdentifier_(false)
{
    Header::update_length(message_id_.length());

    // insert the tlvs using their mutate operations.
    broadcast_area_identifier(broadcastAreaIdentifier);
    broadcast_area_success(broadcastAreaSuccess);
}

/// @brief Construct from a buffer.
/// @param b The buffer (octet array).
Smpp::QueryBroadcastSmResp::QueryBroadcastSmResp(const Smpp::Uint8* b) :
    Response(CommandLength(MinLength),
              CommandId(CommandId::QueryBroadcastSmResp),
              CommandStatus(CommandStatus::ESME_ROK),
              1)
{
    decode(b);
}

Smpp::QueryBroadcastSmResp::~QueryBroadcastSmResp()
{
}

/// @brief Encode the message into an octet array.
/// @return Pointer to the encoded message.
/// @note The Buffer length is the command_length.
const Smpp::Uint8*
Smpp::QueryBroadcastSmResp::encode()
{
    buff_.reset(Header::command_length());

    Response::encode(buff_); // insert the header first
    buff_ += message_id_;
    buff_ += message_state_;

    Header::encode_tlvs(buff_);

    return buff_.get();
}

/// @brief Decode the message from an octet array.
/// @param buff The octet array to decode.
/// @note The octet array maybe the data read from a socket.
void
Smpp::QueryBroadcastSmResp::decode(const Smpp::Uint8* buff)
{
    Response::decode(buff);

    Smpp::Uint32 len = Response::command_length();
    Smpp::Uint32 offset = 16;
    const char* err = "Bad length in query_broadcast_sm_resp";
    if(len < offset)
        throw Error(err);
 
    const Smpp::Char* sptr = reinterpret_cast<const Smpp::Char*>(buff);

    message_id_ = &sptr[offset];
    offset += message_id_.length() + 1;
    if(len < offset)
        throw Error(err);

    message_state_ = buff[offset];
    ++offset;
    if(len < offset)
        throw Error(err);
    
    Header::decode_tlvs(buff + offset, len - offset);
}

