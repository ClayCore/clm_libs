#!/usr/bin/env python

import shutil
import shlex
import os
import sys
import re
import logging as log
import subprocess as sp
from pathlib import Path


"""Build directory of CMake project"""
BUILDDIR = "target"


def run_cmd(cmd: list[str]):
    """Run subprocess and raise error if return code isn't `EXIT_SUCCESS`"""
    result: sp.CompletedProcess = sp.run(cmd)
    if result.returncode != 0:
        msg = "[ERROR]: "
        msg += f"\nstdout: {result.stdout}"
        msg += f"\nstderr: {result.stderr}"
        raise RuntimeError(msg)


def prepare():
    """Prepare project"""
    cmd: list[str] = shlex.split(f"cmake -S . -B {BUILDDIR}")
    run_cmd(cmd)


def build():
    """Build project"""
    cmd: list[str] = shlex.split(f"cmake --build {BUILDDIR}")
    run_cmd(cmd)


def clean():
    """Clean project, only if directory is valid and build dir exists"""
    root = Path(os.getcwd())
    root_str = str(root.name)

    pattern = re.compile('(clm_c*)[a-zA-Z]')
    if not pattern.match(root_str):
        msg = "[ERROR]: "
        msg += "\nroot path does not match regex pattern"
        msg += f"\nroot_path: {root_str}"
        raise RuntimeError(msg)

    log.info(f"Cleaning directory: {root_str}/target")
    target_dir = root / "target"

    if target_dir.exists():
        shutil.rmtree(target_dir)
    else:
        log.warning(f"\'target\' directory does not exist.")


def main():
    log.basicConfig(level=log.DEBUG)

    argv = sys.argv[1:]
    subcommand = argv[0]

    if len(argv) != 1:
        log.error("Invalid launch parameters")

    if "clean" in subcommand:
        clean()
    elif "build" in subcommand:
        build()
    elif "prepare" in subcommand:
        prepare()
    else:
        log.error(f"Unrecognized option: {subcommand}")


if __name__ == "__main__":
    main()
