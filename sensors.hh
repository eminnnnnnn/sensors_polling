#ifndef SENSORS_HH
#define SENSORS_HH

#include <functional>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
#include <wiringPi.h>

/*
 * TODO: Maybe it would be better to read configuration of connected
 * sensors from some file (.conf or .ini) and dynamically initialize
 * containters. Configuration file contains information about sensor
 * name, connected pins, type (single data signal or protocol),
 * data transfer timings (in case of non-standard protocol)
 */

struct sensor_t
{
    std::string name;
    int pin;
    std::function<int(int)> poll_function;
    const char* description;
};


class sensors_map_t : public std::unordered_map<std::string, sensor_t>
{
  public:
    using std::unordered_map<std::string, sensor_t>::unordered_map;

    int poll_sensor (const char* name) const
    {
        auto sensor = find (name);
        if (sensor != end ()) {
            return sensor->second.poll_function (sensor->second.pin);
        }
        return -1;
    }
};

int poll_tilt (int pin);
int poll_flame (int pin);
int poll_knock (int pin);
int poll_humidity (int pin);
int poll_temperature (int pin);

// one data wire sensors
const sensors_map_t sensors {
    { "temp",
      { "ky_015", 23, poll_temperature, "Temperature" } },
    { "hum",
      { "ky_015", 23, poll_humidity,    "Humidity" } },
    { "tilt",
      { "ky_017", 19, poll_tilt,        "Tilt" } },
    { "flame",
      { "ky_026", 20, poll_flame,       "Flame" } },
    { "knock",
      { "ky_031", 22, poll_knock,       "Knock" }
    }
};

// KY-017
int poll_tilt (int pin)
{
    pinMode (pin, INPUT);
    return digitalRead (pin);
}

// KY-026
int poll_flame (int pin)
{
    pinMode (pin, INPUT);
    return digitalRead (pin);
}

using temp_hum_pair = std::pair<int, int>;

// KY-015
temp_hum_pair poll_dht11 (int pin)
{
    // COPYPASTED FROM wiringOP-Python/examples/orangepi-sensors/dht11.py
    // This is the only polling variant that works at all for now...
    // It is difficult to conform to the timings required by DHT11 protocol
    // with Linux on board.
    unsigned int SH = 0, SL = 0, TH = 0, TL = 0, C = 0;
    for (int i = 0; i < 10; ++i) {
    	SH = 0; SL = 0; TH = 0; TL = 0; C = 0;
        std::vector<unsigned int> tl;
        std::vector<int> tb;

        pinMode (pin, OUTPUT);
        digitalWrite (pin, HIGH);
        delay (1);

        digitalWrite (pin, LOW);
        delay (25);

        digitalWrite (pin, HIGH);;
        delayMicroseconds (20);

        pinMode (pin, INPUT);
        while (digitalRead (pin) == 1) {;}

        for (int i = 0; i < 45; ++i) {
            unsigned int tc = micros ();

            while (digitalRead (pin) == 0 ) {;}

            while (digitalRead (pin) == 1) {
                if (micros () - tc > 500)
                    break;
            }

            if (micros () - tc > 500)
                break;

            tl.push_back (micros () - tc);
        }

        for (int i = 1; i < tl.size (); ++i) {
            if (tl[i] > 100)
                tb.push_back (1);
            else
                tb.push_back (0);
        }

        if (tb.size () == 40) {
            for (int k = 0; k < 8; ++k) {
                SH = SH * 2 + tb[k];
                SL = SL * 2 + tb[k + 8];
                TH = TH * 2 + tb[k + 16];
                TL = TL * 2 + tb[k + 24];
                C = C * 2 + tb[k + 32];
            }
            // checksum
            if ((SH + SL + TH + TL) % 256 == C && C != 0) {
                return std::make_pair (SH, TH);
            }
        }
        delay (200);
    }
    return std::make_pair (0, 0);
}

int poll_temperature (int pin)
{
    return (poll_dht11 (pin)).second;
}

int poll_humidity (int pin)
{
    return (poll_dht11 (pin)).first;
}

// KY-031
int poll_knock (int pin)
{
    pinMode (pin, INPUT);
    return digitalRead (pin);
}

#endif