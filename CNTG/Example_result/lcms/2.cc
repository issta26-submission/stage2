
#include <cstddef>
#include <cstdio>
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
  // step 1: Declarations
  cmsContext ctx = NULL;
  cmsHPROFILE bchProfile = NULL;
  cmsHPROFILE inkProfile = NULL;
  cmsHANDLE hIT8 = NULL;
  cmsStage *clutStage = NULL;
  cmsUInt32Number headerFlags = 0;
  const char *patchName = NULL;
  char patchBuf[64];
  memset(patchBuf, 0, sizeof(patchBuf));
  assert(ctx == NULL);
  assert(bchProfile == NULL);
  assert(inkProfile == NULL);
  assert(hIT8 == NULL);
  assert(clutStage == NULL);
  assert(headerFlags == 0);
  assert(patchName == NULL);
  assert(patchBuf[0] == 0);
  assert(patchBuf[sizeof(patchBuf) - 1] == 0);
  // step 2: Setup - create profiles and IT8 handle
  bchProfile = cmsCreateBCHSWabstractProfileTHR(ctx, 16U, 50.0, 10.0, 0.0, 1.2,
                                                5000U, 6500U);
  assert(bchProfile != NULL);
  hIT8 = cmsIT8Alloc(ctx);
  assert(hIT8 != NULL);
  // step 3: Core operations - inspect header, create ink-limiting link and CLUT
  // stage, query IT8 patch
  headerFlags = cmsGetHeaderFlags(bchProfile);
  // Ensure bchProfile interaction was valid; headerFlags is just a bitfield
  // return.
  cmsColorSpaceSignature csig = _cmsICCcolorSpace(0);
  inkProfile = cmsCreateInkLimitingDeviceLinkTHR(ctx, csig,
                                                 (cmsFloat64Number)headerFlags);
  // inkProfile may be NULL if the limit (headerFlags) is invalid/excessive, but
  // if created, it must satisfy invariants.
  if (inkProfile != NULL) {
    assert(cmsGetColorSpace(inkProfile) == csig);
    assert(cmsGetDeviceClass(inkProfile) == cmsSigLinkClass);
  }
  static const cmsFloat32Number clutTable[24] = {
      0.0f,  0.25f, 0.5f,  0.75f, 1.0f,  0.1f,  0.2f,  0.3f,
      0.4f,  0.5f,  0.6f,  0.7f,  0.8f,  0.9f,  1.0f,  0.05f,
      0.15f, 0.25f, 0.35f, 0.45f, 0.55f, 0.65f, 0.75f, 0.85f};
  // 2 grid points, 3 inputs, 3 outputs require 2^3 * 3 = 24 entries. Table size
  // matches exactly.
  clutStage = cmsStageAllocCLutFloat(ctx, 2U, 3U, 3U, clutTable);
  assert(clutStage != NULL);
  assert(cmsStageInputChannels(clutStage) == 3U);
  assert(cmsStageOutputChannels(clutStage) == 3U);
  assert(cmsStageType(clutStage) == cmsSigCLutElemType);
  patchName = cmsIT8GetPatchName(hIT8, 0, patchBuf);
  // hIT8 is a valid handle from the previous state. patchName is NULL or a
  // valid string pointer. step 4: Cleanup - free allocated stage and IT8
  // handle, delete context
  cmsStageFree(clutStage);
  cmsIT8Free(hIT8);
  cmsDeleteContext(ctx);
  // API sequence test completed successfully
  (void)bchProfile;
  (void)inkProfile;
  (void)headerFlags;
  (void)patchName;
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
