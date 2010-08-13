/**
 *  Example on the use of the CastImageFilter
 *
 */

import org.itk.io.*;
import org.itk.simplefilters.*;

public class CastImageFilter
{
  public static void main( String argv[] )
  {
    System.out.println("CastImageFilter Example");

    itkImageFileReaderIUC2 reader = new itkImageFileReaderIUC2();
    itkImageFileWriterIUS2 writer = new itkImageFileWriterIUS2();

    itkCastImageFilterIUC2IUS2 filter = new itkCastImageFilterIUC2IUS2();

    filter.SetInput( reader.GetOutput() );
    writer.SetInput( filter.GetOutput() );

    reader.SetFileName( argv[0] );
    writer.SetFileName( argv[1] );

    writer.Update();
  }

}


