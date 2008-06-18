%module pyBasePython

%include <exception.i>
%include <typemaps.i>

%include std_iostream.i
%include std_sstream.i
%include <std_vector.i>
%include <std_map.i>
%include <std_list.i>

%exception {
  try {
    $action
  } catch (const std::out_of_range& e) {
    SWIG_exception_fail(SWIG_IndexError, e.what());
//  } catch (Swig::DirectorException &e) {
//    SWIG_fail;
  } catch (const std::exception& e) {
    SWIG_exception_fail(SWIG_RuntimeError, e.what());
  }
}

//%feature("director:except") {
//    if ($error != NULL) {
//        throw Swig::DirectorMethodException();
//    }
//}

%typemap(out)  unsigned char &, const unsigned char &, signed char &, const signed char &, unsigned short &, const unsigned short &, signed short &, const signed short &, unsigned int &, const unsigned int &, signed int &, const signed int &, signed long &, const signed long &, unsigned long &, const unsigned long &
  {$result = PyInt_FromLong( *$1 );}

%typemap(out) float &, const float &, double &, const double &
  {$result = PyFloat_FromDouble( *$1 );}

// ignore reference count management
%ignore Delete;
%ignore SetReferenceCount;
%ignore Register;
%ignore UnRegister;


// some code from stl

%template(vectorstring)   std::vector< std::string >;
%template(liststring)     std::list< std::string >;

%template(mapULD)         std::map< unsigned long, double >;

%template(vectorB)        std::vector< bool >;
%template(vectorvectorB)  std::vector< std::vector< bool > >;
%template(vectorUC)       std::vector< unsigned char >;
%template(vectorvectorUC) std::vector< std::vector< unsigned char > >;
%template(vectorUS)       std::vector< unsigned short >;
%template(vectorvectorUS) std::vector< std::vector< unsigned short > >;
%template(vectorUL)       std::vector< unsigned long >;
%template(vectorvectorUL) std::vector< std::vector< unsigned long > >;
%template(vectorSC)       std::vector< signed char >;
%template(vectorvectorSC) std::vector< std::vector< signed char > >;
%template(vectorSS)       std::vector< signed short >;
%template(vectorvectorSS) std::vector< std::vector< signed short > >;
%template(vectorSL)       std::vector< signed long >;
%template(vectorvectorSL) std::vector< std::vector< signed long > >;
%template(vectorF)        std::vector< float >;
%template(vectorvectorF)  std::vector< std::vector< float > >;
%template(vectorD)        std::vector< double >;
%template(vectorvectorD)  std::vector< std::vector< double > >;
