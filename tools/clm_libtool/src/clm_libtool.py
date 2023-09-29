import logging as log
import shutil
import errno
import os
import stat
from pathlib import Path

import colorama
from classopt import classopt


# List of paths to exclude from globbing
EXCLUDE_LIST = ["target", "build", "docs", "private"]

# List of paths (relative to root) to include when updating or preparing
# the library directory
INCLUDE_LIST = ["Makefile", ".clang-format", ".vscode", "LICENSE"]


# Utility for replacing the functionality of `shutil.copytree`
def copyanything(src, dst, symlinks=False, ignore=None):
    if not os.path.exists(dst):
        os.makedirs(dst)
        shutil.copystat(src, dst)

    lst = os.listdir(src)
    if ignore:
        excl = ignore(src, lst)
        lst = [x for x in lst if x not in excl]

    for item in lst:
        s = os.path.join(src, item)
        d = os.path.join(dst, item)
        if symlinks and os.path.islink(s):
            if os.path.lexists(d):
                os.remove(d)

            os.symlink(os.readlink(s), d)
            try:
                st = os.lstat(s)
                mode = stat.S_IMODE(st.st_mode)
                os.lchmod(d, mode)
            except:
                pass  # lchmod not available
        elif os.path.isdir(s):
            shutil.copytree(s, d, symlinks, ignore)
        else:
            shutil.copy2(s, d)


@classopt(default_long=True)
class Opt:
    "used for parsing launch parameters"

    root_path: str
    action: str
    name: str


class Libtool(object):
    def __init__(self, root: Path, libs: list[Path], name: str):
        self.root_path = root / "shared"
        self.name = name
        self.libs = libs

    def validate_mtime(self, name: str, file: Path) -> bool:
        """
        `validate_date`

        Validates modification times of two files and if the original is newer
        the destination remains unchanged, otherwise the destination gets removed and gets
        replaced by the updated source
        """

        src = self.root_path / name
        dst = file / name

        if (not src.exists()) or (not dst.exists()):
            log.error(f"validate_date: source or destination doesn't exist")

        stat_src = src.stat()
        stat_dst = dst.stat()

        mdate_src = round(stat_src.st_mtime)
        mdate_dst = round(stat_dst.st_mtime)

        if mdate_src > mdate_dst:
            if dst.is_dir():
                shutil.rmtree(dst)

                dst_dir = dst / name
                self.copy(src, dst_dir)
            else:
                dst.unlink()
                self.copy(src, dst)

    def prepare(self):
        log.info(f"Preparing library {self.name}")

        # Validate modification dates
        for filename in INCLUDE_LIST:
            for path in self.libs:
                new_path = path / filename
                if new_path.exists():
                    self.validate_mtime(filename, path)
                else:
                    self.copy(filename, path)

    def clean(self):
        log.info(f"Cleaning library {self.name}")

        self.remove_all()

    def remove(self, filename: str, path: Path):
        new_path = path / filename
        if new_path.exists() and new_path.is_dir():
            log.warning(f"removing dir \'{str(new_path)}\'")
            # shutil.rmtree(new_path)
        elif new_path.exists() and not new_path.is_dir():
            log.warning(f"removing file \'{str(new_path)}\'")
            # new_path.unlink()
        else:
            log.error(f"remove: failed to remove \'{str(new_path)}\'")

    def remove_all(self):
        """
        `remove_all`
        remove all of the copied filepaths in the library directory 
        """
        for filename in INCLUDE_LIST:
            for path in self.libs:
                self.remove(filename, path)

    def copy(self, filename: str, path: Path):
        new_path = path / filename
        if new_path.exists():
            pass
        else:
            source = self.root_path / filename
            src = source.resolve()
            dst = path.resolve()

            log.debug(f"Copying \'{str(src)}\' to \'{str(dst)}\'")

            if not src.is_dir():
                shutil.copy2(src, dst)
            else:
                # This hack is expressly because `copy_tree`
                # will not copy the source directory itself, unless
                # that directory is also specified in the destination
                dst_dir = dst / filename
                copyanything(src, dst_dir)

    def copy_all(self):
        """
        `copy_all`
        copies all the combined filepaths into the library directory
        """
        for filename in INCLUDE_LIST:
            for path in self.libs:
                self.copy(filename, path)


def find_lib(root: Path, name: str) -> list[Path]:
    """ 
    `find_lib`
    used for finding library folders that match the specified name
    """

    lib_paths = list(root.glob(f"**/clm_*{name}/"))

    log.debug(f"Raw lib paths: {lib_paths}")

    lib_filtered = []
    for lib in lib_paths:
        lib_str = str(lib)
        for exclude in EXCLUDE_LIST:
            if exclude not in lib_str and lib.is_dir():
                lib_filtered.append(lib)

    lib_unique = list(set(lib_filtered))

    log.debug(f"Filtered lib paths: {lib_unique}")

    return lib_unique


def main():
    "Entry point"

    colorama.init()
    log.basicConfig(
        level=log.DEBUG, format=f"{colorama.Fore.CYAN}%(levelname)s{colorama.Style.RESET_ALL}:%(message)s")

    opt = Opt.from_args()

    root_path = Path(opt.root_path)
    libs = find_lib(root_path, opt.name)

    libtool = Libtool(root_path, libs, opt.name)

    match opt.action:
        case "clean":
            libtool.clean()
        case "prepare":
            libtool.prepare()
        case _:
            pass


if __name__ == "__main__":
    main()
