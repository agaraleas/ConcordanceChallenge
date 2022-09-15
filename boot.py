import os
import subprocess
from pathlib import Path

def main():
    running_dir = Path(__file__).parent.absolute()
    os.chdir(running_dir);

    os.makedirs('./out/build', exist_ok=True)
    subprocess.check_call('cmake -S . -B ./out/build', shell=True)

if __name__ == '__main__':
    main()