WRAP_CLASS("itk::LabelMapToLabelImageFilter" POINTER)
  FOREACH(t ${WRAP_ITK_USIGN_INT})
    FOREACH(d ${WRAP_ITK_DIMS})
      WRAP_TEMPLATE("${ITKM_LM${d}}${ITKM_I${t}${d}}" "${ITKT_LM${d}}, ${ITKT_I${t}${d}}")
    ENDFOREACH(d)
  ENDFOREACH(t)
END_WRAP_CLASS()
