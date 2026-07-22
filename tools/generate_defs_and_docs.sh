#!/usr/bin/env bash

# libadwaitamm/tools/generate_defs_and_docs.sh

# Global environment variables:
# GMMPROC_GEN_SOURCE_DIR  Top directory where source files are searched for.
#                         Default value: $(dirname "$0")/../..
#                         i.e. 2 levels above this file.
# GMMPROC_GEN_BUILD_DIR   Top directory where built files are searched for.
#                         Default value: $GMMPROC_GEN_SOURCE_DIR
#
# If you use jhbuild, you can set these environment variables equal to jhbuild's
# configuration variables checkoutroot and buildroot, respectively.
# Usually you can leave GMMPROC_GEN_SOURCE_DIR undefined.
# If you have set buildroot=None, GMMPROC_GEN_BUILD_DIR can also be undefined.
#
# This script is the jhbuild/no-meson-run-target path for regenerating the
# defs. If you already have a configured meson build directory, it's simpler
# to build the extra_defs_gen executable once and then run:
#   ninja -C <builddir> gen-signals
# which regenerates only libadwaita_signals.defs (see tools/generate_signals.sh).
# This script additionally regenerates libadwaita_docs.xml, libadwaita_enums.defs,
# and libadwaita_methods.defs, which have no meson run_target of their own.
#
# See tools/README.md for an overview of all the libadwaita_*.defs files,
# which ones are generated vs. hand-written, and how to regenerate each.

# Generated files:
#   libadwaitamm/libadwaita/src/libadwaita_docs.xml
#   libadwaitamm/libadwaita/src/libadwaita_enums.defs
#   libadwaitamm/libadwaita/src/libadwaita_methods.defs
#   libadwaitamm/libadwaita/src/libadwaita_signals.defs

# Root directory of libadwaitamm source files.
root_dir="$(dirname "$0")/.."

# Where to search for source files.
if [ -z "$GMMPROC_GEN_SOURCE_DIR" ]; then
  GMMPROC_GEN_SOURCE_DIR="$root_dir/.."
fi

# Where to search for built files.
if [ -z "$GMMPROC_GEN_BUILD_DIR" ]; then
  GMMPROC_GEN_BUILD_DIR="$GMMPROC_GEN_SOURCE_DIR"
fi

# Scripts in glibmm. These are source files.
gen_docs="$GMMPROC_GEN_SOURCE_DIR/glibmm/tools/defs_gen/docextract_to_xml.py"
gen_methods="$GMMPROC_GEN_SOURCE_DIR/glibmm/tools/defs_gen/h2def.py"
gen_enums="$GMMPROC_GEN_SOURCE_DIR/glibmm/tools/enum.pl"

# Where to find the executable that generates extra defs (signals and properties).
extra_defs_gen_dir="$GMMPROC_GEN_BUILD_DIR/libadwaitamm/tools/extra_defs_gen"
### If libadwaitamm is built with meson:
if [ "$GMMPROC_GEN_SOURCE_DIR" == "$GMMPROC_GEN_BUILD_DIR" ]; then
  # libadwaitamm is built with meson, which requires non-source-dir builds.
  # This is what jhbuild does, if necesary, to force non-source-dir builds.
  extra_defs_gen_dir="$GMMPROC_GEN_BUILD_DIR/libadwaitamm/build/tools/extra_defs_gen"
fi
### If libadwaitamm is built with autotools:
# libadwaitamm is built with autotools.
# autotools support, but don't require, non-source-dir builds.

source_prefix="$GMMPROC_GEN_SOURCE_DIR/libadwaita"
build_prefix="$GMMPROC_GEN_BUILD_DIR/libadwaita"
### If libadwaita is built with meson:
if [ "$source_prefix" == "$build_prefix" ]; then
  # libadwaita is built with meson, which requires non-source-dir builds.
  # This is what jhbuild does, if neccesary, to force non-source-dir builds.
  build_prefix="$build_prefix/build"
fi
### If libadwaita is built with autotools:
# libadwaita is built with autotools, which support, but don't require, non-source-dir builds.

out_dir="$root_dir/libadwaita/src"

# Where the installed libadwaita-1 headers live. On systems where pkg-config
# knows about libadwaita-1 (e.g. Homebrew), use that; otherwise fall back to
# the traditional /usr/include location.
system_include_dir="/usr/include/libadwaita-1"
if pkg_includedir="$(pkg-config --variable=includedir libadwaita-1 2>/dev/null)" \
    && [ -d "$pkg_includedir/libadwaita-1" ]; then
  system_include_dir="$pkg_includedir/libadwaita-1"
fi

# Documentation
echo === libadwaita_docs.xml ===
params="--with-properties --no-recursion"
for dir in "$source_prefix/src" "$build_prefix/libadwaita"; do
  if [ -d "$dir" ]; then
    params="$params -s $dir"
  fi
done
"$gen_docs" $params > "$out_dir/libadwaita_docs.xml"

shopt -s nullglob # Skip a filename pattern that matches no file

# Enums
echo === libadwaita_enums.defs ===
"$gen_enums" "$system_include_dir"/*.h "$build_prefix"/libadwaita/*.h  > "$out_dir/libadwaita_enums.defs"

# Functions and methods
echo === libadwaita_methods.defs ===
"$gen_methods" "$system_include_dir"/*.h "$build_prefix"/libadwaita/*.h  > "$out_dir/libadwaita_methods.defs"

# Properties and signals
echo === libadwaita_signals.defs ===
"$extra_defs_gen_dir"/generate_defs_libadwaita > "$out_dir/libadwaita_signals.defs"

