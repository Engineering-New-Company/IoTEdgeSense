/**
 * @file sensor_base.h
 * @brief Base class for all sensor implementations
 * 
 * This abstract class defines the interface that all sensor
 * implementations must adhere to, allowing for uniform sensor
 * management regardless of the underlying sensor type.
 */

#ifndef SENSOR_BASE_H
#define SENSOR_BASE_H

#include <cstdint>
#include <string>
#include <vector>
#include <memory>
#include "../config.h"
#include "../system/error_handler.h"

namespace Sensors {

/**
 * @brief Enum representing different sensor states
 */
enum class SensorState {
    UNINITIALIZED,
    INITIALIZED,
    RUNNING,
    SLEEPING,
    ERROR
};

/**
 * @brief Structure for sensor readings
 */
struct SensorReading {
    uint64_t timestamp;         ///< Timestamp of the reading in milliseconds
    std::vector<float> values;  ///< Measured values 
    std::string unit;           ///< Unit of measurement
    uint8_t sensorId;           ///< Unique identifier for the sensor
    bool valid;                 ///< Flag indicating if the reading is valid
    
    SensorReading() : timestamp(0), values(), unit(""), sensorId(0), valid(false) {}
    
    SensorReading(uint64_t ts, const std::vector<float>& vals, const std::string& u, uint8_t id, bool v = true)
        : timestamp(ts), values(vals), unit(u), sensorId(id), valid(v) {}
};

/**
 * @brief Abstract base class for all sensor types
 */
class SensorBase {
public:
    /**
     * @brief Constructor
     * 
     * @param id Unique sensor identifier
     * @param name Human-readable name of the sensor
     */
    SensorBase(uint8_t id, const std::string& name);
    
    /**
     * @brief Virtual destructor
     */
    virtual ~SensorBase();
    
    /**
     * @brief Initialize the sensor
     * 
     * @return true if initialization successful, false otherwise
     */
    virtual bool initialize() = 0;
    
    /**
     * @brief Read data from the sensor
     * 
     * @return SensorReading object containing the measured values
     */
    virtual SensorReading read() = 0;
    
    /**
     * @brief Calibrate the sensor
     * 
     * @return true if calibration successful, false otherwise
     */
    virtual bool calibrate() = 0;
    
    /**
     * @brief Put the sensor in low-power mode
     * 
     * @return true if successful, false otherwise
     */
    virtual bool sleep() = 0;
    
    /**
     * @brief Wake up the sensor from low-power mode
     * 
     * @return true if successful, false otherwise
     */
    virtual bool wakeUp() = 0;
    
    /**
     * @brief Self-test the sensor
     * 
     * @return true if self-test passed, false otherwise
     */
    virtual bool selfTest() = 0;
    
    /**
     * @brief Set the sampling rate of the sensor
     * 
     * @param rateMs Sampling rate in milliseconds
     * @return true if successful, false otherwise
     */
    virtual bool setSamplingRate(uint32_t rateMs);
    
    /**
     * @brief Get the current sensor state
     * 
     * @return Current state of the sensor
     */
    SensorState getState() const;
    
    /**
     * @brief Get the sensor ID
     * 
     * @return Sensor ID
     */
    uint8_t getId() const;
    
    /**
     * @brief Get the sensor name
     * 
     * @return Sensor name
     */
    std::string getName() const;
    
    /**
     * @brief Get the last error that occurred
     * 
     * @return Last error code
     */
    System::ErrorCode getLastError() const;
    
    /**
     * @brief Check if the sensor is valid and operational
     * 
     * @return true if the sensor is operational, false otherwise
     */
    bool isValid() const;

protected:
    uint8_t mId;                     ///< Unique sensor identifier
    std::string mName;               ///< Human-readable name
    SensorState mState;              ///< Current state of the sensor
    uint32_t mSamplingRateMs;        ///< Sampling rate in milliseconds
    System::ErrorCode mLastError;    ///< Last error that occurred
    bool mIsValid;                   ///< Indicates if the sensor is operational
    
    /**
     * @brief Set the sensor state
     * 
     * @param state New state
     */
    void setState(SensorState state);
    
    /**
     * @brief Set the last error
     * 
     * @param error Error code
     */
    void setLastError(System::ErrorCode error);
};

} // namespace Sensors

#endif // SENSOR_BASE_H
