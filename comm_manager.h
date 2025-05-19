/**
 * @file comm_manager.h
 * @brief Communication manager for IoT device
 * 
 * This file provides a manager class that handles all
 * communication protocols and ensures secure, reliable 
 * data exchange with the platform layer.
 */

#ifndef COMM_MANAGER_H
#define COMM_MANAGER_H

#include <string>
#include <vector>
#include <memory>
#include <cstdint>
#include <functional>
#include "../config.h"
#include "../sensors/sensor_base.h"
#include "../system/error_handler.h"
#include "mqtt_client.h"
#include "coap_client.h"

namespace Communication {

/**
 * @brief Data transmission status
 */
enum class TransmissionStatus {
    SUCCESS,
    NETWORK_ERROR,
    AUTHENTICATION_ERROR,
    TIMEOUT,
    DATA_ERROR,
    UNKNOWN_ERROR
};

/**
 * @brief Message priority levels
 */
enum class MessagePriority {
    LOW,
    NORMAL,
    HIGH,
    CRITICAL
};

/**
 * @brief Data message structure
 */
struct DataMessage {
    std::string deviceId;
    uint64_t timestamp;
    std::vector<Sensors::SensorReading> readings;
    MessagePriority priority;
    bool encrypted;
    
    DataMessage() 
        : deviceId(DeviceConfig::DEVICE_ID), 
          timestamp(0), 
          readings(), 
          priority(MessagePriority::NORMAL),
          encrypted(DeviceConfig::ENABLE_TLS) {}
};

/**
 * @brief Command callback function type
 */
using CommandCallback = std::function<void(const std::string&, const std::string&)>;

/**
 * @brief Communication manager class
 */
class CommManager {
public:
    /**
     * @brief Constructor
     */
    CommManager();
    
    /**
     * @brief Destructor
     */
    ~CommManager();
    
    /**
     * @brief Initialize the communication manager
     * 
     * @return true if initialization successful, false otherwise
     */
    bool initialize();
    
    /**
     * @brief Connect to the backend platform
     * 
     * @return true if connection successful, false otherwise
     */
    bool connect();
    
    /**
     * @brief Disconnect from the backend platform
     * 
     * @return true if disconnection successful, false otherwise
     */
    bool disconnect();
    
    /**
     * @brief Send sensor readings to the platform
     * 
     * @param readings Vector of sensor readings
     * @param priority Message priority
     * @return Transmission status
     */
    TransmissionStatus sendSensorData(
        const std::vector<Sensors::SensorReading>& readings,
        MessagePriority priority = MessagePriority::NORMAL);
    
    /**
     * @brief Send device status information
     * 
     * @param status Status information as JSON string
     * @return Transmission status
     */
    TransmissionStatus sendStatusUpdate(const std::string& status);
    
    /**
     * @brief Send error report
     * 
     * @param errorCode Error code
     * @param message Error message
     * @return Transmission status
     */
    TransmissionStatus sendErrorReport(System::ErrorCode errorCode, const std::string& message);
    
    /**
     * @brief Register callback for incoming commands
     * 
     * @param callback Function to call when command is received
     */
    void registerCommandCallback(CommandCallback callback);
    
    /**
     * @brief Check connection status
     * 
     * @return true if connected, false otherwise
     */
    bool isConnected() const;
    
    /**
     * @brief Get last error code
     * 
     * @return Last error code
     */
    System::ErrorCode getLastError() const;

private:
    bool mInitialized;
    bool mConnected;
    System::ErrorCode mLastError;
    CommandCallback mCommandCallback;
    
    std::unique_ptr<MQTTClient> mMqttClient;
    std::unique_ptr<CoAPClient> mCoapClient;
    
    /**
     * @brief Internal command handler
     * 
     * @param topic Command topic
     * @param payload Command payload
     */
    void handleCommand(const std::string& topic, const std::string& payload);
    
    /**
     * @brief Convert sensor data to JSON format
     * 
     * @param readings Sensor readings
     * @return JSON string
     */
    std::string sensorDataToJson(const std::vector<Sensors::SensorReading>& readings);
    
    /**
     * @brief Encrypt data using configured method
     * 
     * @param data Data to encrypt
     * @return Encrypted data
     */
    std::string encryptData(const std::string& data);
    
    /**
     * @brief Set the last error code
     * 
     * @param error Error code
     */
    void setLastError(System::ErrorCode error);
    
    /**
     * @brief Initialize TLS/SSL configuration
     * 
     * @return true if successful, false otherwise
     */
    bool initializeTLS();
};

} // namespace Communication

#endif // COMM_MANAGER_H
