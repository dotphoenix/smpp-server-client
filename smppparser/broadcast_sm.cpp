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
/// @file broadcast_sm.cpp
/// @brief Declares a broadcast_sm

#include "broadcast_sm.hpp"

/// @brief Default constructor.
/// @note Sets all parameters to their default values.
Smpp::BroadcastSm::BroadcastSm() :
    Request(CommandLength(MinLength), 
    CommandId(CommandId::BroadcastSm), 
    SequenceNumber::Min),
    defaultBroadcastAreaIdentifier_(true)
{
    // The command_length is automatically updated
    insert_tlv(BroadcastAreaIdentifier(0x00, 0, 0));
    insert_tlv(BroadcastContentType(0x00, 0x0000));
    insert_16bit_tlv(Smpp::Tlv::broadcast_rep_num, 0x0000);
    insert_tlv(BroadcastFrequencyInterval(0x00, 0x0000));
}

/// @brief Constructor that requires values for all the mandatory parameters.
/// @note The sm_length parameter is derived from the short_message parameter.
/// @param sequenceNumber The sequence number to use.
/// @param serviceType The service type to use.
/// @param sourceAddr The source address (ton, npi and address) to use.
/// @param messageId The message is.
/// @param priorityFlag The priority flag to use to use.
/// @param scheduleDeliveryTime The schedule delivery time to use.
/// @param validityPeriod The validity period to use.
/// @param replaceIfPresentFlag The replace if present flag to use.
/// @param dataCoding The data coding value to use.
/// @param smDefaultMsgId The SM default message Id to use.
/// @param broadcastAreaIdentifier The broadcast_area_identifier (should be valid).
/// @param broadcastContentType The broadcast_content_type.
/// @param broadcastRepNum The broadcast_rep_num.
/// @param broadcastFrequencyInterval The broadcast_frequency_interval.
Smpp::BroadcastSm::BroadcastSm(
        const SequenceNumber& sequenceNumber,
        const ServiceType& serviceType,
        const SmeAddress& sourceAddr,
        const MessageId& messageId,
        const PriorityFlag& priorityFlag,
        const Smpp::Time& scheduleDeliveryTime,
        const Smpp::Time& validityPeriod,
        const ReplaceIfPresentFlag& replaceIfPresentFlag,
        const DataCoding& dataCoding,
        const SmDefaultMsgId& smDefaultMsgId,
        const BroadcastAreaIdentifier& broadcastAreaIdentifier,
        const BroadcastContentType& broadcastContentType,
        const Smpp::Uint16& broadcastRepNum,
        const BroadcastFrequencyInterval& broadcastFrequencyInterval) :
    Request(CommandLength(MinLength), 
             CommandId(CommandId::BroadcastSm), 
             sequenceNumber),
    service_type_(serviceType),
    source_addr_(sourceAddr),
    message_id_(messageId),
    priority_flag_(priorityFlag),
    schedule_delivery_time_(scheduleDeliveryTime),
    validity_period_(validityPeriod),
    replace_if_present_flag_(replaceIfPresentFlag),
    data_coding_(dataCoding),
    sm_default_msg_id_(smDefaultMsgId),
    defaultBroadcastAreaIdentifier_(false)
{
    Header::update_length(service_type_.length() +
                           source_addr_.address().length() +
                           message_id_.length() +
                           schedule_delivery_time_.length() +
                           validity_period_.length());
    
    // insert the tlvs using their mutate operations.
    insert_tlv(broadcastAreaIdentifier);
    insert_tlv(broadcastContentType);
    insert_16bit_tlv(Smpp::Tlv::broadcast_rep_num, broadcastRepNum);
    insert_tlv(broadcastFrequencyInterval); 
}

/// @brief Construct from a buffer.
/// @param b The buffer (octet array).
Smpp::BroadcastSm::BroadcastSm(const Smpp::Uint8* b) :
    Request(CommandLength(MinLength), CommandId(CommandId::BroadcastSm), 1)
{
    decode(b);
}

Smpp::BroadcastSm::~BroadcastSm()
{
}

/// @brief Encode the message into an octet array.
/// @return Pointer to the encoded message.
/// @note The Buffer length is the command_length.
const Smpp::Uint8*
Smpp::BroadcastSm::encode()
{
    buff_.reset(Header::command_length());

    Request::encode(buff_); // insert the header first
    buff_ += service_type_;
    buff_ += source_addr_;
    buff_ += message_id_;
    buff_ += priority_flag_;
    buff_ += schedule_delivery_time_;
    buff_ += validity_period_;
    buff_ += replace_if_present_flag_;
    buff_ += data_coding_;
    buff_ += sm_default_msg_id_;

    Header::encode_tlvs(buff_);

    return buff_.get();
}

/// @brief Decode the message from an octet array.
/// @param buff The octet array to decode.
/// @note The octet array maybe the data read from a socket.
void
Smpp::BroadcastSm::decode(const Smpp::Uint8* buff)
{
    Request::decode(buff);

    Smpp::Uint32 len = Request::command_length();
    Smpp::Uint32 offset = 16;
    const char* err = "Bad length in broadcast_sm";
    if(len < offset)
        throw Error(err);
 
    const Smpp::Char* sptr = reinterpret_cast<const Smpp::Char*>(buff);

    service_type_ = sptr + offset;
    offset += service_type_.length() + 1;
    if(len < offset)
        throw Error(err);

    source_addr_.decode(buff+offset, len - offset);
    offset += source_addr_.address().length() + 3; // ton + npi + '\0'
    if(len < offset)
        throw Error(err);

    message_id_ = sptr+offset;
    offset += message_id_.length() + 1;
    if(len < offset)
        throw Error(err);

    priority_flag_ = buff[offset];
    ++offset;
    if(len < offset)
        throw Error(err);
    
    schedule_delivery_time_ = sptr + offset;
    if(len < offset)
        throw Error(err);

    offset += schedule_delivery_time_.length() + 1;
    if(len < offset)
        throw Error(err);
    
    validity_period_ = sptr + offset;
    if(len < offset)
        throw Error(err);

    offset += validity_period_.length() + 1;
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
    
    Header::decode_tlvs(buff + offset, len - offset);

    // check mandatory TLVs are present (order is not important)
    if(!find_tlv(Tlv::broadcast_area_identifier)     ||
        !find_tlv(Tlv::broadcast_content_type)       ||
        !find_tlv(Tlv::broadcast_rep_num)            ||
        !find_tlv(Tlv::broadcast_frequency_interval))
        throw Smpp::Error("Missing mandatory TLV parameter");
}

