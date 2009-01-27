# new features introduced by itk module
# each new feature use a name in lower case
clrLine = "\033[2K\033E\033[1A"

def auto_not_in_place( v=True ) :
  """Force it to not run in place
  """
  import itkConfig
  itkConfig.NotInPlace = v

def auto_progress( progressType = 1 ):
  """Set up auto progress report
  
  progressType:
    1 or True  -> auto progress be used in a terminal
    2          -> simple auto progress (without special characters)
    0 or False -> disable auto progress
  """
  import itkConfig
  
  if progressType == True or progressType == 1 :
    itkConfig.ImportCallback = terminal_import_callback
    itkConfig.ProgressCallback = terminal_progress_callback
    
  elif progressType == 2 :
    itkConfig.ImportCallback = simple_import_callback
    itkConfig.ProgressCallback = simple_progress_callback
    
  elif progressType == False or progressType == 0 :
    itkConfig.ImportCallback = None
    itkConfig.ProgressCallback = None
    
  else:
    raise ValueError("Invalid auto progress type: "+repr(progressType))
  
def terminal_progress_callback(name, p):
  """Display the progress of an object and clean the display once complete
  
  This function can be used with itkConfig.ProgressCallback
  """
  import sys
  print >> sys.stderr, clrLine+"%s: %f" % (name, p),
  if p == 1 :
    print >> sys.stderr, clrLine,
  
def terminal_import_callback(name, p):
  """Display the loading of a module and clean the display once complete
  
  This function can be used with itkConfig.ImportCallback
  """
  import sys
  print >> sys.stderr, clrLine+"Loading %s..." % name,
  if p == 1 :
    print >> sys.stderr, clrLine,
  
def simple_import_callback(name, p):
  """Print a message when a module is loading
  
  This function can be used with itkConfig.ImportCallback
  """
  import sys
  if p == 0:
    print >> sys.stderr, "Loading %s..." % name,
  elif p == 1 :
    print >> sys.stderr, "done"

def simple_progress_callback(name, p):
  """Print a message when an object is running
  
  This function can be used with itkConfig.ProgressCallback
  """
  import sys
  if p == 0 :
    print >> sys.stderr, "Running %s..." % name,
  elif p == 1 :
    print >> sys.stderr, "done"


def force_load():
  """force itk to load all the submodules"""
  import itk
  for k in dir(itk):
    getattr(itk, k)


import sys
def echo(object, f=sys.stderr) :
   """Print an object is f
   
   If the object has a method Print(), this method is used.
   repr(object) is used otherwise
   """
   import itk
   ss = itk.stringstream()
   try :
      try:
         object.Print(ss)
      except:
         object.Print(ss, Indent.New());
   except:
      print >> f, repr(object)
   else:
      print >> f, ss.str()
del sys


def size(imageOrFilter) :
  """Return the size of an image, or of the output image of a filter
  
  This method take care of updating the needed informations
  """
  # we don't need the entire output, only its size
  imageOrFilter.UpdateOutputInformation()
  img = output(imageOrFilter)
  return img.GetLargestPossibleRegion().GetSize()
  

def physical_size(imageOrFilter) :
  """Return the physical size of an image, or of the output image of a filter
  
  This method take care of updating the needed informations
  """
  from __builtin__ import range # required because range is overladed in this module
  spacing_ = spacing(imageOrFilter)
  size_ = size(imageOrFilter)
  result = []
  for i in range(0, spacing_.Size()):
    result.append( spacing_.GetElement(i) * size_.GetElement(i) )
  return result


def spacing(imageOrFilter) :
  """Return the spacing of an image, or of the output image of a filter
  
  This method take care of updating the needed informations
  """
  # we don't need the entire output, only its size
  imageOrFilter.UpdateOutputInformation()
  img = output(imageOrFilter)
  return img.GetSpacing()
  

def origin(imageOrFilter) :
  """Return the origin of an image, or of the output image of a filter
  
  This method take care of updating the needed informations
  """
  # we don't need the entire output, only its size
  imageOrFilter.UpdateOutputInformation()
  img = output(imageOrFilter)
  return img.GetOrigin()
  

def index(imageOrFilter) :
  """Return the index of an image, or of the output image of a filter
  
  This method take care of updating the needed informations
  """
  # we don't need the entire output, only its size
  imageOrFilter.UpdateOutputInformation()
  img = output(imageOrFilter)
  return img.GetLargestPossibleRegion().GetIndex()
  

def region(imageOrFilter) :
  """Return the region of an image, or of the output image of a filter
  
  This method take care of updating the needed informations
  """
  # we don't need the entire output, only its size
  imageOrFilter.UpdateOutputInformation()
  img = output(imageOrFilter)
  return img.GetLargestPossibleRegion()
  

def strel(dim, radius=1) :
  """A method to create a ball structuring element
  """
  import itk
  import sys
  # print >> sys.stderr, "strel() is deprecated and will be removed in the next release" 
  return itk.FlatStructuringElement[dim].Ball(radius)
  
# return an image
from itkTemplate import image, output


def template(cl) :
  """Return the template of a class (or of the class of an object) and its parameters
  
  template() returns a tuple with 2 elements:
    - the first one is the itkTemplate object
    - the second is a tuple containing the template parameters
  """
  from itkTemplate import itkTemplate
  return itkTemplate.__class_to_template__[class_(cl)]
  

def ctype(s) :
  """Return the c type corresponding to the string passed in parameter
   
  The string can contain some extra spaces.
  see also itkCType
  """
  from itkTypes import itkCType
  ret = itkCType.GetCType(" ".join(s.split()))
  if ret == None :
    raise KeyError("Unrecognized C type '%s'" % s)
  return ret
  

def class_(obj) :
  """Return a class from an object
  
  Often in itk, the __class__ is not what the user is expecting.
  class_() should do a better job
  """
  import inspect
  if inspect.isclass(obj) :
    # obj is already a class !
    return obj
  else :
    return obj.__class__


def range(imageOrFilter) :
  """Return the range of values in a image of in the output image of a filter
  
  The minimum and maximum values are returned in a tuple: (min, max)
  range() take care of updating the pipeline
  """
  import itk
  img = output(imageOrFilter)
  img.UpdateOutputInformation()
  img.Update()
  # don't put that calculator in the automatic pipeline
  tmp_auto_pipeline = auto_pipeline.current
  auto_pipeline.current = None
  comp = itk.MinimumMaximumImageCalculator[img].New(Image=img)
  auto_pipeline.current = tmp_auto_pipeline
  comp.Compute()
  return (comp.GetMinimum(), comp.GetMaximum())


def write(imageOrFilter, fileName, compression=False):
  """Write a image or the output image of a filter to filename
  
  The writer is instantiated with the image type of the image in
  parameter (or, again, with the output image of the filter in parameter)
  """
  import itk
  img = output(imageOrFilter)
  img.UpdateOutputInformation()
  # don't put that writer in the automatic pipeline
  tmp_auto_pipeline = auto_pipeline.current
  auto_pipeline.current = None
  writer = itk.ImageFileWriter[img].New(Input=img, FileName=fileName, UseCompression=compression)
  auto_pipeline.current = tmp_auto_pipeline
  writer.Update()
  

def index_to_physical_point( imageOrFilter, idx ):
  """Get the pysical point in an image from an index
  
  imageOrFilter is the image where the physical point must be computed
  idx is the index used to compute the physical point. It can be a continuous index.
  """
  from __builtin__ import range # required because range is overladed in this module
  # get the image if needed
  img = output( imageOrFilter )
  dim = img.GetImageDimension()
  o = origin( img )
  s = spacing( img )
  
  # use the typemaps to really get a continuous index
  import itk
  idx = itk.ContinuousIndex[ itk.D, dim ]( idx )
  
  # create the output object
  p = itk.Point[ itk.D, dim ]()
  for i in range( 0, dim ):
    p[i] = s[i] * idx[i] + o[i]
  return p
  

def physical_point_to_continuous_index( imageOrFilter, p ):
  """Get the continuous index in an image from the physical point
  
  imageOrFilter is the image where the physical point must be computed
  p is the point used to compute the index
  """
  from __builtin__ import range # required because range is overladed in this module
  # get the image if needed
  img = output( imageOrFilter )
  dim = img.GetImageDimension()
  o = origin( img )
  s = spacing( img )
  
  # use the typemaps to really get a point
  import itk
  p = itk.Point[ itk.D, dim ]( p )
  
  # create the output object
  idx = itk.ContinuousIndex[ itk.D, dim ]()
  for i in range( 0, dim ):
    idx.SetElement( i, ( p[i] - o[i] ) / s[i] )
  return idx
  

def physical_point_to_index( imageOrFilter, p ):
  """Get the index in an image from the physical point
  
  image is the image where the physical point must be computed
  p is the point used to compute the index
  """
  from __builtin__ import range # required because range is overladed in this module
  # get the image if needed
  img = output( imageOrFilter )
  dim = img.GetImageDimension()
  o = origin( img )
  s = spacing( img )
  
  # use the typemaps to really get a point
  import itk
  p = itk.Point[ itk.D, dim ]( p )
  
  # create the output object
  idx = itk.Index[ dim ]()
  for i in range( 0, dim ):
    idx.SetElement( i, int( round( ( p[i] - o[i] ) / s[i] ) ) )
  return idx


def search( s, case_sensitive=False): #, fuzzy=True):
  """Search for a class name in itk module.
  """
  s = s.replace(" ", "")
  if not case_sensitive:
    s = s.lower()
  import itk
  names = dir(itk)
  names.sort()
  # exact match first
  if case_sensitive:
    res = [n for n in names if s == n]
  else:
    res = [n for n in names if s == n.lower()]
  # then exact match inside the name
  if case_sensitive:
    res += [n for n in names if s in n and s != n]
  else:
    res += [n for n in names if s in n.lower() and s != n.lower()]
#   if fuzzy:
#     try:
#       # everything now requires editdist
#       import editdist
#       if case_sensitive:
#         res.sort(key=lambda x: editdist.distance(x, s))
#       else:
#         res.sort(key=lambda x: (editdist.distance(x.lower(), s), x))
#     except:
#       pass
  return res


def set_inputs( newItkObject, args, kargs ):
  # try to get the images from the filters in args
  args = [output(arg) for arg in args]
  
  # args without name are filter used to set input image
  #
  # count SetInput calls to call SetInput, SetInput2, SetInput3, ...
  # usefull with filter which take 2 input (or more) like SubstractImageFiler
  # Ex: substract image2.png to image1.png and save the result in result.png
  # r1 = itk.ImageFileReader.US2.New(FileName='image1.png')
  # r2 = itk.ImageFileReader.US2.New(FileName='image2.png')
  # s = itk.SubtractImageFilter.US2US2US2.New(r1, r2)
  # itk.ImageFileWriter.US2.New(s, FileName='result.png').Update()
  try :
    for setInputNb, arg  in enumerate(args) :
      methodName = 'SetInput%i' % (setInputNb+1)
      if methodName in dir(newItkObject) :
        # first try to use methods called SetInput1, SetInput2, ...
        # those method should have more chances to work in case of multiple
        # input types
        getattr(newItkObject, methodName)(arg)
      else :
        # no method called SetInput?
        # try with the standard SetInput(nb, input)
        newItkObject.SetInput(setInputNb, arg)
  except TypeError, e :
    # the exception have (at least) to possible reasons:
    # + the filter don't take the input number as first argument
    # + arg is an object of wrong type
    # 
    # if it's not the first input, re-raise the exception
    if setInputNb != 0 :
      raise e
    # it's the first input, try to use the SetInput() method without input number
    newItkObject.SetInput(args[0])
    # but raise an exception if there is more than 1 argument
    if len(args) > 1 :
      raise TypeError('Object accept only 1 input.')
  except AttributeError :
    # There is no SetInput() method, try SetImage
    # but before, check the number of inputs
    if len(args) > 1 :
      raise TypeError('Object accept only 1 input.')
    methodList = ['SetImage', 'SetInputImage']
    methodName = None
    for m in methodList:
      if m in dir(newItkObject):
        methodName = m
    if methodName :
      getattr(newItkObject, methodName)(args[0])
    else:
      raise AttributeError('No method found to set the input.')
    
  # named args : name is the function name, value is argument(s)
  for attribName, value in kargs.iteritems() :
    # use Set as prefix. It allow to use a shorter and more intuitive
    # call (Ex: itk.ImageFileReader.UC2.New(FileName='image.png')) than with the
    # full name (Ex: itk.ImageFileReader.UC2.New(SetFileName='image.png'))
    if attribName != "auto_progress" :
      attrib = getattr(newItkObject, 'Set' + attribName)
      attrib(value)


def show(input, **kargs) :
  """display an image
  """
  import itk
  img = output(input)
  if img.GetImageDimension() == 3 and "show3D" in dir(itk):
	  return itk.show3D(input, **kargs)
  else :
	  # print "2D not supported yet, use the 3D viewer."
	  return show2D(input, **kargs)
    
class show2D :
  """Display a 2D image
  """
  def __init__(self, imageOrFilter, Label=False) :
    import tempfile, itk, os
    # get some data from the environment
    command = os.environ.get("WRAPITK_SHOW2D_COMMAND", "imview %s -fork")
    label_command = os.environ.get("WRAPITK_SHOW2D_LABEL_COMMAND", "imview %s -c regions.lut -fork")
    compress = os.environ.get("WRAPITK_SHOW2D_COMPRESS", "true").lower() in ["on", "true", "yes", "1"]
    extension = os.environ.get("WRAPITK_SHOW2D_EXTENSION", ".tif")
    # use the tempfile module to get a non used file name and to put
    # the file at the rignt place
    self.__tmpFile__ = tempfile.NamedTemporaryFile(suffix=extension)
    # get an updated image
    img = output(imageOrFilter)
    img.UpdateOutputInformation()
    img.Update()
    # change the LabelMaps to an Image, so we can look at them easily
    if 'LabelMap' in dir(itk) and img.GetNameOfClass() == 'LabelMap':
      # retreive the biggest label in the label map
      maxLabel = img.GetNthLabelObject( img.GetNumberOfLabelObjects() - 1 ).GetLabel()
      # search for a filter to convert the label map
      label_image_type = sorted( [params[1] for params in itk.LabelMapToLabelImageFilter.keys() if params[0] == class_(img) and itk.NumericTraits[itk.template(params[1])[1][0]].max() >= maxLabel ] )[0]
      convert = itk.LabelMapToLabelImageFilter[ img, label_image_type ].New( img )
      convert.Update()
      img = convert.GetOutput()
      # this is a label image - force the parameter
      Label = True
    write(img, self.__tmpFile__.name, compress)
    # now run imview
    import os
    if Label:
      os.system( label_command % self.__tmpFile__.name)
    else:
      os.system( command % self.__tmpFile__.name)
    #tmpFile.close()


class pipeline:
  """A convenient class to store the reference to the filters of a pipeline
  
  With this class, a method can create a pipeline of several filters and return
  it without losing the references to the filters in this pipeline. The pipeline
  object act almost like a filter (it has a GetOutput() method) and thus can
  be simply integrated in another pipeline.
  """
  def __init__( self, input=None ):
    self.clear()
    self.SetInput( input )

  def connect( self, filter ):
    """Connect a new filter to the pipeline
    
    The output of the first filter will be used as the input of this
    one and the filter passed as parameter will be added to the list
    """
    if self.GetOutput() != None:
      filter.SetInput( self.GetOutput() )
    self.append( filter )

  def append( self, filter ):
    """Add a new filter to the pipeline
    
    The new filter will not be connected. The user must connect it.
    """
    self.filter_list.append( filter )

  def clear( self ):
    """Clear the filter list
    """
    self.filter_list = []

  def GetOutput( self ):
    """Return the output of the pipeline
    
    If another output is needed, use
    pipeline[-1].GetAnotherOutput() instead of this method, or subclass
    pipeline to implement another GetOutput() method
    """
    if len(self) == 0:
      return self.GetInput()
    else :
      return self[-1].GetOutput()

  def SetInput( self, input ):
    """Set the input of the pipeline
    """
    if len(self) != 0:
      self[0].SetInput(input)
    self.input = input

  def GetInput( self ):
    """Get the input of the pipeline
    """
    return self.input
    
  def Update( self ):
    """Update the pipeline
    """
    if len(self) > 0:
      return self[-1].Update()
  
  def UpdateLargestPossibleRegion( self ):
    """Update the pipeline
    """
    if len(self) > 0:
      return self[-1].UpdateLargestPossibleRegion()
  
  def UpdateOutputInformation( self ):
    if "UpdateOutputInformation" in dir(self[-1]):
      self[-1].UpdateOutputInformation()
    else:	
      self.Update()
      
  def __getitem__( self, i ):
     return self.filter_list[i]

  def __len__( self ):
     return len(self.filter_list)

  def __call__(self, *args, **kargs):
     import itk
     itk.set_inputs( self, args, kargs )
     self.UpdateLargestPossibleRegion()
     return self

  def expose(self, name, new_name=None, position=-1):
     """Expose an attribute from a filter of the minipeline.
     
     Once called, the pipeline instance has a new Set/Get set of methods to access
     directly the corresponding method of one of the filter of the pipeline.
     Ex: p.expose( "Radius" )
         p.SetRadius( 5 )
         p.GetRadius( 5 )
     By default, the attribute usable on the pipeline instance has the same name than
     the one of the filter, but it can be changed by providing a value to new_name.
     The last filter of the pipeline is used by default, but another one may be used
     by giving its position.
     Ex: p.expose("Radius", "SmoothingNeighborhood", 2)
         p.GetSmoothingNeighborhood()
     """
     if new_name == None:
       new_name = name
     src = self[position]
     ok = False
     set_name = "Set" + name
     if set_name in dir(src):
       setattr(self, "Set" + new_name, getattr(src, set_name))
       ok = True
     get_name = "Get" + name
     if get_name in dir(src):
       setattr(self, "Get" + new_name, getattr(src, get_name))
       ok = True
     if not ok:
       raise RuntimeError("No attribute %s at position %s." % (name, position))
     

class auto_pipeline(pipeline):
   current = None
   
   def __init__(self, *args, **kargs):
     pipeline.__init__(self, *args, **kargs)
     self.Start()
   
   def Start(self):
     auto_pipeline.current = self

   def Stop(self):
     auto_pipeline.current = None
     

def down_cast(obj):
  """Down cast an itkLightObject (or a object of a subclass) to its most specialized type.
  """
  import itk, itkTemplate
  className = obj.GetNameOfClass()
  t = getattr(itk, className)
  if isinstance(t, itkTemplate.itkTemplate):
    for c in t.values():
      try:
        return c.cast(obj)
      except:
        # fail silently for now
        pass
    raise RuntimeError("Can't downcast to a specialization of %s" % className)
  else:
    return t.cast(obj)
    
# now loads the other modules we may found in the same directory
import os.path, sys
directory = os.path.dirname(__file__)
moduleNames = [name[:-len('.py')] for name in os.listdir(directory) if name.endswith('.py') and name != '__init__.py']
for name in moduleNames:
  # there should be another way - I don't like to much exec -, but the which one ??
  exec "from %s import *" % name
# some cleaning
del directory, os, sys, moduleNames, name
