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

string get_agent(int index) {
    const char *list_agent[] = {
            "Mozilla/5.0 (Linux; Android 8.0.0; SM-G960F Build/R16NW) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/Zoiper Click2Dial62.0.3202.84 Mobile Safari/537.36",
            "Mozilla/5.0 (Linux; Android 7.0; SM-G892A Build/NRD90M; wv) AppleWebKit/537.36 (KHTML, like Gecko) Version/4.0 Chrome/60.0.3112.107 Mobile Safari/537.36",
            "Mozilla/5.0 (Linux; Android 6.0.1; SM-G935S Build/MMB29K; wv) AppleWebKit/537.36 (KHTML, like Gecko) Version/4.0 Chrome/Zoiper Click2Dial55.0.2883.91 Mobile Safari/537.36",
            "Mozilla/5.0 (iPhone; CPU iPhone OS 12_0 like Mac OS X) AppleWebKit/605.1.15 (KHTML, like Gecko) CriOS/69.0.3497.105 Mobile/15E148 Safari/605.1",
    }
    return list_agent[index % 4];
}

using namespace std;

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    ((string *) userp)->append((char *) contents, size * nmemb);
    return size * nmemb;
}

int main(int argc, char *argv[]) {
    try {
        /* Create a connection */
        sql::Driver *driver = get_driver_instance();
        sql::Connection *con = driver->connect(DB_URL, DB_USER, DB_PASSWORD);

        /* Connect to the MySQL test database */
        con->setSchema(DB_SHEMA);

        sql::PreparedStatement *select_stmt = con->prepareStatement(
                "select name, location from hotel;");
        sql::ResultSet *res = select_stmt->executeQuery();
        while (res->next()) {
            string url = "http://35.233.25.116/rates/" + res->getString("location")+ "/10502/?&destination=Amsterdam&arrivalDate=2021-12-25&departureDate=2021-12-26&numPersons=1"

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


            sql::PreparedStatement *stmt = con->prepareStatement(
                    "insert into data (data) values (?);");
            for (sregex_iterator i = link_begin; i != link_end; ++i) {
                smatch match = *i;
                // group 0, volledige match
                // group 1, regex group 1
                // ...
                string match_str = match.str(1);
                stmt->setString(1, match_str);
                stmt->execute();
            }
            stmt->close();
            con->close();
            delete con;
            driver->threadEnd();
            delete stmt, driver;
        } catch (sql::SQLException & e)
        {
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
