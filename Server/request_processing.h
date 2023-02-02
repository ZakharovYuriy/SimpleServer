#pragma once
//для выдачи страницы
#include <fstream>
#include <filesystem>
#include <string>
#include <string_view>

namespace request{
	void HtmlOk (int descriptor){
		std::string html_ans = "HTTP/1.1 200 OK\n\n"s;
		send (descriptor, html_ans.data(),html_ans.size(), MSG_NOSIGNAL); 
		std::cout<<html_ans<<std::endl;
	}
	
	void HtmlErr (int descriptor){
		std::string html_ans = "HTTP/1.1 404\n\n"s;
		send (descriptor, html_ans.data(),html_ans.size(), MSG_NOSIGNAL); 
		std::cout<<html_ans<<std::endl;
	}
	
	void HtmlOk (int descriptor,std::ifstream& file){
		std::string html_ok = "HTTP/1.1 200 OK\n"s;
		const auto fsize = file.tellg();
		file.seekg(0L, std::ios_base::beg);
		
		html_ok += "Content-Length:" + std::to_string(fsize) + "\n\n";
		send (descriptor, html_ok.data(),html_ok.size(), MSG_NOSIGNAL); 
		std::cout<<html_ok<<std::endl;
	}

	//посылка страницы
	void SendFile(int descriptor, std::string_view file_name){
	using namespace std::literals;
	using std::filesystem::path;
	
		path p = path("..") / path("WebPage") / path(file_name);
		std::ifstream file(p, std::ios::in | std::ios::ate | std::ios::binary);
		if(!file.is_open()){
			std::cout<<"error read file "s<< p <<std::endl;
			std::cout<<"file |"s<< file_name <<"|"s<<std::endl;
			
			HtmlErr(descriptor);			
			return;
		}
		
		HtmlOk(descriptor,file);
	
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
	};

	enum RequestType{
		GET,
		POST,
		HEAD,
		OPTIONS,
		UNNOUN
	};

	//Размер слова до пробела
	std::string_view GetWord (const int first, const char* packet_data){
		int pos=first;
		char temp = packet_data[pos];
		int elements=0;
		while (temp!=' '){
			++elements;
			temp=packet_data[++pos];
		}
	return std::string_view(packet_data+first,elements);
	};
	
	RequestType GetType(std::string_view type){
	using namespace std::literals;
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

	//ответ на запрос
	void Answer (const int descriptor, const char* packet_data){
	using namespace std::literals;
		//размер названия запроса
		std::string_view type = GetWord(0,packet_data);
		
		//по запросу GET посылаем запрашиваемый файл
		if (GetType(type)==GET){
			//размер имени запрашиваемого файла
			std::string_view name = GetWord(type.size()+2,packet_data);
			std::cout<<"FileName "s<<name<<std::endl;
			
			if(name.empty()){
				SendFile(descriptor,"site.html"s);
			}else{
				SendFile(descriptor,name);
			}
		}
		
		//по запросу GET посылаем запрашиваемый файл
		if (GetType(type)==POST){
			//размер имени запрашиваемого файла
			std::string_view name = GetWord(type.size()+2,packet_data);
			std::cout<<"FileName "s<<name<<std::endl;
			
			HtmlOk(descriptor);
		}
	};
}

