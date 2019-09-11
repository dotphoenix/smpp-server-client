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
#ifndef __SMPP_SUBMIT_SM_HPP__
#define __SMPP_SUBMIT_SM_HPP__

/// @file submit_sm.hpp
/// @brief An SMPP submit_sm PDU.

#include "aux_types.hpp"
#include "header.hpp"

namespace Smpp {

    /// @class SubmitSm
    /// @brief Definition of an SMPP submit_sm, derived from a Request.
    class SubmitSm : public Request {
        ServiceType service_type_;
        SmeAddress source_addr_;
        SmeAddress destination_addr_;
        EsmClass esm_class_;
        ProtocolId protocol_id_;
        PriorityFlag priority_flag_;
        Time schedule_delivery_time_;
        Time validity_period_;
        RegisteredDelivery registered_delivery_;
        ReplaceIfPresentFlag replace_if_present_flag_;
        DataCoding data_coding_;
        SmDefaultMsgId sm_default_msg_id_;
        ShortMessage short_message_;

        Smpp::Buffer buff_;

      public:
        /** submit_sm minimum length in octets */
        const static int MinLength = 33;
        
        /** @brief Default constructor. */
        SubmitSm();
        
        /** @brief Constructor requiring all mandatory parameters. */
        SubmitSm(
                const SequenceNumber& sequenceNumber,
                const ServiceType& serviceType,
                const SmeAddress& sourceAddr,
                const SmeAddress& destinationAddr,
                const EsmClass& esmClass,
                const ProtocolId& protocolId,
                const PriorityFlag& priorityFlag,
                const Time& scheduleDeliveryTime,
                const Time& validityPeriod,
                const RegisteredDelivery& registeredDelivery,
                const ReplaceIfPresentFlag& replaceIfPresentFlag,
                const DataCoding& dataCoding,
                const SmDefaultMsgId& smDefaultMsgId,
                const ShortMessage& shortMessage);
       
        /// @brief Construct from a buffer.
        SubmitSm(const Smpp::Uint8* b);

        /// @brief Destructor - does nothing.
        ~SubmitSm();
        
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
        
        /// @brief Set the destination address using an SmeAddress object.
        /// @param p The new destination address.
        void destination_addr(const SmeAddress& p) {
            int diff =
                p.address().length() - destination_addr_.address().length();
            destination_addr_ = p;
            Header::update_length(diff);
        }
        
        /// @brief Set the destination address using just the address.
        /// @param p The destination address characters.
        void destination_addr(const Address& p) {
            int diff = p.length() - destination_addr_.address().length();
            destination_addr_ = SmeAddress(Smpp::Ton(Smpp::Ton::Unknown),
                                            Smpp::Npi(Smpp::Npi::Unknown),
                                            p);
            Header::update_length(diff);
        }

        /// @brief Set the esm class.
        /// @param p The new esm class.
        void esm_class(const int& p) { esm_class_ = p; }

        /// @brief Set the protocol Id.
        /// @param p The new protocol id.
        void protocol_id(const int& p) { protocol_id_ = p; }

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

        /// @brief Set the registered delivery.
        /// @param p The new registered delivery.
        void registered_delivery(const RegisteredDelivery& p) {
            registered_delivery_ = p;
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
        
        /// @brief Set the short message and sm length.
        /// @param p The new short message.
        /// @param l The new short message length.
        void short_message(const Smpp::Uint8* p, Smpp::Uint8 l) {
            int diff = l - short_message_.length();
            short_message_ = ShortMessage(p, l);
            Header::update_length(diff);
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
        const SmeAddress& destination_addr() const { return destination_addr_; }
        
        /// @brief Access the esm class.
        /// @return The esm class.
        const EsmClass& esm_class() const { return esm_class_; }
        
        /// @brief Access the protocol id.
        /// @return The protocol id.
        const ProtocolId& protocol_id() const { return protocol_id_; }
        
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
        
        /// @brief Access the registered delivery.
        /// @return The registered delivery.
        const RegisteredDelivery& registered_delivery() const {
            return registered_delivery_;
        }
        
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
        
        /// @brief Access the short message length.
        /// @return The short message length.
        const Smpp::String::size_type sm_length() const {
            return short_message_.length();
        }
        
        /// @brief Access the short message.
        /// @return The short message.
        const ShortMessage& short_message() const { return short_message_; }
      
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

