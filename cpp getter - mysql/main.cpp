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


#define DB_PASSWORD "GmkbyY9s8swPdx4G"
#define DB_USER "doadmin"
#define DB_SHEMA "defaultdb"
#define DB_URL "tcp://hackthesystem-do-user-2712550-0.b.db.ondigitalocean.com:25060"

//Mysql lib's
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/prepared_statement.h>

#ifdef _WIN32
#include <Windows.h>
#else

#include <unistd.h>

#endif

extern "C" size_t decode_html_entities_utf8(char *dest, const char *src);


using namespace  std;

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    		((string*)userp)->append((char*)contents, size * nmemb);
    		return size * nmemb;
}


std::string gen_random(const int len) {
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    std::string tmp_s;
    tmp_s.reserve(len);

    for (int i = 0; i < len; ++i) {
        tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
    }
    
    return tmp_s;
}





int main(int argc, char *argv[])
{

  CURL *curl;
  CURLcode res;
  string readBuffer;

  curl = curl_easy_init();
  int crawl = 1;

  while(crawl < 14) {
  if(curl) {
    string url = "http://35.233.25.116/sitemap/hotels/Brussels/?page=";
    url += to_string(crawl);
    crawl++;
    cout << "URL: "<< url << endl;
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, gen_random(12));
    res = curl_easy_perform(curl);
    
    cout << "BUFFER" << res << endl;

    regex link_regex("class=\"hotellink\" href=\"(.*)\"");
    
    auto link_begin = sregex_iterator(readBuffer.begin(), readBuffer.end(), link_regex);
    auto link_end = sregex_iterator();

    cout << "I GOT HERE" << endl;
    sleep(5);
	 try {
                            /* Create a connection */
                            sql::Driver *driver = get_driver_instance();
                            sql::Connection *con = driver->connect(DB_URL, DB_USER, DB_PASSWORD);

                            /* Connect to the MySQL test database */
                            con->setSchema(DB_SHEMA);
                            sql::PreparedStatement *stmt = con->prepareStatement(
                                    "insert into hotel (name, rooms, lat, `long`, location) values (?, ?, ?, ?, 'Brussels');");
                            for (sregex_iterator i = link_begin; i != link_end; ++i) {
                                smatch match = *i;
                                // group 0, volledige match
                                // group 1, regex group 1
                                // ...
                                string match_str = match.str(1);
                            string read;
                            cout << "THIS PAGE IS: "<< read << endl;
                            string url = "http://35.233.25.116";
                            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &read);
                            url += match_str;
                            cout << url << endl;
                            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
                            curl_easy_setopt(curl, CURLOPT_USERAGENT, gen_random(12));
                            cout << gen_random(12);
                            res = curl_easy_perform(curl);
                            cout << "RES IS: " << res << endl;
                            if (res != 0){
                                continue;
                            }
                            
                            long http_code = 0;
                            curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &http_code);
                            cout << http_code << endl;

                            //decode_html_entities_utf8(read.c_str(), 0);
                            regex regex("<h5 class=\"card-title text-truncate\">(.*)</h5>");
                            std::smatch m;
                            regex_search(read, m, regex);
                            string hotelNaam = m[1];
                            stmt->setString(1, hotelNaam.c_str());
                            std::regex roomRegex("([0-9]+) rooms", std::regex_constants::icase);
                            std::smatch rm;
                            regex_search(read, rm , roomRegex);
                            string roomAantal = rm[1];
                            stmt->setString(2, roomAantal);


                            std::regex lat("lat ([0-9.]*)", std::regex_constants::icase);
                            std::smatch latm;
                            regex_search(read, latm, lat);
                            string latCoords = latm[1];
                            stmt->setString(3, latCoords);



                            std::regex mLong("long ([0-9.]*)", std::regex_constants::icase);
                            std::smatch longm;
                            regex_search(read, longm, mLong);
                            string longCoords = longm[1];
                            stmt->setString(4, longCoords);





                            stmt->execute();
                            }
                            stmt->close();
                            con->close();
                            delete con;
                            driver->threadEnd();
                            delete stmt, driver;
                        } catch (sql::SQLException &e) {
                            cout << "# ERR: SQLException in " << __FILE__;
                            cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
                            cout << "# ERR: " << e.what();
                            cout << " (MySQL error code: " << e.getErrorCode();
                            cout << ", SQLState: " << e.getSQLState() << " )" << endl;
                        }

    }
  readBuffer.clear();
    
  }


  return 0;
}
