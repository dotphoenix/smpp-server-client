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
#ifndef __SMPP_COMMAND_STATUS_HPP__
#define __SMPP_COMMAND_STATUS_HPP__

/// @file command_status.hpp
/// @brief Defines a command status parameter.

#include <cstring>
#include "aux_types.hpp"

namespace Smpp {

    /**
     * @class CommandStatus
     */
    class CommandStatus {
        
        Smpp::Uint32 val_;
    public:
        /** Status code values */
        enum {
            ESME_ROK = 0x00000000,
            ESME_RINVMSGLEN = 0x00000001,
            ESME_RINVCMDLEN = 0x00000002,
            ESME_RINVCMDID = 0x00000003,
            ESME_RINVBNDSTS = 0x00000004,
            ESME_RALYBND = 0x00000005,
            ESME_RINVPRTFLG = 0x00000006,
            ESME_RINVREGDLVFLG = 0x00000007,
            ESME_RSYSERR = 0x00000008,
            ESME_RINVSRCADR = 0x0000000A,
            ESME_RINVDSTADR = 0x0000000B,
            ESME_RINVMSGID = 0x0000000C,
            ESME_RBINDFAIL = 0x0000000D,
            ESME_RINVPASWD = 0x0000000E,
            ESME_RINVSYSID = 0x0000000F,
            ESME_RCANCELFAIL = 0x00000011,
            ESME_RREPLACEFAIL = 0x00000013,
            ESME_RMSGQFUL = 0x00000014,
            ESME_RINVSERTYP = 0x00000015,
            ESME_RINVNUMDESTS = 0x00000033,
            ESME_RINVDLNAME = 0x00000034,
            ESME_RINVDESTFLAG = 0x00000040,
            ESME_RINVSUBREP = 0x00000042,
            ESME_RINVESMCLASS = 0x00000043,
            ESME_RCNTSUBDL = 0x00000044,
            ESME_RSUBMITFAIL = 0x00000045,
            ESME_RINVSRCTON = 0x00000048,
            ESME_RINVSRCNPI = 0x00000049,
            ESME_RINVDSTTON = 0x00000050,
            ESME_RINVDSTNPI = 0x00000051,
            ESME_RINVSYSTYP = 0x00000053,
            ESME_RINVREPFLAG = 0x00000054,
            ESME_RINVNUMMSGS = 0x00000055,
            ESME_RTHROTTLED = 0x00000058,
            ESME_RINVSCHED = 0x00000061,
            ESME_RINVEXPIRY = 0x00000062,
            ESME_RINVDFTMSGID = 0x00000063,
            ESME_RX_T_APPN = 0x00000064,
            ESME_RX_P_APPN = 0x00000065,
            ESME_RX_R_APPN = 0x00000066,
            ESME_RQUERYFAIL = 0x00000067,
            ESME_RINVTLVSTREAM = 0x000000C0,
            ESME_RTLVNOTALLWD = 0x000000C1,
            ESME_RINVTLVLEN = 0x000000C2,
            ESME_RMISSINGTLV = 0x000000C3,
            ESME_RINVTLVVAL = 0x000000C4,
            ESME_RDELIVERYFAILURE = 0x000000FE,
            ESME_RUNKNOWNERR = 0x000000FF,
            ESME_RSERTYPUNAUTH = 0x00000100,
            ESME_RPROHIBITED = 0x00000101,
            ESME_RSERTYPUNAVAIL = 0x00000102,
            ESME_RSERTYPDENIED = 0x00000103,
            ESME_RINVDCS = 0x00000104,
            ESME_RINVSRCADDRSUBUNIT = 0x00000105,
            ESME_RINVDSTADDRSUBUNIT = 0x00000106,
            ESME_RINVBCASTFREQINT = 0x00000107,
            ESME_RINVBCASTALIAS_NAME = 0x00000108,
            ESME_RINVBCASTAREAFMT = 0x00000109,
            ESME_RINVNUMBCAST_AREAS = 0x0000010A,
            ESME_RINVBCASTCNTTYPE = 0x0000010B,
            ESME_RINVBCASTMSGCLASS = 0x0000010C,
            ESME_RBCASTFAIL = 0x0000010D,
            ESME_RBCASTQUERYFAIL = 0x0000010E,
            ESME_RBCASTCANCELFAIL = 0x0000010F,
            ESME_RINVBCAST_REP = 0x00000110,
            ESME_RINVBCASTSRVGRP = 0x00000111,
            ESME_RINVBCASTCHANIND = 0x00000112,
			/*
			 0x00000400- 0x000004FF
			 Reserved for SMSC vendor specific errors
			 */
			ESME_RRDBFAILED = 0x00000400,
			ESME_RWDBFAILED = 0x00000401,
			ESME_RCOMMITDBFAILED = 0x00000402,
			ESME_REXECUTEDBFAILED = 0x00000403,
            ESME_RCREATEDBFAILED = 0x00000404,
            ESME_ROPENDBFAILED = 0x00000405,
			ESME_RSEQUENCENUMBERNOTEQUAL = 0x00000406,

			ESME_RINVBINDIP = 0x00000410,
			ESME_RBALANCENOTENOUGH = 0x00000411,
			ESME_RNUMBERINBLACKLIST = 0x00000412,
			ESME_RNUMBERINWHITELIST = 0x00000413,
			ESME_RCONTENTINCLUDESENSITIVEWORDS = 0x00000414,
			ESME_RDESTNOTREACHABLEBYPREFIX = 0x00000415,
			ESME_RDESTNOTREACHABLEBYRATETABLE = 0x00000416,
            ESME_RDESTOPERATORNOTREACHABLE = 0x00000417,
            

			ESME_RCOMPANYACCOUNTALREADYEXIST = 0x00000420,
			ESME_RCOMPANYACCOUNTNOTEXIST = 0x00000421,
			ESME_RPWINCORRECT = 0x00000422,
			ESME_RSUBCCOUNTALREADYEXIST = 0x00000423,
			ESME_RSUBACCOUNTNOTEXIST = 0x00000424,
            ESME_RSUBACCOUNTNOTINCOMPANYACCOUNT = 0x00000424,

            ESME_RDATALENGTHNOTCORRECT = 0x00000430,
            ESME_RDATALASTBYTEMUSTBE0 = 0x00000431,
            ESME_RDATAPARSEHEADERFAILED = 0x00000432,
            ESME_RDATAPARSEBODYFAILED = 0x00000433,
			ESME_RDATABODYPARAMINCORRECT = 0x00000434,

            ESME_RCREATESOCKFAILED = 0x00000440,
            ESME_RWSOCKFAILED = 0x00000441,
            ESME_RRSOCKFAILED = 0x00000442,
            ESME_RCSOCKFAILED = 0x00000443,

            ESME_RKEYWORDALREADYEXIST = 0x00000450,
            ESME_RSENSITIVEWORDALREADYEXIST = 0x00000451,

            ESME_RSMSINFONOTEXIST = 0x00000460,
			
			ESME_RUNKNOWN = 0x000004A0,

            ESME_RJUMBLEDATA = 0x000004FF,
            ESME_RNULLDATA = 0x000004FF,
        };

        /** @brief A brief description of the command status
         * @return A brief string description of the command status */
        inline static std::string description(const CommandStatus& p) {
            switch(p) {
                case ESME_ROK:
                    return "No Error";
                    break;
                case ESME_RINVMSGLEN:
                    return "Message Length is invalid";
                    break;
                case ESME_RINVCMDLEN:
                    return "Command Length is invalid";
                    break;
                case ESME_RINVCMDID:
                    return "Invalid Command ID";
                    break;
                case ESME_RINVBNDSTS:
                    return "Incorrect BIND Status for given command";
                    break;
                case ESME_RALYBND:
                    return "ESME Already in Bound State";
                    break;
                case ESME_RINVPRTFLG:
                    return "Invalid Priority Flag";
                    break;
                case ESME_RINVREGDLVFLG:
                    return "Invalid Registered Delivery Flag";
                    break;
                case ESME_RSYSERR:
                    return "System Error";
                    break;
                case ESME_RINVSRCADR:
                    return "Invalid Source Address";
                    break;
                case ESME_RINVDSTADR:
                    return "Invalid Destination Address";
                    break;
                case ESME_RINVMSGID:
                    return "Message ID is invalid";
                    break;
                case ESME_RBINDFAIL:
                    return "Bind Failed";
                    break;
                case ESME_RINVPASWD:
                    return "Invalid Password";
                    break;
                case ESME_RINVSYSID:
                    return "Invalid System ID";
                    break;
                case ESME_RCANCELFAIL:
                    return "Cancel SM Failed";
                    break;
                case ESME_RREPLACEFAIL:
                    return "Replace SM Failed";
                    break;
                case ESME_RMSGQFUL:
                    return "Message Queue Full";
                    break;
                case ESME_RINVSERTYP:
                    return "Invalid Service Type";
                    break;
                case ESME_RINVNUMDESTS:
                    return "Invalid number of destinations";
                    break;
                case ESME_RINVDLNAME:
                    return "Invalid Distribution List name";
                    break;
                case ESME_RINVDESTFLAG:
                    return "Destination flag is invalid (submit_multi)";
                    break;
                case ESME_RINVSUBREP:
                    return "Submit w/replace functionality has been requested where it is either unsupported or inappropriate for the particular MC";
                    break;
                case ESME_RINVESMCLASS:
                    return "Invalid esm_class field data";
                    break;
                case ESME_RCNTSUBDL:
                    return "Cannot Submit to Distribution List";
                    break;
                case ESME_RSUBMITFAIL:
                    return "submit_sm, data_sm or submit_multi failed";
                    break;
                case ESME_RINVSRCTON:
                    return "Invalid Source address TON";
                    break;
                case ESME_RINVSRCNPI:
                    return "Invalid Source address NPI";
                    break;
                case ESME_RINVDSTTON:
                    return "Invalid Destination address TON";
                    break;
                case ESME_RINVDSTNPI:
                    return "Invalid Destination address NPI";
                    break;
                case ESME_RINVSYSTYP:
                    return "Invalid system_type field";
                    break;
                case ESME_RINVREPFLAG:
                    return "Invalid replace_if_present flag";
                    break;
                case ESME_RINVNUMMSGS:
                    return "Invalid number of messages";
                    break;
                case ESME_RTHROTTLED:
                    return "Throttling Error (ESME has exceeded allowed message limits)";
                    break;
                case ESME_RINVSCHED:
                    return "Invalid Scheduled Delivery Time";
                    break;
                case ESME_RINVEXPIRY:
                    return "Invalid message validity period (Expiry time)";
                    break;
                case ESME_RINVDFTMSGID:
                    return "Predefined Message ID is Invalid or specified predefined message was not found";
                    break;
                case ESME_RX_T_APPN:
                    return "ESME Receiver Temporary App Error Code";
                    break;
                case ESME_RX_P_APPN:
                    return "ESME Receiver Permanent App Error Code";
                    break;
                case ESME_RX_R_APPN:
                    return "ESME Receiver Reject Message Error Code";
                    break;
                case ESME_RQUERYFAIL:
                    return "query_sm request failed";
                    break;
                case ESME_RINVTLVSTREAM:
                    return "Error in the optional part of the PDU Body";
                    break;
                case ESME_RTLVNOTALLWD:
                    return "TLV not allowed";
                    break;
                case ESME_RINVTLVLEN:
                    return "Invalid Parameter Length";
                    break;
                case ESME_RMISSINGTLV:
                    return "Expected TLV missing";
                    break;
                case ESME_RINVTLVVAL:
                    return "Invalid TLV Value";
                    break;
                case ESME_RDELIVERYFAILURE:
                    return "Transaction Delivery Failure";
                    break;
                case ESME_RUNKNOWNERR:
                    return "Unknown Error";
                    break;
                case ESME_RSERTYPUNAUTH:
                    return "ESME Not authorised to use specified service_type";
                    break;
                case ESME_RPROHIBITED:
                    return "ESME Prohibited from using specified operation";
                    break;
                case ESME_RSERTYPUNAVAIL:
                    return "Specified service_type is unavailable";
                    break;
                case ESME_RSERTYPDENIED:
                    return "Specified service_type is denied";
                    break;
                case ESME_RINVDCS:
                    return "Invalid Data Coding Scheme";
                    break;
                case ESME_RINVSRCADDRSUBUNIT:
                    return "Source Address Sub unit is Invalid";
                    break;
                case ESME_RINVDSTADDRSUBUNIT:
                    return "Destination Address Sub unit is Invalid";
                    break;
                case ESME_RINVBCASTFREQINT:
                    return "Broadcast Frequency Interval is invalid";
                    break;
                case ESME_RINVBCASTALIAS_NAME:
                    return "Broadcast Alias Name is invalid";
                    break;
                case ESME_RINVBCASTAREAFMT:
                    return "Broadcast Area Format is invalid";
                    break;
                case ESME_RINVNUMBCAST_AREAS:
                    return "Number of Broadcast Areas is invalid";
                    break;
                case ESME_RINVBCASTCNTTYPE:
                    return "Broadcast Content Type is invalid";
                    break;
                case ESME_RINVBCASTMSGCLASS:
                    return "Broadcast Message Class is invalid";
                    break;
                case ESME_RBCASTFAIL:
                    return "broadcast_sm operation failed";
                    break;
                case ESME_RBCASTQUERYFAIL:
                    return "query_broadcast_sm operation failed";
                    break;
                case ESME_RBCASTCANCELFAIL:
                    return "cancel_broadcast_sm operation failed";
                    break;
                case ESME_RINVBCAST_REP:
                    return "Number of Repeated Broadcasts is invalid";
                    break;
                case ESME_RINVBCASTSRVGRP:
                    return "Broadcast Service Group is invalid";
                    break;
                case ESME_RINVBCASTCHANIND:
                    return "Broadcast Channel Indicator is invalid";
                    break;
            }
            if(p >= 0x00000400 && p <= 0x000004FF)
                return "Reserved for MC vendor specific Errors";

            return "Reserved by SMPP";
        }

        /** @brief A long description of a command status
         * @return A long description of a command status */
        inline static std::string long_description(const CommandStatus& p) {
            switch(p) {
                case ESME_ROK:
                    return "Specified in a response PDU to indicate the success of the corresponding request PDU.";
                    break;
                case ESME_RINVMSGLEN:
                    return "short_message field or message_payload TLV has an invalid length (usually too long for the given MC or underlying network technology).";
                    break;
                case ESME_RINVCMDLEN:
                    return "PDU length is considered invalid, either because the value is too short or too large for the given PDU.";
                    break;
                case ESME_RINVCMDID:
                    return "Command ID is not recognised, either because the operation is not supported or unknown.";
                    break;
                case ESME_RINVBNDSTS:
                    return "PDU has been sent in the wrong session state. E.g. sending a submit_sm without first establishing a Bound_TX session state.";
                    break;
                case ESME_RALYBND:
                    return "A bind request has been issued within a session that is already bound.";
                    break;
                case ESME_RINVPRTFLG:
                    return "Priority flag contains an illegal or unsupported value.";
                    break;
                case ESME_RINVREGDLVFLG:
                    return "Registered field contains an invalid setting.";
                    break;
                case ESME_RSYSERR:
                    return "MC system Error indicating that all or part of the MC is currently unavailable. This can be returned in any response PDU.";
                    break;
                case ESME_RINVSRCADR:
                    return "Source address of message is considered invalid. Usually this is because the field is either too long or contains invalid characters.";
                    break;
                case ESME_RINVDSTADR:
                    return "Destination address of message is considered invalid. Usually this is because the field is either zero length, too long or contains invalid characters.";
                    break;
                case ESME_RINVMSGID:
                    return "Message ID specified in cancel_sm, query_sm or other operations is invalid.";
                    break;
                case ESME_RBINDFAIL:
                    return "A generic failure scenario for a bind attempt. This may be due to a provisioning Error, incorrect password or other reason. A MC will typically return this Error for an invalid system_id, system_type, password or other attribute that may cause a bind failure.";
                    break;
                case ESME_RINVPASWD:
                    return "Password field in bind PDU is invalid. This is usually returned when the length is too short or too long. It is not supposed to be returned when the ESME has specified the incorrect password.";
                    break;
                case ESME_RINVSYSID:
                    return "The System ID field in bind PDU is invalid. This is usually returned when the length is too short or too long. It is not supposed to be returned when the ESME has specified the incorrect system id.";
                    break;
                case ESME_RCANCELFAIL:
                    return "Generic failure Error for cancel_sm operation.";
                    break;
                case ESME_RREPLACEFAIL:
                    return "Generic failure for replace_sm operation.";
                    break;
                case ESME_RMSGQFUL:
                    return "Used to indicate a resource Error within the MC. This may be interpreted as the maximum number of messages addressed to a single destination or a global maximum of undelivered messages within the MC.";
                    break;
                case ESME_RINVSERTYP:
                    return "Service type is rejected either because it is not recognised by the MC or because its length is not within the defined range.";
                    break;
                case ESME_RINVNUMDESTS:
                    return "The number_of_dests field in the submit_multi PDU is invalid.";
                    break;
                case ESME_RINVDLNAME:
                    return "The dl_name field specified in the submit_multi PDU is either invalid, or non-existent.";
                    break;
                case ESME_RINVDESTFLAG:
                    return "The dest_flag field in the submit_multi PDU has been encoded with an invalid setting.";
                    break;
                case ESME_RINVSUBREP:
                    return "This can typically occur with submit_multi where the context of \"replace if present\" is often a best effort operation and MCs may not support the feature in submit_multi. Another reason for returning this Error would be where the feature has been denied to an ESME.";
                    break;
                case ESME_RINVESMCLASS:
                    return "The esm_class field has an unsupported setting.";
                    break;
                case ESME_RCNTSUBDL:
                    return "Distribution lists are not supported, are denied or unavailable.";
                    break;
                case ESME_RSUBMITFAIL:
                    return "Generic failure Error for submission operations.";
                    break;
                case ESME_RINVSRCTON:
                    return "The source TON of the message is either invalid or unsupported.";
                    break;
                case ESME_RINVSRCNPI:
                    return "The source NPI of the message is either invalid or unsupported.";
                    break;
                case ESME_RINVDSTTON:
                    return "The destination TON of the message is either invalid or unsupported.";
                    break;
                case ESME_RINVDSTNPI:
                    return "The destination NPI of the message is either invalid or unsupported.";
                    break;
                case ESME_RINVSYSTYP:
                    return "The System type of bind PDU has an incorrect length or contains illegal characters.";
                    break;
                case ESME_RINVREPFLAG:
                    return "The replace_if_present flag has been encoded with an invalid or unsupported setting.";
                    break;
                case ESME_RINVNUMMSGS:
                    return "Invalid number of messages.";
                    break;
                case ESME_RTHROTTLED:
                    return "This type of Error is usually returned where an ESME has exceeded a predefined messaging rate restriction applied by the operator.";
                    break;
                case ESME_RINVSCHED:
                    return "Scheduled delivery time is either the incorrect length or is invalid.";
                    break;
                case ESME_RINVEXPIRY:
                    return "Expiry time is either the incorrect length or is invalid.";
                    break;
                case ESME_RINVDFTMSGID:
                    return "The default (pre-defined) message id is either invalid or refers to a non-existent pre-defined message.";
                    break;
                case ESME_RX_T_APPN:
                    return "Rx or Trx ESME is unable to process a delivery due to a temporary problem and is requesting that the message be retried at some future point.";
                    break;
                case ESME_RX_P_APPN:
                    return "Rx or Trx ESME is unable to process a delivery due to a permanent problem relating to the given destination address and is requesting that the message and all other messages queued to the same destination should NOT be retried any further.";
                    break;
                case ESME_RX_R_APPN:
                    return "Rx or Trx ESME is unable to process a delivery due to a problem relating to the given message and is requesting that the message is rejected and not retried. This does not affect other messages queued for the same ESME or destination address.";
                    break;
                case ESME_RQUERYFAIL:
                    return "Generic failure scenario for a query request.";
                    break;
                case ESME_RINVTLVSTREAM:
                    return "Decoding of TLVs (Optional Parameters) has resulted in one of the following scenarios:\n\t1. PDU decoding completed with 1- 3 octets of data remaining, indicating a corrupt PDU.\n\t2. A TLV indicated a length that was not present in the remaining PDU data (e.g. a TLV specifying a length of 10 where only 6 octets of PDU data remain).";
                    break;
                case ESME_RTLVNOTALLWD:
                    return "A TLV has been used in an invalid context, either inappropriate or deliberately rejected by the operator.";
                    break;
                case ESME_RINVTLVLEN:
                    return "A TLV has specified a length that is considered invalid.";
                    break;
                case ESME_RMISSINGTLV:
                    return "A mandatory TLV such as the message_payload TLV within a data_sm PDU is missing.";
                    break;
                case ESME_RINVTLVVAL:
                    return "The data content of a TLV is invalid and cannot be decoded.";
                    break;
                case ESME_RDELIVERYFAILURE:
                    return "A data_sm or submit_sm operation issued in transaction mode has resulted in a failed delivery.";
                    break;
                case ESME_RUNKNOWNERR:
                    return "Some unexpected Error has occurred.";
                    break;
                case ESME_RSERTYPUNAUTH:
                    return "Specific service_type has been denied for use by the given ESME.";
                    break;
                case ESME_RPROHIBITED:
                    return "The PDU request was recognised but is denied to the ESME.";
                    break;
                case ESME_RSERTYPUNAVAIL:
                    return "Due to a service outage within the MC, a service is unavailable.";
                    break;
                case ESME_RSERTYPDENIED:
                    return "Due to inappropriate message content wrt. the selected service_type.";
                    break;
                case ESME_RINVDCS:
                    return "Specified DCS is invalid or MC does not support it.";
                    break;
                case ESME_RINVSRCADDRSUBUNIT:
                    return "Source Address Sub unit is Invalid.";
                    break;
                case ESME_RINVDSTADDRSUBUNIT:
                    return "Destination Address Sub unit is Invalid.";
                    break;
                case ESME_RINVBCASTFREQINT:
                    return "Specified value is either invalid or not supported.";
                    break;
                case ESME_RINVBCASTALIAS_NAME:
                    return "Specified value has an incorrect length or contains invalid/unsupported characters.";
                    break;
                case ESME_RINVBCASTAREAFMT:
                    return "Specified value violates protocol or is unsupported.";
                    break;
                case ESME_RINVNUMBCAST_AREAS:
                    return "Specified value violates protocol or is unsupported.";
                    break;
                case ESME_RINVBCASTCNTTYPE:
                    return "Specified value violates protocol or is unsupported.";
                    break;
                case ESME_RINVBCASTMSGCLASS:
                    return "Specified value violates protocol or is unsupported.";
                    break;
                case ESME_RBCASTFAIL:
                    return "broadcast_sm operation failed.";
                    break;
                case ESME_RBCASTQUERYFAIL:
                    return "query_broadcast_sm operation failed.";
                    break;
                case ESME_RBCASTCANCELFAIL:
                    return "cancel_broadcast_sm operation failed.";
                    break;
                case ESME_RINVBCAST_REP:
                    return "Specified value violates protocol or is unsupported.";
                    break;
                case ESME_RINVBCASTSRVGRP:
                    return "Specified value violates protocol or is unsupported.";
                    break;
                case ESME_RINVBCASTCHANIND:
                    return "Specified value violates protocol or is unsupported.";
                    break;
            }
            if(p >= 0x00000400 && p <= 0x000004FF)
                return "Reserved for MC vendor specific Errors";

            return "Reserved by SMPP";
        }

		CommandStatus():val_(0){}
        CommandStatus(const Smpp::Uint32& p) : val_(p) {}
        operator Smpp::Uint32() const { return val_; }

        Smpp::Uint32 decode(const Smpp::Char* buff) {
            Smpp::Uint32 status;
            memcpy(&status, &buff[8], sizeof status);
            return Smpp::ntoh32(status);
        }
    };

} // namespace Smpp

#endif
