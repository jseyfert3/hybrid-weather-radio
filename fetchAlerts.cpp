#include <iostream>
#include <string>
#include <curl/curl.h>

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::string fetchAlerts(std::string state)
{
    CURL *curl = curl_easy_init(); //initialize curl
    std::string readBuffer; //buffer string to hold reply from NWS
    const char *curl_easy_strerror(CURLcode errornum); // for getting human-readable error message
 
    if(curl)
    {
        CURLcode res;
        std::string stateUrl = "https://api.weather.gov/alerts/active?area=" + state;
        curl_easy_setopt(curl, CURLOPT_URL, stateUrl.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "jseyfert3@gmail.com"); 
        res = curl_easy_perform(curl); //if 0, ok. Non-zero means error occured.
        if(res != CURLE_OK)
        {
            std::cout << "curl_easy_perform() failed with error code " << int(res) << ": " << curl_easy_strerror(res) << '\n';
        }
        curl_easy_cleanup(curl);
    }
    return readBuffer;
}

