
// Header
#include <cassert>
#include <cre2.h>
#include <cstddef>  // Added as per rule 4
#include <cstdio>   // For printf in main
#include <iostream> // For std::cerr, std::endl in main
#include <string.h> // For memset, memcpy
int test_api_sequence() {
  // step 1: Declarations
  const char pattern[] = "(test)(ing)?";
  const int pattern_len = (int)(sizeof(pattern) - 1);
  const char text[] = "this is testing and test only";
  const int text_len = (int)(sizeof(text) - 1);
  const char replacement[] = "X";
  const int replacement_len = (int)(sizeof(replacement) - 1);
  cre2_options_t *opt = nullptr; // Replaced NULL with nullptr for C++ style
  cre2_regexp_t *re = nullptr;   // Replaced NULL with nullptr for C++ style
  cre2_string_t match;
  cre2_string_t text_and_target;
  cre2_string_t rewrite;
  char buffer[256];
  int easy_res = 0;
  int replace_res = 0;
  assert(pattern != nullptr);
  assert(pattern_len > 0);
  assert(pattern[pattern_len] == '\0');
  assert(pattern_len == (int)strlen(pattern));
  assert(text != nullptr);
  assert(text_len > 0);
  assert(text[text_len] == '\0');
  assert(text_len == (int)strlen(text));
  assert(replacement != nullptr);
  assert(replacement_len > 0);
  assert(replacement[replacement_len] == '\0');
  assert(replacement_len == (int)strlen(replacement));
  assert(opt == nullptr);
  assert(re == nullptr);
  assert(sizeof(buffer) == 256);
  assert(easy_res == 0);
  assert(replace_res == 0);
  // step 2: Setup (initialize structures and create options)
  memset(&match, 0, sizeof(match));
  assert(match.data == nullptr);
  assert(match.length == 0);
  memset(&text_and_target, 0, sizeof(text_and_target));
  assert(text_and_target.data == nullptr);
  assert(text_and_target.length == 0);
  memset(&rewrite, 0, sizeof(rewrite));
  assert(rewrite.data == nullptr);
  assert(rewrite.length == 0);
  memset(buffer, 0, sizeof(buffer));
  assert(buffer[0] == '\0');
  assert(buffer[sizeof(buffer) - 1] == '\0');
  memcpy(buffer, text, (size_t)text_len);
  assert(memcmp(buffer, text, text_len) == 0);
  assert(buffer[text_len] == '\0');
  text_and_target.data = buffer;
  assert(text_and_target.data == buffer);
  text_and_target.length = text_len;
  assert(text_and_target.length == text_len);
  assert(memcmp(text_and_target.data, text, text_and_target.length) == 0);
  rewrite.data = replacement;
  assert(rewrite.data == replacement);
  rewrite.length = replacement_len;
  assert(rewrite.length == replacement_len);
  assert(memcmp(rewrite.data, replacement, rewrite.length) == 0);
  opt = cre2_opt_new();
  assert(opt != nullptr);
  // step 3: Configure (set case sensitivity on options)
  cre2_opt_set_case_sensitive(opt, 1);
  assert(opt != nullptr);
  re = cre2_new(pattern, pattern_len, opt);
  assert(re != nullptr);
  // step 4: Operate (perform an easy match and a global replace using the
  // pattern)
  easy_res = cre2_easy_match(pattern, pattern_len, text, text_len, &match, 1);
  assert(easy_res == 1);
  assert(match.length == pattern_len);
  assert(memcmp(match.data, pattern, pattern_len) == 0);
  replace_res = cre2_global_replace(pattern, &text_and_target, &rewrite);
  assert(replace_res > 0);
  assert(text_and_target.length == text_len - (replace_res * pattern_len) +
                                       (replace_res * rewrite.length));
  assert(text_and_target.data != text);
  // step 5: Cleanup (release compiled regexp and options)
  assert(re != NULL);
  cre2_delete(re);
  assert(opt != NULL);
  cre2_opt_delete(opt);
  // API sequence test completed successfully
  (void)easy_res;
  (void)replace_res;
  (void)match;
  (void)text_and_target;
  (void)rewrite;
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
