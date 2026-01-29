import sys
import os
import glob
import time
import argparse
import subprocess
# Add the parent directory to Python path to import utils modules
sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

import utils.code_extractor as extractor
from utils.generate_invariants import process_all_blocks
from utils.compiles import compile_and_run_cpp_code, assemble_program


def main():
    parser = argparse.ArgumentParser(description="Run compile with a specific library")
    parser.add_argument("--lib", required=True, help="Name of the library to use")
    args = parser.parse_args()
    # Get the directory containing this script
    script_dir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    original_seed_dir = os.path.join(script_dir, "modified_seed")
    cc_files = glob.glob(os.path.join(original_seed_dir, "*.cc"))
    cc_files = [
        f for f in cc_files
        if f"{os.path.splitext(os.path.basename(f))[0]}" in map(str, range(0,100))
    ]
    if not cc_files:
        print("No .cc files found in Original_Seed directory.")
        return
    
    print(f"Found {len(cc_files)} .cc files to process:")
    for file in cc_files:
        print(f"  - {os.path.basename(file)}")
    print("="*60)
    
    total_success = 0
    total_failed = 0
    start_time = time.time()
    for file_path in cc_files:
        filename = os.path.basename(file_path)
        print(f"\n{'='*20} Processing {filename} {'='*20}")
        
        try:
            success, final_program = process_all_blocks(file_path,args.lib)
            print("args.lib:",args.lib)
            if success:
                print(f"✅ {filename} - SUCCESS")
                total_success += 1
                print(f"Generated program saved to successful_program/{filename.replace('.cc', '_success.cc')}")
            else:
                print(f"❌ {filename} - FAILED")
                total_failed += 1
                
        except Exception as e:
            print(f"❌ {filename} - ERROR: {str(e)}")
            total_failed += 1
        
        print("-" * 60)
    
    print(f"\n{'='*20} SUMMARY {'='*20}")
    print(f"Total files processed: {len(cc_files)}")
    print(f"Successful: {total_success}")
    print(f"Failed: {total_failed}")
    print(f"Success rate: {total_success/len(cc_files)*100:.1f}%")
    end_time = time.time()
    # with open("/root/CGNTG/CNTG/Original_Seed/1.cc", "r") as f:
    #     code_str = f.read()

    # compile_and_run_cpp_code(code_str,args.lib)
    print(f"Total time taken: {end_time - start_time:.2f} seconds")
if __name__ == "__main__":
    
    main()