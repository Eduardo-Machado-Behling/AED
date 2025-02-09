#!/usr/bin/python3
from typing import *
import random
import shutil

from dataclasses import dataclass
import subprocess
import os
import json
import sys
import matplotlib
matplotlib.use('TkAgg')
import matplotlib.pyplot as plt
import numpy as np

from collections import defaultdict
from BADCASE import *

BENCHMARK_PATH = os.path.join(os.getcwd(), "bin", "benchmark")
MAX_INPUT_SIZE = 100_000

class Cache:
    def __init__(self, type: str, level: int, size: int, num_sharing: int):
        self.type = type
        self.level = level
        self.size = size
        self.num_sharing = num_sharing

class Context:
    def __init__(self, date: str, host_name: str, executable: str, num_cpus: int, mhz_per_cpu: int,
                 cpu_scaling_enabled: bool, caches: List[Cache], load_avg: List[float], library_build_type: str,
                 library_version: str|None = None, json_schema_version: str|None = None):
        self.date = date
        self.host_name = host_name
        self.executable = executable
        self.num_cpus = num_cpus
        self.mhz_per_cpu = mhz_per_cpu
        self.cpu_scaling_enabled = cpu_scaling_enabled
        self.caches = caches
        self.load_avg = load_avg
        self.library_build_type = library_build_type
        self.library_version = library_version
        self.json_schema_version = json_schema_version

class Benchmark:
    def __init__(self, name: str, family_index: int, per_family_instance_index: int, run_name: str,
                 run_type: str, repetitions: int, repetition_index: int, threads: int, iterations: int,
                 real_time: float, cpu_time: float, time_unit: str):
        self.name = name
        self.family_index = family_index
        self.per_family_instance_index = per_family_instance_index
        self.run_name = run_name
        self.run_type = run_type
        self.repetitions = repetitions
        self.repetition_index = repetition_index
        self.threads = threads
        self.iterations = iterations
        self.real_time = real_time
        self.cpu_time = cpu_time
        self.time_unit = time_unit

class BenchmarkData:
    def __init__(self, context: Context, benchmarks: List[Benchmark]):
        self.context = context
        self.benchmarks = benchmarks

    @staticmethod
    def from_json(json_data: str):
        data = json.loads(json_data)
        context_data = data["context"]
        caches = [Cache(**cache) for cache in context_data["caches"]]
        context_data["caches"] = caches  # Assign caches to context_data
        context = Context(**context_data)  # Pass context_data with caches included
        benchmarks = [Benchmark(**bm) for bm in data["benchmarks"]]
        return BenchmarkData(context, benchmarks)

@dataclass
class Input:
    message: str
    extras: int
    classes: List[int]

    def generateFile(self) -> None:
        path = os.path.join(BENCHMARK_PATH, "input.txt")
        with open(path, 'w') as f:
            f.write(self.message + '\n')
            f.write(f"{self.extras}\n")
            f.write(f"{len(self.classes)}\n")
            f.writelines(map(lambda x: f"{x[0]},{x[1]}\n", self.classes))

INTMAX = 2**31 - 1

def generateInput(amount: int) -> List[int]:
    def genPair() -> Tuple[int, int]:
        students = random.randint(0, INTMAX)
        passed = random.randint(0, students)
        return passed, students

    return [genPair() for _ in range(amount)]

def cleanFolder(folder: str) -> None:
    for filename in os.listdir(folder):
        file_path = os.path.join(folder, filename)
        try:
            if os.path.isfile(file_path) or os.path.islink(file_path):
                os.unlink(file_path)
            elif os.path.isdir(file_path):
                shutil.rmtree(file_path)
        except Exception as e:
            print('Failed to delete %s. Reason: %s' % (file_path, e))

def runWithInput(input: Input, firstTime = []) -> BenchmarkData | None:
    input.generateFile()
    executable = "bench"
    if os.name == 'nt':
        executable += '.exe'
    
    executable = os.path.join(BENCHMARK_PATH, executable)
    process = subprocess.Popen([executable, "--benchmark_format=json"], cwd=BENCHMARK_PATH, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
    data_raw = process.stdout.read()

    output_folder = os.path.join(BENCHMARK_PATH, "outputs")
    if not os.path.exists(output_folder):
        os.mkdir(output_folder)
    elif not firstTime:             #hacky way, because quick and dirty
        cleanFolder(output_folder)

    input_folder = os.path.join(BENCHMARK_PATH, "inputs")
    if not os.path.exists(input_folder):
        os.mkdir(input_folder)
    elif not firstTime:             #TODO: hacky way, because it's to late in the night
        cleanFolder(input_folder)
        firstTime.append(1)

    with open(os.path.join(output_folder, f"data{len(input.classes)}.json"), 'w') as js:
        js.write(data_raw)
    
    os.rename(os.path.join(BENCHMARK_PATH, "input.txt"), os.path.join(input_folder, f"input{len(input.classes)}.txt"))

    try:
        benchdata = BenchmarkData.from_json(data_raw)
    except json.JSONDecodeError as e:
        print("Error while decoding output")
        print(e)
        print("Faulty json will be in ./bin/benchmark/data.json:")
        return None
    return benchdata

def graph(extras: int, inputsSize: List[int], benchmarks: List[BenchmarkData]) -> None:
    plt.figure(figsize=(8, 5))

    data = defaultdict(list)
    [[data[bench.name].append(bench.cpu_time * 1_000_000) for bench in benchmark.benchmarks] for benchmark in benchmarks]

    for k,v in  data.items():
        plt.plot(inputsSize, v, label=k)

    plt.xlabel('Input Size')
    plt.ylabel('Time in ms')
    plt.yscale('log')
    plt.title(f'Implementation comparision (extras={extras})')
    plt.grid(True)
    plt.legend()

    plt.savefig("graph.png")
    plt.show()




def main(extra: int|None, cases: int|None) -> None:
    benchmarks = []
    sizes = []
    if(extra == None):
        extra = random.randint(8, 256)
    if(cases == None):
        cases = 16

    print(f"Running with extra={extra}, cases={cases}")
    os.system("cmake -S . -B build -DBENCHMARK=ON -DCMAKE_BUILD_TYPE=Release")
    os.system("cmake --build build")
    for gen in np.ceil(np.linspace(2, MAX_INPUT_SIZE, cases)):
        gen = int(gen)
        if gen == MAX_INPUT_SIZE:
            print("Reached BAD_CASE")
            break
        inp = Input(f"Random ({extra}/{gen})", extra, generateInput(gen))
        benchdata = runWithInput(inp)

        if benchdata == None:
            print("Aborting early, graph will be visible")
            break
        print(f"Finished benchmark for (inputSize={gen})")

        benchmarks.append(runWithInput(inp))
        sizes.append(gen)
    
    benchmarks.append(runWithInput(Input(f"Test case that timeouts my code", BADCASE_EXTRAS, BADCASE_INPUT)))
    sizes.append(len(BADCASE_INPUT))
    graph(extra, sizes, benchmarks)


    

if __name__ == '__main__':
    extra = None
    cases = None
    if len(sys.argv) > 2:
        cases = int(sys.argv[2])
        extra = int(sys.argv[1])
    elif len(sys.argv) > 1:
        extra = int(sys.argv[1])

    main(extra, cases)
