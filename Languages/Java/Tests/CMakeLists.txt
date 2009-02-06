
# configure the test driver
FIND_PROGRAM(ITK_TEST_DRIVER itkTestDriver)

SET(JAVA_DRIVER "${ITK_TEST_DRIVER}"
  "${JAVA_RUNTIME}"
  -cp ".:${WRAP_ITK_JAVA_BINARY_DIR}/Proxies/src"
  -Djava.library.path=${WrapITK_BINARY_DIR}/lib
)
  

# put all generated files in this list
SET(targets "")

MACRO(BUILD_JAVA_TEST name)
  # a convenient macro to build java tests
  # 
  ADD_CUSTOM_COMMAND(
    OUTPUT ${name}.class
    DEPENDS ${WrapITK_BINARY_DIR}/lib/org.itk.base.jar
    COMMAND ${JAVA_COMPILE}
    ARGS -classpath "${WRAP_ITK_JAVA_BINARY_DIR}/Proxies/src"
    -d "${CMAKE_CURRENT_BINARY_DIR}" ${CMAKE_CURRENT_SOURCE_DIR}/${name}.java
    )
  SET(targets ${targets} ${name}.class)
  
ENDMACRO(BUILD_JAVA_TEST)

MACRO(BUILD_JAVA_TEST2 name)
  # a convenient macro to build java tests
  # 
  ADD_CUSTOM_COMMAND(
    OUTPUT ${name}.class
    DEPENDS ${WrapITK_BINARY_DIR}/lib/org.itk.base.jar
    COMMAND ${JAVA_COMPILE}
    ARGS -classpath "${WRAP_ITK_JAVA_BINARY_DIR}/Proxies/src"
    -d "${CMAKE_CURRENT_BINARY_DIR}" ${CMAKE_CURRENT_BINARY_DIR}/${name}.java
    )
  SET(targets ${targets} ${name}.class)
  
ENDMACRO(BUILD_JAVA_TEST2)




# configure the tests




UNIQUE(types "${WRAP_ITK_SCALAR};UC")
# signed char can't be used to store an image with values up to 255
LIST(REMOVE_ITEM types SC)
FOREACH(JAVA_TEST_IMAGE_DIMENSION ${WRAP_ITK_DIMS})
  FOREACH(JAVA_TEST_PIXEL_TYPE ${types})

    SET(TEST_FILE_NAME simplePipeline${JAVA_TEST_PIXEL_TYPE}${JAVA_TEST_IMAGE_DIMENSION})

    CONFIGURE_FILE("${CMAKE_CURRENT_SOURCE_DIR}/simplePipeline.java.in"
                  "${CMAKE_CURRENT_BINARY_DIR}/${TEST_FILE_NAME}.java"
                  @ONLY IMMEDIATE)

    ADD_CUSTOM_COMMAND(
      OUTPUT ${TEST_FILE_NAME}.class
      COMMAND ${JAVA_COMPILE}
      ARGS -classpath "${WRAP_ITK_JAVA_BINARY_DIR}/Proxies/src" -d "${CMAKE_CURRENT_BINARY_DIR}" ${CMAKE_CURRENT_BINARY_DIR}/${TEST_FILE_NAME}.java
      DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${TEST_FILE_NAME}.java
    )

    ADD_TEST(JavaSimplePipeline${JAVA_TEST_PIXEL_TYPE}${JAVA_TEST_IMAGE_DIMENSION}
      ${JAVA_DRIVER}
      ${TEST_FILE_NAME}
      ${WrapITK_SOURCE_DIR}/images/cthead1.png
      ${TEST_FILE_NAME}.nrrd
      --compare ${TEST_FILE_NAME}.nrrd ${WrapITK_SOURCE_DIR}/images/cthead1.png
    )
    
    SET(targets ${targets} ${TEST_FILE_NAME}.class)

  ENDFOREACH(JAVA_TEST_PIXEL_TYPE)
ENDFOREACH(JAVA_TEST_IMAGE_DIMENSION)




ADD_CUSTOM_TARGET(JavaTests ALL DEPENDS ${targets})

