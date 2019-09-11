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
#ifndef __SMPP_BUFFER_HPP__
#define __SMPP_BUFFER_HPP__

/// @file buffer.hpp
/// @brief A buffer used for encoding PDUs.

#include "aux_types.hpp"

namespace Smpp {
    /// @brief Used for encoding a PDU.
    class Buffer {
        Smpp::Uint8* data_;
        Smpp::Uint32 offset_;

        // Stop copying and assigning
        Buffer(const Buffer&);
        Buffer& operator=(const Buffer&);
      public:
        Buffer();
        explicit Buffer(Uint32 len);
        ~Buffer() throw();

        void reset();
        void reset(const Smpp::Uint32&);
        Buffer& operator+=(const Smpp::Uint32& p);
        Buffer& operator+=(const Smpp::Uint16& p);
        Buffer& operator+=(const Smpp::Uint8& p);
        Buffer& operator+=(const Smpp::String& p);
        Buffer& operator+=(const ShortMessage& p);
        Buffer& operator+=(const SmeAddress& p);
        Buffer& operator+=(const MultiDestinationAddresses& p);
        Buffer& operator+=(const UnsuccessSmeColl& p);
        Buffer& add_octet_array(const Smpp::Uint32& length, const Smpp::Uint8* arr);

        const Smpp::Uint8* get() const { return data_; }
        friend void print(const Buffer& b, FILE* fptr);
    };

    /// @brief Resets the buffer\n.
    /// Deallocates any data and sets the the length to 0.
    inline void Buffer::reset()
    {
        if(data_) {
            delete [] data_;
            data_ = 0;
            offset_ = 0;
        }
    }
    
    /// @brief Resets the buffer\n.
    /// Deallocates any data and sets the the length to l.
    /// @param l Allocates for \c l octets.
    inline void Buffer::reset(const Smpp::Uint32& l)
    {
        reset();
        data_ = new Smpp::Uint8[l];
    }
   
    /// @brief Copies a 32 bit unsigned integer to the buffer.
    /// @param p The 32 bit unsigned integer to copy.
    inline Buffer& Buffer::operator+=(const Smpp::Uint32& p)
    {
        ::memcpy(&data_[offset_], &p, sizeof p);
        offset_ += sizeof p;
        return *this;
    }
    
    /// @brief Copies a 16 bit unsigned integer to the buffer.
    /// @param p The 16 bit unsigned integer to copy.
    inline Buffer& Buffer::operator+=(const Smpp::Uint16& p)
    {
        ::memcpy(&data_[offset_], &p, sizeof p);
        offset_ += sizeof p;
        return *this;
    }
    
    /// @brief Copies an 8 bit unsigned integer to the buffer.
    /// @param p The 8 bit unsigned integer to copy.
    inline Buffer& Buffer::operator+=(const Smpp::Uint8& p)
    {
        data_[offset_++] = p;
        return *this;
    }
    
    /// @brief Copies a string to the buffer.
    /// @param p The string to copy.
    inline Buffer& Buffer::operator+=(const Smpp::String& p)
    {
        std::copy(p.begin(), p.end(), data_ + offset_);
        offset_ += p.length() + 1;
        data_[offset_-1] = 0;
        return *this;
    }

    /// @brief Copies a ShortMessage object to the buffer.
    /// @param p The ShortMessage to copy.
    inline Buffer& Buffer::operator+=(const ShortMessage& p)
    {
        data_[offset_++] = p.length();
        std::copy(p.begin(), p.end(), &data_[offset_]);
        offset_ += p.length();
        return *this;
    }

    /// @brief Copies a SmeAddress object to the buffer.
    /// @param p The SmeAddress to copy.
    inline Buffer& Buffer::operator+=(const SmeAddress& p)
    {
        *this += p.ton();
        *this += p.npi();
        *this += p.address();
        return *this;
    }

    /// @brief Functor used to copy submit_multi destination addresses.
    class CopySubmitMultiAddresses {
        Smpp::Buffer& b_;
      public:
        CopySubmitMultiAddresses(Smpp::Buffer& b) : b_(b) {}
        Smpp::Buffer& get() { return b_; }
        void operator()(const MultiDestinationAddressBase* p);
    };

    /// @brief Copies a MultiDestinationAddress object to the buffer.
    /// @param p The MultiDestinationAddress to copy.
    inline Buffer& Buffer::operator+=(const MultiDestinationAddresses& p)
    {
        *this += p.size();
        CopySubmitMultiAddresses i(*this);
        return std::for_each(p.get_list().begin(), p.get_list().end(), i).get();
    }

    /// @brief Functor used to copy submit_multi_resp unsuccess SMEs.
    class CopyUnsuccessSmeColl {
        Smpp::Buffer& b_;
      public:
        CopyUnsuccessSmeColl(Smpp::Buffer& b) : b_(b) {}

        Smpp::Buffer& get() { return b_; }

        void operator()(const UnsuccessSme* p) {
            b_ += p->smeAddress();
            b_ += Smpp::hton32(p->error());
        }
    };

    /// @brief Copies a UnsuccessSmeColl object to the buffer.
    /// @param p The UnsuccessSmeColl to copy.
    inline Buffer& Buffer::operator+=(const UnsuccessSmeColl& p)
    {
        *this += p.size();
        CopyUnsuccessSmeColl i(*this);
        return std::for_each(p.get_list().begin(), p.get_list().end(), i).get();
    }
    
    /// @brief Copies an octet array to the buffer.
    /// @param p The octet array to copy.
    inline Buffer& Buffer::add_octet_array(
                                        const Smpp::Uint32& length, const Smpp::Uint8* arr)
    {
        std::copy(arr, arr+length, data_ + offset_);
        offset_ += length;
        return *this;
    }
}

#endif

