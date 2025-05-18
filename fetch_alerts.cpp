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
#include <fstream>
#include <string>
#include <curl/curl.h>
#include "libraries/nlohmann/json.hpp"
#include <chrono> // for date & time

using json = nlohmann::json;
std::string state = "OK";

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

int main(void)
{
  CURL *curl = curl_easy_init(); //initialize curl
  std::string readBuffer; //buffer string to hold reply from NWS

  if(curl) {
	CURLcode res;
    std::string stateUrl = "https://api.weather.gov/alerts/active?area=" + state;
    curl_easy_setopt(curl, CURLOPT_URL, stateUrl.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
	curl_easy_setopt(curl, CURLOPT_USERAGENT, "jseyfert3@gmail.com"); 
	res = curl_easy_perform(curl); //if 0, ok. Non-zero means error occured.
    if(res != 0)
    {
        std::cout << "Error: curl error #" << res << "\n";
        return 1;
    }
    curl_easy_cleanup(curl);

    auto now = std::chrono::system_clock::now(); // get current time and date
    std::chrono::zoned_time localTime{"America/Chicago", now}; // if wrong timezone, will throw error. Consider try/catch https://en.cppreference.com/w/cpp/chrono/time_zone/to_local
    std::string datetime = std::format("{0:%F_%T%z}_", localTime); // formate into a string https://en.cppreference.com/w/cpp/chrono/system_clock/formatter
    
    std::string filename = datetime + state + "_alerts.json";
	std::ofstream file(filename);
	file << readBuffer;;
	json data = json::parse(readBuffer);
	std::cout << "Number of alerts: ";
	std::cout << data["features"].size() << std::endl;
	file.close();
	}
  return 0;
} 
