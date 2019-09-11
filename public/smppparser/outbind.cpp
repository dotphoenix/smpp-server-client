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
/// @file outbind.cpp
/// @brief Declares an outbind

#include "outbind.hpp"

/// @brief Default constructor.
/// @note Sets all parameters to their default values.
Smpp::Outbind::Outbind() :
    Request(CommandLength(MinLength), 
    CommandId(CommandId::Outbind), 
    SequenceNumber::Min)
{
}

/// @brief Constructor that requires values for all the mandatory parameters.
/// @param sequenceNumber The sequence number to use.
/// @param systemId The system id to use.
/// @param password The password to use.
/// @param systemType The system type to use.
/// @param interfaceVersion The interface version to use.
/// @param addrTon The addr ton to use.
/// @param addrNpi The addr npi to use.
/// @param addressRange The address range to use.
Smpp::Outbind::Outbind(
        const SequenceNumber& sequenceNumber,
        const SystemId& systemId,
        const Password& password) :
    Request(CommandLength(MinLength), 
             CommandId(CommandId::Outbind), 
             sequenceNumber),
    system_id_(systemId),
    password_(password)
{
    Header::update_length(system_id_.length() +
                           password_.length());
}

/// @brief Construct from a buffer.
/// @param b The buffer (octet array).
Smpp::Outbind::Outbind(const Smpp::Uint8* b) :
    Request(CommandLength(MinLength),
    CommandId(CommandId::Outbind),
    1)
{
    decode(b);
}

Smpp::Outbind::~Outbind()
{
}

/// @brief Encode the message into an octet array.
/// @return Pointer to the encoded message.
/// @note The Buffer length is the command_length.
const Smpp::Uint8*
Smpp::Outbind::encode()
{
    buff_.reset(Header::command_length());

    Request::encode(buff_); // insert the header first
    buff_ += system_id_;
    buff_ += password_;

    Header::encode_tlvs(buff_);

    return buff_.get();
}

/// @brief Decode the message from an octet array.
/// @param buff The octet array to decode.
/// @note The octet array maybe the data read from a socket.
void
Smpp::Outbind::decode(const Smpp::Uint8* buff)
{
    Request::decode(buff);

    Smpp::Uint32 len = Request::command_length();
    Smpp::Uint32 offset = 16;
    const char* err = "Invalid outbind PDU length";
    if(len < offset)
        throw Error(err);
 
    const Smpp::Char* sptr = reinterpret_cast<const Smpp::Char*>(buff);

    system_id_ = &sptr[offset];
    offset += system_id_.length() + 1;
    if(len < offset)
        throw Error(err);

    password_ = &sptr[offset];
    offset += password_.length() + 1;
    if(len < offset)
        throw Error(err);

    Header::decode_tlvs(buff + offset, len - offset);
}

