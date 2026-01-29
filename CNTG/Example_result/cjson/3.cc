// Header
#include <cJSON.h>
#include <cassert>
#include <cstddef>
#include <cstdio> // Required for printf
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
  cJSON *root = cJSON_CreateObject();
  cJSON *arr = cJSON_CreateArray();
  cJSON *str_item = cJSON_CreateString("example");
  cJSON *false_item = cJSON_CreateFalse();
  char *json_text = NULL;
  cJSON_bool is_str = 0;
  cJSON_bool is_false = 0;
  assert(root != NULL);
  assert(root->type == cJSON_Object);
  assert(root->child == NULL);
  assert(root->string == NULL);
  assert(arr != NULL);
  assert(arr->type == cJSON_Array);
  assert(arr->child == NULL);
  assert(arr->string == NULL);
  assert(str_item != NULL);
  assert(str_item->type == cJSON_String);
  assert(str_item->valuestring != NULL);
  assert(strcmp(str_item->valuestring, "example") == 0);
  assert(false_item != NULL);
  assert(false_item->type == cJSON_False);
  assert(json_text == NULL);
  assert(is_str == 0);
  assert(is_false == 0);
  // step 2: Setup (Configure)
  cJSON_AddItemToObject(root, "items", arr);
  assert(root->child == arr);
  assert(arr->string != NULL);
  assert(strcmp(arr->string, "items") == 0);
  assert(arr->next == NULL);
  cJSON_AddItemToArray(arr, str_item);
  assert(arr->child == str_item);
  assert(str_item->string == NULL);
  assert(str_item->next == NULL);
  cJSON_AddNumberToObject(root, "count", 1.0);
  assert(arr->next != NULL);
  assert(arr->next->type == cJSON_Number);
  assert(arr->next->valuedouble == 1.0);
  assert(arr->next->string != NULL);
  assert(strcmp(arr->next->string, "count") == 0);
  cJSON_AddItemToObject(root, "flag", false_item);
  assert(arr->next->next == false_item);
  assert(false_item->type == cJSON_False);
  assert(false_item->string != NULL);
  assert(strcmp(false_item->string, "flag") == 0);
  assert(false_item->next == NULL);
  // step 3: Operate -> Validate
  json_text = cJSON_PrintUnformatted(root);
  assert(json_text != NULL);
  assert(strlen(json_text) > 0);
  assert(json_text[0] == '{');
  assert(json_text[strlen(json_text) - 1] == '}');
  assert(strstr(json_text, "\"items\":") != NULL);
  assert(strstr(json_text, "\"count\":1") != NULL);
  assert(strstr(json_text, "\"flag\":false") != NULL);
  is_str = cJSON_IsString(str_item);
  assert(str_item != NULL);
  assert(str_item->type == cJSON_String);
  assert(is_str == 1);
  is_false = cJSON_IsFalse(false_item);
  assert(false_item != NULL);
  assert(false_item->type == cJSON_False);
  assert(is_false == 1);
  (void)is_str;
  (void)is_false;
  (void)json_text;
  // step 4: Cleanup
  assert(json_text != NULL);
  cJSON_free(json_text);
  assert(root != NULL);
  cJSON_Delete(root);
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
