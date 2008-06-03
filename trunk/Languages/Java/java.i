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
*/
// more compact alternative
SWIG_JAVABODY_METHODS(public, public, SWIGTYPE)



//%refobject   itkLightObject "$this->Register();"
//%unrefobject itkLightObject "$this->UnRegister();"


// TODO: always tell swig we're the owner
// TODO: itk classes with no New() must be marked as abstract
%define DECLARE_REF_COUNT_CLASS_JAVA(itkClass)

	// Extend the itk classtype defined for wrapping to simulate a smart pointer in SWIG.
	// Also, make the ctor public to make the 'new' operator available in java
	%extend itkClass {
		public:
		itkClass() {
			typedef ::itk::SmartPointer<itkLightObject> Pointer;
			Pointer smtPtr = itkClass::New();
			itkClass *rawPtr = dynamic_cast<itkClass *>(smtPtr.GetPointer());
			rawPtr->Register();
			return rawPtr;
		};
		~itkClassWrapped() {
			self->UnRegister();
		};
	}
/*
	%typemap(out) itkClass *, itkClass & {
		if (ptrRaw) {
			ptrRaw->Register();
		}
		*(itkClass **)&$result = ptrRaw;
	}
*/	
	%typemap(out) itkClass##_Pointer, itkClass##_Pointer *, itkClass##_Pointer & {
		itkClass* ptrRaw = $1.GetPointer();
		if (ptrRaw) {
			ptrRaw->Register();
		}
		*(itkClass **)&$result = ptrRaw;
	}

	// Do not wrap the corresponding itkSmartPointer
	%ignore itkClass##_Pointer;

// itkvtk, this is temp

//%typemap(jtype) vtkImageData* "vtk.vtkImageData"
%typemap(jstype) vtkImageData* "vtk.vtkImageData"
%typemap(javaout) vtkImageData*{
    long cPtr = itkImageToVTKImageFilterJavaJNI.itkImageToVTKImageFilterIUC2_GetOutput(swigCPtr, this);
    if (cPtr == 0) return null;
    vtk.vtkImageData obj = null;
    java.lang.ref.WeakReference ref = (java.lang.ref.WeakReference)vtk.vtkGlobalJavaHash.PointerToReference.get(new Long(cPtr));
    if (ref != null) {
      obj = (vtk.vtkImageData)ref.get();
    }
    if (obj == null) {
    	vtk.vtkImageData tempObj = new vtk.vtkImageData(cPtr);
      String className = tempObj.GetClassName();
      try {
        Class c = Class.forName("vtk." + className);
        java.lang.reflect.Constructor cons = c.getConstructor(new Class[] {long.class} );
        obj = (vtk.vtkImageData)cons.newInstance(new Object[] {new Long(cPtr)});
      } catch (Exception e) {
        e.printStackTrace();
      }
      tempObj.Delete();
    }
    return obj;
}

/*
//%typemap(out) vtkImageData*, vtkImageImport*, vtkImageExport* {
%typemap(out) vtkImageData*{
  itkClass* ptr = (itkClass*) vtkJavaGetPointerFromObject(jenv, jarg1_);
  *(vtkImageData **)&$result = ptr->GetOutput();
}

arg1 = (itkImageToVTKImageFilterIUC2 *)vtkJavaGetPointerFromObject(jenv,jarg1_);

%typemap(in) vtkImageData* {
  $1 = (itkClass*) $input;
  $1 = (vtkImageData*) vtkJavaGetPointerFromObject(jenv, $1);
  //if ( $1 == NULL ) { SWIG_fail; }
}
*/
// end itkvtk

%enddef