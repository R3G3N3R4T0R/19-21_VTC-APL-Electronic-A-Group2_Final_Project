#ifndef PWRMONCLIENT_H
#define PWRMONCLIENT_H

#include "PwrMonClient_global.h"

#include <QBluetoothAddress>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothDeviceInfo>
#include <QBluetoothLocalDevice>
#include <QBluetoothUuid>
#include <QLowEnergyController>

#include <string>
#include <unordered_map>

//! Class for communicating with custom BLE sensor devices
class PWRMONCLIENT_EXPORT PwrMonClient : public QObject
{
    Q_OBJECT

    //! struct for Asynchronous Communication for a Map
    //! \var AsyncList::item
    //!     the actual item on the list
    //! \var AsyncList::cycle
    //!     current cycle of the list
    typedef struct AsyncList
    {
        std::unordered_map<std::string, std::string> item;
        unsigned char cycle;
    } AsyncList;

public:
    //!< State of the BLE Controller
    enum class State
    {
        unconnected, //!< Not connected to a device
        connected, //!< Connected to device but services not known
        discovered, //!< Services discovered and usable
    };
    State state(); //!< getter for private state

    PwrMonClient(); //!< Default Constructor
    PwrMonClient(std::string uuid); //!< Contruct object with filter
    ~PwrMonClient(void); //!< Dommon Destructor

    AsyncList *DeviceList; //!< AsyncList of Devices returned by ScanService
    std::string *Data; //!< Array of data returned by Read()
// Discovery constructs
    //! Set Service UUID for remote control
    //! \details Define service_uuid private object for filtering devices
    //! \param uuid Service UUID in format specified in QBluetoothUUID class
    void SetServiceUuid(std::string uuid);

    //!< Set the Name ame of remote device
    //! \details Define device_name for filtering devices
    void SetDeviceName(std::string name);

    //! Initiate a scan for devices in vicinity
    //! \details Initiate a filtered scan based on set parameters service_uuid and
    //! \param msec search time in microsecond
    //! \return
    unsigned char Scan(int msec);

    //! Choose the remote device to connecct to manually
    //! \details QBluetoothDeviceInfo must be obtained for connecting to an external
    //! device on non-BlueZ(Linux) Platform. This function input the name and address
    //! previously assaigned and combine with the 24bit Class of Device to construct
    //! a QBluetoothDeviceInfo for connection to the remtoe device. Only useful if
    //! CoD is known in advance and not needed if device is found on Scan().
    //! \param cod Class of Device or Bluetooth Baseband of the remote device as
    //! specified in the Bluetooth Specification
    void SetRemoteDevice(uint32_t cod);
    //! Overload of SetRemoteDevice that also set the address and name of the device
    void SetRemoteDevice(std::string address, std::string name, uint32_t cod);
    //! Delete QBluetoothDeviceInfo of current remote device
    void ResetRemoteDevice(void);
    
    std::string address; //!< Address selected by user
    char Connect(std::string address); //!< Initiate connection with remote device
    char Connect(void); //!< Connect(std::string uuid) with everything defined
    void Terminate(void); //!< Terminate connection with device

    AsyncList data; //!< Characteristic buffer with data stored as first[address], second[data]
    //! Retrieve data from device and return a list with header
    /*!
     * \param uuid The characteristic UUID to retrieve from
     * \details Request for the valus of a characteristic and store it in an
     * AsyncList structure
     */
    void Read(std::string uuid);
    //! Write data to device with custom arguments as input
    /*!
     * \param input A string of arguments to be entered delimited with |
     * \param uuid The characteristic UUID to write to
     * \details Write to device using flags, variables are assaigned using ":="
     * the write operation is done asynchronously and response with incrementation
     * in the cycle value of the characteristic
     */
    void Write(std::string input, std::string uuid);

private:
    State return_state; // Internal state tracking connection behavior
    AsyncList *TempDeviceList; //Temporary DeviceList
    std::unordered_map<std::string, QBluetoothDeviceInfo> internalDeviceList;

    std::string device_name; // device name for filteringg scanned devices
    QBluetoothDeviceInfo *remoteDevice; //device info for its uuid and type
    QBluetoothUuid service_id; // service uuid for this object
    QLowEnergyService *remoteService; //Qt service object

    void BtSetup(); // Standard Bluetooth device startup
    void Initialize(); // Initialize Pointers and default variables

    std::string PackData(std::string input, QBluetoothUuid uuid); // Packing for output to device
    std::string UnpackData(std::string output, QBluetoothUuid uuid); // Unpacking data inpu

    QBluetoothDeviceDiscoveryAgent *deviceDiscoveryAgent; // Qt Class for device discovery
    QLowEnergyController *deviceController; //Qt class for BLE Central device operations
    QBluetoothLocalDevice localDevice; // Qt Class for local BT module functions

public slots:
    // Local Device
    void deviceDiscovered(const QBluetoothDeviceInfo &entry);
    void deviceScanFinished();
    // Remote Device
    void remoteConnected();
    void remoteDisconnected();
    void remoteDiscovered();
    void remoteState();
    // Basic IO
    void returnChange(const QLowEnergyCharacteristic &characteristic, const QByteArray &value);
    // errorLog()
    void errorLog(QBluetoothDeviceDiscoveryAgent::Error message);
    void errorLog(QLowEnergyController::Error message);
};

#endif // PWRMONCLIENT_H
