#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>

typedef struct flmData
{
  int margin;
  int n_peaks;
  int radius;
  int z_range;
  
  int xsize;
  int ysize;
  int zsize;

  double threshold;
  
  double *z_values;

  int32_t **taken;
  double **images;
} flmData;

/* Function Declarations */

void findLocalMaxima(flmData *, double *, double *, double *, double *);
int isLocalMaxima(flmData *, double, int, int, int, int, int, int, int, int);
int *dragGaussiansArea(double *,int , int ,int ,int ,int )

/*flm_data has two features, images and taken.cut slice of z stack and cut slice of y range and search x, if pixel value exceeds threshold, run isLocalMaxima to start a local check if it's a local maxima. if true, set flag ++,record the current_z, current_y,current_x,pixel value and peak number. if it's local maxima, teh taken will much be larger than 1*/
void findLocalMaxima(flmData *flm_data, double *z, double *y, double *x, double *h)
{
  int np,xi,yi,zi;
  int ex,ey,ez,sx,sy,sz;
  double cur;

  np = 0;
  for(zi=0;zi<flm_data->zsize;zi++){//it's actually not z, but different images in the same z position

    /* Set z search range. */
    sz = 0
    if(sz<0){ sz = 0;}
    ez = flm_data->z_range-1;//image number
    if(ez>=flm_data->zsize){ ez = flm_data->zsize-1; }
    
    for(yi=flm_data->margin;yi<(flm_data->ysize - flm_data->margin);yi++){

      /* Set y search range. */
      sy = yi - flm_data->radius;
      if(sy<0){ sy = 0; }
      ey = yi + flm_data->radius;
      if(ey>=flm_data->ysize){ ey = flm_data->ysize-1; }

      for(xi=flm_data->margin;xi<(flm_data->xsize - flm_data->margin);xi++){
	if(flm_data->images[zi][yi*flm_data->xsize+xi]>flm_data->threshold){
	  if(flm_data->taken[zi][yi*flm_data->xsize+xi]<1){

	    /* Set x search range. */
	    sx = xi - flm_data->radius;
	    if(sx<0){ sx = 0; }
	    ex = xi + flm_data->radius;
	    if(ex>=flm_data->xsize){ ex = flm_data->xsize-1; }

	    cur = flm_data->images[zi][yi*flm_data->xsize+xi];
	    if(isLocalMaxima(flm_data, cur, sz, ez, sy, yi, ey, sx, xi, ex)){
	      flm_data->taken[zi][yi*flm_data->xsize+xi]++;
              //get 3D location
	      z[np] = flm_data->z_values[zi];
	      y[np] = yi;
	      x[np] = xi;
	      h[np] = cur;
	      np++;
	    }

	    if (np >= flm_data->n_peaks){
	      printf("Warning! Found maximum number of peaks!\n");
	      return;
	    }
	  }
	}
      }
    }
  }

  flm_data->n_peaks = np;
}

/*
 * isLocalMaxima()
 *
 * Does a local search to check if the current pixel is a maximum. The search area
 * is a cylinder with it's axis pointed along the z axis.
 */
int isLocalMaxima(flmData *flm_data, double cur, int sz, int ez, int sy, int cy, int ey, int sx, int cx, int ex)
{
  int dx,dy,rr,xi,yi,zi;

  rr = flm_data->radius * flm_data->radius;
  
  for(zi=sz;zi<=ez;zi++){
    for(yi=sy;yi<=ey;yi++){
      dy = (yi - cy)*(yi - cy);
      for(xi=sx;xi<=ex;xi++){
	dx = (xi - cx)*(xi - cx);
	if((dx+dy)<=rr){

	  /*
	   * This is supposed to deal with two pixels that have exactly the same intensity
	   * and that are within radius of each other. In this case we'll choose the one
	   * with greater xi,yi. Note also that this order is such that we avoid the problem
	   * of the pixel not being greater than itself without explicitly testing for
	   * this condition.
	   */
	  if((yi<=cy)&&(xi<=cx)){
	    if(flm_data->images[zi][yi*flm_data->xsize+xi]>cur){
	      return 0;
	    }
	  }
	  else{
	    if(flm_data->images[zi][yi*flm_data->xsize+xi]>=cur){
	      return 0;
	    }
	  }
	}
      }
    }
  }
  return 1;
}


int *dragGaussiansArea(double *image,int xp, int yp,int value,int image_x,int image_y)
{

  int xs,ys,xe,ye;
  int width[2]={0,0};

  for(xs=xp;xs>0;xs--){
     if(image[yp*image_x+xs]>value/10.0){continue;}
     else{break;}
  }
  for(xe=xp;xe<image_x;xe++){
     if(image[yp*image_x+xe]>value/10.0){continue;}
     else{break;}
  }
  for(ys=yp;ys>0;ys--){
     if(image[ys*image_x+xp]>value/10.0){continue;}
     else{break;}
  }
  for(ye=yp;ye<image_y;ye++){
     if(image[ye*image_x+xp]>value/10.0){continue;}
     else{break;}
  }
  if(xp-xs>xe-xp){width[0]=xp-xs;}
  else{width[0]=xe-xp;}
  if(yp-ys>ye-yp){width[1]=yp-ys;}
  else{width[1]=ye-yp;}
  return width
}
















































