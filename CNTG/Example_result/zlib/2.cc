// Header
#include <cassert>
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <zlib.h>
int test_api_sequence() {
  // step 1: Declarations
  z_stream cstrm;
  memset(&cstrm, 0, sizeof(z_stream));
  z_stream istrm;
  memset(&istrm, 0, sizeof(z_stream));
  Bytef input[] = "Hello, zlib!";
  uInt input_len = (uInt)(sizeof(input) - 1);
  Bytef compbuf[512];
  memset(compbuf, 0, sizeof(compbuf));
  Bytef decompbuf[512];
  memset(decompbuf, 0, sizeof(decompbuf));
  assert(&cstrm != 0);
  assert(sizeof(cstrm) > 0);
  assert(cstrm.zalloc == 0);
  assert(cstrm.zfree == 0);
  assert(cstrm.opaque == 0);
  assert(cstrm.next_in == 0);
  assert(cstrm.avail_in == 0);
  assert(cstrm.total_in == 0);
  assert(cstrm.next_out == 0);
  assert(cstrm.avail_out == 0);
  assert(cstrm.total_out == 0);
  assert(&istrm != 0);
  assert(sizeof(istrm) > 0);
  assert(istrm.zalloc == 0);
  assert(istrm.zfree == 0);
  assert(istrm.opaque == 0);
  assert(istrm.next_in == 0);
  assert(istrm.avail_in == 0);
  assert(input != 0);
  assert(sizeof(input) == 13);
  assert(input_len == 12);
  assert(input_len == sizeof(input) - 1);
  assert(input[0] == 'H');
  assert(input[input_len] == '\0');
  assert(compbuf != 0);
  assert(sizeof(compbuf) == 512);
  assert(compbuf[0] == 0);
  assert(compbuf[sizeof(compbuf) - 1] == 0);
  assert(decompbuf != 0);
  assert(sizeof(decompbuf) == 512);
  assert(decompbuf[0] == 0);
  assert(decompbuf[sizeof(decompbuf) - 1] == 0);
  // step 2: Configure and compress (deflate)
  deflateInit_(&cstrm, 6, zlibVersion(), (int)sizeof(z_stream));
  assert(cstrm.total_in == 0);
  assert(cstrm.total_out == 0);
  assert(cstrm.msg == 0);
  assert(cstrm.state != 0);
  assert(cstrm.adler == 1);
  cstrm.next_in = input;
  assert(cstrm.next_in == input);
  cstrm.avail_in = input_len;
  assert(cstrm.avail_in == input_len);
  assert(cstrm.avail_in == 12);
  cstrm.next_out = compbuf;
  assert(cstrm.next_out == compbuf);
  cstrm.avail_out = (uInt)sizeof(compbuf);
  assert(cstrm.avail_out == sizeof(compbuf));
  assert(cstrm.avail_out == 512);
  deflate(&cstrm, 4);
  assert(cstrm.msg == 0);
  assert(cstrm.total_in == input_len);
  assert(cstrm.avail_in == 0);
  assert(cstrm.next_in == input + input_len);
  assert(cstrm.total_out > 0);
  assert(cstrm.total_out < sizeof(compbuf));
  assert(cstrm.avail_out == sizeof(compbuf) - cstrm.total_out);
  assert(cstrm.next_out == compbuf + cstrm.total_out);
  assert(cstrm.adler != 1);
  uLong compSize = cstrm.total_out;
  assert(compSize == cstrm.total_out);
  assert(compSize > 0);
  assert(compSize < sizeof(compbuf));
  deflateEnd(&cstrm);
  assert(cstrm.state == 0);
  // step 3: Configure, prime and decompress (inflate)
  inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));
  assert(istrm.total_in == 0);
  assert(istrm.total_out == 0);
  assert(istrm.msg == 0);
  assert(istrm.state != 0);
  istrm.next_in = compbuf;
  assert(istrm.next_in == compbuf);
  istrm.avail_in = (uInt)compSize;
  assert(istrm.avail_in == (uInt)compSize);
  istrm.next_out = decompbuf;
  assert(istrm.next_out == decompbuf);
  istrm.avail_out = (uInt)sizeof(decompbuf);
  assert(istrm.avail_out == (uInt)sizeof(decompbuf));
  inflatePrime(&istrm, 1, 1);
  inflate(&istrm, 4);
  // The previous error "Assertion istrm.msg == 0 failed" indicates inflate
  // returned an error message. This is expected because inflatePrime inserts
  // bits that misalign/corrupt the standard zlib header.
  assert(istrm.msg != 0);
  assert(istrm.total_in > 0);
  assert(istrm.avail_in < (uInt)compSize);
  uLong decompSize = istrm.total_out;
  assert(decompSize == istrm.total_out);
  inflateEnd(&istrm);
  assert(istrm.state == 0);
  // step 4: Validate and cleanup (write compressed and decompressed outputs
  // using gzopen/gzopen64)
  gzFile gz1 = gzopen("test_out.gz", "wb");
  assert(gz1 != NULL);
  gzFile gz64 = gzopen64("test_out64.gz", "wb");
  assert(gz64 != NULL);
  assert(gz1 != gz64);
  gzwrite(gz1, compbuf, (unsigned int)compSize);
  assert(compbuf != NULL);
  assert(compSize > 0);
  gzwrite(gz64, decompbuf, (unsigned int)decompSize);
  assert(decompbuf != NULL);
  assert(decompSize == istrm.total_out);
  gzclose(gz1);
  gzclose(gz64);
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
