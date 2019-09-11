//
//  cb_client_api.h
//  CBClient
//
//  Created by Alex on 12/20/14.
//  Copyright (c) 2014 BiFang technology. All rights reserved.
//

#ifndef cb_client_API_H
#define cb_client_API_H
#include <string>
#include <vector>
#include "json_parser.h"
/*********************************************************
 Function:    cb_client_start
 Description: 启动smpp client系统库
 Input: configer_file 配置文件的路径，配置文件的格式，请参考附录A
 *********************************************************/
void cb_client_start(const std::string& configer_file, bool record_to_db = true);
bool cb_client_is_bound();

/*********************************************************
 Function:    cb_client_stop
 Description: 停止smpp client系统库。请确认当前无正在发送的SM，否则
			  正在发送的短信可能丢失

 *********************************************************/
void cb_client_stop();

/*********************************************************
 Function:    cb_client_submit
 Description: 请求smpp client系统库发送一条或多条信息
 
 Input: json 封装好的一组待发送目的号码和内容的json格式
 Output:
 Return: 返回一个session id，后续的状态报告里面会包含该session id，请保证一次提交
 的目的号码和内容是唯一的，这样利于匹配;如果返回session_id的长度为0，说明异常，
 需要重新检查数据后再次发送
 *********************************************************/
std::string cb_client_submit_sms(const std::string& json);

/*********************************************************
 Function:    cb_client_format_sms
 Description: 将要发送的目的号码及内容格式化成json格式
 
 Input: numbers 目的号码，可以是多个，以英文的分号(;)分开，如85266668888;85266668889;85266668880
 Input: content 短信内容
 Input: src_addr 源号码
 Input: count_of_batch 每一批短信的数量，如果是长短信，每个号码相当于多条短信
 Output: jsons 封装好的待发送目的号码和内容的json格式的数组
 Return: 封装后的总短信条数；总条数和目的号码的总数量不一定相等，因为如果content的长度超过了一条短信的最大字数(中文70，英文160），
 该函数会自动拆分。
 
 *********************************************************/
int cb_client_format_sms(const std::string& numbers, const std::string& content, const std::string& src_addr, int count_of_batch, std::vector<std::string>& jsons);

/*********************************************************
 Function:	  cb_client_get_dlrs
 Description: 获取已发送短信的状态报告
 
 Input:
 record: 是否将获取到的dlrs存储到本地数据库中以供后续cb_client_query_dlrs查询
 to_get: 想要获取多少个dlrs(实际返回的dlrs数量会小于或者等于改值)

 Output: 
 dlrs: 获取到的dlrs
 json: 获取到的dlrs的json格式,如果没有dlrs,则返回""
 Return:
 *********************************************************/
void cb_client_get_dlrs(int to_get, std::string& json);
void cb_client_get_dlrs(int to_get, std::vector<sm_status>& dlrs);

/*********************************************************
 Function:	  cb_client_query_dlrs
 Description: 从本地数据库中查询dlrs(数据库中的dlrs是由cb_client_get_dlrs调用时写入的)
 
 Input: 
 begin_time: 起始时间，格式为调用time函数获得的值
 end_time: 结束时间，格式为调用time函数获得的值
 Output:
 dlrs: 查询到的结果
 Return:
 *********************************************************/
void cb_client_query_dlrs(boost::uint64_t begin_time, boost::uint64_t end_time, std::vector<sm_status>& dlrs);
/*********************************************************
Function:	  cb_client_get_mos
Description: 获取收到的mos

Input:
record: 是否将获取到的mos存储到本地数据库中以供后续cb_client_query_mos查询

Output: 
mos: 获取到的mos
json: 获取到的mos的json格式,如果没有mos,则返回""
Return:
 *********************************************************/
void cb_client_get_mos(std::string& json);
void cb_client_get_mos(std::vector<sm_mo>& mos);
/*********************************************************
 Function:	  cb_client_query_mos
 Description: 从本地数据库中查询dlrs(数据库中的mos是由cb_client_get_mos调用时写入的)
 
 Input: 
 begin_time: 起始时间，格式为调用time函数获得的值
 end_time: 结束时间，格式为调用time函数获得的值
 Output:
 mos: 查询到的结果
 Return:
 *********************************************************/
void cb_client_query_mos(boost::uint64_t begin_time, boost::uint64_t end_time, std::vector<sm_mo>& mos);

/*********************************************************
 Function:	  cb_client_get_statistic_string
 Description: 获取系统内部的统计信息字符串
 
 Return:
 *********************************************************/
void cb_client_get_statistic_string(std::string& s);

/*********************************************************
 附录A：配置文件的格式
 
 #配置文件的格式为：
 [log]
 console=1 #1表示输出到控制台，其它值表示输出到文件
 level=2 #log的级别，1表示error，2表示warning，3表示debug
 
 [smpp_client_nb]
 nb=1 #client的数量
 
 [smpp_client11]
 serverip=117.18.48.9
 serverport=5016
 sysid=MNHIO1
 password=WUwutH7y
 srcaddr=cb
 srcaddr_ton=1
 srcaddr_npi=1
 bind_mode=TRX
 max_sms_per_second=100
 using_hex_decimal_message_id=1
 force_dlr_datacoding=-1
 force_mo_datacoding=-1
 enquire_link_period=15
 waiting_ack_timeout=5
 waiting_dlr_timeout=259200
 systype=systype
 servtype=SERV
 is_gateway=0

 *********************************************************/
#endif
