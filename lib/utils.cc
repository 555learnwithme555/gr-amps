#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <itpp/comm/bch.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include "utils.h"

using namespace itpp;
using std::string;

namespace gr {
    namespace amps {

        /**
         * Decode a Manchester-encoded byte buffer (unsigned char buf of 0x0 
         * and 0x1 values) into another byte buffer.
         *
         * The length specified is the size of the destination buffer.
         * Returns the number of invalid bits.
         */
        size_t
        manchester_decode_binbuf(const unsigned char *srcbuf, unsigned char *dstbuf, size_t dstbufsz) {
            const size_t srcbufsz = dstbufsz * 2;
            size_t badbits = 0;
            assert(srcbufsz > dstbufsz);
            size_t o = 0;
            for(size_t i = 0; i < srcbufsz; i += 2) {
                const unsigned short sval = ((srcbuf[i] & 0xff) << 8) | (srcbuf[i+1] & 0xff);
                bool outbit;
                switch(sval) {
                    case 0x101:
                        outbit = 0;
                        badbits++;
                        break;
                    case 0x000:
                        outbit = 1;
                        badbits++;
                        break;
                    case 0x100:
                        outbit = 0;
                        break;
                    case 0x001:
                        outbit = 1;
                        break;
                    default:
                        assert(0);
                        break;
                }
                dstbuf[o] = outbit;
                o++;
            }
            return badbits;
        }

        std::vector<char>
        string_to_cvec(std::string binstr) {
            std::vector<char> outvec;
            for(string::const_iterator it = binstr.begin(); it != binstr.end(); ++it) {
                const char c = *it;
                switch(c) {
                    case '0':
                        outvec.push_back(0);
                        break;
                    case '1':
                        outvec.push_back(1);
                        break;
                    case ' ':
                        break;
                    case 0:
                        break;
                    default:
                        std::cerr << "invalid value: " << (int)c << std::endl;
                        throw std::invalid_argument("invalid character in bit vector string: expected 1, 0, or space");
                }
            }
            return outvec;
        }
        void charv_to_bvec(const std::vector<char> &sv, bvec &bv) {
            assert(bv.size() >= sv.size());
            for(int i = 0; i < sv.size(); i++) {
                assert(sv[i] == 0 || sv[i] == 1);
                bv[i] = sv[i];
            }
        }

        /**
         * Using MSB-first order, convert the specified number of bits from 
         * the given unsigned numeric value into a 1-byte-per-bit array.
         *
         * For example, when nbits=3 and val=6, this will set:
         *     outbuf[0] = 1
         *     outbuf[1] = 1
         *     outbuf[2] = 0
         */
        void expandbits(unsigned char *outbuf, size_t nbits, u_int64_t val) {
            assert(nbits <= 64);
            while(nbits > 0) {
                nbits--;
                outbuf[nbits] = ((val & 0x1) == 0x1) ? 1 : 0;
                val >>= 1;
            }
        }

		/* NOTE: NOT THREAD SAFE */
		const char * getstamp() {
			static char stampbuf[1024];
			struct timeval tv;
			memset(&tv, 0, sizeof(struct timeval));
			if(gettimeofday(&tv, NULL) == -1) {
				stampbuf[0] = 0;
				return stampbuf;
			}
			struct tm gtm;
			memset(&gtm, 0, sizeof(struct tm));
			if(gmtime_r(&tv.tv_sec, &gtm) == NULL) {
				stampbuf[0] = 0;
				return stampbuf;
			}
			char strfbuf[256];
			if(strftime(strfbuf, sizeof(strfbuf), "%F %T", &gtm) == 0) {
				stampbuf[0] = 0;
				return stampbuf;
			}
			snprintf(stampbuf, sizeof(stampbuf), "%s.%06ld", strfbuf, (long)tv.tv_usec);
			return stampbuf;
		}

	}
}
