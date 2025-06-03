/**
 * @file data_filter.h
 * @brief Data filtering classes for sensor readings
 * 
 * This file provides filter implementations for cleaning and
 * processing sensor data, including noise reduction, outlier 
 * detection, and more.
 */

#ifndef DATA_FILTER_H
#define DATA_FILTER_H

#include <string>
#include <vector>
#include <deque>
#include "../sensors/sensor_base.h"

namespace Data {

/**
 * @brief Base class for all data filters
 */
class DataFilter {
public:
    /**
     * @brief Constructor
     * 
     * @param id Unique filter identifier
     * @param name Human-readable filter name
     */
    DataFilter(const std::string& id, const std::string& name);
    
    /**
     * @brief Virtual destructor
     */
    virtual ~DataFilter();
    
    /**
     * @brief Apply filter to a set of sensor readings
     * 
     * @param readings Vector of sensor readings to filter
     * @return Filtered vector of sensor readings
     */
    virtual std::vector<Sensors::SensorReading> apply(
        const std::vector<Sensors::SensorReading>& readings) = 0;
    
    /**
     * @brief Get filter ID
     * 
     * @return Filter ID
     */
    std::string getId() const;
    
    /**
     * @brief Get filter name
     * 
     * @return Filter name
     */
    std::string getName() const;
    
    /**
     * @brief Check if filter is enabled
     * 
     * @return true if enabled, false otherwise
     */
    bool isEnabled() const;
    
    /**
     * @brief Enable or disable the filter
     * 
     * @param enabled Whether to enable the filter
     */
    void setEnabled(bool enabled);

protected:
    std::string mId;       ///< Unique filter identifier
    std::string mName;     ///< Human-readable name
    bool mEnabled;         ///< Whether the filter is enabled
};

/**
 * @brief Moving average filter
 * 
 * This filter applies a moving average to smooth out sensor readings
 * and reduce noise.
 */
class MovingAverageFilter : public DataFilter {
public:
    /**
     * @brief Constructor
     * 
     * @param id Unique filter identifier
     * @param windowSize Size of the moving average window
     */
    MovingAverageFilter(const std::string& id, size_t windowSize = 5);
    
    /**
     * @brief Destructor
     */
    ~MovingAverageFilter();
    
    /**
     * @brief Apply moving average filter to sensor readings
     * 
     * @param readings Vector of sensor readings to filter
     * @return Filtered vector of sensor readings
     */
    std::vector<Sensors::SensorReading> apply(
        const std::vector<Sensors::SensorReading>& readings) override;
    
    /**
     * @brief Set window size
     * 
     * @param windowSize New window size
     */
    void setWindowSize(size_t windowSize);
    
    /**
     * @brief Get window size
     * 
     * @return Current window size
     */
    size_t getWindowSize() const;
    
    /**
     * @brief Reset the filter's internal state
     */
    void reset();

private:
    size_t mWindowSize;    ///< Size of the moving average window
    std::map<uint8_t, std::deque<std::vector<float>>> mHistory;  ///< History of values per sensor
    
    /**
     * @brief Calculate moving average for a single reading
     * 
     * @param reading Reading to average
     * @return Averaged reading
     */
    Sensors::SensorReading calculateAverage(const Sensors::SensorReading& reading);
};

/**
 * @brief Threshold filter
 * 
 * This filter removes readings that are outside a specified range.
 */
class ThresholdFilter : public DataFilter {
public:
    /**
     * @brief Constructor
     * 
     * @param id Unique filter identifier
     * @param minThreshold Minimum allowed value
     * @param maxThreshold Maximum allowed value
     */
    ThresholdFilter(
        const std::string& id,
        float minThreshold = -std::numeric_limits<float>::infinity(),
        float maxThreshold = std::numeric_limits<float>::infinity()
    );
    
    /**
     * @brief Destructor
     */
    ~ThresholdFilter();
    
    /**
     * @brief Apply threshold filter to sensor readings
     * 
     * @param readings Vector of sensor readings to filter
     * @return Filtered vector of sensor readings
     */
    std::vector<Sensors::SensorReading> apply(
        const std::vector<Sensors::SensorReading>& readings) override;
    
    /**
     * @brief Set minimum threshold
     * 
     * @param minThreshold New minimum threshold
     */
    void setMinThreshold(float minThreshold);
    
    /**
     * @brief Set maximum threshold
     * 
     * @param maxThreshold New maximum threshold
     */
    void setMaxThreshold(float maxThreshold);
    
    /**
     * @brief Get minimum threshold
     * 
     * @return Current minimum threshold
     */
    float getMinThreshold() const;
    
    /**
     * @brief Get maximum threshold
     * 
     * @return Current maximum threshold
     */
    float getMaxThreshold() const;

private:
    float mMinThreshold;   ///< Minimum allowed value
    float mMaxThreshold;   ///< Maximum allowed value
    
    /**
     * @brief Check if value is within thresholds
     * 
     * @param value Value to check
     * @return true if value is within thresholds, false otherwise
     */
    bool isWithinThresholds(float value) const;
};

/**
 * @brief Delta filter
 * 
 * This filter only passes readings that have changed by more than
 * a specified delta from the previous reading.
 */
class DeltaFilter : public DataFilter {
public:
    /**
     * @brief Constructor
     * 
     * @param id Unique filter identifier
     * @param minDelta Minimum change required to pass filter
     */
    DeltaFilter(const std::string& id, float minDelta = 0.1f);
    
    /**
     * @brief Destructor
     */
    ~DeltaFilter();
    
    /**
     * @brief Apply delta filter to sensor readings
     * 
     * @param readings Vector of sensor readings to filter
     * @return Filtered vector of sensor readings
     */
    std::vector<Sensors::SensorReading> apply(
        const std::vector<Sensors::SensorReading>& readings) override;
    
    /**
     * @brief Set minimum delta
     * 
     * @param minDelta New minimum delta
     */
    void setMinDelta(float minDelta);
    
    /**
     * @brief Get minimum delta
     * 
     * @return Current minimum delta
     */
    float getMinDelta() const;
    
    /**
     * @brief Reset the filter's internal state
     */
    void reset();

private:
    float mMinDelta;   ///< Minimum change required to pass filter
    std::map<uint8_t, std::vector<float>> mLastValues;  ///< Last values per sensor
    
    /**
     * @brief Check if reading has changed by more than minDelta
     * 
     * @param reading Reading to check
     * @return true if reading has changed enough, false otherwise
     */
    bool hasChangedEnough(const Sensors::SensorReading& reading);
};

/**
 * @brief Median filter
 * 
 * This filter applies a median filter to remove outliers and
 * noise from sensor readings.
 */
class MedianFilter : public DataFilter {
public:
    /**
     * @brief Constructor
     * 
     * @param id Unique filter identifier
     * @param windowSize Size of the median filter window (odd number)
     */
    MedianFilter(const std::string& id, size_t windowSize = 5);
    
    /**
     * @brief Destructor
     */
    ~MedianFilter();
    
    /**
     * @brief Apply median filter to sensor readings
     * 
     * @param readings Vector of sensor readings to filter
     * @return Filtered vector of sensor readings
     */
    std::vector<Sensors::SensorReading> apply(
        const std::vector<Sensors::SensorReading>& readings) override;
    
    /**
     * @brief Set window size
     * 
     * @param windowSize New window size (must be odd)
     */
    void setWindowSize(size_t windowSize);
    
    /**
     * @brief Get window size
     * 
     * @return Current window size
     */
    size_t getWindowSize() const;
    
    /**
     * @brief Reset the filter's internal state
     */
    void reset();

private:
    size_t mWindowSize;    ///< Size of the median filter window
    std::map<uint8_t, std::deque<std::vector<float>>> mHistory;  ///< History of values per sensor
    
    /**
     * @brief Calculate median for a single reading
     * 
     * @param reading Reading to median filter
     * @return Median-filtered reading
     */
    Sensors::SensorReading calculateMedian(const Sensors::SensorReading& reading);
    
    /**
     * @brief Calculate median of a vector of floats
     * 
     * @param values Vector of values
     * @return Median value
     */
    float calculateMedianValue(std::vector<float> values);
};

} // namespace Data

#endif // DATA_FILTER_H
