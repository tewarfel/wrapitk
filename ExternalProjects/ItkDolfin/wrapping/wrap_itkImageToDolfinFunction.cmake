WRAP_CLASS("itk::ImageToDolfinFunction")
  UNIQUE(types "${WRAP_ITK_SCALAR};UC")
  WRAP_IMAGE_FILTER("${types}" 1)
END_WRAP_CLASS()
