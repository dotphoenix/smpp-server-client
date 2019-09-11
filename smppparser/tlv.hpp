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
#ifndef __SMPP_TLV_HPP__
#define __SMPP_TLV_HPP__

/// @file tlv.hpp
/// @brief class defining a tlv.

#include "aux_types.hpp"

namespace Smpp {

    class Tlv {
      public:
        enum {
            dest_addr_subunit            = 0x0005,
            dest_network_type            = 0x0006,
            dest_bearer_type             = 0x0007,
            dest_telematics_id           = 0x0008,
            source_addr_subunit          = 0x000D,
            source_network_type          = 0x000E,
            source_bearer_type           = 0x000F,
            source_telematics_id         = 0x0010,
            qos_time_to_live             = 0x0017,
            payload_type                 = 0x0019,
            additional_status_info_text  = 0x001D,
            receipted_message_id         = 0x001E,
            ms_msg_wait_facilities       = 0x0030,
            privacy_indicator            = 0x0201,
            source_subaddress            = 0x0202,
            dest_subaddress              = 0x0203,
            user_message_reference       = 0x0204,
            user_response_code           = 0x0205,
            source_port                  = 0x020A,
            dest_port                    = 0x020B,
            sar_msg_ref_num              = 0x020C,
            language_indicator           = 0x020D,
            sar_total_segments           = 0x020E,
            sar_segment_seqnum           = 0x020F,
            sc_interface_version         = 0x0210,
            callback_num_pres_ind        = 0x0302,
            callback_num_atag            = 0x0303,
            number_of_messages           = 0x0304,
            callback_num                 = 0x0381,
            dpf_result                   = 0x0420,
            set_dpf                      = 0x0421,
            ms_availability_status       = 0x0422,
            network_error_code           = 0x0423,
            message_payload              = 0x0424,
            delivery_failure_reason      = 0x0425,
            more_messages_to_send        = 0x0426,
            message_state                = 0x0427,
            congestion_state             = 0x0428,
            ussd_service_op              = 0x0501,
            broadcast_channel_indicator  = 0x0600,
            broadcast_content_type       = 0x0601,
            broadcast_content_type_info  = 0x0602,
            broadcast_message_class      = 0x0603,
            broadcast_rep_num            = 0x0604,
            broadcast_frequency_interval = 0x0605,
            broadcast_area_identifier    = 0x0606,
            broadcast_Error_status       = 0x0607,
            broadcast_area_success       = 0x0608,
            broadcast_end_time           = 0x0609,
            broadcast_service_group      = 0x060A,
            billing_identification       = 0x060B,
            source_network_id            = 0x060D,
            dest_network_id              = 0x060E,
            source_node_id               = 0x060F,
            dest_node_id                 = 0x0610,
            dest_addr_np_resolution      = 0x0611,
            dest_addr_np_information     = 0x0612,
            dest_addr_np_country         = 0x0613,
            display_time                 = 0x1201,
            sms_signal                   = 0x1203,
            ms_validity                  = 0x1204,
            alert_on_message_delivery    = 0x130C,
            its_reply_type               = 0x1380,
            its_session_info             = 0x1383
        };

      private:
        Smpp::Uint16 tag_;
        Smpp::Uint16 length_;
        const Smpp::Uint8* value_;

        /// @brief Helper that allocates and copies the value
        /// @param l The length of the value in octets.
        /// @param v The value to be copied.
        /// @return The copied value.
        Smpp::Uint8* build_value(const Smpp::Uint16& l, const Smpp::Uint8* v) {
            Smpp::Uint8* t(new Smpp::Uint8[l]);
            std::copy(v, v+l, t);
            return t;
        }

      public:
        /// @brief Function object used to encode a Tlv into an Buffer.
        class Encoder {
            Smpp::Buffer& b_;
          public:
            Encoder(Smpp::Buffer& b) : b_(b) {}
            void operator()(const Tlv*& tlv) {
                b_ += Smpp::hton16(tlv->tag());
                b_ += Smpp::hton16(tlv->length());
                b_.add_octet_array(tlv->length(), tlv->value());
            }
        };
      
        /// @brief Default constructor acts as a null TLV.
        Tlv() : tag_(0), length_(0), value_(0) {}

        /// @brief Copy constructor.
        /// @param tlv The TLV to copy from.
        Tlv(const Tlv& tlv) :
            tag_(tlv.tag_),
            length_(tlv.length_),
            value_(build_value(tlv.length_, tlv.value_)) {
        }
        
        /// @brief Construct using tag length and value
        /// @param t The tag.
        /// @param l The length of the value in octets.
        /// @param v The value.
        Tlv(const Smpp::Uint16& t, const Smpp::Uint16& l, const Smpp::Uint8* v) :
            tag_(t), length_(l), value_(build_value(l, v)) {
        }
      
        /// @brief Deallocate the value.
        virtual ~Tlv() { delete [] value_; }
      
        Tlv& operator=(const Tlv& rhs) {
            if(this == &rhs)
                return *this;

            tag_ = rhs.tag_;
            length_ = rhs.length_;
            value_ = build_value(rhs.length_, rhs.value_);
			return *this;
        }
        
        /// @brief tag comparsion.
        bool operator==(const Smpp::Uint16& tag) const { return tag == tag_; }

        /// @brief Test validity of the tlv.
        operator bool() const { return tag_ != 0; }

        /// @return The tlv tag.
        Smpp::Uint16 tag() const { return tag_; }
        
        /// @return The tlv length.
        Smpp::Uint16 length() const { return length_; }

        /// @return The tlv data as its raw octet array.
        const Smpp::Uint8* value() const { return value_; }

        /// @brief Functor used to compare a tag to a Tlv pointer.
        class CompareTag {
            const Smpp::Uint16& tag_;
          public:
            CompareTag(const Smpp::Uint16& tag) : tag_(tag) {}
            bool operator()(const Tlv* p) const {
                return p->tag() == tag_;
            }
        };

      protected:
        /// @brief Construct using tag length and value called from a
        /// derived class.
        /// @note The value is allocated by the derived class.
        /// @param t The tag.
        /// @param l The length of the value in octets.
        /// @param t The value.
        /// @param x An extra field to differenciate it from the public
        /// constructor.
        Tlv(const Smpp::Uint16& t,
                const Smpp::Uint16& l, const Smpp::Uint8* v, int x) :
            tag_(t), length_(l), value_(v) {
        }
    };

    //
    // Specific TLVs derived from Tlv
    //
    
    /// @brief Builds a broadcast_area_identifier TLV.
    class BroadcastAreaIdentifier : public Tlv {
        Smpp::Uint8* allocate_value(const Smpp::Uint8& format,
                                    const Smpp::Uint8* details,
                                    const Smpp::Uint16& length) {
            Smpp::Uint8* v = new Smpp::Uint8[ length+1];
            v[0] = format;
            if(length)
                std::copy(details, details+length, v+1);
            return v;
        }
        BroadcastAreaIdentifier();
      public:
        /// @brief Construct using format, details and details length.
        /// @param format The format.
        /// @param details The details array.
        /// @param length The details length in octets.
        BroadcastAreaIdentifier(const Smpp::Uint8& format,
                                const Smpp::Uint8* details,
                                const Smpp::Uint16& length) :
            Tlv(Tlv::broadcast_area_identifier,
                 length+1,
                 allocate_value(format, details, length),
                 0) {}
    };
    
    /// @brief Builds a broadcast_content_type TLV.
    class BroadcastContentType : public Tlv {
        Smpp::Uint8* allocate_value(const Smpp::Uint8& typeTag, const Smpp::Uint16& type) {
            Smpp::Uint8* v = new Smpp::Uint8[3];
            v[0] = typeTag;
            Smpp::Uint16 t = Smpp::hton16(type);
            Smpp::Uint8* tptr = reinterpret_cast<Uint8*>(&t);
            std::memcpy(v+1, tptr, 2);
            return v;
        }
        BroadcastContentType();
      public:
        /// @brief Construct using type tag and type.
        /// @param typeTag The type tag.
        /// @param type The type.
        BroadcastContentType(const Smpp::Uint8& typeTag, const Smpp::Uint16& type) :
            Tlv(Tlv::broadcast_content_type,
                 3,
                 allocate_value(typeTag, type),
                 0) {}
    };
   
    /// @brief Builds a broadcast_frequency_interval TLV.
    class BroadcastFrequencyInterval : public Tlv {
        Smpp::Uint8* allocate_value(const Smpp::Uint8& type, const Smpp::Uint16& units) {
            Smpp::Uint8* v = new Smpp::Uint8[3];
            v[0] = type;
            Smpp::Uint16 t = Smpp::hton16(units);
            Smpp::Uint8* tptr = reinterpret_cast<Uint8*>(&t);
            std::memcpy(v+1, tptr, 2);
            return v;
        }
        BroadcastFrequencyInterval();
      public:
        /// @brief Construct using unit type and units.
        /// @param type The units types.
        /// @param units The number of units.
        BroadcastFrequencyInterval(const Smpp::Uint8& type, const Smpp::Uint16& units) :
            Tlv(Tlv::broadcast_frequency_interval,
                 3,
                 allocate_value(type, units),
                 0) {}
    };
  
    /// @brief Builds a broadcast_area_success TLV.
    class BroadcastAreaSuccess : public Tlv {
        Smpp::Uint8* allocate_value(const Smpp::Uint8& sRate) {
            Smpp::Uint8* v = new Smpp::Uint8[1];
            v[0] = sRate;
            return v;
        }
        BroadcastAreaSuccess();
      public:
        /// @brief Construct using the broadcast successs rate.
        /// @param sRate The success rate.
        BroadcastAreaSuccess(const Smpp::Uint8& sRate) :
            Tlv(Tlv::broadcast_area_success,
                 1,
                 allocate_value(sRate),
                 0) {}
    };

} // namespace Smpp

#endif
