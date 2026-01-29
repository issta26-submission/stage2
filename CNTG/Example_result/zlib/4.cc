#include <cstddef> // Mandatory first header

// Original headers, filtered and deduplicated/removed unused ones
#include <cassert>
#include <cstdio>   // For printf
#include <iostream> // For std::cerr
#include <string.h> // For memset
#include <zlib.h>
int test_api_sequence() {
  // step 1: Declarations and input preparation
  z_stream istrm;
  memset(&istrm, 0, sizeof(z_stream));
  Bytef input[] = "Hello, zlib!";
  uLong inputLen = (uLong)(sizeof(input) - 1);
  Bytef compbuf[1024];
  Bytef decompbuf[1024];
  memset(compbuf, 0, sizeof(compbuf));
  memset(decompbuf, 0, sizeof(decompbuf));
  uLongf compLen = (uLongf)sizeof(compbuf);
  assert(sizeof(istrm) > 0);
  assert(istrm.zalloc == 0);
  assert(istrm.zfree == 0);
  assert(istrm.opaque == 0);
  assert(istrm.next_in == 0);
  assert(istrm.avail_in == 0);
  assert(sizeof(input) == 13);
  assert(inputLen == 12);
  assert(input[0] == 'H');
  assert(input[inputLen] == '\0');
  assert(sizeof(compbuf) == 1024);
  assert(compbuf[0] == 0);
  assert(compbuf[sizeof(compbuf) - 1] == 0);
  assert(sizeof(decompbuf) == 1024);
  assert(decompbuf[0] == 0);
  assert(decompbuf[sizeof(decompbuf) - 1] == 0);
  assert(compLen == 1024);
  assert(compLen == sizeof(compbuf));
  // step 2: Compress the input using the convenience API
  compress2(compbuf, &compLen, input, inputLen, 6);
  assert(compLen > 0);
  assert(compLen < 1024);
  assert(compLen <= sizeof(compbuf));
  assert(inputLen == 12);
  assert(memcmp(input, "Hello, zlib!", 12) == 0);
  assert(compbuf[0] != 0);
  uLong compSize = (uLong)compLen;
  assert(compSize == compLen);
  assert(compSize > 0);
  assert(compSize < 1024);
  // step 3: Initialize inflate, prime bit buffer, and perform inflation
  inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));
  assert(istrm.total_in == 0);
  assert(istrm.total_out == 0);
  assert(istrm.state != NULL); // Verify internal state allocation
  istrm.next_in = compbuf;
  istrm.avail_in = (uInt)compSize;
  istrm.next_out = decompbuf;
  istrm.avail_out = (uInt)sizeof(decompbuf);
  assert(istrm.next_in == compbuf);
  assert(istrm.avail_in > 0);
  assert(istrm.avail_in == compSize);
  assert(istrm.next_out == decompbuf);
  assert(istrm.avail_out >= 12); // Output buffer must hold "Hello, zlib!"
  inflatePrime(&istrm, 0, 0);
  assert(istrm.state != NULL);
  assert(istrm.total_in == 0); // Prime with 0 bits should not advance input
  assert(istrm.total_out == 0);
  inflate(&istrm, 4);            // 4 corresponds to Z_FINISH
  assert(istrm.total_out == 12); // "Hello, zlib!" length
  assert(istrm.total_out == inputLen);
  assert(istrm.total_in == compSize);
  assert(istrm.avail_in == 0); // Should consume all input with Z_FINISH
  assert(memcmp(decompbuf, "Hello, zlib!", 12) ==
         0); // Verify semantic correctness (data round-trip)
  assert(memcmp(decompbuf, input, inputLen) == 0);
  uLong decompressedBytes = istrm.total_out;
  assert(decompressedBytes == 12);
  assert(decompressedBytes == inputLen);
  inflateEnd(&istrm);
  // step 4: Persist results using gzopen and gzopen64, then cleanup and finish
  gzFile gz1 = gzopen("out.gz", "wb");
  assert(gz1 != NULL);
  assert(compbuf != NULL);
  assert(compSize > 0);
  gzwrite(gz1, compbuf, (unsigned int)compSize);
  assert(gz1 != NULL);
  gzclose(gz1);
  gzFile gz2 = gzopen64("out64.gz", "wb");
  assert(gz2 != NULL);
  assert(decompbuf != NULL);
  assert(decompressedBytes > 0);
  assert(decompressedBytes == inputLen);
  assert(memcmp(decompbuf, input, inputLen) == 0);
  gzwrite(gz2, decompbuf, (unsigned int)decompressedBytes);
  assert(gz2 != NULL);
  gzclose(gz2);
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
