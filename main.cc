#include <iostream>
#include <wiringPi.h>
#include <cstring>

#include "sensors.hh"

using std::cout;
using std::endl;
using std::strcmp;

int main (int argc, char** argv)
{
    if (argc < 2) {
        cout << "-1" << endl;
        return 0;
    }
    wiringPiSetup ();

    const int ky_017_pin = 19;
    const int ky_026_pin = 20;
    const int ky_031_pin = 22;
    const int ky_015_pin = 23;

    const char* value_name = argv[1];
    if (strcmp (value_name, "tilt") == 0) {
        cout << poll_tilt (ky_017_pin) << endl;
    } else if (strcmp (value_name, "flame") == 0) {
        cout << poll_flame (ky_026_pin) << endl;
    } else if (strcmp (value_name, "temp") == 0) {
        cout << poll_temperature (ky_015_pin) << endl;
    } else if (strcmp (value_name, "hum") == 0) {
        cout << poll_humidity (ky_015_pin) << endl;
    } else if (strcmp (value_name, "knock") == 0) {
        cout << poll_knock (ky_031_pin) << endl;
    } else {
        cout << "-1" << endl;
    }

    return 0;
}
