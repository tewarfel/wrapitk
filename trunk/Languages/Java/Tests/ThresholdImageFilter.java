/**
 *  Example on the use of the ThresholdImageFilter
 *
 */

import org.itk.io.*;
import org.itk.segmentationandthreshold.*;

public class ThresholdImageFilter
{
  public static void main( String argv[] )
  {
    System.out.println("ThresholdImageFilter Example");

    itkImageFileReaderIUS2 reader = new itkImageFileReaderIUS2();
    itkImageFileWriterIUS2 writer = new itkImageFileWriterIUS2();

    itkThresholdImageFilterIUS2 filter = new itkThresholdImageFilterIUS2();

    filter.SetInput( reader.GetOutput() );
    writer.SetInput( filter.GetOutput() );

    reader.SetFileName( argv[0] );
    writer.SetFileName( argv[1] );

    filter.SetOutsideValue( Integer.parseInt( argv[2] )  );
    filter.ThresholdAbove(  Integer.parseInt( argv[3] )  );

    writer.Update();
  }

}
