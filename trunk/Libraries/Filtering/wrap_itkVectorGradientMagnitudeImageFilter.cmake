WRAP_CLASS("itk::VectorGradientMagnitudeImageFilter" POINTER)
  foreach(d ${WRAP_ITK_DIMS})
    foreach(t ${WRAP_ITK_VECTOR_REAL})
      foreach(t2 ${WRAP_ITK_REAL})
        WRAP_TEMPLATE("${ITKM_I${t}${d}${d}}${ITKM_${t2}}"
"${ITKT_I${t}${d}${d}}, ${ITKT_${t2}}")
      endforeach(t2)
    endforeach(t)
  endforeach(d)
END_WRAP_CLASS()