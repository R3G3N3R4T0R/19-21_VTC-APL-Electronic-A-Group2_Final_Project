#include "pwrmonclient.h"

#include <QBluetoothAddress>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothDeviceInfo>
#include <QBluetoothLocalDevice>
#include <QBluetoothUuid>
#include <QByteArray>
#include <QLowEnergyController>
#include <QString>

#include <algorithm>
#include <unordered_map>
#include <string>

/* Local Device */
void PwrMonClient::BtSetup()
{
    if (localDevice.isValid())
    {
        localDevice.powerOn();
        localDevice.setHostMode(QBluetoothLocalDevice::HostConnectable);
    }

  //Device Discovery
    deviceDiscoveryAgent = new QBluetoothDeviceDiscoveryAgent(this);

    //connecting Signals for when devies are discovered
    connect(deviceDiscoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered,
            this                , &PwrMonClient::deviceDiscovered );
    //Both finish and stop results in terminating the scan operation so same slot
    connect(deviceDiscoveryAgent, &QBluetoothDeviceDiscoveryAgent::finished,
            this                , &PwrMonClient::deviceScanFinished);
    connect(deviceDiscoveryAgent, &QBluetoothDeviceDiscoveryAgent::canceled,
            this                , &PwrMonClient::deviceScanFinished);
    connect(deviceDiscoveryAgent, QOverload<QBluetoothDeviceDiscoveryAgent::Error>::of(&QBluetoothDeviceDiscoveryAgent::error),
            this                , QOverload<QBluetoothDeviceDiscoveryAgent::Error>::of(&PwrMonClient::errorLog));
} // BtSetup()

void PwrMonClient::Initialize()
{
    DeviceList = nullptr;
    TempDeviceList = nullptr;
    remoteDevice = nullptr;
    deviceController = nullptr;
} // Initialize()

PwrMonClient::PwrMonClient()
{
    Initialize();
    BtSetup();
} // PwrMonClient()

PwrMonClient::PwrMonClient(std::string uuid)
{
    Initialize();
    service_id = QBluetoothUuid(QString(uuid.c_str()));
    BtSetup();
} // PwrMonClient(std::string)

/* Device Discovery */
void PwrMonClient::SetServiceUuid(std::string uuid)
{
    service_id = QBluetoothUuid(QString(uuid.c_str()));
} // SetServiceUuid(std::string)

void PwrMonClient::SetDeviceName(std::string name)
{
    device_name = name;
} // SetDeviceName(std::string)

unsigned char PwrMonClient::Scan(int msec)
{
    deviceDiscoveryAgent->setLowEnergyDiscoveryTimeout(msec);
    deviceDiscoveryAgent->start();

    TempDeviceList = new AsyncList;
    TempDeviceList->cycle = DeviceList ? DeviceList->cycle+1 : 0;

    return DeviceList->cycle;
} // Scan(int)

    //Slots to ScanService
void PwrMonClient::deviceDiscovered(const QBluetoothDeviceInfo &entry)
{
    if(entry.coreConfigurations() == QBluetoothDeviceInfo::LowEnergyCoreConfiguration &&
       entry.serviceUuids().contains(service_id))
    {
        if(!address.empty())
            if(entry.address().toString() != address.c_str())
                return; // do not insert into DeviceList if address are not the same
        if(!device_name.empty())
            if(entry.name().toStdString().find(device_name) == std::string::npos)
                return; // do not return is device_name is not in the name of they entry
        internalDeviceList[entry.address().toString().toStdString()] = entry;
        TempDeviceList->item[entry.name().toStdString()] = entry.address().toString().toStdString();
    }
} // SLOT: deviceDiscovered(const &QBluetoothDeviceInfo)

void PwrMonClient::deviceScanFinished()
{
    delete DeviceList;
    DeviceList = TempDeviceList;
    delete TempDeviceList;
} // SLOT: deviceScanFinished()

/* Remote Device Connection */
void PwrMonClient::SetRemoteDevice(uint32_t cod)
{
    remoteDevice = new QBluetoothDeviceInfo(QBluetoothAddress(QString(address.c_str())),
                                            QString(device_name.c_str()),
                                            cod);
} // SetRemoteDevice(uint32_t)

void PwrMonClient::SetRemoteDevice(std::string address, std::string name, uint32_t cod)
{
    this->address = address;
    device_name = name;
    SetRemoteDevice(cod);
} // SetRemoteDevice(std::string, std::string, uint32_t)

void PwrMonClient::ResetRemoteDevice()
{
    delete remoteDevice;
    remoteDevice = nullptr;
} // ResetRemoteDevice()

char PwrMonClient::Connect(std::string address)
{
    //delete old object if exist
    delete deviceController;
    deviceController= nullptr;
    // Define remoteDevice if not already
    if(DeviceList && !remoteDevice)
        remoteDevice = new QBluetoothDeviceInfo(internalDeviceList[address]);

    if(remoteDevice)
    {
        deviceController = QLowEnergyController::createCentral(*remoteDevice, this);
        //Wiring up Signals
        connect(deviceController, &QLowEnergyController::connected,
                this            , &PwrMonClient::remoteConnected);
/*      connect(deviceController, &QLowEnergyController::disconnected, NOT USED HERE
                this            , &PwrMonClient::remoteDisconnected);
        connect(deviceController, &QLowEnergyController::discoveryFinished,
                this            , &PwrMonClient::remoteDiscovered); */
        connect(deviceController, &QLowEnergyController::stateChanged,
                this            , &PwrMonClient::remoteState);
        //Connect to Device
        deviceController->connectToDevice();
    }
    return remoteDevice ? 1 : 0;
} // Connect(std::string)

char PwrMonClient::Connect()
{
    return address.empty() ? 0 : Connect(address);
} // Connect()

void PwrMonClient::Terminate()
{
    deviceController->disconnectFromDevice();
    deviceController = nullptr;
    remoteDevice = nullptr;
}

/* BLE Service */
    /* SLOTS */
void PwrMonClient::remoteConnected()
{
    deviceController->discoverServices();
    remoteService = deviceController->createServiceObject(service_id, this);
} // SLOT: remoteConnected()
/*  STILL UNUSED DISCONNECT AND DISCOVER FUNCTIONS
void PwrMonClient::remoteDisconnected()
{
    return; // nothing to put here yet
} // SLOT: remoteDisconnected()

void PwrMonClient::remoteDiscovered()
{
    return;// nothing to put here yet
} // SLOT: remoteDiscovered()
*/
void PwrMonClient::remoteState()
{
    switch (deviceController->state())
    {
    case QLowEnergyController::UnconnectedState:
    case QLowEnergyController::ConnectingState:
        return_state = State::unconnected;
        break;
    case QLowEnergyController::ConnectedState:
    case QLowEnergyController::DiscoveringState:
        return_state = State::connected;
        break;
    default :
        return_state = State::connected;
    }
} // SLOT: remoteState()

/* Basic IO */
void PwrMonClient::Read(std::string uuid)
{
    remoteService->readCharacteristic(remoteService->characteristic(QBluetoothUuid(QString(uuid.c_str()))));
} // Read(std::string)

void PwrMonClient::Write(std::string input, std::string uuid)
{
    std::string streamstr = PackData(input, QBluetoothUuid(QString(uuid.c_str())));
    remoteService->writeCharacteristic(remoteService->characteristic(QBluetoothUuid(QString(uuid.c_str()))),
                                       QByteArray(streamstr.c_str()) );
} // Write(std::string)
    /* SLOTS */
void PwrMonClient::returnChange(const QLowEnergyCharacteristic &characteristic, const QByteArray &value)
{
    data.cycle++;
    data.item[characteristic.uuid().toString().toStdString()] = PackData(value.toStdString(), characteristic.uuid());
}

/* Error Logging (Incomplete) */
void PwrMonClient::errorLog(QBluetoothDeviceDiscoveryAgent::Error message)
{
    switch(message)
    {
    case QBluetoothDeviceDiscoveryAgent::NoError:
        break;
    case QBluetoothDeviceDiscoveryAgent::PoweredOffError:
        break;
    default:
        break;
    }
}

PwrMonClient::~PwrMonClient()
{
    delete DeviceList;
    delete TempDeviceList;
    delete remoteDevice;
} // ~PwrMonClient()
