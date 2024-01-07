#include "ConfigurationManagerReturnCodeException.h"

#include <iomanip>
#include <sstream>

#define CASE_TO_STR(X) case X: os << #X; break
#define CASE_DEFAULT(X) default: os << "0x" << std::uppercase << std::hex << std::setw(8) << X << std::nouppercase; break

namespace Native
{
	namespace Windows
	{
		ConfigurationManagerReturnCodeException::ConfigurationManagerReturnCodeException(const CONFIGRET returnCode, const char* methodName)
			: Exception(nameof(ConfigurationManagerReturnCodeException), ConfigurationManagerReturnCodeException::FormatErrorMessage(returnCode, methodName)),
			_returnCode(returnCode)
		{
		}

		std::string ConfigurationManagerReturnCodeException::FormatErrorMessage(const CONFIGRET returnCode, const std::string& methodName)
		{
			std::ostringstream os;
			os << std::hex << std::setfill('0');

			os << methodName << "(...) failed! (CONFIGRET=";

			switch (returnCode)
			{
				CASE_TO_STR(CR_DEFAULT);
				CASE_TO_STR(CR_OUT_OF_MEMORY);
				CASE_TO_STR(CR_INVALID_POINTER);
				CASE_TO_STR(CR_INVALID_FLAG);
				CASE_TO_STR(CR_INVALID_DEVNODE);
				CASE_TO_STR(CR_INVALID_RES_DES);
				CASE_TO_STR(CR_INVALID_LOG_CONF);
				CASE_TO_STR(CR_INVALID_ARBITRATOR);
				CASE_TO_STR(CR_INVALID_NODELIST);
				CASE_TO_STR(CR_DEVNODE_HAS_REQS);
				CASE_TO_STR(CR_INVALID_RESOURCEID);
				CASE_TO_STR(CR_DLVXD_NOT_FOUND);
				CASE_TO_STR(CR_NO_SUCH_DEVNODE);
				CASE_TO_STR(CR_NO_MORE_LOG_CONF);
				CASE_TO_STR(CR_NO_MORE_RES_DES);
				CASE_TO_STR(CR_ALREADY_SUCH_DEVNODE);
				CASE_TO_STR(CR_INVALID_RANGE_LIST);
				CASE_TO_STR(CR_INVALID_RANGE);
				CASE_TO_STR(CR_FAILURE);
				CASE_TO_STR(CR_NO_SUCH_LOGICAL_DEV);
				CASE_TO_STR(CR_CREATE_BLOCKED);
				CASE_TO_STR(CR_NOT_SYSTEM_VM);
				CASE_TO_STR(CR_REMOVE_VETOED);
				CASE_TO_STR(CR_APM_VETOED);
				CASE_TO_STR(CR_INVALID_LOAD_TYPE);
				CASE_TO_STR(CR_BUFFER_SMALL);
				CASE_TO_STR(CR_NO_ARBITRATOR);
				CASE_TO_STR(CR_NO_REGISTRY_HANDLE);
				CASE_TO_STR(CR_REGISTRY_ERROR);
				CASE_TO_STR(CR_INVALID_DEVICE_ID);
				CASE_TO_STR(CR_INVALID_DATA);
				CASE_TO_STR(CR_INVALID_API);
				CASE_TO_STR(CR_DEVLOADER_NOT_READY);
				CASE_TO_STR(CR_NEED_RESTART);
				CASE_TO_STR(CR_NO_MORE_HW_PROFILES);
				CASE_TO_STR(CR_DEVICE_NOT_THERE);
				CASE_TO_STR(CR_NO_SUCH_VALUE);
				CASE_TO_STR(CR_WRONG_TYPE);
				CASE_TO_STR(CR_INVALID_PRIORITY);
				CASE_TO_STR(CR_NOT_DISABLEABLE);
				CASE_TO_STR(CR_FREE_RESOURCES);
				CASE_TO_STR(CR_QUERY_VETOED);
				CASE_TO_STR(CR_CANT_SHARE_IRQ);
				CASE_TO_STR(CR_NO_DEPENDENT);
				CASE_TO_STR(CR_SAME_RESOURCES);
				CASE_TO_STR(CR_NO_SUCH_REGISTRY_KEY);
				CASE_TO_STR(CR_INVALID_MACHINENAME);
				CASE_TO_STR(CR_REMOTE_COMM_FAILURE);
				CASE_TO_STR(CR_MACHINE_UNAVAILABLE);
				CASE_TO_STR(CR_NO_CM_SERVICES);
				CASE_TO_STR(CR_ACCESS_DENIED);
				CASE_TO_STR(CR_CALL_NOT_IMPLEMENTED);
				CASE_TO_STR(CR_INVALID_PROPERTY);
				CASE_TO_STR(CR_DEVICE_INTERFACE_ACTIVE);
				CASE_TO_STR(CR_NO_SUCH_DEVICE_INTERFACE);
				CASE_TO_STR(CR_INVALID_REFERENCE_STRING);
				CASE_TO_STR(CR_INVALID_CONFLICT_LIST);
				CASE_TO_STR(CR_INVALID_INDEX);
				CASE_TO_STR(CR_INVALID_STRUCTURE_SIZE);
				CASE_TO_STR(NUM_CR_RESULTS);
				CASE_DEFAULT(returnCode);
			}

			os << ")";

			return os.str();
		}
	}
}