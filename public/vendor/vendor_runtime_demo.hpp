//
//  vendor_runtime_demo.hpp
//  SMPPServer
//
//  Created by Alex on 2018/8/24.
//  Copyright © 2018 Bifang technology. All rights reserved.
//
// 这是一个模拟的运行时，处理待发送短信，模拟报告，模拟上行
#ifndef vendor_demo_runtime_hpp
#define vendor_demo_runtime_hpp

#include <stdio.h>
#include "protocol_structure.h"
#include "data_structure.h"
#include <vector>
#include <boost/thread/thread.hpp>
#include "vendor_runtime.h"
#include "smpp.hpp"

class vendor_runtime_demo: public vendor_runtime
{
public:
public:
	vendor_runtime_demo();
	virtual ~vendor_runtime_demo(){}

protected:
	void generate_mos(const std::string& sysid, int to_generate);

	virtual bool sender();
	virtual bool query_dlrs_and_mos();
	virtual bool query_accounts();
protected:
	void initialize_test_data();
};
#endif /* vendor_demo_runtime_hpp */
