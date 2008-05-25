# Copyright 2004 Roman Yakovenko.
# Distributed under the Boost Software License, Version 1.0. (See
# accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

import os
import sys
import config
import pygccxml.utils
import linker
import scanner
import declarations_cache
import patcher
from pygccxml.declarations import *
from pygccxml import utils 

class gccxml_runtime_error_t( RuntimeError ):
    def __init__( self, msg ):
        RuntimeError.__init__( self, msg )


def bind_aliases( decls ):
    """
    This function binds between class and it's typedefs. 

    @param decls: list of all declarations
    @type all_classes: list of L{declaration_t} items

    @return: None
    """   
    visited = set()
    typedefs = filter( lambda decl: isinstance( decl, typedef_t ), decls )
    for decl in typedefs:
        type_ = remove_alias( decl.type )
        if not isinstance( type_, declarated_t ):
            continue
        cls_inst = type_.declaration
        if not isinstance( cls_inst, class_t ):
            continue
        if id( cls_inst ) not in visited:
            visited.add( id( cls_inst ) )
            del cls_inst.aliases[:]
        cls_inst.aliases.append( decl )

class source_reader_t:
    """
    This class reads C++ source code and returns declarations tree.
    
    This class is the only class that have an intime knowledge about GCC-XML. 
    It has only one responsibility: it calls GCC-XML with a source file specified
    by user and creates declarations tree. The implementation of this class is split
    to 2 classes: 
    
    1. L{scanner_t} - this class scans the "XML" file, generated by GCC-XML and 
       creates `pygccxml`_ declarations and types classes. After the xml file has 
       been processed declarations and type class instances keeps references to 
       each other using GCC-XML generated id's. 

    2. L{linker_t} - this class contains logic for replacing GCC-XML generated 
       ids with references to declarations or type class instances.
    """
    def __init__( self, config, cache=None, decl_factory=None ):
        """
        @param config: instance of L{config_t} class, that contains GCC-XML 
                       configuration
        @type config: L{config_t}
        
        @param cache: reference to cache object, that will be updated after 
                      file has been parsed.
        @param cache: instance of class, that derives from {cache_base_t}
                            
        @param decl_factory: declarations factory, if not given default 
                             declarations factory L{decl_factory_t} will be used
        """
        self.logger = utils.loggers.gccxml
        self.__search_directories = []
        self.__config = config
        self.__search_directories.append( config.working_directory )
        self.__search_directories.extend( config.include_paths )
        if not cache:
            cache = declarations_cache.dummy_cache_t()
        self.__dcache = cache
        self.__raise_on_wrong_settings()
        self.__decl_factory = decl_factory
        if not decl_factory:
            self.__decl_factory = decl_factory_t()

    def __raise_on_wrong_settings(self):
        # won't raise anything
        return
        
        if not os.path.isfile( self.__config.gccxml_path ):
            if sys.platform == 'win32':
                gccxml_name = 'gccxml' + '.exe'
                environment_var_delimiter = ';'
            elif sys.platform == 'linux2' or sys.platform == 'darwin':
                gccxml_name = 'gccxml'
                environment_var_delimiter = ':'
            else:
                raise RuntimeError( 'unable to find out location of gccxml' )
            may_be_gccxml = os.path.join( self.__config.gccxml_path, gccxml_name )
            if os.path.isfile( may_be_gccxml ):
                self.__config.gccxml_path = may_be_gccxml
            else:
                for path in os.environ['PATH'].split( environment_var_delimiter ):
                    gccxml_path = os.path.join( path, gccxml_name )
                    if os.path.isfile( gccxml_path ):
                        self.__config.gccxml_path = gccxml_path
                        break
                else:
                    msg = 'gccxml_path("%s") should exists or to be a valid file name.' \
                          % self.__config.gccxml_path
                    # raise RuntimeError( msg )
        if not os.path.isdir( self.__config.working_directory ):
            msg = 'working_directory("%s") should exists or to be a valid directory name.' \
                  % self.__config.working_directory
            raise RuntimeError( msg )
        for include_path in self.__config.include_paths:
            if not os.path.isdir( include_path ):
                msg = 'include path "%s" should exists or to be a valid directory name.' \
                      % include_path
                raise RuntimeError( msg )

    def __create_command_line(self, file, xmlfile):
        assert isinstance( self.__config, config.config_t )
        #returns
        cmd = []
        #first is gccxml executable
        if 'win32' in sys.platform:
            cmd.append( '"%s"' % os.path.normpath( self.__config.gccxml_path ) )
        else:
            cmd.append(  '%s' % os.path.normpath( self.__config.gccxml_path ) )
        # Add all cflags passed
        if self.__config.cflags != "":
            cmd.append(" %s "%self.__config.cflags)
        #second all additional includes directories
        cmd.append( ''.join( [' -I"%s"' % search_dir for search_dir in self.__search_directories] ) )
        #third all additional defined symbols
        cmd.append( ''.join( [' -D"%s"' % defined_symbol for defined_symbol in self.__config.define_symbols] ) )
        cmd.append( ''.join( [' -U"%s"' % undefined_symbol for undefined_symbol in self.__config.undefine_symbols] ) )
        #fourth source file
        cmd.append( '"%s"' % file )
        #five destination file
        cmd.append( '-fxml="%s"' % xmlfile )
        if self.__config.start_with_declarations:
            cmd.append( '-fxml-start="%s"' % ','.join( self.__config.start_with_declarations ) )
        
        cmd_line = ' '.join(cmd)
        if 'win32' in sys.platform :
            cmd_line = '"%s"' % cmd_line
        self.logger.info( 'gccxml cmd: %s' % cmd_line )
        return cmd_line

    def create_xml_file( self, header, destination=None ):
        """
        This function will return the file name of the file, created by GCC-XML
        for "header" file. If destination_file_path is not None, then this file 
        path will be used and returned.

        @param header: path to source file, that should be parsed
        @type header: str
        
        @param destination: if given, will be used as target file/path for 
                            GCC-XML generated file.
        @type destination: str
                            
        @return: path to GCC-XML generated file
        """
        gccxml_file = destination
        # If file specified, remove it to start else create new file name
        if gccxml_file:
            pygccxml.utils.remove_file_no_raise( gccxml_file )
        else:
            gccxml_file = pygccxml.utils.create_temp_file_name( suffix='.xml' )
        try:
            ffname = header
            if not os.path.isabs( ffname ):
                  ffname = self.__file_full_name(header)
            command_line = self.__create_command_line( ffname, gccxml_file )
            input_, output = os.popen4( command_line )
            input_.close()
            gccxml_reports = []
            while True:
                  data = output.readline()
                  gccxml_reports.append( data )
                  if not data:
                       break
            exit_status = output.close()
            gccxml_msg = ''.join(gccxml_reports)
            if self.__config.ignore_gccxml_output:
                if not os.path.isfile(gccxml_file):
                    raise gccxml_runtime_error_t( "Error occured while running GCC-XML: %s status:%s" % (gccxml_msg, exit_status) )
            else:
                if gccxml_msg or exit_status or not os.path.isfile(gccxml_file):
                    raise gccxml_runtime_error_t( "Error occured while running GCC-XML: %s" % gccxml_msg )
        except Exception, error:
            pygccxml.utils.remove_file_no_raise( gccxml_file )
            raise error
        return gccxml_file

    def create_xml_file_from_string( self, content, destination=None ):
        """
        Creates XML file from text.
        
        @param content: C++ source code
        @type content: str
        
        @param destination: file name for GCC-XML generated file
        @type destination: str
        
        @return: returns file name of GCC-XML generated file
        """
        header_file = pygccxml.utils.create_temp_file_name( suffix='.h' )
        gccxml_file = None
        try:
            header_file_obj = file(header_file, 'w+')
            header_file_obj.write( content )
            header_file_obj.close()
            gccxml_file = self.create_xml_file( header_file, destination )
        finally:
            pygccxml.utils.remove_file_no_raise( header_file )
        return gccxml_file
        
    def read_file(self, source_file):
        """
        Reads C++ source file and returns declarations tree
        
        @param source_file: path to C++ source file
        @type source_file: str
        """
        declarations, types = None, None
        gccxml_file = ''
        try:
            ffname = self.__file_full_name(source_file)
            self.logger.debug( "Reading source file: [%s]." % ffname )
            declarations = self.__dcache.cached_value( ffname, self.__config )
            if not declarations:
                self.logger.debug( "File has not been found in cache, parsing..." )
                gccxml_file = self.create_xml_file( ffname )
                declarations, files = self.__parse_gccxml_created_file( gccxml_file )
                self.__dcache.update( ffname, self.__config, declarations, files )
            else:
                self.logger.debug( "File has not been changed, reading declarations from cache." )
        except Exception, error:
            if gccxml_file:
                pygccxml.utils.remove_file_no_raise( gccxml_file )
            raise error
        if gccxml_file:
            pygccxml.utils.remove_file_no_raise( gccxml_file )
        return declarations

    def read_xml_file(self, gccxml_created_file):
        """
        Reads GCC-XML generated XML file.
        
        @param gccxml_created_file: path to GCC-XML generated file
        @type gccxml_created_file: str
        
        @return: declarations tree
        """
        self.logger.debug( "Reading xml file: [%s]" % gccxml_created_file )

        declarations, files = self.__parse_gccxml_created_file( gccxml_created_file )
        return declarations

    def read_string(self, content):
        """
        Reads Python string, that contains valid C++ code, and returns
        declarations tree.
        """
        header_file = pygccxml.utils.create_temp_file_name( suffix='.h' )
        header_file_obj = file(header_file, 'w+')
        header_file_obj.write( content )
        header_file_obj.close()
        declarations = None
        try:
            declarations = self.read_file( header_file )
        except Exception, error:
            pygccxml.utils.remove_file_no_raise( header_file )
            raise error
        pygccxml.utils.remove_file_no_raise( header_file )
        return declarations

    def __file_full_name( self, file ):
        if os.path.isfile( file ):
            return file
        for path in self.__search_directories:
            file_path = os.path.join( path, file )
            if os.path.isfile( file_path ):
                  return file_path
        raise RuntimeError( "pygccxml error: file '%s' does not exist" % file )

    def __produce_full_file( self, file_path ):
        if os.path.isabs( file_path ):
            return file_path
        try:
            abs_file_path = os.path.realpath( os.path.join( self.__config.working_directory, file_path ) )
            if os.path.exists( abs_file_path ):
                return os.path.normpath( abs_file_path )
            return file_path
        except Exception:
            return file_path
        
    def __parse_gccxml_created_file( self, gccxml_file ):
        scanner_ = scanner.scanner_t( gccxml_file, self.__decl_factory )
        scanner_.read()
        decls = scanner_.declarations()
        types = scanner_.types()
        files = {}
        for file_id, file_path in scanner_.files().items():
            files[file_id] = self.__produce_full_file(file_path)
        linker_ = linker.linker_t( decls=decls
                                   , types=types
                                   , access=scanner_.access()
                                   , membership=scanner_.members()
                                   , files=files )
        for type_ in list( types.itervalues() ):
            #I need this copy because internaly linker change types collection
            linker_.instance = type_
            apply_visitor( linker_, type_ )
        for decl in decls.itervalues():
            linker_.instance = decl
            apply_visitor( linker_, decl )
        bind_aliases( decls.itervalues() )
        decls = filter( lambda inst: isinstance(inst, declaration_t) and not inst.parent, decls.itervalues() )
        #some times gccxml report typedefs defined in no namespace
        #it happens for example in next situation
        #template< typename X>
        #void ddd(){ typedef typename X::Y YY;}
        decls = filter( lambda inst: isinstance( inst, namespace_t ), decls )
        decls = patcher.patch_it( decls )
        decls_all = make_flatten( decls )
        for decl in decls_all:
            if decl.location:
                decl.location.file_name = self.__produce_full_file( decl.location.file_name )
        return ( decls, files.values() )

