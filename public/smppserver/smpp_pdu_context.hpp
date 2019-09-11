#ifndef SMPP_PDU_CONTEXT_H
#define SMPP_PDU_CONTEXT_H
#include "smpp.hpp"
enum SMPP_SESSION_STATE;
/*
SMPP Protocol Specification v3.4

2.3 SMPP PDUs
The following table lists the SMPP PDU set and the context in which each PDU may be used:

*/
class smpp_pdu_context
{
public:
	static bool is_state_bound(SMPP_SESSION_STATE current_state)
	{
		if((current_state == SESSION_BOUND_RX) || (current_state == SESSION_BOUND_TX) || (current_state == SESSION_BOUND_TRX))
		{
			return true;
		}
		return false;
	}
	//
	static bool is_state_bound_rxable(SMPP_SESSION_STATE current_state)
	{
		if((current_state == SESSION_BOUND_RX)  || (current_state == SESSION_BOUND_TRX))
		{
			return true;
		}
		return false;
	}
	//
	static bool is_state_bound_txable(SMPP_SESSION_STATE current_state)
	{
		if( (current_state == SESSION_BOUND_TX) || (current_state == SESSION_BOUND_TRX))
		{
			return true;
		}
		return false;
	}
	static bool valid( SMPP_SESSION_STATE current_state, Smpp::CommandId& commandid)
	{
		switch((Smpp::Uint32)commandid)
		{
			case Smpp::CommandId::BindReceiver:
			case Smpp::CommandId::BindTransceiver:
			case Smpp::CommandId::BindTransmitter:
			{
				return (current_state == SESSION_OPEN);
			}
			case Smpp::CommandId::Unbind:
			case Smpp::CommandId::UnbindResp:
			case Smpp::CommandId::GenericNack:
			case Smpp::CommandId::DeliverSmResp:
			{
				return is_state_bound(current_state);
			}
			case Smpp::CommandId::SubmitMulti:
			case Smpp::CommandId::SubmitSm:
			case Smpp::CommandId::DataSm:
			case Smpp::CommandId::DataSmResp:
			{
				return is_state_bound_txable(current_state);
			}
			case Smpp::CommandId::QuerySm:
			{
				return is_state_bound_rxable(current_state);
			}
			case Smpp::CommandId::CancelSm:
			case Smpp::CommandId::ReplaceSm:
			{
				return is_state_bound(current_state);
			}
			case Smpp::CommandId::AlertNotification:
			{
				return is_state_bound_rxable(current_state);
			}
			case Smpp::CommandId::EnquireLink:
			case Smpp::CommandId::EnquireLinkResp:
			{
				return (is_state_bound(current_state) || (current_state == SESSION_OPEN));
			}
			default:
			{
				return false;
			}
		}
		return false;
	}
};
#endif