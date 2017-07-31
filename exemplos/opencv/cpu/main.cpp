#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <libgen.h>
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv; 


void print_methods(){
	  printf("Method: \n 0: SQDIFF \n 1: SQDIFF NORMED \n 2: TM CCORR \n 3: TM CCORR NORMED \n 4: TM COEFF \n 5: TM COEFF NORMED\n");
}





int main( int argc, char** argv ) {

  /// Load image and template
	if (argc < 3){
		printf("Usage: ./app original.jpg template.jpg\n");
	  //print_methods();
		return 1;
	}


  Mat img = imread( argv[1], 1 );
  Mat templ = imread( argv[2], 1 );
  Mat result;
  int match_method;
/*  int match_method = atoi(argv[3]);
	if(match_method < 0 || match_method > 5){
	  print_methods();
		return 2;
	}

*/

  int result_cols =  img.cols - templ.cols + 1;
  int result_rows = img.rows - templ.rows + 1;

  double minVal; 
  double maxVal; 
  Point minLoc; 
  Point maxLoc;
  Point matchLoc;

  
  result.create( result_rows, result_cols, CV_32FC1 );
  int i;
matchTemplate( img, templ, result, 0);
//printf("method,template_matching(ms), minMaxLoc(ms), X, Y\n");
double template_match_total[6];
double minMaxLoc_total[6];
for(i=0; i<6; i++){
	template_match_total[i] = 0.;
	minMaxLoc_total[i]=0.;
}
for(i=0; i<10*6; i++){
  match_method = i/10;
  size_t t0 = clock();
  /// Do the Matching and Normalize
  matchTemplate( img, templ, result, match_method );
   //  normalize( result, result, 0, 1, NORM_MINMAX, -1, Mat() );
  size_t t1 = clock();  
  minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );
  size_t t2 = clock();
  //printf("CPU match time: %f ms\n", (double(t1 - t0)/CLOCKS_PER_SEC*1000.0));
  //printf("CPU minMaxLoc time: %f ms\n", (double(t2 - t1)/CLOCKS_PER_SEC*1000.0));


  /// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
  if( match_method  == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED )
    { matchLoc = minLoc; }
  else
    { matchLoc = maxLoc; }

  /// Show me what you got
  //printf("X,Y = %i, %i\n", matchLoc.x, matchLoc.y);
  template_match_total[match_method]+=(double(t1 - t0)/CLOCKS_PER_SEC*1000.0);
  minMaxLoc_total[match_method]+=(double(t2 - t1)/CLOCKS_PER_SEC*1000.0);
 // printf("%d, %12.9lf, %12.9lf, %i, %i \n",match_method, (double(t1 - t0)/CLOCKS_PER_SEC*1000.0),  (double(t2 - t1)/CLOCKS_PER_SEC*1000.0), matchLoc.x, matchLoc.y );
}
for(i=0;i<6; i++){
	printf("%d, %12.9f, %12.9f, %12.9f\n",i,template_match_total[i]/10., minMaxLoc_total[i]/10., (template_match_total[i]+minMaxLoc_total[i])/10.);
}
  return 0;
}

