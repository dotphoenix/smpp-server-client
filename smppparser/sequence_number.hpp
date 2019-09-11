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
#ifndef __SMPP_SEQUENCE_NUMBER_HPP__
#define __SMPP_SEQUENCE_NUMBER_HPP__

/// @file sequence_number.hpp

#include <cstring>
#include "aux_types.hpp"

namespace Smpp {

    /// @class SequenceNumber
    /// @brief Defines sequence number rules.
    class SequenceNumber {
        SequenceNumber();
        Smpp::Uint32 val_;
    public:
        /// Sequence number limits.
		//The allowed sequence_number range is from 0x00000001 to 0x7FFFFFFF.
		//we should disable 0 to comply with the spec.
        enum {
            Min = 0x00000000, ///< minimum sequence number
            Max = 0x7fffffff  ///< maximum sequence number
        };

        /// @brief Sets a sequence number.
        /// @param p The sequence number.
        /// @param allow_0 True if 0 (zero) is a valid sequence number.
        /// @return The sequence number.
        /// @throw Smpp::Error if the sequence number is invalid
        static Smpp::Uint32 assign(const Smpp::Uint32& p,  bool allow_0 = false) {
            if((p <= Max && p > 0) || (p == 0 && allow_0 == true))
                return p;
            else
                throw Error("Invalid sequence number");
        }
       
		static bool valid(Smpp::Uint32 v)
		{
			return ((v <= Max) && (v >= Min));
		}
        /// @brief Constructs a sequence number from p.
        /// @param p The sequence number.
        /// @param allow_0 True if 0 (zero) is a valid sequence number.
        /// @return The sequence number.
        /// @throw Smpp::Error if the sequence number is invalid
        SequenceNumber(const Smpp::Uint32& p,  bool allow_0 = true) :
            val_(assign(p, allow_0)) {}

        /// @brief The prefix increment operator.
        /// @return The incremented value
        SequenceNumber& operator++() {
            val_ = (val_ == Max) ? Min : ++val_;
            return *this;
        }
        
        /// @brief The postfix increment operator.
        /// @return The value before been incremented.
        SequenceNumber operator++(int) {
            SequenceNumber t = *this;
            val_ = (val_ == Max) ? Min : ++val_;
            return t;
        }
        
		bool operator== (const SequenceNumber& sn)
		{
			return (sn.val_ == val_);
		}
		
		SequenceNumber& operator= (Smpp::Uint32 val)
		{
			val_ = val;
			return *this;
		}
		
        /// @brief Accesses the sequence number as a 32 bit unsigned int.
        /// @return The sequence number.
        operator Smpp::Uint32() const { return val_; }

        /// @brief Decode the sequence number from an SMPP PDU octet buffer.
        /// @param buff The octet buffer (array).
        /// @return The sequence number.
        static Smpp::Uint32 decode(const Smpp::Char* buff) {
            Smpp::Uint32 seqnum;
            memcpy(&seqnum, &buff[12], sizeof seqnum);
            return Smpp::ntoh32(seqnum);
        }

    };

} // namespace Smpp

#endif

