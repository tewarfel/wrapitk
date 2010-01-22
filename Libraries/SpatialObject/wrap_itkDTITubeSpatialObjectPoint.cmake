WRAP_CLASS("itk::DTITubeSpatialObjectPoint")
#  FOREACH(d ${WRAP_ITK_DIMS})
#    WRAP_TEMPLATE("${d}" "${d}")
#  ENDFOREACH(d)

  # seems to be usable only with dim=3       
  FILTER_DIMS(d 3)                                
  IF(d)
    WRAP_TEMPLATE(3 3)
  ENDIF(3)

END_WRAP_CLASS()
