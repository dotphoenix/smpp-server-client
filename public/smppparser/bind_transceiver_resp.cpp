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
/// @file bind_transceiver_resp.cpp
/// @brief Declares a bind_transceiver_resp

#include "bind_transceiver_resp.hpp"

/// @brief Default constructor.
/// note Sets all values to their defaults.
Smpp::BindTransceiverResp::BindTransceiverResp() :
    Response(CommandLength(MinLength), 
    CommandId(CommandId::BindTransceiverResp),
    CommandStatus(CommandStatus::ESME_ROK),
    SequenceNumber::Min)
{
}

/// @brief Constructor that requires values for all the mandatory parameters.
/// @param commandStatus The command status to use.
/// @param sequenceNumber The sequence number to use.
/// @param systemId The system id to use.
Smpp::BindTransceiverResp::BindTransceiverResp(
        const CommandStatus& commandStatus,
        const SequenceNumber& sequenceNumber,
        const SystemId& systemId) :
    Response(CommandLength(MinLength), 
             CommandId(CommandId::BindTransceiverResp), 
             commandStatus,
             sequenceNumber),
    system_id_(systemId)
{
    Header::update_length(system_id_.length());
}

/// @brief Construct from a buffer.
/// @param b The buffer (octet array).
Smpp::BindTransceiverResp::BindTransceiverResp(const Smpp::Uint8* b) :
    Response(CommandLength(MinLength),
              CommandId(CommandId::BindTransceiverResp),
              CommandStatus(CommandStatus::ESME_ROK),
              1)
{
    decode(b);
}

Smpp::BindTransceiverResp::~BindTransceiverResp()
{
}

/// @brief Encode the message into an octet array.
/// @return Pointer to the encoded message.
/// @note The Buffer length is the command_length.
const Smpp::Uint8*
Smpp::BindTransceiverResp::encode()
{
    buff_.reset(Header::command_length());

    Response::encode(buff_); // insert the header first
    buff_ += system_id_;

    Header::encode_tlvs(buff_);

    return buff_.get();
}

/// @brief Decode the message from an octet array.
/// @param buff The octet array to decode.
/// @note The octet array maybe the data read from a socket.
void
Smpp::BindTransceiverResp::decode(const Smpp::Uint8* buff)
{
    Response::decode(buff);

    Smpp::Uint32 len = Response::command_length();
    Smpp::Uint32 offset = 16;
    const char* err = "Bad length in bind_transceiver_resp";
    if(len < offset)
        throw Error(err);
 
    const Smpp::Char* sptr = reinterpret_cast<const Smpp::Char*>(buff);
    system_id_ = &sptr[offset];
    offset += system_id_.length() + 1;
    
    if(len < offset)
        throw Error(err);

    Header::decode_tlvs(buff + offset, len - offset);
}

