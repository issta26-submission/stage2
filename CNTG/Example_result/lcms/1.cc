
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <lcms2.h>
#include <sstream>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

// Header
#include <cassert>
int test_api_sequence() {
  // step 1
  cmsContext ctx = cmsCreateContext(NULL, NULL);
  cmsHPROFILE profile = cmsCreateNULLProfileTHR(ctx);
  cmsUInt32Number encVersion = cmsGetEncodedICCversion(profile);
  (void)encVersion;
  cmsUInt32Number headerFlags = cmsGetHeaderFlags(profile);
  assert(ctx != NULL);
  assert(profile != NULL);
  assert(encVersion != 0);
  assert(encVersion == cmsGetEncodedICCversion(profile));
  assert(((encVersion >> 24) == 2) || ((encVersion >> 24) == 4));
  assert(headerFlags == cmsGetHeaderFlags(profile));
  assert(headerFlags == 0);
  // step 2
  (void)headerFlags;
  cmsUInt32Number headerCreator = cmsGetHeaderCreator(profile);
  assert(headerCreator == cmsGetHeaderCreator(profile));
  (void)headerCreator;
  cmsUInt32Number headerModel = cmsGetHeaderModel(profile);
  assert(headerModel == cmsGetHeaderModel(profile));
  (void)headerModel;
  cmsUInt32Number renderingIntent = cmsGetHeaderRenderingIntent(profile);
  assert(renderingIntent == cmsGetHeaderRenderingIntent(profile));
  assert(renderingIntent <= 3);
  (void)renderingIntent;
  // step 3
  cmsUInt32Number bytesBuf = 1024;
  void *buffer = malloc((size_t)bytesBuf);
  assert(buffer != NULL);
  assert(profile != NULL);
  int saved = cmsSaveProfileToMem(profile, buffer, &bytesBuf);
  assert(saved == 0 || saved == 1);
  assert(!saved || (bytesBuf > 0 && bytesBuf <= 1024));
  (void)saved;
  // step 4
  cmsHPROFILE profile_from_mem = cmsOpenProfileFromMem(buffer, bytesBuf);
  // If the profile was successfully saved to the buffer, opening it from memory
  // should succeed (barring OOM).
  assert(!saved || profile_from_mem != NULL);
  // If the profile handle is valid, its encoded version must match the original
  // profile's version, ensuring data integrity.
  assert(profile_from_mem == NULL ||
         cmsGetEncodedICCversion(profile_from_mem) ==
             cmsGetEncodedICCversion(profile));
  (void)cmsGetEncodedICCversion(profile_from_mem);
  free(buffer);
  (void)ctx;
  (void)profile;
  (void)profile_from_mem;
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
