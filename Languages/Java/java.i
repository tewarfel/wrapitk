%include "typemaps.i"
%include "stl.i"

//######################################################################
// The ignore list

%ignore New();
//%ignore CreateAnother();
%ignore Delete();
%ignore Register();
%ignore UnRegister();

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


// TODO: always tell swig we're the owner
%define DECLARE_REF_COUNT_CLASS_JAVA(itkClass)

	// Extend the itk classtype defined for wrapping to simulate a smart pointer in SWIG.
	// Also, make the ctor public to make the 'new' operator available in java
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

	%typemap(out) itkClass##_Pointer, itkClass##_Pointer *, itkClass##_Pointer & {
		itkClass* ptrRaw = $1.GetPointer();
		if (ptrRaw) {
			ptrRaw->Register();
		}
		*(itkClass **)&$result = ptrRaw;
	}
/*	
	%typemap(in) itkClass##_Pointer, itkClass##_Pointer *, itkClass##_Pointer & {
	}
*/
	// Do not wrap the corresponding itkSmartPointer
	%ignore itkClass##_Pointer;     
%enddef

DECLARE_REF_COUNT_CLASS_JAVA(itkLightObject)

