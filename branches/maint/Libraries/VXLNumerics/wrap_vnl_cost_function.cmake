# auto include feature must be disable because the class is not in the file
# with the same name
SET(WRAPPER_AUTO_INCLUDE_HEADERS OFF)
WRAP_INCLUDE("vnl/vnl_cost_function.h")

WRAP_NON_TEMPLATE_CLASS("vnl_cost_function" FORCE_INSTANTIATE)
