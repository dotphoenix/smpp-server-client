#below is the public block of all the makefiles in CBPlatform projects
TARGET_OS = $(shell uname)

CFLAGS = -Wall -O3 -DSMPP_CLIENT

TARGET_NAME = BFSMPPClient


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
PROCESS_DIR = $(PUBLIC_DIR)/process
SMPP_PARSER_PATH = $(PUBLIC_DIR)/smppparser
SQLITE_SRC_DIR = $(PUBLIC_DIR)/sqlite
MONGOOSE_DIR = $(PUBLIC_DIR)/mongoose
SMPP_DATACODING_CONVERT_DIR = $(PUBLIC_DIR)/smpp_datacoding_convert
CLIENT_COMMON_DIR = $(PUBLIC_DIR)/client_common
SMPP_CLIENT_DIR = $(PUBLIC_DIR)/smppclient
CRASH_MONITOR_DIR = $(PUBLIC_DIR)/crash_monitor
PROTOCOL_PUBLIC_DIR = $(PUBLIC_DIR)/protocol_public

#private dirs
PROJECT_SRC_DIR = $(THIS_PROJ_TOPDIR)

#public headers
HEADER_PUBLIC_PATHS = -I$(ALGORITHM_TOOLKIT_DIR) -I$(CONFIGER_DIR)   \
		-I$(LOGGER_DIR) -I$(MEMORY_BUFFER_DIR) -I$(PROCESS_DIR) -I$(MONGOOSE_DIR) \
		-I$(SMPP_PARSER_PATH) -I$(SQLITE_SRC_DIR) -I$(PROTOCOL_PUBLIC_DIR) \
		-I$(PROJECT_SRC_DIR) -I$(SMPP_DATACODING_CONVERT_DIR) \
		-I$(PROCESS_DIR) -I$(CRASH_MONITOR_DIR) -I$(SMPP_CLIENT_DIR) \
		-I$(CLIENT_COMMON_DIR) \

#boost headers		
HEADER_BOOST_PATH = -I $(BOOST_DIR_PATH)/include

#
HEADER_SEARCH_PATHS = $(HEADER_PUBLIC_PATHS) $(HEADER_BOOST_PATH) 

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
		OTHER_LIBS := -ldl -lpthread $(LIB_SMPPPARSER_PATH)/libsmppparser.a
	else
		OTHER_LIBS := -lrt -lpthread $(LIB_SMPPPARSER_PATH)/libsmppparser.a
	endif
endif

#public objs
TOOL_OBJS = $(OBJOUTPUT_DIR)/md5.o  \
			 $(OBJOUTPUT_DIR)/configer.o \
			 $(OBJOUTPUT_DIR)/logger.o \
			 $(OBJOUTPUT_DIR)/base64.o \

PROTOCOL_PUBLIC_OBJS = $(OBJOUTPUT_DIR)/cb_public_functions.o \
							$(OBJOUTPUT_DIR)/cb_protocol_public.o \
							$(OBJOUTPUT_DIR)/sync_tcp_client.o \
							$(OBJOUTPUT_DIR)/data_encoding_tools.o \

SQLITE_OBJS = $(OBJOUTPUT_DIR)/sqlite3.o \
				$(OBJOUTPUT_DIR)/sqlite3pp.o \
				$(OBJOUTPUT_DIR)/sqlite3ppext.o \

MONGOOSE_OBJS = $(OBJOUTPUT_DIR)/mongoose.o \


SMPP_CLIENT_OBJS = $(OBJOUTPUT_DIR)/smppclient.o \

CLIENT_COMMON_OBJS = $(OBJOUTPUT_DIR)/clients_manager.o \
	   				$(OBJOUTPUT_DIR)/short_message_db.o \
	   				$(OBJOUTPUT_DIR)/json_parser.o \
					$(OBJOUTPUT_DIR)/client_data_source_manager.o \
					$(OBJOUTPUT_DIR)/cb_client_api.o \

SMPP_DATACODING_CONVERT_OBJ = $(OBJOUTPUT_DIR)/smpp_datacoding_toolkit.o \

TARGET = $(BINOUTPUT_DIR)/$(TARGET_NAME)

INCLUDES := $(HEADER_SEARCH_PATHS) \

#################################################################################
##below is the private block of each projects 
OBJS = $(OBJOUTPUT_DIR)/data_generator.o \
		$(OBJOUTPUT_DIR)/simple_webserver.o \
	   $(OBJOUTPUT_DIR)/main.o \
		
$(TARGET) : $(OBJS) $(TOOL_OBJS) $(CLIENT_COMMON_OBJS) $(SQLITE_OBJS) $(MONGOOSE_OBJS) $(SMPP_CLIENT_OBJS) $(SMPP_DATACODING_CONVERT_OBJ) $(PROTOCOL_PUBLIC_OBJS) $(OTHER_LIBS)
	$(CXX) $(CFLAGS) $(BOOST_OBJS) $(OBJS) $(TOOL_OBJS) $(SQLITE_OBJS) $(CLIENT_COMMON_OBJS) $(MONGOOSE_OBJS) $(SMPP_CLIENT_OBJS) $(SMPP_DATACODING_CONVERT_OBJ) $(PROTOCOL_PUBLIC_OBJS) $(OTHER_LIBS) -o $(TARGET)

#tool objs 
$(OBJOUTPUT_DIR)/logger.o : $(LOGGER_DIR)/logger.cpp
	$(CXX) $(CFLAGS) $(INCLUDES) -c $(LOGGER_DIR)/logger.cpp -o $(OBJOUTPUT_DIR)/logger.o
$(OBJOUTPUT_DIR)/md5.o : $(ALGORITHM_TOOLKIT_DIR)/md5.cpp
	$(CXX) $(CFLAGS) $(INCLUDES) -c $(ALGORITHM_TOOLKIT_DIR)/md5.cpp -o $(OBJOUTPUT_DIR)/md5.o
$(OBJOUTPUT_DIR)/configer.o : $(CONFIGER_DIR)/configer.cpp
	$(CXX) $(CFLAGS) $(INCLUDES) -c $(CONFIGER_DIR)/configer.cpp -o $(OBJOUTPUT_DIR)/configer.o
$(OBJOUTPUT_DIR)/base64.o : $(ALGORITHM_TOOLKIT_DIR)/base64.cpp
	$(CXX) $(CFLAGS) $(INCLUDES) -c $(ALGORITHM_TOOLKIT_DIR)/base64.cpp -o $(OBJOUTPUT_DIR)/base64.o

#protocol_public objs
$(OBJOUTPUT_DIR)/cb_public_functions.o : $(PROTOCOL_PUBLIC_DIR)/cb_public_functions.cpp
	$(CXX) $(CFLAGS) $(INCLUDES) -c $(PROTOCOL_PUBLIC_DIR)/cb_public_functions.cpp -o $(OBJOUTPUT_DIR)/cb_public_functions.o	
$(OBJOUTPUT_DIR)/cb_protocol_public.o : $(PROTOCOL_PUBLIC_DIR)/cb_protocol_public.cpp
	$(CXX) $(CFLAGS) $(INCLUDES) -c $(PROTOCOL_PUBLIC_DIR)/cb_protocol_public.cpp -o $(OBJOUTPUT_DIR)/cb_protocol_public.o
$(OBJOUTPUT_DIR)/sync_tcp_client.o : $(PROTOCOL_PUBLIC_DIR)/sync_tcp_client.cpp
	$(CXX) $(CFLAGS) $(INCLUDES) -c $(PROTOCOL_PUBLIC_DIR)/sync_tcp_client.cpp -o $(OBJOUTPUT_DIR)/sync_tcp_client.o
$(OBJOUTPUT_DIR)/data_encoding_tools.o : $(PROTOCOL_PUBLIC_DIR)/data_encoding_tools.cpp
	$(CXX) $(CFLAGS) $(INCLUDES) -c $(PROTOCOL_PUBLIC_DIR)/data_encoding_tools.cpp -o $(OBJOUTPUT_DIR)/data_encoding_tools.o

#sqlite
$(OBJOUTPUT_DIR)/sqlite3.o : $(SQLITE_SRC_DIR)/sqlite3.c
	$(XX) $(CFLAGS) $(INCLUDES) -c $(SQLITE_SRC_DIR)/sqlite3.c -o $(OBJOUTPUT_DIR)/sqlite3.o
$(OBJOUTPUT_DIR)/sqlite3pp.o : $(SQLITE_SRC_DIR)/sqlite3pp.cpp
	$(CXX) $(CFLAGS) $(INCLUDES) -c $(SQLITE_SRC_DIR)/sqlite3pp.cpp -o $(OBJOUTPUT_DIR)/sqlite3pp.o
$(OBJOUTPUT_DIR)/sqlite3ppext.o : $(SQLITE_SRC_DIR)/sqlite3ppext.cpp
	$(CXX) $(CFLAGS) $(INCLUDES) -c $(SQLITE_SRC_DIR)/sqlite3ppext.cpp -o $(OBJOUTPUT_DIR)/sqlite3ppext.o

#mongoose
$(OBJOUTPUT_DIR)/mongoose.o : $(MONGOOSE_DIR)/mongoose.cpp
	$(CXX) $(CFLAGS) $(INCLUDES) -c $(MONGOOSE_DIR)/mongoose.cpp -o $(OBJOUTPUT_DIR)/mongoose.o

#smppclient
$(OBJOUTPUT_DIR)/smppclient.o : $(SMPP_CLIENT_DIR)/smppclient.cpp
	$(XX) $(CFLAGS) $(INCLUDES) -c $(SMPP_CLIENT_DIR)/smppclient.cpp -o $(OBJOUTPUT_DIR)/smppclient.o

#client_common
$(OBJOUTPUT_DIR)/clients_manager.o : $(CLIENT_COMMON_DIR)/clients_manager.cpp
	$(CXX) $(CFLAGS) $(INCLUDES) -c $(CLIENT_COMMON_DIR)/clients_manager.cpp -o $(OBJOUTPUT_DIR)/clients_manager.o
$(OBJOUTPUT_DIR)/json_parser.o : $(CLIENT_COMMON_DIR)/json_parser.cpp
	$(CXX) $(CFLAGS) $(INCLUDES) -c $(CLIENT_COMMON_DIR)/json_parser.cpp -o $(OBJOUTPUT_DIR)/json_parser.o
$(OBJOUTPUT_DIR)/client_data_source_manager.o : $(CLIENT_COMMON_DIR)/client_data_source_manager.cpp
	$(CXX) $(CFLAGS) $(INCLUDES) -c $(CLIENT_COMMON_DIR)/client_data_source_manager.cpp -o $(OBJOUTPUT_DIR)/client_data_source_manager.o
$(OBJOUTPUT_DIR)/short_message_db.o : $(CLIENT_COMMON_DIR)/short_message_db.cpp
	$(CXX) $(CFLAGS) $(INCLUDES) -c $(CLIENT_COMMON_DIR)/short_message_db.cpp -o $(OBJOUTPUT_DIR)/short_message_db.o
$(OBJOUTPUT_DIR)/cb_client_api.o : $(CLIENT_COMMON_DIR)/cb_client_api.cpp
	$(CXX) $(CFLAGS) $(INCLUDES) -c $(CLIENT_COMMON_DIR)/cb_client_api.cpp -o $(OBJOUTPUT_DIR)/cb_client_api.o

#smpp_datacoding_convert
$(OBJOUTPUT_DIR)/smpp_datacoding_toolkit.o : $(SMPP_DATACODING_CONVERT_DIR)/smpp_datacoding_toolkit.cpp
	$(CXX) $(CFLAGS) $(INCLUDES) -c $(SMPP_DATACODING_CONVERT_DIR)/smpp_datacoding_toolkit.cpp -o $(OBJOUTPUT_DIR)/smpp_datacoding_toolkit.o

#main
$(OBJOUTPUT_DIR)/data_generator.o : $(PROJECT_SRC_DIR)/data_generator.cpp
	$(CXX) $(CFLAGS) $(INCLUDES) -c $(PROJECT_SRC_DIR)/data_generator.cpp -o $(OBJOUTPUT_DIR)/data_generator.o
$(OBJOUTPUT_DIR)/simple_webserver.o : $(PROJECT_SRC_DIR)/simple_webserver.cpp
	$(CXX) $(CFLAGS) $(INCLUDES) -c $(PROJECT_SRC_DIR)/simple_webserver.cpp -o $(OBJOUTPUT_DIR)/simple_webserver.o
$(OBJOUTPUT_DIR)/main.o : $(PROJECT_SRC_DIR)/main.cpp
	$(CXX) $(CFLAGS) $(INCLUDES) -c $(PROJECT_SRC_DIR)/main.cpp -o $(OBJOUTPUT_DIR)/main.o

create_dirs:
	mkdir -p $(OBJOUTPUT_DIR)
	mkdir -p $(BINOUTPUT_DIR)
		
clean:
	rm -f $(TARGET) $(OBJS) $(TOOL_OBJS) $(CLIENT_COMMON_OBJS) $(SQLITE_OBJS) $(MONGOOSE_OBJS) $(SMPP_CLIENT_OBJS) $(SMPP_DATACODING_CONVERT_OBJ)
run:
	echo ******
	echo $(PUBLIC_DIR)
	$(TARGET) 
