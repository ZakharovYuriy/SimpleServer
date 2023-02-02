#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
//для выдачи страницы
#include <fstream>
#include <filesystem>
#include <string>
#include <string_view>
#include <iostream>

#include "request_processing.h"

using namespace std::literals;
using std::filesystem::path;

void Request::HtmlOk (){
	std::string html_ans = "HTTP/1.1 200 OK\n\n"s;
	send (descriptor_, html_ans.data(),html_ans.size(), MSG_NOSIGNAL); 
	std::cout<<html_ans<<std::endl;
}
	
void Request::HtmlErr (){
	std::string html_ans = "HTTP/1.1 404\n\n"s;
	send (descriptor_, html_ans.data(),html_ans.size(), MSG_NOSIGNAL); 
	std::cout<<html_ans<<std::endl;
}

void Request::HtmlOk (std::ifstream& file){
	std::string html_ok = "HTTP/1.1 200 OK\n"s;
	const auto fsize = file.tellg();
	file.seekg(0L, std::ios_base::beg);
	
	html_ok += "Content-Length:" + std::to_string(fsize) + "\n\n";
	send (descriptor_, html_ok.data(),html_ok.size(), MSG_NOSIGNAL); 
	std::cout<<html_ok<<std::endl;
}

//посылка страницы
void Request::SendFile(std::string_view file_name){
	path p = path("..") / path("WebPage") / path(file_name);
	std::ifstream file(p, std::ios::in | std::ios::ate | std::ios::binary);
	if(!file.is_open()){
		std::cout<<"error read file "s<< p <<std::endl;
		std::cout<<"file |"s<< file_name <<"|"s<<std::endl;
		HtmlErr();			
		return;
	}
	
	HtmlOk(file);

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
		send (descriptor_,buffer,file.gcount(), MSG_NOSIGNAL); 
	} while (file);
	file.close();  
};


//Размер слова до пробела
std::string_view Request::GetWord (const int first, const char* packet_data){
	int pos=first;
	char temp = packet_data[pos];
	int elements=0;
	while (temp!=' '){
		++elements;
		temp=packet_data[++pos];
	}
return std::string_view(packet_data+first,elements);
};

Request::RequestType Request::GetType(std::string_view type){
	if(type=="GET"sv){
		return RequestType::GET;
	}else if(type=="GET"sv){
		return RequestType::GET;
	}else if(type=="POST"sv){
		return RequestType::POST;
	}else if(type=="HEAD"sv){
		return RequestType::HEAD;
	}else if(type=="OPTIONS"sv){
		return RequestType::OPTIONS;
	}
	return RequestType::UNNOUN;
}

void Request::PocessingGET(const char* packet_data, std::string_view type){
	//размер имени запрашиваемого файла
	std::string_view name = GetWord(type.size()+2,packet_data);
	std::cout<<"FileName "s<<name<<std::endl;
	if(name.empty()){
		SendFile("site.html"s);
	}else{
		SendFile(name);
	}
}

void Request::PocessingPOST(const char* packet_data, std::string_view type){
	//размер имени запрашиваемого файла
	std::string_view name = GetWord(type.size()+2,packet_data);
	std::cout<<"FileName "s<<name<<std::endl;
	HtmlOk();
}

//ответ на запрос
void Request::Answer (const char* packet_data){
	//размер названия запроса
	std::string_view type = GetWord(0,packet_data);
	//по запросу GET посылаем запрашиваемый файл
	if (GetType(type)==GET){
		PocessingGET(packet_data,type);
	}
	//по запросу GET посылаем запрашиваемый файл
	if (GetType(type)==POST){
		PocessingPOST(packet_data,type);
	}
};


