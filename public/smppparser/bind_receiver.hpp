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
#ifndef __SMPP_BIND_RECEIVER_HPP__
#define __SMPP_BIND_RECEIVER_HPP__

/// @file bind_receiver.hpp
/// @brief An SMPP bind_receiver PDU.

#include "aux_types.hpp"
#include "header.hpp"

namespace Smpp {

    // It would be neater to write one templated BasicBind class and typedef
    // the different binds off it. But that would need all functions to be
    // either inlined or good support for the export keyword. So each bind has
    // its separate class defined.

    /// @class BindReceiver
    /// @brief Definition of an SMPP bind_receiver, derived from a Request.
    class BindReceiver : public Request {
        SystemId system_id_;
        Password password_;
        SystemType system_type_;
        InterfaceVersion interface_version_;
        Ton addr_ton_;
        Npi addr_npi_;
        AddressRange address_range_;

        Smpp::Buffer buff_;

      public:
        /** submit_sm minimum length in octets */
        const static int MinLength = 23;
        
        /** @brief Default constructor. */
        BindReceiver();
        
        /** @brief Constructor requiring all mandatory parameters. */
        BindReceiver(
                const SequenceNumber& sequenceNumber,
                const SystemId& systemId,
                const Password& password,
                const SystemType& systemType,
                const InterfaceVersion& interfaceVersion,
                const Ton& addrTon,
                const Npi& addrNpi,
                const AddressRange& addressRange);
       
        /// @brief Construct from a buffer.
        BindReceiver(const Smpp::Uint8* b);

        /// @brief Destructor - does nothing.
        ~BindReceiver();
        
        //
        // Mutating
        //
        
        /// @brief Set the system id.
        /// @param p The new system id.
        void system_id(const SystemId& p) {
            int diff = p.length() - system_id_.length();
            system_id_ = p;
            Header::update_length(diff);
        }
        
        /// @brief Set the system id.
        /// @param p The new system id.
        void system_id(const Smpp::Char* p) {
            int diff = strlen(p) - system_id_.length();
            system_id_ = p;
            Header::update_length(diff);
        }
        
        /// @brief Set the password.
        /// @param p The new password.
        void password(const Password& p) {
            int diff = p.length() - password_.length();
            password_ = p;
            Header::update_length(diff);
        }
        
        /// @brief Set the password.
        /// @param p The new password.
        void password(const Smpp::Char* p) {
            int diff = strlen(p) - password_.length();
            password_ = p;
            Header::update_length(diff);
        }
        
        /// @brief Set the system type.
        /// @param p The new system type.
        void system_type(const SystemType& p) {
            int diff = p.length() - system_type_.length();
            system_type_ = p;
            Header::update_length(diff);
        }
        
        /// @brief Set the system type.
        /// @param p The new system type.
        void system_type(const Smpp::Char* p) {
            int diff = strlen(p) - system_type_.length();
            system_type_ = p;
            Header::update_length(diff);
        }
       
        /// @brief Set the interface version.
        /// @param p The new interface version.
        void interface_version(const int& p) { interface_version_ = p; }

        /// @brief Set the addr ton.
        /// @param p The new addr ton.
        void addr_ton(const int& p) { addr_ton_ = p; }

        /// @brief Set the addr np.
        /// @param p The new addr np.
        void addr_npi(const int& p) { addr_npi_ = p; }

        /// @brief Set the address range.
        /// @param p The new address range.
        void address_range(const AddressRange& p) {
            int diff = p.length() - address_range_.length();
            address_range_ = p;
            Header::update_length(diff);
        }
        
        /// @brief Set the system type.
        /// @param p The new system type.
        void address_range(const Smpp::Char* p) {
            int diff = strlen(p) - address_range_.length();
            address_range_ = p;
            Header::update_length(diff);
        }
       
        // 
        // Accessing
        // 
        
        /// @brief Access the system id.
        /// @return The system id.
        const SystemId& system_id() const { return system_id_; }
        
        /// @brief Access the password.
        /// @return The password.
        const Password& password() const { return password_; }
        
        /// @brief Access the system type.
        /// @return The system type.
        const SystemType& system_type() const { return system_type_; }
        
        /// @brief Access the interface version.
        /// @return The interface version.
        const InterfaceVersion& interface_version() const {
            return interface_version_;
        }
        
        /// @brief Access the addr ton.
        /// @return The addr ton.
        const Ton& addr_ton() const { return addr_ton_; }
        
        /// @brief Access the addr npi.
        /// @return The addr npi.
        const Npi& addr_npi() const { return addr_npi_; }
        
        /// @brief Access the address range.
        /// @return The address range.
        const AddressRange& address_range() const { return address_range_; }
        
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

