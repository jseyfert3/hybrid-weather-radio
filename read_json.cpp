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
#include "libraries/nlohmann/json.hpp"
#include <string>
using json = nlohmann::json;

int main(void)
{
	std::ifstream f("2025-04-19 2324 TX alerts.json");
	json data = json::parse(f);
//	std::cout << data["features"];
//	std::cout << data["features"][1]["geometry"]["coordinates"];
//	std::cout << data["features"][0]["properties"]["description"] << std::endl; //prints newlines, because json excapes them when importing
//	std::string data2 = data["features"][0]["properties"]["description"]; //no newlines, since excaping newlines isn't done when exporting json to string or cout, so they display here
//	std::cout << data2 << std::endl;
//	std::cout << "Number of alerts: ";
//	std::cout << data["features"].size() << std::endl;
	for(int i = 0; i < data["features"].size(); i++)
	{
//		std::string display = data["features"][i]["properties"]["description"];
		if (data["features"][i]["properties"]["event"] == "Tornado Warning")
		{
			std::cout << "Alert #" << i + 1 << std::endl;
			std::cout << data["features"][i]["properties"]["areaDesc"] << std::endl;
			std::cout << data["features"][i]["properties"]["description"] << std::endl << std::endl;
		}
	}
}
