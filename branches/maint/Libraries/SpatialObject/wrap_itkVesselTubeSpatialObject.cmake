WRAP_CLASS("itk::VesselTubeSpatialObject" POINTER_WITH_SUPERCLASS)
  FOREACH(d ${WRAP_ITK_DIMS})
    WRAP_TEMPLATE(${d} ${d})
  ENDFOREACH(d)
END_WRAP_CLASS()
