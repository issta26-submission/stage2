// Header
#include <cJSON.h>
#include <cassert>
#include <cstddef>
#include <iostream>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
int test_api_sequence() {
  // step 1: Declarations (Initialize)
  cJSON *root = cJSON_CreateArray();
  cJSON *obj = cJSON_CreateObject();
  cJSON *nameItem = cJSON_CreateString("example");
  cJSON *numItem = cJSON_CreateNumber(123.0);
  cJSON *flagItem = cJSON_CreateFalse();
  assert(root != NULL);
  assert(root->type == cJSON_Array);
  assert(root->child == NULL);
  assert(root->next == NULL);
  assert(root->prev == NULL);
  assert(root->string == NULL);
  assert(obj != NULL);
  assert(obj->type == cJSON_Object);
  assert(obj->child == NULL);
  assert(obj->next == NULL);
  assert(obj->prev == NULL);
  assert(obj->string == NULL);
  assert(nameItem != NULL);
  assert(nameItem->type == cJSON_String);
  assert(nameItem->valuestring != NULL);
  assert(strcmp(nameItem->valuestring, "example") == 0);
  assert(nameItem->child == NULL);
  assert(nameItem->next == NULL);
  assert(nameItem->prev == NULL);
  assert(nameItem->string == NULL);
  assert(numItem != NULL);
  assert(numItem->type == cJSON_Number);
  assert(numItem->valuedouble == 123.0);
  assert(numItem->valueint == 123);
  assert(numItem->child == NULL);
  assert(numItem->next == NULL);
  assert(numItem->prev == NULL);
  assert(numItem->string == NULL);
  assert(flagItem != NULL);
  assert(flagItem->type == cJSON_False);
  assert(flagItem->child == NULL);
  assert(flagItem->next == NULL);
  assert(flagItem->prev == NULL);
  assert(flagItem->string == NULL);
  // step 2: Setup (Configure)
  cJSON_AddItemToObjectCS(obj, "name", nameItem);
  assert(obj->child == nameItem);
  assert(nameItem->next == NULL);
  assert(nameItem->string != NULL);
  assert(strcmp(nameItem->string, "name") == 0);
  cJSON_AddItemToObjectCS(obj, "value", numItem);
  assert(nameItem->next == numItem);
  assert(numItem->prev == nameItem);
  assert(numItem->next == NULL);
  assert(numItem->string != NULL);
  assert(strcmp(numItem->string, "value") == 0);
  cJSON_AddItemToObjectCS(obj, "flag", flagItem);
  assert(numItem->next == flagItem);
  assert(flagItem->prev == numItem);
  assert(flagItem->next == NULL);
  assert(flagItem->string != NULL);
  assert(strcmp(flagItem->string, "flag") == 0);
  cJSON_AddItemToArray(root, obj);
  assert(root->child == obj);
  assert(obj->next == NULL);
  // step 3: Operate and Validate (Operate -> Validate)
  char *printed = cJSON_PrintUnformatted(root);
  assert(printed != NULL);
  assert(strlen(printed) > 2); /* Must contain at least brackets and content */
  assert(printed[0] == '[');   /* root is an Array, so output starts with '[' */
  assert(printed[strlen(printed) - 1] == ']'); /* Output ends with ']' */
  /* Verify that the unformatted string contains the keys added in previous
   * steps */
  assert(strstr(printed, "\"name\"") != NULL);
  assert(strstr(printed, "\"value\"") != NULL);
  assert(strstr(printed, "\"flag\"") != NULL);
  /* Verify that the print operation did not mutate the root structure */
  assert(root->child == obj);
  assert(obj->next == NULL);
  cJSON_bool flag_is_false = cJSON_IsFalse(flagItem);
  assert(flagItem != NULL); /* flagItem was previously added to obj */
  assert(flag_is_false == 0 ||
         flag_is_false == 1); /* Return value must be boolean */
  /* Verify API contract: return value matches the internal type state */
  assert(flag_is_false == ((flagItem->type & 0xFF) == cJSON_False));
  /* Verify that the read-only check did not alter pointers */
  assert(flagItem->prev == numItem);
  assert(flagItem->next == NULL);
  (void)flag_is_false;
  (void)printed;
  // step 4: Cleanup
  assert(printed != NULL);
  cJSON_free(printed);
  assert(root != NULL);
  assert(root->child == obj);
  assert(obj->next == NULL);
  assert(obj->child != NULL);
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
