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
/// @file header.cpp
/// @brief Declaration of an SMPP header.

#include "header.hpp"

/// @brief Constructor that is only called by derived classes.
/// @note This is the only place where command id can be set, i.e. during
/// derived class construction.
/// @param commandLength The command length.
/// @param commandID The command id.
/// @param commandStatus The command status.
/// @param sequenceNumber The sequence number.
Smpp::Header::Header(const CommandLength& commandLength, 
        const CommandId& commandID, 
        const CommandStatus& commandStatus, 
        const SequenceNumber& sequenceNumber) :
    command_length_(commandLength),
    command_id_(commandID),
    command_status_(commandStatus),
    sequence_number_(sequenceNumber)
{
}

Smpp::Header::~Header()
{
}

/// @brief Decode octets into tlv objects.
/// @param b The buffer to decode (start of the tlv)
/// @param len The number of octets left in the buffer.
void
Smpp::Header::decode_tlvs(const Smpp::Uint8* b, Smpp::Uint32 len)
{
    while(len > 0) {
        // should be enough bytes for tag and length.
        if(len < 4)
            throw Smpp::Error("Invalid command_length");

	// decode the TLV tag
        Smpp::Uint16 tag;
        std::copy(b, b+2, reinterpret_cast<Uint8*>(&tag));
        tag = Smpp::ntoh16(tag);
        b += 2;
        len -= 2;
       
	//  decode the TLV length 
        Smpp::Uint16 length;
        std::copy(b, b+2, reinterpret_cast<Uint8*>(&length));
        length = Smpp::ntoh16(length);
        b += 2;
        len -= 2;
     
        // should be enough bytes for the TLV value.
        if(len < length)
            throw Smpp::Error("Invalid command_length");

        tlvs_.list_.push_back(new Tlv(tag, length, b));
        b += length;
        len -= length;
    }
}
        
/// @brief Insert a TLV after the last occurance of a another TLV.
/// Used to add mandatory TLVs (broadcast_sm) in order listed in spec.
/// @param tlv The TLV to add.
/// @param tag The id of the previous tlv.
void
Smpp::Header::insert_after_tlv(const Tlv* tlv, Smpp::Uint16 tag)
{
    TlvList::reverse_iterator i;
    i = std::find_if(tlvs_.list_.rbegin(), tlvs_.list_.rend(),
                     Tlv::CompareTag(tag));
    if(i == tlvs_.list_.rend())
        throw Smpp::Error("Missing mandatory TLV");
            
    tlvs_.list_.insert(i.base(), tlv);
    update_length(tlv->length() + 4);
}

/// @brief Insert a TLV before the first occurance of a another TLV.
/// Used to add mandatory TLVs (broadcast_sm) in order listed in spec.
/// @param tlv The TLV to add.
/// @param tag The id of the previous tlv.
void
Smpp::Header::insert_before_tlv(const Tlv* tlv, Smpp::Uint16 tag)
{
    TlvList::iterator i;
    i = std::find_if(tlvs_.list_.begin(), tlvs_.list_.end(),
                     Tlv::CompareTag(tag));
    if(i == tlvs_.list_.end())
        throw Smpp::Error("Missing mandatory TLV");
    
    tlvs_.list_.insert(i, tlv);
    update_length(tlv->length() + 4);
}

/// @brief Constructor for a request PDU, command status is abscence as it
/// defaults to 0.
/// @param commandLength The command length.
/// @param commandID The command id.
/// @param sequenceNumber The sequence number.
Smpp::Request::Request(
        const CommandLength& commandLength,
        const CommandId& commandID,
        const SequenceNumber& sequenceNumber) : 
    Header(commandLength, commandID, CommandStatus::ESME_ROK, sequenceNumber)
{
}

/// @brief Constructor for a response PDU.
/// @param commandLength The command length.
/// @param commandID The command id.
/// @param commandStatus The command status.
/// @param sequenceNumber The sequence number.
Smpp::Response::Response(
        const CommandLength& commandLength,
        const CommandId& commandID,
        const CommandStatus& commandStatus,
        const SequenceNumber& sequenceNumber) : 
    Header(commandLength, commandID, commandStatus, sequenceNumber)
{
}

