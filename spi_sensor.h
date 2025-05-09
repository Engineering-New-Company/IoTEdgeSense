/**
 * @file spi_sensor.h
 * @brief Implementation of SPI-based sensors
 * 
 * This file provides classes for interfacing with sensors
 * that communicate via the SPI (Serial Peripheral Interface) bus.
 */

#ifndef SPI_SENSOR_H
#define SPI_SENSOR_H

#include "sensor_base.h"

namespace Sensors {

/**
 * @brief SPI operating modes
 */
enum class SPIMode {
    MODE0 = 0, ///< CPOL=0, CPHA=0
    MODE1 = 1, ///< CPOL=0, CPHA=1
    MODE2 = 2, ///< CPOL=1, CPHA=0
    MODE3 = 3  ///< CPOL=1, CPHA=1
};

/**
 * @brief Class for SPI sensor interface
 */
class SPISensor : public SensorBase {
public:
    /**
     * @brief Constructor for SPI sensor
     * 
     * @param id Unique sensor identifier
     * @param name Human-readable name of the sensor
     * @param spiBus SPI bus number (e.g., 0, 1)
     * @param chipSelect Chip select pin number
     * @param mode SPI mode (0-3)
     * @param speedHz SPI clock frequency in Hz
     */
    SPISensor(uint8_t id, 
              const std::string& name, 
              uint8_t spiBus, 
              uint8_t chipSelect, 
              SPIMode mode = SPIMode::MODE0, 
              uint32_t speedHz = 1000000);
    
    /**
     * @brief Destructor
     */
    virtual ~SPISensor();
    
    /**
     * @brief Initialize the SPI sensor
     * 
     * @return true if initialization successful, false otherwise
     */
    bool initialize() override;
    
    /**
     * @brief Read data from the SPI sensor
     * 
     * @return SensorReading object containing the measured values
     */
    SensorReading read() override;
    
    /**
     * @brief Calibrate the SPI sensor
     * 
     * @return true if calibration successful, false otherwise
     */
    bool calibrate() override;
    
    /**
     * @brief Put the SPI sensor in low-power mode
     * 
     * @return true if successful, false otherwise
     */
    bool sleep() override;
    
    /**
     * @brief Wake up the SPI sensor from low-power mode
     * 
     * @return true if successful, false otherwise
     */
    bool wakeUp() override;
    
    /**
     * @brief Self-test the SPI sensor
     * 
     * @return true if self-test passed, false otherwise
     */
    bool selfTest() override;
    
    /**
     * @brief Transfer data over SPI
     * 
     * @param txData Data to transmit
     * @param rxData Buffer to store received data
     * @param length Number of bytes to transfer
     * @return true if successful, false otherwise
     */
    bool transfer(const uint8_t* txData, uint8_t* rxData, size_t length);
    
    /**
     * @brief Write command and read response
     * 
     * @param command Command byte
     * @param response Buffer to store response
     * @param responseLength Expected response length
     * @return true if successful, false otherwise
     */
    bool commandResponse(uint8_t command, uint8_t* response, size_t responseLength);

protected:
    int mSPIFileDescriptor;   ///< File descriptor for SPI bus
    uint8_t mSPIBus;          ///< SPI bus number
    uint8_t mChipSelect;      ///< Chip select pin number
    SPIMode mMode;            ///< SPI mode
    uint32_t mSpeedHz;        ///< SPI clock frequency in Hz
    
    /**
     * @brief Open the SPI bus
     * 
     * @return true if successful, false otherwise
     */
    bool openSPIBus();
    
    /**
     * @brief Close the SPI bus
     */
    void closeSPIBus();
    
    /**
     * @brief Set chip select line
     * 
     * @param state true to activate (low), false to deactivate (high)
     */
    void setChipSelect(bool state);
};

} // namespace Sensors

#endif // SPI_SENSOR_H
