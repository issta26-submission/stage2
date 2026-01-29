#include <cstddef>
#include <cstdio>
#include <fcntl.h>
#include <iostream>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>

// Header
#include <cassert>
int test_api_sequence() {
  // step 1
  z_stream strm;
  memset(&strm, 0, sizeof(strm));
  strm.zalloc = NULL;
  strm.zfree = NULL;
  strm.opaque = NULL;
  const char buf1[] = "Hello";
  const char buf2[] = "World";
  unsigned char outbuf[128];
  memset(outbuf, 0, sizeof(outbuf));
  gzFile gz = 0;
  uLong ad1 = 0;
  uLong ad2 = 0;
  uLong combined = 0;
  int init_ret = 0;
  int end_ret = 0;
  int gzclose_ret = 0;
  int eof_flag = 0;
  assert(strm.zalloc == NULL);
  assert(strm.zfree == NULL);
  assert(strm.opaque == NULL);
  assert(strm.next_in == NULL);
  assert(strm.avail_in == 0);
  assert(strm.next_out == NULL);
  assert(strm.avail_out == 0);
  assert(sizeof(buf1) == 6);
  assert(buf1[0] == 'H');
  assert(buf1[5] == '\0');
  assert(sizeof(buf2) == 6);
  assert(buf2[0] == 'W');
  assert(buf2[5] == '\0');
  assert(sizeof(outbuf) == 128);
  assert(outbuf[0] == 0);
  assert(outbuf[127] == 0);
  assert(gz == 0);
  assert(ad1 == 0);
  assert(ad2 == 0);
  assert(combined == 0);
  assert(init_ret == 0);
  assert(end_ret == 0);
  assert(gzclose_ret == 0);
  assert(eof_flag == 0);
  // step 2
  init_ret = deflateInit2_(&strm, 6, 8, 15, 8, 0, zlibVersion(),
                           (int)sizeof(z_stream));
  assert(init_ret == 0);
  assert(strm.zalloc != NULL);
  assert(strm.zfree != NULL);
  assert(strm.opaque == NULL);
  assert(strm.msg == NULL);
  // step 3
  ad1 = adler32(1UL, (const Bytef *)buf1, (uInt)(sizeof(buf1) - 1));
  assert(ad1 != 0);
  assert(buf1 != NULL || (sizeof(buf1) - 1) == 0);
  ad2 = adler32(1UL, (const Bytef *)buf2, (uInt)(sizeof(buf2) - 1));
  assert(ad2 != 0);
  assert(buf2 != NULL || (sizeof(buf2) - 1) == 0);
  combined = adler32_combine64(ad1, ad2, (off_t)(sizeof(buf2) - 1));
  assert(combined != 0);
  // Invariant: If the second sequence is empty, the combined checksum equals
  // the first checksum.
  if ((sizeof(buf2) - 1) == 0)
    assert(combined == ad1);
  gz = gzopen("test_zlib_api.gz", "wb");
  assert(gz != NULL); // gzopen must return a valid handle for "wb" mode
  gzwrite(gz, buf1, (unsigned int)(sizeof(buf1) - 1));
  // assert that the file handle remains valid after write
  assert(gz != NULL);
  gzwrite(gz, buf2, (unsigned int)(sizeof(buf2) - 1));
  assert(gz != NULL);
  gzflush(gz, 2);
  // 2 is Z_SYNC_FLUSH; the file must be open for writing (which we asserted via
  // gz != NULL)
  gzclearerr(gz);
  // After clearing errors, error flags should be reset.
  eof_flag = gzeof(gz);
  // For a file opened in "wb" mode, we are writing, so EOF (end of input) is
  // not relevant/set.
  assert(eof_flag == 0);
  // step 4
  assert(&strm != NULL); // Ensure the address of the stream structure is valid
  end_ret = deflateEnd(&strm);
  // API Contract: deflateEnd returns Z_OK on success, or specific error codes
  // for inconsistency
  assert(end_ret == Z_OK || end_ret == Z_STREAM_ERROR ||
         end_ret == Z_DATA_ERROR);
  // Invariant: If deflateEnd succeeds, the internal state pointer must be
  // nullified
  if (end_ret == Z_OK) {
    assert(strm.state == Z_NULL);
  }
  // gz handle must be valid (not NULL) before calling gzclose, based on
  // previous assertions
  assert(gz != NULL);
  gzclose_ret = gzclose(gz);
  // API Contract: gzclose returns Z_OK, or specific error codes
  assert(gzclose_ret == Z_OK || gzclose_ret == Z_ERRNO ||
         gzclose_ret == Z_STREAM_ERROR || gzclose_ret == Z_BUF_ERROR);
  // Functional Correctness: Given the successful open ("wb") and writes in the
  // previous state, close should succeed
  assert(gzclose_ret == Z_OK);
  // API sequence test completed successfully
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
