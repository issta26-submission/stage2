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
  // step 1: Declarations & Initialize
  cJSON *root = cJSON_CreateObject();
  cJSON *arr = cJSON_CreateArray();
  cJSON_AddItemToObject(root, "items", arr);
  cJSON *version_item = cJSON_AddNumberToObject(root, "version", 1.23);
  assert(root != NULL);
  assert(arr != NULL);
  assert(version_item != NULL);
  assert(root->child == arr);
  assert(arr->next == version_item);
  assert(version_item->prev == arr);
  assert(version_item->valuedouble == 1.23);
  assert(root->type == cJSON_Object);
  assert(arr->type == cJSON_Array);
  assert(version_item->type == cJSON_Number);
  // step 2: Configure
  cJSON *str_item = cJSON_CreateString("hello");
  assert(str_item != NULL);
  assert(str_item->type == cJSON_String);
  assert(str_item->valuestring != NULL);
  assert(strcmp(str_item->valuestring, "hello") == 0);
  cJSON_AddItemToArray(arr, str_item);
  assert(arr->child == str_item);
  cJSON *false_item = cJSON_CreateFalse();
  assert(false_item != NULL);
  assert(false_item->type == cJSON_False);
  cJSON_AddItemToArray(arr, false_item);
  assert(str_item->next == false_item);
  assert(false_item->prev == str_item);
  assert(false_item->next == NULL);
  // step 3: Operate & Validate
  char *unformatted = cJSON_PrintUnformatted(root);
  assert(unformatted != NULL);
  assert(strlen(unformatted) > 0);
  cJSON *parsed = cJSON_Parse(unformatted);
  assert(parsed != NULL);
  assert(parsed->type == cJSON_Object);
  cJSON *parsed_items = cJSON_GetObjectItem(parsed, "items");
  assert(parsed_items != NULL);
  assert(parsed_items->type == cJSON_Array);
  cJSON *parsed_str = cJSON_GetArrayItem(parsed_items, 0);
  assert(parsed_str != NULL);
  assert(parsed_str->type == cJSON_String);
  assert(parsed_str->valuestring != NULL);
  assert(strcmp(parsed_str->valuestring, "hello") == 0);
  // Verify the array structure (linked list reconstruction)
  assert(parsed_items->child == parsed_str);
  cJSON *parsed_false = cJSON_GetArrayItem(parsed_items, 1);
  assert(parsed_false != NULL);
  assert(parsed_false->type == cJSON_False);
  // Verify list connectivity between reconstructed items
  assert(parsed_str->next == parsed_false);
  assert(parsed_false->prev == parsed_str);
  assert(parsed_false->next == NULL);
  cJSON_bool string_check = cJSON_IsString(parsed_str);
  assert(string_check == 1);
  cJSON_bool false_check = cJSON_IsFalse(parsed_false);
  assert(false_check == 1);
  (void)string_check;
  (void)false_check;
  (void)version_item;
  // step 4: Cleanup
  cJSON_free(unformatted);
  // Validate that the parsed object tree is still intact and separate from root
  assert(parsed != NULL);
  assert(parsed->type == cJSON_Object);
  assert(root != NULL);
  assert(parsed != root);
  cJSON_Delete(parsed);
  // Validate that the root object is still valid before deletion
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
