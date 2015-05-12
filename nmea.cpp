#include <boost/optional/optional_io.hpp>

#include "nmea.h"

// define custom stream operators

// first a helper: a custom manipulator for skipping commas
std::istream& comma(std::istream& is) {
    char ch;
    is >> ch;
    if (ch != ',') {
        is.setstate(std::ios_base::failbit);
    }
    return is;
}

std::istream&
operator>>(std::istream& is, gga& g) {
    using namespace std;

    // define some temporaries
    char ch;
    string str;

    // a failure in one of the operator>> execution will cause all later ones to fail
    // so we don't need to do early exits

    while (!is.fail() && str.length() < 6) {
        is >> ch;
        str += ch;
    }
    if (str != "$GPGGA") {
        is.setstate(ios_base::failbit);
    }
    comma(is);

    is >> g.timestamp >> comma >> g.latitude >> comma;

    is >> g.lat_hemi;
    if ((g.lat_hemi != 'N') && (g.lat_hemi != 'S')) {
        is.setstate(ios_base::failbit);
    }
    comma(is);

    is >> g.longitude >> comma;

    is >> g.long_hemi;
    if ((g.long_hemi != 'E') && (g.long_hemi != 'W')) {
        is.setstate(ios_base::failbit);
    }
    comma(is);

    is >> g.quality >> comma >> g.sat_count >> comma >> g.dilution >> comma >> g.altitude >> comma;
    is >> g.alt_units >> comma >> g.sea_level >> comma >> g.sea_level_units >> comma;

    // handle optional values
    if (is.peek() != ',') {
        is >> g.time_since_dgps;
    }
    comma(is);

    if (is.peek() != ',') {
        is >> g.dgps_station_id;
    }
    comma(is);

    is >> ch;
    if (ch != '*') {
        is.setstate(ios_base::failbit);
    }
    is >> g.checksum;

    return is;
}

std::ostream& operator<<(std::ostream& os, gga const& g) {
    os << '(' << g.latitude << g.lat_hemi << ',' << g.longitude << g.long_hemi << ')';
    return os;
}
