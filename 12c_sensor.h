/**
 * @file i2c_sensor.h
 * @brief Implementation of I2C-based sensors
 * 
 * This file provides classes for interfacing with sensors
 * that communicate via the I2C (Inter-Integrated Circuit) bus.
 */

#ifndef I2C_SENSOR_H
#define I2C_SENSOR_H

#include "sensor_base.h"

namespace Sensors {

/**
 * @brief Class for I2C sensor interface
 */
class I2CSensor : public SensorBase {
public:
    /**
     * @brief Constructor for I2C sensor
     * 
     * @param id Unique sensor identifier
     * @param name Human-readable name of the sensor
     * @param i2cBus I2C bus number (e.g., 0, 1)
     * @param i2cAddress 7-bit I2C device address
     */
    I2CSensor(uint8_t id, const std::string& name, uint8_t i2cBus, uint8_t i2cAddress);
    
    /**
     * @brief Destructor
     */
    virtual ~I2CSensor();
    
    /**
     * @brief Initialize the I2C sensor
     * 
     * @return true if initialization successful, false otherwise
     */
    bool initialize() override;
    
    /**
     * @brief Read data from the I2C sensor
     * 
     * @return SensorReading object containing the measured values
     */
    SensorReading read() override;
    
    /**
     * @brief Calibrate the I2C sensor
     * 
     * @return true if calibration successful, false otherwise
     */
    bool calibrate() override;
    
    /**
     * @brief Put the I2C sensor in low-power mode
     * 
     * @return true if successful, false otherwise
     */
    bool sleep() override;
    
    /**
     * @brief Wake up the I2C sensor from low-power mode
     * 
     * @return true if successful, false otherwise
     */
    bool wakeUp() override;
    
    /**
     * @brief Self-test the I2C sensor
     * 
     * @return true if self-test passed, false otherwise
     */
    bool selfTest() override;
    
    /**
     * @brief Write data to I2C register
     * 
     * @param regAddr Register address
     * @param data Data byte to write
     * @return true if successful, false otherwise
     */
    bool writeRegister(uint8_t regAddr, uint8_t data);
    
    /**
     * @brief Read data from I2C register
     * 
     * @param regAddr Register address
     * @param data Reference to store read data
     * @return true if successful, false otherwise
     */
    bool readRegister(uint8_t regAddr, uint8_t& data);
    
    /**
     * @brief Read multiple bytes from I2C registers
     * 
     * @param regAddr Starting register address
     * @param data Buffer to store read data
     * @param length Number of bytes to read
     * @return true if successful, false otherwise
     */
    bool readRegisters(uint8_t regAddr, uint8_t* data, size_t length);

protected:
    int mI2CFileDescriptor;   ///< File descriptor for I2C bus
    uint8_t mI2CBus;          ///< I2C bus number
    uint8_t mI2CAddress;      ///< 7-bit I2C device address
    
    /**
     * @brief Open the I2C bus
     * 
     * @return true if successful, false otherwise
     */
    bool openI2CBus();
    
    /**
     * @brief Close the I2C bus
     */
    void closeI2CBus();
};

} // namespace Sensors

#endif // I2C_SENSOR_H
