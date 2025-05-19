/*
Copyright 2025 Jonathan Seyfert

This file is part of hybrid-weather-radio.

hybrid-weather-radio is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License
as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

hybrid-weather-radio is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with hybrid-weather-radio. If not, see <https://www.gnu.org/licenses/>. 
*/

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
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "https://github.com/jseyfert3/hybrid-weather-radio"); 
        res = curl_easy_perform(curl); //if 0, ok. Non-zero means error occured.
        if(res != CURLE_OK)
        {
            std::cout << "curl_easy_perform() failed with error code " << int(res) << ": " << curl_easy_strerror(res) << '\n';
        }
        curl_easy_cleanup(curl);
    }
    return readBuffer;
}

