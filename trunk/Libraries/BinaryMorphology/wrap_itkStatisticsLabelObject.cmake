WRAP_CLASS("itk::StatisticsLabelObject" POINTER)
  FOREACH(d ${WRAP_ITK_DIMS})
    WRAP_TEMPLATE("${ITKM_UL}${d}" "${ITKT_UL},${d}")
  ENDFOREACH(d)
END_WRAP_CLASS()
