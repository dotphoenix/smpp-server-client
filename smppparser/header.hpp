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
#ifndef __SMPP_HEADER_HPP__
#define __SMPP_HEADER_HPP__

/// @file header.hpp
/// @brief Definition of an SMPP header.
/// @note All SMPP PDUs are derived these types.

#include <list>
#include "aux_types.hpp"
#include "error.hpp"
#include "command_length.hpp"
#include "command_id.hpp"
#include "command_status.hpp"
#include "sequence_number.hpp"
#include "buffer.hpp"
#include "tlv.hpp"
#include <iterator>

/// @brief Smpp namespace, all classes, functions, etc. are defined in here.
///
/// To use any classes, functions, etc. of the API they need to be prefixed
/// with Smpp::<class> or else provide a using declaration
/// (\c using \c namespace \c Smpp;).
namespace Smpp {

    /// Short hand for a Tlv list.
    typedef std::list<const Tlv*> TlvList;

    /**
     * @brief Base class for all PDUs.
     */
    class Header {
        /** @brief Default constructor declared as private */
        Header();

        CommandLength command_length_;
        CommandId command_id_;
        CommandStatus command_status_;
        SequenceNumber sequence_number_;

        // Copying and assigning the list of Tlv pointers.
        struct CTlvList {
            /// @brief Functor to delete the tlv.
            struct Delete {
                void operator()(const Smpp::Tlv* tlv) { delete tlv; }
            };

            /// @brief Functor to copy the tlv.
            struct Copy {
                Tlv* operator()(const Tlv* tp) { return new Tlv(*tp); }
            };

            TlvList list_;

            CTlvList() {}
            ~CTlvList() throw() {
                std::for_each(list_.begin(), list_.end(), Delete());
            }
           
            /// @brief Copy constructor
            /// @param t List to copy
            CTlvList(const CTlvList& t) {
                std::transform(t.list_.begin(), t.list_.end(),
                        std::back_inserter(list_),
                        Copy()); 
            }

            CTlvList& operator=(const CTlvList& t) {
                if(this == &t) return *this;
                std::transform(t.list_.begin(), t.list_.end(),
                        std::back_inserter(list_),
                        Copy()); 
                return *this;
            }
        } tlvs_;
        
      protected:
        /** @brief Header constructor */
        Header(const CommandLength& commandLength, 
                const CommandId& commandID, 
                const CommandStatus& commandStatus, 
                const SequenceNumber& sequenceNumber);

        /** @brief Allows the command length to be set by a derived class.
         * @param p The new command length.  */
        void command_length(const Smpp::Uint32& p) { command_length_ = p; }

        void update_length(const int& p) { command_length_ += p; }
        
        /// @brief A helper function to access the commmand status.
        /// @return Command status.
        const CommandStatus& get_command_status(void) const {
            return command_status_;
        }

        /// @brief A helper function to set the command status.
        /// @param p The new command status.
        void set_command_status(const CommandStatus& p) {
            command_status_ = p;
        }

        /// @brief Writes the header to the Buffer
        /// @param b The Smpp::Buffer
        void encode(Smpp::Buffer& b) const {
            b += Smpp::hton32(command_length_);
            b += Smpp::hton32(command_id_);
            b += Smpp::hton32(command_status_);
            b += Smpp::hton32(sequence_number_);
        }

        /// @brief Reads from the array b in to the header parameters
        /// @param b The array to read from
        void decode(const Smpp::Uint8* buff) {
            const Smpp::Uint32* p = reinterpret_cast<const Smpp::Uint32*>(buff);
            Smpp::Uint32 t;
            memcpy(&t, p, 4); command_length_ = Smpp::ntoh32(t);
            memcpy(&t, p+1, 4); command_id_ = Smpp::ntoh32(t);
            memcpy(&t, p+2, 4); command_status_ = Smpp::ntoh32(t);
            memcpy(&t, p+3, 4); sequence_number_ = Smpp::ntoh32(t);
        }
            
        /// @brief Encode the contents of the tlvs_.list_ into the Buffer.
        void encode_tlvs(Smpp::Buffer& b)
        {
            Tlv::Encoder t(b);
            std::for_each(tlvs_.list_.begin(), tlvs_.list_.end(), t);
        }

        /// @brief Decode octets into tlv objects.
        void decode_tlvs(const Smpp::Uint8* b, Smpp::Uint32 l);

	/// @brief Insert a TLV.
	/// @param tlv The TLV to insert.
        void insert_tlv(const Tlv* tlv) {
            tlvs_.list_.push_back(tlv);
            update_length(tlv->length() + 4);
        }
        
        /// @brief Insert a TLV after the last occurance of another TLV.
        void insert_after_tlv(const Tlv* tlv, Smpp::Uint16 tag);

        /// @brief Insert a TLV before the first occurance of another TLV.
        void insert_before_tlv(const Tlv* tlv, Smpp::Uint16 tag);

      public:
        virtual ~Header();
        
        /* Accessing */

        /// @brief Accesses the subclasses command length.
        /// @return Command length.
        Smpp::Uint32 command_length() const { return command_length_; }

        /// @brief Accesses the subclasses command id
        /// @return Command Id
        Smpp::Uint32 command_id() const { return command_id_; }

        /// @brief Accesses the subclasses sequence number
        /// @return Sequence number
        Smpp::Uint32 sequence_number() const { return sequence_number_; }
       
        /* Mutating */

        /// @brief Mutates the subclasses sequence number
        /// @param p The new sequence number
        /// @param allow_0 Optional parameter that when set to true allows a 
        ///                sequence number 0 to be set, the default is false. */
        void sequence_number(const Smpp::Uint32& p, bool allow_0 = true) {
            sequence_number_ = SequenceNumber(p, allow_0);
        }
        
        /* Access to command_status is dependant on message type.
         * That is a request or a response. */
        virtual Smpp::Uint32 command_status() const = 0;
        virtual void command_status(const Smpp::Uint32& p) = 0;

        /// @brief Insert a TLV.
        /// @param tlv The TLV to insert.
        void insert_tlv(const Tlv& tlv) {
            insert_tlv(new Tlv(tlv));
        }

        /// @brief Insert a TLV containing an 8 bit integer.
        /// @param t The TLV tag.
        /// @param v The integer to insert.
        void insert_8bit_tlv(Uint16 t, const Smpp::Uint8 v)
        {
            insert_tlv(new Tlv(t, 1, &v));
        }
        
        /// @brief Insert a TLV containing a 16 bit integer.
        /// @param t The TLV tag.
        /// @param v The integer to insert in host byte order.
        void insert_16bit_tlv(Uint16 t, Smpp::Uint16 v)
        {
            Smpp::Uint16 i = Smpp::hton16(v);
            insert_tlv(new Tlv(t, 2, reinterpret_cast<Uint8*>(&i)));
        }
        
        /// @brief Insert a TLV containing a 32 bit integer.
        /// @param t The TLV tag.
        /// @param v The integer to insert in host byte order.
        void insert_32bit_tlv(Uint16 t, Smpp::Uint32 v)
        {
            Smpp::Uint32 i = Smpp::hton32(v);
            insert_tlv(new Tlv(t, 4, reinterpret_cast<Uint8*>(&i)));
        }
        
        /// @brief Insert a TLV containing a C-String (NULL terminated)
        /// @param t The TLV tag.
        /// @param v The TLV null terminated c-string.
        void insert_string_tlv(Uint16 t, const Smpp::String& v)
        {
            insert_tlv(new Tlv(t,
                                v.length()+1,
                                reinterpret_cast<const Smpp::Uint8*>(v.data())));
        }
        
        /// @brief Insert a TLV containing an array of octets.
        /// @param t The TLV tag.
        /// @param l The TLV octet length.
        /// @param v The TLV octet string.
        void insert_array_tlv(Uint16 t, Smpp::Uint16 l, const Smpp::Uint8* v)
        {
            insert_tlv(new Tlv(t, l, v));
        }

        /// @brief Access the complete Tlv list.
        /// @return The Tlv list.
        const TlvList& tlv_list() const {
            return tlvs_.list_;
        }

        /// @brief Functor that generates a Tlv list with the same tag.
        class ListFinder {
            TlvList& l_;
            Smpp::Uint16 tag_;
          public:
            ListFinder(TlvList& l, Smpp::Uint16 tag) : l_(l), tag_(tag) {}
            void operator()(const Tlv* tlv) {
                if(tlv->tag() == tag_)
                    l_.push_back(tlv); 
            }

            const TlvList& get_list() const { return l_; }
        };
        
        /// @brief Access all TLVs with the same tag.
        /// @param tag The Tlv tag to access.
        /// @return List of Tlvs.
        /// @note The list will have 0 (zero) size if the Tlv is not found.
        const TlvList find_tlv_list(Uint16 tag) const {
            TlvList l;
            ListFinder t(l, tag);
            std::for_each(tlvs_.list_.begin(), tlvs_.list_.end(), t);
            return t.get_list();
        }
        
        /// @brief Access a Tlv using its tag.
        /// @param tag The Tlv tag to access.
        /// @return Pointer to the the Tlv.
        /// @retval 0 If the Tlv is not found.
        const Tlv* find_tlv(Uint16 tag) const {
            TlvList::const_iterator i =
                std::find_if(tlvs_.list_.begin(), tlvs_.list_.end(), Tlv::CompareTag(tag));
            if(i == tlvs_.list_.end())
                return 0;

            return *i;
        }
        
        /// @brief Remove all instances of a Tlv.
        /// @return The tag denoting the tlv(s) to be removed.
        void remove_tlv(Uint16 t)
        {
            for(TlvList::iterator i =
                 std::find_if(tlvs_.list_.begin(), tlvs_.list_.end(),
                              Tlv::CompareTag(t));
                 i != tlvs_.list_.end();
                 i = std::find_if(i, tlvs_.list_.end(), Tlv::CompareTag(t)))
            {
                TlvList::iterator j = i;
                ++j;
                const Tlv* t = *i;
                tlvs_.list_.erase(i);
                update_length(0 - (t->length() + 4));
                delete t;
                i = j;
            }
        }
    };

    /**
     * @class Request
     * @brief Base class for all request SMPP PDUs, derived from Header
     */
    class Request : public Header {
        /** @brief Declared as private to block mutating by a request.  */
        void command_status(const Smpp::Uint32& p) {}
      protected:
        /** @brief Constructor */
        Request(const CommandLength& commandLength, 
                 const CommandId& commandID, 
                 const SequenceNumber& sequenceNumber);

        /**
         * @brief Writes the header to the Buffer
         * @param b The Smpp::Buffer
         */
        void encode(Smpp::Buffer& b) const { Header::encode(b); }

        /**
         * @brief Reads from the array b in to the header parameters
         * @param b The array to read from
         */
        void decode(const Smpp::Uint8* b) { Header::decode(b); }
        
      public:

        /** @brief Access to the command status.
         * @return Command status, should always be 0 for a request.  */
        Smpp::Uint32 command_status() const { return Header::get_command_status(); }
    };

    /** @class Response
     * @brief Base class for all response SMPP PDUs, derived from Header.  */
    class Response : public Header {
      protected:
        /** @brief Constructor for a response PDU.  */
        Response(const CommandLength& commandLength, 
                  const CommandId& commandID,
                  const CommandStatus& commandStatus,
                  const SequenceNumber& sequenceNumber);

        /**
         * @brief Writes the header to the Buffer
         * @param b The Smpp::Buffer
         */
        void encode(Smpp::Buffer& b) const { Header::encode(b); }

        /**
         * @brief Reads from the array b in to the header parameters
         * @param b The array to read from
         */
        void decode(const Smpp::Uint8* b) { Header::decode(b); }
        
      public:
        /** @brief Mutates the command status.  */
        void command_status(const Smpp::Uint32& p) {
            Header::set_command_status(p);
        }
       
        /** @brief Accesses the command status.
         * @return The command status.  */
        Smpp::Uint32 command_status() const { 
            return Header::get_command_status();
        }
    };

} // namespace Smpp

/**
 * @mainpage
 *
 * See the README.html in the source directory for a general description
 *
 */
#endif

