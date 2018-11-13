#pragma once
#ifndef __AUTH
#define __AUTH

namespace AUTHENTICATION
{
	static int writer(char *data, size_t size, size_t nmemb, std::string *writerData);

	std::string PostCall(std::string strAddress, std::string strPost);

	std::string GetServerVariable(std::string key);
	std::string base64_decode(const std::string &in);
	VOID MessageBox_(LPCSTR Text, LPCSTR Title);
}
#endif