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
  cJSON *root = cJSON_CreateArray();
  cJSON *obj = cJSON_CreateObject();
  cJSON *nameItem = cJSON_CreateString("example_name");
  cJSON *falseItem = cJSON_CreateFalse();
  assert(root != NULL);
  assert(root->type == cJSON_Array);
  assert(root->child == NULL);
  assert(root->string == NULL);
  assert(root->next == NULL);
  assert(root->prev == NULL);
  assert(obj != NULL);
  assert(obj->type == cJSON_Object);
  assert(obj->child == NULL);
  assert(obj->string == NULL);
  assert(obj->next == NULL);
  assert(obj->prev == NULL);
  assert(nameItem != NULL);
  assert(nameItem->type == cJSON_String);
  assert(nameItem->valuestring != NULL);
  assert(strcmp(nameItem->valuestring, "example_name") == 0);
  assert(nameItem->child == NULL);
  assert(nameItem->string == NULL);
  assert(nameItem->next == NULL);
  assert(nameItem->prev == NULL);
  assert(falseItem != NULL);
  assert(falseItem->type == cJSON_False);
  assert(falseItem->child == NULL);
  assert(falseItem->string == NULL);
  assert(falseItem->next == NULL);
  assert(falseItem->prev == NULL);
  // step 2: Setup (Configure)
  cJSON_bool add_name_ok = cJSON_AddItemToObjectCS(obj, "name", nameItem);
  assert(add_name_ok != 0);
  assert(obj->child == nameItem);
  // In optimized cJSON, the first child's prev pointer points to the last child
  // (itself in this case)
  assert(nameItem->prev == nameItem);
  assert(nameItem->next == NULL);
  assert(nameItem->string != NULL);
  assert(strcmp(nameItem->string, "name") == 0);
  cJSON_bool add_flag_ok = cJSON_AddItemToObjectCS(obj, "active", falseItem);
  assert(add_flag_ok != 0);
  assert(obj->child == nameItem);
  assert(nameItem->next == falseItem);
  // After adding second item, head->prev points to the new tail (falseItem)
  assert(nameItem->prev == falseItem);
  assert(falseItem->prev == nameItem);
  assert(falseItem->next == NULL);
  assert(falseItem->string != NULL);
  assert(strcmp(falseItem->string, "active") == 0);
  (void)add_name_ok;
  (void)add_flag_ok;
  cJSON_AddItemToArray(root, obj);
  assert(root->child == obj);
  // Similarly for the array, root->child (obj) prev points to tail (obj)
  assert(obj->prev == obj);
  assert(obj->next == NULL);
  // step 3: Operate and Validate (Operate -> Validate)
  char *printed = cJSON_PrintUnformatted(root);
  assert(printed != NULL);
  assert(strlen(printed) > 0);
  // Verify that the printed string represents a JSON array containing the
  // expected object
  assert(printed[0] == '[');
  assert(printed[strlen(printed) - 1] == ']');
  assert(strstr(printed, "{") != NULL);
  assert(strstr(printed, "}") != NULL);
  // Validate that the keys added in the Setup step are present in the output
  assert(strstr(printed, "\"name\":") != NULL);
  assert(strstr(printed, "\"active\":") != NULL);
  // Validate that the falseItem value is represented correctly as "false"
  assert(strstr(printed, "false") != NULL);
  // Ensure printing did not modify the original structure
  assert(root->child == obj);
  assert(obj->child == nameItem);
  cJSON_bool false_check = cJSON_IsFalse(falseItem);
  assert(false_check == 1);
  // Verify internal consistency of the item type
  assert((falseItem->type & 0xFF) == cJSON_False);
  // Verify API contract: cJSON_IsFalse returns 0 for NULL input
  assert(cJSON_IsFalse(NULL) == 0);
  // Verify API contract: cJSON_IsFalse returns 0 for non-boolean items (root is
  // an Array)
  assert(cJSON_IsFalse(root) == 0);
  (void)false_check;
  (void)printed;
  // step 4: Cleanup
  cJSON_free(printed);
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
