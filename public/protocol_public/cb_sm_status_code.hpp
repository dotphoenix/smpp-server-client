#ifndef CB_SM_STATUS_CODE_H
#define CB_SM_STATUS_CODE_H

#define CB_SM_STATUS_CODE_NOT_SEND 0x1000
#define CB_SM_STATUS_CODE_IS_SENDING 0x1001

/*
 if < 10000, it means temporary status,
 if > 10000 && < 1000000, it means
 
 */
#define CB_SM_STATUS_CODE_OK 0x1000000
#define CB_SM_STATUS_CODE_SMPP_BALANCE_NOT_ENOUGH 0x1000001
#define CB_SM_STATUS_CODE_UNKNOWN 0x1000002

inline bool is_final_status_code(unsigned int status_code)
{
	return (status_code >= 10000);
}
#endif