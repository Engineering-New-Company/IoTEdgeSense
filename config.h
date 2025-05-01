/**
 * @file config.h
 * @brief Global configuration settings for the IoT firmware
 * 
 * This file contains all configurable parameters for the firmware,
 * allowing for easy customization without modifying core code.
 */

#ifndef CONFIG_H
#define CONFIG_H

#include <cstdint>
#include <string>

/**
 * @brief Device configuration
 */
namespace DeviceConfig {
    // Device identification
    constexpr char DEVICE_ID[] = "IOT_EDGE_DEVICE_001";
    constexpr char DEVICE_NAME[] = "IoT Edge Sensor Node";
    constexpr char FIRMWARE_VERSION[] = "1.0.0";
    
    // Network configuration
    constexpr bool USE_WIFI = true;
    constexpr bool USE_ETHERNET = false;
    constexpr bool USE_CELLULAR = false;
    constexpr char WIFI_SSID[] = ""; // To be loaded from secure storage
    constexpr char WIFI_PASSWORD[] = ""; // To be loaded from secure storage
    
    // Security
    constexpr bool ENABLE_TLS = true;
    constexpr char TLS_CA_CERT_PATH[] = "/certs/ca.crt";
    constexpr char TLS_CLIENT_CERT_PATH[] = "/certs/client.crt";
    constexpr char TLS_CLIENT_KEY_PATH[] = "/certs/client.key";
    
    // Communication configuration
    constexpr bool USE_MQTT = true;
    constexpr bool USE_COAP = false;
    constexpr char MQTT_BROKER[] = "mqtt.example.com";
    constexpr uint16_t MQTT_PORT = 8883; // TLS port
    constexpr char MQTT_CLIENT_ID[] = "IOT_EDGE_DEVICE_001";
    constexpr char MQTT_USERNAME[] = ""; // To be loaded from secure storage
    constexpr char MQTT_PASSWORD[] = ""; // To be loaded from secure storage
    constexpr char MQTT_TOPIC_TELEMETRY[] = "devices/data";
    constexpr char MQTT_TOPIC_COMMANDS[] = "devices/commands";
    constexpr char MQTT_TOPIC_STATUS[] = "devices/status";
    
    // Data processing
    constexpr uint32_t DEFAULT_SAMPLING_RATE_MS = 1000;
    constexpr uint16_t DEFAULT_BUFFER_SIZE = 64;
    constexpr uint16_t DATA_BATCH_SIZE = 10;
    constexpr bool ENABLE_LOCAL_STORAGE = true;
    constexpr char LOCAL_STORAGE_PATH[] = "/data/";
    
    // Power management
    constexpr bool ENABLE_LOW_POWER_MODE = true;
    constexpr uint32_t SLEEP_DURATION_MS = 10000;
    constexpr uint32_t ACTIVE_DURATION_MS = 5000;
    constexpr float BATTERY_LOW_THRESHOLD_V = 3.3f;
    constexpr float BATTERY_CRITICAL_THRESHOLD_V = 3.0f;
    
    // System
    constexpr uint32_t WATCHDOG_TIMEOUT_MS = 60000;
    constexpr bool ENABLE_ERROR_REPORTING = true;
    constexpr uint8_t MAX_RETRY_COUNT = 3;
    constexpr uint16_t ERROR_LOG_SIZE = 50;
    
    // Logging
    enum class LogLevel {
        DEBUG,
        INFO,
        WARNING,
        ERROR,
        CRITICAL
    };
    
    constexpr LogLevel CURRENT_LOG_LEVEL = LogLevel::INFO;
    constexpr bool LOG_TO_SERIAL = true;
    constexpr bool LOG_TO_FILE = true;
    constexpr char LOG_FILE_PATH[] = "/logs/device.log";
    constexpr uint32_t MAX_LOG_FILE_SIZE_KB = 1024;
}

#endif // CONFIG_H
