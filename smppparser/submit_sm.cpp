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
/// @file submit_sm.cpp
/// @brief Declares a submit_sm

#include "submit_sm.hpp"

/// @brief Default constructor.
/// @note Sets all parameters to their default values.
Smpp::SubmitSm::SubmitSm() :
    Request(CommandLength(MinLength), 
    CommandId(CommandId::SubmitSm), 
    SequenceNumber::Min)
{
}

/// @brief Constructor that requires values for all the mandatory parameters.
/// @note The sm_length parameter is derived from the short_message parameter.
/// @param sequenceNumber The sequence number to use.
/// @param serviceType The service type to use.
/// @param sourceAddr The source address (ton, npi and address)t o use.
/// @param destinationAddr The destination address (ton, npi and address).
/// @param esmClass The esm class to use.
/// @param protocolId The protocol Id to use.
/// @param priorityFlag The priority flag to use to use.
/// @param scheduleDeliveryTime The schedule delivery time to use.
/// @param validityPeriod The validity period to use.
/// @param registeredDelivery The registered delivery to use.
/// @param replaceIfPresentFlag The replace if present flag to use.
/// @param dataCoding The data coding value to use.
/// @param smDefaultMsgId The SM default message Id to use.
/// @param shortMessage The short message to use, its length is sm_length.
Smpp::SubmitSm::SubmitSm(
        const SequenceNumber& sequenceNumber,
        const ServiceType& serviceType,
        const SmeAddress& sourceAddr,
        const SmeAddress& destinationAddr,
        const EsmClass& esmClass,
        const ProtocolId& protocolId,
        const PriorityFlag& priorityFlag,
        const Smpp::Time& scheduleDeliveryTime,
        const Smpp::Time& validityPeriod,
        const RegisteredDelivery& registeredDelivery,
        const ReplaceIfPresentFlag& replaceIfPresentFlag,
        const DataCoding& dataCoding,
        const SmDefaultMsgId& smDefaultMsgId,
        const ShortMessage& shortMessage) :
    Request(CommandLength(MinLength), 
             CommandId(CommandId::SubmitSm), 
             sequenceNumber),
    service_type_(serviceType),
    source_addr_(sourceAddr),
    destination_addr_(destinationAddr),
    esm_class_(esmClass),
    protocol_id_(protocolId),
    priority_flag_(priorityFlag),
    schedule_delivery_time_(scheduleDeliveryTime),
    validity_period_(validityPeriod),
    registered_delivery_(registeredDelivery),
    replace_if_present_flag_(replaceIfPresentFlag),
    data_coding_(dataCoding),
    sm_default_msg_id_(smDefaultMsgId),
    short_message_(shortMessage)
{
    Header::update_length(service_type_.length() +
                           source_addr_.address().length() +
                           destination_addr_.address().length() +
                           schedule_delivery_time_.length() +
                           validity_period_.length() +
                           short_message_.length());
}

/// @brief Construct from a buffer.
/// @param b The buffer (octet array).
Smpp::SubmitSm::SubmitSm(const Smpp::Uint8* b) :
    Request(CommandLength(MinLength), CommandId(CommandId::SubmitSm), 1)
{
    decode(b);
}

Smpp::SubmitSm::~SubmitSm()
{
}

/// @brief Encode the message into an octet array.
/// @return Pointer to the encoded message.
/// @note The Buffer length is the command_length.
const Smpp::Uint8*
Smpp::SubmitSm::encode()
{
    buff_.reset(Header::command_length());

    Request::encode(buff_); // insert the header first
    buff_ += service_type_;
    buff_ += source_addr_;
    buff_ += destination_addr_;
    buff_ += esm_class_;
    buff_ += protocol_id_;
    buff_ += priority_flag_;
    buff_ += schedule_delivery_time_;
    buff_ += validity_period_;
    buff_ += registered_delivery_;
    buff_ += replace_if_present_flag_;
    buff_ += data_coding_;
    buff_ += sm_default_msg_id_;
    buff_ += short_message_;

    Header::encode_tlvs(buff_);

    return buff_.get();
}

/// @brief Decode the message from an octet array.
/// @param buff The octet array to decode.
/// @note The octet array maybe the data read from a socket.
void
Smpp::SubmitSm::decode(const Smpp::Uint8* buff)
{
    Request::decode(buff);

    Smpp::Uint32 len = Request::command_length();
    Smpp::Uint32 offset = 16;
    const char* err = "Bad length in submit_sm";
    if(len < offset)
        throw Error(err);
 
    const Smpp::Char* sptr = reinterpret_cast<const Smpp::Char*>(buff);

    service_type_ = &sptr[offset];
    offset += service_type_.length() + 1;
    if(len < offset)
        throw Error(err);

    source_addr_.decode(buff+offset, len - offset);
    offset += source_addr_.address().length() + 3; // ton + npi + '\0'
    if(len < offset)
        throw Error(err);

    destination_addr_.decode(buff+offset, len - offset);
    offset += destination_addr_.address().length() + 3; // ton + npi + '\0'
    if(len < offset)
        throw Error(err);

    esm_class_ = buff[offset];
    ++offset;
    if(len < offset)
        throw Error(err);
    
    protocol_id_ = buff[offset];
    ++offset;
    if(len < offset)
        throw Error(err);
    
    priority_flag_ = buff[offset];
    ++offset;
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
    
    replace_if_present_flag_ = buff[offset];
    ++offset;
    if(len < offset)
        throw Error(err);
    
    data_coding_ = buff[offset];
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

