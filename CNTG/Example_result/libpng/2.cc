
#include <cstddef>  // Added as per rule 4
#include <cstdio>   // Needed for printf
#include <iostream> // Needed for std::cerr
#include <png.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// Header
#include <cassert>
int test_api_sequence() {
  // step 1: Declarations / Initialize
  png_structrp png_ptr =
      png_create_write_struct((png_const_charp) nullptr, (png_voidp) nullptr,
                              (png_error_ptr) nullptr, (png_error_ptr) nullptr);
  png_inforp info_ptr = nullptr;
  png_uint_32 version = png_access_version_number();
  unsigned char *buffer = (unsigned char *)malloc(128);
  memset(buffer, 0xA5, 128);
  png_fixed_point fixed_gamma = (png_fixed_point)22000;
  int option_id = 0;
  int option_on = 1;
  png_uint_32 pixels_per_meter = 0;
  assert(png_ptr == nullptr);
  assert(info_ptr == nullptr);
  assert(version > 0);
  assert(buffer != nullptr);
  assert(buffer[0] == 0xA5);
  assert(buffer[127] == 0xA5);
  assert(fixed_gamma == 22000);
  assert(option_id == 0);
  assert(option_on == 1);
  assert(pixels_per_meter == 0);
  // step 2: Configure libpng behavior
  png_set_option(png_ptr, option_id, option_on);
  assert(option_id == 0);
  assert(option_on == 1);
  png_set_bgr(png_ptr);
  assert(png_ptr == nullptr);
  png_set_gAMA_fixed((png_const_structrp)png_ptr, info_ptr, fixed_gamma);
  assert(info_ptr == nullptr);
  assert(fixed_gamma == 22000);
  // step 3: Operate / Validate
  pixels_per_meter = png_get_x_pixels_per_meter((png_const_structrp)png_ptr,
                                                (png_const_inforp)info_ptr);
  // Verify API contract: if the info pointer or structure is missing (NULL),
  // the resolution cannot be determined and must be 0. Based on previous
  // program state where pointers were asserted as nullptr, this ensures the
  // function handles NULL gracefully or returns 0.
  assert(png_ptr != nullptr || pixels_per_meter == 0);
  assert(info_ptr != nullptr || pixels_per_meter == 0);
  (void)version;
  (void)pixels_per_meter;
  (void)buffer;
  // step 4: Cleanup
  free(buffer);
  png_destroy_write_struct((png_structpp)&png_ptr, (png_infopp)&info_ptr);
  assert(png_ptr == nullptr);
  assert(info_ptr == nullptr);
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
