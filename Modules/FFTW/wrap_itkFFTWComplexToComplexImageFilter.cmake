WRAP_CLASS("itk::FFTWComplexToComplexImageFilter" POINTER)
  FOREACH(d ${WRAP_ITK_DIMS})
    IF(WRAP_complex_float)
      WRAP_TEMPLATE("${ITKM_F}${d}" "${ITKT_F},${d}")
    ENDIF(WRAP_complex_float)
  
    IF(WRAP_complex_double)
      WRAP_TEMPLATE("${ITKM_D}${d}" "${ITKT_D},${d}")
    ENDIF(WRAP_complex_double)
  ENDFOREACH(d)
END_WRAP_CLASS()