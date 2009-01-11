WRAP_CLASS("itk::Size")
  UNIQUE(dims "${WRAP_ITK_DIMS};1")
  FOREACH(d ${dims})
    WRAP_TEMPLATE(${d} ${d})
  ENDFOREACH(d)  
END_WRAP_CLASS()