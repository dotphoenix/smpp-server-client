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
/// @file cancel_broadcast_sm.cpp
/// @brief Declares a cancel_broadcast_sm

#include "cancel_broadcast_sm.hpp"

/// @brief Default constructor.
/// @note Sets all parameters to their default values.
Smpp::CancelBroadcastSm::CancelBroadcastSm() :
    Request(CommandLength(MinLength), 
    CommandId(CommandId::CancelBroadcastSm), 
    SequenceNumber::Min)
{
}

/// @brief Constructor that requires values for all the mandatory parameters.
/// @note The sm_length parameter is derived from the short_message parameter.
/// @param sequenceNumber The sequence number to use.
/// @param serviceType The service type to use.
/// @param messageId The message id to use.
/// @param sourceAddr The source address (ton, npi and address)t o use.
Smpp::CancelBroadcastSm::CancelBroadcastSm(
        const SequenceNumber& sequenceNumber,
        const ServiceType& serviceType,
        const MessageId& messageId,
        const SmeAddress& sourceAddr) :
    Request(CommandLength(MinLength), 
             CommandId(CommandId::CancelBroadcastSm), 
             sequenceNumber),
    service_type_(serviceType),
    message_id_(messageId),
    source_addr_(sourceAddr)
{
    Header::update_length(service_type_.length() +
                           message_id_.length() +
                           source_addr_.address().length());
}

/// @brief Construct from a buffer.
/// @param b The buffer (octet array).
Smpp::CancelBroadcastSm::CancelBroadcastSm(const Smpp::Uint8* b) :
    Request(CommandLength(MinLength),
    CommandId(CommandId::CancelBroadcastSm),
    1)
{
    decode(b);
}

Smpp::CancelBroadcastSm::~CancelBroadcastSm()
{
}

/// @brief Encode the message into an octet array.
/// @return Pointer to the encoded message.
/// @note The Buffer length is the command_length.
const Smpp::Uint8*
Smpp::CancelBroadcastSm::encode()
{
    buff_.reset(Header::command_length());

    Request::encode(buff_); // insert the header first
    buff_ += service_type_;
    buff_ += message_id_;
    buff_ += source_addr_;

    Header::encode_tlvs(buff_);

    return buff_.get();
}

/// @brief Decode the message from an octet array.
/// @param buff The octet array to decode.
/// @note The octet array maybe the data read from a socket.
void
Smpp::CancelBroadcastSm::decode(const Smpp::Uint8* buff)
{
    Request::decode(buff);

    Smpp::Uint32 len = Request::command_length();
    Smpp::Uint32 offset = 16;
    const char* err = "Bad length in cancel_broadcast_sm";
    if(len < offset)
        throw Error(err);
 
    const Smpp::Char* sptr = reinterpret_cast<const Smpp::Char*>(buff);

    service_type_ = &sptr[offset];
    offset += service_type_.length() + 1;
    if(len < offset)
        throw Error(err);

    message_id_ = &sptr[offset];
    offset += message_id_.length() + 1;
    if(len < offset)
        throw Error(err);

    source_addr_.decode(buff+offset, len - offset);
    offset += source_addr_.address().length() + 3; // ton + npi + '\0'
    if(len < offset)
        throw Error(err);

    Header::decode_tlvs(buff + offset, len - offset);
}

