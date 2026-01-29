import re
import sys
import os
from pathlib import Path

def split_cpp_by_step(file_path):
    """
    Split C++ file into blocks based on Step comments.
    
    Args:
        file_path: Path to the C++ source file
        
    Returns:
        List of dictionaries with 'step' and 'content' keys
    """
    with open(file_path, 'r', encoding='utf-8') as f:
        content = f.read()
    func_start_match = re.search(r'(int\s+test_\w*api_sequence\s*\(\)\s*\{)', content)
    if not func_start_match:
        print("Warning: Could not find test_*api_sequence function start")
        return "", []

    func_start_pos = func_start_match.start()

    
    headers = content[:func_start_pos].strip()
    #print(f"Headers and declarations:\n{headers}\n{'-'*40}")
    # Find the test function body
    func_match = re.search(r'int\s+test_\w*api_sequence\s*\(\)\s*\{(.*?)\}(?=\s*int\s+main)', content, re.DOTALL)
    if not func_match:
        print("Warning: Could not find test_*api_sequence function")
        return []
    
    func_body = func_match.group(1)
    
    # Split by Step comments
    # Pattern matches variations like "// Step 1:", "// Step 2: Setup", etc.
    step_pattern = re.compile(r'(\s*//\s*Step\s+\d+[:\s].*?)(?=\s*//\s*Step\s+\d+|$)', re.IGNORECASE | re.DOTALL)
    
    blocks = []
    
    # Find all step matches
    matches = list(step_pattern.finditer(func_body))
    
    if not matches:
        # If no steps found, treat entire function body as one block
        blocks.append({
            "step": "complete_function",
            "content": func_body.strip(),
            "step_number": 0
        })
        return blocks
    
    # Extract preamble (everything before first step)
    first_step_start = matches[0].start()
    preamble = func_body[:first_step_start].strip()
    
    if preamble:
        blocks.append({
            "step": "preamble", 
            "content": preamble,
            "step_number": -1
        })
    
    # Extract each step
    for i, match in enumerate(matches):
        step_content = match.group(1).strip()
        
        # Extract step number from the comment
        step_num_match = re.search(r'Step\s+(\d+)', step_content, re.IGNORECASE)
        step_number = int(step_num_match.group(1)) if step_num_match else i + 1
        
        # Extract step description 
        step_desc_match = re.search(r'Step\s+\d+[:\s]+(.+?)(?:\n|$)', step_content, re.IGNORECASE)
        step_desc = step_desc_match.group(1).strip() if step_desc_match else f"Step {step_number}"
        
        blocks.append({
            "step": f"step_{step_number}",
            "step_description": step_desc,
            "content": step_content,
            "step_number": step_number
        })
    
    return blocks,headers
