#pragma once
//для выдачи страницы
#include <fstream>
#include <filesystem>
#include <string>
#include <string_view>

class Request{
public:
	Request() = delete;
	Request(int descriptor,const char* packet_data,int resived_bytes):descriptor_(descriptor),
	packet_data_(packet_data),resived_bytes_(resived_bytes) {};
		//ответ на запрос
	void Answer ();

private:
const int descriptor_;
const char* packet_data_ = nullptr;
int resived_bytes_ = 0;


	void HtmlOk ();
	void HtmlErr ();
	void HtmlOk (std::ifstream& file);

	//посылка страницы
	void SendFile(std::string_view file_name);

	enum RequestType{
		GET,
		POST,
		HEAD,
		OPTIONS,
		UNNOUN
	};

	//Размер слова до пробела
	std::string_view GetWord (const int first);
	RequestType GetType(std::string_view type);
	std::string GetPostText();
	
	void PocessingGET(std::string_view type);
	void PocessingPOST(std::string_view type);
};

