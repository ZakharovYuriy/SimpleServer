#pragma once
//для выдачи страницы
#include <fstream>
#include <filesystem>
#include <string>
#include <string_view>

class Request{
public:
	Request() = delete;
	Request(int descriptor):descriptor_{descriptor}{};
		//ответ на запрос
	void Answer (const char* packet_data);

private:
const int descriptor_;

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
	std::string_view GetWord (const int first, const char* packet_data);
	RequestType GetType(std::string_view type);
	
	void PocessingGET(const char* packet_data, std::string_view type);
	void PocessingPOST(const char* packet_data, std::string_view type);
};

