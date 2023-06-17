#include "../includes/UART.h"

using namespace std::literals::chrono_literals;

uart::uart(){
	this->uart0_filestream = -1;
	this->openUart0 = false;

	this->readbuffer = new unsigned char[9];
	this->sendbuffer = new unsigned char[9];
}

uart::~uart(){
	if (this->openUart0) close(this->uart0_filestream);
}

bool uart::openUart(){

	this->uart0_filestream = open("/dev/serial0",O_RDWR | O_NOCTTY | O_NDELAY);
	if (this->uart0_filestream == -1){
		std::cout << "Failed to open uart serial 0" << std::endl;
		return false;
	}
	this->openUart0 = true;
	return true;
}

bool uart::defaultOptions(){
	struct termios options;

	if (this->uart0_filestream == -1){
		std::cout << "Uart 0 was not properly open, can't set options: " << this->uart0_filestream << std::endl;
		return false;
	}
	tcgetattr(this->uart0_filestream, &options);	

	options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;
	options.c_iflag = IGNPAR;
	options.c_oflag = 0;
	options.c_lflag = 0;

	tcflush(this->uart0_filestream, TCIFLUSH);
	tcsetattr(this->uart0_filestream, TCSANOW, &options);
	
	return true;
}

bool uart::sendMessage(){
	
	int count = write(this->uart0_filestream,this->sendbuffer,9);

	if (count < 0){
		std::cout << "Failed to send data" << std::endl;
		return false;
	}

	return true;
}

bool uart::readMessage(bool returnType){
	int count = read(this->uart0_filestream,(void*)this->readbuffer,9);

	if (count < 0){
		std::cout << "Failed to read data: " << count << std::endl;
		for (int i = 0; i < 9; ++i) printf("%d ",this->readbuffer[i]);
		return false;
	}

	if (count == 0){
		std::cout << "No data available" << std::endl;
		return true;
	}
	
	if(returnType == true) memcpy(&this->data,&this->readbuffer[3],sizeof(float));
	if(returnType == false) memcpy(&this->intdata,&this->readbuffer[3],sizeof(int));

	return true;
}

unsigned char* uart::buildGetMessage(){
	unsigned char* msg = new unsigned char[9] {ADDRESS, GET, ZEROCODE, 
		REGISTRATION0, 
		REGISTRATION1, 
		REGISTRATION2, 
		REGISTRATION3
	};
	return msg;
}


void uart::getUserInput(){
	this->sendbuffer = buildGetMessage();
	this->sendbuffer[2] = USER_COMMANDS;

	uint16_t crc = this->computeCrc(this->sendbuffer,7);
	memcpy(&this->sendbuffer[7],&crc,sizeof(crc));

	this->sendMessage();
	std::this_thread::sleep_for(0.5s);
	this->readMessage(false);

	switch(this->intdata) {
		case 5:
			std::cout << "CASE 5: + time" << std::endl;
			this->userTimer++;
			break;
		case 6:
			std::cout << "CASE 6: + time" << std::endl;
			this->userTimer--;
			break;	
		default:
			std::cout << "default case" << std::endl;
	}
}

void uart::getTemperature(unsigned char WICHTEMPERATURE){
	this->sendbuffer = buildGetMessage();
	this->sendbuffer[2] = WICHTEMPERATURE;


	uint16_t crc = this->computeCrc(this->sendbuffer,7);
	memcpy(&this->sendbuffer[7],&crc,sizeof(crc));
	
	this->sendMessage();
	std::this_thread::sleep_for(0.5s);
	this->readMessage(true);

	if (this->compareCrc()) this->getTemperature(WICHTEMPERATURE);
}

float uart::getInternalTemperature(){
	this->getTemperature(INTERNAL_TEMP);
	std::cout << "Internal Temperature: " <<this->data << std::endl;
	return this->data;
}

float uart::getReferenceTemperature(){
	this->getTemperature(REFERENCE_TEMP);
	std::cout << "Reference Temperature: " <<this->data << std::endl;
	return this->data;
}

bool uart::compareCrc() {
	return (this->sendbuffer[2] != this->readbuffer[2]);
}