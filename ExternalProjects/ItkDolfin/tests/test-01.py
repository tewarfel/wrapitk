from dolfin import *
import viper
import itk
import time

dim = 2
inType = itk.Image[itk.D, dim]

reader = itk.ImageFileReader[inType].New(FileName="/media/USB-WD-1/data/tests/image_0010.bmp")
#reader = itk.ImageFileReader[inType].New(FileName="/path/to/file.bmp")
reader.Update()

t1 = time.time()

itk2dolfin = itk.ImageToDolfinFunction[inType]
#itk2dolfin.SetInput(itk2dolfin, reader.GetOutput())
#itk2dolfin.Update()
#f = itk2dolfin.GetOutput()
f = itk2dolfin.Convert(reader.GetOutput())

#imfun = itk.DolfinImageFunction[inType]

#interpolate(f, f.function_space()) # evaluate

t2 = time.time()
print 'total time =  %f ms per pixel.'% ((t2-t1) * 1e3)
#print '%i * %i = %i pixels took %0.3f ms transfering from itk to dolfin through numpy'% (shape[0], shape[1], shape[0]*shape[1], (t2-t1)*1e3)
#print 'That is %f us per pixel.'% ((t2-t1) * 1e6 / (shape[0]*shape[1]))

plot(f) # can viper plot 2d images better than this?
#interactive()

#v = viper.Viper(f)
#v.interactive()


