#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/types.h>
#include <gnuradio/io_signature.h>
#include <itpp/comm/bch.h>
#include <iostream>
#include <sstream>
#include <vector>

using boost::shared_ptr;
using itpp::bvec;
using std::string;
using std::vector;
using std::ostringstream;

#define LOG_WARNING(fmt, ...) printf("%s: %s:%d WARNING: " fmt "\n", getstamp(), __FILE__, __LINE__, ##__VA_ARGS__);
#define LOG_DEBUG(fmt, ...) printf("%s: %s:%d DEBUG: " fmt "\n", getstamp(), __FILE__, __LINE__, ##__VA_ARGS__);

namespace gr {
    namespace amps {
        void charv_to_bvec(const std::vector<char> &sv, bvec &bv);
        std::vector<char> string_to_cvec(std::string binstr);
        size_t manchester_decode_binbuf(const unsigned char *srcbuf, unsigned char *dstbuf, size_t dstbufsz);
        void expandbits(unsigned char *outbuf, size_t nbits, u_int64_t val);
		const char * getstamp();
    }
}

