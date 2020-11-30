#include "pwrmonclient.h"

#include <QBluetoothUuid>
#include <QString>

#include <string>
#include <sstream>

std::string PwrMonClient::UnpackData( std::string input, QBluetoothUuid uuid)
{
    std::stringstream parsed;

    if (uuid == QBluetoothUuid(QString("eb402e5a-51d6-4280-8b62-441f93fddd70")))
    { // For original Electrical Characteristic of the BLE server
        const int ValueBase = 1023;
        int voltage, current, max_v, max_i = 0;

        std::string values = input.substr(0, input.find("_")); // Separate data values from the base values "%d-%d_%d-%d" "data_max"
        std::string bases = input.substr(input.find("_")+1);

        voltage = static_cast<int>( atoi( values.substr( 0, values.find("-") ).c_str() )); // assign values from "%d-%d" "voltage-current"
        current = static_cast<int>( atoi( values.substr( values.find("-") + 1).c_str() ));

        max_v = static_cast<int>( atoi( bases.substr( 0, bases.find("-") ).c_str() )); // assign max valus from "%d-%d" syntax same as above
        max_i = static_cast<int>( atoi( bases.substr( bases.find("-") + 1).c_str() ));

        float output_v, output_i;
        output_v = static_cast<float>(voltage) * static_cast<float>(max_v/ValueBase);
        output_i = static_cast<float>(current) * static_cast<float>(max_i/ValueBase);

        parsed <<  "V(V):=";
        parsed << output_v;
        parsed << "|I(A):=";
        parsed << output_i;
    }

    else if (uuid == QBluetoothUuid(QString("137ae80c-8287-43b0-ac82-d2c7bc28b390")))
    { // For original Thermals Characteristic of the BLE server
        int systemp, upper, lower = 0;
        const float ValueBase = 1023;

        size_t range_start = input.find("_") + 1;
        systemp = static_cast<int>( atoi( input.substr(range_start-1).c_str() ));

        std::string range_values = input.substr(range_start);
        upper = static_cast<int>( atoi( range_values.substr( 0, range_values.find("-") ).c_str() ));
        lower = static_cast<int>( atoi( range_values.substr( range_values.find("-") + 1).c_str() ));

        float output_temp;
        output_temp = static_cast<float>(systemp)/ValueBase;
        output_temp /= static_cast<float>(upper-lower);
        output_temp += static_cast<float>(lower);

        parsed << "Temp(C):=";
        parsed << output_temp;
    }

    else if (uuid == QBluetoothUuid(QString("ad54a8ca-e96b-43e1-b057-d17d75078c90")))
    { // For original Receiver Characteristic of the BLE server, only check relay state
        const char *flags = input.c_str();
        if (flags[0] & 0x02)
            parsed << "Relay:=ON";
        else
            parsed << "Relay:=OFF";
    }


    return parsed.rdbuf()->in_avail() != 0 ? parsed.str(): "0"; // return 0 if nothing is returned
}
