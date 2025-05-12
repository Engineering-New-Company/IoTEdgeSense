/**
 * @file uart_sensor.h
 * @brief Implementation of UART-based sensors
 * 
 * This file provides classes for interfacing with sensors
 * that communicate via UART (Universal Asynchronous Receiver-Transmitter).
 */

#ifndef UART_SENSOR_H
#define UART_SENSOR_H

#include "sensor_base.h"
#include <termios.h>

namespace Sensors {

/**
 * @brief UART parity options
 */
enum class UARTParity {
    NONE,
    ODD,
    EVEN
};

/**
 * @brief UART stop bits options
 */
enum class UARTStopBits {
    ONE,
    TWO
};

/**
 * @brief Class for UART sensor interface
 */
class UARTSensor : public SensorBase {
public:
    /**
     * @brief Constructor for UART sensor
     * 
     * @param id Unique sensor identifier
     * @param name Human-readable name of the sensor
     * @param port UART port (e.g., "/dev/ttyS0")
     * @param baudRate Baud rate (e.g., 9600, 115200)
     * @param parity Parity setting
     * @param stopBits Stop bits setting
     * @param dataBits Data bits (5-8)
     */
    UARTSensor(uint8_t id, 
               const std::string& name,
               const std::string& port,
               speed_t baudRate = B9600,
               UARTParity parity = UARTParity::NONE,
               UARTStopBits stopBits = UARTStopBits::ONE,
               uint8_t dataBits = 8);
    
    /**
     * @brief Destructor
     */
    virtual ~UARTSensor();
    
    /**
     * @brief Initialize the UART sensor
     * 
     * @return true if initialization successful, false otherwise
     */
    bool initialize() override;
    
    /**
     * @brief Read data from the UART sensor
     * 
     * @return SensorReading object containing the measured values
     */
    SensorReading read() override;
    
    /**
     * @brief Calibrate the UART sensor
     * 
     * @return true if calibration successful, false otherwise
     */
    bool calibrate() override;
    
    /**
     * @brief Put the UART sensor in low-power mode
     * 
     * @return true if successful, false otherwise
     */
    bool sleep() override;
    
    /**
     * @brief Wake up the UART sensor from low-power mode
     * 
     * @return true if successful, false otherwise
     */
    bool wakeUp() override;
    
    /**
     * @brief Self-test the UART sensor
     * 
     * @return true if self-test passed, false otherwise
     */
    bool selfTest() override;
    
    /**
     * @brief Send data over UART
     * 
     * @param data Data buffer to send
     * @param length Number of bytes to send
     * @return true if successful, false otherwise
     */
    bool send(const uint8_t* data, size_t length);
    
    /**
     * @brief Receive data from UART
     * 
     * @param buffer Buffer to store received data
     * @param maxLength Maximum number of bytes to receive
     * @param timeoutMs Timeout in milliseconds
     * @return Number of bytes received, -1 on error
     */
    int receive(uint8_t* buffer, size_t maxLength, uint32_t timeoutMs = 1000);
    
    /**
     * @brief Send command and wait for response
     * 
     * @param command Command string
     * @param response Buffer to store response
     * @param maxResponseLength Maximum response length
     * @param timeoutMs Timeout in milliseconds
     * @return true if successful, false otherwise
     */
    bool sendCommand(const std::string& command, 
                     std::string& response, 
                     size_t maxResponseLength = 256, 
                     uint32_t timeoutMs = 1000);

protected:
    int mUARTFileDescriptor;      ///< File descriptor for UART port
    std::string mPortName;        ///< UART port name
    speed_t mBaudRate;            ///< Baud rate
    UARTParity mParity;           ///< Parity setting
    UARTStopBits mStopBits;       ///< Stop bits setting
    uint8_t mDataBits;            ///< Data bits
    struct termios mOriginalTios; ///< Original terminal settings
    
    /**
     * @brief Open the UART port
     * 
     * @return true if successful, false otherwise
     */
    bool openUARTPort();
    
    /**
     * @brief Close the UART port
     */
    void closeUARTPort();
    
    /**
     * @brief Configure UART port settings
     * 
     * @return true if successful, false otherwise
     */
    bool configurePort();
    
    /**
     * @brief Flush UART buffers
     * 
     * @return true if successful, false otherwise
     */
    bool flushBuffers();
};

} // namespace Sensors

#endif // UART_SENSOR_H
