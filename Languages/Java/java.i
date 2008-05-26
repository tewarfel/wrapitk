%include "typemaps.i"
%include "stl.i"

/*
// By default swig-java creates protected constructors, when breaking the wrapping into
// packages, we need public, ie visible, constructors.
//%typemap(javabody) SWIGTYPE, SWIGTYPE *, SWIGTYPE &, SWIGTYPE [], SWIGTYPE (CLASS::*)
%typemap(javabody) SWIGTYPE
%{
  private long swigCPtr;
  private boolean swigCMemOwn;

  public $javaclassname(long cPtr, boolean cMemoryOwn) {
    swigCPtr = cPtr;
    swigCMemOwn = cMemoryOwn;
  }

  public static long getCPtr($javaclassname obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }
%}
// And the ame goes for the derived classes
//%typemap(javabody_derived) SWIGTYPE, SWIGTYPE *, SWIGTYPE &, SWIGTYPE [], SWIGTYPE (CLASS::*)
%typemap(javabody_derived) SWIGTYPE
%{
  private long swigCPtr;
  private boolean swigCMemOwn;

  public $javaclassname(long cPtr, boolean cMemoryOwn) {
	  super(cPtr, cMemoryOwn);
  }
%}
// more compact alternative
SWIG_JAVABODY_METHODS(public, public, SWIGTYPE)
*/

//%refobject   itkLightObject "$this->Register();"
//%unrefobject itkLightObject "$this->UnRegister();"


%define DECLARE_REF_COUNT_CLASS_JAVA(itkClass)

	// Extend the itk classtype defined for wrapping to simulate a smart pointer in SWIG 
	%extend itkClass {
		public:
		itkClass() {
			typedef ::itk::SmartPointer<itkClass> Pointer;
			Pointer smtPtr = itkClass::New();
			smtPtr.GetPointer()->Register();
			return smtPtr.GetPointer();
		};
		~itkClassWrapped() {
			self->UnRegister();
		};
	}

     
%enddef

DECLARE_REF_COUNT_CLASS_JAVA(itkLightObject)
