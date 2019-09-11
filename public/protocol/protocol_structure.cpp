#include "protocol_structure.h"
#include "base64.h"
#include "data_structure.h"

std::string protocol_strings::REQUEST = "request";
std::string protocol_strings::RESPONSE = "response";
std::string protocol_strings::COMMAND = "command";
std::string protocol_strings::COMPANY_ACCOUNT_LIST = "company_account_list";
std::string protocol_strings::SUBACCOUNT_LIST = "subaccount_list";
std::string protocol_strings::ROUTE_LIST = "route_list";
std::string protocol_strings::TEMPLATE_LIST = "template_list";
std::string protocol_strings::SIGNATURE_LIST = "signature_list";
std::string protocol_strings::FEERATE_LIST = "feerate_list";
std::string protocol_strings::PROVIDER_ACCOUNT_LIST = "provider_account_list";
std::string protocol_strings::ROUTE_SIGNATURE_LIST = "route_signature_list";
std::string protocol_strings::SUBACCOUNT_SIGNATURE_LIST = "subaccount_signature_list";
std::string protocol_strings::ROUTE_FEERATE_LIST = "route_feerate_list";
std::string protocol_strings::SUBACCOUNT_FEERATE_LIST = "subaccount_feerate_list";
std::string protocol_strings::SUBACCOUNT_CHARGE_LIST = "subaccount_charge_list";
std::string protocol_strings::ROUTE_CHARGE_LIST = "route_charge_list";
std::string protocol_strings::COMPANY_ACCOUNT_BALANCE_LIST = "company_account_balance_list";
std::string protocol_strings::ROUTE_BALANCE_LIST = "route_balance_list";
std::string protocol_strings::PROVIDER_ACCOUNT_BALANCE_LIST = "provider_account_balance_list";
std::string protocol_strings::SPECIFIED_NUMBER_LIST = "specified_number_list";
std::string protocol_strings::SENSITIVE_WORD_LIST = "sensitive_word_list";
std::string protocol_strings::BLACK_NUMBER_LIST = "black_number_list";
std::string protocol_strings::HIGH_FREQUENCY_WHITE_NUMBER_LSIT = "high_frequency_white_number_list";
std::string protocol_strings::SUBACCOUNT_ROUTE_LIST = "subaccount_route_list";
std::string protocol_strings::ID = "id";
std::string protocol_strings::PROVIDER_ACCOUNT_ID = "provider_account_id";
std::string protocol_strings::USERNAME = "username";
std::string protocol_strings::PASSWORD = "password";
std::string protocol_strings::STATUS = "status";
std::string protocol_strings::STATE = "state";
std::string protocol_strings::NAME = "name";
std::string protocol_strings::DESCRIPTION = "description";
std::string protocol_strings::COMPANY_NAME = "company_name";
std::string protocol_strings::BALANCE = "balance";
std::string protocol_strings::BALANCE_MONEY = "balance_money";
std::string protocol_strings::BALANCE_COUNT = "balance_count";
std::string protocol_strings::CREDIT_LINE = "credit_line";
std::string protocol_strings::CREDIT_LINE_MONEY = "credit_line_money";
std::string protocol_strings::CREDIT_LINE_COUNT = "credit_line_count";
std::string protocol_strings::THRESHOLD_VALUE = "threshold_value";
std::string protocol_strings::THRESHOLD_VALUE_MONEY = "threshold_value_money";
std::string protocol_strings::THRESHOLD_VALUE_COUNT = "threshold_value_count";
std::string protocol_strings::DAILY_CONSUME = "daily_consume";
std::string protocol_strings::MONTHLY_CONSUME = "monthly_consume";
std::string protocol_strings::TYPE = "type";
std::string protocol_strings::FUNCTION_TYPE = "function_type";
std::string protocol_strings::SUBACCOUNT_COUNT = "subaccount_count";
std::string protocol_strings::SETUP_TIME = "setup_time";
std::string protocol_strings::EFFECTIVE_TIME = "effetive_time";
std::string protocol_strings::MANAGER = "manager";
std::string protocol_strings::COMPANY_ACCOUNT_USERNAME = "company_account_username";
std::string protocol_strings::SUBACCOUNT_USERNAME = "subaccount_username";
std::string protocol_strings::IP_ADDRESS = "ip_address";
std::string protocol_strings::PORT = "port";
std::string protocol_strings::SENDNUMBER_TYPE = "send_number_type";
std::string protocol_strings::SEND_NUMBER = "send_number";
std::string protocol_strings::RECEIVELIMIT_TYPE = "receive_limit_type";
std::string protocol_strings::RECEIVELIMIT_NETCODE = "receive_limit_netcode";
std::string protocol_strings::PROTOCOL_TYPE = "protocol_type";
std::string protocol_strings::INDUSTRY_TYPE = "industry_type";
std::string protocol_strings::INDUSTRY = "industry";
std::string protocol_strings::ENCODE_TYPE = "encode_type";
std::string protocol_strings::ENCODE_SIZE = "encode_size";
std::string protocol_strings::ASCALL_SIZE = "ascall_size";
std::string protocol_strings::UCS2_SIZE = "ucs2_size";
std::string protocol_strings::TEMPLATES = "templates";
std::string protocol_strings::SIGNATURE_TYPE = "signature_type";
std::string protocol_strings::SIGNATURE_POSITION = "signature_position";
std::string protocol_strings::CONTENT = "content";
std::string protocol_strings::SIGNATURE = "signature";
std::string protocol_strings::INTERVAL_TIME = "interval_time";
std::string protocol_strings::TIMES = "times";
std::string protocol_strings::CYCLE = "cycle";
std::string protocol_strings::RECEIVE_ACK_MAX_TIME = "reveive_ack_max_time";
std::string protocol_strings::RATIO = "ratio";
std::string protocol_strings::DISABLE_CONNECT_MAX_TIME = "disable_connect_max_time";
std::string protocol_strings::PACKAGE_STATE = "package_state";
std::string protocol_strings::PACKAGE_COUNT = "package_count";
std::string protocol_strings::PACKAGE_USED_COUNT = "package_used_count";
std::string protocol_strings::PACKAGE_SURPLUS_COUNT = "package_surplus_count";
std::string protocol_strings::HEARTBEAT = "heartbeat";
std::string protocol_strings::BELONGINGNESS = "belongingness";
std::string protocol_strings::MCC = "mcc";
std::string protocol_strings::MNC = "mnc";
std::string protocol_strings::MT_PRICE = "mt_price";
std::string protocol_strings::MO_PRICE = "mo_price";
std::string protocol_strings::CHARGE_MODE = "charge_mode";
std::string protocol_strings::ROUTE_PRIORITY = "route_priority";
std::string protocol_strings::INDEX = "index";
std::string protocol_strings::INDEX_VALUE = "index_value";
std::string protocol_strings::LINKMAN = "linkman";
std::string protocol_strings::E_MAIL = "e_mail";
std::string protocol_strings::PHONE_NUMBER = "phone_number";
std::string protocol_strings::MASTER_ROUTE_ID = "master_route_id";
std::string protocol_strings::SLAVE_ROUTE_ID = "slave_route_id";
std::string protocol_strings::SECOND_SLAVE_ROUTE_ID = "second_slave_route_id";
std::string protocol_strings::ROUTE_ID = "route_id";
std::string protocol_strings::NUMBER = "number";
std::string protocol_strings::FEE = "fee";
std::string protocol_strings::WHITE_LIST_STATE = "white_list_state";
std::string protocol_strings::DEVOUR_RATE = "devour_rate";
std::string protocol_strings::ALREADY_SEND_COUNT = "already_send_count";
std::string protocol_strings::MASTER_ROUTE_RATIO = "master_route_ratio";
std::string protocol_strings::SLAVE_ROUTE_RATIO = "slave_route_ratio";
std::string protocol_strings::SECOND_SLAVE_ROUTE_RATIO = "second_slave_route_ratio";
std::string protocol_strings::COUNT = "count";
std::string protocol_strings::MONEY = "money";
std::string protocol_strings::BALANCE_ADDRESS = "balance_address";
std::string protocol_strings::RECHARGE_MODE = "recharge_mode";
std::string protocol_strings::MAX_COUNT_PER_SECOND = "max_count_per_second";
std::string protocol_strings::STATISTICS_BELONGINGNESS = "statistics_belongingness";
std::string protocol_strings::ACCESS_LEVEL = "access_level";
std::string protocol_strings::LOGIN_STATE = "login_state";
std::string protocol_strings::CREATOR = "creator";
std::string protocol_strings::SPECIFIED_NUMBER = "specified_number";
std::string protocol_strings::SPECIFIED_NUMBER_TYPE = "specified_number_type";
std::string protocol_strings::SPLIT_STATE = "split_state";
std::string protocol_strings::BASE_INFO = "base_info";
std::string protocol_strings::LINK_INFO = "link_info";
std::string protocol_strings::DEVOUR_THRESHOLD = "devour_threshold";
std::string protocol_strings::VERIFY_MODE = "verify_mode";
std::string protocol_strings::HIGH_FREQUENCY_THRESHOLD = "high_frequency_threshold";

std::string protocol_commands::LOGIN = "login";
std::string protocol_commands::LOGOUT = "logout";

std::string protocol_commands::GET_ALL_LIST = "get_all_list";
std::string protocol_commands::GET_COMACCOUNT = "get_comaccount";
std::string protocol_commands::GET_COMACCOUNT_LIST = "get_comaccount_list";
std::string protocol_commands::ADD_COMACCOUNT = "add_comaccount";
std::string protocol_commands::EDIT_COMACCOUNT = "edit_comaccount";

std::string protocol_commands::GET_SUBACCOUNT = "get_subaccount";
std::string protocol_commands::GET_SUBACCOUNT_LIST = "get_subaccount_list";
std::string protocol_commands::GET_SUBACCOUNT_BASE_INFORMATION_LIST = "get_subaccount_base_information_list";
std::string protocol_commands::GET_SUBACCOUNT_ROUTE_LIST = "get_subaccount_route_list";
std::string protocol_commands::ADD_SUBACCOUNT = "add_subaccount";
std::string protocol_commands::DELETE_SUBACCOUNT = "delete_subaccount";

std::string protocol_commands::ADD_ROUTE = "add_route";
std::string protocol_commands::EDIT_ROUTE = "edit_route";
std::string protocol_commands::DELETE_ROUTE = "delete_route";
std::string protocol_commands::GET_ROUTE_LIST = "get_route_list";
std::string protocol_commands::CHOOSE_ROUTE_LIST = "choose_route_list";

std::string protocol_commands::ADD_TEMPLATE = "add_template";
std::string protocol_commands::GET_TEMPLATE_LIST = "get_template_list";
std::string protocol_commands::SET_TEMPLATE_LIST = "set_template_list";

std::string protocol_commands::ADD_SIGNATURE = "add_signature";
std::string protocol_commands::GET_SIGNATURE_LIST = "get_signature_list";
std::string protocol_commands::SET_SIGNATURE_LIST = "set_signature_list";

std::string protocol_commands::GET_PROACCOUNT_LIST = "get_proaccount_list";
std::string protocol_commands::ADD_PROACCOUNT = "add_proaccount";

std::string protocol_commands::GET_FEERATE_LIST = "get_feerate_list";
std::string protocol_commands::ADD_FEERATE = "add_feerate";

std::string protocol_commands::GET_SPECIFIED_NUMBER_LIST = "get_specified_number_list";
std::string protocol_commands::ADD_SPECIFIED_NUMBER = "add_specified_number";

std::string protocol_commands::ADD_SENSITIVE_WORD = "add_sensitive_word";
std::string protocol_commands::GET_SENSITIVE_WORD_LIST = "get_sensitive_word_list";

std::string protocol_commands::ADD_BLACK_NUMBER = "add_black_number";
std::string protocol_commands::GET_BLACK_NUMBER_LIST = "get_black_number_list";

std::string protocol_commands::ADD_HIGH_FREQUENCY_WHITE_NUMBER = "add_high_frequency_white_number";
std::string protocol_commands::GET_HIGH_FREQUENCY_WHITE_NUMBER_LIST = "get_high_frequency_white_number_list";

std::string protocol_commands::DEDUCT_SUBACCOUNT_CHARGE = "deduct_subaccount_charge";
std::string protocol_commands::DEDUCT_ROUTE_CHARGE = "deduct_route_charge";


bool parse_command(const std::string& json, std::string& command)
{
	UNPACK_JSON_BEGIN
	pt_protocol = pt_root.get_child(protocol_strings::REQUEST);
	command = pt_protocol.get<std::string>(protocol_strings::COMMAND);
	UNPACK_JSON_END
}

void login_user::pack(boost::property_tree::ptree& pt)
{
	pt.add(protocol_strings::USERNAME, username);
	pt.add(protocol_strings::PASSWORD, password);
	pt.add(protocol_strings::NAME, name);
	pt.add(protocol_strings::ACCESS_LEVEL, access_level);
	pt.add(protocol_strings::LOGIN_STATE, login_state);
	pt.add(protocol_strings::CREATOR, creator);
	pt.add(protocol_strings::SETUP_TIME, setup_time);
}

void login_user::unpack(boost::property_tree::ptree& pt)
{
	username = pt.get<std::string>(protocol_strings::USERNAME);
	password = pt.get<std::string>(protocol_strings::PASSWORD);
	name = pt.get<std::string>(protocol_strings::NAME);
	access_level = (ACCESS_LEVEL)pt.get<int>(protocol_strings::ACCESS_LEVEL);
	login_state = (LOGIN_STATE)pt.get<int>(protocol_strings::LOGIN_STATE);
	creator = pt.get<std::string>(protocol_strings::CREATOR);
	setup_time = pt.get<boost::uint64_t>(protocol_strings::SETUP_TIME);
}

void login::pack(boost::property_tree::ptree& pt)
{
	pt.add(protocol_strings::USERNAME, username);
	pt.add(protocol_strings::PASSWORD, password);
}

void login::unpack(boost::property_tree::ptree& pt)
{
	username = pt.get<std::string>(protocol_strings::USERNAME);
	password = pt.get<std::string>(protocol_strings::PASSWORD);
}

bool login_req::pack(std::string& json)
{
	PACK_JSON_BEGIN
	pt_protocol.add(protocol_strings::COMMAND, protocol_commands::LOGIN);
	login_.pack(pt_protocol);
	pt_root.add_child(protocol_strings::REQUEST, pt_protocol);
	PACK_JSON_END
}

bool login_req::unpack(const std::string& json)
{
	UNPACK_JSON_BEGIN
	pt_protocol = pt_root.get_child(protocol_strings::REQUEST);
	login_.unpack(pt_protocol);
	UNPACK_JSON_END
}

bool login_resp::pack(std::string& json)
{
	PACK_JSON_BEGIN
	pt_protocol.add(protocol_strings::STATUS, (int)status);
	login_user_.pack(pt_protocol);
	pt_root.add_child(protocol_strings::RESPONSE, pt_protocol);
	PACK_JSON_END
}

bool login_resp::unpack(const std::string& json)
{
	UNPACK_JSON_BEGIN
	pt_protocol = pt_root.get_child(protocol_strings::RESPONSE);
	status = (PROTOCOL_RESP_STATUS)pt_protocol.get<int>(protocol_strings::STATUS);
	login_user_.unpack(pt_protocol);
	UNPACK_JSON_END
}

bool logout_req::pack(std::string& json)
{
	PACK_JSON_BEGIN
	pt_protocol.add(protocol_strings::COMMAND, protocol_commands::LOGOUT);
	login_.pack(pt_protocol);
	pt_root.add_child(protocol_strings::REQUEST, pt_protocol);
	PACK_JSON_END
}

bool logout_req::unpack(const std::string& json)
{
	UNPACK_JSON_BEGIN
	pt_protocol = pt_root.get_child(protocol_strings::REQUEST);
	login_.unpack(pt_protocol);
	UNPACK_JSON_END
}

bool logout_resp::pack(std::string& json)
{
	PACK_JSON_BEGIN
	pt_protocol.add(protocol_strings::STATUS, (int)status);
	pt_root.add_child(protocol_strings::RESPONSE, pt_protocol);
	PACK_JSON_END
}

bool logout_resp::unpack(const std::string& json)
{
	UNPACK_JSON_BEGIN
	pt_protocol = pt_root.get_child(protocol_strings::RESPONSE);
	status = (PROTOCOL_RESP_STATUS)pt_protocol.get<int>(protocol_strings::STATUS);
	UNPACK_JSON_END
}

void company_account::pack(boost::property_tree::ptree& pt)
{
	pt.add(protocol_strings::USERNAME, username);
	//pt.add(protocol_strings::COMPANY_NAME, company_name);
	std::string company_name_base64 = base64_encode((const unsigned char *)company_name.c_str(),(unsigned int)company_name.length());
	pt.add(protocol_strings::COMPANY_NAME, company_name_base64);
	pt.add(protocol_strings::PASSWORD, password);
	pt.add(protocol_strings::DESCRIPTION, description);

	pt.add(protocol_strings::STATE, state);
	pt.add(protocol_strings::BALANCE_MONEY, balance_money);
	pt.add(protocol_strings::CREDIT_LINE_MONEY, credit_line_money);
	pt.add(protocol_strings::DAILY_CONSUME, daily_consume);

	pt.add(protocol_strings::SETUP_TIME, setup_time);
	pt.add(protocol_strings::EFFECTIVE_TIME, effective_time);

	//pt.add(protocol_strings::LINKMAN,linkman);
	std::string linkman_base64 = base64_encode((const unsigned char *)linkman.c_str(),(unsigned int)linkman.length());
	pt.add(protocol_strings::LINKMAN, linkman_base64);
	pt.add(protocol_strings::PHONE_NUMBER,phone_number);
	pt.add(protocol_strings::E_MAIL,e_mail);
}

void company_account::unpack(boost::property_tree::ptree& pt)
{
	username = pt.get<std::string>(protocol_strings::USERNAME);
	//company_name = pt.get<std::string>(protocol_strings::COMPANY_NAME);
	std::string company_name_base64 = pt.get<std::string>(protocol_strings::COMPANY_NAME);
	std::vector<unsigned char> v_name = base64_decode(company_name_base64);
	company_name = std::string(v_name.begin(), v_name.end());
	password = pt.get<std::string>(protocol_strings::PASSWORD);
	description = pt.get<std::string>(protocol_strings::DESCRIPTION);

	state = (ACCOUNT_STATE)pt.get<int>(protocol_strings::STATE);
	balance_money = pt.get<double>(protocol_strings::BALANCE_MONEY);
	credit_line_money = pt.get<double>(protocol_strings::CREDIT_LINE_MONEY);
	daily_consume = pt.get<double>(protocol_strings::DAILY_CONSUME);

	setup_time = pt.get<boost::uint64_t>(protocol_strings::SETUP_TIME);
	effective_time = pt.get<boost::uint64_t>(protocol_strings::EFFECTIVE_TIME);

	//linkman = pt.get<std::string>(protocol_strings::LINKMAN);
	std::string linkman_base64 = pt.get<std::string>(protocol_strings::LINKMAN);
	v_name = base64_decode(linkman_base64);
	linkman = std::string(v_name.begin(), v_name.end());
	phone_number = pt.get<std::string>(protocol_strings::PHONE_NUMBER);
	e_mail = pt.get<std::string>(protocol_strings::E_MAIL);
}

bool add_company_account_req::pack(std::string& json)
{
	PACK_JSON_BEGIN
	pt_protocol.add(protocol_strings::COMMAND, protocol_commands::ADD_COMACCOUNT);
	account.pack(pt_protocol);
	pt_root.add_child(protocol_strings::REQUEST, pt_protocol);
	PACK_JSON_END
}

bool add_company_account_req::unpack(const std::string& json)
{
	UNPACK_JSON_BEGIN
	pt_protocol = pt_root.get_child(protocol_strings::REQUEST);
	account.unpack(pt_protocol);
	UNPACK_JSON_END
}

bool add_company_account_resp::pack(std::string& json)
{
	PACK_JSON_BEGIN
	pt_protocol.add(protocol_strings::STATUS, (int)status);
	pt_root.add_child(protocol_strings::RESPONSE, pt_protocol);
	PACK_JSON_END
}

bool add_company_account_resp::unpack(const std::string& json)
{
	UNPACK_JSON_BEGIN
	pt_protocol = pt_root.get_child(protocol_strings::RESPONSE);
	status = (PROTOCOL_RESP_STATUS)pt_protocol.get<int>(protocol_strings::STATUS);
	UNPACK_JSON_END
}

bool get_company_account_req::pack(std::string& json)
{
	PACK_JSON_BEGIN
	pt_protocol.add(protocol_strings::COMMAND, protocol_commands::GET_COMACCOUNT);
	pt_protocol.add(protocol_strings::USERNAME,username);
	pt_root.add_child(protocol_strings::REQUEST, pt_protocol);
	PACK_JSON_END
}

bool get_company_account_req::unpack(const std::string& json)
{
	UNPACK_JSON_BEGIN
	pt_protocol = pt_root.get_child(protocol_strings::REQUEST);
	username = pt_protocol.get<std::string>(protocol_strings::USERNAME);
	UNPACK_JSON_END
}

bool get_company_account_resp::pack(std::string& json)
{
	PACK_JSON_BEGIN
	pt_protocol.add(protocol_strings::STATUS, (int)status);
	account.pack(pt_protocol);
	pt_root.add_child(protocol_strings::RESPONSE, pt_protocol);
	PACK_JSON_END
}

bool get_company_account_resp::unpack(const std::string& json)
{
	UNPACK_JSON_BEGIN
	pt_protocol = pt_root.get_child(protocol_strings::RESPONSE);
	status = (PROTOCOL_RESP_STATUS)pt_protocol.get<int>(protocol_strings::STATUS);
	account.unpack(pt_protocol);
	UNPACK_JSON_END
}

bool edit_company_account_req::pack(std::string& json)
{
	PACK_JSON_BEGIN
	pt_protocol.add(protocol_strings::COMMAND, protocol_commands::EDIT_COMACCOUNT);
	account.pack(pt_protocol);
	pt_root.add_child(protocol_strings::REQUEST, pt_protocol);
	PACK_JSON_END
}

bool edit_company_account_req::unpack(const std::string& json)
{
	UNPACK_JSON_BEGIN
	pt_protocol = pt_root.get_child(protocol_strings::REQUEST);
	account.unpack(pt_protocol);
	UNPACK_JSON_END
}

bool edit_company_account_resp::pack(std::string& json)
{
	PACK_JSON_BEGIN
	pt_protocol.add(protocol_strings::STATUS, (int)status);
	account.pack(pt_protocol);
	pt_root.add_child(protocol_strings::RESPONSE, pt_protocol);
	PACK_JSON_END
}

bool edit_company_account_resp::unpack(const std::string& json)
{
	UNPACK_JSON_BEGIN
	pt_protocol = pt_root.get_child(protocol_strings::RESPONSE);
	status = (PROTOCOL_RESP_STATUS)pt_protocol.get<int>(protocol_strings::STATUS);
	account.unpack(pt_protocol);
	UNPACK_JSON_END
}

bool get_company_account_list_req::pack(std::string& json)
{
	PACK_JSON_BEGIN
	pt_protocol.add(protocol_strings::COMMAND, protocol_commands::GET_COMACCOUNT_LIST);
	pt_root.add_child(protocol_strings::REQUEST, pt_protocol);
	PACK_JSON_END
}

bool get_company_account_list_req::unpack(const std::string& json)
{
	UNPACK_JSON_BEGIN
	pt_protocol = pt_root.get_child(protocol_strings::REQUEST);
	UNPACK_JSON_END
}

bool get_company_account_list_resp::pack(std::string& json)
{
	PACK_JSON_BEGIN
	pt_protocol.add(protocol_strings::STATUS, (int)status);
	boost::property_tree::ptree childen;
	for (size_t i = 0;i < accounts.size();i++)
	{
		boost::property_tree::ptree child;
		accounts.at(i).pack(child);
		childen.push_back(std::make_pair("",child));
	}
	pt_protocol.add_child(protocol_strings::COMPANY_ACCOUNT_LIST, childen);
	pt_root.add_child(protocol_strings::RESPONSE, pt_protocol);
	PACK_JSON_END	
}

bool get_company_account_list_resp::unpack(const std::string& json)
{
	UNPACK_JSON_BEGIN
	pt_protocol = pt_root.get_child(protocol_strings::RESPONSE);
	status = (PROTOCOL_RESP_STATUS)pt_protocol.get<int>(protocol_strings::STATUS);
	boost::property_tree::ptree children = pt_protocol.get_child(protocol_strings::COMPANY_ACCOUNT_LIST);		
	BOOST_FOREACH(boost::property_tree::ptree::value_type &v, children)
	{
		boost::property_tree::ptree p = v.second;
		company_account account;
		account.unpack(p);
		accounts.push_back(account);
	}
	UNPACK_JSON_END
}

void signature::pack(boost::property_tree::ptree& pt)
{
	pt.add(protocol_strings::ID,id);
	pt.add(protocol_strings::INDEX,index);
	pt.add(protocol_strings::SIGNATURE_TYPE, type);
	pt.add(protocol_strings::SIGNATURE_POSITION, position);
	std::string content_base64 = base64_encode((const unsigned char *)content.c_str(),(unsigned int)content.length());
	pt.add(protocol_strings::CONTENT, content_base64);
}

void signature::unpack(boost::property_tree::ptree& pt)
{
	id = pt.get<std::string>(protocol_strings::ID);
	index = pt.get<std::string>(protocol_strings::INDEX);
	type = (SIGNATURE_TYPE)pt.get<int>(protocol_strings::SIGNATURE_TYPE);
	position = (SIGNATURE_POSITION)pt.get<int>(protocol_strings::SIGNATURE_POSITION);
	std::string content_base64 = pt.get<std::string>(protocol_strings::CONTENT);
	std::vector<unsigned char> v_name = base64_decode(content_base64);
	content = std::string(v_name.begin(), v_name.end());
}

bool add_signature_req::pack(std::string& json)
{
	PACK_JSON_BEGIN
	pt_protocol.add(protocol_strings::COMMAND,protocol_commands::ADD_SIGNATURE);
	pt_protocol.add(protocol_strings::BELONGINGNESS,belongingness);
	signature_.pack(pt_protocol);
	pt_root.add_child(protocol_strings::REQUEST,pt_protocol);
	PACK_JSON_END
}

bool add_signature_req::unpack(const std::string& json)
{
	UNPACK_JSON_BEGIN
	pt_protocol = pt_root.get_child(protocol_strings::REQUEST);
	belongingness = (SIGNATURE_BELONGINGNESS)pt_protocol.get<int>(protocol_strings::BELONGINGNESS);
	signature_.unpack(pt_protocol);
	UNPACK_JSON_END
}

bool add_signature_resp::pack(std::string& json)
{
	PACK_JSON_BEGIN
	pt_protocol.add(protocol_strings::STATUS,status);
	pt_root.add_child(protocol_strings::RESPONSE,pt_protocol);
	PACK_JSON_END
}

bool add_signature_resp::unpack(const std::string& json)
{
	UNPACK_JSON_BEGIN
	pt_protocol = pt_root.get_child(protocol_strings::RESPONSE);
	status = (PROTOCOL_RESP_STATUS)pt_protocol.get<int>(protocol_strings::STATUS);
	UNPACK_JSON_END
}

bool get_signature_list_req::pack(std::string& json)
{
	PACK_JSON_BEGIN
	pt_protocol.add(protocol_strings::COMMAND, protocol_commands::GET_SIGNATURE_LIST);
	pt_protocol.add(protocol_strings::BELONGINGNESS,belongingness);
	pt_protocol.add(protocol_strings::INDEX_VALUE,index_value);
	pt_root.add_child(protocol_strings::REQUEST, pt_protocol);
	PACK_JSON_END
}

bool get_signature_list_req::unpack(const std::string& json)
{
	UNPACK_JSON_BEGIN
	pt_protocol = pt_root.get_child(protocol_strings::REQUEST);
	belongingness = (SIGNATURE_BELONGINGNESS)pt_protocol.get<int>(protocol_strings::BELONGINGNESS);
	index_value = pt_protocol.get<std::string>(protocol_strings::INDEX_VALUE);
	UNPACK_JSON_END
}

bool get_signature_list_resp::pack(std::string& json)
{
	PACK_JSON_BEGIN
	pt_protocol.add(protocol_strings::STATUS, (int)status);
	boost::property_tree::ptree childen;
	for (size_t i = 0;i < signatures.size();i++)
	{
		boost::property_tree::ptree child;
		signatures.at(i).pack(child);
		childen.push_back(std::make_pair("",child));
	}
	pt_protocol.add_child(protocol_strings::SIGNATURE_LIST, childen);
	pt_root.add_child(protocol_strings::RESPONSE, pt_protocol);
	PACK_JSON_END	
}

bool get_signature_list_resp::unpack(const std::string& json)
{
	UNPACK_JSON_BEGIN
	pt_protocol = pt_root.get_child(protocol_strings::RESPONSE);
	status = (PROTOCOL_RESP_STATUS)pt_protocol.get<int>(protocol_strings::STATUS);
	boost::property_tree::ptree children = pt_protocol.get_child(protocol_strings::SIGNATURE_LIST);		
	BOOST_FOREACH(boost::property_tree::ptree::value_type &v, children)
	{
		boost::property_tree::ptree p = v.second;
		signature signature_;
		signature_.unpack(p);
		signatures.push_back(signature_);
	}
	UNPACK_JSON_END
}

#if 0
bool set_template_list_req::pack(std::string& json)
{
	PACK_JSON_BEGIN
		pt_protocol.add(protocol_strings::COMMAND, protocol_commands::SET_TEMPLATE_LIST);
	pt_protocol.add(protocol_strings::ID,route_id);
	boost::property_tree::ptree childen;
	for (size_t i = 0;i < templates.size();i++)
	{
		boost::property_tree::ptree child;
		templates.at(i).pack(child);
		childen.push_back(std::make_pair("",child));
	}
	pt_protocol.add_child(protocol_strings::TEMPLATE_LIST, childen);
	pt_root.add_child(protocol_strings::REQUEST, pt_protocol);
	PACK_JSON_END
}

bool set_template_list_req::unpack(const std::string& json)
{
	UNPACK_JSON_BEGIN
		pt_protocol = pt_root.get_child(protocol_strings::REQUEST);
	route_id = pt_protocol.get<std::string>(protocol_strings::ID);
	boost::property_tree::ptree children = pt_protocol.get_child(protocol_strings::TEMPLATE_LIST);		
	BOOST_FOREACH(boost::property_tree::ptree::value_type &v, children)
	{
		boost::property_tree::ptree p = v.second;
		single_template single_template_;
		single_template_.unpack(p);
		templates.push_back(single_template_);
	}
	UNPACK_JSON_END
}

bool set_template_list_resp::pack(std::string& json)
{
	PACK_JSON_BEGIN
		pt_protocol.add(protocol_strings::STATUS, (int)status);
	pt_root.add_child(protocol_strings::RESPONSE, pt_protocol);
	PACK_JSON_END	
}

bool set_template_list_resp::unpack(const std::string& json)
{
	UNPACK_JSON_BEGIN
		pt_protocol = pt_root.get_child(protocol_strings::RESPONSE);
	status = (PROTOCOL_RESP_STATUS)pt_protocol.get<int>(protocol_strings::STATUS);
	UNPACK_JSON_END
}

bool set_signature_list_req::pack(std::string& json)
{
	PACK_JSON_BEGIN
	pt_protocol.add(protocol_strings::COMMAND, protocol_commands::SET_SIGNATURE_LIST);
	pt_protocol.add(protocol_strings::BELONGINGNESS, belongingness);
	//pt_protocol.add(protocol_strings::SUBACCOUNT_USERNAME, subaccount_user);
	//pt_protocol.add(protocol_strings::ID,route_id);
	pt_protocol.add(protocol_strings::INDEX_VALUE,index_value);
	boost::property_tree::ptree childen;
	for (size_t i = 0;i < signatures.size();i++)
	{
		boost::property_tree::ptree child;
		signatures.at(i).pack(child);
		childen.push_back(std::make_pair("",child));
	}
	pt_protocol.add_child(protocol_strings::SIGNATURE_LIST, childen);
	pt_root.add_child(protocol_strings::REQUEST, pt_protocol);
	PACK_JSON_END
}

bool set_signature_list_req::unpack(const std::string& json)
{
	UNPACK_JSON_BEGIN
	pt_protocol = pt_root.get_child(protocol_strings::REQUEST);
	belongingness = (SIGNATURE_BELONGINGNESS)pt_protocol.get<int>(protocol_strings::BELONGINGNESS);
	//subaccount_user = pt_protocol.get<std::string>(protocol_strings::SUBACCOUNT_USERNAME);
	//route_id = pt_protocol.get<std::string>(protocol_strings::ID);
	index_value = pt_protocol.get<std::string>(protocol_strings::INDEX_VALUE);
	boost::property_tree::ptree children = pt_protocol.get_child(protocol_strings::SIGNATURE_LIST);		
	BOOST_FOREACH(boost::property_tree::ptree::value_type &v, children)
	{
		boost::property_tree::ptree p = v.second;
		signature signature_;
		signature_.unpack(p);
		signatures.push_back(signature_);
	}
	UNPACK_JSON_END
}

bool set_signature_list_resp::pack(std::string& json)
{
	PACK_JSON_BEGIN
	pt_protocol.add(protocol_strings::STATUS, (int)status);
	pt_root.add_child(protocol_strings::RESPONSE, pt_protocol);
	PACK_JSON_END	
}

bool set_signature_list_resp::unpack(const std::string& json)
{
	UNPACK_JSON_BEGIN
	pt_protocol = pt_root.get_child(protocol_strings::RESPONSE);
	status = (PROTOCOL_RESP_STATUS)pt_protocol.get<int>(protocol_strings::STATUS);
	UNPACK_JSON_END
}
#endif

void single_template::pack(boost::property_tree::ptree& pt)
{
	pt.add(protocol_strings::ID, id);
	pt.add(protocol_strings::INDEX, index);
	pt.add(protocol_strings::ENCODE_TYPE, code_type);
	std::string content_base64 = base64_encode((const unsigned char *)content.c_str(),(unsigned int)content.length());
	pt.add(protocol_strings::CONTENT, content_base64);
}

void single_template::unpack(boost::property_tree::ptree& pt)
{
	id = pt.get<std::string>(protocol_strings::ID);
	index = pt.get<std::string>(protocol_strings::INDEX);
	code_type = (ENCODE_TYPE)pt.get<int>(protocol_strings::ENCODE_TYPE);
	std::string content_base64 = pt.get<std::string>(protocol_strings::CONTENT);
	std::vector<unsigned char> v_name = base64_decode(content_base64);
	content = std::string(v_name.begin(), v_name.end());
}

bool add_template_req::pack(std::string& json)
{
	PACK_JSON_BEGIN
	pt_protocol.add(protocol_strings::COMMAND,protocol_commands::ADD_TEMPLATE);
	pt_protocol.add(protocol_strings::BELONGINGNESS,belongingness);
	template_.pack(pt_protocol);
	pt_root.add_child(protocol_strings::REQUEST,pt_protocol);
	PACK_JSON_END
}

bool add_template_req::unpack(const std::string& json)
{
	UNPACK_JSON_BEGIN
	pt_protocol = pt_root.get_child(protocol_strings::REQUEST);
	belongingness = (TEMPLATE_BELONGINGNESS)pt_protocol.get<unsigned int>(protocol_strings::BELONGINGNESS);
	template_.unpack(pt_protocol);
	UNPACK_JSON_END
}

bool add_template_resp::pack(std::string& json)
{
	PACK_JSON_BEGIN
	pt_protocol.add(protocol_strings::STATUS,status);
	pt_root.add_child(protocol_strings::RESPONSE,pt_protocol);
	PACK_JSON_END
}

bool add_template_resp::unpack(const std::string& json)
{
	UNPACK_JSON_BEGIN
	pt_protocol = pt_root.get_child(protocol_strings::RESPONSE);
	status = (PROTOCOL_RESP_STATUS)pt_protocol.get<int>(protocol_strings::STATUS);
	UNPACK_JSON_END
}

bool get_template_list_req::pack(std::string& json)
{
	PACK_JSON_BEGIN
	pt_protocol.add(protocol_strings::COMMAND, protocol_commands::GET_TEMPLATE_LIST);
	pt_protocol.add(protocol_strings::BELONGINGNESS,belongingness);
	pt_protocol.add(protocol_strings::INDEX_VALUE,index_value);
	pt_root.add_child(protocol_strings::REQUEST, pt_protocol);
	PACK_JSON_END
}

bool get_template_list_req::unpack(const std::string& json)
{
	UNPACK_JSON_BEGIN
	pt_protocol = pt_root.get_child(protocol_strings::REQUEST);
	belongingness = (TEMPLATE_BELONGINGNESS)pt_protocol.get<unsigned int>(protocol_strings::BELONGINGNESS);
	index_value = pt_protocol.get<std::string>(protocol_strings::INDEX_VALUE);
	UNPACK_JSON_END
}

bool get_template_list_resp::pack(std::string& json)
{
	PACK_JSON_BEGIN
	pt_protocol.add(protocol_strings::STATUS, (int)status);
	boost::property_tree::ptree childen;
	for (size_t i = 0;i < templates.size();i++)
	{
		boost::property_tree::ptree child;
		templates.at(i).pack(child);
		childen.push_back(std::make_pair("",child));
	}
	pt_protocol.add_child(protocol_strings::TEMPLATE_LIST, childen);
	pt_root.add_child(protocol_strings::RESPONSE, pt_protocol);
	PACK_JSON_END	
}

bool get_template_list_resp::unpack(const std::string& json)
{
	UNPACK_JSON_BEGIN
	pt_protocol = pt_root.get_child(protocol_strings::RESPONSE);
	status = (PROTOCOL_RESP_STATUS)pt_protocol.get<int>(protocol_strings::STATUS);
	boost::property_tree::ptree children = pt_protocol.get_child(protocol_strings::TEMPLATE_LIST);		
	BOOST_FOREACH(boost::property_tree::ptree::value_type &v, children)
	{
		boost::property_tree::ptree p = v.second;
		single_template single_template_;
		single_template_.unpack(p);
		templates.push_back(single_template_);
	}
	UNPACK_JSON_END
}

void subaccount_route::pack(boost::property_tree::ptree& pt)
{
	pt.add(protocol_strings::USERNAME,username);
	pt.add(protocol_strings::ROUTE_PRIORITY,route_priority);
	pt.add(protocol_strings::ROUTE_ID,route_id);
	pt.add(protocol_strings::RATIO,ratio);
	//pt.add(protocol_strings::NUMBER,number);
	//pt.add(protocol_strings::SPECIFIED_NUMBER_TYPE,specified_number_type);
	{
		boost::property_tree::ptree child;
		specified_number_.pack(child);
		pt.add_child(protocol_strings::SPECIFIED_NUMBER,child);
	}

	{
		boost::property_tree::ptree child;
		signature_.pack(child);
		pt.add_child(protocol_strings::SIGNATURE,child);
	}
}

void subaccount_route::unpack(boost::property_tree::ptree& pt)
{
	username = pt.get<std::string>(protocol_strings::USERNAME);
	route_priority = (ROUTE_PRIORITY)pt.get<int>(protocol_strings::ROUTE_PRIORITY);
	route_id = pt.get<std::string>(protocol_strings::ROUTE_ID);
	ratio = pt.get<int>(protocol_strings::RATIO);
	//number = pt.get<unsigned int>(protocol_strings::NUMBER);
	//specified_number_type = (SPECIFIED_NUMBER_TYPE)pt.get<unsigned int>(protocol_strings::SPECIFIED_NUMBER_TYPE);
	{
		boost::property_tree::ptree child = pt.get_child(protocol_strings::SPECIFIED_NUMBER);
		specified_number_.unpack(child);
	}

	{
		boost::property_tree::ptree child = pt.get_child(protocol_strings::SIGNATURE);
		signature_.unpack(child);
	}
}

bool get_subaccount_route_list_req::pack(std::string& json)
{
	PACK_JSON_BEGIN
	pt_protocol.add(protocol_strings::COMMAND, protocol_commands::GET_SUBACCOUNT_ROUTE_LIST);
	pt_protocol.add(protocol_strings::USERNAME,username);
	pt_root.add_child(protocol_strings::REQUEST, pt_protocol);
	PACK_JSON_END
}

bool get_subaccount_route_list_req::unpack(const std::string& json)
{
	UNPACK_JSON_BEGIN
	pt_protocol = pt_root.get_child(protocol_strings::REQUEST);
	username = pt_protocol.get<std::string>(protocol_strings::USERNAME);
	UNPACK_JSON_END
}

bool get_subaccount_route_list_resp::pack(std::string& json)
{
	PACK_JSON_BEGIN
	pt_protocol.add(protocol_strings::STATUS, (int)status);
	boost::property_tree::ptree childen;
	for (size_t i = 0;i < subaccout_routes.size();i++)
	{
		boost::property_tree::ptree child;
		subaccout_routes.at(i).pack(child);
		childen.push_back(std::make_pair("",child));
	}
	pt_protocol.add_child(protocol_strings::SUBACCOUNT_ROUTE_LIST, childen);
	pt_root.add_child(protocol_strings::RESPONSE, pt_protocol);
	PACK_JSON_END	
}

bool get_subaccount_route_list_resp::unpack(const std::string& json)
{
	UNPACK_JSON_BEGIN
	pt_protocol = pt_root.get_child(protocol_strings::RESPONSE);
	status = (PROTOCOL_RESP_STATUS)pt_protocol.get<int>(protocol_strings::STATUS);
	boost::property_tree::ptree children = pt_protocol.get_child(protocol_strings::SUBACCOUNT_ROUTE_LIST);		
	BOOST_FOREACH(boost::property_tree::ptree::value_type &v, children)
	{
		boost::property_tree::ptree p = v.second;
		subaccount_route subaccount_route_temp;
		subaccount_route_temp.unpack(p);
		subaccout_routes.push_back(subaccount_route_temp);
	}
	UNPACK_JSON_END
}

void link_information::pack(boost::property_tree::ptree& pt)
{
	std::string linkman_base64 = base64_encode((const unsigned char *)linkman.c_str(),(unsigned int)linkman.length());
	pt.add(protocol_strings::LINKMAN, linkman_base64);
	pt.add(protocol_strings::PHONE_NUMBER,phone_number);
	pt.add(protocol_strings::E_MAIL,e_mail);
}

void link_information::unpack(boost::property_tree::ptree& pt)
{
	std::string linkman_base64 = pt.get<std::string>(protocol_strings::LINKMAN);
	std::vector<unsigned char> v_name = base64_decode(linkman_base64);
	linkman = std::string(v_name.begin(), v_name.end());
	phone_number = pt.get<std::string>(phone_number);
	e_mail = pt.get<std::string>(protocol_strings::E_MAIL);
}

void subaccount_base_information::pack(boost::property_tree::ptree& pt)
{
	pt.add(protocol_strings::USERNAME, username);
	pt.add(protocol_strings::PASSWORD, password);
	pt.add(protocol_strings::COMPANY_ACCOUNT_USERNAME, company_account_username);
	pt.add(protocol_strings::DESCRIPTION, description);

	pt.add(protocol_strings::FUNCTION_TYPE, function_type);
	pt.add(protocol_strings::STATE, state);
	pt.add(protocol_strings::CHARGE_MODE,charge_mode);
	pt.add(protocol_strings::SENDNUMBER_TYPE,send_number_type);
	pt.add(protocol_strings::SEND_NUMBER,send_number);
	pt.add(protocol_strings::RECEIVELIMIT_TYPE,receive_limit_type);
	pt.add(protocol_strings::RECEIVELIMIT_NETCODE,receive_limit_netcode);

	pt.add(protocol_strings::DAILY_CONSUME, daily_consume);
	pt.add(protocol_strings::MONTHLY_CONSUME, monthly_consume);
	pt.add(protocol_strings::IP_ADDRESS, ip_address);

	pt.add(protocol_strings::WHITE_LIST_STATE, white_list_state);
	pt.add(protocol_strings::DEVOUR_RATE, devour_rate);
	pt.add(protocol_strings::DEVOUR_THRESHOLD,devoure_threshold);
	pt.add(protocol_strings::ALREADY_SEND_COUNT,already_send_count);
	pt.add(protocol_strings::SPLIT_STATE,split_state);
	pt.add(protocol_strings::VERIFY_MODE,verify_mode);
	pt.add(protocol_strings::HIGH_FREQUENCY_THRESHOLD,high_frequency_threshold);
#if 0
	{
		boost::property_tree::ptree childen;
		for (size_t i = 0;i < routes.size();i++)
		{
			boost::property_tree::ptree child;
			routes.at(i).pack(child);
			childen.push_back(std::make_pair("",child));
		}
		pt.add_child(protocol_strings::ROUTE_LIST, childen);
	}
#endif
}

void subaccount_base_information::unpack(boost::property_tree::ptree& pt)
{
	username = pt.get<std::string>(protocol_strings::USERNAME);
	password = pt.get<std::string>(protocol_strings::PASSWORD);
	company_account_username = pt.get<std::string>(protocol_strings::COMPANY_ACCOUNT_USERNAME);
	description = pt.get<std::string>(protocol_strings::DESCRIPTION);

	function_type = (FUNCTION_TYPE)pt.get<int>(protocol_strings::FUNCTION_TYPE);
	state = (ACCOUNT_STATE)pt.get<int>(protocol_strings::STATE);
	charge_mode = (CHARGE_MODE)pt.get<int>(protocol_strings::CHARGE_MODE);

	send_number_type = (SEND_NUMBER_TYPE)pt.get<int>(protocol_strings::SENDNUMBER_TYPE);
	send_number = pt.get<std::string>(protocol_strings::SEND_NUMBER);
	receive_limit_type = (RECEIVE_LIMIT_TYPE)pt.get<int>(protocol_strings::RECEIVELIMIT_TYPE);
	receive_limit_netcode = pt.get<std::string>(protocol_strings::RECEIVELIMIT_NETCODE);
	daily_consume = pt.get<double>(protocol_strings::DAILY_CONSUME);
	monthly_consume = pt.get<double>(protocol_strings::MONTHLY_CONSUME);
	ip_address = pt.get<std::string>(protocol_strings::IP_ADDRESS);

	white_list_state = (USING_STATE)pt.get<int>(protocol_strings::WHITE_LIST_STATE);
	devour_rate = pt.get<double>(protocol_strings::DEVOUR_RATE);
	devoure_threshold = pt.get<unsigned int>(protocol_strings::DEVOUR_THRESHOLD);
	already_send_count = pt.get<unsigned int>(protocol_strings::ALREADY_SEND_COUNT);
	split_state = (SPLIT_STATE)pt.get<unsigned int>(protocol_strings::SPLIT_STATE);
	verify_mode = (VERIFY_MODE)pt.get<unsigned int>(protocol_strings::VERIFY_MODE);
	high_frequency_threshold = pt.get<unsigned int>(protocol_strings::HIGH_FREQUENCY_THRESHOLD);
#if 0
	{
		boost::property_tree::ptree children = pt.get_child(protocol_strings::ROUTE_LIST);		
		BOOST_FOREACH(boost::property_tree::ptree::value_type &v, children)
		{
			boost::property_tree::ptree p = v.second;
			subaccount_route route;
			route.unpack(p);
			routes.push_back(route);
		}
	}
#endif
}

bool get_subaccount_base_information_list_req::pack(std::string& json)
{
	PACK_JSON_BEGIN
	pt_protocol.add(protocol_strings::COMMAND, protocol_commands::GET_SUBACCOUNT_BASE_INFORMATION_LIST);
	pt_protocol.add(protocol_strings::COMPANY_ACCOUNT_USERNAME,company_account_username);
	pt_root.add_child(protocol_strings::REQUEST, pt_protocol);
	PACK_JSON_END
}

bool get_subaccount_base_information_list_req::unpack(const std::string& json)
{
	UNPACK_JSON_BEGIN
	pt_protocol = pt_root.get_child(protocol_strings::REQUEST);
	company_account_username = pt_protocol.get<std::string>(protocol_strings::COMPANY_ACCOUNT_USERNAME);
	UNPACK_JSON_END
}

bool get_subaccount_base_information_list_resp::pack(std::string& json)
{
	PACK_JSON_BEGIN
	pt_protocol.add(protocol_strings::STATUS, (int)status);
	boost::property_tree::ptree childen;
	for (size_t i = 0;i < subaccounts.size();i++)
	{
		boost::property_tree::ptree child;
		subaccounts.at(i).pack(child);
		childen.push_back(std::make_pair("",child));
	}
	pt_protocol.add_child(protocol_strings::SUBACCOUNT_LIST, childen);
	pt_root.add_child(protocol_strings::RESPONSE, pt_protocol);
	PACK_JSON_END	
}

bool get_subaccount_base_information_list_resp::unpack(const std::string& json)
{
	UNPACK_JSON_BEGIN
	pt_protocol = pt_root.get_child(protocol_strings::RESPONSE);
	status = (PROTOCOL_RESP_STATUS)pt_protocol.get<int>(protocol_strings::STATUS);
	boost::property_tree::ptree children = pt_protocol.get_child(protocol_strings::SUBACCOUNT_LIST);		
	BOOST_FOREACH(boost::property_tree::ptree::value_type &v, children)
	{
		boost::property_tree::ptree p = v.second;
		subaccount_base_information sub_account;
		sub_account.unpack(p);
		subaccounts.push_back(sub_account);
	}
	UNPACK_JSON_END
}

void subaccount::pack(boost::property_tree::ptree& pt)
{
	{
		boost::property_tree::ptree child;
		base_info.pack(child);
		pt.add_child(protocol_strings::BASE_INFO,child);
	}

	{
		boost::property_tree::ptree child;
		link_info.pack(child);
		pt.add_child(protocol_strings::LINK_INFO,child);
	}

	{
		boost::property_tree::ptree childen;
		for (size_t i = 0;i < routes.size();i++)
		{
			boost::property_tree::ptree child;
			routes.at(i).pack(child);
			childen.push_back(std::make_pair("",child));
		}
		pt.add_child(protocol_strings::ROUTE_LIST, childen);
	}

#if 0
	{
		boost::property_tree::ptree childen;
		for (size_t i = 0;i < signatures.size();i++)
		{
			boost::property_tree::ptree child;
			signatures.at(i).pack(child);
			childen.push_back(std::make_pair("",child));
		}
		pt.add_child(protocol_strings::SIGNATURE_LIST, childen);
	}
#endif
}

void subaccount::unpack(boost::property_tree::ptree& pt)
{
	{
		boost::property_tree::ptree child = pt.get_child(protocol_strings::BASE_INFO);
		base_info.unpack(child);
	}

	{
		boost::property_tree::ptree child = pt.get_child(protocol_strings::LINK_INFO);
		link_info.unpack(child);
	}

	{
		boost::property_tree::ptree child = pt.get_child(protocol_strings::ROUTE_LIST);		
		BOOST_FOREACH(boost::property_tree::ptree::value_type &v, child)
		{
			boost::property_tree::ptree p = v.second;
			subaccount_route route;
			route.unpack(p);
			routes.push_back(route);
		}
	}

#if 0
	{
		boost::property_tree::ptree children = pt.get_child(protocol_strings::SIGNATURE_LIST);		
		BOOST_FOREACH(boost::property_tree::ptree::value_type &v, children)
		{
			boost::property_tree::ptree p = v.second;
			signature signaturetemp;
			signaturetemp.unpack(p);
			signatures.push_back(signaturetemp);
		}
	}
#endif
}

bool add_subaccount_req::pack(std::string& json)
{
	PACK_JSON_BEGIN
	pt_protocol.add(protocol_strings::COMMAND, protocol_commands::ADD_SUBACCOUNT);
	sub_account.pack(pt_protocol);
	pt_root.add_child(protocol_strings::REQUEST, pt_protocol);
	PACK_JSON_END
}

bool add_subaccount_req::unpack(const std::string& json)
{
	UNPACK_JSON_BEGIN
	pt_protocol = pt_root.get_child(protocol_strings::REQUEST);
	sub_account.unpack(pt_protocol);
	UNPACK_JSON_END
}

bool add_subaccount_resp::pack(std::string& json)
{
	PACK_JSON_BEGIN
	pt_protocol.add(protocol_strings::STATUS, (int)status);
	pt_root.add_child(protocol_strings::RESPONSE, pt_protocol);
	PACK_JSON_END
}

bool add_subaccount_resp::unpack(const std::string& json)
{
	UNPACK_JSON_BEGIN
	pt_protocol = pt_root.get_child(protocol_strings::RESPONSE);
	status = (PROTOCOL_RESP_STATUS)pt_protocol.get<int>(protocol_strings::STATUS);
	UNPACK_JSON_END
}

bool get_subaccount_list_req::pack(std::string& json)
{
	PACK_JSON_BEGIN
	pt_protocol.add(protocol_strings::COMMAND, protocol_commands::GET_SUBACCOUNT_LIST);
	pt_protocol.add(protocol_strings::COMPANY_ACCOUNT_USERNAME,company_account_username);
	pt_root.add_child(protocol_strings::REQUEST, pt_protocol);
	PACK_JSON_END
}

bool get_subaccount_list_req::unpack(const std::string& json)
{
	UNPACK_JSON_BEGIN
	pt_protocol = pt_root.get_child(protocol_strings::REQUEST);
	company_account_username = pt_protocol.get<std::string>(protocol_strings::COMPANY_ACCOUNT_USERNAME);
	UNPACK_JSON_END
}

bool get_subaccount_list_resp::pack(std::string& json)
{
	PACK_JSON_BEGIN
	pt_protocol.add(protocol_strings::STATUS, (int)status);
	boost::property_tree::ptree childen;
	for (size_t i = 0;i < subaccounts.size();i++)
	{
		boost::property_tree::ptree child;
		subaccounts.at(i).pack(child);
		childen.push_back(std::make_pair("",child));
	}
	pt_protocol.add_child(protocol_strings::SUBACCOUNT_LIST, childen);
	pt_root.add_child(protocol_strings::RESPONSE, pt_protocol);
	PACK_JSON_END	
}

bool get_subaccount_list_resp::unpack(const std::string& json)
{
	UNPACK_JSON_BEGIN
	pt_protocol = pt_root.get_child(protocol_strings::RESPONSE);
	status = (PROTOCOL_RESP_STATUS)pt_protocol.get<int>(protocol_strings::STATUS);
	boost::property_tree::ptree children = pt_protocol.get_child(protocol_strings::SUBACCOUNT_LIST);		
	BOOST_FOREACH(boost::property_tree::ptree::value_type &v, children)
	{
		boost::property_tree::ptree p = v.second;
		subaccount sub_account;
		sub_account.unpack(p);
		subaccounts.push_back(sub_account);
	}
	UNPACK_JSON_END
}

void route_disconnect::pack(boost::property_tree::ptree& pt)
{
	pt.add(protocol_strings::INTERVAL_TIME, interval_time);
	pt.add(protocol_strings::TIMES, times);
}

void route_disconnect::unpack(boost::property_tree::ptree& pt)
{
	interval_time = pt.get<int>(protocol_strings::INTERVAL_TIME);
	times = pt.get<int>(protocol_strings::TIMES);
}

void route_statistics::pack(boost::property_tree::ptree& pt)
{
	pt.add(protocol_strings::CYCLE, cycle);
	pt.add(protocol_strings::RECEIVE_ACK_MAX_TIME, receive_ack_max_time);
	pt.add(protocol_strings::RATIO, ratio);
}

void route_statistics::unpack(boost::property_tree::ptree& pt)
{
	cycle = pt.get<int>(protocol_strings::CYCLE);
	receive_ack_max_time = pt.get<int>(protocol_strings::RECEIVE_ACK_MAX_TIME);
	ratio = pt.get<double>(protocol_strings::RATIO);
}

void route_attribute::pack(boost::property_tree::ptree& pt)
{
	pt.add(protocol_strings::ID, id);
	pt.add(protocol_strings::PROVIDER_ACCOUNT_ID,provider_account_id);
	pt.add(protocol_strings::USERNAME, username);
	pt.add(protocol_strings::PASSWORD, password);
	pt.add(protocol_strings::DESCRIPTION, description);

	pt.add(protocol_strings::FUNCTION_TYPE,function_type);
	pt.add(protocol_strings::PROTOCOL_TYPE, protocol_type);
	pt.add(protocol_strings::STATE,state);
	pt.add(protocol_strings::CHARGE_MODE,charge_mode);
	pt.add(protocol_strings::IP_ADDRESS, ip_address);
	pt.add(protocol_strings::PORT, port);
	pt.add(protocol_strings::HEARTBEAT, heartbeat);

	//pt.add(protocol_strings::COMPANY_NAME, company_name);
	std::string company_name_base64 = base64_encode((const unsigned char *)company_name.c_str(),(unsigned int)company_name.length());
	pt.add(protocol_strings::COMPANY_NAME, company_name_base64);
	std::string industry_base64 = base64_encode((const unsigned char *)industry.c_str(),(unsigned int)industry.length());
	pt.add(protocol_strings::INDUSTRY, industry_base64);
	pt.add(protocol_strings::ASCALL_SIZE, ascall_size);
	pt.add(protocol_strings::UCS2_SIZE, ucs2_size);

	pt.add(protocol_strings::STATISTICS_BELONGINGNESS,statistics_belongingness);
	pt.add(protocol_strings::RECHARGE_MODE, recharge_mode);
	//pt.add(protocol_strings::BALANCE, balance);
	pt.add(protocol_strings::BALANCE_MONEY,balance_money);
	pt.add(protocol_strings::BALANCE_COUNT,balance_count);
	pt.add(protocol_strings::BALANCE_ADDRESS, balance_address);
	//pt.add(protocol_strings::CREDIT_LINE,credit_line);
	pt.add(protocol_strings::CREDIT_LINE_MONEY,credit_line_money);
	pt.add(protocol_strings::CREDIT_LINE_COUNT,credit_line_count);
	//pt.add(protocol_strings::THRESHOLD_VALUE, threshold_value);
	pt.add(protocol_strings::THRESHOLD_VALUE_MONEY,threshold_value_money);
	pt.add(protocol_strings::THRESHOLD_VALUE_COUNT,threshold_value_count);

	pt.add(protocol_strings::MAX_COUNT_PER_SECOND, max_count_per_second);
	pt.add(protocol_strings::PACKAGE_STATE, package_state);
	pt.add(protocol_strings::PACKAGE_COUNT, package_count);
	pt.add(protocol_strings::PACKAGE_USED_COUNT, package_used_count);
	pt.add(protocol_strings::PACKAGE_SURPLUS_COUNT, package_surplus_count);

	pt.add(protocol_strings::DISABLE_CONNECT_MAX_TIME, disable_connect_max_time);
	route_disconnect_.pack(pt);
	route_statistics_.pack(pt);

	pt.add(protocol_strings::SIGNATURE_TYPE, signature_type);
	pt.add(protocol_strings::SIGNATURE_POSITION, signature_position);

	pt.add(protocol_strings::SPECIFIED_NUMBER_TYPE,specified_number_type);

#if 0
	{
		boost::property_tree::ptree childen;
		for (size_t i = 0;i < signatures.size();i++)
		{
			boost::property_tree::ptree child;
			signatures.at(i).pack(child);
			childen.push_back(std::make_pair("",child));
		}
		pt.add_child(protocol_strings::SIGNATURE_LIST, childen);
	}

	{
		boost::property_tree::ptree childen;
		for (size_t i = 0;i < templates.size();i++)
		{
			boost::property_tree::ptree child;
			templates.at(i).pack(child);
			childen.push_back(std::make_pair("",child));
		}
		pt.add_child(protocol_strings::TEMPLATE_LIST, childen);
	}
#endif
}

void route_attribute::unpack(boost::property_tree::ptree& pt)
{
	id = pt.get<std::string>(protocol_strings::ID);
	provider_account_id = pt.get<std::string>(protocol_strings::PROVIDER_ACCOUNT_ID);
	username = pt.get<std::string>(protocol_strings::USERNAME);
	password = pt.get<std::string>(protocol_strings::PASSWORD);
	description = pt.get<std::string>(protocol_strings::DESCRIPTION);

	function_type = (FUNCTION_TYPE)pt.get<int>(protocol_strings::FUNCTION_TYPE);
	protocol_type = (ROUTE_PROTOCOL)pt.get<int>(protocol_strings::PROTOCOL_TYPE);
	state = (ACCOUNT_STATE)pt.get<int>(protocol_strings::STATE);
	charge_mode = (CHARGE_MODE)pt.get<int>(protocol_strings::CHARGE_MODE);
	ip_address = pt.get<std::string>(protocol_strings::IP_ADDRESS);
	port = pt.get<int>(protocol_strings::PORT);
	heartbeat = pt.get<double>(protocol_strings::HEARTBEAT);
	
	//company_name = pt.get<std::string>(protocol_strings::COMPANY_NAME);
	std::string company_name_base64 = pt.get<std::string>(protocol_strings::COMPANY_NAME);
	std::vector<unsigned char> v_name = base64_decode(company_name_base64);
	company_name = std::string(v_name.begin(), v_name.end());
	std::string industry_base64 = pt.get<std::string>(protocol_strings::INDUSTRY);
     v_name = base64_decode(industry_base64);
	industry = std::string(v_name.begin(), v_name.end());
	ascall_size = pt.get<int>(protocol_strings::ASCALL_SIZE);
	ucs2_size = pt.get<int>(protocol_strings::UCS2_SIZE);

	statistics_belongingness = (STATISTICS_BELONGINGNESS)pt.get<int>(protocol_strings::STATISTICS_BELONGINGNESS);
	recharge_mode = (RECHARGE_MODE)pt.get<int>(protocol_strings::RECHARGE_MODE);
	//balance = pt.get<double>(protocol_strings::BALANCE);
	balance_money = pt.get<double>(protocol_strings::BALANCE_MONEY);
	balance_count = pt.get<int>(protocol_strings::BALANCE_COUNT);
	balance_address = pt.get<std::string>(protocol_strings::BALANCE_ADDRESS);
	//credit_line = pt.get<double>(protocol_strings::CREDIT_LINE);
	credit_line_money = pt.get<double>(protocol_strings::CREDIT_LINE_MONEY);
	credit_line_count = pt.get<unsigned int>(protocol_strings::CREDIT_LINE_COUNT);
	//threshold_value = pt.get<double>(protocol_strings::THRESHOLD_VALUE);
	threshold_value_money = pt.get<double>(protocol_strings::THRESHOLD_VALUE_MONEY);
	threshold_value_count = pt.get<unsigned int>(protocol_strings::THRESHOLD_VALUE_COUNT);
	max_count_per_second = pt.get<int>(protocol_strings::MAX_COUNT_PER_SECOND);

	package_state = (PACKAGE_STATE)pt.get<int>(protocol_strings::PACKAGE_STATE);
	package_count = pt.get<int>(protocol_strings::PACKAGE_COUNT);
	package_used_count = pt.get<int>(protocol_strings::PACKAGE_USED_COUNT);
	package_surplus_count = pt.get<int>(protocol_strings::PACKAGE_SURPLUS_COUNT);

	disable_connect_max_time = pt.get<int>(protocol_strings::DISABLE_CONNECT_MAX_TIME);
	route_disconnect_.unpack(pt);
	route_statistics_.unpack(pt);
	signature_type = (SIGNATURE_TYPE)pt.get<int>(protocol_strings::SIGNATURE_TYPE);
	signature_position = (SIGNATURE_POSITION)pt.get<int>(protocol_strings::SIGNATURE_POSITION);

	specified_number_type = (SPECIFIED_NUMBER_TYPE)pt.get<int>(protocol_strings::SPECIFIED_NUMBER_TYPE);

#if 0
	{
		boost::property_tree::ptree children = pt.get_child(protocol_strings::SIGNATURE_LIST);		
		BOOST_FOREACH(boost::property_tree::ptree::value_type &v, children)
		{
			boost::property_tree::ptree p = v.second;
			signature signaturetemp;
			signaturetemp.unpack(p);
			signatures.push_back(signaturetemp);
		}
	}

	{
		boost::property_tree::ptree children = pt.get_child(protocol_strings::TEMPLATE_LIST);		
		BOOST_FOREACH(boost::property_tree::ptree::value_type &v, children)
		{
			boost::property_tree::ptree p = v.second;
			single_template templatetemp;
			templatetemp.unpack(p);
			templates.push_back(templatetemp);
		}
	}
#endif
}

bool add_route_req::pack(std::string& json)
{
	PACK_JSON_BEGIN
	pt_protocol.add(protocol_strings::COMMAND, protocol_commands::ADD_ROUTE);
	route.pack(pt_protocol);
	pt_root.add_child(protocol_strings::REQUEST, pt_protocol);
	PACK_JSON_END
}

bool add_route_req::unpack(const std::string& json)
{
	UNPACK_JSON_BEGIN
	pt_protocol = pt_root.get_child(protocol_strings::REQUEST);
	route.unpack(pt_protocol);
	UNPACK_JSON_END
}

bool add_route_resp::pack(std::string& json)
{
	PACK_JSON_BEGIN
	pt_protocol.add(protocol_strings::STATUS, (int)status);
	pt_root.add_child(protocol_strings::RESPONSE, pt_protocol);
	PACK_JSON_END	
}

bool add_route_resp::unpack(const std::string& json)
{
	UNPACK_JSON_BEGIN
	pt_protocol = pt_root.get_child(protocol_strings::RESPONSE);
	status = (PROTOCOL_RESP_STATUS)pt_protocol.get<int>(protocol_strings::STATUS);
	UNPACK_JSON_END
}

bool get_route_list_req::pack(std::string& json)
{
	PACK_JSON_BEGIN
	pt_protocol.add(protocol_strings::COMMAND, protocol_commands::GET_ROUTE_LIST);
	pt_protocol.add(protocol_strings::PROVIDER_ACCOUNT_ID, provider_account_id);
	pt_root.add_child(protocol_strings::REQUEST, pt_protocol);
	PACK_JSON_END
}

bool get_route_list_req::unpack(const std::string& json)
{
	UNPACK_JSON_BEGIN
	pt_protocol = pt_root.get_child(protocol_strings::REQUEST);
	provider_account_id = pt_protocol.get<std::string>(protocol_strings::PROVIDER_ACCOUNT_ID);
	UNPACK_JSON_END
}

bool get_route_list_resp::pack(std::string& json)
{
	PACK_JSON_BEGIN
	pt_protocol.add(protocol_strings::STATUS, (int)status);
	boost::property_tree::ptree childen;
	for (size_t i = 0;i < routes.size();i++)
	{
		boost::property_tree::ptree child;
		routes.at(i).pack(child);
		childen.push_back(std::make_pair("",child));
	}
	pt_protocol.add_child(protocol_strings::ROUTE_LIST, childen);
	pt_root.add_child(protocol_strings::RESPONSE, pt_protocol);
	PACK_JSON_END	
}

bool get_route_list_resp::unpack(const std::string& json)
{
	UNPACK_JSON_BEGIN
	pt_protocol = pt_root.get_child(protocol_strings::RESPONSE);
	status = (PROTOCOL_RESP_STATUS)pt_protocol.get<int>(protocol_strings::STATUS);
	boost::property_tree::ptree children = pt_protocol.get_child(protocol_strings::ROUTE_LIST);		
	BOOST_FOREACH(boost::property_tree::ptree::value_type &v, children)
	{
		boost::property_tree::ptree p = v.second;
		route_attribute route;
		route.unpack(p);
		routes.push_back(route);
	}
	UNPACK_JSON_END
}

void provider_account::pack(boost::property_tree::ptree& pt)
{
	pt.add(protocol_strings::ID, id);
	pt.add(protocol_strings::USERNAME, username);
	pt.add(protocol_strings::PASSWORD, password);
	//pt.add(protocol_strings::COMPANY_NAME, company_name);
	std::string company_name_base64 = base64_encode((const unsigned char *)company_name.c_str(),(unsigned int)company_name.length());
	pt.add(protocol_strings::COMPANY_NAME, company_name_base64);
	pt.add(protocol_strings::DESCRIPTION, description);

	pt.add(protocol_strings::STATE, state);
	pt.add(protocol_strings::STATISTICS_BELONGINGNESS, statistics_belongingness);
	pt.add(protocol_strings::RECHARGE_MODE,recharge_mode);
	//pt.add(protocol_strings::BALANCE,balance);
	pt.add(protocol_strings::BALANCE_MONEY,balance_money);
	pt.add(protocol_strings::BALANCE_COUNT,balance_count);
	pt.add(protocol_strings::BALANCE_ADDRESS, balance_address);
	//pt.add(protocol_strings::CREDIT_LINE, credit_line);
	pt.add(protocol_strings::CREDIT_LINE_MONEY,credit_line_money);
	pt.add(protocol_strings::CREDIT_LINE_COUNT,credit_line_count);
	//pt.add(protocol_strings::THRESHOLD_VALUE,threshold_value);
	pt.add(protocol_strings::THRESHOLD_VALUE_MONEY,threshold_value_money);
	pt.add(protocol_strings::THRESHOLD_VALUE_COUNT,threshold_value_count);

	pt.add(protocol_strings::DAILY_CONSUME, daily_consume);
	pt.add(protocol_strings::PACKAGE_STATE,package_state);
	pt.add(protocol_strings::PACKAGE_COUNT,package_count);
	pt.add(protocol_strings::SETUP_TIME, setup_time);
	//pt.add(protocol_strings::MANAGER, manager);
	std::string manager_base64 = base64_encode((const unsigned char *)manager.c_str(),(unsigned int)manager.length());
	pt.add(protocol_strings::MANAGER,manager_base64);

	//pt.add(protocol_strings::LINKMAN, linkman);
	std::string linkman_base64 = base64_encode((const unsigned char *)linkman.c_str(),(unsigned int)linkman.length());
	pt.add(protocol_strings::LINKMAN,linkman_base64);
	pt.add(protocol_strings::PHONE_NUMBER, phone_number);
	pt.add(protocol_strings::E_MAIL, e_mail);
}

void provider_account::unpack(boost::property_tree::ptree& pt)
{
	id = pt.get<std::string>(protocol_strings::ID);
	username = pt.get<std::string>(protocol_strings::USERNAME);
	password = pt.get<std::string>(protocol_strings::PASSWORD);
	//company_name = pt.get<std::string>(protocol_strings::COMPANY_NAME);
	std::string company_name_base64 = pt.get<std::string>(protocol_strings::COMPANY_NAME);
	std::vector<unsigned char> v_name = base64_decode(company_name_base64);
	company_name = std::string(v_name.begin(), v_name.end());
	description = pt.get<std::string>(protocol_strings::DESCRIPTION);

	state = (ACCOUNT_STATE)pt.get<int>(protocol_strings::STATE);
	statistics_belongingness = (STATISTICS_BELONGINGNESS)pt.get<int>(protocol_strings::STATISTICS_BELONGINGNESS);
	recharge_mode = (RECHARGE_MODE)pt.get<int>(protocol_strings::RECHARGE_MODE);
	//balance = pt.get<double>(protocol_strings::BALANCE);
	balance_money = pt.get<double>(protocol_strings::BALANCE_MONEY);
	balance_count = pt.get<int>(protocol_strings::BALANCE_COUNT);
	balance_address = pt.get<std::string>(protocol_strings::BALANCE_ADDRESS);
	//credit_line = pt.get<double>(protocol_strings::CREDIT_LINE);
	credit_line_money = pt.get<double>(protocol_strings::CREDIT_LINE_MONEY);
	credit_line_count = pt.get<unsigned int>(protocol_strings::CREDIT_LINE_COUNT);
	//threshold_value = pt.get<double>(protocol_strings::THRESHOLD_VALUE);
	threshold_value_money = pt.get<double>(protocol_strings::THRESHOLD_VALUE_MONEY);
	threshold_value_count = pt.get<unsigned int>(protocol_strings::THRESHOLD_VALUE_COUNT);

	daily_consume = pt.get<double>(protocol_strings::DAILY_CONSUME);
	package_state = (PACKAGE_STATE)pt.get<int>(protocol_strings::PACKAGE_STATE);
	package_count = pt.get<int>(protocol_strings::PACKAGE_COUNT);
	setup_time = pt.get<boost::uint64_t>(protocol_strings::SETUP_TIME);
	//manager = pt.get<std::string>(protocol_strings::MANAGER);
	std::string manager_base64 = pt.get<std::string>(protocol_strings::MANAGER);
	v_name = base64_decode(manager_base64);
	manager = std::string(v_name.begin(),v_name.end());

	//linkman = pt.get<std::string>(protocol_strings::LINKMAN);
	std::string linkman_base64 = pt.get<std::string>(protocol_strings::LINKMAN);
	v_name = base64_decode(linkman_base64);
	linkman = std::string(v_name.begin(),v_name.end());
	phone_number = pt.get<std::string>(protocol_strings::PHONE_NUMBER);
	e_mail = pt.get<std::string>(protocol_strings::E_MAIL);
}

bool add_provider_account_req::pack(std::string& json)
{
	PACK_JSON_BEGIN
	pt_protocol.add(protocol_strings::COMMAND, protocol_commands::ADD_PROACCOUNT);
	pro_account.pack(pt_protocol);
	pt_root.add_child(protocol_strings::REQUEST, pt_protocol);
	PACK_JSON_END
}

bool add_provider_account_req::unpack(const std::string& json)
{
	UNPACK_JSON_BEGIN
	pt_protocol = pt_root.get_child(protocol_strings::REQUEST);
	pro_account.unpack(pt_protocol);
	UNPACK_JSON_END
}

bool add_provider_account_resp::pack(std::string& json)
{
	PACK_JSON_BEGIN
	pt_protocol.add(protocol_strings::STATUS, (int)status);
	pt_root.add_child(protocol_strings::RESPONSE, pt_protocol);
	PACK_JSON_END
}

bool add_provider_account_resp::unpack(const std::string& json)
{
	UNPACK_JSON_BEGIN
	pt_protocol = pt_root.get_child(protocol_strings::RESPONSE);
	status = (PROTOCOL_RESP_STATUS)pt_protocol.get<int>(protocol_strings::STATUS);
	UNPACK_JSON_END
}

bool get_provider_account_list_req::pack(std::string& json)
{
	PACK_JSON_BEGIN
	pt_protocol.add(protocol_strings::COMMAND, protocol_commands::GET_PROACCOUNT_LIST);
	pt_root.add_child(protocol_strings::REQUEST, pt_protocol);
	PACK_JSON_END
}

bool get_provider_account_list_req::unpack(const std::string& json)
{
	UNPACK_JSON_BEGIN
	pt_protocol = pt_root.get_child(protocol_strings::REQUEST);
	UNPACK_JSON_END
}

bool get_provider_account_list_resp::pack(std::string& json)
{
	PACK_JSON_BEGIN
	pt_protocol.add(protocol_strings::STATUS, (int)status);
	boost::property_tree::ptree childen;
	for (size_t i = 0;i < pro_accounts.size();i++)
	{
		boost::property_tree::ptree child;
		pro_accounts.at(i).pack(child);
		childen.push_back(std::make_pair("",child));
	}
	pt_protocol.add_child(protocol_strings::PROVIDER_ACCOUNT_LIST, childen);
	pt_root.add_child(protocol_strings::RESPONSE, pt_protocol);
	PACK_JSON_END	
}

bool get_provider_account_list_resp::unpack(const std::string& json)
{
	UNPACK_JSON_BEGIN
	pt_protocol = pt_root.get_child(protocol_strings::RESPONSE);
	status = (PROTOCOL_RESP_STATUS)pt_protocol.get<int>(protocol_strings::STATUS);
	boost::property_tree::ptree children = pt_protocol.get_child(protocol_strings::PROVIDER_ACCOUNT_LIST);		
	BOOST_FOREACH(boost::property_tree::ptree::value_type &v, children)
	{
		boost::property_tree::ptree p = v.second;
		provider_account pro_account;
		pro_account.unpack(p);
		pro_accounts.push_back(pro_account);
	}
	UNPACK_JSON_END
}

void feerate::pack(boost::property_tree::ptree& pt)
{
	pt.add(protocol_strings::INDEX, index);
	pt.add(protocol_strings::MCC, mcc);
	pt.add(protocol_strings::MNC, mnc);
	pt.add(protocol_strings::MT_PRICE, mt_price);
	pt.add(protocol_strings::MO_PRICE, mo_price);
}

void feerate::unpack(boost::property_tree::ptree& pt)
{
	index = pt.get<std::string>(protocol_strings::INDEX);
	mcc = pt.get<std::string>(protocol_strings::MCC);
	mnc = pt.get<std::string>(protocol_strings::MNC);
	mt_price = pt.get<double>(protocol_strings::MT_PRICE);
	mo_price = pt.get<double>(protocol_strings::MO_PRICE);
}

bool add_feerate_list_req::pack(std::string& json)
{
	PACK_JSON_BEGIN
	pt_protocol.add(protocol_strings::COMMAND, protocol_commands::ADD_FEERATE);
	pt_protocol.add(protocol_strings::BELONGINGNESS,belongingness);
	//pt_protocol.add(protocol_strings::INDEX_VALUE,index_value);
	boost::property_tree::ptree childen;
	for (int i = 0;i < feerates.size();i++)
	{
		boost::property_tree::ptree child;
		feerates.at(i).pack(child);
		childen.push_back(make_pair("",child));
	}
	pt_protocol.add_child(protocol_strings::FEERATE_LIST,childen);
	pt_root.add_child(protocol_strings::REQUEST, pt_protocol);
	PACK_JSON_END
}

bool add_feerate_list_req::unpack(const std::string& json)
{
	UNPACK_JSON_BEGIN
	pt_protocol = pt_root.get_child(protocol_strings::REQUEST);
	belongingness = (FEERATE_BELONGINGNESS)pt_protocol.get<int>(protocol_strings::BELONGINGNESS);
	//index_value = pt_protocol.get<std::string>(protocol_strings::INDEX_VALUE);
	boost::property_tree::ptree children = pt_protocol.get_child(protocol_strings::FEERATE_LIST);		
	BOOST_FOREACH(boost::property_tree::ptree::value_type &v, children)
	{
		boost::property_tree::ptree p = v.second;
		feerate feeratetemp;
		feeratetemp.unpack(p);
		feerates.push_back(feeratetemp);
	}
	UNPACK_JSON_END
}

bool add_feerate_list_resp::pack(std::string& json)
{
	PACK_JSON_BEGIN
	pt_protocol.add(protocol_strings::STATUS, (int)status);
	pt_root.add_child(protocol_strings::RESPONSE, pt_protocol);
	PACK_JSON_END	
}

bool add_feerate_list_resp::unpack(const std::string& json)
{
	UNPACK_JSON_BEGIN
	pt_protocol = pt_root.get_child(protocol_strings::RESPONSE);
	status = (PROTOCOL_RESP_STATUS)pt_protocol.get<int>(protocol_strings::STATUS);
	UNPACK_JSON_END
}

bool get_feerate_list_req::pack(std::string& json)
{
	PACK_JSON_BEGIN
	pt_protocol.add(protocol_strings::COMMAND, protocol_commands::GET_FEERATE_LIST);
	pt_protocol.add(protocol_strings::BELONGINGNESS,belongingness);
	pt_protocol.add(protocol_strings::INDEX_VALUE,index_value);
	pt_root.add_child(protocol_strings::REQUEST, pt_protocol);
	PACK_JSON_END
}

bool get_feerate_list_req::unpack(const std::string& json)
{
	UNPACK_JSON_BEGIN
	pt_protocol = pt_root.get_child(protocol_strings::REQUEST);
	belongingness = (FEERATE_BELONGINGNESS)pt_protocol.get<int>(protocol_strings::BELONGINGNESS);
	index_value = pt_protocol.get<std::string>(protocol_strings::INDEX_VALUE);
	UNPACK_JSON_END
}

bool get_feerate_list_resp::pack(std::string& json)
{
	PACK_JSON_BEGIN
	pt_protocol.add(protocol_strings::STATUS, (int)status);
	boost::property_tree::ptree childen;
	for (size_t i = 0;i < feerates.size();i++)
	{
		boost::property_tree::ptree child;
		feerates.at(i).pack(child);
		childen.push_back(std::make_pair("",child));
	}
	pt_protocol.add_child(protocol_strings::FEERATE_LIST, childen);
	pt_root.add_child(protocol_strings::RESPONSE, pt_protocol);
	PACK_JSON_END	
}

bool get_feerate_list_resp::unpack(const std::string& json)
{
	UNPACK_JSON_BEGIN
	pt_protocol = pt_root.get_child(protocol_strings::RESPONSE);
	status = (PROTOCOL_RESP_STATUS)pt_protocol.get<int>(protocol_strings::STATUS);
	boost::property_tree::ptree children = pt_protocol.get_child(protocol_strings::FEERATE_LIST);		
	BOOST_FOREACH(boost::property_tree::ptree::value_type &v, children)
	{
		boost::property_tree::ptree p = v.second;
		feerate feeratetemp;
		feeratetemp.unpack(p);
		feerates.push_back(feeratetemp);
	}
	UNPACK_JSON_END
}

bool choose_route_list_req::pack(std::string& json)
{
	PACK_JSON_BEGIN
	pt_protocol.add(protocol_strings::COMMAND, protocol_commands::CHOOSE_ROUTE_LIST);
	pt_protocol.add(protocol_strings::FUNCTION_TYPE, function_type);
	pt_protocol.add(protocol_strings::CHARGE_MODE, charge_mode);
	pt_root.add_child(protocol_strings::REQUEST, pt_protocol);
	PACK_JSON_END
}

bool choose_route_list_req::unpack(const std::string& json)
{
	UNPACK_JSON_BEGIN
	pt_protocol = pt_root.get_child(protocol_strings::REQUEST);
	function_type = (FUNCTION_TYPE)pt_protocol.get<int>(protocol_strings::FUNCTION_TYPE);
	charge_mode = (CHARGE_MODE)pt_protocol.get<int>(protocol_strings::CHARGE_MODE);
	UNPACK_JSON_END
}

bool choose_route_list_resp::pack(std::string& json)
{
	PACK_JSON_BEGIN
	pt_protocol.add(protocol_strings::STATUS, (int)status);
	boost::property_tree::ptree childen;
	for (size_t i = 0;i < routes.size();i++)
	{
		boost::property_tree::ptree child;
		routes.at(i).pack(child);
		childen.push_back(std::make_pair("",child));
	}
	pt_protocol.add_child(protocol_strings::ROUTE_LIST, childen);
	pt_root.add_child(protocol_strings::RESPONSE, pt_protocol);
	PACK_JSON_END
}

bool choose_route_list_resp::unpack(const std::string& json)
{
	UNPACK_JSON_BEGIN
	pt_protocol = pt_root.get_child(protocol_strings::RESPONSE);
	status = (PROTOCOL_RESP_STATUS)pt_protocol.get<int>(protocol_strings::STATUS);
	boost::property_tree::ptree children = pt_protocol.get_child(protocol_strings::ROUTE_LIST);		
	BOOST_FOREACH(boost::property_tree::ptree::value_type &v, children)
	{
		boost::property_tree::ptree p = v.second;
		route_attribute route;
		route.unpack(p);
		routes.push_back(route);
	}
	UNPACK_JSON_END
}

void specified_number::pack(boost::property_tree::ptree& pt)
{
	pt.add(protocol_strings::ID,id);
	pt.add(protocol_strings::ROUTE_ID, route_id);
	pt.add(protocol_strings::NUMBER, number);
	pt.add(protocol_strings::TYPE,type);
	pt.add(protocol_strings::STATE, state);
	pt.add(protocol_strings::SUBACCOUNT_USERNAME, subaccout_user_name);
}

void specified_number::unpack(boost::property_tree::ptree& pt)
{
	id = pt.get<std::string>(protocol_strings::ID);
	route_id = pt.get<std::string>(protocol_strings::ROUTE_ID);
	number = pt.get<unsigned int>(protocol_strings::NUMBER);
	type = (SPECIFIED_NUMBER_TYPE)pt.get<int>(protocol_strings::TYPE);
	state = (USING_STATE)pt.get<int>(protocol_strings::STATE);
	subaccout_user_name = pt.get<std::string>(protocol_strings::SUBACCOUNT_USERNAME);
}

bool add_specified_number_req::pack(std::string& json)
{
	PACK_JSON_BEGIN
	pt_protocol.add(protocol_strings::COMMAND, protocol_commands::ADD_SPECIFIED_NUMBER);
	specified_number_.pack(pt_protocol);
	pt_root.add_child(protocol_strings::REQUEST, pt_protocol);
	PACK_JSON_END
}

bool add_specified_number_req::unpack(const std::string& json)
{
	UNPACK_JSON_BEGIN
	pt_protocol = pt_root.get_child(protocol_strings::REQUEST);
	specified_number_.unpack(pt_protocol);
	UNPACK_JSON_END
}

bool add_specified_number_resp::pack(std::string& json)
{
	PACK_JSON_BEGIN
	pt_protocol.add(protocol_strings::STATUS, (int)status);
	pt_root.add_child(protocol_strings::RESPONSE, pt_protocol);
	PACK_JSON_END
}

bool add_specified_number_resp::unpack(const std::string& json)
{
	UNPACK_JSON_BEGIN
	pt_protocol = pt_root.get_child(protocol_strings::RESPONSE);
	status = (PROTOCOL_RESP_STATUS)pt_protocol.get<int>(protocol_strings::STATUS);
	UNPACK_JSON_END
}

bool get_specified_number_list_req::pack(std::string& json)
{
	PACK_JSON_BEGIN
	pt_protocol.add(protocol_strings::COMMAND, protocol_commands::GET_SPECIFIED_NUMBER_LIST);
	pt_protocol.add(protocol_strings::ROUTE_ID,route_id);
	pt_root.add_child(protocol_strings::REQUEST, pt_protocol);
	PACK_JSON_END
}

bool get_specified_number_list_req::unpack(const std::string& json)
{
	UNPACK_JSON_BEGIN
	pt_protocol = pt_root.get_child(protocol_strings::REQUEST);
	route_id = pt_protocol.get<std::string>(protocol_strings::ROUTE_ID);
	UNPACK_JSON_END
}

bool get_specified_number_list_resp::pack(std::string& json)
{
	PACK_JSON_BEGIN
	pt_protocol.add(protocol_strings::STATUS, (int)status);
	boost::property_tree::ptree childen;
	for (size_t i = 0;i < specified_numbers.size();i++)
	{
		boost::property_tree::ptree child;
		specified_numbers.at(i).pack(child);
		childen.push_back(std::make_pair("",child));
	}
	pt_protocol.add_child(protocol_strings::SPECIFIED_NUMBER_LIST, childen);
	pt_root.add_child(protocol_strings::RESPONSE, pt_protocol);
	PACK_JSON_END	
}

bool get_specified_number_list_resp::unpack(const std::string& json)
{
	UNPACK_JSON_BEGIN
	pt_protocol = pt_root.get_child(protocol_strings::RESPONSE);
	status = (PROTOCOL_RESP_STATUS)pt_protocol.get<int>(protocol_strings::STATUS);
	boost::property_tree::ptree children = pt_protocol.get_child(protocol_strings::SPECIFIED_NUMBER_LIST);		
	BOOST_FOREACH(boost::property_tree::ptree::value_type &v, children)
	{
		boost::property_tree::ptree p = v.second;
		specified_number specified_number_temp;
		specified_number_temp.unpack(p);
		specified_numbers.push_back(specified_number_temp);
	}
	UNPACK_JSON_END
}

void subaccount_charge::pack(boost::property_tree::ptree& pt)
{
	pt.add(protocol_strings::ID, id);
	pt.add(protocol_strings::SUBACCOUNT_USERNAME, subaccount_username);
	pt.add(protocol_strings::COMPANY_ACCOUNT_USERNAME, company_account_username);
	pt.add(protocol_strings::COUNT, count);
	pt.add(protocol_strings::MONEY, money);
	pt.add(protocol_strings::SETUP_TIME, setup_time);
}

void subaccount_charge::unpack(boost::property_tree::ptree& pt)
{
	id = pt.get<unsigned int>(protocol_strings::ID);
	subaccount_username = pt.get<std::string>(protocol_strings::SUBACCOUNT_USERNAME);
	company_account_username = pt.get<std::string>(protocol_strings::COMPANY_ACCOUNT_USERNAME);
	count = pt.get<int>(protocol_strings::COUNT);
	money = pt.get<double>(protocol_strings::MONEY);
	setup_time = pt.get<boost::uint64_t>(protocol_strings::SETUP_TIME);
}

void company_account_balance::pack(boost::property_tree::ptree& pt)
{
	pt.add(protocol_strings::COMPANY_ACCOUNT_USERNAME, company_account_username);
	pt.add(protocol_strings::BALANCE_MONEY, balance_money);
}

void company_account_balance::unpack(boost::property_tree::ptree& pt)
{
	company_account_username = pt.get<std::string>(protocol_strings::COMPANY_ACCOUNT_USERNAME);
	balance_money = pt.get<double>(protocol_strings::BALANCE_MONEY);
}

bool deduct_subaccount_charge_req::pack(std::string& json)
{
	PACK_JSON_BEGIN
	pt_protocol.add(protocol_strings::COMMAND, protocol_commands::DEDUCT_SUBACCOUNT_CHARGE);
	boost::property_tree::ptree childen;
	for (int i = 0;i < subaccount_charges.size();i++)
	{
		boost::property_tree::ptree child;
		subaccount_charges.at(i).pack(child);
		childen.push_back(make_pair("",child));
	}
	pt_protocol.add_child(protocol_strings::SUBACCOUNT_CHARGE_LIST,childen);
	pt_root.add_child(protocol_strings::REQUEST, pt_protocol);
	PACK_JSON_END
}

bool deduct_subaccount_charge_req::unpack(const std::string& json)
{
	UNPACK_JSON_BEGIN
	pt_protocol = pt_root.get_child(protocol_strings::REQUEST);
	boost::property_tree::ptree children = pt_protocol.get_child(protocol_strings::SUBACCOUNT_CHARGE_LIST);		
	BOOST_FOREACH(boost::property_tree::ptree::value_type &v, children)
	{
		boost::property_tree::ptree p = v.second;
		subaccount_charge charge_temp;
		charge_temp.unpack(p);
		subaccount_charges.push_back(charge_temp);
	}
	UNPACK_JSON_END
}

bool deduct_subaccount_charge_resp::pack(std::string& json)
{
	PACK_JSON_BEGIN
	pt_protocol.add(protocol_strings::STATUS, (int)status);
	boost::property_tree::ptree childen;
	for (size_t i = 0;i < company_account_balances.size();i++)
	{
		boost::property_tree::ptree child;
		company_account_balances.at(i).pack(child);
		childen.push_back(std::make_pair("",child));
	}
	pt_protocol.add_child(protocol_strings::COMPANY_ACCOUNT_BALANCE_LIST, childen);
	pt_root.add_child(protocol_strings::RESPONSE, pt_protocol);
	PACK_JSON_END
}

bool deduct_subaccount_charge_resp::unpack(const std::string& json)
{
	UNPACK_JSON_BEGIN
	pt_protocol = pt_root.get_child(protocol_strings::RESPONSE);
	status = (PROTOCOL_RESP_STATUS)pt_protocol.get<int>(protocol_strings::STATUS);
	boost::property_tree::ptree children = pt_protocol.get_child(protocol_strings::COMPANY_ACCOUNT_BALANCE_LIST);		
	BOOST_FOREACH(boost::property_tree::ptree::value_type &v, children)
	{
		boost::property_tree::ptree p = v.second;
		company_account_balance balance_temp;
		balance_temp.unpack(p);
		company_account_balances.push_back(balance_temp);
	}
	UNPACK_JSON_END
}

void route_charge::pack(boost::property_tree::ptree& pt)
{
	pt.add(protocol_strings::ROUTE_ID, route_id);
	pt.add(protocol_strings::PROVIDER_ACCOUNT_ID, provider_account_id);
	pt.add(protocol_strings::COUNT, count);
	pt.add(protocol_strings::MONEY, money);
	pt.add(protocol_strings::SETUP_TIME, setup_time);
}

void route_charge::unpack(boost::property_tree::ptree& pt)
{
	route_id = pt.get<std::string>(protocol_strings::ROUTE_ID);
	provider_account_id = pt.get<std::string>(protocol_strings::PROVIDER_ACCOUNT_ID);
	count = pt.get<int>(protocol_strings::COUNT);
	money = pt.get<double>(protocol_strings::MONEY);
	setup_time = pt.get<boost::uint64_t>(protocol_strings::SETUP_TIME);
}

void provider_account_balance::pack(boost::property_tree::ptree& pt)
{
	//pt.add(protocol_strings::ROUTE_ID, route_id);
	//pt.add(protocol_strings::PROVIDER_ACCOUNT_ID, provider_account_id);
	pt.add(protocol_strings::STATISTICS_BELONGINGNESS, statistics_belongingness);
	pt.add(protocol_strings::ID, id);
	pt.add(protocol_strings::RECHARGE_MODE,recharge_mode);
	pt.add(protocol_strings::BALANCE_MONEY, balance_money);
	pt.add(protocol_strings::BALANCE_COUNT, balance_count);
}

void provider_account_balance::unpack(boost::property_tree::ptree& pt)
{
	//route_id = pt.get<std::string>(protocol_strings::ROUTE_ID);
	//provider_account_id = pt.get<std::string>(protocol_strings::PROVIDER_ACCOUNT_ID);
	statistics_belongingness = (STATISTICS_BELONGINGNESS)pt.get<int>(protocol_strings::STATISTICS_BELONGINGNESS);
	id = pt.get<std::string>(protocol_strings::ID);
	recharge_mode = (RECHARGE_MODE)pt.get<int>(protocol_strings::RECHARGE_MODE);
	balance_money = pt.get<double>(protocol_strings::BALANCE_MONEY);
	balance_count = pt.get<int>(protocol_strings::BALANCE_COUNT);
}

bool deduct_route_charge_req::pack(std::string& json)
{
	PACK_JSON_BEGIN
	pt_protocol.add(protocol_strings::COMMAND, protocol_commands::DEDUCT_ROUTE_CHARGE);
	boost::property_tree::ptree childen;
	for (int i = 0;i < route_charges.size();i++)
	{
		boost::property_tree::ptree child;
		route_charges.at(i).pack(child);
		childen.push_back(make_pair("",child));
	}
	pt_protocol.add_child(protocol_strings::ROUTE_CHARGE_LIST,childen);
	pt_root.add_child(protocol_strings::REQUEST, pt_protocol);
	PACK_JSON_END
}

bool deduct_route_charge_req::unpack(const std::string& json)
{
	UNPACK_JSON_BEGIN
	pt_protocol = pt_root.get_child(protocol_strings::REQUEST);
	boost::property_tree::ptree children = pt_protocol.get_child(protocol_strings::ROUTE_CHARGE_LIST);		
	BOOST_FOREACH(boost::property_tree::ptree::value_type &v, children)
	{
		boost::property_tree::ptree p = v.second;
		route_charge charge_temp;
		charge_temp.unpack(p);
		route_charges.push_back(charge_temp);
	}
	UNPACK_JSON_END
}

bool deduct_route_charge_resp::pack(std::string& json)
{
	PACK_JSON_BEGIN
	pt_protocol.add(protocol_strings::STATUS, (int)status);
	boost::property_tree::ptree childen;
	for (size_t i = 0;i < provider_account_balances.size();i++)
	{
		boost::property_tree::ptree child;
		provider_account_balances.at(i).pack(child);
		childen.push_back(std::make_pair("",child));
	}
	pt_protocol.add_child(protocol_strings::PROVIDER_ACCOUNT_BALANCE_LIST, childen);
	pt_root.add_child(protocol_strings::RESPONSE, pt_protocol);
	PACK_JSON_END
}

bool deduct_route_charge_resp::unpack(const std::string& json)
{
	UNPACK_JSON_BEGIN
	pt_protocol = pt_root.get_child(protocol_strings::RESPONSE);
	status = (PROTOCOL_RESP_STATUS)pt_protocol.get<int>(protocol_strings::STATUS);
	boost::property_tree::ptree children = pt_protocol.get_child(protocol_strings::PROVIDER_ACCOUNT_BALANCE_LIST);		
	BOOST_FOREACH(boost::property_tree::ptree::value_type &v, children)
	{
		boost::property_tree::ptree p = v.second;
		provider_account_balance balance_temp;
		balance_temp.unpack(p);
		provider_account_balances.push_back(balance_temp);
	}
	UNPACK_JSON_END
}

void sensitive_word::pack(boost::property_tree::ptree& pt)
{
	pt.add(protocol_strings::ID, id);
	pt.add(protocol_strings::TYPE,type);
	std::string content_base64 = base64_encode((const unsigned char *)content.c_str(),(unsigned int)content.length());
	pt.add(protocol_strings::CONTENT,content_base64);
}

void sensitive_word::unpack(boost::property_tree::ptree& pt)
{
	id = pt.get<std::string>(protocol_strings::ID);
	type = (SENSITIVE_WORD_TYPE)pt.get<int>(protocol_strings::TYPE);
	std::string content_base64 = pt.get<std::string>(protocol_strings::CONTENT);
	std::vector<unsigned char> v_name = base64_decode(content_base64);
	content = std::string(v_name.begin(),v_name.end());
}

bool add_sensitive_word_req::pack(std::string& json)
{
	PACK_JSON_BEGIN
	pt_protocol.add(protocol_strings::COMMAND, protocol_commands::ADD_SENSITIVE_WORD);
	sensitive_word_.pack(pt_protocol);
	pt_root.add_child(protocol_strings::REQUEST, pt_protocol);
	PACK_JSON_END
}

bool add_sensitive_word_req::unpack(const std::string& json)
{
	UNPACK_JSON_BEGIN
	pt_protocol = pt_root.get_child(protocol_strings::REQUEST);
	sensitive_word_.unpack(pt_protocol);
	UNPACK_JSON_END
}

bool add_sensitive_word_resp::pack(std::string& json)
{
	PACK_JSON_BEGIN
	pt_protocol.add(protocol_strings::STATUS, (int)status);
	pt_root.add_child(protocol_strings::RESPONSE, pt_protocol);
	PACK_JSON_END
}

bool add_sensitive_word_resp::unpack(const std::string& json)
{
	UNPACK_JSON_BEGIN
	pt_protocol = pt_root.get_child(protocol_strings::RESPONSE);
	status = (PROTOCOL_RESP_STATUS)pt_protocol.get<int>(protocol_strings::STATUS);
	UNPACK_JSON_END
}

bool get_sensitive_word_list_req::pack(std::string& json)
{
	PACK_JSON_BEGIN
	pt_protocol.add(protocol_strings::COMMAND, protocol_commands::GET_SENSITIVE_WORD_LIST);
	pt_root.add_child(protocol_strings::REQUEST, pt_protocol);
	PACK_JSON_END
}

bool get_sensitive_word_list_req::unpack(const std::string& json)
{
	UNPACK_JSON_BEGIN
	pt_protocol = pt_root.get_child(protocol_strings::REQUEST);
	UNPACK_JSON_END
}

bool get_sensitive_word_list_resp::pack(std::string& json)
{
	PACK_JSON_BEGIN
	pt_protocol.add(protocol_strings::STATUS, (int)status);
	boost::property_tree::ptree childen;
	for (size_t i = 0;i < sensitive_words.size();i++)
	{
		boost::property_tree::ptree child;
		sensitive_words.at(i).pack(child);
		childen.push_back(std::make_pair("",child));
	}
	pt_protocol.add_child(protocol_strings::SENSITIVE_WORD_LIST, childen);
	pt_root.add_child(protocol_strings::RESPONSE, pt_protocol);
	PACK_JSON_END	
}

bool get_sensitive_word_list_resp::unpack(const std::string& json)
{
	UNPACK_JSON_BEGIN
	pt_protocol = pt_root.get_child(protocol_strings::RESPONSE);
	status = (PROTOCOL_RESP_STATUS)pt_protocol.get<int>(protocol_strings::STATUS);
	boost::property_tree::ptree children = pt_protocol.get_child(protocol_strings::SENSITIVE_WORD_LIST);		
	BOOST_FOREACH(boost::property_tree::ptree::value_type &v, children)
	{
		boost::property_tree::ptree p = v.second;
		sensitive_word sensitive_word_temp;
		sensitive_word_temp.unpack(p);
		sensitive_words.push_back(sensitive_word_temp);
	}
	UNPACK_JSON_END
}

void black_number::pack(boost::property_tree::ptree& pt)
{
	pt.add(protocol_strings::ID, id);
	pt.add(protocol_strings::NUMBER,number);
}

void black_number::unpack(boost::property_tree::ptree& pt)
{
	id = pt.get<std::string>(protocol_strings::ID);
	number = pt.get<std::string>(protocol_strings::NUMBER);
}

bool add_black_number_req::pack(std::string& json)
{
	PACK_JSON_BEGIN
	pt_protocol.add(protocol_strings::COMMAND, protocol_commands::ADD_BLACK_NUMBER);
	black_number_.pack(pt_protocol);
	pt_root.add_child(protocol_strings::REQUEST, pt_protocol);
	PACK_JSON_END
}

bool add_black_number_req::unpack(const std::string& json)
{
	UNPACK_JSON_BEGIN
	pt_protocol = pt_root.get_child(protocol_strings::REQUEST);
	black_number_.unpack(pt_protocol);
	UNPACK_JSON_END
}

bool add_black_number_resp::pack(std::string& json)
{
	PACK_JSON_BEGIN
	pt_protocol.add(protocol_strings::STATUS, (int)status);
	pt_root.add_child(protocol_strings::RESPONSE, pt_protocol);
	PACK_JSON_END
}

bool add_black_number_resp::unpack(const std::string& json)
{
	UNPACK_JSON_BEGIN
	pt_protocol = pt_root.get_child(protocol_strings::RESPONSE);
	status = (PROTOCOL_RESP_STATUS)pt_protocol.get<int>(protocol_strings::STATUS);
	UNPACK_JSON_END
}

bool get_black_number_list_req::pack(std::string& json)
{
	PACK_JSON_BEGIN
	pt_protocol.add(protocol_strings::COMMAND, protocol_commands::GET_BLACK_NUMBER_LIST);
	pt_root.add_child(protocol_strings::REQUEST, pt_protocol);
	PACK_JSON_END
}

bool get_black_number_list_req::unpack(const std::string& json)
{
	UNPACK_JSON_BEGIN
	pt_protocol = pt_root.get_child(protocol_strings::REQUEST);
	UNPACK_JSON_END
}

bool get_black_number_list_resp::pack(std::string& json)
{
	PACK_JSON_BEGIN
	pt_protocol.add(protocol_strings::STATUS, (int)status);
	boost::property_tree::ptree childen;
	for (size_t i = 0;i < black_numbers.size();i++)
	{
		boost::property_tree::ptree child;
		black_numbers.at(i).pack(child);
		childen.push_back(std::make_pair("",child));
	}
	pt_protocol.add_child(protocol_strings::BLACK_NUMBER_LIST, childen);
	pt_root.add_child(protocol_strings::RESPONSE, pt_protocol);
	PACK_JSON_END	
}

bool get_black_number_list_resp::unpack(const std::string& json)
{
	UNPACK_JSON_BEGIN
	pt_protocol = pt_root.get_child(protocol_strings::RESPONSE);
	status = (PROTOCOL_RESP_STATUS)pt_protocol.get<int>(protocol_strings::STATUS);
	boost::property_tree::ptree children = pt_protocol.get_child(protocol_strings::BLACK_NUMBER_LIST);		
	BOOST_FOREACH(boost::property_tree::ptree::value_type &v, children)
	{
		boost::property_tree::ptree p = v.second;
		black_number black_number_temp;
		black_number_temp.unpack(p);
		black_numbers.push_back(black_number_temp);
	}
	UNPACK_JSON_END
}

void high_frequency_white_number::pack(boost::property_tree::ptree& pt)
{
	pt.add(protocol_strings::ID, id);
	pt.add(protocol_strings::NUMBER,number);
}

void high_frequency_white_number::unpack(boost::property_tree::ptree& pt)
{
	id = pt.get<std::string>(protocol_strings::ID);
	number = pt.get<std::string>(protocol_strings::NUMBER);
}

bool add_high_frequency_white_number_req::pack(std::string& json)
{
	PACK_JSON_BEGIN
	pt_protocol.add(protocol_strings::COMMAND, protocol_commands::ADD_HIGH_FREQUENCY_WHITE_NUMBER);
	high_white_number.pack(pt_protocol);
	pt_root.add_child(protocol_strings::REQUEST, pt_protocol);
	PACK_JSON_END
}

bool add_high_frequency_white_number_req::unpack(const std::string& json)
{
	UNPACK_JSON_BEGIN
	pt_protocol = pt_root.get_child(protocol_strings::REQUEST);
	high_white_number.unpack(pt_protocol);
	UNPACK_JSON_END
}

bool add_high_frequency_white_number_resp::pack(std::string& json)
{
	PACK_JSON_BEGIN
	pt_protocol.add(protocol_strings::STATUS, (int)status);
	pt_root.add_child(protocol_strings::RESPONSE, pt_protocol);
	PACK_JSON_END
}

bool add_high_frequency_white_number_resp::unpack(const std::string& json)
{
	UNPACK_JSON_BEGIN
	pt_protocol = pt_root.get_child(protocol_strings::RESPONSE);
	status = (PROTOCOL_RESP_STATUS)pt_protocol.get<int>(protocol_strings::STATUS);
	UNPACK_JSON_END
}

bool get_high_frequency_white_number_list_req::pack(std::string& json)
{
	PACK_JSON_BEGIN
	pt_protocol.add(protocol_strings::COMMAND, protocol_commands::GET_HIGH_FREQUENCY_WHITE_NUMBER_LIST);
	pt_root.add_child(protocol_strings::REQUEST, pt_protocol);
	PACK_JSON_END
}

bool get_high_frequency_white_number_list_req::unpack(const std::string& json)
{
	UNPACK_JSON_BEGIN
	pt_protocol = pt_root.get_child(protocol_strings::REQUEST);
	UNPACK_JSON_END
}

bool get_high_frequency_white_number_list_resp::pack(std::string& json)
{
	PACK_JSON_BEGIN
	pt_protocol.add(protocol_strings::STATUS, (int)status);
	boost::property_tree::ptree childen;
	for (size_t i = 0;i < high_white_numbers.size();i++)
	{
		boost::property_tree::ptree child;
		high_white_numbers.at(i).pack(child);
		childen.push_back(std::make_pair("",child));
	}
	pt_protocol.add_child(protocol_strings::HIGH_FREQUENCY_WHITE_NUMBER_LSIT, childen);
	pt_root.add_child(protocol_strings::RESPONSE, pt_protocol);
	PACK_JSON_END	
}

bool get_high_frequency_white_number_list_resp::unpack(const std::string& json)
{
	UNPACK_JSON_BEGIN
	pt_protocol = pt_root.get_child(protocol_strings::RESPONSE);
	status = (PROTOCOL_RESP_STATUS)pt_protocol.get<int>(protocol_strings::STATUS);
	boost::property_tree::ptree children = pt_protocol.get_child(protocol_strings::HIGH_FREQUENCY_WHITE_NUMBER_LSIT);		
	BOOST_FOREACH(boost::property_tree::ptree::value_type &v, children)
	{
		boost::property_tree::ptree p = v.second;
		high_frequency_white_number white_number_temp;
		white_number_temp.unpack(p);
		high_white_numbers.push_back(white_number_temp);
	}
	UNPACK_JSON_END
}

void all_list::pack(boost::property_tree::ptree& pt)
{
	{
		boost::property_tree::ptree childen;
		for (size_t i = 0;i < provier_accounts.size();i++)
		{
			boost::property_tree::ptree child;
			provier_accounts.at(i).pack(child);
			childen.push_back(std::make_pair("",child));
		}
		pt.add_child(protocol_strings::PROVIDER_ACCOUNT_LIST, childen);
	}

	{
		boost::property_tree::ptree childen;
		for (size_t i = 0;i < routes.size();i++)
		{
			boost::property_tree::ptree child;
			routes.at(i).pack(child);
			childen.push_back(std::make_pair("",child));
		}
		pt.add_child(protocol_strings::ROUTE_LIST, childen);
	}

	{
		boost::property_tree::ptree childen;
		for (size_t i = 0;i < company_accounts.size();i++)
		{
			boost::property_tree::ptree child;
			company_accounts.at(i).pack(child);
			childen.push_back(std::make_pair("",child));
		}
		pt.add_child(protocol_strings::COMPANY_ACCOUNT_LIST, childen);
	}

	{
		boost::property_tree::ptree childen;
		for (size_t i = 0;i < subaccounts.size();i++)
		{
			boost::property_tree::ptree child;
			subaccounts.at(i).pack(child);
			childen.push_back(std::make_pair("",child));
		}
		pt.add_child(protocol_strings::SUBACCOUNT_LIST, childen);
	}

	{
		boost::property_tree::ptree childen;
		for (size_t i = 0;i < subaccount_routes.size();i++)
		{
			boost::property_tree::ptree child;
			subaccount_routes.at(i).pack(child);
			childen.push_back(std::make_pair("",child));
		}
		pt.add_child(protocol_strings::SUBACCOUNT_ROUTE_LIST, childen);
	}

	{
		boost::property_tree::ptree childen;
		for (size_t i = 0;i < templates.size();i++)
		{
			boost::property_tree::ptree child;
			templates.at(i).pack(child);
			childen.push_back(std::make_pair("",child));
		}
		pt.add_child(protocol_strings::TEMPLATE_LIST, childen);
	}

#if 0
	{
		boost::property_tree::ptree childen;
		for (size_t i = 0;i < route_signatures.size();i++)
		{
			boost::property_tree::ptree child;
			route_signatures.at(i).pack(child);
			childen.push_back(std::make_pair("",child));
		}
		pt.add_child(protocol_strings::ROUTE_SIGNATURE_LIST, childen);
	}

	{
		boost::property_tree::ptree childen;
		for (size_t i = 0;i < subaccount_signatures.size();i++)
		{
			boost::property_tree::ptree child;
			subaccount_signatures.at(i).pack(child);
			childen.push_back(std::make_pair("",child));
		}
		pt.add_child(protocol_strings::SUBACCOUNT_SIGNATURE_LIST, childen);
	}
#endif

	{
		boost::property_tree::ptree childen;
		for (size_t i = 0;i < route_feerates.size();i++)
		{
			boost::property_tree::ptree child;
			route_feerates.at(i).pack(child);
			childen.push_back(std::make_pair("",child));
		}
		pt.add_child(protocol_strings::ROUTE_FEERATE_LIST, childen);
	}

	{
		boost::property_tree::ptree childen;
		for (size_t i = 0;i < subaccount_feerates.size();i++)
		{
			boost::property_tree::ptree child;
			subaccount_feerates.at(i).pack(child);
			childen.push_back(std::make_pair("",child));
		}
		pt.add_child(protocol_strings::SUBACCOUNT_FEERATE_LIST, childen);
	}

	{
		boost::property_tree::ptree childen;
		for (size_t i = 0;i < sensitive_words.size();i++)
		{
			boost::property_tree::ptree child;
			sensitive_words.at(i).pack(child);
			childen.push_back(std::make_pair("",child));
		}
		pt.add_child(protocol_strings::SENSITIVE_WORD_LIST, childen);
	}

	{
		boost::property_tree::ptree childen;
		for (size_t i = 0;i < black_numbers.size();i++)
		{
			boost::property_tree::ptree child;
			black_numbers.at(i).pack(child);
			childen.push_back(std::make_pair("",child));
		}
		pt.add_child(protocol_strings::BLACK_NUMBER_LIST, childen);
	}

	{
		boost::property_tree::ptree childen;
		for (size_t i = 0;i < high_frequency_white_numbers.size();i++)
		{
			boost::property_tree::ptree child;
			high_frequency_white_numbers.at(i).pack(child);
			childen.push_back(std::make_pair("",child));
		}
		pt.add_child(protocol_strings::HIGH_FREQUENCY_WHITE_NUMBER_LSIT, childen);
	}
}

void all_list::unpack(boost::property_tree::ptree& pt)
{
	{
		boost::property_tree::ptree children = pt.get_child(protocol_strings::PROVIDER_ACCOUNT_LIST);		
		BOOST_FOREACH(boost::property_tree::ptree::value_type &v, children)
		{
			boost::property_tree::ptree p = v.second;
			provider_account pro_account;
			pro_account.unpack(p);
			provier_accounts.push_back(pro_account);
		}
	}

	{
		boost::property_tree::ptree children = pt.get_child(protocol_strings::ROUTE_LIST);		
		BOOST_FOREACH(boost::property_tree::ptree::value_type &v, children)
		{
			boost::property_tree::ptree p = v.second;
			route_attribute route;
			route.unpack(p);
			routes.push_back(route);
		}
	}

	{
		boost::property_tree::ptree children = pt.get_child(protocol_strings::COMPANY_ACCOUNT_LIST);		
		BOOST_FOREACH(boost::property_tree::ptree::value_type &v, children)
		{
			boost::property_tree::ptree p = v.second;
			company_account com_account;
			com_account.unpack(p);
			company_accounts.push_back(com_account);
		}
	}

	{
		boost::property_tree::ptree children = pt.get_child(protocol_strings::SUBACCOUNT_LIST);		
		BOOST_FOREACH(boost::property_tree::ptree::value_type &v, children)
		{
			boost::property_tree::ptree p = v.second;
			subaccount_base_information sub_account;
			sub_account.unpack(p);
			subaccounts.push_back(sub_account);
		}
	}

	{
		boost::property_tree::ptree children = pt.get_child(protocol_strings::SUBACCOUNT_ROUTE_LIST);		
		BOOST_FOREACH(boost::property_tree::ptree::value_type &v, children)
		{
			boost::property_tree::ptree p = v.second;
			subaccount_route subaccount_route_temp;
			subaccount_route_temp.unpack(p);
			subaccount_routes.push_back(subaccount_route_temp);
		}
	}

	{
		boost::property_tree::ptree children = pt.get_child(protocol_strings::TEMPLATE_LIST);		
		BOOST_FOREACH(boost::property_tree::ptree::value_type &v, children)
		{
			boost::property_tree::ptree p = v.second;
			single_template template_temp;
			template_temp.unpack(p);
			templates.push_back(template_temp);
		}
	}

#if 0
	{
		boost::property_tree::ptree children = pt.get_child(protocol_strings::ROUTE_SIGNATURE_LIST);		
		BOOST_FOREACH(boost::property_tree::ptree::value_type &v, children)
		{
			boost::property_tree::ptree p = v.second;
			signature signature_temp;
			signature_temp.unpack(p);
			route_signatures.push_back(signature_temp);
		}
	}

	{
		boost::property_tree::ptree children = pt.get_child(protocol_strings::SUBACCOUNT_SIGNATURE_LIST);		
		BOOST_FOREACH(boost::property_tree::ptree::value_type &v, children)
		{
			boost::property_tree::ptree p = v.second;
			signature signature_temp;
			signature_temp.unpack(p);
			subaccount_signatures.push_back(signature_temp);
		}
	}
#endif

	{
		boost::property_tree::ptree children = pt.get_child(protocol_strings::ROUTE_FEERATE_LIST);		
		BOOST_FOREACH(boost::property_tree::ptree::value_type &v, children)
		{
			boost::property_tree::ptree p = v.second;
			feerate feerate_temp;
			feerate_temp.unpack(p);
			route_feerates.push_back(feerate_temp);
		}
	}

	{
		boost::property_tree::ptree children = pt.get_child(protocol_strings::SUBACCOUNT_FEERATE_LIST);		
		BOOST_FOREACH(boost::property_tree::ptree::value_type &v, children)
		{
			boost::property_tree::ptree p = v.second;
			feerate feerate_temp;
			feerate_temp.unpack(p);
			subaccount_feerates.push_back(feerate_temp);
		}
	}

	{
		boost::property_tree::ptree children = pt.get_child(protocol_strings::SENSITIVE_WORD_LIST);		
		BOOST_FOREACH(boost::property_tree::ptree::value_type &v, children)
		{
			boost::property_tree::ptree p = v.second;
			sensitive_word sensitive_word_temp;
			sensitive_word_temp.unpack(p);
			sensitive_words.push_back(sensitive_word_temp);
		}
	}

	{
		boost::property_tree::ptree children = pt.get_child(protocol_strings::BLACK_NUMBER_LIST);		
		BOOST_FOREACH(boost::property_tree::ptree::value_type &v, children)
		{
			boost::property_tree::ptree p = v.second;
			black_number black_number_temp;
			black_number_temp.unpack(p);
			black_numbers.push_back(black_number_temp);
		}
	}

	{
		boost::property_tree::ptree children = pt.get_child(protocol_strings::HIGH_FREQUENCY_WHITE_NUMBER_LSIT);		
		BOOST_FOREACH(boost::property_tree::ptree::value_type &v, children)
		{
			boost::property_tree::ptree p = v.second;
			high_frequency_white_number high_frequency_white_number_temp;
			high_frequency_white_number_temp.unpack(p);
			high_frequency_white_numbers.push_back(high_frequency_white_number_temp);
		}
	}
}

bool get_all_list_req::pack(std::string& json)
{
	PACK_JSON_BEGIN
	pt_protocol.add(protocol_strings::COMMAND, protocol_commands::GET_ALL_LIST);
	pt_root.add_child(protocol_strings::REQUEST, pt_protocol);
	PACK_JSON_END
}

bool get_all_list_req::unpack(const std::string& json)
{
	UNPACK_JSON_BEGIN
	pt_protocol = pt_root.get_child(protocol_strings::REQUEST);
	UNPACK_JSON_END
}

bool get_all_list_resp::pack(std::string& json)
{
	PACK_JSON_BEGIN
	pt_protocol.add(protocol_strings::STATUS, (int)status);
	all_list_.pack(pt_protocol);

	pt_root.add_child(protocol_strings::RESPONSE, pt_protocol);
	PACK_JSON_END
}

bool get_all_list_resp::unpack(const std::string& json)
{
	UNPACK_JSON_BEGIN
	pt_protocol = pt_root.get_child(protocol_strings::RESPONSE);
	status = (PROTOCOL_RESP_STATUS)pt_protocol.get<int>(protocol_strings::STATUS);
	all_list_.unpack(pt_protocol);
	UNPACK_JSON_END
}
