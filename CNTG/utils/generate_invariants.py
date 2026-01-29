import os
import time
import re
import utils.code_extractor as extractor
from utils import compiles
from utils import api_document
from utils import save_seed
from utils.llm_provider import generate_text
def extract_and_format_api_docs(code,lib_name):
    """
    Extract zlib APIs from code and format documentation for prompt
    """
    try:
        if lib_name=="zlib":
        # Extract APIs used in the code
            zlib_apis = api_document.library_list("zlib")
            used_apis = api_document.extract_api_calls(code, zlib_apis)
            
            if not used_apis:
                return ""
            
            # Load API documentation
            script_dir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
            docs = api_document.load_api_docs(os.path.join(script_dir, "library", "zlib.yaml"))
            
            # Format documentation for prompt
            api_docs_text = "\n\nRelevant zlib API Documentation:\n"
            api_docs_text += "="*50 + "\n"
            
            for api in used_apis:
                doc = api_document.get_api_doc(api, docs)
                if doc:
                    api_docs_text += api_document.format_api_doc_for_gpt(api, doc)
                    api_docs_text += "-" * 30 + "\n"
                    
        #  print(f"api docs extracted for APIs: {', '.join(used_apis)}")        
            return api_docs_text
        elif lib_name=="re2":
            # Extract APIs used in the code
            re2_apis = api_document.library_list("re2")
            used_apis = api_document.extract_api_calls(code, re2_apis)
            
            if not used_apis:
                return ""
            
            # Load API documentation
            script_dir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
            docs = api_document.load_api_docs(os.path.join(script_dir, "library", "re2.yaml"))
            
            # Format documentation for prompt
            api_docs_text = "\n\nRelevant re2 API Documentation:\n"
            api_docs_text += "="*50 + "\n"
            
            for api in used_apis:
                doc = api_document.get_api_doc(api, docs)
                if doc:
                    api_docs_text += api_document.format_api_doc_for_gpt(api, doc)
                    api_docs_text += "-" * 30 + "\n"
            return api_docs_text
        

        elif lib_name=="libpng":
            # Extract APIs used in the code
            libpng_apis = api_document.library_list("libpng")
          #  print("libpng_apis:{}",format(libpng_apis))
        #   print("code:{}",format(code))
            used_apis = api_document.extract_api_calls(code, libpng_apis)
            
            if not used_apis:
                return ""
            
            # Load API documentation
            script_dir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
            docs = api_document.load_api_docs(os.path.join(script_dir, "library", "libpng.yaml"))
            print("used_apis:{}",format(used_apis))
            # Format documentation for prompt
            api_docs_text = "\n\nRelevant libpng API Documentation:\n"
            api_docs_text += "="*50 + "\n"
            
            for api in used_apis:
                doc = api_document.get_api_doc(api, docs)
                if doc:
                    api_docs_text += api_document.format_api_doc_for_gpt(api, doc)
                    api_docs_text += "-" * 30 + "\n"
            
            return api_docs_text

        
        elif lib_name=="cJSON":
            # Extract APIs used in the code
            libpng_apis = api_document.library_list("cJSON")
          #  print("libpng_apis:{}",format(libpng_apis))
        #   print("code:{}",format(code))
            used_apis = api_document.extract_api_calls(code, libpng_apis)
            
            if not used_apis:
                return ""
            
            # Load API documentation
            script_dir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
            docs = api_document.load_api_docs(os.path.join(script_dir, "library", "cJSON.yaml"))
            print("used_apis:{}",format(used_apis))
            # Format documentation for prompt
            api_docs_text = "\n\nRelevant cjson API Documentation:\n"
            api_docs_text += "="*50 + "\n"
            
            for api in used_apis:
                doc = api_document.get_api_doc(api, docs)
                if doc:
                    api_docs_text += api_document.format_api_doc_for_gpt(api, doc)
                    api_docs_text += "-" * 30 + "\n"
            
            return api_docs_text
        
        elif lib_name=="lcms":
            # Extract APIs used in the code
            libpng_apis = api_document.library_list("lcms")
          #  print("libpng_apis:{}",format(libpng_apis))
        #   print("code:{}",format(code))
            used_apis = api_document.extract_api_calls(code, libpng_apis)
            
            if not used_apis:
                return ""
            
            # Load API documentation
            script_dir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
            docs = api_document.load_api_docs(os.path.join(script_dir, "library", "lcms.yaml"))
            print("used_apis:{}",format(used_apis))
            # Format documentation for prompt
            api_docs_text = "\n\nRelevant lcms API Documentation:\n"
            api_docs_text += "="*50 + "\n"
            
            for api in used_apis:
                doc = api_document.get_api_doc(api, docs)
                if doc:
                    api_docs_text += api_document.format_api_doc_for_gpt(api, doc)
                    api_docs_text += "-" * 30 + "\n"
            
            return api_docs_text

        elif lib_name=="libaom":
            # Extract APIs used in the code
            libaom_apis = api_document.library_list("libaom")
            used_apis = api_document.extract_api_calls(code, libaom_apis)

            if not used_apis:
                return ""

            # Load API documentation
            script_dir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
            docs = api_document.load_api_docs(os.path.join(script_dir, "library", "libaom.yaml"))
            print("used_apis:{}",format(used_apis))
            # Format documentation for prompt
            api_docs_text = "\n\nRelevant libaom API Documentation:\n"
            api_docs_text += "="*50 + "\n"

            for api in used_apis:
                doc = api_document.get_api_doc(api, docs)
                if doc:
                    api_docs_text += api_document.format_api_doc_for_gpt(api, doc)
                    api_docs_text += "-" * 30 + "\n"

            return api_docs_text

        elif lib_name=="sqlite3":
            # Extract APIs used in the code
            sqlite3_apis = api_document.library_list("sqlite3")
            used_apis = api_document.extract_api_calls(code, sqlite3_apis)

            if not used_apis:
                return ""

            # Load API documentation
            script_dir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
            docs = api_document.load_api_docs(os.path.join(script_dir, "library", "sqlite3.yaml"))
            print("used_apis:{}".format(used_apis))
            # Format documentation for prompt
            api_docs_text = "\n\nRelevant sqlite3 API Documentation:\n"
            api_docs_text += "=" * 50 + "\n"

            for api in used_apis:
                doc = api_document.get_api_doc(api, docs)
                if doc:
                    api_docs_text += api_document.format_api_doc_for_gpt(api, doc)
                    api_docs_text += "-" * 30 + "\n"

            return api_docs_text

        elif lib_name=="libpcap":
            # Extract APIs used in the code
            libpcap_apis = api_document.library_list("libpcap")
            used_apis = api_document.extract_api_calls(code, libpcap_apis)

            if not used_apis:
                return ""

            # Load API documentation
            script_dir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
            docs = api_document.load_api_docs(os.path.join(script_dir, "library", "libpcap.yaml"))
            print("used_apis:{}".format(used_apis))
            # Format documentation for prompt
            api_docs_text = "\n\nRelevant libpcap API Documentation:\n"
            api_docs_text += "=" * 50 + "\n"

            for api in used_apis:
                doc = api_document.get_api_doc(api, docs)
                if doc:
                    api_docs_text += api_document.format_api_doc_for_gpt(api, doc)
                    api_docs_text += "-" * 30 + "\n"

            return api_docs_text
        
    except Exception as e:
        print(f"Warning: Could not load API documentation: {e}")
        return ""

def clean_response(text):
    return text.replace("```c++","").replace("```cpp","").replace("```","").strip()


def generate_invariants_step_first(block,lib_name):
    code= block['content']
    """
    Use configured LLM to generate the first block's invariants for a given C++ code block.
    Args:
        block: A dictionary containing the firstcode block and its metadata.
    Returns:
        invariants: Generated invariants as C++ assert statements.
    """
    
    # Extract and format API documentation
    api_docs_text = extract_and_format_api_docs(code,lib_name)
   # print(f"api docs text length: {len(api_docs_text)}")
    prompt_begin = f"""
    You are an expert C/C++ programmer with deep knowledge of program invariants.
    Your task is to generate high-quality, meaningful invariants for a given snippet of C++ code.
    For each important operation, generate assert statements that:
    - Prefer contracts and behaviors described in api_docs_text when available; translate them into concrete assertions
    - Check not only return values, but also the state of key variables, buffers, and structs.
    - Cover boundary conditions, pointer validity, buffer contents, and any observable side effects.
    - Use only variables and objects that are defined in the code.
    - Do not invent new variables or functions.
    - Do not create any branches or conditional checks.
    - Output only assert(...) statements, no extra text or comments.
    
    code:
    {code}
    {api_docs_text}
    """
    start_time = time.time()
    try:
        response_text = generate_text(
            prompt_begin,
            gemini_model="gemini-3-pro-preview",
            openai_model=os.getenv("OPENAI_MODEL") or "gpt-4.1"
        )
        if not response_text:
            print("\ngenerate failure: API returned empty response")
            return ""
        invariants = clean_response(response_text)
        elapsed_time=time.time()-start_time
        print(f"time: {elapsed_time:.2f} seconds")
        return invariants
    except Exception as e:
        print(f"\ngenerate failure: {str(e)}")
        return ""

def generate_invariants_next_step(block,program,lib_name):
    """
    Generate invariants for the next step in the sequence of C++ code blocks.
    Args:
        block: code block 
        program: the program state after running the previous code block.
    Returns:
        invariants: Generated invariants as C++ assert statements.
    """
    code= block['content']
    
    # Extract and format API documentation
    api_docs_text = extract_and_format_api_docs(code,lib_name)
    #print("api docs:{}",format(api_docs_text))
    prompt = f"""
    You are an expert C/C++ programmer specializing in formal verification.

    Your task is to insert `assert(...)` statements into the given C/C++ `code` snippet. These assertions must represent deep semantic invariants that are logically true at specific points, based on the `previous program state` and the provided `api_docs_text` (when present, prefer translating its contracts into assertions).
    IMPORTANT INSTRUCTIONS:
    1. Only output the current code block with assert statements inserted
    2. Do not add any #include statements, comments, or explanations
    3. Do not redeclare any variables that are already declared in previous blocks
    4. Do not create new functions or main() function
    5. Keep the original code structure intact
    6. Do not create any branches or conditional checks
    Instructions for assertions:

    1.  Go Beyond Return Values: Your primary focus is on validating internal state, not just API return codes. Assert changes in `z_stream` fields (`avail_in`, `total_out`, `state`, etc.), buffer contents, and output parameters.
    
    2.  Assert Functional Correctness: Verify the logical outcomes of operations. This includes:
        Data Identity: Use `memcmp(...) == 0` to confirm that a compression-decompression cycle restores the original data.
        State Consistency: Assert that counters and pointers are updated correctly and consistently after each operation.
        API Contracts: Directly translate guarantees from the `api_docs_text` into assertions (e.g., `deflateReset` must reset `total_in` to 0), but not limited to them.
        
    3.  Strict Context Adherence:
        Only assert on variables and fields that have been initialized.
        Only use variables and types already present in the `code` and `previous program state`. Do not declare new ones.
        Avoid common C errors like variable redeclaration.
        
    4.  Output Format:
        Insert assertions after the relevant lines of code, not just at the end.
        Output only the current code block with assertions, not the entire program.
    
    5. Do not introduce any new variables or functions. Use only those defined in the `code` and `previous program state`.

    code:
    {code}

    previous program state:
    {program}

    api_docs_text:
    {api_docs_text}
    """
    # print("code:    {}".format(code))
    # print(f"program: {program}")
    # print(f"Processing block - code length: {len(code)}")
    # print(f"Previous program length: {len(program)}")
    start_time = time.time()
    try:
        response_text = generate_text(
            prompt,
            gemini_model="gemini-3-pro-preview",
            openai_model=os.getenv("OPENAI_MODEL") or "gpt-4.1"
        )
        if not response_text:
            print("\ngenerate failure: API returned empty response")
            return code
        invariants = clean_response(response_text)
        elapsed_time=time.time()-start_time
        print(f"time: {elapsed_time:.2f} seconds")
        return invariants
    except Exception as e:
        print(f"\ngenerate failure: {str(e)}")
        return code

def process_all_blocks(file_path,lib_name):
    try:
        blocks, headers = extractor.split_cpp_by_step(file_path)
        if not blocks:
            print("No blocks found in the file.")
            return False, None

        print(f"Found {len(blocks)} blocks to process.")

        # first block
        print(f"Processing block 0...")
        response = generate_invariants_step_first(blocks[0],lib_name)
        code = blocks[0]['content']
        generated_assertions = clean_response(response)
        
        def filter_invalid_lines(text):
            # Remove lines that are just a single 'c' or empty/whitespace
            return '\n'.join([line for line in text.splitlines() if line.strip() and line.strip() != 'c'])

        block0_code = filter_invalid_lines(code + '\n' + generated_assertions)
        all_code = [block0_code]
        cumulative_code = [block0_code]
        
        # Store the complete previous block (code + assertions) for next iteration
        previous_complete_block = block0_code
        
        assemble_programs, _ = compiles.assemble_program('\n'.join(cumulative_code), headers, '')
        for attempt in range(5):
            compile_success, compile_error = compiles.compile_and_run_cpp_code(assemble_programs,lib_name)
            if compile_success:
                break
            print(f"Block 0 compile failed (attempt {attempt + 1}/5). Retrying with regenerated assertions...")
            print(f"Error during compile or run:\n{compile_error}")
            
            # Check if this is an assertion failure
            is_assertion_error = "Assertion" in compile_error and "failed" in compile_error
            
            retry_prompt = f'''
            You are a C/C++ assertion generation expert. Please regenerate more reasonable assert statements based on the following C++ code snippet and error message.
            
            IMPORTANT INSTRUCTIONS:
            1. Only output assert(...) statements, no other content
            2. Do not add any #include statements or comments
            3. Do not redeclare any variables
            4. Use only variables that are already declared in the code
            5. **CRITICAL FOR ASSERTION FAILURES**: If the error message shows an assertion failed (like "Assertion `X` failed"), it means the assertion is checking for an incorrect condition. You need to:
               - Analyze why the assertion failed based on the actual runtime behavior
               - Either REMOVE the incorrect assertion entirely
               - Or RELAX the assertion to match the actual valid behavior
               - Or CORRECT the assertion if you understand the actual invariant that should hold
               - For example, if `memcmp(buffer, expected, 8) == 0` fails, the buffer content might be different - either remove this assertion or check for a different pattern
            6. Do not introduce branches or conditional checks.
            Previous assertions (that caused the error):
            {generated_assertions}
            
            Original code:
            {code}
            
            Error message:
            {compile_error}
            
            Output only the assert statements that should be added after the code. {"Make sure to address the assertion failure by removing or fixing the problematic assertion." if is_assertion_error else ""}
            '''
            try:
                retry_text = generate_text(
                    retry_prompt,
                    gemini_model="gemini-3-pro-preview",
                    openai_model=os.getenv("OPENAI_MODEL") or "gpt-4.1"
                )
                generated_assertions = clean_response((retry_text or "").strip())
            except Exception as e:
                print(f"Block 0 regeneration failed on attempt {attempt + 1}/5: {e}")
                generated_assertions = ""
            block0_code = filter_invalid_lines(code + '\n' + generated_assertions)
            all_code[0] = block0_code
            cumulative_code[0] = block0_code
            previous_complete_block = block0_code  # Update the complete block
            assemble_programs, _ = compiles.assemble_program('\n'.join(cumulative_code), headers, '')
        else:
            return False, None

        # remaining blocks
        for i in range(1, len(blocks)):
            print(f"Processing block {i}...")
            current_block = blocks[i].copy()
            if i == len(blocks) - 1:
                print("Processing final block - removing duplicate return statement...")
                block_content = current_block['content']
                lines = block_content.split('\n')
                filtered_lines = [line for line in lines if not re.search(r'\breturn\s+66\b', line)]
                current_block['content'] = '\n'.join(filtered_lines)
                print(f"Final block content after cleanup:\n{current_block['content']}")
            
            # Pass the complete previous block (code + assertions) as context
            next_invariants = generate_invariants_next_step(current_block, previous_complete_block,lib_name)
            block_code_raw = clean_response(next_invariants)  # LLM now returns complete code with assertions
            block_code = filter_invalid_lines(block_code_raw)
            all_code.append(block_code)
            cumulative_code.append(block_code)
            # print(f"Generated code for block {i}:\n{block_code}")
            # print(f"Block {i} code length: {len(block_code)}")
            # Compile cumulative code up to current block
            assemble_programs, _ = compiles.assemble_program('\n'.join(cumulative_code), headers, '')
            count=0
            while count<5:
                compile_success, compile_error = compiles.compile_and_run_cpp_code(assemble_programs,lib_name)
                if compile_success:
                    # Update previous_complete_block to current processed block
                    previous_complete_block = block_code
                    count=0
                    break
                print(f"Block {i} compile failed (attempt {count + 1}/5). Retrying with regenerated assertions...")
                print(f"Error during compile or run:\n{compile_error}")
                error_type = save_seed.classify_error(compile_error)
                if count==4:
                    save_seed.save_error_program(file_path, error_type, compile_error, assemble_programs, output_dir="error_programs")

                # Check if this is an assertion failure
                is_assertion_error = "Assertion" in compile_error and "failed" in compile_error
                
                retry_prompt = f'''
                You are a C/C++ assertion generation expert. Please regenerate the complete code block with assert statements based on the following C++ code snippet and error message. 
                
                IMPORTANT INSTRUCTIONS:
                1. Only output the code block for the current step, do not add any #include statements
                2. Do not redeclare any variables - use only the variables that are already declared in the function
                3. Do not create new functions or main() function
                4. Only insert assert statements after relevant lines of code
                5. Keep the original code structure intact
                6. **CRITICAL FOR ASSERTION FAILURES**: If the error message shows an assertion failed (like "Assertion `X` failed"), it means the assertion is checking for an incorrect condition. You need to:
                   - Analyze why the assertion failed based on the actual runtime behavior
                   - Either REMOVE the incorrect assertion entirely
                   - Or RELAX the assertion to match the actual valid behavior (e.g., if `assert(x == 10)` fails but x is 9, change to `assert(x >= 0 && x <= 10)` if that range is valid)
                   - Or CORRECT the assertion if you understand the actual invariant that should hold
                   - For example, if `memcmp(buffer, expected, 8) == 0` fails, the buffer content might be different - either remove this assertion or check for a different pattern
                7. If no error message is provides, please regard it as a segmentation fault(execution error)
                
                Previous generated code with assertions (that caused the error):
                {block_code}
                
                Original code block (without assertions):
                {current_block['content']}
                
                Previous code block (for context):
                {previous_complete_block}
                
                Error message:
                {compile_error}
                
                Output only the current code block with appropriate assert statements inserted. {"Make sure to address the assertion failure by removing or fixing the problematic assertion." if is_assertion_error else ""}
                '''
                try:
                    retry_text = generate_text(
                        retry_prompt,
                        gemini_model="gemini-3-pro-preview",
                        openai_model=os.getenv("OPENAI_MODEL") or "gpt-4.1"
                    )
                    block_code_raw = clean_response((retry_text or "").strip())
                except Exception as e:
                    print(f"Block {i} regeneration failed on attempt {count + 1}/5: {e}")
                    block_code_raw = current_block['content']
                block_code = filter_invalid_lines(block_code_raw)
                all_code[-1] = block_code
                cumulative_code[-1] = block_code
                assemble_programs, _ = compiles.assemble_program('\n'.join(cumulative_code), headers, '')
                count+=1
            else:
                return False, None

        print("All blocks processed successfully!")
        total_code = headers + '\n#include <cassert>\nint test_api_sequence() {\n' + '\n'.join(all_code) + '\n    return 66;\n}\n\nint main() {\n    for (int i = 0; i < 10; i++) {\n        int result = test_api_sequence();\n        if (result != 66) {\n            std::cerr << "API sequence test failed with error code: " << result << std::endl;\n            exit(100);\n        }\n    }\n    printf("Test completed successfully.\\n");\n    return 0;\n}\n'
        save_seed.save_successful_program(total_code, file_path, output_dir="successful_program")
        return True, total_code

    except Exception as e:
        print(f"Error processing blocks: {str(e)}")
        return False, None
        
# if __name__ == "__main__":
#     time_start = time.time()
#     file_path = "/home/ruoguyang/CNTG/Original_Seed/1.cc"
#     success, final_program = process_all_blocks(file_path)
    
#     if success:
#         print("Program completed successfully!")
#         print("\n" + "="*50)
#         print("FINAL ASSEMBLED PROGRAM:")
#         print("="*50)
#         print(final_program)
#         print("="*50)
#         time_end = time.time()
#         print(f"Total time: {time_end - time_start:.2f} seconds")
#     else:
#         print("Program encountered errors.")