/* Header only lib's, no need to install just include.
It does require to install Boost*/
#include <regex>
#include <iterator>

#include <stdlib.h>
#include <algorithm>
#include <iostream>

#include <sys/stat.h>

#include <string>
#include <curl/curl.h>

using namespace  std;

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    		((std::string*)userp)->append((char*)contents, size * nmemb);
    		return size * nmemb;
}

int main(int argc, char *argv[])
{

  CURL *curl;
  CURLcode res;
  std::string readBuffer;

  curl = curl_easy_init();
  if(curl) {
    curl_easy_setopt(curl, CURLOPT_URL, "https://www.google.com");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    std::regex regex("href=\"(.*?)\"");
    
    auto words_begin = std::sregex_iterator(readBuffer.begin(), readBuffer.end(), regex);
    auto words_end = std::sregex_iterator();

    for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
        std::smatch match = *i;
        std::string match_str = match.str();
        std::cout << match_str << '\n';
    }
  }

}
