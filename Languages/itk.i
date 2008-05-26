// Common SWIG support applied to all languages.

//######################################################################
// The ignore list

// win32
// TODO: this would be wrapped between #ifdefs 
%ignore itkLightObject::operator new(size_t n);
%ignore itkLightObject::operator new[](size_t n);
%ignore itkLightObject::operator delete(void* m);
%ignore itkLightObject::operator delete[](void* m, size_t);


// TODO: This macro is not for all languages, it works for python.
// Maybe should be moved to python.i and renamed to DECLARE_REF_COUNT_CLASS_PYTHON.
//######################################################################
// This macro replaces the use of itk::SmartPointer.
// class_name is class name without namespace qualifiers.
// Reference: http://www.nabble.com/attachment/16653644/0/SwigRefCount.i
%define DECLARE_REF_COUNT_CLASS(class_name)

	// pointers and references
	%typemap(out) class_name *, class_name & {
		// always tell SWIG_NewPointerObj we're the owner
		$result = SWIG_NewPointerObj((void *) $1, $1_descriptor, 1);
		if ($1) {
			$1->Register();
		}
	}

  // transform smart pointers in raw pointers
	%typemap(out) class_name##_Pointer {
	  // get the raw pointer from the smart pointer
	  class_name * ptr = $1.GetPointer();
		// always tell SWIG_NewPointerObj we're the owner
		$result = SWIG_NewPointerObj((void *) ptr, $descriptor(class_name *), 1);
		// register the object, it it exists
		if (ptr) {
			ptr->Register();
		}
	}

	// make "deletion" in scripting language just decrement ref. count
	%extend class_name {
		public:
		~class_name() {self->UnRegister();};
	}

	%ignore class_name::~class_name;
	
	%ignore class_name##_Pointer;
     
%enddef
