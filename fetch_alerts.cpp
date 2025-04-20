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

using json = nlohmann::json;

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
	curl_easy_setopt(curl, CURLOPT_URL, "https://api.weather.gov/alerts/active?area=OK");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
	curl_easy_setopt(curl, CURLOPT_USERAGENT, "jseyfert3@gmail.com"); 
	res = curl_easy_perform(curl); //if 0, ok. Non-zero means error occured.
    curl_easy_cleanup(curl);

//	std::cout << readBuffer << std::endl;
	std::cout << "Creating file" << std::endl;
	std::ofstream file("alerts.json");
	file << readBuffer;;
	json data = json::parse(readBuffer);
	std::cout << "Number of alerts: ";
	std::cout << data["features"].size() << std::endl;
	file.close();
	}
  return 0;
} 
