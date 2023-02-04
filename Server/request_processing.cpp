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
#include "program_exec.h"

using namespace std;
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
std::string_view Request::GetWord (const int first){
	int pos=first;
	char temp = packet_data_[pos];
	int elements=0;
	while (temp!=' ' && pos<resived_bytes_){
		++elements;
		temp=packet_data_[++pos];
	}
return std::string_view(packet_data_+first,elements);
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

void Request::PocessingGET(std::string_view type){
	//размер имени запрашиваемого файла
	std::string_view name = GetWord(type.size()+2);
	std::cout<<"FileName "s<<name<<std::endl;
	if(name.empty()){
		SendFile("site.html"s);
	}else{
		SendFile(name);
	}
}

string Request::GetPostText(){
	bool got_it = false;
	int pos=0;
	int i=0;
	while(i<resived_bytes_){
		if(packet_data_[i]=='\n'){
			if(got_it){
				pos=i;
				break;
			}
			got_it=true;
		}else if(packet_data_[i]>32){
			got_it=false;
		}
		++i;
	}
	string result="";
	while(++pos<resived_bytes_){
		result+=packet_data_[pos];
	}
	return result;
}

void Request::PocessingPOST(std::string_view type){
	//размер имени запрашиваемого файла
	std::string_view file_name = GetWord(type.size()+2);
	std::cout<<"FileName "s<<file_name<<std::endl;
	if(file_name.empty()){
		HtmlErr();
	}else{
		//open executable file
		path execf = path("..") / path("WebPage") / path("programs") / path(file_name);
		string answer = ExecFile(execf, GetPostText());
		//open result file
		path resf = path("..") / path("WebPage") / path("programs") / path("answers") / path(file_name);
		std::ofstream result(resf, std::ios::out | std::ios::trunc);
		if(!result.is_open()){
			std::cout<<"error open file "s<< resf <<std::endl;
			std::cout<<"file |"s<< file_name <<"|"s<<std::endl;
			HtmlErr();			
			return;
		}
		answer+='\n';
		while(answer.size()<8)
		{
			answer+=" ";
		}
		result<<answer;
		result.close();
		HtmlOk();
	}
}

//ответ на запрос
void Request::Answer (){
	//размер названия запроса
	std::string_view type = GetWord(0);
	//по запросу GET посылаем запрашиваемый файл
	if (GetType(type)==GET){
		PocessingGET(type);
	}
	//по запросу GET посылаем запрашиваемый файл
	if (GetType(type)==POST){
		PocessingPOST(type);
	}
};


