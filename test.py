#! /usr/bin/env python3

import sys
import os, os.path as path
import re
import threading
import subprocess
import difflib
from datetime import timedelta
from pathlib import Path
from typing import *

def usage():
    print("Usage: test.py <executable> <tests_directory> <memory_limit> <time_limit> <malloc_override> [flags]")
    print("")
    print("flags:")
    print("    --help | -h:     print this message")

def parse_human_readable_size(human_readable_size: str) -> int:
    units = {
        "B":  2 ** (10 * 0),
        "KB": 2 ** (10 * 1),
        "MB": 2 ** (10 * 2),
        "GB": 2 ** (10 * 3),
        "TB": 2 ** (10 * 4)
    }

    human_readable_size = human_readable_size.upper()

    if ' ' not in human_readable_size:
        human_readable_size = re.sub(r'([KMGT]?B)', r' \1', human_readable_size)
    
    number, unit = [
        string.strip() for string in human_readable_size.split()  
    ]

    return int(
        float(number) * units[unit]
    )

def parse_human_readable_time(human_readable_time: str) -> int:
    units = {
        "ms": timedelta(milliseconds=1) / timedelta(milliseconds=1),
        "s":  timedelta(seconds=1) / timedelta(milliseconds=1),
        "m":  timedelta(minutes=1) / timedelta(milliseconds=1),
        "h":  timedelta(hours=1) / timedelta(milliseconds=1)
    }

    parts = re.findall(
        r"(\d+(?:\.\d)?)(ms?|[smhd])",
        human_readable_time
    )

    total_ms = sum(float(number) * units[unit] for number, unit in parts)

    return int(total_ms)

def run_tests(executable: str, tests: list[Tuple[str, str]], memory_limit: int, time_limit: int, tests_directory: str, malloc_override: str) -> Tuple[list[str], list[str], list[str]]:
    timeouts = []
    fails = []
    memory_limit_exceeded = []
    
    def run_test(test: Tuple[str, str], memory_limit: int, time_limit: int, malloc_override: str):
        input_str:  str = Path(tests_directory + "/" + test[0]).read_text()
        output_exp: str = Path(tests_directory + "/" + test[1]).read_text()

        subprocess_env = os.environ.copy()
        subprocess_env["MEM_LIMIT"]  = str(memory_limit)
        subprocess_env["LD_PRELOAD"] = str(malloc_override)

        process = subprocess.Popen(
            "./" + executable, 
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            stdin =subprocess.PIPE,
            env=subprocess_env
        )
        
        try:
            outs, errs = process.communicate(input=input_str.encode(), timeout=time_limit / 1000)
        except subprocess.TimeoutExpired:
            process.kill()
            _, _ = process.communicate()

            timeouts.append(test[0])
            return

        if outs.decode() != output_exp:
            if "Memory limit exceeded" in outs.decode():
                print(outs.decode())
                memory_limit_exceeded.append(test[0])
            else:
                print(f"Expected: {output_exp.strip()}. Got: {list(outs)}")

                fails.append(test[0])
            return
                    
    threads = []

    for test in tests:
        thread = threading.Thread(target=run_test, args=(test, memory_limit, time_limit, malloc_override))
        threads.append(thread)

    [t.start() for t in threads]
    [t.join() for t in threads]

    timeouts.sort()
    fails.sort()
    memory_limit_exceeded.sort()

    return (fails, timeouts, memory_limit_exceeded)

def print_result(tests: list[Tuple[str, str]], failed_l: list[str], timeout_l: list[str], memory_limit_exceeded_l: list[str]):
    failed:  set[str] = set(failed_l)
    timeout: set[str] = set(timeout_l)
    memory_limit_exceeded: set[str] = set(memory_limit_exceeded_l)

    for test in tests:
        print(f"TEST {test[0]}...    ", end='')

        if test[0] in failed:
            print("FAILED")
        elif test[0] in timeout:
            print("TIME OUT")
        elif test[0] in memory_limit_exceeded:
            print("MEMORY LIMIT EXCEEDED")
        else:
            print("OK")

    print("")
    print(f"PASSED {len(tests) - (len(failed) + len(timeout) + len(memory_limit_exceeded))} / {len(tests)}")

def main():
    argv: list[str] = sys.argv[1:]
    argc: int = len(argv)

    if argc != 5:
        for arg in argv:
            if arg == "--help" or arg == "-h":
                usage()
                return 0

        print("[ERROR] Invalid command line arguments provided. Run `test.py --help` for help", file=sys.stderr)
        return 1

    (executable_r, tests_directory_r, memory_limit_r, time_limit_r, malloc_override) = argv

    memory_limit: int = parse_human_readable_size(memory_limit_r)
    time_limit: int = parse_human_readable_time(time_limit_r)
    executable: Path = Path(executable_r)
    tests_directory: Path = Path(tests_directory_r)

    if (not executable.is_file()) or (not tests_directory.exists()):
        file_or_dir: Tuple[Path, str] = (executable, "executable") if not executable.is_file() else (tests_directory, "directory")

        print(f"[ERROR] Provided {file_or_dir[1]} `{file_or_dir[0]}` does not exist. Run `test.py --help` for help", file=sys.stderr)
        return 1

    tests_in_sol: list[str] = os.listdir(tests_directory) 
    tests_in:  list[str] = list(filter(lambda p: path.splitext(p)[1] == '.in',  tests_in_sol))
    tests_sol: list[str] = list(filter(lambda p: path.splitext(p)[1] == '.sol' or path.splitext(p)[1] == '.out', tests_in_sol))
    tests: list[Tuple[str, str]] = list(zip(tests_in, tests_sol))

    del tests_in_sol, tests_in, tests_sol

    tests.sort()

    (fails, timeouts, memory_limit_exceeded) = run_tests(str(executable), tests, memory_limit, time_limit, str(tests_directory), malloc_override)

    print_result(tests, fails, timeouts, memory_limit_exceeded)

if __name__ == "__main__":
    sys.exit(main())