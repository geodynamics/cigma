#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "disloc3d.h"
#include "cervelli.h"

/* function fault_params_to_okada_form
 *
 * Based on fault_params_to_okada_form.m MATLAB script
 * (comments copied verbatim)
 *
 * This function takes fault trace, dip and locking depth information
 * and calculates the anchor coordinates, length, width and strike
 * of the fault plane as per Okada (1985).
 *
 * It may be beneficial in the future to compute the midpoint base
 * anchor as well for using Okada (1992) Stanford binary.
 *
 * We should also allow for buried faults in the future. This can
 * be achieved by passing a locking depth and a burial depth. The
 * only output changed would be the width of the fault plane.
 *
 * We may need to be more formal about say endpoint1 < endpoint2
 * ... maybe
 *
 * Arguments:
 *   sx1:  x coord of fault trace endpoint1
 *   sy1:  y coord of fault trace endpoint1
 *   sx2:  x coord of fault trace endpoint2
 *   sy2:  y coord of fault trace endpoint2
 *   dip:  dip of fault plane [radians]
 *   D  :  fault locking depth
 *   bd :  burial depth (top "locking depth")
 *
 * Returned variables:
 *   strike:  strike of fault plane
 *   L     :  fault length
 *   W     :  fault width
 *   ofx   :  x coord of fault anchor
 *   ofy   :  y coord of fault anchor
 *   ofxe  :  x coord of other buried corner
 *   ofye  :  y coord of other buried corner
 *   tfx   :  x coord of fault anchor (top relative)
 *   tfy   :  y coord of fault anchor (top relative)
 *   tfxe  :  x coord of other buried corner (top relative)
 *   tfye  :  y coord of other buried corner (top relative)
 */
static void
fault_params_to_okada_form(double sx1, double sy1,
                           double sx2, double sy2,
                           double dip, double D, double bd,
                           double *strike,
                           double *L, double *W,
                           double *ofx, double *ofy,
                           double *ofxe, double *ofye,
                           double *tfx, double *tfy,
                           double *tfxe, double *tfye)
{
    //
    // Do some basic checks
    //
    if (D < bd)
    {
        fprintf(stderr, "Burial depth is greater than locking depth!\n");
        fprintf(stderr, "Fault width will be negative!\n");
    }
    if (D == bd)
    {
        fprintf(stderr, "Burial depth is equal to locking depth!\n");
        fprintf(stderr, "Fault width will be zero!\n");
    }

    //
    // Calculate needed parameters for Okada input
    //
    *strike = atan2(sy1 - sy2, sx1 - sx2) + M_PI;
    *L      = sqrt((sx2 - sx1)*(sx2 - sx1) + (sy2 - sy1)*(sy2 - sy1));
    *W      = (D - bd) / sin(dip);

    //
    // For older versions without a burial depth option
    //
    // *W = D/sin(dip)
    //

    //
    // Calculate fault segment anchor and other buried point
    //
    *ofx  = sx1 + D / tan(dip) + sin(*strike);
    *ofy  = sy1 - D / tan(dip) + cos(*strike);
    *ofxe = sx2 + D / tan(dip) + sin(*strike);
    *ofye = sy2 - D / tan(dip) + cos(*strike);

    //
    // Calculate fault segment anchor and other buried point (top relative)
    //
    *tfx  = sx1 + bd / tan(dip) + sin(*strike);
    *tfy  = sy1 - bd / tan(dip) + cos(*strike);
    *tfxe = sx2 + bd / tan(dip) + sin(*strike);
    *tfye = sy2 - bd / tan(dip) + cos(*strike);
}



/* function getM
 * Convert fault parameters into form needed for Cervelli calculations.
 *
 *  Inputs:     fi      fault info
 *              nmod    number of fault patches
 *              mu      Poisson's ratio
 *
 *  Outputs:    M       matrix with fault stuff
 *                      (see below for what is on the rows)
 *                      (one row per fault patch)
 */
void getM(double *fi, int nmod, double mu, double *M)
{
    int j;
    double *fault;
    double *model;

    double fx1,fy1;
    double fx2,fy2;
    double dip;
    double D;
    double Pr;
    double bd; // burial depth
    double ss;
    double ds;
    double ts;

    double strike;
    double L, W;
    double ofx, ofy;
    double ofxe, ofye;
    double tfx, tfy;
    double tfxe, tfye;

    double length;
    double width;
    double depth;
    double str;
    double east;
    double north;

    for (j = 0; j < nmod; j++)
    {
        fault = &fi[11*j];
        model = &M[10*j];

        fx1 = fault[0];
        fy1 = fault[1];
        fx2 = fault[2];
        fy2 = fault[3];
        dip = fault[4]; // in degs here
        D   = fault[5];
        Pr  = fault[6];
        bd  = fault[7];
        ss  = fault[8];
        ds  = fault[9];
        ts  = fault[10];

        fault_params_to_okada_form(
            fx1, fy1, fx2, fy2, dip*M_PI/180.0, D, bd,
            &strike, &L, &W, &ofx, &ofy, &ofxe, &ofye, &tfx, &tfy, &tfxe, &tfye);

        length = L;
        width  = W;

        if (dip < 0)
        {
            east  = (L/2) * cos(strike) + tfx;  // east component of top center
            north = (L/2) * sin(strike) + tfy;  // north    "     "   "    "
            depth = bd;
            width = abs(width);
            ss    = -1 * ss;
            ds    = -1 * ds;
            ts    = -1 * ts;
        }
        else
        {
            east  = (L/2) * cos(strike) + ofx;  // east component of bottom center
            north = (L/2) * sin(strike) + ofy;
            depth = D;
        }

        // Cervelli takes strike in degs (strike comes out of
        // fault_params_to_okada_form in rads)
        str = (180.0/M_PI) * ((M_PI/2) - strike);

        model[0] = length;
        model[1] = width;
        model[2] = depth;
        model[3] = dip;
        model[4] = str;
        model[5] = east;
        model[6] = north;
        model[7] = ss;
        model[8] = ds;
        model[9] = ts;
    }

}



/* function calc_disp_cervelli
 *
 * Calculate displacement's via Cervelli's Okada (1992) code
 *
 *  Inputs:     fault_info      matrix with fault stuff
 *              sx              station x coord
 *              sy              station y coord
 *              sz              station z coord
 *
 *  Outputs:    dispx           disp in x direction
 *              dispy           disp in y direction
 *              dispz           disp in z direction
 *              D               matrix with derivatives
 *              S               Stresses (not used)
 *
 * TODO: update this comment
 */
void calc_disp_cervelli(double mu, double nu,
                        double *models, double *fault_info, int nfaults,
                        double *stations, int nstations,
                        double *disp, double *deriv, double *stress,
                        double *flag, double *flag2)
{
    //
    // get the "model matrix" with
    // fault patch info in the form for cervelli calcs
    //
    getM(fault_info, nfaults, mu, models);

    //
    // call the disloc3d wrapper
    //
    disloc3d(models, nfaults,
             stations, nstations,
             mu, nu,
             disp, deriv, stress,
             flag, flag2);

}

