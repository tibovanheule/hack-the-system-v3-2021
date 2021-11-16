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

#include <iostream>
#include <fstream>

using namespace  std;

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    		((string*)userp)->append((char*)contents, size * nmemb);
    		return size * nmemb;
}

int main(int argc, char *argv[])
{

  CURL *curl;
  CURLcode res;
  string readBuffer;

  curl = curl_easy_init();

  if(curl) {
    curl_easy_setopt(curl, CURLOPT_URL, "https://www.google.com");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    regex link_regex("href=\"(.*?)\"");
    
    auto link_begin = sregex_iterator(readBuffer.begin(), readBuffer.end(), link_regex);
    auto link_end = sregex_iterator();

    ofstream results;
    results.open("results.csv");
    
    for (sregex_iterator i = link_begin; i != link_end; ++i) {
        smatch match = *i;
        // group 0, volledige match
        // group 1, regex group 1
        // ...
        string match_str = match.str(1);
        cout << match_str << '\n';

        results << match_str << "\n";

    }

    results.close();
    return 0;
  }

}
