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
/// @file bind_transceiver.cpp
/// @brief Declares a bind_transceiver

#include "bind_transceiver.hpp"

/// @brief Default constructor.
/// @note Sets all parameters to their default values.
Smpp::BindTransceiver::BindTransceiver() :
    Request(CommandLength(MinLength), 
    CommandId(CommandId::BindTransceiver), 
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
Smpp::BindTransceiver::BindTransceiver(
        const SequenceNumber& sequenceNumber,
        const SystemId& systemId,
        const Password& password,
        const SystemType& systemType,
        const InterfaceVersion& interfaceVersion,
        const Ton& addrTon,
        const Npi& addrNpi,
        const AddressRange& addressRange) :
    Request(CommandLength(MinLength), 
             CommandId(CommandId::BindTransceiver), 
             sequenceNumber),
    system_id_(systemId),
    password_(password),
    system_type_(systemType),
    interface_version_(interfaceVersion),
    addr_ton_(addrTon),
    addr_npi_(addrNpi),
    address_range_(addressRange)
{
    Header::update_length(system_id_.length() +
                           password_.length() +
                           system_type_.length() +
                           address_range_.length());
}

/// @brief Construct from a buffer.
/// @param b The buffer (octet array).
Smpp::BindTransceiver::BindTransceiver(const Smpp::Uint8* b) :
    Request(CommandLength(MinLength),
    CommandId(CommandId::BindTransceiver),
    1)
{
    decode(b);
}

Smpp::BindTransceiver::~BindTransceiver()
{
}

/// @brief Encode the message into an octet array.
/// @return Pointer to the encoded message.
/// @note The Buffer length is the command_length.
const Smpp::Uint8*
Smpp::BindTransceiver::encode()
{
    buff_.reset(Header::command_length());

    Request::encode(buff_); // insert the header first
    buff_ += system_id_;
    buff_ += password_;
    buff_ += system_type_;
    buff_ += interface_version_;
    buff_ += addr_ton_;
    buff_ += addr_npi_;
    buff_ += address_range_;

    Header::encode_tlvs(buff_);

    return buff_.get();
}

/// @brief Decode the message from an octet array.
/// @param buff The octet array to decode.
/// @note The octet array maybe the data read from a socket.
void
Smpp::BindTransceiver::decode(const Smpp::Uint8* buff)
{
    Request::decode(buff);

    Smpp::Uint32 len = Request::command_length();
    Smpp::Uint32 offset = 16;
    const char* err = "Invalid bind PDU length";
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

    system_type_ = &sptr[offset];
    offset += system_type_.length() + 1;
    if(len < offset)
        throw Error(err);

    interface_version_ = buff[offset];
    ++offset;
   
    addr_ton_ = buff[offset];
    ++offset;
   
    addr_npi_ = buff[offset];
    ++offset;
   
    address_range_ = &sptr[offset];
    offset += address_range_.length() + 1;
    if(len < offset)
        throw Error(err);

    Header::decode_tlvs(buff + offset, len - offset);
}

