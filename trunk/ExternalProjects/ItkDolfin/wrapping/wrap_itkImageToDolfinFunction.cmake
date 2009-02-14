WRAP_CLASS("itk::ImageToDolfinFunction")
  UNIQUE(types "${WRAP_ITK_SCALAR};D")
  WRAP_IMAGE_FILTER("${types}" 1)
END_WRAP_CLASS()

#WRAP_CLASS("itk::DolfinImageFunctionSpace")
#  UNIQUE(types "${WRAP_ITK_SCALAR};D")
#  WRAP_IMAGE_FILTER("${types}" 1)
#END_WRAP_CLASS()
