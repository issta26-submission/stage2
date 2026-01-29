
#include <cre2.h>
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <stdlib.h>

// Header
#include <cassert>
int test_api_sequence() {
  // step 1: Declarations
  const char pattern[] = "(test)";
  const int pattern_len = (int)(sizeof(pattern) - 1);
  const char text[] = "this is test and more test";
  const int text_len = (int)(sizeof(text) - 1);
  const char replacement[] = "REPL";
  const int replacement_len = (int)(sizeof(replacement) - 1);
  char buffer[256];
  cre2_options_t *opt = cre2_opt_new();
  cre2_regexp_t *re = NULL;
  cre2_string_t match;
  cre2_string_t text_and_target;
  cre2_string_t rewrite;
  int easy_res = 0;
  int replace_res = 0;
  assert(pattern != NULL);
  assert(pattern_len == sizeof("(test)") - 1);
  assert(pattern[pattern_len] == '\0');
  assert(strlen(pattern) == pattern_len);
  assert(text != NULL);
  assert(text_len == sizeof("this is test and more test") - 1);
  assert(text[text_len] == '\0');
  assert(strlen(text) == text_len);
  assert(replacement != NULL);
  assert(replacement_len == sizeof("REPL") - 1);
  assert(replacement[replacement_len] == '\0');
  assert(strlen(replacement) == replacement_len);
  assert(sizeof(buffer) == 256);
  assert(opt != NULL);
  assert(re == NULL);
  assert(easy_res == 0);
  assert(replace_res == 0);
  // step 2: Setup / Configure options
  memset(&match, 0, sizeof(match));
  assert(match.data == NULL);
  assert(match.length == 0);
  memset(&text_and_target, 0, sizeof(text_and_target));
  assert(text_and_target.data == NULL);
  assert(text_and_target.length == 0);
  memset(&rewrite, 0, sizeof(rewrite));
  assert(rewrite.data == NULL);
  assert(rewrite.length == 0);
  memset(buffer, 0, sizeof(buffer));
  assert(buffer[0] == '\0');
  assert(buffer[sizeof(buffer) - 1] == '\0');
  memcpy(buffer, text, (size_t)text_len);
  assert(memcmp(buffer, text, text_len) == 0);
  assert(buffer[text_len] == '\0');
  text_and_target.data = buffer;
  assert(text_and_target.data != NULL);
  assert(text_and_target.data == &buffer[0]);
  text_and_target.length = text_len;
  assert(text_and_target.length == (int)strlen(text));
  assert(memcmp(text_and_target.data, text, text_and_target.length) == 0);
  cre2_opt_set_case_sensitive(opt, 1);
  // step 3: Operate (compile the pattern with configured options)
  re = cre2_new(pattern, pattern_len, opt);
  assert(re != NULL);
  assert(cre2_error_code(re) == 0);
  assert(strcmp(cre2_pattern(re), pattern) == 0);
  // step 4: Operate (perform an easy match to capture first occurrence)
  easy_res = cre2_easy_match(pattern, pattern_len, buffer, text_len, &match, 1);
  assert(easy_res == 1);
  assert(match.data != NULL);
  assert(match.length > 0);
  assert(match.data >= buffer);
  assert(match.data + match.length <= buffer + text_len);
  // step 5: Operate (prepare rewrite and perform global replace)
  rewrite.data = replacement;
  rewrite.length = replacement_len;
  replace_res = cre2_global_replace(pattern, &text_and_target, &rewrite);
  assert(replace_res >= easy_res);
  assert(text_and_target.data != NULL);
  // step 6: Cleanup
  cre2_delete(re);
  cre2_opt_delete(opt);
  assert(replace_res >= easy_res);
  assert(text_and_target.data != NULL);
  assert(rewrite.data == replacement);
  assert(rewrite.length == replacement_len);
  (void)easy_res;
  (void)replace_res;
  (void)match;
  (void)text_and_target;
  (void)rewrite;
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
