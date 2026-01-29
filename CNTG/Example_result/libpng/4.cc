
// Header
#include <cassert>
#include <cstddef>
#include <cstdio> // For printf
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
int test_api_sequence() {
  // step 1: Declarations / Initialize
  png_structrp png_ptr = (png_structrp)0;
  png_inforp info_ptr = (png_inforp)0;
  png_image image;
  png_alloc_size_t out_memory_bytes = 4096;
  unsigned char out_memory[4096];
  unsigned char input_buffer[512];
  memset(&image, 0, sizeof(image));
  memset(out_memory, 0, sizeof(out_memory));
  memset(input_buffer, 0x7F, sizeof(input_buffer));
  image.version = png_access_version_number();
  image.width = 2;
  image.height = 2;
  image.format = 0;
  image.flags = 0;
  image.colormap_entries = 0;
  image.warning_or_error = 0;
  assert(png_ptr == NULL);
  assert(info_ptr == NULL);
  assert(out_memory_bytes == 4096);
  assert(out_memory[0] == 0);
  assert(out_memory[4095] == 0);
  assert(input_buffer[0] == 0x7F);
  assert(input_buffer[511] == 0x7F);
  assert(image.version == png_access_version_number());
  assert(image.width == 2);
  assert(image.height == 2);
  assert(image.format == 0);
  assert(image.flags == 0);
  assert(image.colormap_entries == 0);
  assert(image.warning_or_error == 0);
  // step 2: Configure libpng callbacks and transforms
  png_set_read_status_fn(png_ptr, (png_read_status_ptr)0);
  png_set_filler(png_ptr, (png_uint_32)0xFF, 0);
  png_set_sPLT((png_const_structrp)png_ptr, info_ptr, (png_const_sPLT_tp)0, 0);
  // step 3: Operate - write image to provided memory buffer
  png_imagep imgp = &image;
  assert(imgp != NULL);
  assert(imgp->width > 0);
  assert(imgp->height > 0);
  assert(out_memory != NULL);
  assert(input_buffer != NULL);
  png_int_32 row_stride = (png_int_32)(image.width * 3);
  assert(row_stride > 0);
  png_image_write_to_memory(imgp, out_memory, &out_memory_bytes, 1,
                            input_buffer, row_stride, (const void *)0);
  assert(out_memory_bytes > 0);
  // step 4: Validation / Cleanup
  memset(input_buffer, 0, sizeof(input_buffer));
  assert(input_buffer[0] == 0);
  assert(input_buffer[sizeof(input_buffer) - 1] == 0);
  memset(out_memory, 0, sizeof(out_memory));
  assert(out_memory[0] == 0);
  assert(out_memory_bytes > 0);
  assert(out_memory[out_memory_bytes - 1] == 0);
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
