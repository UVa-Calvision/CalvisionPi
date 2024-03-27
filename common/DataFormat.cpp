#include "DataFormat.h"

template <DataFormat f>
struct read_to_raw_functor {
    raw_type operator()(const std::string& s) const {
        return static_cast<raw_type>(format_to_type<f>::read(s));
    }
};

std::optional<raw_type> read_to_raw(DataFormat f, const std::string& s) {
    return DataFormatIndexer::dispatch<read_to_raw_functor>(f, s);
}

template <DataFormat f>
struct dump_dataformat_functor {
    void operator()(std::ostream& out, raw_type x) {
        out << format_to_type<f>::convert_raw(x);
    }
};

void dump_dataformat(std::ostream& out, DataFormat f, raw_type x) {
    DataFormatIndexer::dispatch<dump_dataformat_functor>(f, out, x);
}
