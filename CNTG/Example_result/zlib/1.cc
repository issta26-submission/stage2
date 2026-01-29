// Header
#include <cassert>
#include <cstddef>
#include <cstdio>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>
int test_api_sequence() {
  // step 1: Initialize and declarations
  z_stream istrm;
  memset(&istrm, 0, sizeof(z_stream));
  Bytef src1[] = "Hello, zlib!";
  Bytef src2[] = "Second block.";
  uInt len1 = (uInt)(sizeof(src1) - 1);
  uInt len2 = (uInt)(sizeof(src2) - 1);
  uLong crc1 = crc32(0UL, src1, len1);
  uLong crc2 = crc32(0UL, src2, len2);
  uLong combined_crc = crc32_combine(crc1, crc2, (off_t)len2);
  uLong totalLen = (uLong)(len1 + len2);
  Bytef src_combined[256];
  memset(src_combined, 0, sizeof(src_combined));
  memcpy(src_combined, src1, len1);
  memcpy(src_combined + len1, src2, len2);
  assert(len1 == 12);
  assert(len2 == 13);
  assert(totalLen == (uLong)(len1 + len2));
  assert(istrm.avail_in == 0);
  assert(memcmp(src_combined, src1, len1) == 0);
  assert(memcmp(src_combined + len1, src2, len2) == 0);
  assert(crc1 == crc32(0UL, src1, len1));
  assert(crc2 == crc32(0UL, src2, len2));
  assert(combined_crc == crc32(crc1, src2, len2));
  assert(combined_crc == crc32(0UL, src_combined, totalLen));
  // step 2: Configure and compress the combined source
  uLong bound = compressBound(totalLen);
  assert(bound >= totalLen);
  assert(bound <= 1024); // Ensure the upper bound fits within the fixed buffer
                         // size we are about to declare
  uLongf compLen = (uLongf)bound;
  assert(compLen == bound);
  Bytef compbuf[1024];
  memset(compbuf, 0, sizeof(compbuf));
  assert(compbuf[0] == 0);
  assert(sizeof(compbuf) >=
         bound); // Verify buffer capacity against the calculated bound
  compress2(compbuf, &compLen, src_combined, totalLen, 6);
  assert(compLen > 0);
  assert(compLen <= bound); // The actual compressed size must be within the
                            // calculated upper bound
  assert(compLen <= sizeof(compbuf));
  // Verify source data integrity was maintained (input buffer not corrupted)
  assert(memcmp(src_combined, src1, len1) == 0);
  assert(memcmp(src_combined + len1, src2, len2) == 0);
  // step 3: Initialize inflate, perform inflate, query codes used
  istrm.next_in = compbuf;
  istrm.avail_in = (uInt)compLen;
  assert(istrm.next_in == compbuf);
  assert(istrm.avail_in == (uInt)compLen);
  Bytef outbuf[512];
  memset(outbuf, 0, sizeof(outbuf));
  assert(outbuf[0] == 0);
  istrm.next_out = outbuf;
  istrm.avail_out = (uInt)sizeof(outbuf);
  assert(istrm.next_out == outbuf);
  assert(istrm.avail_out == (uInt)sizeof(outbuf));
  inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));
  assert(istrm.total_in == 0);
  assert(istrm.total_out == 0);
  assert(istrm.msg == NULL);
  assert(istrm.state != NULL);
  inflate(&istrm, 4);
  assert(istrm.total_in > 0);
  assert(istrm.total_out > 0);
  assert(istrm.total_in <= (uInt)compLen);
  assert(istrm.total_out <= (uInt)sizeof(outbuf));
  assert(istrm.next_in == compbuf + istrm.total_in);
  assert(istrm.avail_in == (uInt)compLen - istrm.total_in);
  assert(istrm.next_out == outbuf + istrm.total_out);
  assert(istrm.avail_out == (uInt)sizeof(outbuf) - istrm.total_out);
  assert(memcmp(outbuf, src_combined, istrm.total_out) == 0);
  unsigned long codes_used = inflateCodesUsed(&istrm);
  (void)codes_used;
  // step 4: Validate and cleanup (write compressed data to gz, clear errors,
  // close, end inflate)
  inflateEnd(&istrm);
  assert(istrm.state == NULL);
  gzFile gz = gzopen("test_zlib_api.gz", "wb");
  assert(gz != NULL);
  gzwrite(gz, compbuf, (unsigned int)compLen);
  assert(compbuf != NULL);
  assert(compLen > 0);
  gzclearerr(gz);
  gzclose(gz);
  return 66;
}

int main() {
  for (int i = 0; i < 10; i++) {
    int result = test_api_sequence();
    if (result != 66) {
      std::cerr << "API sequence test failed with error code: " << result
                << std::endl;
      exit(100);
    }
  }
  printf("Test completed successfully.\n");
  return 0;
}
