# hybrid-weather-radio
Internet-based weather alerts, with weather radio fallback in case of internet or server issues.

### Background
In the USA, there is a network of [transmitters that broadcast](https://en.wikipedia.org/wiki/Weather_radio) computer-voiced forecasts 24/7 with covereage in nearly all of the USA. With the internet, these are not nearly as useful as they once were. However, there is one particular area of special interest for the author of this project still: Weather alerts. You may be familiar with weather radios and alerts, or you may be familiar with the [Emergency Alert System](https://en.wikipedia.org/wiki/Emergency_Alert_System), which broadcasts alerts on TV and radio broadcasts. And of course more recently, there is [Wireless Emergency Alerts](https://en.wikipedia.org/wiki/Wireless_Emergency_Alerts) (WEA) which has weather alerts on your phone. However, at least in the experience of the author, WEA is not nearly as reliable as weather alerts on weather radio. And EAS, while reliable, is not very relivant anymore, since fewer and fewer people watch broadcast TV or listen to broadcast radio. So weather radio alerts are still important, especially in case of internet, cell phone, and/or power outages.

While highly reliable, weather radio alerts have a downside, and that is false alarms. At least in the midwest, the primary weather radio alert of interest would be a tornado warning. These are issued by the [NWS](https://www.weather.gov/) on a county-wide bases using [Specific Area Message Encoding](https://en.wikipedia.org/wiki/Specific_Area_Message_Encoding) (SAME). SAME has a digital header that includes county codes. With rare exceptions, this means that currently a tornado warning for any part of a county will activate the alarm feature of a listening weather radio, even if the tornado warning is not for the area of the listener. The rare exception is that SAME header county codes do have the ability in the protocol to sub-divide a county into up to 9 sub-divisions, but it's currently only used in two counties in the USA. I reached out to the local NWS office with another person to inquire if they would like to impliment county sub-division, and we got a less than enthusiastic reply that indicated no interest in implementing that for our area.

So what then? Well, the NWS draws tornado warnings as polygons using lat/long coordinates for the corners of the polygon. SAME headers do not include these lat/long coordinates, unfortunately. But they can be obtained from the NWS API. For example, for Wisconsin, you can access https://api.weather.gov/alerts/active?area=WI which will return JSON formatted data with all current alerts for the state of Wisconsin. If there are active alerts, then they will appear like this tornado warning in Oklahoma on 2025-04-19 with a query ran at 2324 CDT. Of particular interest is the `coordinates` key, which has the lat/long coordinates that describe a polygon that covers the area for the tornado warning:
```
{
    "@context": [
        "https://geojson.org/geojson-ld/geojson-context.jsonld",
        {
            "@version": "1.1",
            "wx": "https://api.weather.gov/ontology#",
            "@vocab": "https://api.weather.gov/ontology#"
        }
    ],
    "type": "FeatureCollection",
    "features": [
        {
            "id": "https://api.weather.gov/alerts/urn:oid:2.49.0.1.840.0.1ee7b56ae8604bf07e78a0ea34a4abf0da708c34.001.1",
            "type": "Feature",
            "geometry": {
                "type": "Polygon",
                "coordinates": [
                    [
                        [
                            -96.38,
                            34.7299999
                        ],
                        [
                            -96.39,
                            34.86
                        ],
                        [
                            -96.09,
                            34.97
                        ],
                        [
                            -96.09,
                            34.769999999999996
                        ],
                        [
                            -96.38,
                            34.7299999
                        ]
                    ]
                ]
            },
            "properties": {
                "@id": "https://api.weather.gov/alerts/urn:oid:2.49.0.1.840.0.1ee7b56ae8604bf07e78a0ea34a4abf0da708c34.001.1",
                "@type": "wx:Alert",
                "id": "urn:oid:2.49.0.1.840.0.1ee7b56ae8604bf07e78a0ea34a4abf0da708c34.001.1",
                "areaDesc": "Coal, OK; Hughes, OK",
                "geocode": {
                    "SAME": [
                        "040029",
                        "040063"
                    ],
                    "UGC": [
                        "OKC029",
                        "OKC063"
                    ]
                },
                "affectedZones": [
                    "https://api.weather.gov/zones/county/OKC029",
                    "https://api.weather.gov/zones/county/OKC063"
                ],
                "references": [],
                "sent": "2025-04-19T23:23:00-05:00",
                "effective": "2025-04-19T23:23:00-05:00",
                "onset": "2025-04-19T23:23:00-05:00",
                "expires": "2025-04-20T00:15:00-05:00",
                "ends": "2025-04-20T00:15:00-05:00",
                "status": "Actual",
                "messageType": "Alert",
                "category": "Met",
                "severity": "Extreme",
                "certainty": "Observed",
                "urgency": "Immediate",
                "event": "Tornado Warning",
                "sender": "w-nws.webmaster@noaa.gov",
                "senderName": "NWS Norman OK",
                "headline": "Tornado Warning issued April 19 at 11:23PM CDT until April 20 at 12:15AM CDT by NWS Norman OK",
                "description": "TOROUN\n\nThe National Weather Service in Norman has issued a\n\n* Tornado Warning for...\nNorth central Coal County in southeastern Oklahoma...\nSoutheastern Hughes County in southeastern Oklahoma...\n\n* Until 1215 AM CDT.\n\n* At 1123 PM CDT, a severe thunderstorm capable of producing a\ntornado was located 6 miles southwest of Gerty, moving northeast at\n20 mph.\n\nHAZARD...Tornado.\n\nSOURCE...Radar indicated rotation.\n\nIMPACT...Flying debris will be dangerous to those caught without\nshelter. Mobile homes will be damaged or destroyed.\nDamage to roofs, windows, and vehicles will occur.  Tree\ndamage is likely.\n\n* Locations impacted include...\nStuart and Gerty.",
                "instruction": "TAKE COVER NOW! Move to a storm shelter, safe room or an interior\nroom on the lowest floor of a sturdy building. Avoid windows. If you\nare outdoors, in a mobile home, or in a vehicle, move to the closest\nsubstantial shelter and protect yourself from flying debris.",
                "response": "Shelter",
                "parameters": {
                    "AWIPSidentifier": [
                        "TOROUN"
                    ],
                    "WMOidentifier": [
                        "WFUS54 KOUN 200423"
                    ],
                    "eventMotionDescription": [
                        "2025-04-20T04:23:00-00:00...storm...226DEG...19KT...34.75,-96.34"
                    ],
                    "maxHailSize": [
                        "0.00"
                    ],
                    "tornadoDetection": [
                        "RADAR INDICATED"
                    ],
                    "BLOCKCHANNEL": [
                        "EAS",
                        "NWEM"
                    ],
                    "EAS-ORG": [
                        "WXR"
                    ],
                    "VTEC": [
                        "/O.NEW.KOUN.TO.W.0033.250420T0423Z-250420T0515Z/"
                    ],
                    "eventEndingTime": [
                        "2025-04-20T05:15:00+00:00"
                    ],
                    "WEAHandling": [
                        "Imminent Threat"
                    ],
                    "CMAMtext": [
                        "NWS: TORNADO WARNING in this area til 12:15 AM CDT. Take shelter now. Check media."
                    ],
                    "CMAMlongtext": [
                        "National Weather Service: TORNADO WARNING in this area until 12:15 AM CDT. Take shelter now in a basement or an interior room on the lowest floor of a sturdy building. If you are outdoors, in a mobile home, or in a vehicle, move to the closest substantial shelter and protect yourself from flying debris. Check media."
                    ]
                }
            }
        },
```
_There is more to this query, but I've snipped it here for the example. See the [entire file here](test%20jsons/2025-04-19%202324%20OK%20alerts.json)._

## Summary
The general idea, therefore, is to combine the reliability of the weather radio alerts with the lack of false alarms by combining both a weather radio and an internet-connected device that queries api.weather.gov. If a radio alert comes in, the device will query api.weather.gov and try to cross-reference the alert. If successfully cross-referenced on weather.gov, it will use the lat/long coordinates to determine whether to sound the audible alarm or not. If the alert cannot be cross-referenced, due to loss of internet or NWS servers going down, it will sound the audible alarm based on the county-level SAME header like a normal weather radio.

## Dependancies
* libcurl4-openssl-dev
* nlohmann/json.hpp

## Building
```
g++ -std=c++20 fetch_alerts.cpp -o fetch_alerts -lcurl
```
