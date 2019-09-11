#ifndef _PROTOCOL_STRUCTURE__H_
#define _PROTOCOL_STRUCTURE__H_

#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <boost/cstdint.hpp>
#include <boost/foreach.hpp>
#define BOOST_SPIRIT_THREADSAFE
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "logger.hpp"
//#include "algorithm.hpp"

#define PACK_JSON_BEGIN \
	boost::property_tree::ptree pt_root;\
	boost::property_tree::ptree pt_protocol;\

#define PACK_JSON_END \
	std::stringstream ss;\
	boost::property_tree::write_json(ss, pt_root);\
	json = ss.str();\
	return true; \

#define UNPACK_JSON_BEGIN \
	boost::property_tree::ptree parser;\
	std::istringstream iss;\
	iss.str(json.c_str());\
	boost::property_tree::ptree pt_root;\
	boost::property_tree::ptree pt_protocol; \
	try\
{\
	boost::property_tree::json_parser::read_json(iss, pt_root);\


#define UNPACK_JSON_END \
	return true;\
}\
	catch(boost::property_tree::json_parser_error& e)\
{\
	logger_log(__FUNCTION__, LEVEL_ERROR,"exception: %s, file: %s", e.what(), json.c_str());\
	return false;\
}\
	catch(std::exception& e)\
{\
	logger_log(__FUNCTION__, LEVEL_ERROR,"exception: %s, file: %s", e.what(), json.c_str());\
	return false;\
}\

class protocol_strings
{
public:
	static std::string REQUEST;
	static std::string RESPONSE;
	static std::string COMMAND;
	static std::string ID;
	static std::string PROVIDER_ACCOUNT_ID;
	static std::string USERNAME;
	static std::string PASSWORD;
	static std::string STATUS;
	static std::string STATE;
	static std::string COMPANY_ACCOUNT_LIST;
	static std::string SUBACCOUNT_LIST;
	//static std::string SUBACCOUNT_BASE_INFORMATION_LIST;
	static std::string PROVIDER_ACCOUNT_LIST;
	static std::string ROUTE_SIGNATURE_LIST;
	static std::string SUBACCOUNT_SIGNATURE_LIST;
	static std::string ROUTE_FEERATE_LIST;
	static std::string SUBACCOUNT_FEERATE_LIST;
	static std::string SUBACCOUNT_CHARGE_LIST;
	static std::string ROUTE_CHARGE_LIST;
	static std::string COMPANY_ACCOUNT_BALANCE_LIST;
	static std::string ROUTE_BALANCE_LIST;
	static std::string PROVIDER_ACCOUNT_BALANCE_LIST;
	static std::string SPECIFIED_NUMBER_LIST;
	static std::string SENSITIVE_WORD_LIST;
	static std::string BLACK_NUMBER_LIST;
	static std::string HIGH_FREQUENCY_WHITE_NUMBER_LSIT;
	static std::string SUBACCOUNT_ROUTE_LIST;
	static std::string NAME;
	static std::string DESCRIPTION;
	static std::string COMPANY_NAME;
	static std::string BALANCE;
	static std::string BALANCE_MONEY;
	static std::string BALANCE_COUNT;
	static std::string CREDIT_LINE;
	static std::string CREDIT_LINE_MONEY;
	static std::string CREDIT_LINE_COUNT;
	static std::string THRESHOLD_VALUE;
	static std::string THRESHOLD_VALUE_MONEY;
	static std::string THRESHOLD_VALUE_COUNT;
	static std::string TYPE;
	static std::string DAILY_CONSUME;
	static std::string MONTHLY_CONSUME;
	static std::string FUNCTION_TYPE;
	static std::string SUBACCOUNT_COUNT;
	static std::string SETUP_TIME;
	static std::string EFFECTIVE_TIME;
	static std::string MANAGER;
	static std::string COMPANY_ACCOUNT_USERNAME;
	static std::string SUBACCOUNT_USERNAME;
	static std::string IP_ADDRESS;
	static std::string PORT;
	static std::string SENDNUMBER_TYPE;
	static std::string SEND_NUMBER;
	static std::string RECEIVELIMIT_TYPE;
	static std::string RECEIVELIMIT_NETCODE;
	static std::string PROTOCOL_TYPE;
	static std::string INDUSTRY_TYPE;
	static std::string INDUSTRY;
	static std::string ENCODE_TYPE;
	static std::string ENCODE_SIZE;
	static std::string ASCALL_SIZE;
	static std::string UCS2_SIZE;
	static std::string TEMPLATES;
	static std::string SIGNATURE_TYPE;
	static std::string SIGNATURE_POSITION;
	static std::string CONTENT;
	static std::string SIGNATURE;
	static std::string ROUTE_LIST;
	static std::string TEMPLATE_LIST;
	static std::string SIGNATURE_LIST;
	static std::string FEERATE_LIST;
	static std::string INTERVAL_TIME;
	static std::string TIMES;
	static std::string CYCLE;
	static std::string RECEIVE_ACK_MAX_TIME;
	static std::string RATIO;
	static std::string DISABLE_CONNECT_MAX_TIME;
	static std::string PACKAGE_STATE;
	static std::string PACKAGE_COUNT;
	static std::string PACKAGE_USED_COUNT;
	static std::string PACKAGE_SURPLUS_COUNT;
	static std::string HEARTBEAT;
	static std::string BELONGINGNESS;
	static std::string MCC;
	static std::string MNC;
	static std::string MT_PRICE;
	static std::string MO_PRICE;
	static std::string CHARGE_MODE;
	static std::string ROUTE_PRIORITY;
	static std::string INDEX;
	static std::string INDEX_VALUE;
	static std::string LINKMAN;
	static std::string E_MAIL;
	static std::string PHONE_NUMBER;
	static std::string MASTER_ROUTE_ID;
	static std::string SLAVE_ROUTE_ID;
	static std::string SECOND_SLAVE_ROUTE_ID;
	static std::string ROUTE_ID;
	static std::string NUMBER;
	static std::string FEE;
	static std::string WHITE_LIST_STATE;
	static std::string DEVOUR_RATE;
	static std::string ALREADY_SEND_COUNT;
	static std::string MASTER_ROUTE_RATIO;
	static std::string SLAVE_ROUTE_RATIO;
	static std::string SECOND_SLAVE_ROUTE_RATIO;
	static std::string COUNT;
	static std::string MONEY;
	static std::string BALANCE_ADDRESS;
	static std::string RECHARGE_MODE;
	static std::string MAX_COUNT_PER_SECOND;
	static std::string STATISTICS_BELONGINGNESS;
	static std::string ACCESS_LEVEL;
	static std::string LOGIN_STATE;
	static std::string CREATOR;
	static std::string SPECIFIED_NUMBER;
	static std::string SPECIFIED_NUMBER_TYPE;
	static std::string SPLIT_STATE;
	static std::string BASE_INFO;
	static std::string LINK_INFO;
	static std::string DEVOUR_THRESHOLD;
	static std::string VERIFY_MODE;
	static std::string HIGH_FREQUENCY_THRESHOLD;
};


class protocol_commands
{
public:
	static std::string LOGIN;
	static std::string LOGOUT;
	static std::string GET_ALL_LIST;
	static std::string GET_COMACCOUNT;
	static std::string GET_COMACCOUNT_LIST;
	static std::string ADD_COMACCOUNT;
	static std::string EDIT_COMACCOUNT;
	static std::string GET_SUBACCOUNT;
	static std::string GET_SUBACCOUNT_LIST;
	static std::string GET_SUBACCOUNT_BASE_INFORMATION_LIST;
	static std::string ADD_SUBACCOUNT;
	static std::string DELETE_SUBACCOUNT;
	static std::string ADD_ROUTE;
	static std::string EDIT_ROUTE;
	static std::string DELETE_ROUTE;
	static std::string GET_ROUTE_LIST;
	static std::string CHOOSE_ROUTE_LIST;
	static std::string ADD_TEMPLATE;
	static std::string GET_TEMPLATE_LIST;
	static std::string SET_TEMPLATE_LIST;
	static std::string ADD_SIGNATURE;
	static std::string GET_SIGNATURE_LIST;
	static std::string SET_SIGNATURE_LIST;
	static std::string GET_PROACCOUNT_LIST;
	static std::string ADD_PROACCOUNT;
	static std::string GET_FEERATE_LIST;
	static std::string ADD_FEERATE;
	static std::string GET_SPECIFIED_NUMBER_LIST;
	static std::string ADD_SPECIFIED_NUMBER;
	static std::string ADD_SENSITIVE_WORD;
	static std::string GET_SENSITIVE_WORD_LIST;
	static std::string ADD_BLACK_NUMBER;
	static std::string GET_BLACK_NUMBER_LIST;
	static std::string ADD_HIGH_FREQUENCY_WHITE_NUMBER;
	static std::string GET_HIGH_FREQUENCY_WHITE_NUMBER_LIST;
	static std::string GET_SUBACCOUNT_ROUTE_LIST;
	static std::string DEDUCT_SUBACCOUNT_CHARGE;
	static std::string DEDUCT_ROUTE_CHARGE;
};
#endif

