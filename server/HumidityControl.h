#pragma once

#include "CppUtils/io/I2cHandle.h"
#include "CppUtils/c_util/Enum.h"
#include <string_view>

#include "CppUtils/c_util/BitArray.h"

#include "Input.h"
#include "DataFormat.h"

class aht10 : private I2cReaderWriter {
public:

    aht10(const HumidityInput& input);

    using I2cReaderWriter::good;

    bool request_read();

    float humidity() const;
    float temperature() const;

    void soft_reset();

private:
    size_t num_retries_;
    float temperature_, humidity_;
};
