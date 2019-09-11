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
/// @file replace_sm.cpp
/// @brief Declares a replace_sm

#include "replace_sm.hpp"

/// @brief Default constructor.
/// @note Sets all parameters to their default values.
Smpp::ReplaceSm::ReplaceSm() :
    Request(CommandLength(MinLength), 
    CommandId(CommandId::ReplaceSm), 
    SequenceNumber::Min)
{
}

/// @brief Constructor that requires values for all the mandatory parameters.
/// @note The sm_length parameter is derived from the short_message parameter.
/// @param sequenceNumber The sequence number to use.
/// @param messageId The service type to use.
/// @param sourceAddr The source address (ton, npi and address)t o use.
/// @param destinationAddr The destination address (ton, npi and address).
/// @param scheduleDeliveryTime The schedule delivery time to use.
/// @param validityPeriod The validity period to use.
/// @param registeredDelivery The registered delivery to use.
/// @param smDefaultMsgId The SM default message Id to use.
/// @param shortMessage The short message to use, its length is sm_length.
Smpp::ReplaceSm::ReplaceSm(
        const SequenceNumber& sequenceNumber,
        const MessageId& messageId,
        const SmeAddress& sourceAddr,
        const Smpp::Time& scheduleDeliveryTime,
        const Smpp::Time& validityPeriod,
        const RegisteredDelivery& registeredDelivery,
        const SmDefaultMsgId& smDefaultMsgId,
        const ShortMessage& shortMessage) :
    Request(CommandLength(MinLength), 
             CommandId(CommandId::ReplaceSm), 
             sequenceNumber),
    message_id_(messageId),
    source_addr_(sourceAddr),
    schedule_delivery_time_(scheduleDeliveryTime),
    validity_period_(validityPeriod),
    registered_delivery_(registeredDelivery),
    sm_default_msg_id_(smDefaultMsgId),
    short_message_(shortMessage)
{
    Header::update_length(message_id_.length() +
                           source_addr_.address().length() +
                           schedule_delivery_time_.length() +
                           validity_period_.length() +
                           short_message_.length());
}

/// @brief Construct from a buffer.
/// @param b The buffer (octet array).
Smpp::ReplaceSm::ReplaceSm(const Smpp::Uint8* b) :
    Request(CommandLength(MinLength), CommandId(CommandId::ReplaceSm), 1)
{
    decode(b);
}

Smpp::ReplaceSm::~ReplaceSm()
{
}

/// @brief Encode the message into an octet array.
/// @return Pointer to the encoded message.
/// @note The Buffer length is the command_length.
const Smpp::Uint8*
Smpp::ReplaceSm::encode()
{
    buff_.reset(Header::command_length());

    Request::encode(buff_); // insert the header first
    buff_ += message_id_;
    buff_ += source_addr_;
    buff_ += schedule_delivery_time_;
    buff_ += validity_period_;
    buff_ += registered_delivery_;
    buff_ += sm_default_msg_id_;
    buff_ += short_message_;

    Header::encode_tlvs(buff_);

    return buff_.get();
}

/// @brief Decode the message from an octet array.
/// @param buff The octet array to decode.
/// @note The octet array maybe the data read from a socket.
void
Smpp::ReplaceSm::decode(const Smpp::Uint8* buff)
{
    Request::decode(buff);

    Smpp::Uint32 len = Request::command_length();
    Smpp::Uint32 offset = 16;
    const char* err = "Bad length in replace_sm";
    if(len < offset)
        throw Error(err);
 
    const Smpp::Char* sptr = reinterpret_cast<const Smpp::Char*>(buff);

    message_id_ = &sptr[offset];
    offset += message_id_.length() + 1;
    if(len < offset)
        throw Error(err);

    source_addr_.decode(buff+offset, len - offset);
    offset += source_addr_.address().length() + 3; // ton + npi + '\0'
    if(len < offset)
        throw Error(err);

    schedule_delivery_time_ = &sptr[offset];
    offset += schedule_delivery_time_.length() + 1;
    if(len < offset)
        throw Error(err);
 
    validity_period_ = &sptr[offset];
    offset += validity_period_.length() + 1;
    if(len < offset)
        throw Error(err);
 
    registered_delivery_ = buff[offset];
    ++offset;
    if(len < offset)
        throw Error(err);
 
    sm_default_msg_id_ = buff[offset];
    ++offset;
    if(len < offset)
        throw Error(err);

    short_message_ = buff+offset;
    offset += short_message_.length() + 1; // plus sm_length
    if(len < offset)
        throw Error(err);

    Header::decode_tlvs(buff + offset, len - offset);
}

