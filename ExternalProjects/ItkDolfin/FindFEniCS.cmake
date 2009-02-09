# Try to find FEniCS
#
# FENICS_FOUND - FEniCS is installed
# FENICS_DIR   - FEniCS root directory
#
#

  # TODO
  FIND_PATH(FENICS_DIR <file>
    /usr/include/dolfin
    DOC "Directory where ..."
    )

  IF(FENICS_DIR)
    SET (FENICS_FOUND 1 CACHE INTERNAL "FEniCS is installed")
  ENDIF(FENICS_DIR)

