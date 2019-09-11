#ifndef _DATA_STRUCTURE__H_
#define _DATA_STRUCTURE__H_
#include <string>
#include <vector>
#include <boost/cstdint.hpp>
#include <boost/property_tree/ptree.hpp>

enum DIALOG_MODE
{
	DIALOG_MODE_ADD = 0,
	DIALOG_MODE_EDIT,
	DIALOG_MODE_SHOW
};

enum DIALOG_TYPE
{
	DIALOG_TYPE_SUBACCOUNTS = 0,
	DIALOG_TYPE_ROUTETABLE,
	DIALOG_TYPE_TEMPLATES,
	DIALOG_TYPE_SIGNATURES,
	DIALOG_TYPE_FEERATETABLE
};

enum PROTOCOL_RESP_STATUS
{
	RESP_STATUS_OK = 0,
	RESP_STATUS_NETWORK_ERR,
	RESP_STATUS_JSON_FORMAT_ERR,
	RESP_STATUS_JSON_COMMAND_ERR,
	RESP_STATUS_JSON_CONTENT_ERR,

	RESP_STATUS_DATABASE_ERR = 5,

	RESP_STATUS_LOGIN_USERNAME_NOTEXIST = 100,
	RESP_STATUS_LOGIN_USERNAME_EXIST,
	RESP_STATUS_LONGIN_USERNAME_DISABLE,
	RESP_STATUS_LONGIN_PASSWD_ERR,
	RESP_STATUS_PROVIDER_ACCOUNT_ID_EXIST,

	RESP_STATUS_ROUTE_ID_EXIST = 105,
	RESP_STATUS_TEMPLATE_EXIST,
	RESP_STATUS_SIGNATURE_EXIST,
	RESP_STATUS_FEERATE_EXIST,
	RESP_STATUS_COMPANY_ACCOUNT_USERNAME_EXIST,

	RESP_STATUS_SUBACCOUNT_USERNAME_EXIST,
	
	RESP_STATUS_MAX
};

enum COMPANY_ACCOUNT_TYPE
{
	COMPANY_ACCOUNT_CUNTOMER = 0,
	COMPANY_ACCOUNT_PROVIDER
};

enum FUNCTION_TYPE
{
	FUNCTION_TYPE_MT = 0,
	FUNCTION_TYPE_2WAY,
	FUNCTION_TYPE_P2P
};

enum ACCOUNT_STATE
{
	ACCOUNT_STATE_CLOSE = 0,
	ACCOUNT_STATE_OPEN,
	ACCOUNT_STATE_SELECTED
};

enum CHARGE_MODE
{
	CHARGE_MODE_SUBMISSION = 0,
	CHARGE_MODE_SUCCESS
};

enum RECHARGE_MODE
{
	RECHARGE_MODE_MONEY = 0,
	RECHARGE_MODE_COUNT
};

enum PACKAGE_STATE
{
	PACKAGE_STATE_CLOSE = 0,
	PACKAGE_STATE_OPEN,
};

enum SEND_NUMBER_TYPE
{
	SEND_NUMBER_TYPE_DEFAULT = 0,
	SEND_NUMBER_TYPE_TRANSPARENT,
	SEND_NUMBER_TYPE_SPECIFIED
};

enum RECEIVE_LIMIT_TYPE
{
	RECEIVE_LIMIT_ADMIT = 0,
	RECEIVE_LIMIT_FORBID
};

enum ROUTE_PROTOCOL
{
	ROUTE_PROTOCOL_CMPP = 0,
	ROUTE_PROTOCOL_SMPP,
	ROUTE_PROTOCOL_SGIP
};

enum INDUSTRY_TYPE
{
	INDUSTRY_TYPE_SECURITY_CODE = 0,
	INDUSTRY_TYPE_NOTICE,
	INDUSTRY_TYPE_SUPERMARKET,
	INDUSTRY_TYPE_MARKETING,
	INDUSTRY_TYPE_FOURCLASS
};

enum ENCODE_TYPE
{
	ENCODE_TYPE_ASCAII = 0,
	ENCODE_TYPE_UCS2
};

enum SIGNATURE_TYPE
{
	SIGNATURE_TYPE_NO_SIGNATURE = 0,
	SIGNATURE_TYPE_SPECIFIED,
	SIGNATURE_TYPE_USER_DEFINED
};

enum SIGNATURE_POSITION
{
	SIGNATURE_POSITION_FRONT = 0,
	SIGNATURE_POSITION_BACK,
	SIGNATURE_POSITION_BOTH
};

enum SIGNATURE_BELONGINGNESS
{
	SIGNATURE_BELONGINGNESS_ROUTE = 0,
	SIGNATURE_BELONGINGNESS_SUBACCOUNT
};

enum TEMPLATE_BELONGINGNESS
{
	TEMPLATE_BELONGINGNESS_ROUTE = 0,
	TEMPLATE_BELONGINGNESS_SUBACCOUNT
};

enum FEERATE_BELONGINGNESS
{
	FEERATE_BELONGINGNESS_ROUTE = 0,
	FEERATE_BELONGINGNESS_SUBACCOUNT
};

enum STATISTICS_BELONGINGNESS
{
	STATISTICS_BELONGINGNESS_PROVIDER_ACCOUNT = 0,
	STATISTICS_BELONGINGNESS_ROUTE
};

enum ROUTE_TABLE_BELONGINGNESS
{
	ROUTE_TABLE_BELONGINGNESS_PROACCOUNT = 0,
	ROUTE_TABLE_BELONGINGNESS_SUBACCOUNT
};

enum SPECIFIED_NUMBER_BELONGINGNESS
{
	SPECIFIED_NUMBER_BELONGINGNESS_ROUTE = 0,
	SPECIFIED_NUMBER_BELONGINGNESS_SUBACCOUNT
};

enum ROUTE_PRIORITY
{
	ROUTE_PRIORITY_MASTER = 0,
	ROUTE_PRIORITY_SLAVE,
	ROUTE_PRIORITY_SECOND_SLAVE,

	ROUTE_PRIORITY_MAX
};

enum USING_STATE
{
	USING_STATE_CLOSE = 0,
	USING_STATE_OPEN
};

enum ACCESS_LEVEL
{
	ACCESS_LEVEL_ADMIN = 0,
	ACCESS_LEVEL_WRITE_READ,
	ACCESS_LEVEL_READ
};

enum LOGIN_STATE
{
	LOGIN_STATE_ONLINE = 0,
	LOGIN_STATE_OFFLINE
};

enum SPECIFIED_NUMBER_TYPE
{
	SPECIFIED_NUMBER_TYPE_WHOLE_NUMBER = 0,
	SPECIFIED_NUMBER_TYPE_PART_NUMBER	
};

enum SPLIT_STATE
{
	SPLIT_STATE_ABLE = 0,
	SPLIT_STATE_DISABLE
};

enum SENSITIVE_WORD_TYPE
{
	SENSITIVE_WORD_TYPE_AMBIGUOUS = 0,
	SENSITIVE_WORD_TYPE_ABSOLUTE
};

enum VERIFY_MODE
{
	VERIFY_MODE_NO = 0,
	VERIFY_MODE_YES
};

struct templates_key
{
	TEMPLATE_BELONGINGNESS belongingness;
	std::string value;
};

struct signatures_key
{
	SIGNATURE_BELONGINGNESS belongingness;
	std::string value;
};

struct feerates_key
{
	FEERATE_BELONGINGNESS belongingness;
	std::string value;
};

struct login_user
{
	std::string username;
	std::string password;
	std::string name;
	ACCESS_LEVEL access_level;
	LOGIN_STATE login_state;
	std::string creator;
	boost::uint64_t setup_time;
	void pack(boost::property_tree::ptree& pt);
	void unpack(boost::property_tree::ptree& pt);
};

struct login
{
	std::string username;
	std::string password;
	void pack(boost::property_tree::ptree& pt);
	void unpack(boost::property_tree::ptree& pt);
};

struct login_req
{
public:
	login login_;
	bool pack(std::string& json);
	bool unpack(const std::string& json);
};

struct login_resp
{
public:
	PROTOCOL_RESP_STATUS status;
	login_user login_user_;
	bool pack(std::string& json);
	bool unpack(const std::string& json);
};

struct logout_req
{
public:
	login login_;
	bool pack(std::string& json);
	bool unpack(const std::string& json);
};

struct logout_resp
{
public:
	PROTOCOL_RESP_STATUS status;
	bool pack(std::string& json);
	bool unpack(const std::string& json);
};

struct company_account
{
	std::string username;
	std::string password;
	std::string company_name;
	std::string description;
	ACCOUNT_STATE state;
	double balance_money;
	double credit_line_money;
	double daily_consume;
	boost::uint64_t setup_time;
	boost::uint64_t effective_time;
	std::string linkman;
	std::string phone_number;
	std::string e_mail;
	void pack(boost::property_tree::ptree& pt);
	void unpack(boost::property_tree::ptree& pt);
};

struct add_company_account_req
{
public:
	company_account account;
	bool pack(std::string& json);
	bool unpack(const std::string& json);
};

struct add_company_account_resp
{
public:
	PROTOCOL_RESP_STATUS status;
	bool pack(std::string& json);
	bool unpack(const std::string& json);
};

struct get_company_account_req
{
public:
	std::string username;
	bool pack(std::string& json);
	bool unpack(const std::string& json);
};

struct get_company_account_resp
{
public:
	PROTOCOL_RESP_STATUS status;
	company_account account;
	bool pack(std::string& json);
	bool unpack(const std::string& json);
};

struct edit_company_account_req
{
public:
	company_account account;
	bool pack(std::string& json);
	bool unpack(const std::string& json);
};

struct edit_company_account_resp
{
public:
	PROTOCOL_RESP_STATUS status;
	company_account account;
	bool pack(std::string& json);
	bool unpack(const std::string& json);
};

struct get_company_account_list_req
{
public:
	bool pack(std::string& json);
	bool unpack(const std::string& json);
};

struct get_company_account_list_resp
{
public:
	PROTOCOL_RESP_STATUS status;
	std::vector<company_account> accounts;
	bool pack(std::string& json);
	bool unpack(const std::string& json);
};

struct signature
{
	std::string id;
	std::string index;
	SIGNATURE_TYPE type;
	SIGNATURE_POSITION position;
	std::string content;
	void pack(boost::property_tree::ptree& pt);
	void unpack(boost::property_tree::ptree& pt);
};

struct add_signature_req
{
public:
	SIGNATURE_BELONGINGNESS belongingness;
	//std::string index_value;
	signature signature_;
	bool pack(std::string& json);
	bool unpack(const std::string& json);
};

struct add_signature_resp
{
public:
	PROTOCOL_RESP_STATUS status;
	bool pack(std::string& json);
	bool unpack(const std::string& json);
};

struct get_signature_list_req
{
public:
	SIGNATURE_BELONGINGNESS belongingness;
	std::string index_value;
	bool pack(std::string& json);
	bool unpack(const std::string& json);
};

struct get_signature_list_resp
{
public:
	PROTOCOL_RESP_STATUS status;
	std::vector<signature> signatures;
	bool pack(std::string& json);
	bool unpack(const std::string& json);
};

struct single_template
{
	std::string id;
	std::string index;
	ENCODE_TYPE code_type;
	std::string content;
	void pack(boost::property_tree::ptree& pt);
	void unpack(boost::property_tree::ptree& pt);
};

struct add_template_req
{
public:
	TEMPLATE_BELONGINGNESS belongingness;
	single_template template_;
	bool pack(std::string& json);
	bool unpack(const std::string& json);
};

struct add_template_resp
{
public:
	PROTOCOL_RESP_STATUS status;
	bool pack(std::string& json);
	bool unpack(const std::string& json);
};

struct get_template_list_req
{
public:
	TEMPLATE_BELONGINGNESS belongingness;
	std::string index_value;
	bool pack(std::string& json);
	bool unpack(const std::string& json);
};

struct get_template_list_resp
{
public:
	PROTOCOL_RESP_STATUS status;
	std::vector<single_template> templates;
	bool pack(std::string& json);
	bool unpack(const std::string& json);
};

#if 0
struct set_signature_list_req
{
public:
	SIGNATURE_BELONGINGNESS belongingness;
	std::string index_value;
	std::vector<signature> signatures;
	bool pack(std::string& json);
	bool unpack(const std::string& json);
};

struct set_signature_list_resp
{
public:
	PROTOCOL_RESP_STATUS status;
	bool pack(std::string& json);
	bool unpack(const std::string& json);
};

struct set_template_list_req
{
public:
	std::string route_id;
	std::vector<single_template> templates;
	bool pack(std::string& json);
	bool unpack(const std::string& json);
};

struct set_template_list_resp
{
public:
	PROTOCOL_RESP_STATUS status;
	bool pack(std::string& json);
	bool unpack(const std::string& json);
};
#endif

struct specified_number
{
	std::string id;
	std::string route_id;
	unsigned int number;
	SPECIFIED_NUMBER_TYPE type;
	USING_STATE state;
	std::string subaccout_user_name;
	void pack(boost::property_tree::ptree& pt);
	void unpack(boost::property_tree::ptree& pt);
};

struct add_specified_number_req
{
public:
	specified_number specified_number_;
	bool pack(std::string& json);
	bool unpack(const std::string& json);
};

struct add_specified_number_resp
{
public:
	PROTOCOL_RESP_STATUS status;
	bool pack(std::string& json);
	bool unpack(const std::string& json);
};

struct get_specified_number_list_req
{
public:
	std::string route_id;
	bool pack(std::string& json);
	bool unpack(const std::string& json);
};

struct get_specified_number_list_resp
{
public:
	PROTOCOL_RESP_STATUS status;
	std::vector<specified_number> specified_numbers;
	bool pack(std::string& json);
	bool unpack(const std::string& json);
};

struct subaccount_route
{
	std::string username;
	std::string route_id;
	ROUTE_PRIORITY route_priority;	
	//SPECIFIED_NUMBER_TYPE specified_number_type;
	//unsigned int number;
	int ratio;
	specified_number specified_number_;
	signature signature_;
	void pack(boost::property_tree::ptree& pt);
	void unpack(boost::property_tree::ptree& pt);
};

struct get_subaccount_route_list_req
{
public:
	std::string username;
	bool pack(std::string& json);
	bool unpack(const std::string& json);
};

struct get_subaccount_route_list_resp
{
public:
	PROTOCOL_RESP_STATUS status;
	std::vector<subaccount_route> subaccout_routes;
	bool pack(std::string& json);
	bool unpack(const std::string& json);
};

struct link_information
{
	std::string linkman;
	std::string phone_number;
	std::string e_mail;
	void pack(boost::property_tree::ptree& pt);
	void unpack(boost::property_tree::ptree& pt);
};

struct subaccount_base_information
{
	std::string username;
	std::string password;
	std::string description;
	std::string company_account_username;
	FUNCTION_TYPE function_type;
	ACCOUNT_STATE state;
	CHARGE_MODE charge_mode;
	SEND_NUMBER_TYPE send_number_type;
	std::string send_number;
	RECEIVE_LIMIT_TYPE receive_limit_type;
	std::string receive_limit_netcode;
	double daily_consume;
	double monthly_consume;
	std::string ip_address;
	//std::string linkman;
	//std::string phone_number;
	//std::string e_mail;
	//std::string master_route_id;
	//std::string slave_route_id;
	//std::string second_salve_route_id;
	//int master_route_ratio;
	//int slave_route_ratio;
	//int second_slave_route_ratio;
	USING_STATE white_list_state;
	double devour_rate;
	unsigned int devoure_threshold;
	unsigned int already_send_count;
	SPLIT_STATE  split_state;
	VERIFY_MODE verify_mode;
	unsigned int high_frequency_threshold;
	void pack(boost::property_tree::ptree& pt);
	void unpack(boost::property_tree::ptree& pt);
};

struct get_subaccount_base_information_list_req
{
public:
	std::string company_account_username;
	bool pack(std::string& json);
	bool unpack(const std::string& json);
};

struct get_subaccount_base_information_list_resp
{
public:
	PROTOCOL_RESP_STATUS status;
	std::vector<subaccount_base_information> subaccounts;
	bool pack(std::string& json);
	bool unpack(const std::string& json);
};

struct subaccount
{
	subaccount_base_information base_info;
	link_information link_info;
	std::vector<subaccount_route> routes;
	//std::vector<signature> signatures;
	void pack(boost::property_tree::ptree& pt);
	void unpack(boost::property_tree::ptree& pt);
};

struct add_subaccount_req
{
public:
	subaccount sub_account;
	bool pack(std::string& json);
	bool unpack(const std::string& json);
};

struct add_subaccount_resp
{
public:
	PROTOCOL_RESP_STATUS status;
	bool pack(std::string& json);
	bool unpack(const std::string& json);
};

struct get_subaccount_list_req
{
public:
	std::string company_account_username;
	bool pack(std::string& json);
	bool unpack(const std::string& json);
};

struct get_subaccount_list_resp
{
public:
	PROTOCOL_RESP_STATUS status;
	std::vector<subaccount> subaccounts;
	bool pack(std::string& json);
	bool unpack(const std::string& json);
};

struct route_disconnect
{
	int interval_time;
	int times;
	void pack(boost::property_tree::ptree& pt);
	void unpack(boost::property_tree::ptree& pt);
};

struct route_statistics
{
	int cycle;
	int receive_ack_max_time;
	double ratio;
	void pack(boost::property_tree::ptree& pt);
	void unpack(boost::property_tree::ptree& pt);
};

struct route_attribute
{
	std::string id;
	std::string provider_account_id;
	std::string username;
	std::string password;
	std::string description;
	FUNCTION_TYPE function_type;
	ROUTE_PROTOCOL protocol_type;
	ACCOUNT_STATE state;
	CHARGE_MODE charge_mode;
	std::string ip_address;
	int port;
	double heartbeat;
	std::string company_name;	
	STATISTICS_BELONGINGNESS statistics_belongingness;
	RECHARGE_MODE recharge_mode;
	//double balance;
	double balance_money;
	int balance_count;
	std::string balance_address;
	//double credit_line;
	double credit_line_money;
	unsigned int credit_line_count;
	//double threshold_value;
	double threshold_value_money;
	unsigned int threshold_value_count;
	std::string industry;
	int ascall_size;
	int ucs2_size;
	int max_count_per_second;
	PACKAGE_STATE package_state;
	int package_count;
	int package_used_count;
	int package_surplus_count;
	int disable_connect_max_time;
	route_disconnect route_disconnect_;
	route_statistics route_statistics_;
	SIGNATURE_TYPE signature_type;
	SIGNATURE_POSITION signature_position;
	SPECIFIED_NUMBER_TYPE specified_number_type;
	//std::vector<signature> signatures;
	//std::vector<single_template> templates;
	void pack(boost::property_tree::ptree& pt);
	void unpack(boost::property_tree::ptree& pt);
};

struct add_route_req
{
public:
	route_attribute route;
	bool pack(std::string& json);
	bool unpack(const std::string& json);
};

struct add_route_resp
{
public:
	PROTOCOL_RESP_STATUS status;
	bool pack(std::string& json);
	bool unpack(const std::string& json);
};

struct get_route_list_req
{
public:
	std::string provider_account_id;
	bool pack(std::string& json);
	bool unpack(const std::string& json);
};

struct get_route_list_resp
{
public:
	PROTOCOL_RESP_STATUS status;
	std::vector<route_attribute> routes;
	bool pack(std::string& json);
	bool unpack(const std::string& json);
};

struct provider_account
{
	std::string id;
	std::string username;
	std::string password;
	std::string company_name;
	std::string description;
	ACCOUNT_STATE state;
	STATISTICS_BELONGINGNESS statistics_belongingness;
	RECHARGE_MODE recharge_mode;
	//double balance;
	double balance_money;
	int balance_count;
	std::string balance_address;
	//double credit_line;
	double credit_line_money;
	unsigned int credit_line_count;
	//double threshold_value;
	double threshold_value_money;
	unsigned int threshold_value_count;
	double daily_consume;
	PACKAGE_STATE package_state;
	unsigned int package_count;
	boost::uint64_t setup_time;
	std::string manager;
	std::string linkman;
	std::string phone_number;
	std::string e_mail;
	void pack(boost::property_tree::ptree& pt);
	void unpack(boost::property_tree::ptree& pt);
};

struct add_provider_account_req
{
public:
	provider_account pro_account;
	bool pack(std::string& json);
	bool unpack(const std::string& json);
};

struct add_provider_account_resp
{
public:
	PROTOCOL_RESP_STATUS status;
	bool pack(std::string& json);
	bool unpack(const std::string& json);
};

struct get_provider_account_list_req
{
public:
	bool pack(std::string& json);
	bool unpack(const std::string& json);
};

struct get_provider_account_list_resp
{
public:
	PROTOCOL_RESP_STATUS status;
	std::vector<provider_account> pro_accounts;
	bool pack(std::string& json);
	bool unpack(const std::string& json);
};

struct feerate
{
	std::string index;
	std::string mcc;
	std::string mnc;
	double mt_price;
	double mo_price;
	void pack(boost::property_tree::ptree& pt);
	void unpack(boost::property_tree::ptree& pt);
};

struct add_feerate_list_req
{
public:
	FEERATE_BELONGINGNESS belongingness;
	//std::string index_value;
	std::vector<feerate> feerates;
	bool pack(std::string& json);
	bool unpack(const std::string& json);
};

struct add_feerate_list_resp
{
public:
	PROTOCOL_RESP_STATUS status;
	bool pack(std::string& json);
	bool unpack(const std::string& json);	
};

struct get_feerate_list_req
{
public:
	FEERATE_BELONGINGNESS belongingness;
	std::string index_value;
	bool pack(std::string& json);
	bool unpack(const std::string& json);
};

struct get_feerate_list_resp
{
public:
	PROTOCOL_RESP_STATUS status;
	std::vector<feerate> feerates;
	bool pack(std::string& json);
	bool unpack(const std::string& json);
};

struct choose_route_list_req
{
public:
	FUNCTION_TYPE function_type;
	CHARGE_MODE charge_mode;
	bool pack(std::string& json);
	bool unpack(const std::string& json);
};

struct choose_route_list_resp
{
public:
	PROTOCOL_RESP_STATUS status;
	std::vector<route_attribute> routes;
	bool pack(std::string& json);
	bool unpack(const std::string& json);
};

struct subaccount_charge
{
	unsigned int id;
	std::string subaccount_username;
	std::string company_account_username;
	unsigned int count;
	double money;
	boost::uint64_t setup_time; 
	void pack(boost::property_tree::ptree& pt);
	void unpack(boost::property_tree::ptree& pt);
};

struct company_account_balance
{
	std::string company_account_username;
	double balance_money;
	void pack(boost::property_tree::ptree& pt);
	void unpack(boost::property_tree::ptree& pt);
};

struct deduct_subaccount_charge_req
{
	std::vector<subaccount_charge> subaccount_charges;
	bool pack(std::string& json);
	bool unpack(const std::string& json);
};

struct deduct_subaccount_charge_resp
{
	PROTOCOL_RESP_STATUS status;
	std::vector<company_account_balance> company_account_balances;
	bool pack(std::string& json);
	bool unpack(const std::string& json);
};

struct route_charge
{
	std::string route_id;
	std::string provider_account_id;
	unsigned int count;
	double money;
	boost::uint64_t setup_time; 
	void pack(boost::property_tree::ptree& pt);
	void unpack(boost::property_tree::ptree& pt);
};

struct provider_account_balance
{
	STATISTICS_BELONGINGNESS statistics_belongingness;
	std::string id;
	RECHARGE_MODE recharge_mode;
	double balance_money;
	unsigned int balance_count;
	void pack(boost::property_tree::ptree& pt);
	void unpack(boost::property_tree::ptree& pt);
};

struct deduct_route_charge_req
{
	std::vector<route_charge> route_charges;
	bool pack(std::string& json);
	bool unpack(const std::string& json);
};

struct deduct_route_charge_resp
{
	PROTOCOL_RESP_STATUS status;
	std::vector<provider_account_balance> provider_account_balances;
	bool pack(std::string& json);
	bool unpack(const std::string& json);
};

struct sensitive_word
{
	std::string id;
	SENSITIVE_WORD_TYPE type;
	std::string content;
	void pack(boost::property_tree::ptree& pt);
	void unpack(boost::property_tree::ptree& pt);
};

struct add_sensitive_word_req
{
public:
	sensitive_word sensitive_word_;
	bool pack(std::string& json);
	bool unpack(const std::string& json);
};

struct add_sensitive_word_resp
{
public:
	PROTOCOL_RESP_STATUS status;
	bool pack(std::string& json);
	bool unpack(const std::string& json);
};

struct get_sensitive_word_list_req
{
public:
	bool pack(std::string& json);
	bool unpack(const std::string& json);
};

struct get_sensitive_word_list_resp
{
public:
	PROTOCOL_RESP_STATUS status;
	std::vector<sensitive_word> sensitive_words;
	bool pack(std::string& json);
	bool unpack(const std::string& json);
};

struct black_number
{
	std::string id;
	std::string number;
	void pack(boost::property_tree::ptree& pt);
	void unpack(boost::property_tree::ptree& pt);
};

struct add_black_number_req
{
public:
	black_number black_number_;
	bool pack(std::string& json);
	bool unpack(const std::string& json);
};

struct add_black_number_resp
{
public:
	PROTOCOL_RESP_STATUS status;
	bool pack(std::string& json);
	bool unpack(const std::string& json);
};

struct get_black_number_list_req
{
public:
	bool pack(std::string& json);
	bool unpack(const std::string& json);
};

struct get_black_number_list_resp
{
public:
	PROTOCOL_RESP_STATUS status;
	std::vector<black_number> black_numbers;
	bool pack(std::string& json);
	bool unpack(const std::string& json);
};

struct high_frequency_white_number
{
	std::string id;
	std::string number;
	void pack(boost::property_tree::ptree& pt);
	void unpack(boost::property_tree::ptree& pt);
};

struct add_high_frequency_white_number_req
{
public:
	high_frequency_white_number high_white_number;
	bool pack(std::string& json);
	bool unpack(const std::string& json);
};

struct add_high_frequency_white_number_resp
{
public:
	PROTOCOL_RESP_STATUS status;
	bool pack(std::string& json);
	bool unpack(const std::string& json);
};

struct get_high_frequency_white_number_list_req
{
public:
	bool pack(std::string& json);
	bool unpack(const std::string& json);
};

struct get_high_frequency_white_number_list_resp
{
public:
	PROTOCOL_RESP_STATUS status;
	std::vector<high_frequency_white_number> high_white_numbers;
	bool pack(std::string& json);
	bool unpack(const std::string& json);
};

struct all_list
{
	std::vector<provider_account> provier_accounts;
	std::vector<route_attribute> routes;
	std::vector<company_account> company_accounts;
	std::vector<subaccount_base_information> subaccounts;
	std::vector<subaccount_route> subaccount_routes;
	std::vector<single_template> templates;
	//std::vector<signature> route_signatures;
	//std::vector<signature> subaccount_signatures;
	std::vector<feerate> route_feerates;
	std::vector<feerate> subaccount_feerates;
	std::vector<sensitive_word> sensitive_words;
	std::vector<black_number> black_numbers;
	std::vector<high_frequency_white_number> high_frequency_white_numbers;
	void pack(boost::property_tree::ptree& pt);
	void unpack(boost::property_tree::ptree& pt);
};

struct get_all_list_req
{
	bool pack(std::string& json);
	bool unpack(const std::string& json);
};

struct get_all_list_resp
{
	PROTOCOL_RESP_STATUS status;
	all_list all_list_;
	bool pack(std::string& json);
	bool unpack(const std::string& json);
};
#endif