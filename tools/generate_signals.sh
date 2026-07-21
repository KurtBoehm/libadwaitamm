#!/bin/bash -e

# libadwaitamm/tools/generate_signals.sh
#
# Regenerates libadwaita/src/libadwaita_signals.defs (properties and signals).
#
# Not meant to be run directly: this is invoked by the meson run_target
# 'gen-signals' (defined in the top-level meson.build, only when
# maintainer_mode is enabled), as:
#   ninja -C <builddir> gen-signals
# meson passes the path to the built generate_defs_libadwaita executable as
# $1, and sets $MESON_SOURCE_ROOT to the project root.
#
# If the list of ADW_TYPE_* classes in
# tools/extra_defs_gen/generate_defs_libadwaita.cc falls out of sync with the
# classes wrapped in libadwaita/src/*.hg, this will silently omit their
# properties and signals. See tools/README.md.
#
# For the other libadwaita_*.defs files, see tools/generate_defs_and_docs.sh.

"$1" > "${MESON_SOURCE_ROOT}/libadwaita/src/libadwaita_signals.defs"
