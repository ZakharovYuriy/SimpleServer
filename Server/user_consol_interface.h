#pragma once
#include <iostream>
#include <string>

class UserRequest{
public:
UserRequest(int argc, char* argv[]){
using namespace std::literals;
	//обработка входных аргументов
	if (argc == 2) {
		const std::string mode(argv[1]);
		if (mode == "-help"s || mode == "-h"s) {
			PrintHelp();
			exit(EXIT_SUCCESS);
		} else {
			try{
				IP = ParceIP(mode);
				PORT = ParcePort(mode);
			}
			catch(...){
				std::cout<<"error_wrong IP"<<std::endl;
				exit(EXIT_SUCCESS);
			}
		} 
	}else if (argc==3){
		try{
			IP = argv[1];
			PORT = ParcePort(argv[2]);
		}
		catch(...){
			std::cout<<"error_wrong IP"<<std::endl;
			exit(EXIT_SUCCESS);
		}
	}
}

void ShowData(const char* packet_data, int resived_bytes){
	std::cout<<"recived "<<resived_bytes<< " bytes"<<std::endl;
	for(int i = 0; i < resived_bytes-1; ++i){
		std::cout<<packet_data[i];
	}
	std::cout<<std::endl;
}

std::string GetIP (){
	return IP;
}

int GetPORT (){
	return PORT;
}

private:
std::string IP="127.0.0.1";
int PORT=12345;

void PrintHelp(std::ostream &stream = std::cerr) {
	std::string help =
R"123(
ways to start the server:
-without parameters: the server will start at 127.0.0.1:12345

-with ip address and port separated by a space "127.0.0.1 12345"
 
-with ip address and port colon space "127.0.0.1:12345"
)123";
	stream << help;
}

std::string ParceIP(const std::string& str){
	std::string result;
	int pos=0;
	while(pos < str.size() && str[pos]!=':'){
		result+=str[pos++];
	}
	return result;
}

int ParcePort(const std::string& str){
	int pos = ParceIP(str).size();
	pos = pos==str.size()? 0 : pos+1;
	return std::stoi(str.substr(pos,str.size()-pos));
}
};
