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
  string url = "http://35.233.25.116/sitemap/hotels/Amsterdam/?page=";
  int crawl = 1;

  while(crawl) {
  if(curl) {
    url += std::to_string(crawl);
    crawl++;
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1L);
    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    
    if (res == 22) {
        crawl = 0;
    }

    regex link_regex("class=\"hotellink\" href=\"(.*)\"");
    
    auto link_begin = sregex_iterator(readBuffer.begin(), readBuffer.end(), link_regex);
    auto link_end = sregex_iterator();


	 try {
                            /* Create a connection */
                            sql::Driver *driver = get_driver_instance();
                            sql::Connection *con = driver->connect(DB_URL, DB_USER, DB_PASSWORD);

                            /* Connect to the MySQL test database */
                            con->setSchema(DB_SHEMA);
                            sql::PreparedStatement *stmt = con->prepareStatement(
                                    "insert into data (data) values (?);");
                            for (sregex_iterator i = link_begin; i != link_end; ++i) {
                                smatch match = *i;
                                // group 0, volledige match
                                // group 1, regex group 1
                                // ...
                                string match_str = match.str(1);
                            stmt->setString(1,match_str  );
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
    
  }


  return 0;
}
