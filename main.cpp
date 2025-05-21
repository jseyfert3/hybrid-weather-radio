/*
Copyright 2025 Jonathan Seyfert

This file is part of hybrid-weather-radio.

hybrid-weather-radio is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License
as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

hybrid-weather-radio is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with hybrid-weather-radio. If not, see <https://www.gnu.org/licenses/>. 
*/

#include <iostream> // for cout, etc
#include <fstream> // for config (and future other file) IO
#include <string>
#include "libraries/nlohmann/json.hpp" // for parsing config files and JSON data returned from api.weather.gov
#include <chrono> // for date & time
#include <thread> // for sleep
#include "fetchAlerts.h"

int main(void)
{    
	std::ifstream configFile("config.json");
	nlohmann::json config = nlohmann::json::parse(configFile);
	configFile.close();
	std::string state = config["state"].template get<std::string>(); // TODO: if this state isn't valid, then we will get a json back with an error saying so. Need to handle that properly
	
	//std::string alerts = "";
	std::string loadFile = config["loadFile"].template get<std::string>(); // if we want to load a config file instead of poll api.weather.gov
	nlohmann::json alerts;
	if(loadFile != "")
	{
		std::ifstream alertFile(loadFile);
		alerts = nlohmann::json::parse(alertFile);
		alertFile.close();
		std::cout << "loaded alerts from file " << loadFile << "\n";
	}
	else
	{
		alerts = nlohmann::json::parse(fetchAlerts(state)); //TODO: will those error parsing if doesn't fetch results
		std::cout << "Fetched alerts for state of " << state << "\n";
	}
	

	auto now = std::chrono::system_clock::now(); // get current time and date
	std::chrono::zoned_time localTime{"America/Chicago", now}; // if wrong timezone, will throw error. Consider try/catch https://en.cppreference.com/w/cpp/chrono/time_zone/to_local
	std::string datetime = std::format("{0:%F_%T%z}_", localTime); // formate into a string https://en.cppreference.com/w/cpp/chrono/system_clock/formatter
	
	if(loadFile == "") // save a file, only if we are not loading alerts from existing config file
	{
		std::string filename = datetime + state + "_alerts.json";
		std::ofstream file(filename);
		file << std::setw(4) << alerts; // "pretty print" the nlohmann::json to file
		file.close();
	}
	
	std::cout << "Number of alerts: ";
	std::cout << alerts["features"].size() << std::endl;
	
	for(long unsigned int i = 0; i < alerts["features"].size(); i++) 
	{
		std::cout << "Alert #" << i << ":\n";
		std::cout << "    " << alerts["features"][i]["properties"]["event"] << "\n";
		std::cout << "    SAME codes:\n";
		for(long unsigned int j = 0; j < alerts["features"][i]["properties"]["geocode"]["SAME"].size(); j++)
		{
			std::cout << "        " << alerts["features"][i]["properties"]["geocode"]["SAME"][j].template get<std::string>();
			if(alerts["features"][i]["properties"]["geocode"]["SAME"][j].template get<std::string>() == "040131")
				std::cout << " WARNING!\n";
			else
				std::cout << "\n";
		}
		std::cout << "    Has geometry: ";
		if(alerts["features"][i]["geometry"] != nullptr)
			std::cout << "Yes" << "\n";
		else
			std::cout << "No" << "\n";
		std::cout << "\n";
	}
	return 0;
} 
