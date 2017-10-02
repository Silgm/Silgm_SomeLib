#include "SerialPort.h"


namespace slib {

	SerialPort::SerialPort()
		: m_comName("COM1"), m_handler(NULL), m_connected(false), m_errorMessage("No Error")
	{
	}

	bool SerialPort::connect(TypeString com, TypeBaudRate baudRate,
		TypeParity parity,
		TypeByteSize byteSize,
		TypeStopBits stopBits)
	{
		if (this->m_connected)
			return this->m_connected;
		this->m_comName = com;
		this->m_baudRate = baudRate;
		this->m_parity = parity;
		this->m_byteSize = byteSize;
		this->m_stopBits = stopBits;

		this->m_handler = CreateFileA(static_cast<LPCSTR>((std::string("\\\\.\\") + this->m_comName).c_str()),
			GENERIC_READ | GENERIC_WRITE,
			0,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL);

		if (this->m_handler == INVALID_HANDLE_VALUE)
		{
			if (GetLastError() == ERROR_FILE_NOT_FOUND)
			{
				this->m_errorMessage = this->m_comName + " is not available\n";
				this->m_connected = false;
				return this->m_connected;
			}
			else
			{
				this->m_errorMessage = "Unknown Error\n";
				this->m_connected = false;
				return this->m_connected;
			}
		}
		else
		{
			DCB dcbSerialParameters = { 0 };

			if (!GetCommState(this->m_handler, &dcbSerialParameters))
			{
				this->m_errorMessage = "failed to get current serial parameters";
				this->m_connected = false;
				return this->m_connected;
			}
			else
			{
				dcbSerialParameters.BaudRate = this->m_baudRate;
				//dcbSerialParameters.ByteSize = 8;
				//dcbSerialParameters.StopBits = ONESTOPBIT;
				//dcbSerialParameters.Parity = NOPARITY;
				dcbSerialParameters.ByteSize = this->m_byteSize;
				dcbSerialParameters.StopBits = this->m_stopBits;
				dcbSerialParameters.Parity = this->m_parity;
				dcbSerialParameters.fDtrControl = DTR_CONTROL_ENABLE;

				if (!SetCommState(m_handler, &dcbSerialParameters))
				{
					this->m_errorMessage = "Could not set Serial port parameters\n";
					this->m_connected = false;
					return this->m_connected;
				}
				else {
					this->m_connected = true;
					PurgeComm(this->m_handler, PURGE_RXCLEAR | PURGE_TXCLEAR);
					Sleep(BUILD_COM_TIME);
				}
			}
		}
		return this->m_connected;
	}
	bool SerialPort::disConnect()
	{
		if (this->m_connected) {
			this->m_connected = CloseHandle(this->m_handler);
		}
		return this->m_connected;
	}
	bool SerialPort::isConnect() const
	{
		return this->m_connected;
	}
	SerialPort::TypeString SerialPort::getErrorInfo() const
	{
		return this->m_errorMessage;
	}

	int SerialPort::printf(const char * format, ...)
	{
		static char buffer[MAX_BUFF_LEN] = "";
		int count = 0;
		va_list argptr;

		va_start(argptr, format);
		count = vsprintf_s(buffer, MAX_BUFF_LEN, format, argptr);
		va_end(argptr);

		writeSerialPort(buffer, MAX_BUFF_LEN);

		return count;
	}

	SerialPort::TypeString SerialPort::readStr(TypeString::size_type size)
	{
		static char buffer[MAX_BUFF_LEN] = "";
		readSerialPort(buffer, size);
		return SerialPort::TypeString(buffer);
	}

	int SerialPort::readSerialPort(char * buffer, unsigned int buf_size)
	{
		DWORD bytesRead;
		unsigned int toRead = 0;

		ClearCommError(this->m_handler, &this->m_errors, &this->m_status);

		if (this->m_status.cbInQue > 0) {
			if (this->m_status.cbInQue > buf_size) {
				toRead = buf_size;
			}
			else toRead = this->m_status.cbInQue;
		}

		if (ReadFile(this->m_handler, buffer, toRead, &bytesRead, NULL))
			return bytesRead;

		return 0;
	}

	bool SerialPort::writeSerialPort(const char * buffer, unsigned int buf_size)
	{
		DWORD bytesSend;

		if (!WriteFile(this->m_handler, (void*)buffer, buf_size, &bytesSend, 0)) {
			ClearCommError(this->m_handler, &this->m_errors, &this->m_status);
			return false;
		}
		else return true;
	}

	SerialPort::~SerialPort()
	{
		disConnect();
	}
}
