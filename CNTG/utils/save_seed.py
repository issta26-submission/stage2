import os
from datetime import datetime
def save_successful_program(final_program,file_name,output_dir="successful_program"):
    os.makedirs(output_dir,exist_ok=True)
    base_name=os.path.basename(file_name).replace('.cc', '')
    datatime_str = datetime.now().strftime("%Y%m%d_%H%M%S")
    output_file=f"{output_dir}/{base_name}_success_{datatime_str}.cc"
    with open(output_file, 'w') as f:
        f.write(final_program)
    print(f"Program saved successfully to {output_dir}")
    return output_file

def save_error_program(file_name, error_type, error_message, program, output_dir="error_programs"):
    error_dir = f"{output_dir}/{error_type}"
    os.makedirs(error_dir, exist_ok=True)
    base_name = os.path.basename(file_name).replace('.cc', '')
    datatime_str = datetime.now().strftime("%Y%m%d_%H%M%S")
    error_file=f"{error_dir}/{base_name}_error_{datatime_str}.cc"
    with open(error_file, 'w') as f:
        f.write(f"Error Type: {error_type}\n")
        f.write(f"Program: {program}\n")
        f.write(f"Error Message:\n{error_message}\n")
    return error_file

def classify_error(error_message):
    if "Assertion" in error_message:
        return "assertion_error"
    elif "redeclaration" in error_message:
        return "syntax_error"
    elif "not declared" in error_message:
        return "syntax_error"
    elif "no such file or directory" in error_message:
        return "file_not_found"
    else:
        return "other_error"