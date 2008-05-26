// Common SWIG support applied to all languages.

//######################################################################
// The ignore list

// win32
%ignore itkLightObject::operator new(size_t n);
%ignore itkLightObject::operator new[](size_t n);
%ignore itkLightObject::operator delete(void* m);
%ignore itkLightObject::operator delete[](void* m, size_t);


/*
//######################################################################
// This macro replaces the use of itk::SmartPointer.
// short_name is class name without namespace qualifiers, whereas qual_name includes them.
// Reference: http://www.nabble.com/attachment/16653644/0/SwigRefCount.i
%define DECLARE_REF_COUNT_CLASS(short_name, qual_name)

	// pointers and references
	%typemap(out) qual_name *, qual_name & {
		// always tell SWIG_NewPointerObj we're the owner
		$result = SWIG_NewPointerObj((void *) $1, $1_descriptor, 1);
		if ($1) {
			$1->Register();
		}
	}

	// return by value
	%typemap(out) qual_name {
		$&1_ltype resultptr;
		resultptr = new $1_ltype(($1_ltype &) $1);
		$result = SWIG_NewPointerObj((void *) resultptr, $&1_descriptor, 1);
		resultptr->Register();
	}

	// make "deletion" in scripting language just decrement ref. count
	%extend qual_name {
		public:
		~short_name() {self->UnRegister();};
	}

	%ignore qual_name::~short_name;
     
%enddef
*/

