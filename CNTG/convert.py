import os
import time
import shutil
import re
from pathlib import Path
from typing import Tuple
from utils.llm_provider import generate_text

def add_io_debugging(code: str) -> Tuple[str, float]:
    start_time = time.time()

    prompt = f"""
You are a C/C++ system programming expert. When generating code, strictly follow these rules:

1. If there are any if branches, delete all if statements inside the function test_api_sequence (the logic is already guaranteed to be correct, no conditional checks or closing operations are needed).
2. Keep the original logic unchanged, but replace off64_t with off_t.
3. Remove any duplicate definitions.
4. header cstddef needs to add at the top of the code.
5. The output format must strictly follow:
    // Header
    int test__api_sequence() {{

        // Step 1: Declarations / Initialize
        
        // Step 2: Configure
        
        // Step 3: Operate / Validate
        
        // Step 4: Cleanup
        
        
        }}
    int main() {{
        for (int i = 0; i < 10; i++) {{
            int result = test_api_sequence();
            if (result != 66) {{
                std::cerr << "API sequence test failed with error code: " << result << std::endl;
            }}
        }}
        printf("Test completed successfully.\n");
        return 0;
    }}

Output only the complete code, do not add extra explanations or any additional content.

Here is the code that needs to be processed:
{code}
"""

    try:
        response_text = generate_text(
            prompt,
            gemini_model="gemini-2.5-flash",
            openai_model=os.getenv("OPENAI_MODEL") or "gpt-4.1-mini"
        )
        elapsed = time.time() - start_time
        modified_code = (response_text or "").strip()

        if modified_code.startswith("```"):
            first_newline = modified_code.find("\n")
            if first_newline != -1:
                modified_code = modified_code[first_newline:].strip()
        if modified_code.endswith("```"):
            modified_code = modified_code[:-3].strip()

        return modified_code, elapsed

    except Exception as e:
        print(f"\Io debug false: {str(e)}")
        return code, time.time() - start_time


def extract_function_calls(code: str) -> list:
    pattern = r'\b([a-zA-Z_]\w*)\s*\('
    matches = re.findall(pattern, code)
    keywords = {
        'if', 'for', 'while', 'switch', 'return', 'sizeof',
        'printf', 'fprintf', 'malloc', 'free','strlen','test_zlib_api_sequence','main','CERR_PRINTF','writing'
    }

    filtered = [fn for fn in matches if fn not in keywords]
    return filtered


def extract_api_pairs(calls: list) -> list:
    return [(calls[i], calls[i + 1]) for i in range(len(calls) - 1)]


if __name__ == "__main__":
    input_path = Path("Original_Seed")
    output_path = Path("modified_seed")

    print(f"current path: {input_path}")
    if not input_path.exists():
        print(f"path: {input_path} unexists, exit.")
        exit(1)


    for i in range(0, 100):  # change range as needed
        file = input_path / f"{i}.cc"
        if not file.exists():
            print(f"file does not exist: {file.name}")
            continue

        with open(file, 'r', encoding='utf-8') as f:
            code = f.read()

        print(f"\ndeal with file: {file.name}")
        modified_code, elapsed_time = add_io_debugging(code)

        if not modified_code or modified_code.strip() == code.strip():
            print(f"just unmodified file: {file.name}")
            continue

        output_file = output_path / file.name
        with open(output_file, 'w', encoding='utf-8') as f:
            f.write(modified_code)


        print(f"✅ done!: {file.name}, time: {elapsed_time:.2f}秒")
