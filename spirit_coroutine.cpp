// Demonstrating the use of ranges built by executing a spirit::qi grammar in a coroutine
#include <string>
#include <iostream>
#include <sstream>

#include <boost/spirit/include/qi.hpp>

#include <range/v3/all.hpp>

#include "nmea.h"

typedef boost::spirit::istream_iterator iter_t;
typedef std::vector<gga_t> parse_result_t;

struct nmea_parser : boost::spirit::qi::grammar<iter_t, 
                                                parse_result_t()>
{
    nmea_parser() : nmea_parser::base_type(sentences)
    {
        using namespace boost::spirit;

        gga_sentence =
            qi::lit("$GPGGA_T") >> ',' >> qi::int_ >> ',' >>
            qi::double_ >> ',' >> qi::char_("NS") >> ',' >>
            qi::double_ >> ',' >> qi::char_("EW") >> ',' >>
            qi::int_ >> ',' >> qi::int_ >> ',' >>
            qi::double_ >> ',' >>
            qi::double_ >> ',' >> qi::char_ >> ',' >>
            qi::double_ >> ',' >> qi::char_ >> ',' >>
            -qi::int_ >> ',' >> -qi::int_ >> ",*" >> qi::int_ >> -qi::eol;

        // define other "sentences" here

        // combine alternates
        sentences = *(gga_sentence   // | bwc_sentence | gll_sentence | ... )
            );

    }

    template<typename R> using Rule = boost::spirit::qi::rule<boost::spirit::istream_iterator, R()>;

    Rule<gga_t> gga_sentence;
    Rule<parse_result_t> sentences;

};

std::ostream& operator<<(std::ostream& os, gga_t const& g) {
    os << '(' << g.latitude << g.lat_hemi << ',' << g.longitude << g.long_hemi << ')';
    return os;
}

int main() {
    std::string test("$GPGGA_T,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,,*47");
    test +=        "\n$GPGGA_T,170834,4124.8963,N,08151.6838,W,1,05,1.5,280.2,M,-34.0,M,,,*59";

    std::istringstream ss(test);
    ss.unsetf(std::ios::skipws);
    parse_result_t result;
    iter_t beg(ss), end;
    nmea_parser nmea;
    if (!boost::spirit::qi::parse(beg, end, nmea, result)) {
        std::cerr << "parse failed!\n";
    } else {
        for( gga_t const & g : result ) {
            std::cout << g << "\n";
        }
    }
}
