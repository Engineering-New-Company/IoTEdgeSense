/**
 * @file mqtt_client.h
 * @brief MQTT client implementation for IoT communication
 * 
 * This file provides a wrapper around a third-party MQTT library
 * to handle publish/subscribe messaging with an MQTT broker.
 */

#ifndef MQTT_CLIENT_H
#define MQTT_CLIENT_H

#include <string>
#include <functional>
#include <vector>
#include <mutex>
#include "../system/error_handler.h"

// Forward declaration for the MQTT client implementation
// In a real implementation, this would be a concrete type
// from a library like Paho or Mosquitto
struct mqtt_client;

namespace Communication {

/**
 * @brief QoS (Quality of Service) levels for MQTT
 */
enum class MQTTQoS {
    AT_MOST_ONCE = 0,  ///< Fire and forget (QoS 0)
    AT_LEAST_ONCE = 1, ///< Acknowledged delivery (QoS 1)
    EXACTLY_ONCE = 2   ///< Assured delivery (QoS 2)
};

/**
 * @brief MQTT connection states
 */
enum class MQTTConnectionState {
    DISCONNECTED,
    CONNECTING,
    CONNECTED,
    DISCONNECTING,
    CONNECTION_FAILED,
    CONNECTION_LOST
};

/**
 * @brief MQTT message callback type
 */
using MQTTMessageCallback = std::function<void(const std::string&, const std::string&)>;

/**
 * @brief MQTT client class
 */
class MQTTClient {
public:
    /**
     * @brief Constructor
     * 
     * @param clientId Unique client identifier
     * @param broker MQTT broker address
     * @param port MQTT broker port
     * @param username Username for authentication (optional)
     * @param password Password for authentication (optional)
     * @param useTls Whether to use TLS encryption
     */
    MQTTClient(
        const std::string& clientId,
        const std::string& broker,
        uint16_t port = 1883,
        const std::string& username = "",
        const std::string& password = "",
        bool useTls = false
    );
    
    /**
     * @brief Destructor
     */
    ~MQTTClient();
    
    /**
     * @brief Initialize the MQTT client
     * 
     * @return true if initialization successful, false otherwise
     */
    bool initialize();
    
    /**
     * @brief Connect to the MQTT broker
     * 
     * @param cleanSession Whether to start a clean session
     * @param keepAliveInterval Keep-alive interval in seconds
     * @return true if connection successful, false otherwise
     */
    bool connect(bool cleanSession = true, uint16_t keepAliveInterval = 60);
    
    /**
     * @brief Disconnect from the MQTT broker
     * 
     * @return true if disconnection successful, false otherwise
     */
    bool disconnect();
    
    /**
     * @brief Publish a message to a topic
     * 
     * @param topic Topic to publish to
     * @param payload Message payload
     * @param qos Quality of Service level
     * @param retain Whether the message should be retained
     * @return true if publish successful, false otherwise
     */
    bool publish(
        const std::string& topic,
        const std::string& payload,
        MQTTQoS qos = MQTTQoS::AT_LEAST_ONCE,
        bool retain = false
    );
    
    /**
     * @brief Subscribe to a topic
     * 
     * @param topic Topic to subscribe to
     * @param qos Quality of Service level
     * @return true if subscription successful, false otherwise
     */
    bool subscribe(
        const std::string& topic,
        MQTTQoS qos = MQTTQoS::AT_LEAST_ONCE
    );
    
    /**
     * @brief Unsubscribe from a topic
     * 
     * @param topic Topic to unsubscribe from
     * @return true if unsubscription successful, false otherwise
     */
    bool unsubscribe(const std::string& topic);
    
    /**
     * @brief Set callback for message reception
     * 
     * @param callback Function to call when message is received
     */
    void setMessageCallback(MQTTMessageCallback callback);
    
    /**
     * @brief Check if client is connected
     * 
     * @return true if connected, false otherwise
     */
    bool isConnected() const;
    
    /**
     * @brief Get current connection state
     * 
     * @return Current connection state
     */
    MQTTConnectionState getConnectionState() const;
    
    /**
     * @brief Get last error code
     * 
     * @return Last error code
     */
    System::ErrorCode getLastError() const;
    
    /**
     * @brief Set TLS/SSL certificate files
     * 
     * @param caCert CA certificate file path
     * @param clientCert Client certificate file path
     * @param privateKey Private key file path
     * @return true if successful, false otherwise
     */
    bool setTLSCertificates(
        const std::string& caCert,
        const std::string& clientCert = "",
        const std::string& privateKey = ""
    );

private:
    std::string mClientId;
    std::string mBroker;
    uint16_t mPort;
    std::string mUsername;
    std::string mPassword;
    bool mUseTls;
    mqtt_client* mClient;
    MQTTConnectionState mConnectionState;
    System::ErrorCode mLastError;
    MQTTMessageCallback mMessageCallback;
    std::string mCaCert;
    std::string mClientCert;
    std::string mPrivateKey;
    std::mutex mMutex;
    
    /**
     * @brief Process incoming messages
     * 
     * @param topic Message topic
     * @param payload Message payload
     */
    void onMessageReceived(const std::string& topic, const std::string& payload);
    
    /**
     * @brief Handle connection state changes
     * 
     * @param state New connection state
     */
    void onConnectionStateChanged(MQTTConnectionState state);
    
    /**
     * @brief Set the last error code
     * 
     * @param error Error code
     */
    void setLastError(System::ErrorCode error);
};

} // namespace Communication

#endif // MQTT_CLIENT_H
