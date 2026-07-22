#!/usr/bin/env python3

# libadwaitamm/tools/update_extra_defs_types.py
#
# Updates the ADW_TYPE_* list in tools/extra_defs_gen/generate_defs_libadwaita.cc
# to match the GObject/GInterface types actually wrapped by libadwaita/src/*.hg,
# cross-checked against the installed libadwaita headers (the source of truth
# for which ADW_TYPE_* macros really exist, and what version they need).
#
# Usage:
#   tools/update_extra_defs_types.py [--header-dir DIR] [--dry-run] [--check]
#
# With no --header-dir, searches common install prefixes (same ones
# generate_defs_and_docs.sh and generate_vfuncs_defs.py use), preferring
# pkg-config's libadwaita-1 includedir when available.
#
# What it does:
#   - Scans libadwaita/src/*.hg for _CLASS_GOBJECT/_CLASS_GTKOBJECT/
#     _CLASS_INTERFACE(CppName, CName, TypeMacroPrefix, ...) lines -- the
#     only three that register a GType with properties/signals get_defs()
#     can extract (unlike _CLASS_BOXEDTYPE/_CLASS_OPAQUE_REFCOUNTED) -- and
#     derives each one's ADW_TYPE_* macro from TypeMacroPrefix, the same way
#     gmmproc itself does. The .hg tree is authoritative for *removals*: an
#     existing entry is only dropped if no .hg file wraps it anymore.
#   - Before *adding* a brand-new entry, confirms its macro is actually
#     `#define`d somewhere in the installed headers -- catching typos and
#     guarding against types too new for the installed libadwaita (these just
#     get a warning and are left out, since the headers can't tell us what
#     version they'd need either).
#   - Diffs that "wanted" set against the get_defs(ADW_TYPE_*) calls already in
#     generate_defs_libadwaita.cc, alphabetically by type name (matching the
#     file's existing convention), and:
#       * adds missing entries, guarded with `#if ADW_CHECK_VERSION(...)` when
#         every wrapped member of the class carries the same (or a newer)
#         `newin "X.Y"` tag than the project's minimum libadwaita version
#         (read from meson.build's libadwaita_req);
#       * removes entries whose .hg class is gone.
#   - Never touches the guard or formatting of untouched, pre-existing entries.
#
# This only maintains the type *list*; run generate_defs_and_docs.sh (or
# `ninja -C <builddir> gen-signals`) afterwards to regenerate
# libadwaita_signals.defs itself. See tools/README.md.

from __future__ import annotations

import argparse
import re
import subprocess
import sys
from pathlib import Path

TOOLS_DIR = Path(__file__).resolve().parent
REPO_ROOT = TOOLS_DIR.parent
DEFAULT_CC_FILE = TOOLS_DIR / "extra_defs_gen" / "generate_defs_libadwaita.cc"
DEFAULT_HG_DIR = REPO_ROOT / "libadwaita" / "src"
DEFAULT_MESON_FILE = REPO_ROOT / "meson.build"

DEFAULT_HEADER_DIRS = [
    "/usr/include/libadwaita-1",
    "/usr/local/include/libadwaita-1",
    "/opt/homebrew/include/libadwaita-1",
]

# Only these three macros register a GType with properties/signals get_defs()
# can extract; _CLASS_BOXEDTYPE/_CLASS_OPAQUE_REFCOUNTED don't apply.
HG_CLASS_RE = re.compile(
    r"_CLASS_(GOBJECT|GTKOBJECT|INTERFACE)\s*\(\s*(\w+)\s*,\s*(\w+)\s*,\s*(ADW_\w+)"
)
NEWIN_RE = re.compile(r'newin\s+"(\d+)\.(\d+)"')
HEADER_DEFINE_RE = re.compile(r"#\s*define\s+(ADW_TYPE_\w+)\b")
LIBADWAITA_REQ_RE = re.compile(r"libadwaita_req\s*=\s*'>=\s*(\d+)\.(\d+)(?:\.(\d+))?'")
GET_DEFS_RE = re.compile(r"get_defs\((ADW_TYPE_\w+)\)")


def find_header_dir() -> Path | None:
    try:
        out = subprocess.run(
            ["pkg-config", "--variable=includedir", "libadwaita-1"],
            capture_output=True,
            text=True,
            check=True,
        ).stdout.strip()
        if out and (Path(out) / "libadwaita-1").is_dir():
            return Path(out) / "libadwaita-1"
    except (OSError, subprocess.CalledProcessError):
        pass
    for d in DEFAULT_HEADER_DIRS:
        if Path(d).is_dir():
            return Path(d)
    return None


def parse_header_macros(header_dir: Path) -> set[str]:
    """Collect every ADW_TYPE_* macro `#define`d across the given headers."""
    macros: set[str] = set()
    for header in sorted(header_dir.glob("*.h")):
        macros.update(HEADER_DEFINE_RE.findall(header.read_text()))
    return macros


def parse_min_libadwaita_version(meson_file: Path) -> tuple[int, int, int]:
    text = meson_file.read_text()
    m = LIBADWAITA_REQ_RE.search(text)
    if not m:
        return (1, 2, 0)
    major, minor, micro = m.group(1), m.group(2), m.group(3)
    return (int(major), int(minor), int(micro) if micro else 0)


class WantedType:
    def __init__(self, macro: str, hg_path: Path, min_version: tuple[int, int] | None):
        self.macro = macro
        self.hg_path = hg_path
        self.min_version = min_version  # (major, minor), or None if untagged


def parse_wanted_types(hg_dir: Path) -> dict[str, WantedType]:
    """Every ADW_TYPE_* a .hg file wraps, derived from its _CLASS_* macro's
    type-macro-prefix argument (e.g. ADW_NAVIGATION_PAGE -> ADW_TYPE_NAVIGATION_PAGE)
    -- the same transformation gmmproc itself applies, so this needs no header
    cross-reference to be authoritative."""
    wanted: dict[str, WantedType] = {}
    for hg_file in sorted(hg_dir.glob("*.hg")):
        text = hg_file.read_text()
        matches = list(HG_CLASS_RE.finditer(text))
        for i, m in enumerate(matches):
            prefix = m.group(4)  # e.g. "ADW_NAVIGATION_PAGE"
            macro = "ADW_TYPE_" + prefix[len("ADW_") :]
            # Scan from just after this _CLASS_ line to the next one (or EOF)
            # for this class's own newin tags.
            block_start = m.end()
            block_end = matches[i + 1].start() if i + 1 < len(matches) else len(text)
            block = text[block_start:block_end]
            versions = [
                (int(vm.group(1)), int(vm.group(2))) for vm in NEWIN_RE.finditer(block)
            ]
            min_version = min(versions) if versions else None
            wanted[macro] = WantedType(macro, hg_file, min_version)
    return wanted


class Block:
    def __init__(self, type_name: str, lines: list[str]):
        self.type_name = type_name
        self.lines = lines


def parse_existing_blocks(body_lines: list[str]) -> list[Block]:
    """Split main()'s body into one Block per get_defs() call, keeping any
    surrounding `#if .../#endif` guard lines attached to their single inner
    call (the only shape currently used in this file)."""
    blocks: list[Block] = []
    i = 0
    n = len(body_lines)
    while i < n:
        line = body_lines[i]
        if line.strip().startswith("#if"):
            guard_lines = [line]
            i += 1
            while i < n and not body_lines[i].strip().startswith("#endif"):
                guard_lines.append(body_lines[i])
                i += 1
            guard_lines.append(body_lines[i])  # #endif
            i += 1
            inner_types = GET_DEFS_RE.findall("\n".join(guard_lines))
            blocks.append(Block(inner_types[0] if inner_types else "", guard_lines))
        elif line.strip():
            m = GET_DEFS_RE.search(line)
            blocks.append(Block(m.group(1) if m else "", [line]))
            i += 1
        else:
            i += 1
    return blocks


def format_new_block(type_name: str, min_version: tuple[int, int] | None,
                      required: tuple[int, int, int]) -> Block:
    line = f"  std::cout << get_defs({type_name});"
    if min_version is not None and min_version > required[:2]:
        major, minor = min_version
        return Block(
            type_name,
            [f"#if ADW_CHECK_VERSION({major}, {minor}, 0)", line, "#endif"],
        )
    return Block(type_name, [line])


def update(cc_file: Path, hg_dir: Path, header_dir: Path, meson_file: Path):
    warnings: list[str] = []
    header_macros = parse_header_macros(header_dir)
    wanted = parse_wanted_types(hg_dir)
    required = parse_min_libadwaita_version(meson_file)

    text = cc_file.read_text()
    lines = text.splitlines()
    init_idx = next(i for i, l in enumerate(lines) if "adw_init();" in l)
    close_idx = len(lines) - 1
    while close_idx > init_idx and lines[close_idx].strip() != "}":
        close_idx -= 1

    prefix_lines = lines[: init_idx + 1]
    body_lines = lines[init_idx + 1 : close_idx]
    suffix_lines = lines[close_idx:]

    blocks = parse_existing_blocks(body_lines)
    existing_types = {b.type_name for b in blocks if b.type_name}

    candidates = sorted(set(wanted) - existing_types)
    added = []
    for t in candidates:
        if t in header_macros:
            added.append(t)
        else:
            warnings.append(
                f"{wanted[t].hg_path.name}: {t} not `#define`d in any installed "
                "header -- skipping (typo, or too new for these headers?)"
            )
    removed = sorted(existing_types - set(wanted))

    kept_blocks = [b for b in blocks if b.type_name not in removed]
    new_blocks = [
        format_new_block(t, wanted[t].min_version, required) for t in added
    ]
    all_blocks = sorted(kept_blocks + new_blocks, key=lambda b: b.type_name)

    new_body_lines: list[str] = []
    for b in all_blocks:
        new_body_lines.extend(b.lines)

    new_text = "\n".join(prefix_lines + [""] + new_body_lines + suffix_lines) + "\n"
    return new_text, added, removed, warnings


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--cc-file", default=str(DEFAULT_CC_FILE))
    parser.add_argument("--hg-dir", default=str(DEFAULT_HG_DIR))
    parser.add_argument("--header-dir", default=None)
    parser.add_argument("--meson-file", default=str(DEFAULT_MESON_FILE))
    parser.add_argument(
        "--dry-run", action="store_true", help="print the new file instead of writing it"
    )
    parser.add_argument(
        "--check",
        action="store_true",
        help="exit 1 if the file would change, without writing it",
    )
    args = parser.parse_args()

    header_dir = Path(args.header_dir) if args.header_dir else find_header_dir()
    if header_dir is None or not header_dir.is_dir():
        print("error: no libadwaita-1 header directory found; pass --header-dir",
              file=sys.stderr)
        return 1

    cc_file = Path(args.cc_file)
    new_text, added, removed, warnings = update(
        cc_file, Path(args.hg_dir), header_dir, Path(args.meson_file)
    )

    for w in warnings:
        print(f"warning: {w}", file=sys.stderr)
    for t in added:
        print(f"+ {t}")
    for t in removed:
        print(f"- {t}")

    changed = new_text != cc_file.read_text()
    if args.check:
        return 1 if changed else 0
    if args.dry_run:
        print(new_text, end="")
    elif changed:
        cc_file.write_text(new_text)
        print(f"Updated {cc_file} ({len(added)} added, {len(removed)} removed).")
    else:
        print("Already up to date.")
    return 0


if __name__ == "__main__":
    sys.exit(main())
