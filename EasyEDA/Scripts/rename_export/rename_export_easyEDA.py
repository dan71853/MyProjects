#!/usr/bin/env python3

import sys
import subprocess
from pathlib import Path

# -------------------------------
# Configuration: rename patterns
# -------------------------------

RENAME_RULES = {
    "3D_PCB": "IMAGE",
    "BOM_": "BOM",
    "Gerber_": "GERBER",
    "PickAndPlace_": "P&P",
    "SCH_": "SCH",
}

# -------------------------------
# Helpers
# -------------------------------

def safe_rename(src: Path, dst: Path):
    if dst.exists():
        print(f"SKIP (already exists): {dst.name}")
        return False
    src.rename(dst)
    print(f"RENAMED: {src.name} -> {dst.name}")
    return True


def git_command(args, cwd):
    subprocess.run(args, cwd=cwd, check=True)


# -------------------------------
# Main
# -------------------------------

def main():
    if len(sys.argv) < 2:
        print("Usage: drag a version folder onto this script")
        input()
        sys.exit(1)

    version_dir = Path(sys.argv[1]).resolve()

    if not version_dir.is_dir():
        print("Error: argument is not a directory")
        sys.exit(1)

    version = version_dir.name                   # e.g. V1.0
    pcb_name = version_dir.parent.name           # e.g. can_module

    print(f"PCB name : {pcb_name}")
    print(f"Version  : {version}")
    print()

    renamed_files = []

    for file in version_dir.iterdir():
        if not file.is_file():
            continue

        for match, prefix in RENAME_RULES.items():
            if file.name.startswith(match):
                new_name = f"{prefix}-{pcb_name}-{version}{file.suffix}"
                target = file.with_name(new_name)

                if safe_rename(file, target):
                    renamed_files.append(target)
                break

    if not renamed_files:
        print("\nNo files were renamed.")
        return

    print("\nRenaming complete.")
    answer = input("Commit to git? [y/n]: ").strip().lower()

    if answer != "y":
        print("Git commit skipped.")
        return

    try:
        # git add
        git_command(["git", "add"] + [str(f) for f in renamed_files],
                    cwd=version_dir.parent)

        # git commit
        commit_message = (
            f"{pcb_name}-{version}:\n"
            f"- Exported manufacturing files"
        )

        git_command(
            ["git", "commit", "-m", commit_message],
            cwd=version_dir.parent
        )

        print("Git commit complete.")

    except subprocess.CalledProcessError as e:
        print("Git operation failed.")
        print(e)


if __name__ == "__main__":
    main()
