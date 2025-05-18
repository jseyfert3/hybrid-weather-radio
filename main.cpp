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
#include "libraries/nlohmann/json.hpp"
#include <chrono> // for date & time
#include <thread> // for sleep
#include "fetchAlerts.h"

//using json = nlohmann::json;
//std::string state = "OK"; // TODO: if this state isn't valid, then we will get a json back with an error saying so. Need to handle that properly

int main(void)
{    
    std::ifstream config_file("config.json");
    nlohmann::json config = nlohmann::json::parse(config_file);
    config_file.close();
    std::string state = config["state"].template get<std::string>();

    std::string alerts = fetchAlerts(state);
    if(alerts.size() > 0)
    {
        auto now = std::chrono::system_clock::now(); // get current time and date
        std::chrono::zoned_time localTime{"America/Chicago", now}; // if wrong timezone, will throw error. Consider try/catch https://en.cppreference.com/w/cpp/chrono/time_zone/to_local
        std::string datetime = std::format("{0:%F_%T%z}_", localTime); // formate into a string https://en.cppreference.com/w/cpp/chrono/system_clock/formatter
        
        std::string filename = datetime + state + "_alerts.json";
        std::ofstream file(filename);
        file << alerts;;
        nlohmann::json data = nlohmann::json::parse(alerts);
        std::cout << "Number of alerts: ";
        std::cout << data["features"].size() << std::endl;
        file.close();
    }

    return 0;
} 
