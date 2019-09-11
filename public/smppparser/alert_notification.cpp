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
/// @file alert_notification.cpp
/// @brief Declares a alert_notification

#include "alert_notification.hpp"

/// @brief Default constructor.
/// @note Sets all parameters to their default values.
Smpp::AlertNotification::AlertNotification() :
    Request(CommandLength(MinLength), 
    CommandId(CommandId::AlertNotification), 
    SequenceNumber::Min)
{
}

/// @brief Constructor that requires values for all the mandatory parameters.
/// @param sequenceNumber The sequence number to use.
/// @param sourceAddr The source address (ton, npi and address)t o use.
/// @param esmeAddr The esme address (ton, npi and address).
Smpp::AlertNotification::AlertNotification(
        const SequenceNumber& sequenceNumber,
        const SmeAddress& sourceAddr,
        const SmeAddress& esmeAddr) :
    Request(CommandLength(MinLength), 
             CommandId(CommandId::AlertNotification), 
             sequenceNumber),
    source_addr_(sourceAddr),
    esme_addr_(esmeAddr)
{
    Header::update_length(source_addr_.address().length() +
                           esme_addr_.address().length());
}

/// @brief Construct from a buffer.
/// @param b The buffer (octet array).
Smpp::AlertNotification::AlertNotification(const Smpp::Uint8* b) :
    Request(CommandLength(MinLength),
             CommandId(CommandId::AlertNotification),
             1)
{
    decode(b);
}

Smpp::AlertNotification::~AlertNotification()
{
}

/// @brief Encode the message into an octet array.
/// @return Pointer to the encoded message.
/// @note The Buffer length is the command_length.
const Smpp::Uint8*
Smpp::AlertNotification::encode()
{
    buff_.reset(Header::command_length());

    Request::encode(buff_); // insert the header first
    buff_ += source_addr_;
    buff_ += esme_addr_;

    Header::encode_tlvs(buff_);

    return buff_.get();
}

/// @brief Decode the message from an octet array.
/// @param buff The octet array to decode.
/// @note The octet array maybe the data read from a socket.
void
Smpp::AlertNotification::decode(const Smpp::Uint8* buff)
{
    Request::decode(buff);

    Smpp::Uint32 len = Request::command_length();
    Smpp::Uint32 offset = 16;
    const char* err = "Bad length in alert_notification";
    if(len < offset)
        throw Error(err);
 
    source_addr_.decode(buff+offset, len - offset);
    offset += source_addr_.address().length() + 3; // ton + npi + '\0'
    if(len < offset)
        throw Error(err);

    esme_addr_.decode(buff+offset, len - offset);
    offset += esme_addr_.address().length() + 3; // ton + npi + '\0'
    if(len < offset)
        throw Error(err);

    Header::decode_tlvs(buff + offset, len - offset);
}

