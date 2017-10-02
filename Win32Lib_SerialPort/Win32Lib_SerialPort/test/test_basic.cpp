#include "test_main.h"	//Test Main head
#include "SerialPort.h"	//Lib Tested


#include <iostream>
#include <cstdlib>

using namespace slib;

int test_main(int argc, char *argv[]) {
	SerialPort serial;
	
	if (!serial.connect("COM2", 9600)) {
		std::cout << serial.getErrorInfo() << std::endl;
		return EXIT_FAILURE;
	}

	std::cout << "Connect!" << std::endl;
	while (1) {
		serial.printf("%d %s\n", 521, "Hello");
		Sleep(1000);
	}

	return EXIT_SUCCESS;
}

