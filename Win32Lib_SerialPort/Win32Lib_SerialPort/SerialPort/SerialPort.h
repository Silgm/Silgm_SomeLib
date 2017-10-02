#ifndef _SERIAL_PORT_H_
#define _SERIAL_PORT_H_

//C++ std head file
#include <string>
#include <sstream>

//C std head file
#include <cstdarg>
#include <cstdio>
#include <cstdlib>

//Windows api
#include <windows.h>

namespace slib {
	class SerialPort {
		static constexpr size_t MAX_BUFF_LEN = 1024;
		static constexpr size_t BUILD_COM_TIME = 1000;

	public:
		using TypeString = std::string;
		using TypeBaudRate = DWORD;
		using TypeParity = BYTE;
		using TypeByteSize = BYTE;
		using TypeStopBits = BYTE;

		enum EnumParity {
			P_NO = NOPARITY,
			P_ODD = ODDPARITY,
			P_EVEN = EVENPARITY,
			P_MARK = MARKPARITY,
			P_SPACE = SPACEPARITY
		};

		enum EnumStopBits {
			SB_1 = ONESTOPBIT,
			//SB_1_5 = ONE5STOPBITS, /*Can not be used by virtual SerialPort*/
			SB_2 = TWOSTOPBITS
		};

		SerialPort();
		virtual ~SerialPort();


		bool connect(TypeString com, TypeBaudRate baudRate,
			TypeParity parity = P_NO,
			TypeByteSize byteSize = 8,
			TypeStopBits stopBits = SB_1
		);
		bool disConnect();
		bool isConnect() const;

		TypeString getErrorInfo() const;

		int printf(const char *format, ...);
		TypeString readStr(TypeString::size_type size);

	private:
		HANDLE			m_handler;		//当前串口的句柄
		COMSTAT			m_status;		//串口状态
		DWORD			m_errors;		//串口错误标志

		TypeString		m_errorMessage;	//串口错误信息
		TypeString		m_comName;		//串口名称（标号 如:"COM1", "COM2", "COM3" ...）
		TypeBaudRate	m_baudRate;		//串口波特率（ 如:9600, 4800 ...）
		TypeParity		m_parity;		//串口奇偶校验（如:P_NO）
		TypeByteSize	m_byteSize;		//串口数据位（如:5,6,7,8）
		TypeStopBits	m_stopBits;		//串口停止位（如:SB_1, SB_2）

		std::ostringstream m_ostrm;		//内置字符流
		std::istringstream m_istrm;		//内置字符流

		bool			m_connected;	//串口连接状态（如true为已经连接, false为未连接）

										//private function
		bool writeSerialPort(const char * buffer, unsigned int buf_size);
		int	readSerialPort(char *buffer, unsigned int buf_size);
	};
}


#endif // !_SERIAL_PORT_H_
