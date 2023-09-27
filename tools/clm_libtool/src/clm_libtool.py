from pathlib import Path

import colorama
from classopt import classopt


@classopt(default_long=True)
class Opt:
    root_path: str
    action: str
    name: str


def main():
    colorama.init()

    opt = Opt.from_args()
    print(opt)


if __name__ == "__main__":
    main()
