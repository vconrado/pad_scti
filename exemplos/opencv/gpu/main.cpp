#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/gpu/gpu.hpp"

#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

/// Global Variables
Mat img; 
Mat templ; 
Mat result;

int match_method;

/** @function main 

  Stripped down version, without GUI functionality 

*/
int main( int argc, char** argv )
{
  /// Load image and template
  img = imread( argv[1], 1 );
  templ = imread( argv[2], 1 );
  match_method = 0; //atoi(argv[3]);
  //printf("match_method %i\n",match_method);
  int i;

  int result_cols =  img.cols - templ.cols + 1;
  int result_rows = img.rows - templ.rows + 1;

  result.create( result_cols, result_rows, CV_32F);
double gpu_initialize;
  try
  {
    size_t t0 = clock();
    //gpu::printCudaDeviceInfo(gpu::getDevice());
    gpu::resetDevice(); 
    size_t t1 = clock();
   gpu_initialize = (double(t1 - t0)/CLOCKS_PER_SEC*1000.0);
   // printf("GPU initialize: %f ms\n", gpu_initialize);
  }
  catch (const std::exception& e)
  {
    //no GPU, DLL not compiled with GPU
    printf("Exception thrown: %s\n", e.what());
    return 0;
  }


  gpu::GpuMat d_src, d_templ, d_dst;
  size_t t0 = clock();
  d_templ.upload(templ);
  size_t t1 = clock();
  double load_temp = (double(t1 - t0)/CLOCKS_PER_SEC*1000.0);
  //printf("GPU load templ: %f ms\n", load_temp);
  size_t t2 = clock();
  d_src.upload(img);
  size_t t3 = clock();
double gpu_load_img = (double(t3 - t2)/CLOCKS_PER_SEC*1000.0);

 // printf("GPU load img: %f ms\n", gpu_load_img);

  //d_templ.upload(templ);
  //printf("GPU load templ: %f ms\n", (double(clock() - t1)/CLOCKS_PER_SEC*1000.0));
  size_t t4 = clock();
  d_dst.upload(result);
  size_t t5 = clock();
  double load_result =  (double(t5 - t4)/CLOCKS_PER_SEC*1000.0);
 // printf("GPU load result: %f ms\n", load_result);
fprintf(stderr, "gpu_initialize(ms), load_template(ms), load_img(ms), load_result(ms)\n");
  fprintf(stderr, "%12.9f, %12.9f, %12.9f, %12.9f\n", gpu_initialize, load_temp, gpu_load_img, load_result);
 gpu::matchTemplate( d_src, d_templ, d_dst,0 );
printf("method,template_matching(ms), minMaxLoc(ms), X, Y\n");
double template_match_total[6];
 double minMaxLoc_total[6];
 for(i=0; i<6; i++){
         template_match_total[i] = 0.;
         minMaxLoc_total[i]=0.;
}


  for(i = 0; i<6*10; i++){
    match_method = i/10;
 //   printf("Method: %d\n", match_method);
  /// Do the Matching
  size_t t6 = clock();
  gpu::matchTemplate( d_src, d_templ, d_dst, match_method );
  size_t t7 = clock();
  double template_match = (double(t7 - t6)/CLOCKS_PER_SEC*1000.0);
   template_match_total[match_method]+= template_match;
  //printf("GPU template match: %f ms\n",template_match);

  /// Localizing the best match with minMaxLoc
  double minVal; double maxVal; Point minLoc; Point maxLoc;
  Point matchLoc;
  size_t t8 = clock();
  gpu::minMaxLoc( d_dst, &minVal, &maxVal, &minLoc, &maxLoc);
  size_t t9 = clock();
  double  minMaxLocTime = (double(t9 - t8)/CLOCKS_PER_SEC*1000.0);
  //printf("GPU minMaxLoc: %f ms\n", minMaxLocTime);
  minMaxLoc_total[match_method]+= minMaxLocTime;
  /// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
  if( match_method  == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED )
    { matchLoc = minLoc; }
  else
    { matchLoc = maxLoc; }

  /// Show me what you got
  //printf("Match location at: %d %d\n", matchLoc.x, matchLoc.y);
//   printf("%i, %12.9f, %12.9f, %i, %i\n", match_method,template_match, minMaxLocTime,  matchLoc.x, matchLoc.y);
  }
for(i=0;i<6; i++){
         printf("%d,%12.9f,%12.9f,%12.9f\n",i,template_match_total[i]/10., minMaxLoc_total[i]/10., (template_match_total[i]+minMaxLoc_total[i])/10.);
}  
return 0;
}
