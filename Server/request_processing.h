#pragma once
//для выдачи страницы
#include <fstream>
#include <filesystem>
#include <string>

//посылка страницы
void SendFile(int descriptor, const std::string& file_name){
using namespace std::literals;
using std::filesystem::path;
	path p = path("..") / path("WebPage") / path(file_name);
	std::ifstream file(p, std::ios::in | std::ios::binary);
	if(!file.is_open()){
		std::cout<<"error read file "<<p<<std::endl;
		std::cout<<"file |"<<file_name<<"|"<<std::endl;
		return;
	}
	
	std::string html_ok = "HTTP/1.1 200 OK\n\n"s;
	send (descriptor, html_ok.data(),html_ok.size(), MSG_NOSIGNAL); 
	std::cout<<html_ok;
	
	char buffer[1024];
	int parts = 0;
	do {
		file.read(buffer, sizeof(buffer));
		
			//вывод буффера в консоль
			std::cout<<"Send "<<parts++<<" part"<<std::endl;
			for(int i = 0; i < file.gcount(); ++i){
				std::cout<<buffer[i];
			}
			std::cout<<std::endl;
			
		//отправка ответа
		send (descriptor, buffer,file.gcount(), MSG_NOSIGNAL); 
		
	} while (file);
	
	file.close();  
}

//ответ на запрос
void Answer(int descriptor, unsigned char* packet_data){
using namespace std::literals;
	std::string name;
	int pos=5;
	char temp=packet_data[pos];
	while (temp!=' '){
		name+=temp;
		temp=packet_data[++pos];
	}
	std::cout<<"FileName "<<name<<std::endl;
	if(name.empty()){
		SendFile(descriptor,"site.html"s);
	}else{
		SendFile(descriptor,name);
	}
}

