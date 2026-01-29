
// Header
#include <cassert>
#include <cre2.h>
#include <cstddef>
#include <cstdio> // Required for printf in main
#include <cstring>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
int test_api_sequence() {
  // step 1: Declarations (Initialize)
  const char *pattern = "(ab)(cd)?";
  int pattern_len = (int)(sizeof("(ab)(cd)?") - 1);
  const char *input_text = "abcd ab abcd";
  int input_len = (int)(sizeof("abcd ab abcd") - 1);
  cre2_regexp_t *rex = cre2_new(pattern, pattern_len, NULL);
  cre2_string_t input_str;
  input_str.data = input_text;
  input_str.length = input_len;
  cre2_string_t full_matches[3];
  cre2_range_t ranges[3];
  memset(full_matches, 0, sizeof(full_matches));
  memset(ranges, 0, sizeof(ranges));
  assert(pattern != NULL);
  assert(pattern_len == 9);
  assert(input_text != NULL);
  assert(input_len == 12);
  assert(rex != NULL);
  assert(input_str.data == input_text);
  assert(input_str.length == input_len);
  assert(full_matches[0].data == NULL && full_matches[0].length == 0);
  assert(full_matches[1].data == NULL && full_matches[1].length == 0);
  assert(full_matches[2].data == NULL && full_matches[2].length == 0);
  // step 2: Setup (Configure)
  cre2_string_t easy_matches[2];
  memset(easy_matches, 0, sizeof(easy_matches));
  assert(easy_matches[0].data == NULL);
  assert(easy_matches[0].length == 0);
  assert(easy_matches[1].data == NULL);
  assert(easy_matches[1].length == 0);
  // step 3: Operate (use full match with compiled regex and easy match)
  cre2_full_match_re(rex, &input_str, full_matches, 3);
  cre2_easy_match("ab", 2, input_text, input_len, easy_matches, 2);
  assert(easy_matches[0].data == input_text);
  assert(easy_matches[0].length == 2);
  assert(memcmp(easy_matches[0].data, "ab", 2) == 0);
  assert(easy_matches[1].data == NULL);
  assert(easy_matches[1].length == 0);
  // step 4: Transform (strings to ranges) and Replace (operate on text buffer)
  cre2_strings_to_ranges(input_text, ranges, full_matches, 3);
  int buf_len = input_len + 128;
  char *buf = new char[buf_len];
  assert(buf != NULL);
  memset(buf, 0, buf_len);
  assert(buf[0] == 0 && buf[buf_len - 1] == 0);
  memcpy(buf, input_text, input_len);
  assert(memcmp(buf, input_text, input_len) == 0);
  assert(buf[input_len] == 0);
  cre2_string_t text_and_target;
  text_and_target.data = buf;
  text_and_target.length = input_len;
  assert(text_and_target.data == buf);
  assert(text_and_target.length == 6);
  cre2_string_t rewrite;
  rewrite.data = "X";
  rewrite.length = 1;
  cre2_replace_re(rex, &text_and_target, &rewrite);
  assert(text_and_target.data == buf);
  assert(text_and_target.length == input_len - 1);
  assert(memcmp(text_and_target.data, "Xcdef", text_and_target.length) == 0);
  assert(buf[5] == 0);
  // step 5: Cleanup (Destroy resources)
  cre2_delete(rex);
  assert(buf != NULL);
  assert(text_and_target.data == buf);
  assert(text_and_target.length == 5);
  assert(memcmp(text_and_target.data, "Xcdef", 5) == 0);
  delete[] buf;
  assert(input_len == 6);
  assert(rewrite.length == 1);
  assert(memcmp(rewrite.data, "X", 1) == 0);
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
