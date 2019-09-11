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
#ifndef __SMPP_AUX_TYPES_HPP__
#define __SMPP_AUX_TYPES_HPP__

/// @file aux_types.hpp
/// @brief Defines a number of SMPP specific types and auxiliary functions.

#include <cstdio>
#include <string>
#include <algorithm>
#include <vector>
#include <list>
#include <iomanip>
#include <string.h>
#include "error.hpp"

namespace Smpp {
    typedef char Char;
    typedef unsigned char Uint8;
    typedef unsigned short int Uint16;
    typedef unsigned int Uint32;
    typedef std::basic_string<Char> String;

    inline Uint32 ntoh32(const Uint32& n);
    inline Uint32 hton32(const Uint32& n);
    inline Uint16 ntoh16(const Uint16& n);
    inline Uint16 hton16(const Uint16& n);

    inline const String& valid_length(const String&, size_t, const char* err);
    inline const Char* valid_length(const Char*, size_t, const char* err);
   
    void chex_dump(const Uint8* buff, Uint32 len, FILE* file);
    void hex_dump(const Uint8* buff, Uint32 len, std::ostream& os);

    /// @class SystemId
    /// @brief Provides system_id semantics.
    class SystemId {
        std::basic_string<Char> v_;
      public:
        enum { MaxLength = 16 };
        SystemId() {}
        explicit SystemId(const Char* p) :
            v_(valid_length(p, MaxLength, "Invalid system_id length")) {
        }
        explicit SystemId(const String& p) :
            v_(valid_length(p, MaxLength, "Invalid system_id length")) {
        }
        SystemId& operator=(const Char* p) {
            v_ = valid_length(p, MaxLength, "Invalid system_id length");
            return *this;
        }
        operator String() const { return v_; }
        size_t length() const { return v_.length(); }
    };
        
    /// @class Password
    /// @brief Provides password semantics.
    class Password {
        std::basic_string<Char> v_;
      public:
        enum { MaxLength = 19 };
        Password() {}
        explicit Password(const Char* p) :
            v_(valid_length(p, MaxLength, "Invalid password length")) {
        }
        explicit Password(const String& p) :
            v_(valid_length(p, MaxLength, "Invalid password length")) {
        }
        Password& operator=(const Char* p) {
            v_ = valid_length(p, MaxLength, "Invalid password length");
            return *this;
        }
        operator String() const { return v_; }
        size_t length() const { return v_.length(); }
    };
        
    /// @class SystemType
    /// @brief Provides system_type semantics.
    class SystemType {
        std::basic_string<Char> v_;
      public:
        enum { MaxLength = 13 };
        SystemType() {}
        explicit SystemType(const Char* p) :
            v_(valid_length(p, MaxLength, "Invalid system_type length")) {
        }
        explicit SystemType(const String& p) :
            v_(valid_length(p, MaxLength, "Invalid system_type length")) {
        }
        SystemType& operator=(const Char* p) {
            v_ = valid_length(p, MaxLength, "Invalid system_type length");
            return *this;
        }
        operator String() const { return v_; }
        size_t length() const { return v_.length(); }
    };
        
    /// @class InterfaceVersion
    /// @brief Interface version
    class InterfaceVersion {
        Uint8 v_;
      public:
        /// Valid interface versions
        enum { 
            V33       = 0x33, ///< Version 3.3
            V34       = 0x34, ///< Version 3.4
            V50       = 0x50  ///< Version 5.0
        };

        InterfaceVersion() : v_(V34) {}
        explicit InterfaceVersion(const Uint8& p) : v_(check(p)) {}
        explicit InterfaceVersion(int p) : v_(check(p)) {}
        InterfaceVersion& operator=(const Uint8& p) {
            v_ = p; return *this;
        }
        InterfaceVersion& operator=(const int& p) {
            v_ = check(p); return *this;
        }
        operator Uint8() const { return v_; }
      private:
        Uint8 check(const int& p) {
            switch(p) {
                case V33: case V34: case V50:
                return p;
            default:
                if(p < V33)
                    return p;
            }
            throw Error("Invalid interface_version");
        }
    };

    /// @class AddressRange
    /// @brief Provides address_range semantics.
    class AddressRange {
        std::basic_string<Char> v_;
      public:
        enum { MaxLength = 41 };
        AddressRange() {}
        explicit AddressRange(const Char* p) :
            v_(valid_length(p, MaxLength, "Invalid address_range length")) {
        }
        explicit AddressRange(const String& p) :
            v_(valid_length(p, MaxLength, "Invalid address_range length")) {
        }
        AddressRange& operator=(const Char* p) {
            v_ = valid_length(p, MaxLength, "Invalid address_range length");
            return *this;
        }
        operator String() const { return v_; }
        size_t length() const { return v_.length(); }
    };
        
    /// @class ServiceType
    /// @brief Provides service_type semantics.
    class ServiceType {
        std::basic_string<Char> v_;
      public:
        enum { MaxLength = 6 };
        ServiceType() {}
        explicit ServiceType(const Char* p) :
            v_(valid_length(p, MaxLength, "Invalid service_type length")) {
        }
        explicit ServiceType(const String& p) :
            v_(valid_length(p, MaxLength, "Invalid service_type length")) {
        }
        ServiceType& operator=(const Char* p) {
            v_ = valid_length(p, MaxLength, "Invalid service_type length");
            return *this;
        }
        operator String() const { return v_; }
        size_t length() const { return v_.length(); }
    };
        
    /// @class Ton
    /// @brief Type Of Number
    class Ton {
        Uint8 v_;
      public:
        /// Valid Types of numbers
        enum { 
            Unknown       = 0x00, ///< Unknown (default)
            International = 0x01, ///< International
            National      = 0x02, ///< National
            Network       = 0x03, ///< Network
            Subscriber    = 0x04, ///< Subscriber
            Alphanumeric  = 0x05, ///< Alphanumeric
            Abbreviated   = 0x06  ///< Abbreviated
        };

        Ton() : v_(Unknown) {}
        explicit Ton(const Uint8& p) : v_(check(p)) {}
        explicit Ton(int p) : v_(check(p)) {}
        Ton& operator=(const Uint8& p) { v_ =  p; return *this; }
        Ton& operator=(const int& p) { v_ = p; return *this; }
        operator Uint8() const { return v_; }
      private:
        Uint8 check(const int& p) {
            switch(p) {
                case Unknown: case International: case National: case Network:
                case Subscriber: case Alphanumeric: case Abbreviated:
                    return p;
            }
            throw Error("Invalid TON");
        }
    };

    /// @class Npi
    /// @brief Numbering Plan Indicator
    class Npi {
        Uint8 v_;
      public:
        /// Valid numbering plan indicators
        enum { 
            Unknown  = 0x00, ///< Unknown
            E164     = 0x01, ///< E164
            Data     = 0x03, ///< Data
            Telex    = 0x04, ///< Telex
            Mobile   = 0x06, ///< Mobile
            National = 0x08, ///< National
            Private  = 0x09, ///< Private
            Ermes    = 0x0a, ///< ERMES
            Internet = 0x0e, ///< Internet
            Wap      = 0x12  ///< WAP
        };

        Npi() : v_(Unknown) {}
        explicit Npi(const Uint8& p) : v_(check(p)) {}
        Npi& operator=(const Uint8& p) { v_ = p; return *this; }
        operator Uint8() const { return v_; }
      private:
        Uint8 check(const Uint8& p) {
            switch(p) {
                case Unknown: case E164: case Data: case Telex: case Mobile:
                case National: case Private: case Ermes: case Internet:
                case Wap:
                    return p;
            }
            throw Error("Invalid NPI");
        }
    };

    /// @class Address
    /// @brief Definition of an address.
    class Address {
        String v_;
        size_t len_;
      public:
        /// The different address lengths
        enum {
            MaxLen = 21,   ///< Normal address length
            MaxDataSmLen = 65 ///< Extended address lenth - data_sm
        };

        Address() : len_(MaxLen) {}
        explicit Address(const String& p, size_t len = MaxLen) :
            v_(valid_length(p, len, "Invalid Address length")), len_(len) {}
        explicit Address(const Char* p, size_t len = MaxLen) :
            v_(valid_length(p, len, "Invalid Address length")), len_(len) {}
        Address& operator=(const char* p) {
            v_ = valid_length(p, len_, "Invalid Address length");
            return *this;
        }
        operator String() const { return v_; }
        size_t length() const { return v_.length(); }
    };
   
    /// @class SmeAddress
    /// @brief Definition of the tri-field address.
    class SmeAddress {
        Ton ton_;
        Npi npi_;
        Address addr_;
      public:
        /// @brief Default constructor. Unknown/Unknown/null.
        SmeAddress() {}

        /// @brief Contruct using all parameters.
        /// @param ton The type of number.
        /// @param npi The numbering plan indicator.
        /// @param addr The address.
        /// @oaram len Max address length
        /// (defaults to Address::MaxLen: 21 octets including the
        /// terminating null).
        /// A data_sm PDU can have a length Address::MaxDataSmLen
        /// (65 octets including the terminating null).
        SmeAddress(const Ton& ton, const Npi& npi, const Address& addr,
                                        size_t len = Address::MaxLen) :
            ton_(ton), npi_(npi), addr_(addr, len) {}

        /// @brief Construct using only the address part
        /// (TON and NPI set to unknown.
        /// @param addr The address.
        /// @oaram len Max address length (defaults to Address::MaxLen).
        explicit SmeAddress(
                const Address& addr, size_t len = Address::MaxLen) :
            addr_(addr, len) {}
        const Ton& ton() const { return ton_; }
        const Npi& npi() const { return npi_; }
        const Address& address() const { return addr_; }
        size_t length() const { return 2 + addr_.length() + 1; }

        /// @brief Decode an SME address.
        /// @param buff SMPP PDU buffer offsetted to the SME address.
        /// @param len Number of octets left in the PDU buffer.
        void decode(const Uint8* buff, Uint32 len);
    };

    /// @brief Abstract base class for the two types of submit_multi
    /// destination addresses.
    class MultiDestinationAddressBase {
      public:
          virtual ~MultiDestinationAddressBase() {}
          virtual Uint8 Type() const = 0;
    };

    /// @brief SME destination Address used in a submit_multi
    class SmeMultiAddress : public MultiDestinationAddressBase {
        SmeAddress addr_;
      public:
        SmeMultiAddress(const SmeAddress& addr) : addr_(addr) {}
        virtual Uint8 Type() const { return 0x01; }
        const SmeAddress& Value() const { return addr_; }
    };

    /// @brief Distribution list address used in a submit_multi
    class DistributionListAddress : public MultiDestinationAddressBase {
        String addr_;
      public:
        DistributionListAddress(const String& addr) : addr_(addr) {}
        virtual Uint8 Type() const { return 0x02; }
        const String& Value() const { return addr_; }
    };
  
    /// @brief Manages the list of submit_multi destination addresses.
    /// @note This is the destination address type used in a submit_multi.
    class MultiDestinationAddresses {
        typedef std::list<MultiDestinationAddressBase*> List;
      private:
        List addrs_;
        Uint32 octetCount_;

        struct Delete {
            void operator()(const MultiDestinationAddressBase* p) {
                delete p;
            }
        };
      public:
        MultiDestinationAddresses() : octetCount_(0) {}

        ~MultiDestinationAddresses() throw() {
            std::for_each(addrs_.begin(), addrs_.end(), Delete());
        }

        /// @brief Add an SmeAddress.
        /// @param p The SmeAddress.
        /// @return The number of octets added.
        /// @retval 0 If it was not possible to add the UnsuccessSme.
        Uint32 add(const SmeAddress& p) {
            if(addrs_.size() < 255) {
                MultiDestinationAddressBase* t = new SmeMultiAddress(p);
                addrs_.push_back(t);
                return p.length() + 1;
            }
            return 0;
        }
        
        /// @brief Add a distribution list name.
        /// @param p The distribution list name.
        /// @return The number of octets added.
        /// @retval 0 If it was not possible to add the UnsuccessSme.
        Uint32 add(const String& p) {
            if(addrs_.size() < 255) {
                MultiDestinationAddressBase* t = new DistributionListAddress(p);
                addrs_.push_back(t);
                return p.length() + 2;
            }
            return 0;
        }
        
        Uint8 size() const { return addrs_.size(); }
        Uint32 octet_count() const { return octetCount_; }

        const List& get_list() const { return addrs_; }

        /// @brief Builds the List from a octet array.
        /// @param buff The octet array.
        /// @param len Number of octets left in the octet array
        void decode(const Uint8* buff, Uint32 len);
    };

    /// @brief An submit_multi_resp unsuccess_sme.
    class UnsuccessSme {
        SmeAddress smeAddress_;
        Uint32 error_;
      public:
        /// @brief Constuct from a SmeAddress and error code.
        /// @param p The SmeAddress.
        /// @param error The error code.
        UnsuccessSme(const SmeAddress& p, const Uint32& error) :
            smeAddress_(p), error_(error) {}

        /// @brief Constuct from an octet array
        /// @param buff The octet array.
        /// @param l The octet array length, maybe more than is necessary.
        UnsuccessSme(const Uint8* buff, Uint32 len) { decode(buff, len); }

        /// @brief Copy constructor.
        /// @param p The object to copy.
        UnsuccessSme(const UnsuccessSme& p) :
            smeAddress_(p.smeAddress_), error_(p.error_) {}

        /// @return The SmeAddress part.
        const SmeAddress& smeAddress() const { return smeAddress_; }
        /// @return The error code part.
        const Uint32& error() const { return error_; }

        /// @brief Access the length in octets of the object.
        /// @return The length.
        Uint32 length() const {
            return smeAddress_.length() + sizeof error_;
        }

        /// @brief Decode from an octet array
        /// @param buff The octet array.
        /// @param l The octet array length, maybe more than is necessary.
        void decode(const Uint8* buff, Uint32 len) {
            smeAddress_.decode(buff, len);
            
            const Uint8* e =
                static_cast<const Uint8*>(buff) + smeAddress_.length();
            std::copy(e, e+sizeof(error_), reinterpret_cast<Uint8*>(&error_));
            error_ = ntoh32(error_);
        }
    };
  
    /// @brief Manages the submit_multi_resp no_unsuccess and unsuccessful
    /// parameters.
    class UnsuccessSmeColl {
      public:
        typedef std::list<UnsuccessSme*> List;
      private:
        List v_;
        Uint32 octetCount_;

        struct Delete {
            void operator()(const UnsuccessSme* p) { delete p; }
        };
      public:
        UnsuccessSmeColl() : octetCount_(0) {}
        
        ~UnsuccessSmeColl() throw() {
            std::for_each(v_.begin(), v_.end(), Delete());
        }

        /// @brief Add an UnsuccessSme.
        /// @param p The UnsuccessSme to add.
        /// @return The number of octets added.
        /// @retval 0 If it was not possible to add the UnsuccessSme.
        Uint32 add(const UnsuccessSme& p) {
            if(v_.size() < 255) {
                UnsuccessSme* ptr = new UnsuccessSme(p);
                v_.push_back(ptr);
                return p.length();
            }
            return 0;
        }

        Uint8 size() const { return v_.size(); }
        Uint32 octet_count() const { return octetCount_; }
        const List& get_list() const { return v_; }

        void decode(const Uint8* buff, Uint32 len);
    };
    
    /// @brief Definition of an esm_class
    /// @todo Provide constances for the different possible values
    class EsmClass {
        Uint8 v_;
      public:
        explicit EsmClass(const Uint8& p = 0x00) : v_(p) {}
        EsmClass& operator=(const Uint8& p) { v_ = p;  return *this; }
        operator Uint8() const { return v_; }
    };

    /// @brief Definition of a protocol_id
    /// @todo Provide constances for the different possible values
    class ProtocolId {
        Uint8 v_;
      public:
        explicit ProtocolId(const Uint8& p = 0x00) : v_(p) {}
        ProtocolId& operator=(const Uint8& p) { v_ = p;  return *this; }
        operator Uint8() const { return v_; }
    };

    /// @brief Definition of a priority_flag
    class PriorityFlag {
        Uint8 v_;
      public:
        enum { 
            Normal     = 0x00,
            Immediate  = 0x01,
            High       = 0x02,
            Reserved   = 0x03, // very urgent, emergency
            Background = 0x04
        };

        PriorityFlag() : v_(Normal) {}
        explicit PriorityFlag(const Uint8& p) : v_(check(p)) {}
        PriorityFlag& operator=(const Uint8& p) { v_ = p; return *this; }
        operator Uint8() const { return v_; }
      private:
    
        const Uint8& check(const Uint8& p) {
            switch(p) {
                case Normal: case Immediate: case High: case Reserved:
                case Background:
                    return p;
            }
            throw Error("Invalid priority flag");
        }
    };

    /// @class Time
    /// @brief Definition of an time parameter.
    /// @todo Provide help operations
    class Time {
        String v_;
      public:
        enum { MaxLength = 17 };

        Time() {}
        explicit Time(const String& p) :
            v_(valid_length(p, MaxLength, "Invalid Time length")) {}
        explicit Time(const Char* p) :
            v_(valid_length(p, MaxLength, "Invalid Time length")) {}
        Time& operator=(const char* p) {
            v_ = valid_length(p, MaxLength, "Invalid Time length");
            return *this;
        }
        operator String() const { return v_; }
        size_t length() const { return v_.length(); }
    };

    /// @class RegisteredDelivery
    /// @brief Definition of a registered_delivery parameter
    /// @todo Provide constances for the different possible values
    class RegisteredDelivery {
        Uint8 v_;
      public:
        enum { Default = 0x00 };

        RegisteredDelivery() : v_(Default) {}
        RegisteredDelivery(const Uint8& p) : v_(check(p)) {}
        RegisteredDelivery& operator=(const Uint8& p) {
            v_ = p; return *this;
        }
        operator Uint8() const { return v_; }
      private:
        const Uint8& check(const Uint8& p) {
            return p;
        }
    };

    /// @class ReplaceIfPresentFlag
    /// @brief Definition of a replace_if_present_flag parameter
    class ReplaceIfPresentFlag {
        Uint8 v_;
      public:
        enum { NoReplace = 0x00, Replace   = 0x01 };

        ReplaceIfPresentFlag() : v_(NoReplace) {}
        ReplaceIfPresentFlag(const Uint8& p) : v_(check(p)) {}
        ReplaceIfPresentFlag& operator=(const Uint8& p) { 
            v_ = p; return *this;
        }
        operator Uint8() const { return v_; }
      private:
        const Uint8& check(const Uint8& p) {
            if(p != NoReplace && p != Replace)
                throw Error("Invalid replace if present flag");
            return p;
        }
    };

    /// @class DataCoding
    /// @brief Definition of a data_coding parameter
    /// @todo Provide constances for the different possible values
    class DataCoding {
	public:
		enum DataCodingType { 
			Alphabet       = 0x0, 
			Ascii = 0x1,//01
			Latin      = 0x3,//011 
			Jis       = 0x5, //0101
			UCS2    = 0x8, //1000 
			Default
		};
	private:
        Uint8 v_;
		DataCodingType dc_type_;

      public:
        DataCoding() : v_(0x00) {dc_type();}
        DataCoding(const Uint8& p) : v_(check(p)) {dc_type();}
        DataCoding& operator=(const Uint8& p) {
            v_ = p;
			dc_type();
			return *this;
        }
        operator Uint8() const { return v_; }
		const DataCodingType& get_type()const
		{
			return dc_type_;
		}
      private:
        const Uint8& check(const Uint8& p) {
            return p;
        }
		void dc_type()
		{
			if(v_ == (Uint8)Alphabet)
			{
				dc_type_ =  Alphabet;
			}
			else	if(v_ == (Uint8)Ascii)
			{
				dc_type_ =  Ascii;
			}
			else if(v_ == (Uint8)Latin)
			{
				dc_type_ =  Latin;
			}
			else if(v_ == (Uint8)Jis)
			{
				dc_type_ =  Jis;
			}
			else if(v_ == (Uint8)UCS2)
			{
				dc_type_ =  UCS2;
			}
			else
			{
				dc_type_ =  Default;
			}
			
		}
    };

    /// @class SmDefaultMsgId
    /// @brief Definition of a sm_default_msg_id parameter
    class SmDefaultMsgId {
        Uint8 v_;
      public:
        enum { Unused = 0x00 };
        
        SmDefaultMsgId() : v_(Unused) {}
        SmDefaultMsgId(const Uint8& p) : v_(check(p)) {}
        SmDefaultMsgId& operator=(const Uint8& p) {
            v_ = p; return *this;
        }
        operator Uint8() const { return v_; }
      private:
        const Uint8& check(const Uint8& p) {
            return p;
        }
    };

    /// @class ShortMessage
    /// @brief Definition of a short_message parameter.
    /// @warning 255 octets (V5) are allowed, in V3.3 and V3.4 the limit is 254
    /// @note The sm_length parameter is the result of the length() operation
    class ShortMessage {
        std::vector<Uint8> v_;
      public:
        enum { MaxLength = 255 };

        ShortMessage() { v_.reserve(MaxLength); }
        ShortMessage(const Uint8* p, Uint8 len) : v_(p, p+len) {}
        explicit ShortMessage(const Char* p) : v_(p, p+strlen(p)) {}
        explicit ShortMessage(const String& p) : v_(p.begin(), p.end()) {}
        ShortMessage& operator=(const Uint8* p) {
            v_.assign(p+1, p+(*p + 1));
            return *this;
        }
        ShortMessage& operator=(const Char* p) {
            v_.assign(p, p+strlen(p));
            return *this;
        }
        ShortMessage& operator=(const String& p) {
            v_.assign(p.begin(), p.end());
            return *this;
        }
        operator const std::vector<Uint8>&() const { return v_; }
        size_t length() const { return v_.size(); }
        std::vector<Uint8>::const_iterator begin() const {
            return v_.begin();
        }
        std::vector<Uint8>::const_iterator end() const {
            return v_.end();
        }
    };
    
    /// @class MessageId
    /// @brief Provides message id semantics.
    class MessageId {
        String v_;
      public:
        enum { MaxLength = 65 };
        MessageId() {}
        explicit MessageId(const Char* p) :
            v_(valid_length(p, MaxLength, "Invalid message_id length")) {
        }
        explicit MessageId(const String& p) :
            v_(valid_length(p, MaxLength, "Invalid message_id length")) {
        }
        MessageId& operator=(const Char* p) {
            v_ = valid_length(p, MaxLength, "Invalid message_id length");
            return *this;
        }
        operator String() const { return v_; }
        size_t length() const { return v_.length(); }
    };

    /// @class MessageState
    /// @brief Definition of a message_state parameter
    /// @todo Provide constances for the different possible values
    class MessageState {
        Uint8 v_;
      public:
        enum {
            SCHEDULED = 0,
            ENROUTE = 1,
            DELIVERED = 2,
            EXPIRED = 3,
            DELETED = 4,
            UNDELIVERABLE = 5,
            ACCEPTED = 6,
            UNKNOWN = 7,
            REJECTED = 8,
            SKIPPED = 9
        };

        MessageState() : v_(0x00) {}
        MessageState(const Uint8& p) : v_(check(p)) {}
        MessageState& operator=(const Uint8& p) {
            v_ = p; return *this;
        }
        operator Uint8() const { return v_; }
      private:
        const Uint8& check(const Uint8& p) {
            switch(p) {
                case SCHEDULED: case ENROUTE: case DELIVERED: case EXPIRED:
                case DELETED: case UNDELIVERABLE: case ACCEPTED: case UNKNOWN:
                case REJECTED: case SKIPPED:
                    return p;
            }
            throw Error("Invalid message_state");
        }
    };

    /// @class ErrorCode
    /// @brief Definition of a error_code parameter
    /// @todo Provide constances for the different possible values
    class ErrorCode {
        Uint8 v_;
      public:
        ErrorCode() : v_(0x00) {}
        ErrorCode(const Uint8& p) : v_(check(p)) {}
        ErrorCode& operator=(const Uint8& p) {
            v_ = p; return *this;
        }
        operator Uint8() const { return v_; }
      private:
        const Uint8& check(const Uint8& p) {
            return p;
        }
    };


    //
    // Functions that check the length of strings
    //

    /// @brief Checks the length of a String.
    /// @param s The string.
    /// @param len The allowed length including the NULL terminator.
    /// @param err Message thrown in exception if length is invalid.
    /// @throw Error Containing the err string.
    inline const String&
    valid_length(const String& s, size_t len, const char* err) {
        if(s.length() >= len)
            throw Error(err);
        return s;
    }
        
    /// @brief Checks the length of a Char*
    /// @param s The Char* string.
    /// @param len The allowed length including the NULL terminator.
    /// @param err Message thrown in exception if length is invalid.
    /// @throw Error Containing the err string.
    inline const Char*
    valid_length(const Char* s, size_t len, const char* err) {
        for(size_t i=0; i < len; ++i)
            if(s[i] == '\0')
                return s;
        throw Error(err);
    }

    //-------------------------------------------------------------------------
    // Inline functions that return header parameters.
    // All are passed a pointer that should point to the start of an
    // SMPP encoded PDU.
    //-------------------------------------------------------------------------

    /// @brief Access the command_length from an encoded SMPP PDU.
    /// @param b Pointer to start of encoded SMPP PDU.
    /// @return The decoded command_length.
    inline Uint32 get_command_length(const Uint8* b) {
        Uint32 x;
        std::copy(&b[0], &b[4], reinterpret_cast<Uint8*>(&x));
        return ntoh32(x);
    }

    /// @brief Access the command_id from an encoded SMPP PDU.
    /// @param b Pointer to start of encoded SMPP PDU.
    /// @return The decoded command_id.
    inline Uint32 get_command_id(const Uint8* b) {
        Uint32 x;
        std::copy(&b[4], &b[8], reinterpret_cast<Uint8*>(&x));
        return ntoh32(x);
    }

    /// @brief Access the command_status from an encoded SMPP PDU.
    /// @param b Pointer to start of encoded SMPP PDU.
    /// @return The decoded command_status.
    inline Uint32 get_command_status(const Uint8* b) {
        Uint32 x;
        std::copy(&b[8], &b[12], reinterpret_cast<Uint8*>(&x));
        return ntoh32(x);
    }

    /// @brief Access the sequence_number from an encoded SMPP PDU.
    /// @param b Pointer to start of encoded SMPP PDU.
    /// @return The decoded sequence_number.
    inline Uint32 get_sequence_number(const Uint8* b) {
        Uint32 x;
        std::copy(&b[12], &b[16], reinterpret_cast<Uint8*>(&x));
        return ntoh32(x);
    }

    //-------------------------------------------------------------------------
    // Inline functions that convert to/from host and network byte order.
    //-------------------------------------------------------------------------

    /// @brief Returns true if the current machine is big endian.
    /// @return true if big endian or false if little endian.
    inline bool is_big_endian() // Big Endian is network byte order.
    {
        const long x = 1;
        return *(const char*)&x ? false : true;
    }

    /// @brief Converts a 32 bit integer to network byte order.
    /// @param n The integer in host byte order.
    /// @return The integer in network byte order.
    inline Uint32 ntoh32(const Uint32& n)
    {
        if(is_big_endian())
            return n;
        return (n & 0x000000ff) << 24 |
               (n & 0x0000ff00) << 8 |
               (n & 0x00ff0000) >> 8 |
               (n & 0xff000000) >> 24;
    }

    /// @brief Converts a 32 bit integer to host byte order.
    /// @param n The integer in network byte order.
    /// @return The integer in host byte order.
    inline Uint32 hton32(const Uint32& n)
    {
        if(is_big_endian())
            return n;
        return (n & 0x000000ff) << 24 |
               (n & 0x0000ff00) << 8 |
               (n & 0x00ff0000) >> 8 |
               (n & 0xff000000) >> 24;
    }

    /// @brief Converts a 16 bit integer to network byte order.
    /// @param n The integer in host byte order.
    /// @return The integer in network byte order.
    inline Uint16 ntoh16(const Uint16& n)
    {
        if(is_big_endian())
            return n;
        return (n & 0x00ff) << 8 | (n & 0xff00) >> 8;
    }

    /// @brief Converts a 16 bit integer to host byte order.
    /// @param n The integer in network byte order.
    /// @return The integer in host byte order.
    inline Uint16 hton16(const Uint16& n)
    {
        if(is_big_endian())
            return n;
        return (n & 0x00ff) << 8 | (n & 0xff00) >> 8;
    }

    /// @brief Convert an octet stream into a 16 bit integer.
    /// @param b The octet stream (must be at least 2 bytes).
    /// @return The 16 bit integer.
    inline Uint16 ntoh16(const Uint8* b)
    {
        Uint16 t;
        std::copy(b, b+2, (Uint8*)&t);
        return ntoh16(t);
    }
    
    /// @brief Convert an octet stream into a 32 bit integer.
    /// @param b The octet stream (must be at least 4 bytes).
    /// @return The 32 bit integer.
    inline Uint32 ntoh32(const Uint8* b)
    {
        Uint32 t;
        std::copy(b, b+4, (Uint8*)&t);
        return ntoh32(t);
    }

} // namespace Smpp

#endif

