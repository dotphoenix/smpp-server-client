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
#ifndef __SMPP_BROADCAST_SM_HPP__
#define __SMPP_BROADCAST_SM_HPP__

/// @file broadcast_sm.hpp
/// @brief An SMPP broadcast_sm PDU.

#include "aux_types.hpp"
#include "header.hpp"

namespace Smpp {

    /// @class BroadcastSm
    /// @brief Definition of an SMPP broadcast_sm, derived from a Request.
    class BroadcastSm : public Request {
        ServiceType service_type_;
        SmeAddress source_addr_;
        MessageId message_id_;
        PriorityFlag priority_flag_;
        Time schedule_delivery_time_;
        Time validity_period_;
        ReplaceIfPresentFlag replace_if_present_flag_;
        DataCoding data_coding_;
        SmDefaultMsgId sm_default_msg_id_;

        // true if the BroadcastAreaIdentifier is set from the default
        // constructor, in which case it should be replaced.
        bool defaultBroadcastAreaIdentifier_;

        Smpp::Buffer buff_;

      public:
        /** broadcast_sm minimum length in octets */
        const static int MinLength = 27; // not including mandatory TLVs
        
        /** @brief Default constructor. */
        BroadcastSm();
        
        /** @brief Constructor requiring all mandatory parameters. */
        BroadcastSm(const SequenceNumber& sequenceNumber,
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
                     const BroadcastFrequencyInterval& broadcastFrequencyInterval);
       
        /// @brief Construct from a buffer.
        BroadcastSm(const Smpp::Uint8* b);

        /// @brief Destructor - does nothing.
        ~BroadcastSm();
        
        //
        // Mutating
        //
        
        /// @brief Set the service type.
        /// @param p The new service type.
        void service_type(const ServiceType& p) {
            int diff = p.length() - service_type_.length();
            service_type_ = p;
            Header::update_length(diff);
        }
        
        /// @brief Set the service type.
        /// @param p The new service type.
        void service_type(const Smpp::Char* p) {
            int diff = strlen(p) - service_type_.length();
            service_type_ = p;
            Header::update_length(diff);
        }
        
        /// @brief Set the source address using an SmeAddress object.
        /// @param p The new source address.
        void source_addr(const SmeAddress& p) {
            int diff = p.address().length() - source_addr_.address().length();
            source_addr_ = p;
            Header::update_length(diff);
        }

        /// @brief Set the source address using just the address.
        /// @param p The source address characters.
        void source_addr(const Address& p) {
            int diff = p.length() - source_addr_.address().length();
            source_addr_ = SmeAddress(Smpp::Ton(Smpp::Ton::Unknown),
                                       Smpp::Npi(Smpp::Npi::Unknown),
                                       p);
            Header::update_length(diff);
        }
        
        /// @brief Sets the message id.
        /// @param p The message id.
        void message_id(const MessageId& p) {
            int diff = p.length() - message_id_.length();
            message_id_ = p;
            Header::update_length(diff);
        }
        
        /// @brief Sets the message id.
        /// @param p The message id.
        void message_id(const Smpp::Char* p) {
            int diff = strlen(p) - message_id_.length();
            message_id_ = p;
            Header::update_length(diff);
        }
        
        /// @brief Set the priority flag.
        /// @param p The new priority flag.
        void priority_flag(const int& p) { priority_flag_ = p; }
        
        /// @brief Set the schedule delivery time.
        /// @param p The new schedule delivery time.
        void schedule_delivery_time(const Time& p) {
            int diff = p.length() - schedule_delivery_time_.length();
            schedule_delivery_time_ = p;
            Header::update_length(diff);
        }
        
        /// @brief Set the schedule delivery time.
        /// @param p The new schedule delivery time.
        void schedule_delivery_time(const Smpp::Char* p) {
            int diff = strlen(p) - schedule_delivery_time_.length();
            schedule_delivery_time_ = p;
            Header::update_length(diff);
        }
        
        /// @brief Set the validity period.
        /// @param p The new validity period.
        void validity_period(const Time& p) {
            int diff = p.length() - validity_period_.length();
            validity_period_ = p;
            Header::update_length(diff);
        }

        /// @brief Set the validity period.
        /// @param p The new validity period.
        void validity_period(const Smpp::Char* p) {
            int diff = strlen(p) - validity_period_.length();
            validity_period_ = p;
            Header::update_length(diff);
        }

        /// @brief Set the replace if present flag.
        /// @param p The new replace if present flag.
        void replace_if_present_flag(const ReplaceIfPresentFlag& p) {
            replace_if_present_flag_ = p;
        }
        
        /// @brief Set the data coding.
        /// @param p The new data coding.
        void data_coding(const DataCoding& p) { data_coding_ = p; }
        
        /// @brief Set the SM default message Id.
        /// @param p The new SM default message Id.
        void sm_default_msg_id(const SmDefaultMsgId& p) {
            sm_default_msg_id_ = p;
        }

        /// @brief Set the broadcast area identifier.
        /// @param p The new broadcast area identifier.
        void broadcast_area_identifier(const BroadcastAreaIdentifier& p) {
            if(defaultBroadcastAreaIdentifier_) {
                remove_tlv(Tlv::broadcast_area_identifier);
                defaultBroadcastAreaIdentifier_ = false;
            }
            this->insert_before_tlv(new Tlv(p), Tlv::broadcast_content_type);
        }
       
        /// @brief Set the broadcast content type.
        /// @param p The new broadcast content type.
        void broadcast_content_type(const BroadcastContentType& p) {
            remove_tlv(Tlv::broadcast_content_type);
            this->insert_after_tlv(new Tlv(p), Tlv::broadcast_area_identifier);
        }
       
        /// @brief Set the broadcast repeat number.
        /// @param p The new broadcast repeat number.
        void broadcast_rep_num(const Smpp::Uint16& p) {
            remove_tlv(Tlv::broadcast_rep_num);
            Smpp::Uint16 i = Smpp::hton16(p);
            const Tlv* tlv = new Tlv(
                    Tlv::broadcast_rep_num, 2, reinterpret_cast<Uint8*>(&i));
            this->insert_after_tlv(tlv, Tlv::broadcast_content_type);
        }

        /// @brief Set the broadcast frequency interval.
        /// @param p The new broadcast frequency interval.
        void broadcast_frequency_interval(const BroadcastFrequencyInterval& p)
        {
            remove_tlv(Tlv::broadcast_frequency_interval);
            this->insert_after_tlv(new Tlv(p), Tlv::broadcast_rep_num);
        }
       
        // 
        // Accessing
        // 
        
        /// @brief Access the service type.
        /// @return The service type.
        const ServiceType& service_type() const { return service_type_; }
        
        /// @brief Access the source address.
        /// @return The source address.
        const SmeAddress& source_addr() const { return source_addr_; }
        
        /// @brief Access the destination address.
        /// @return The destination address.
        const MessageId& message_id() const { return message_id_; }
        
        /// @brief Access the priority flag.
        /// @return The priority flag.
        const PriorityFlag& priority_flag() const { return priority_flag_; }
        
        /// @brief Access the schedule delivery time.
        /// @return The schedule delivery time.
        const Time& schedule_delivery_time() const {
            return schedule_delivery_time_;
        }
        
        /// @brief Access the validity period.
        /// @return The validity period.
        const Time& validity_period() const { return validity_period_; }
        
        /// @brief Access the replace if present flag.
        /// @return The replace if present flag.
        const ReplaceIfPresentFlag& replace_if_present_flag() const {
            return replace_if_present_flag_;
        }
        
        /// @brief Access the data coding.
        /// @return The data coding.
        const DataCoding& data_coding() const { return data_coding_; }
        
        /// @brief Access the SM default message id.
        /// @return The SM default message id.
        const SmDefaultMsgId& sm_default_msg_id() const {
            return sm_default_msg_id_;
        }
       
        /// @brief Access the broadcast area identifiers.
        /// @return List of broadcast area identifiers pointers.
        const TlvList broadcast_area_identifier() const {
            return find_tlv_list(Tlv::broadcast_area_identifier);
        }

        /// @brief Access the broadcast content type.
        /// @return The broadcast content type.
        const BroadcastContentType& broadcast_content_type() const {
            const Tlv* t = find_tlv(Tlv::broadcast_content_type);
            if(!t)
                throw Smpp::Error("Missing broadcast_content_type");
            return *static_cast<const BroadcastContentType*>(t);
        }
       
        /// @brief Access the broadcast repeat number.
        /// @return The broadcast repeat number.
        Smpp::Uint16 broadcast_rep_num() const {
            const Tlv* t = find_tlv(Tlv::broadcast_rep_num);
            if(!t)
                throw Smpp::Error("Missing broadcast_rep_num");
            if(t->length() != 2)
                throw Smpp::Error("Invalid broadcast_rep_num length");
            Smpp::Uint16 i;
            std::memcpy(&i, t->value(), 2);
            return Smpp::ntoh16(i);
        }

        /// @brief Access the broadcast frequency number.
        /// @return The broadcast frequency number.
        const BroadcastFrequencyInterval& broadcast_frequency_interval() const {
            const Tlv* t = find_tlv(Tlv::broadcast_frequency_interval);
            if(!t)
                throw Smpp::Error("Missing broadcast_frequency_interval");
            return *static_cast<const BroadcastFrequencyInterval*>(t);
        }

        /// @brief Serialize the PDU.
        /// @note The length is taken from the command_length().
        /// @return The PDU as an octet array, suitable for outputting.
        const Smpp::Uint8* encode();

        /// @brief Populates the PDU from an array of octets.
        /// @param buff containing PDU as octet array.
        void decode(const Smpp::Uint8* buff);
    };

} // namespace Smpp

#endif

