WRAP_LIBRARY("ITKPyBase" Python)
  
  SET(WRAPPER_LIBRARY_DEPENDS )
  SET(WRAPPER_LIBRARY_LINK_LIBRARIES )
  
  SET(WRAPPER_LIBRARY_SWIG_INPUTS "${CMAKE_CURRENT_SOURCE_DIR}/wrap_pyBase.i")
  
  # no wrap_*.cmake file to include, no need to run that
  # AUTO_INCLUDE_MODULES()
  
  # add the vars defined by including std_iostream and std_stream to the python config
  ADD_PYTHON_CONFIG_TEMPLATE("PySwigIterator" "PySwigIterator" "PySwigIterator" "")
  ADD_PYTHON_CONFIG_TEMPLATE("cerr" "std::cerr" "cerr" "")
  ADD_PYTHON_CONFIG_TEMPLATE("cin" "std::cin" "cin" "")
  ADD_PYTHON_CONFIG_TEMPLATE("clog" "std::clog" "clog" "")
  ADD_PYTHON_CONFIG_TEMPLATE("cout" "std::cout" "cout" "")
  ADD_PYTHON_CONFIG_TEMPLATE("cvar" "std::cvar" "cvar" "")
  ADD_PYTHON_CONFIG_TEMPLATE("endl" "std::endl" "endl" "")
  ADD_PYTHON_CONFIG_TEMPLATE("ends" "std::ends" "ends" "")
  ADD_PYTHON_CONFIG_TEMPLATE("flush" "std::flush" "flush" "")
  ADD_PYTHON_CONFIG_TEMPLATE("ios" "std::ios" "ios" "")
  ADD_PYTHON_CONFIG_TEMPLATE("ios_base" "std::ios_base" "ios_base" "")
  ADD_PYTHON_CONFIG_TEMPLATE("ios_base_sync_with_stdio" "std::ios_base_sync_with_stdio" "ios_base_sync_with_stdio" "")
  ADD_PYTHON_CONFIG_TEMPLATE("ios_base_xalloc" "std::ios_base_xalloc" "ios_base_xalloc" "")
  ADD_PYTHON_CONFIG_TEMPLATE("iostream" "std::iostream" "iostream" "")
  ADD_PYTHON_CONFIG_TEMPLATE("istream" "std::istream" "istream" "")
  ADD_PYTHON_CONFIG_TEMPLATE("istringstream" "std::istringstream" "istringstream" "")
  ADD_PYTHON_CONFIG_TEMPLATE("ostream" "std::ostream" "ostream" "")
  ADD_PYTHON_CONFIG_TEMPLATE("ostringstream" "std::ostringstream" "ostringstream" "")
  ADD_PYTHON_CONFIG_TEMPLATE("streambuf" "std::streambuf" "streambuf" "")
  ADD_PYTHON_CONFIG_TEMPLATE("string" "std::string" "string" "")
  ADD_PYTHON_CONFIG_TEMPLATE("stringstream" "std::stringstream" "stringstream" "")

  FOREACH(t B UC US UL SC SS SL F D)
    ADD_PYTHON_CONFIG_TEMPLATE("map" "std::map" "map${t}${t}" "${ITKT_${t}}, ${ITKT_${t}}")
    ADD_PYTHON_CONFIG_TEMPLATE("list" "std::list" "list${t}" "${ITKT_${t}}")
    ADD_PYTHON_CONFIG_TEMPLATE("vector" "std::vector" "vector${t}" "${ITKT_${t}}")
    ADD_PYTHON_CONFIG_TEMPLATE("vector" "std::vector" "vectorvector${t}" "std::vector< ${ITKT_${t}} >")
  ENDFOREACH(t)
  
  ADD_PYTHON_CONFIG_TEMPLATE("map" "std::map" "mapULD" "unsigned long, double")

END_WRAP_LIBRARY()

