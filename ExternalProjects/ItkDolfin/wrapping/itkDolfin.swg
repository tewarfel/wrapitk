%import dolfin/swig/dolfin.i

%{
#include <dolfin.h>
%}

//%newobject itk::ImageToDolfinFunction< itkDolfinImageFunctionID2 >.DolfinImageFunction;

/*
#define SWIG_SHARED_PTR_NAMESPACE std::tr1
%include <shared_ptr.i>
SWIG_SHARED_PTR(ImageToDolfinFunctionID2,dolfin::Function)
*/

//%import dolfin/swig/dolfin_shared_ptr_classes.i

#define SWIG_SHARED_PTR_NAMESPACE std
#define SWIG_SHARED_PTR_SUBNAMESPACE tr1
%include "boost_shared_ptr.i"

# if defined(SWIG_SHARED_PTR_QNAMESPACE)
SWIG_SHARED_PTR(Function,dolfin::Function)
SWIG_SHARED_PTR(itk::ImageToDolfinFunctionID2,dolfin::Function)
SWIG_SHARED_PTR(ImageToDolfinFunctionID2,dolfin::Function)
SWIG_SHARED_PTR(itkImageToDolfinFunctionID2,dolfin::Function)
SWIG_SHARED_PTR(itk::itkImageToDolfinFunctionID2,dolfin::Function)
#endif
