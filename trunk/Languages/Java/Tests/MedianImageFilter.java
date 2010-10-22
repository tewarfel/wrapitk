/**
 *  Example on the use of the MedianImageFilter
 *
 */

import org.itk.base.*;
import org.itk.io.*;
import org.itk.denoising.*;

public class MedianImageFilter
{
  public static void main( String argv[] )
  {
    System.out.println("MedianImageFilter Example");

    itkImageFileReaderIUS2 reader = new itkImageFileReaderIUS2();
    itkImageFileWriterIUS2 writer = new itkImageFileWriterIUS2();

    itkMedianImageFilterIUS2IUS2 filter = new itkMedianImageFilterIUS2IUS2();

    filter.SetInput( reader.GetOutput() );
    writer.SetInput( filter.GetOutput() );

    reader.SetFileName( argv[0] );
    writer.SetFileName( argv[1] );


    int radius = Integer.parseInt( argv[2] );

    itkSize2 sizeRadius = new itkSize2();

    sizeRadius.SetElement( 0, radius );
    sizeRadius.SetElement( 1, radius );

    filter.SetRadius( sizeRadius );

    writer.Update();
  }

}
