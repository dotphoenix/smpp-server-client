#below is the public block of all the makefiles in CBPlatform projects
TARGET_OS = $(shell uname)

CFLAGS = -Wall -O3 -DCB_SMPPSERVER #Ubuntu下可能要加上  '-std=c++11'

TARGET_NAME = BFSMPPServer


# TOP DIR
THIS_PROJ_TOPDIR = $(shell /bin/pwd)
CBPLATFORM_DIR = $(THIS_PROJ_TOPDIR)/../..
PUBLIC_DIR = $(CBPLATFORM_DIR)/public


#output dir
OBJOUTPUT_DIR= $(CBPLATFORM_DIR)/output/$(TARGET_NAME)/objs
BINOUTPUT_DIR = $(CBPLATFORM_DIR)/output/bin


#public dirs
ALGORITHM_TOOLKIT_DIR = $(PUBLIC_DIR)/algorithm_toolkit
CONFIGER_DIR = $(PUBLIC_DIR)/configer
LOGGER_DIR = $(PUBLIC_DIR)/logger
MEMORY_BUFFER_DIR = $(PUBLIC_DIR)/memory_buffer
NETWORK_DIR = $(PUBLIC_DIR)/network
CLIENT_DIR = $(PUBLIC_DIR)/client
PROCESS_DIR = $(PUBLIC_DIR)/process
SMPP_PARSER_PATH = $(PUBLIC_DIR)/smppparser
DATABASE_SRC_DIR = $(PUBLIC_DIR)/database
MONGOOSE_DIR = $(PUBLIC_DIR)/mongoose
SMPPSERVER_DIR = $(PUBLIC_DIR)/smppserver
SMPP_DATACODING_CONVERT_DIR = $(PUBLIC_DIR)/smpp_datacoding_convert
PROCESS_DIR = $(PUBLIC_DIR)/process
CRASH_MONITOR_DIR = $(PUBLIC_DIR)/crash_monitor
AVHTTP_CLIENT_TOOLS_DIR = $(PUBLIC_DIR)/avhttp
PROTOCOL_PUBLIC_DIR = $(PUBLIC_DIR)/protocol_public
PROTOCOL_DIR = $(PUBLIC_DIR)/protocol
VENDOR_DIR = $(PUBLIC_DIR)/vendor

#private dirs
PROJECT_SRC_DIR = $(THIS_PROJ_TOPDIR)
PROJECT_DB_SRC_DIR = $(THIS_PROJ_TOPDIR)/db

#public headers
HEADER_PUBLIC_PATHS = -I$(ALGORITHM_TOOLKIT_DIR) -I$(CONFIGER_DIR)   \
		-I$(LOGGER_DIR) -I$(MEMORY_BUFFER_DIR) -I$(NETWORK_DIR) \
		-I$(CLIENT_DIR) -I$(PROCESS_DIR) -I$(MONGOOSE_DIR) \
		-I$(SMPP_PARSER_PATH) -I$(DATABASE_SRC_DIR) -I$(PROTOCOL_PUBLIC_DIR) \
		-I$(AVHTTP_CLIENT_TOOLS_DIR) \
		-I$(PROJECT_SRC_DIR) -I$(PROJECT_DB_SRC_DIR) -I$(SMPP_DATACODING_CONVERT_DIR) \
		-I$(PROCESS_DIR) -I$(CRASH_MONITOR_DIR) \
		-I$(SMPPSERVER_DIR) -I$(PROTOCOL_DIR) -I$(VENDOR_DIR)
		

#boost headers		
HEADER_BOOST_PATH = -I $(BOOST_DIR_PATH)/include

#
HEADER_SEARCH_PATHS = $(HEADER_PUBLIC_PATHS) $(HEADER_BOOST_PATH) -I/usr/include/mysql

ifeq ($(TARGET_OS), Darwin)
CXX = clang++  -stdlib=libc++
XX = cc
BOOST_DIR_PATH = /Users/alex/Public/boost
LIB_BOOST_PATH = $(BOOST_DIR_PATH)/lib
LIB_SMPPPARSER_PATH = $(SMPP_PARSER_PATH)

BOOST_OBJS := $(LIB_BOOST_PATH)/libboost_thread.a	$(LIB_BOOST_PATH)/libboost_system.a \
$(LIB_BOOST_PATH)/libboost_regex.a \
		$(LIB_BOOST_PATH)/libboost_date_time.a	$(LIB_BOOST_PATH)/libboost_filesystem.a  
OTHER_LIBS := $(LIB_SMPPPARSER_PATH)/libsmppparser.a

else
	CXX = g++
	XX = cc
	BOOST_DIR_PATH = /home/alex/boost
	LIB_BOOST_PATH=$(BOOST_DIR_PATH)/lib
	LIB_SMPPPARSER_PATH = $(SMPP_PARSER_PATH)

	BOOST_OBJS := -Wl,--whole-archive \
			$(LIB_BOOST_PATH)/libboost_thread.a $(LIB_BOOST_PATH)/libboost_system.a \
			$(LIB_BOOST_PATH)/libboost_regex.a \
			$(LIB_BOOST_PATH)/libboost_date_time.a $(LIB_BOOST_PATH)/libboost_filesystem.a \
			-Wl,--no-whole-archive  \
			
	ifeq ($(TARGET_OS), Ubuntu)
		OTHER_LIBS := -ldl -lpthread $(LIB_SMPPPARSER_PATH)/libsmppparser.a #ubuntu可能要加上 -lrt -ldl -lpthread -licuuc -licui18n -licudata
	else
		OTHER_LIBS := -lrt -lpthread $(LIB_SMPPPARSER_PATH)/libsmppparser.a  #ubuntu可能要加上 -lrt -ldl -lpthread -licuuc -licui18n -licudata
	endif
endif

#public objs

TOOL_OBJS = $(OBJOUTPUT_DIR)/md5.o  \
			 $(OBJOUTPUT_DIR)/configer.o \
			 $(OBJOUTPUT_DIR)/logger.o \
			 $(OBJOUTPUT_DIR)/base64.o \

PROTOCOL_PUBLIC_OBJS = $(OBJOUTPUT_DIR)/cb_public_functions.o \
							$(OBJOUTPUT_DIR)/cb_protocol_public.o \

NETWORK_OBJS = $(OBJOUTPUT_DIR)/connections_manager.o  \
			 $(OBJOUTPUT_DIR)/server.o \
			 $(OBJOUTPUT_DIR)/tcp_connection_impl.o \
			 $(OBJOUTPUT_DIR)/tcp_connection.o \

#$(OBJOUTPUT_DIR)/mysql_client_helper_tester.o
DATABASE_OBJS = $(OBJOUTPUT_DIR)/smis_db.o \
				$(OBJOUTPUT_DIR)/records_db.o \
				$(OBJOUTPUT_DIR)/db_strings.o \

MONGOOSE_OBJS = $(OBJOUTPUT_DIR)/mongoose.o \

SMPPSERVER_OBJS = $(OBJOUTPUT_DIR)/smpp_protocol_impl.o \
	   			$(OBJOUTPUT_DIR)/smppserver_runtime.o \
				$(OBJOUTPUT_DIR)/smppserver_datasource.o \

VENDOR_OBJS = $(OBJOUTPUT_DIR)/vendor_runtime_1.o \
	   			$(OBJOUTPUT_DIR)/vendor_runtime_demo.o \

SMPP_DATACODING_CONVERT_OBJ = $(OBJOUTPUT_DIR)/smpp_datacoding_toolkit.o \

PROTOCOL_OBJS = $(OBJOUTPUT_DIR)/protocol_structure.o \

AVHTTP_CLIENT_TOOLS_OBJS = $(OBJOUTPUT_DIR)/avhttp_client_tools.o \

TARGET = $(BINOUTPUT_DIR)/$(TARGET_NAME)

INCLUDES := $(HEADER_SEARCH_PATHS) \

#################################################################################
##below is the private block of each projects 
OBJS = $(OBJOUTPUT_DIR)/main.o \

#如果使用mysql，则需要在$(OTHER_LIBS) 和 -o之间加上 `mysql_config --cflags --libs`
$(TARGET) : $(OBJS) $(TOOL_OBJS) $(NETWORK_OBJS) $(DATABASE_OBJS) $(AVHTTP_CLIENT_TOOLS_OBJS) $(MONGOOSE_OBJS) $(SMPPSERVER_OBJS) $(VENDOR_OBJS) $(SMPP_DATACODING_CONVERT_OBJ) $(PROTOCOL_PUBLIC_OBJS) $(PROTOCOL_OBJS) $(OTHER_LIBS)
	$(CXX) $(CFLAGS) $(BOOST_OBJS) $(OBJS) $(TOOL_OBJS) $(DATABASE_OBJS) $(NETWORK_OBJS) $(AVHTTP_CLIENT_TOOLS_OBJS) $(MONGOOSE_OBJS) $(SMPPSERVER_OBJS) $(VENDOR_OBJS) $(SMPP_DATACODING_CONVERT_OBJ) $(PROTOCOL_PUBLIC_OBJS) $(PROTOCOL_OBJS) $(OTHER_LIBS) -o $(TARGET)

#tool objs 
$(OBJOUTPUT_DIR)/logger.o : $(LOGGER_DIR)/logger.cpp
	$(CXX) $(CFLAGS) $(INCLUDES) -c $(LOGGER_DIR)/logger.cpp -o $(OBJOUTPUT_DIR)/logger.o
$(OBJOUTPUT_DIR)/md5.o : $(ALGORITHM_TOOLKIT_DIR)/md5.cpp
	$(CXX) $(CFLAGS) $(INCLUDES) -c $(ALGORITHM_TOOLKIT_DIR)/md5.cpp -o $(OBJOUTPUT_DIR)/md5.o
$(OBJOUTPUT_DIR)/configer.o : $(CONFIGER_DIR)/configer.cpp
	$(CXX) $(CFLAGS) $(INCLUDES) -c $(CONFIGER_DIR)/configer.cpp -o $(OBJOUTPUT_DIR)/configer.o
$(OBJOUTPUT_DIR)/base64.o : $(ALGORITHM_TOOLKIT_DIR)/base64.cpp
	$(CXX) $(CFLAGS) $(INCLUDES) -c $(ALGORITHM_TOOLKIT_DIR)/base64.cpp -o $(OBJOUTPUT_DIR)/base64.o

#protocol objs
$(OBJOUTPUT_DIR)/protocol_structure.o : $(PROTOCOL_DIR)/protocol_structure.cpp
	$(CXX) $(CFLAGS) $(INCLUDES) -c $(PROTOCOL_DIR)/protocol_structure.cpp -o $(OBJOUTPUT_DIR)/protocol_structure.o

#protocol_public objs
$(OBJOUTPUT_DIR)/cb_public_functions.o : $(PROTOCOL_PUBLIC_DIR)/cb_public_functions.cpp
	$(CXX) $(CFLAGS) $(INCLUDES) -c $(PROTOCOL_PUBLIC_DIR)/cb_public_functions.cpp -o $(OBJOUTPUT_DIR)/cb_public_functions.o	
$(OBJOUTPUT_DIR)/cb_protocol_public.o : $(PROTOCOL_PUBLIC_DIR)/cb_protocol_public.cpp
	$(CXX) $(CFLAGS) $(INCLUDES) -c $(PROTOCOL_PUBLIC_DIR)/cb_protocol_public.cpp -o $(OBJOUTPUT_DIR)/cb_protocol_public.o

#network objs
$(OBJOUTPUT_DIR)/connections_manager.o : $(NETWORK_DIR)/connections_manager.cpp
	$(CXX) $(CFLAGS) $(INCLUDES) -c $(NETWORK_DIR)/connections_manager.cpp -o $(OBJOUTPUT_DIR)/connections_manager.o
$(OBJOUTPUT_DIR)/tcp_connection.o : $(NETWORK_DIR)/tcp_connection.cpp
	$(CXX) $(CFLAGS) $(INCLUDES) -c $(NETWORK_DIR)/tcp_connection.cpp -o $(OBJOUTPUT_DIR)/tcp_connection.o
$(OBJOUTPUT_DIR)/tcp_connection_impl.o : $(NETWORK_DIR)/tcp_connection_impl.cpp
	$(CXX) $(CFLAGS) $(INCLUDES) -c $(NETWORK_DIR)/tcp_connection_impl.cpp -o $(OBJOUTPUT_DIR)/tcp_connection_impl.o
$(OBJOUTPUT_DIR)/server.o : $(NETWORK_DIR)/server.cpp
	$(CXX) $(CFLAGS) $(INCLUDES) -c $(NETWORK_DIR)/server.cpp -o $(OBJOUTPUT_DIR)/server.o	

#database
#$(OBJOUTPUT_DIR)/mysql_client_helper_tester.o : $(DATABASE_SRC_DIR)/mysql_client_helper_tester.cpp
#	$(XX) $(CFLAGS) $(INCLUDES) -c $(DATABASE_SRC_DIR)/mysql_client_helper_tester.cpp -o $(OBJOUTPUT_DIR)/mysql_client_helper_tester.o
$(OBJOUTPUT_DIR)/smis_db.o : $(DATABASE_SRC_DIR)/smis_db.cpp
	$(CXX) $(CFLAGS) $(INCLUDES) -c $(DATABASE_SRC_DIR)/smis_db.cpp -o $(OBJOUTPUT_DIR)/smis_db.o
$(OBJOUTPUT_DIR)/records_db.o : $(DATABASE_SRC_DIR)/records_db.cpp
	$(CXX) $(CFLAGS) $(INCLUDES) -c $(DATABASE_SRC_DIR)/records_db.cpp -o $(OBJOUTPUT_DIR)/records_db.o
$(OBJOUTPUT_DIR)/db_strings.o : $(DATABASE_SRC_DIR)/db_strings.cpp
	$(CXX) $(CFLAGS) $(INCLUDES) -c $(DATABASE_SRC_DIR)/db_strings.cpp -o $(OBJOUTPUT_DIR)/db_strings.o
#mongoose
$(OBJOUTPUT_DIR)/mongoose.o : $(MONGOOSE_DIR)/mongoose.cpp
	$(CXX) $(CFLAGS) $(INCLUDES) -c $(MONGOOSE_DIR)/mongoose.cpp -o $(OBJOUTPUT_DIR)/mongoose.o

#avhttp_client_tools
$(OBJOUTPUT_DIR)/avhttp_client_tools.o : $(AVHTTP_CLIENT_TOOLS_DIR)/avhttp_client_tools.cpp
	$(CXX) $(CFLAGS) $(INCLUDES) -c $(AVHTTP_CLIENT_TOOLS_DIR)/avhttp_client_tools.cpp -o $(OBJOUTPUT_DIR)/avhttp_client_tools.o

#smpp_datacoding_convert
$(OBJOUTPUT_DIR)/smpp_datacoding_toolkit.o : $(SMPP_DATACODING_CONVERT_DIR)/smpp_datacoding_toolkit.cpp
	$(CXX) $(CFLAGS) $(INCLUDES) -c $(SMPP_DATACODING_CONVERT_DIR)/smpp_datacoding_toolkit.cpp -o $(OBJOUTPUT_DIR)/smpp_datacoding_toolkit.o

#smppserver
$(OBJOUTPUT_DIR)/smpp_protocol_impl.o : $(SMPPSERVER_DIR)/smpp_protocol_impl.cpp
	$(CXX) $(CFLAGS) $(INCLUDES) -c $(SMPPSERVER_DIR)/smpp_protocol_impl.cpp -o $(OBJOUTPUT_DIR)/smpp_protocol_impl.o
$(OBJOUTPUT_DIR)/smppserver_runtime.o : $(SMPPSERVER_DIR)/smppserver_runtime.cpp
	$(CXX) $(CFLAGS) $(INCLUDES) -c $(SMPPSERVER_DIR)/smppserver_runtime.cpp -o $(OBJOUTPUT_DIR)/smppserver_runtime.o
$(OBJOUTPUT_DIR)/smppserver_datasource.o : $(SMPPSERVER_DIR)/smppserver_datasource.cpp
	$(CXX) $(CFLAGS) $(INCLUDES) -c $(SMPPSERVER_DIR)/smppserver_datasource.cpp -o $(OBJOUTPUT_DIR)/smppserver_datasource.o

#vendor
$(OBJOUTPUT_DIR)/vendor_runtime_1.o : $(VENDOR_DIR)/vendor_runtime_1.cpp
	$(CXX) $(CFLAGS) $(INCLUDES) -c $(VENDOR_DIR)/vendor_runtime_1.cpp -o $(OBJOUTPUT_DIR)/vendor_runtime_1.o
$(OBJOUTPUT_DIR)/vendor_runtime_demo.o : $(VENDOR_DIR)/vendor_runtime_demo.cpp
	$(CXX) $(CFLAGS) $(INCLUDES) -c $(VENDOR_DIR)/vendor_runtime_demo.cpp -o $(OBJOUTPUT_DIR)/vendor_runtime_demo.o


#main	
$(OBJOUTPUT_DIR)/main.o : $(PROJECT_SRC_DIR)/main.cpp
	$(CXX) $(CFLAGS) $(INCLUDES) -c $(PROJECT_SRC_DIR)/main.cpp -o $(OBJOUTPUT_DIR)/main.o

create_dirs:
	mkdir -p $(OBJOUTPUT_DIR)
	mkdir -p $(BINOUTPUT_DIR)
		
clean:
	rm -f $(TARGET) $(OBJS) $(TOOL_OBJS) $(NETWORK_OBJS) $(DATABASE_OBJS) $(MONGOOSE_OBJS) $(SMPPSERVER_OBJS)  $(VENDOR_OBJS) $(SMPP_DATACODING_CONVERT_OBJ) $(PROTOCOL_PUBLIC_OBJS)
run:
	$(TARGET) 
