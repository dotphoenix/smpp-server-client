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

/// @file aux_types.cpp
/// @brief Functions declarations from aux_types.hpp

#include "aux_types.hpp"

/// @brief Decode an SME address.
/// @param buff SMPP PDU buffer offsetted to the SME address.
/// @param len Number of octets left in the PDU buffer.
void
Smpp::SmeAddress::decode(const Smpp::Uint8* buff, Smpp::Uint32 len)
{
    if(len < 3)
        throw Smpp::Error("Invalid SmeAddress length");

    const Smpp::Uint8* t = buff;
    ton_ = t[0];
    npi_ = t[1];
    const Smpp::Char* s = reinterpret_cast<const Smpp::Char*>(buff);
    addr_ = s+2;
}

/// @brief Decodes a submit_multi destaination_addr.
/// @param buff The octet array.
/// @param len Number of octets left in the octet array
void Smpp::MultiDestinationAddresses::decode(const Smpp::Uint8* buff, Smpp::Uint32 len)
{
    const Smpp::Uint8* ptr = buff;
    if(len < 3)
        throw Smpp::Error("Invalid command_length");
    
    Smpp::Uint8 cnt = ptr[octetCount_++];

    for(Uint32 i = 0; i < cnt; ++i) {
        Smpp::Uint8 dest_flag = ptr[octetCount_++];
        switch(dest_flag) {
            case 0x01: // SME Address
              {
                SmeAddress smeaddr;
                smeaddr.decode(ptr + octetCount_, len - octetCount_);
                octetCount_ += smeaddr.length();
                if(octetCount_ > len)
                    throw Smpp::Error("Invalid command_length");
                this->add(smeaddr);
                break;
              }
            case 0x02: // Distribution list address
              {
                Smpp::String dlname =
                        reinterpret_cast<const Smpp::Char*>(buff) + octetCount_;
                octetCount_ += dlname.length() + 1; // '\0'
                if(octetCount_ > len)
                    throw Smpp::Error("Invalid command_length");
                this->add(dlname);
                break;
              }
            default:
                throw Smpp::Error("Invalid dest_flag");
                break;
        } // switch(...
    } // for( ...
}

/// @brief decode function for a submit_multi_resp unsuccess_sme.
/// @param buff The buffer that is been decoded.
/// @param len The number of octets left in the buffer.
void
Smpp::UnsuccessSmeColl::decode(const Smpp::Uint8* buff, Smpp::Uint32 len) {
    const Smpp::Uint8* ptr = static_cast<const Smpp::Uint8*>(buff);
    if(len < 1)
        throw Smpp::Error("Invalid command_length");

    // first octet in the number of unsuccess (no_unsuccess).
    Smpp::Uint8 cnt = ptr[octetCount_++];

    for(Uint32 i = 0; i < cnt; ++i) {
        UnsuccessSme addr(ptr + octetCount_, len - octetCount_);
        octetCount_ += addr.length();
        if(octetCount_ > len)
            throw Smpp::Error("Invalid command_length");
        this->add(addr);
    }
}

/// @brief Prints a hex dump of a buffer.
/// @note The debug are rows of 16 octets with their hexadecimal value on
/// the left and if printable their character representation on the right
/// or a dot (.) if not printable.
/// @param buff The buffer to print.
/// @param len Number of octets in buff.
/// @param file The C file stream to write to (default standard output).
void Smpp::chex_dump(const Smpp::Uint8* buff, Smpp::Uint32 len, FILE* file)
{
    if(len == 0)
        return;

    Smpp::Uint32 i;
    for(i = 0; i < len; ++i) {
        if(i != 0 && i % 16 == 0) {
            fprintf(file, "   ");
            for(Smpp::Uint32 j = i - 16; j < i; ++j)
                if(buff[j] > 31 && buff[j] < 128)
                    fprintf(file, "%c", static_cast<char>(buff[j]));
                else
                    fprintf(file, ".");
            fprintf(file, "\n");
        }
    
        fprintf(file, "%02x ", buff[i]);
    }

    Smpp::Uint32 m = i % 16 ? 16 - i % 16 : 0; // octets needed to make up 16
    m && fprintf(file, "%*c", m * 3, ' ');
    fprintf(file, "   ");
    for(Smpp::Uint32 j = i % 16 ? i - i % 16 : i - 16 ; j < i; ++j)
        if(buff[j] > 31 && buff[j] < 128)
            fprintf(file, "%c", static_cast<char>(buff[j]));
        else
            fprintf(file, ".");

    fprintf(file, "\n");
    fflush(file);
}

/// @brief Prints a hex dump of a buffer.
/// @note The debug are rows of 16 octets with their hexadecimal value on
/// the left and if printable their character representation on the right
/// or a dot (.) if not printable.
/// @param buff The buffer to print.
/// @param len Number of octets in buff.
/// @param file The C++ file stream to write to (default standard output).
void Smpp::hex_dump(const Smpp::Uint8* buff, Smpp::Uint32 len, std::ostream& os)
{
    if(len == 0)
        return;
/*
    Smpp::Uint32 i;
    for(i = 0; i < len; ++i) {
        if(i != 0 && i % 16 == 0) {
            os << "   ";
            for(Smpp::Uint32 j = i - 16; j < i; ++j)
                if(buff[j] > 31 && buff[j] < 128)
                    os << static_cast<char>(buff[j]);
                else
                    os << ".";
            os << "\n";
        }
    
        os << std::setw(2) << std::setfill('0')
           << std::hex << (int)buff[i] << std::dec << ' ';
    }

    Smpp::Uint32 m = i % 16 ? 16 - i % 16 : 0;// octets needed to make up 16
    m && os << std::setw(m*3) << std::setfill(' ') << ' ';
    
    os << "   ";
    for(Smpp::Uint32 j = i % 16 ? i - i % 16 : i - 16 ; j < i; ++j)
        if(buff[j] > 31 && buff[j] < 128)
            os << static_cast<char>(buff[j]);
        else
            os << ".";

    os << "\n";
    std::flush(os);
	*/
}

