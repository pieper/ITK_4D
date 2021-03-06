/*=========================================================================
 *
 *  Copyright Insight Software Consortium
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkSimpleFilterWatcher.h"

#include "itkBinaryBallStructuringElement.h"
#include "itkBinaryOpeningByReconstructionImageFilter.h"


int itkBinaryOpeningByReconstructionImageFilterTest(int argc, char * argv[])
{

  if( argc != 6 )
    {
    std::cerr << "usage: " << argv[0] << " input output conn fg kernelSize" << std::endl;
    // std::cerr << "  : " << std::endl;
    exit(1);
    }

  const int dim = 2;

  typedef itk::Image< unsigned char, dim > IType;

  typedef itk::ImageFileReader< IType > ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );
  reader->Update();

  typedef itk::BinaryBallStructuringElement< bool, dim> KernelType;
  KernelType ball;
  KernelType::SizeType ballSize;
  ballSize.Fill( atoi(argv[5]) );
  ball.SetRadius(ballSize);
  ball.CreateStructuringElement();

 typedef itk::BinaryOpeningByReconstructionImageFilter< IType, KernelType > I2LType;
  I2LType::Pointer reconstruction = I2LType::New();
  reconstruction->SetInput( reader->GetOutput() );
  reconstruction->SetKernel( ball );
  reconstruction->SetFullyConnected( atoi(argv[3]) );
  reconstruction->SetForegroundValue( atoi(argv[4]) );
//   reconstruction->SetBackgroundValue( atoi(argv[6]) );
  itk::SimpleFilterWatcher watcher(reconstruction, "filter");

  typedef itk::ImageFileWriter< IType > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput( reconstruction->GetOutput() );
  writer->SetFileName( argv[2] );
  writer->Update();
  return 0;
}
