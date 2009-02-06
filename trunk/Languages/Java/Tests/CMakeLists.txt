
# configure the test driver
FIND_PROGRAM(ITK_TEST_DRIVER itkTestDriver)

SET(JAVA_DRIVER "${ITK_TEST_DRIVER}"
  "${JAVA_RUNTIME}"
  -Djava.library.path=${WrapITK_BINARY_DIR}/lib
)
  

# put all generated files in this list
SET(targets "")
# the required libs
SET(depends "")

MACRO(BUILD_CLASSPATH var)
  SET(${var} ".")
  FOREACH(lib ${ARGN})
    STRING(TOLOWER ${lib} lower_lib)
    SET(${var} "${${var}}:${WrapITK_BINARY_DIR}/lib/org.itk.${lower_lib}.jar")
    SET(depends ${depends} ${lib}JavaJar)
  ENDFOREACH(lib)
ENDMACRO(BUILD_CLASSPATH)


MACRO(BUILD_JAVA_TEST name cp_var)
  # a convenient macro to build java tests
  # 
  BUILD_CLASSPATH(${cp_var} ${ARGN})
  ADD_CUSTOM_COMMAND(
    OUTPUT ${name}.class
    COMMAND ${JAVA_COMPILE}
    ARGS -classpath "${${cp_var}}"
    -d "${CMAKE_CURRENT_BINARY_DIR}" ${CMAKE_CURRENT_SOURCE_DIR}/${name}.java
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${name}.java
    )
  SET(targets ${targets} ${name}.class)
  
ENDMACRO(BUILD_JAVA_TEST)

MACRO(BUILD_JAVA_TEST_BIN name cp_var)
  # a convenient macro to build java tests
  # 
  BUILD_CLASSPATH(${cp_var} ${ARGN})
  ADD_CUSTOM_COMMAND(
    OUTPUT ${name}.class
    COMMAND ${JAVA_COMPILE}
    ARGS -classpath "${${cp_var}}"
    -d "${CMAKE_CURRENT_BINARY_DIR}" ${CMAKE_CURRENT_BINARY_DIR}/${name}.java
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${name}.java
    )
  SET(targets ${targets} ${name}.class)
  
ENDMACRO(BUILD_JAVA_TEST_BIN)




# configure the tests




UNIQUE(types "${WRAP_ITK_SCALAR};UC")
# signed char can't be used to store an image with values up to 255
LIST(REMOVE_ITEM types SC)
FOREACH(d ${WRAP_ITK_DIMS})
  FOREACH(t ${types})

    SET(test_base_name simplePipeline${t}${d})

    SET(WRAP_ITK_JAVA_TEST_IMAGE_DIMENSION ${d})
    SET(WRAP_ITK_JAVA_TEST_PIXEL_TYPE ${t})
    CONFIGURE_FILE("${CMAKE_CURRENT_SOURCE_DIR}/simplePipeline.java.in"
                  "${CMAKE_CURRENT_BINARY_DIR}/${test_base_name}.java"
                  @ONLY IMMEDIATE)

    BUILD_JAVA_TEST_BIN(${test_base_name} class_path IO)

    ADD_TEST(JavaSimplePipeline${t}${d}
      ${JAVA_DRIVER} -classpath ${class_path}
      ${test_base_name}
      ${WrapITK_SOURCE_DIR}/images/cthead1.png
      ${test_base_name}.nrrd
      --compare ${test_base_name}.nrrd ${WrapITK_SOURCE_DIR}/images/cthead1.png
    )

  ENDFOREACH(t)
ENDFOREACH(d)




ADD_CUSTOM_TARGET(JavaTests ALL DEPENDS ${targets})
ADD_DEPENDENCIES(JavaTests ${depends})
