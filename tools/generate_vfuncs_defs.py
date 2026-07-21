#!/usr/bin/env python3

# libadwaitamm/tools/generate_vfuncs_defs.py
#
# Generates libadwaita_vfuncs.defs: a (define-vfunc ...) entry for every
# virtual function declared in a public AdwFooClass struct, across the
# given headers. Fully automatic and unconditional: like
# libadwaita_{enums,methods,signals}.defs, this file is regenerated from
# scratch each run. Don’t hand-edit it -- edits are overwritten next run.
#
# Usage:
#   tools/generate_vfuncs_defs.py [header ...]
#
# With no arguments, searches common install prefixes for libadwaita-1’s
# installed public headers (the same headers generate_defs_and_docs.sh
# points enum.pl/h2def.py at). Pass explicit paths/globs to look elsewhere,
# e.g. tools/generate_vfuncs_defs.py ../c/*.h
#
# Wire-up: `ninja -C <builddir> gen-vfuncs` (maintainer-mode meson build,
# mirrors gen-signals/tools/generate_signals.sh) or run directly. See
# tools/README.md.
#
# A vfunc is any function-pointer member of a "struct _AdwFooClass { ... };"
# block, up to (not including) a "/*< private >*/" marker or the parent_class
# member. Its leading "self" parameter is dropped from the emitted
# (parameters ...) list, since (of-object "AdwFoo") already implies it --
# matching (define-method ...) convention.
#
# Return/parameter types are converted to gmmproc .defs spelling on a
# best-effort basis (e.g. "const char *" -> "const-gchar*", via the
# TYPE_ALIASES table below for common C/GLib fundamentals; anything else is
# hyphenated generically). If gmmproc rejects a generated type, add it to
# TYPE_ALIASES.

import argparse
from collections.abc import Iterable
import re
import sys
from glob import glob
from pathlib import Path

TOOLS_DIR = Path(__file__).resolve().parent
REPO_ROOT = TOOLS_DIR.parent
DEFAULT_DEFS_FILE = REPO_ROOT / "libadwaita" / "src" / "libadwaita_vfuncs.defs"

DEFAULT_HEADER_DIRS = [
    "/usr/include/libadwaita-1",
    "/usr/local/include/libadwaita-1",
    "/opt/homebrew/include/libadwaita-1",
]

CLASS_RE = re.compile(r"^struct\s+_Adw(\w+)Class\b")
# A vfunc member: "<return type> (* <name>) (<params>);", across 1+ lines.
VFUNC_RE = re.compile(
    r"([\w][\w\s]*?)\(\s*\*\s*(\w+)\s*\)\s*\(([^;]*)\)\s*;",
    re.DOTALL,
)

# Bare C fundamental -> glib/.defs spelling. Extend as needed; anything
# missing here just gets its spaces turned into hyphens (e.g. a struct tag
# gmmproc doesn’t otherwise know about would need a real entry anyway).
TYPE_ALIASES = {
    "char": "gchar",
    "unsigned char": "guchar",
    "short": "gshort",
    "unsigned short": "gushort",
    "int": "gint",
    "unsigned int": "guint",
    "unsigned": "guint",
    "long": "glong",
    "unsigned long": "gulong",
    "float": "gfloat",
    "double": "gdouble",
}


def default_headers() -> list[str]:
    for d in DEFAULT_HEADER_DIRS:
        p = Path(d)
        if p.is_dir():
            headers = sorted(p.glob("*.h"))
            if headers:
                return [str(h) for h in headers]
    return []


def expand_header_args(args: list[str]) -> list[str]:
    headers: list[str] = []
    for arg in args:
        matches = glob(arg)
        headers.extend(matches if matches else [arg])
    return headers


def find_class_blocks(text: str) -> Iterable[tuple[str, str]]:
    """Yield (AdwFoo, block_text) for each `struct _AdwFooClass { ... };` in text."""
    lines = text.splitlines()
    i = 0
    n = len(lines)
    while i < n:
        m = CLASS_RE.match(lines[i].strip())
        if not m:
            i += 1
            continue
        name = m.group(1)
        assert isinstance(name, str)
        # Find the opening brace (own line, per project convention).
        while i < n and "{" not in lines[i]:
            i += 1
        depth = lines[i].count("{") - lines[i].count("}") if i < n else 0
        i += 1
        block_lines = []
        while i < n and depth > 0:
            depth += lines[i].count("{") - lines[i].count("}")
            if depth <= 0:
                break
            block_lines.append(lines[i])
            i += 1
        yield "Adw" + name, "\n".join(block_lines)


def parse_vfuncs(block_text: str) -> list[tuple[str, str, str]]:
    """
    Extract (return_type, name, params) vfunc entries, ignoring parent_class and
    anything from a ``/*< private >*/`` marker onward.
    """
    private_marker = block_text.find("/*< private >*/")
    if private_marker != -1:
        block_text = block_text[:private_marker]
    vfuncs: list[tuple[str, str, str]] = []
    for m in VFUNC_RE.finditer(block_text):
        ret = " ".join(m.group(1).split())
        name = m.group(2)
        assert isinstance(name, str)
        params = " ".join(m.group(3).split())
        vfuncs.append((ret, name, params))
    return vfuncs


def format_type(ctype: str):
    """
    Convert a raw C type (e.g. "const char *", "AdwFoo *", "guint") to gmmproc .defs
    spelling (e.g. "const-gchar*", "AdwFoo*", "guint").
    """
    ctype = ctype.strip()
    is_pointer = ctype.endswith("*")
    ctype = ctype.rstrip("*").strip()
    is_const = ctype.startswith("const ")
    if is_const:
        ctype = ctype[len("const ") :].strip()
    ctype = " ".join(ctype.split())
    base = TYPE_ALIASES.get(ctype, ctype.replace(" ", "-"))
    if is_const:
        base = "const-" + base
    if is_pointer:
        base += "*"
    return base


def split_params(params: str) -> list[tuple[str, str]]:
    """
    Split a C parameter list into [(type, name), ...], dropping the leading ``self``
    parameter (implicit via ``of-object`` in ``define-vfunc``, matching
    ``define-method`` convention).
    """
    params = params.strip()
    if params in ("", "void"):
        return []
    result: list[tuple[str, str]] = []
    for part in params.split(","):
        part = part.strip()
        m = re.match(r"^(.*[\s\*])(\w+)$", part)
        ptype, pname = (m.group(1).strip(), m.group(2)) if m else (part, "")
        assert isinstance(ptype, str) and isinstance(pname, str)
        result.append((ptype, pname))
    return result[1:]  # drop self


def format_vfunc(class_name: str, ret: str, name: str, params: str) -> str:
    lines = [f"(define-vfunc {name}"]
    lines.append(f'  (of-object "{class_name}")')
    lines.append(f'  (return-type "{format_type(ret)}")')
    param_pairs = split_params(params)
    if param_pairs:
        lines.append("  (parameters")
        for ptype, pname in param_pairs:
            lines.append(f'    \'("{format_type(ptype)}" "{pname}")')
        lines.append("  )")
    lines.append(")")
    return "\n".join(lines)


def generate(headers: list[str]) -> tuple[str, int]:
    """
    Return (defs_text, count) for every vfunc found across headers, grouped by
    originating header file.
    """
    sections: list[str] = []
    count = 0
    for header in sorted(headers):
        path = Path(header)
        if not path.is_file() or path.name.endswith("-private.h"):
            continue
        text = path.read_text()
        entries = []
        for class_name, block in find_class_blocks(text):
            for ret, name, params in parse_vfuncs(block):
                entries.append(format_vfunc(class_name, ret, name, params))
                count += 1
        if entries:
            sections.append(f";; From {path.name}\n\n" + "\n\n".join(entries))

    header_comment = (
        "; Virtual functions\n"
        "; Generated by tools/generate_vfuncs_defs.py -- do not edit by hand.\n"
        "; See tools/README.md.\n"
    )
    body = header_comment + "\n" + "\n\n".join(sections) + "\n"
    return body, count


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "headers",
        nargs="*",
        help="header files/globs to scan (default: installed libadwaita-1 headers)",
    )
    parser.add_argument("--defs-file", default=str(DEFAULT_DEFS_FILE))
    parser.add_argument(
        "--dry-run",
        action="store_true",
        help="print to stdout instead of writing --defs-file",
    )
    args = parser.parse_args()

    headers = expand_header_args(args.headers) if args.headers else default_headers()
    if not headers:
        print(
            "error: no header files found; pass paths/globs explicitly", file=sys.stderr
        )
        return 1

    body, count = generate(headers)

    if args.dry_run:
        print(body, end="")
    else:
        Path(args.defs_file).write_text(body)
        print(f"Wrote {count} vfunc(s) to {args.defs_file}.")
    return 0


if __name__ == "__main__":
    sys.exit(main())
