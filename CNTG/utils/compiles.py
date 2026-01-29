import os
import subprocess
def clean_response(response: str) -> str:
    response = response.strip()
    if response.startswith("```cpp"):
        response = response[len("```cpp"):].strip()
    if response.endswith("```"):
        response = response[:-3].strip()
    return response

def assemble_program(code:str,headers: str, response: str) -> str:
    code= clean_response(code)
    response = clean_response(response)
    program = f"""{headers}
    #include <cassert>
    #include<cstring>
    int test_api_sequence() {{
    {code}
    {response}
        return 66;
    }}

    int main() {{
        for (int i = 0; i < 10; i++) {{
            int result = test_api_sequence();
            if (result != 66) {{
                std::cerr << "API sequence test failed with error code: " << result << std::endl;
                exit(100);
            }}
        }}
        printf("Test completed successfully.\\n");
        return 0;
    }}
    """
    return program,code+response


def compile_and_run_cpp_code(code_str,lib_name):
    cpp_file = "1.cc"
    exe_file = "a"

    # Get the base directory (parent of CNTG)
    script_dir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    base_dir = os.path.dirname(script_dir)
    
    with open(cpp_file, "w") as f:
        f.write(code_str)

    try:
        # compile
        if lib_name=="zlib":
            compile_cmd = ["g++", cpp_file, "-o", exe_file, "-lz"]
            compile_proc = subprocess.run(compile_cmd, check=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

            # run
            run_cmd = ["./" + exe_file]
            run_proc = subprocess.run(run_cmd, check=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

        elif lib_name=="libpng":
            compile_cmd = ["g++", cpp_file, "-o", exe_file, "-lpng", "-lz"]
            compile_proc = subprocess.run(compile_cmd, check=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

            # run
            run_cmd = ["./" + exe_file]
            run_proc = subprocess.run(run_cmd, check=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        elif lib_name=="cJSON":
            compile_cmd = ["g++", cpp_file, "-o", exe_file, "-I/usr/include/cjson", "-lcjson"]
            compile_proc = subprocess.run(compile_cmd, check=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

            # run
            run_cmd = ["./" + exe_file]
            run_proc = subprocess.run(run_cmd, check=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
            
        elif lib_name=="re2":
            compile_cmd = ["g++", cpp_file, "-o", exe_file, "-std=c++17", "-I/opt/re2/2024-07-02/shared/include", "-L/opt/re2/2024-07-02/shared/lib64", "-lcre2"]
            compile_proc = subprocess.run(compile_cmd, check=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

            # run
            run_cmd = ["bash", "-c", "LD_LIBRARY_PATH=/opt/re2/2024-07-02/shared/lib64 ./a"]
            run_proc = subprocess.run(
                run_cmd,
                check=True,
                stdout=subprocess.PIPE,
                stderr=subprocess.PIPE
            )
        elif lib_name=="lcms":
            lcms_include = os.path.join(base_dir, "output", "build", "lcms", "include")
            lcms_src_include = os.path.join(base_dir, "output", "build", "lcms", "src", "lcms", "include")
            lcms_lib_path = os.path.join(base_dir, "output", "build", "lcms", "src", "lcms", "src", ".libs")
            compile_cmd = [
                "g++",
                cpp_file,
                "-o",
                exe_file,
                f"-I{lcms_include}",
                f"-I{lcms_src_include}",
                f"-L{lcms_lib_path}",
                "-llcms2",
            ]
            compile_proc = subprocess.run(compile_cmd, check=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

            # run
            run_cmd = ["bash", "-c", f"LD_LIBRARY_PATH={lcms_lib_path} ./a"]
            run_proc = subprocess.run(run_cmd, check=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        elif lib_name=="libaom":
            aom_include = os.path.join(base_dir, "aom")
            aom_config_include = os.path.join(base_dir, "aom_build", "config")
            aom_lib = os.path.join(base_dir, "aom_build", "libaom.a")
            compile_cmd = [
                "g++",
                cpp_file,
                "-o",
                exe_file,
                f"-I{aom_include}",
                f"-I{aom_config_include}",
                aom_lib,
            ]
            compile_proc = subprocess.run(compile_cmd, check=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

            # run
            run_cmd = ["./" + exe_file]
            run_proc = subprocess.run(run_cmd, check=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        elif lib_name=="sqlite3":
            compile_cmd = ["g++", cpp_file, "-o", exe_file, "-lsqlite3"]
            compile_proc = subprocess.run(compile_cmd, check=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

            # run
            run_cmd = ["./" + exe_file]
            run_proc = subprocess.run(run_cmd, check=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        elif lib_name=="libpcap":
            libpcap_include = os.path.join(base_dir, "output", "build", "libpcap", "include")
            libpcap_lib = os.path.join(base_dir, "output", "build", "libpcap", "work", "build", "libpcap.a")
            compile_cmd = [
                "g++", 
                cpp_file, 
                "-o", 
                exe_file, 
                f"-I{libpcap_include}",
                libpcap_lib
            ]
            compile_proc = subprocess.run(compile_cmd, check=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

            # run
            run_cmd = ["./" + exe_file]
            run_proc = subprocess.run(run_cmd, check=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        print("Compilation and execution successful.")
        return True, None
    except subprocess.CalledProcessError as e:
        error_msg = e.stderr.decode()
        print("Error during compile or run:")
        print(error_msg)
        return False, error_msg
    finally:
        # clean up
        if os.path.exists(cpp_file):
            os.remove(cpp_file)
        if os.path.exists(exe_file):
            os.remove(exe_file)
