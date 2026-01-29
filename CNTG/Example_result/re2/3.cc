
// Header
#include <cassert>
#include <cre2.h>
#include <cstddef>
#include <cstdio>
#include <iostream>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
int test_api_sequence() {
  // step 1: Declarations
  const char *pattern = "a(b)c";
  int pattern_len = 5;
  const char *text = "abc abc";
  int text_len = 7;
  cre2_options_t *opt = cre2_opt_new();
  cre2_regexp_t *rex = cre2_new(pattern, pattern_len, opt);
  cre2_string_t match_easy[2];
  cre2_string_t match_full[2];
  cre2_range_t ranges[2];
  cre2_string_t text_and_target;
  cre2_string_t rewrite;
  assert(pattern != nullptr);
  assert(pattern_len == 5);
  assert(strlen(pattern) == pattern_len);
  assert(text != nullptr);
  assert(text_len == 7);
  assert(strlen(text) == text_len);
  assert(opt != nullptr);
  assert(rex != nullptr);
  assert(cre2_error_code(rex) == 0);
  assert(cre2_num_capturing_groups(rex) == 1);
  assert(strcmp(cre2_pattern(rex), pattern) == 0);
  // step 2: Setup
  cre2_opt_set_case_sensitive(opt, 1);
  match_easy[0].data = 0;
  match_easy[0].length = 0;
  assert(match_easy[0].data == NULL);
  assert(match_easy[0].length == 0);
  match_easy[1].data = 0;
  match_easy[1].length = 0;
  assert(match_easy[1].data == NULL);
  assert(match_easy[1].length == 0);
  match_full[0].data = 0;
  match_full[0].length = 0;
  assert(match_full[0].data == NULL);
  assert(match_full[0].length == 0);
  match_full[1].data = 0;
  match_full[1].length = 0;
  assert(match_full[1].data == NULL);
  assert(match_full[1].length == 0);
  ranges[0].start = 0;
  ranges[0].past = 0;
  assert(ranges[0].start == 0);
  assert(ranges[0].past == 0);
  ranges[1].start = 0;
  ranges[1].past = 0;
  assert(ranges[1].start == 0);
  assert(ranges[1].past == 0);
  text_and_target.data = text;
  assert(text_and_target.data == text);
  text_and_target.length = text_len;
  assert(text_and_target.length == text_len);
  assert(memcmp(text_and_target.data, "abc abc", text_and_target.length) == 0);
  rewrite.data = "X";
  assert(rewrite.data != NULL);
  rewrite.length = 1;
  assert(rewrite.length == 1);
  assert(memcmp(rewrite.data, "X", rewrite.length) == 0);
  // step 3: Operate - easy match and convert matches to ranges
  cre2_easy_match(pattern, pattern_len, text, text_len, match_easy, 2);
  assert(match_easy[0].data == text);
  assert(match_easy[0].length == 3);
  assert(memcmp(match_easy[0].data, "abc", 3) == 0);
  assert(match_easy[1].data == text + 4);
  assert(match_easy[1].length == 3);
  assert(memcmp(match_easy[1].data, "abc", 3) == 0);
  cre2_strings_to_ranges(text, ranges, match_easy, 2);
  assert(ranges[0].start == 0);
  assert(ranges[0].past == 3);
  assert(ranges[1].start == 4);
  assert(ranges[1].past == 7);
  // step 4: Operate - perform replace using compiled regex and then full match
  // using compiled regex
  cre2_replace_re(rex, &text_and_target, &rewrite);
  assert(text_and_target.length == 11);
  assert(memcmp(text_and_target.data, "ABC def abc", 11) == 0);
  cre2_full_match_re(rex, &text_and_target, match_full, 2);
  assert(text_and_target.length == 11);
  assert(memcmp(text_and_target.data, "ABC def abc", 11) == 0);
  // step 5: Cleanup
  cre2_delete(rex);
  assert(text_and_target.length == 11);
  assert(memcmp(text_and_target.data, "ABC def abc", 11) == 0);
  cre2_opt_delete(opt);
  assert(text_and_target.length == 11);
  assert(memcmp(text_and_target.data, "ABC def abc", 11) == 0);
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
