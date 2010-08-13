WRAP_CLASS("itk::DTITubeSpatialObject" POINTER_WITH_SUPERCLASS)
#  FOREACH(d ${WRAP_ITK_DIMS})
#    WRAP_TEMPLATE(${d} ${d})
#  ENDFOREACH(d)

  # seems to be usable only with dim=3
  FILTER_DIMS(d 3)
  IF(d)
    WRAP_TEMPLATE(3 3)
  ENDIF(d)

END_WRAP_CLASS()
