#include "pwrmonclient.h"

#include <QBluetoothUuid>
#include <QString>

#include <string>
#include <sstream>

std::string PwrMonClient::PackData( std::string input, QBluetoothUuid uuid)
{
    std::stringstream parsed;

         if (uuid == QBluetoothUuid(QString("ad54a8ca-e96b-43e1-b057-d17d75078c90")))
    { // For reciever characteristic of the monitor
        char flags = 0;
        if (input.find("sleep") != std::string::npos)
            flags += 0x08;
        if (input.find("fetch") != std::string::npos)
            flags += 0x04;
        if (input.find("relay_toggle") != std::string::npos)
            flags += 0x01;
        parsed << flags;
    }

    return parsed.rdbuf()->in_avail() != 0 ? parsed.str(): "0"; // return 0 if nothing is returned
}
