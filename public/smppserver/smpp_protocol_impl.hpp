#ifndef SMPP_IMPL_H
#define SMPP_IMPL_H
#include <queue>
#include <boost/thread/mutex.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "smpp.hpp"
#include "memory_buffer.hpp"
#include "smpp_timer.hpp"
#include "abstract_protocol.hpp"
#include "vendor_runtime.h"

#define CB_SMPP_SERVER_VERSION "0.1"
/*
SMPP Protocol Specification v3.4

the definition of SMPP SESSION:

SMPP Session Description
An SMPP session between an SMSC and an ESME is initiated by the ESME first establishing
a network connection with the SMSC and then issuing an SMPP Bind request to open an SMPP
session. An ESME wishing to submit and receive messages is required to establish two network
connections (TCP/IP or X.25) and two SMPP sessions (Transmitter and Receiver).
Alternatively, in this version of the protocol an ESME may establish an SMPP Transceiver
session over a single network connection.
During an SMPP session, an ESME may issue a series of requests to an SMSC and shall receive
the appropriate responses to each request, from the SMSC. Likewise, the SMSC may issue
SMPP requests to the ESME, which must respond accordingly.
The SMPP session may be defined in terms of the following possible states:

OPEN (Connected and Bind Pending)
An ESME has established a network connection to the SMSC but has not yet issued a
Bind request.

• BOUND_TX
A connected ESME has requested to bind as an ESME Transmitter (by issuing a
bind_transmitter PDU) and has received a response from the SMSC authorising its
bind request.
An ESME bound as a transmitter may send short messages to an SMSC for onward
delivery to a Mobile Station or to another ESME. The ESME may also replace, query
or cancel a previously submitted short message.

• BOUND_RX
A connected ESME has requested to bind as an ESME Receiver (by issuing a
bind_receiver PDU) and has received a response from the SMSC authorising its Bind
request.
An ESME bound as a receiver may receive short messages from an SMSC which may
be originated by a mobile station, by another ESME or by the SMSC itself (for example
an SMSC delivery receipt).

• BOUND_TRX
A connected ESME has requested to bind as an ESME Transceiver (by issuing abind_transceiver PDU) and has received a 
response from the SMSC authorising its
Bind request. An ESME bound as a Transceiver supports the complete set of operations
supported by a Transmitter ESME and a Receiver ESME.
Thus an ESME bound as a transceiver may send short messages to an SMSC for onward
delivery to a Mobile Station or to another ESME. The ESME may also receive short
messages from an SMSC which may be originated by a mobile station, by another
ESME or by the SMSC itself (for example an SMSC delivery receipt).

CLOSED (Unbound and Disconnected)
An ESME has unbound from the SMSC and has closed the network connection. The
SMSC may also unbind from the ESME.
*/

enum SMPP_SESSION_STATE
{
	SESSION_OPEN = 0,
	SESSION_BOUND_TX = 1,
	SESSION_BOUND_RX = 2,
	SESSION_BOUND_TRX = 3,
	SESSION_CLOSED = 4
};
class smpp_impl:public abstract_protocol
{
public:
	smpp_impl();
	~smpp_impl();
public:
	unsigned int SMPP_HEADER_LENGTH;
	std::string SMPP_SERVER_SYSID;
	int OTHER_ERROR_TOLERANCE;
	int BIND_ERROR_TOLERANCE;
protected:
	/*
	 if the sequence number of a pdu received, we set error_occured_to true, because this is a serious.
	 if the bind failed times is more then BIND_ERROR_TOLERANCE, set error_occured_ to true
	 if other failed times is more the OTHER_ERROR_TOLERANCE, set error_occured_ to true
	 
	 if error_occured_ is true, it means this connection should be closed, and the smpp_impl should not
	 process any operations
	 */
	int bind_error_count_ ;
	int other_error_count_;
	
	void other_error_occur();
	void bind_error_occur();
	
	int total_submit_;
	int total_qurey_;
	int total_notify_;
	
	boost::posix_time::ptime bind_time_;
	boost::posix_time::ptime unbind_time_;
	std::vector<boost::posix_time::ptime> submit_times_;
	std::vector<boost::posix_time::ptime> query_times_;
protected:

	smpp_timer_manager timer_manager_;
	SMPP_SESSION_STATE current_session_state_;
	std::string sysid_;
	bool is_bound(){return ((current_session_state_ != SESSION_CLOSED) && (current_session_state_ != SESSION_OPEN));}
	std::string convert_to_utf8(int offset, const Smpp::ShortMessage& sm, Smpp::DataCoding::DataCodingType dc);
protected:
	/*
	 As description in SMPP V3.4 , chapter 4.3.1
	 
	 This is a generic negative acknowledgment to an SMPP PDU submitted with an invalid message header. 
	 A generic_nack response is returned in the following cases:
	 •	Invalid command_length If the receiving SMPP entity, on decoding an SMPP PDU, detects an invalid 
	 command_length (either too short or too long), it should assume that the data is corrupt. In such 
	 cases a generic_nack PDU must be returned to the message originator.
	 •	Unknown command_id If an unknown or invalid command_id is received, a generic_nack PDU must also 
	 be returned to the originator.
	 
	 It comprises the SMPP message header only.
	 */
	void generate_and_send_nack(const Smpp::CommandStatus& status, const Smpp::SequenceNumber& sn);
protected:

	
	void process_bind_transmitter(const Smpp::BindTransmitter& transmitter);
	void process_bind_transceiver(const Smpp::BindTransceiver& transceiver);
	void process_bind_receiver(const Smpp::BindReceiver& receiver);
	
	void process_submitsm(const Smpp::SubmitSm& submit_sm);
	
	void process_unbind(const Smpp::Unbind& unbind);
	
	void process_enquirelink(const Smpp::SequenceNumber& sn);
	
	bool send_delivery_sm();
	bool send_mo();
	
	Smpp::SequenceNumber  generate_sequence_number();
	
	//SMPP spec V3.4 - 2.3 
	//The following table lists the SMPP PDU set and the context in which each PDU may be used:
	bool check_pdu_context_valid(Smpp::CommandId& commandid);
	
	bool sequencenumber_valid_no_lock(memory_buffer_ptr& bp);

	std::string format_to_AppendixB_time(time_t t);
public:
	//if return false, it means error occurs
	virtual void process(memory_buffer_ptr& bp);
	
	virtual bool timer_timeout();
	virtual bool need_send(); //overload to process the delivery to the caller
	virtual bool need_process();
	virtual bool get_from_out_queue(memory_buffer_ptr& bp);
	void placeholder_function(const std::string& msg, bool exit)
	{
		logger_log(__FUNCTION__, LEVEL_WARNING, " %s ", msg.c_str());
		assert(!exit);
	}
	
	int get_total_submit(){return total_submit_;}
	int get_total_query(){return total_qurey_;}
	int get_total_notify(){return total_notify_;}
	int clear_resource();
	
	virtual size_t get_header_length(){return smpp_impl::SMPP_HEADER_LENGTH;}
	virtual size_t get_total_length_from_header(memory_buffer_ptr& bp){return Smpp::get_command_length(bp.get()->data());}
	
};
#endif
