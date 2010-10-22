/**
 *  Example on the use of the MeanImageFilter
 *
 */

import org.itk.base.*;
import org.itk.io.*;
import org.itk.denoising.*;

public class MeanImageFilter
{
  public static void main( String argv[] )
  {
    System.out.println("MeanImageFilter Example");

    itkImageFileReaderIUS2 reader = new itkImageFileReaderIUS2();
    itkImageFileWriterIUS2 writer = new itkImageFileWriterIUS2();

    itkMeanImageFilterIUS2IUS2 filter = new itkMeanImageFilterIUS2IUS2();

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
