
// Header
#include <cassert>
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <png.h>
#include <stdlib.h>
int test_api_sequence() {
  // step 1: Initialize
  png_structrp png_ptr = (png_structrp)0;
  png_infop info_ptr = png_create_info_struct(png_ptr);
  png_sPLT_t splt;
  memset(&splt, 0, sizeof(splt));
  splt.depth = 8;
  splt.nentries = 0;
  assert(png_ptr == NULL);
  assert(info_ptr == NULL);
  assert(splt.depth == 8);
  assert(splt.nentries == 0);
  // step 2: Configure
  png_set_read_status_fn(png_ptr, (png_read_status_ptr)0);
  assert(png_ptr == NULL);
  png_set_filler(png_ptr, (png_uint_32)0xFFu, 1);
  assert(png_ptr == NULL);
  png_set_sPLT(png_ptr, info_ptr, (png_const_sPLT_tp)&splt, 1);
  assert(png_ptr == NULL);
  assert(info_ptr == NULL);
  // Verify splt structure integrity (passed as const, should be unmodified)
  assert(splt.depth == 8);
  assert(splt.nentries == 0);
  // step 3: Operate & Validate
  png_image image;
  memset(&image, 0, sizeof(image));
  // Ensure structural integrity after initialization
  assert(image.opaque == NULL);
  assert(image.width == 0);
  image.version = png_access_version_number();
  // Validate version contract from API docs
  assert(image.version > 0);
  image.width = 2;
  image.height = 2;
  image.format = 0;
  // Verify image parameters are set as expected
  assert(image.width == 2);
  assert(image.height == 2);
  assert(image.format == 0);
  unsigned char pixel_buffer[8] = {0xFF, 0x00, 0x00, 0xFF,
                                   0x00, 0xFF, 0x00, 0xFF}; // two RGBA pixels
  // Validate buffer initialization
  assert(pixel_buffer[0] == 0xFF);
  assert(pixel_buffer[3] == 0xFF);
  png_alloc_size_t memory_bytes = 0;
  void *memory_out = 0;
  // API Contract Validation:
  // The call passes &memory_out (non-NULL) as the 'memory' buffer pointer.
  // It passes 0 as 'memory_bytes' (input buffer size).
  // API Docs: "If 'memory' is not NULL... if the output exceeds this, the
  // function fails." Since a PNG requires > 0 bytes, this call must fail.
  int write_result =
      png_image_write_to_memory(&image, &memory_out, &memory_bytes, 1,
                                pixel_buffer, (png_int_32)8, (const void *)0);
  assert(write_result == 0);
  // Validate state consistency:
  // The memory_out variable (initially 0) was passed by address as a buffer.
  // Since the write failed due to 0 size, the variable itself should remain
  // 0/NULL.
  assert(memory_out == NULL);
  // Note: If memory_bytes is updated to the required size on failure, it should
  // be > 0
  if (memory_bytes > 0) {
    assert(memory_bytes > sizeof(image)); // Basic sanity check on PNG overhead
  }
  // step 4: Cleanup
  memset(&splt, 0, sizeof(splt));
  memset(pixel_buffer, 0, sizeof(pixel_buffer));
  // Verify pixel buffer is cleared
  assert(pixel_buffer[0] == 0);
  assert(pixel_buffer[sizeof(pixel_buffer) - 1] == 0);
  memset(&image, 0, sizeof(image));
  // Verify image structure is cleared and reset to initial state
  assert(image.width == 0);
  assert(image.height == 0);
  assert(image.format == 0);
  assert(image.version == 0);
  assert(image.opaque == NULL);
  free(memory_out);
  // API sequence test completed successfully
  (void)write_result;
  (void)info_ptr;
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
