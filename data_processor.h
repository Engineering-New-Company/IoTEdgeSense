/**
 * @file data_processor.h
 * @brief Data processing functionality for sensor readings
 * 
 * This file provides classes for processing, filtering, and
 * analyzing sensor data on the device before transmission.
 */

#ifndef DATA_PROCESSOR_H
#define DATA_PROCESSOR_H

#include <vector>
#include <functional>
#include <memory>
#include <string>
#include "../sensors/sensor_base.h"
#include "data_filter.h"

namespace Data {

/**
 * @brief Types of data processing operations
 */
enum class ProcessingType {
    FILTERING,       ///< Remove noise or unwanted data
    AGGREGATION,     ///< Combine multiple readings
    COMPRESSION,     ///< Reduce data size
    TRANSFORMATION,  ///< Convert data format
    ANOMALY_DETECTION ///< Detect anomalies in data
};

/**
 * @brief Result of data processing operation
 */
struct ProcessingResult {
    bool success;               ///< Whether processing was successful
    std::string message;        ///< Message describing the result
    std::vector<Sensors::SensorReading> readings; ///< Processed readings
    
    ProcessingResult() : success(false), message(""), readings() {}
    
    ProcessingResult(bool s, const std::string& msg, const std::vector<Sensors::SensorReading>& r)
        : success(s), message(msg), readings(r) {}
};

/**
 * @brief Data processor class
 */
class DataProcessor {
public:
    /**
     * @brief Constructor
     */
    DataProcessor();
    
    /**
     * @brief Destructor
     */
    ~DataProcessor();
    
    /**
     * @brief Initialize the data processor
     * 
     * @return true if initialization successful, false otherwise
     */
    bool initialize();
    
    /**
     * @brief Process sensor readings
     * 
     * @param readings Vector of sensor readings to process
     * @return Processing result with processed readings
     */
    ProcessingResult process(const std::vector<Sensors::SensorReading>& readings);
    
    /**
     * @brief Add a filter to the processing pipeline
     * 
     * @param filter Filter to add
     */
    void addFilter(std::shared_ptr<DataFilter> filter);
    
    /**
     * @brief Remove a filter from the processing pipeline
     * 
     * @param filterId ID of the filter to remove
     * @return true if filter was removed, false if not found
     */
    bool removeFilter(const std::string& filterId);
    
    /**
     * @brief Clear all filters
     */
    void clearFilters();
    
    /**
     * @brief Aggregate multiple readings into one
     * 
     * @param readings Vector of sensor readings to aggregate
     * @param method Aggregation method (avg, min, max, sum)
     * @return Aggregated sensor reading
     */
    Sensors::SensorReading aggregate(
        const std::vector<Sensors::SensorReading>& readings,
        const std::string& method = "avg"
    );
    
    /**
     * @brief Detect anomalies in sensor readings
     * 
     * @param readings Vector of sensor readings to check
     * @param threshold Threshold for anomaly detection
     * @return Vector of anomalous readings
     */
    std::vector<Sensors::SensorReading> detectAnomalies(
        const std::vector<Sensors::SensorReading>& readings,
        float threshold = 3.0f
    );
    
    /**
     * @brief Compress sensor readings for transmission
     * 
     * @param readings Vector of sensor readings to compress
     * @return Compressed representation as a string
     */
    std::string compress(const std::vector<Sensors::SensorReading>& readings);
    
    /**
     * @brief Decompress data to sensor readings
     * 
     * @param compressedData Compressed data string
     * @return Vector of decompressed sensor readings
     */
    std::vector<Sensors::SensorReading> decompress(const std::string& compressedData);

private:
    std::vector<std::shared_ptr<DataFilter>> mFilters; ///< Processing filters
    bool mInitialized;  ///< Initialization state
    
    /**
     * @brief Apply all filters to the readings
     * 
     * @param readings Readings to filter
     * @return Filtered readings
     */
    std::vector<Sensors::SensorReading> applyFilters(
        const std::vector<Sensors::SensorReading>& readings
    );
    
    /**
     * @brief Calculate statistics on a set of readings
     * 
     * @param readings Readings to analyze
     * @return Map of statistic name to value
     */
    std::map<std::string, float> calculateStatistics(
        const std::vector<Sensors::SensorReading>& readings
    );
};

} // namespace Data

#endif // DATA_PROCESSOR_H
