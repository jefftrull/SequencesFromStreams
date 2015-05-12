// definitions for types involved in implementing the NMEA protocol for GPS devices

#include <iostream>

#include <boost/optional.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

// NMEA "sentence" describing GPS coordinates
// $GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,,*47
struct gga {
    int                  timestamp;
    double               latitude;
    char                 lat_hemi;
    double               longitude;
    char                 long_hemi;
    int                  quality;
    int                  sat_count;
    double               dilution;
    double               altitude;
    char                 alt_units;
    double               sea_level;
    char                 sea_level_units;
    boost::optional<int> time_since_dgps;
    boost::optional<int> dgps_station_id;
    int                  checksum;
};

// declare stream operators
std::istream& operator>>(std::istream&, gga&);
std::ostream& operator<<(std::ostream&, gga const&);

// adapt gga to make it accessible to Fusion
BOOST_FUSION_ADAPT_STRUCT(
    gga,
    (int,    timestamp)
    (double, latitude)
    (char,   lat_hemi)
    (double, longitude)
    (char,   long_hemi)
    (int,    quality)
    (int,    sat_count)
    (double, dilution)
    (double, altitude)
    (char,   alt_units)
    (double, sea_level)
    (char,   sea_level_units)
    (boost::optional<int>, time_since_dgps)
    (boost::optional<int>, dgps_station_id)
    (int,    checksum)
)
    
