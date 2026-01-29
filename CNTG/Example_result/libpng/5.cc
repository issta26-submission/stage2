
#include <cstddef>
#include <cstdio> // Required for printf
#include <cstring>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <png.h>
#include <sstream>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

// Header
#include <cassert>
int test_api_sequence() {
  // step 1: Initialize / Declarations
  png_structrp png_ptr =
      png_create_read_struct((png_const_charp) "1.6.37", (png_voidp)NULL,
                             (png_error_ptr)NULL, (png_error_ptr)NULL);
  png_inforp info_ptr = (png_inforp)NULL;
  png_uint_32 version = png_access_version_number();
  png_fixed_point gamma_fixed = (png_fixed_point)22000;
  int option_flag = (int)(version & 0xFFu);
  assert(png_ptr != NULL);
  assert(info_ptr == NULL);
  assert(version > 0);
  assert(version >= 10637);
  assert(gamma_fixed == 22000);
  assert(option_flag >= 0 && option_flag <= 255);
  // step 2: Configure libpng behavior
  png_set_option(png_ptr, option_flag, 1);
  png_set_bgr(png_ptr);
  png_set_gAMA_fixed((png_const_structrp)png_ptr, info_ptr, gamma_fixed);
  assert(png_get_valid(png_ptr, info_ptr, PNG_INFO_gAMA) == 0);
  // step 3: Operate (query state and adjust options)
  png_uint_32 x_pixels_per_meter = png_get_x_pixels_per_meter(
      (png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
  assert(png_get_valid(png_ptr, info_ptr, PNG_INFO_pHYs) == 0);
  assert(x_pixels_per_meter == 0);
  png_set_option(png_ptr, option_flag ^ (int)(x_pixels_per_meter & 0xFFu), 0);
  // step 4: Validate-like cleanup
  png_struct *png_ptr_nonrestrict = png_ptr;
  assert(png_ptr_nonrestrict == png_ptr);
  assert(png_ptr_nonrestrict != NULL);
  png_destroy_write_struct(&png_ptr_nonrestrict, (png_infopp)NULL);
  assert(png_ptr_nonrestrict == NULL);
  (void)version;
  (void)x_pixels_per_meter;
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
