/**
 * @file gpio_sensor.h
 * @brief Implementation of GPIO-based sensors
 * 
 * This file provides classes for interfacing with sensors
 * that communicate via GPIO (General Purpose Input/Output) pins.
 */

#ifndef GPIO_SENSOR_H
#define GPIO_SENSOR_H

#include "sensor_base.h"
#include <vector>

namespace Sensors {

/**
 * @brief GPIO direction options
 */
enum class GPIODirection {
    INPUT,
    OUTPUT
};

/**
 * @brief GPIO edge detection options
 */
enum class GPIOEdge {
    NONE,
    RISING,
    FALLING,
    BOTH
};

/**
 * @brief GPIO pull resistor options
 */
enum class GPIOPull {
    NONE,
    UP,
    DOWN
};

/**
 * @brief Class for GPIO pin abstraction
 */
class GPIOPin {
public:
    /**
     * @brief Constructor
     * 
     * @param pinNumber GPIO pin number
     * @param direction Pin direction (input/output)
     * @param edge Edge detection setting
     * @param pull Pull resistor configuration
     */
    GPIOPin(uint8_t pinNumber, 
            GPIODirection direction = GPIODirection::INPUT,
            GPIOEdge edge = GPIOEdge::NONE,
            GPIOPull pull = GPIOPull::NONE);
    
    /**
     * @brief Destructor
     */
    ~GPIOPin();
    
    /**
     * @brief Initialize the GPIO pin
     * 
     * @return true if successful, false otherwise
     */
    bool initialize();
    
    /**
     * @brief Set the pin direction
     * 
     * @param direction Direction (INPUT/OUTPUT)
     * @return true if successful, false otherwise
     */
    bool setDirection(GPIODirection direction);
    
    /**
     * @brief Set edge detection
     * 
     * @param edge Edge detection setting
     * @return true if successful, false otherwise
     */
    bool setEdge(GPIOEdge edge);
    
    /**
     * @brief Set pull resistor configuration
     * 
     * @param pull Pull resistor setting
     * @return true if successful, false otherwise
     */
    bool setPull(GPIOPull pull);
    
    /**
     * @brief Set the output value
     * 
     * @param value Value to set (true=HIGH, false=LOW)
     * @return true if successful, false otherwise
     */
    bool setValue(bool value);
    
    /**
     * @brief Get the input value
     * 
     * @return true if HIGH, false if LOW
     */
    bool getValue();
    
    /**
     * @brief Wait for an edge event
     * 
     * @param timeoutMs Timeout in milliseconds
     * @return true if edge detected, false on timeout or error
     */
    bool waitForEdge(uint32_t timeoutMs = 1000);
    
    /**
     * @brief Get the pin number
     * 
     * @return Pin number
     */
    uint8_t getPinNumber() const;

private:
    uint8_t mPinNumber;       ///< GPIO pin number
    GPIODirection mDirection; ///< Pin direction
    GPIOEdge mEdge;           ///< Edge detection setting
    GPIOPull mPull;           ///< Pull resistor configuration
    int mValueFd;             ///< File descriptor for value file
    int mEdgeFd;              ///< File descriptor for edge detection
    
    /**
     * @brief Export GPIO pin
     * 
     * @return true if successful, false otherwise
     */
    bool exportPin();
    
    /**
     * @brief Unexport GPIO pin
     */
    void unexportPin();
    
    /**
     * @brief Write to GPIO system file
     * 
     * @param filename File to write to
     * @param value Value to write
     * @return true if successful, false otherwise
     */
    bool writeToFile(const std::string& filename, const std::string& value);
    
    /**
     * @brief Read from GPIO system file
     * 
     * @param filename File to read from
     * @param value String to store the read value
     * @return true if successful, false otherwise
     */
    bool readFromFile(const std::string& filename, std::string& value);
};

/**
 * @brief Class for GPIO-based sensor interface
 */
class GPIOSensor : public SensorBase {
public:
    /**
     * @brief Constructor for GPIO sensor
     * 
     * @param id Unique sensor identifier
     * @param name Human-readable name of the sensor
     * @param pins Vector of pin numbers to use
     */
    GPIOSensor(uint8_t id, 
               const std::string& name,
               const std::vector<uint8_t>& pins);
    
    /**
     * @brief Destructor
     */
    virtual ~GPIOSensor();
    
    /**
     * @brief Initialize the GPIO sensor
     * 
     * @return true if initialization successful, false otherwise
     */
    bool initialize() override;
    
    /**
     * @brief Read data from the GPIO sensor
     * 
     * @return SensorReading object containing the measured values
     */
    SensorReading read() override;
    
    /**
     * @brief Calibrate the GPIO sensor
     * 
     * @return true if calibration successful, false otherwise
     */
    bool calibrate() override;
    
    /**
     * @brief Put the GPIO sensor in low-power mode
     * 
     * @return true if successful, false otherwise
     */
    bool sleep() override;
    
    /**
     * @brief Wake up the GPIO sensor from low-power mode
     * 
     * @return true if successful, false otherwise
     */
    bool wakeUp() override;
    
    /**
     * @brief Self-test the GPIO sensor
     * 
     * @return true if self-test passed, false otherwise
     */
    bool selfTest() override;
    
    /**
     * @brief Configure a specific GPIO pin
     * 
     * @param index Pin index in the pins vector
     * @param direction Pin direction
     * @param edge Edge detection setting
     * @param pull Pull resistor configuration
     * @return true if successful, false otherwise
     */
    bool configurePin(size_t index, 
                     GPIODirection direction,
                     GPIOEdge edge = GPIOEdge::NONE,
                     GPIOPull pull = GPIOPull::NONE);
    
    /**
     * @brief Set value of an output pin
     * 
     * @param index Pin index in the pins vector
     * @param value Value to set
     * @return true if successful, false otherwise
     */
    bool setPinValue(size_t index, bool value);
    
    /**
     * @brief Get value of an input pin
     * 
     * @param index Pin index in the pins vector
     * @return true if HIGH, false if LOW
     */
    bool getPinValue(size_t index);

protected:
    std::vector<std::unique_ptr<GPIOPin>> mPins; ///< Vector of GPIO pin objects
};

} // namespace Sensors

#endif // GPIO_SENSOR_H
