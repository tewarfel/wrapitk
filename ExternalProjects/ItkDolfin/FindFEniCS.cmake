# Try to find FEniCS
#
# FENICS_FOUND - FEniCS is installed
# FENICS_DIR   - FEniCS root directory
#
#

  # TODO
  find_path(FENICS_DIR <file>
    /usr/include/dolfin
    DOC "Directory where ..."
    )

  if(FENICS_DIR)
    set(FENICS_FOUND 1 CACHE INTERNAL "FEniCS is installed")
  endif(FENICS_DIR)
