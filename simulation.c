#include <stdio.h>
#include <stdlib.h>
#include <immintrin.h>
#include <math.h>
#include "constants.h"

#define max(x,y) ((x)>(y)?(x):(y))
#define min(x,y) ((x)<(y)?(x):(y))

void computeTentativeVelocity(float **u, float **v, float **f, float **g,
    char **flag, int imax, int jmax, float del_t, float delx, float dely,
    float gamma, float Re)
{
    int  i, j;
    float du2dx, duvdy, duvdx, dv2dy, laplu, laplv;

    #pragma omp parallel for
    for (i=1; i<=imax-1; i++) {
        for (j=1; j<=jmax-1; j++) {
            /* only if both adjacent cells are fluid cells */
            if ((flag[i][j] & C_F) && (flag[i+1][j] & C_F)) {
                du2dx = ((u[i][j]+u[i+1][j])*(u[i][j]+u[i+1][j])+
                    gamma*fabs(u[i][j]+u[i+1][j])*(u[i][j]-u[i+1][j])-
                    (u[i-1][j]+u[i][j])*(u[i-1][j]+u[i][j])-
                    gamma*fabs(u[i-1][j]+u[i][j])*(u[i-1][j]-u[i][j]))
                    /(4.0*delx);
                duvdy = ((v[i][j]+v[i+1][j])*(u[i][j]+u[i][j+1])+
                    gamma*fabs(v[i][j]+v[i+1][j])*(u[i][j]-u[i][j+1])-
                    (v[i][j-1]+v[i+1][j-1])*(u[i][j-1]+u[i][j])-
                    gamma*fabs(v[i][j-1]+v[i+1][j-1])*(u[i][j-1]-u[i][j]))
                    /(4.0*dely);
                laplu = (u[i+1][j]-2.0*u[i][j]+u[i-1][j])/delx/delx+
                    (u[i][j+1]-2.0*u[i][j]+u[i][j-1])/dely/dely;

                f[i][j] = u[i][j]+del_t*(laplu/Re-du2dx-duvdy);
            } else {
                f[i][j] = u[i][j];
            }

            if ((flag[i][j] & C_F) && (flag[i][j+1] & C_F)) {
                duvdx = ((u[i][j]+u[i][j+1])*(v[i][j]+v[i+1][j])+
                    gamma*fabs(u[i][j]+u[i][j+1])*(v[i][j]-v[i+1][j])-
                    (u[i-1][j]+u[i-1][j+1])*(v[i-1][j]+v[i][j])-
                    gamma*fabs(u[i-1][j]+u[i-1][j+1])*(v[i-1][j]-v[i][j]))
                    /(4.0*delx);
                dv2dy = ((v[i][j]+v[i][j+1])*(v[i][j]+v[i][j+1])+
                    gamma*fabs(v[i][j]+v[i][j+1])*(v[i][j]-v[i][j+1])-
                    (v[i][j-1]+v[i][j])*(v[i][j-1]+v[i][j])-
                    gamma*fabs(v[i][j-1]+v[i][j])*(v[i][j-1]-v[i][j]))
                    /(4.0*dely);

                laplv = (v[i+1][j]-2.0*v[i][j]+v[i-1][j])/delx/delx+
                    (v[i][j+1]-2.0*v[i][j]+v[i][j-1])/dely/dely;

                g[i][j] = v[i][j]+del_t*(laplv/Re-duvdx-dv2dy);
            } else {
                g[i][j] = v[i][j];
            }
        }
    }

    j=jmax;
    for (i=1; i<=imax-1; i++) {
      if ((flag[i][j] & C_F) && (flag[i+1][j] & C_F)) {
          du2dx = ((u[i][j]+u[i+1][j])*(u[i][j]+u[i+1][j])+
              gamma*fabs(u[i][j]+u[i+1][j])*(u[i][j]-u[i+1][j])-
              (u[i-1][j]+u[i][j])*(u[i-1][j]+u[i][j])-
              gamma*fabs(u[i-1][j]+u[i][j])*(u[i-1][j]-u[i][j]))
              /(4.0*delx);
          duvdy = ((v[i][j]+v[i+1][j])*(u[i][j]+u[i][j+1])+
              gamma*fabs(v[i][j]+v[i+1][j])*(u[i][j]-u[i][j+1])-
              (v[i][j-1]+v[i+1][j-1])*(u[i][j-1]+u[i][j])-
              gamma*fabs(v[i][j-1]+v[i+1][j-1])*(u[i][j-1]-u[i][j]))
              /(4.0*dely);
          laplu = (u[i+1][j]-2.0*u[i][j]+u[i-1][j])/delx/delx+
              (u[i][j+1]-2.0*u[i][j]+u[i][j-1])/dely/dely;

          f[i][j] = u[i][j]+del_t*(laplu/Re-du2dx-duvdy);
      } else {
          f[i][j] = u[i][j];
      }
    }

    i=imax;
    for (j=1; j<=jmax-1; j++) {
      if ((flag[i][j] & C_F) && (flag[i][j+1] & C_F)) {
          duvdx = ((u[i][j]+u[i][j+1])*(v[i][j]+v[i+1][j])+
              gamma*fabs(u[i][j]+u[i][j+1])*(v[i][j]-v[i+1][j])-
              (u[i-1][j]+u[i-1][j+1])*(v[i-1][j]+v[i][j])-
              gamma*fabs(u[i-1][j]+u[i-1][j+1])*(v[i-1][j]-v[i][j]))
              /(4.0*delx);
          dv2dy = ((v[i][j]+v[i][j+1])*(v[i][j]+v[i][j+1])+
              gamma*fabs(v[i][j]+v[i][j+1])*(v[i][j]-v[i][j+1])-
              (v[i][j-1]+v[i][j])*(v[i][j-1]+v[i][j])-
              gamma*fabs(v[i][j-1]+v[i][j])*(v[i][j-1]-v[i][j]))
              /(4.0*dely);

          laplv = (v[i+1][j]-2.0*v[i][j]+v[i-1][j])/delx/delx+
              (v[i][j+1]-2.0*v[i][j]+v[i][j-1])/dely/dely;

          g[i][j] = v[i][j]+del_t*(laplv/Re-duvdx-dv2dy);
      } else {
          g[i][j] = v[i][j];
      }
    }

    /* f & g at external boundaries */
    for (j=1; j<=jmax; j++) {
        f[0][j]    = u[0][j];
        f[imax][j] = u[imax][j];
    }
    for (i=1; i<=imax; i++) {
        g[i][0]    = v[i][0];
        g[i][jmax] = v[i][jmax];
    }
}


/* Computation of tentative velocity field (f, g) */

void computeRhs(float **f, float **g, float **rhs, char **flag, int imax,
int jmax, float del_t, float delx, float dely)
{
    int i, j;
    int unroll_jmax = (jmax/4)*4;
    #pragma omp parallel for schedule(static) shared(imax)
    for (i=1;i<=imax;i++) {
        for (j=1;j<=unroll_jmax;j+=4) {
          rhs[i][j] = (
                       (f[i][j]-f[i-1][j])/delx +
                       (g[i][j]-g[i][j-1])/dely
                      ) / del_t;
          rhs[i][j+1] = (
                       (f[i][j+1]-f[i-1][j+1])/delx +
                       (g[i][j+1]-g[i][j])/dely
                      ) / del_t;
          rhs[i][j+2] = (
                       (f[i][j+2]-f[i-1][j+2])/delx +
                       (g[i][j+2]-g[i][j+1])/dely
                      ) / del_t;
          rhs[i][j+3] = (
                       (f[i][j+3]-f[i-1][j+3])/delx +
                       (g[i][j+3]-g[i][j+2])/dely
                      ) / del_t;
        }
        for(;j<jmax;j++){
          rhs[i][j] = (
                       (f[i][j]-f[i-1][j])/delx +
                       (g[i][j]-g[i][j-1])/dely
                      ) / del_t;
        }
    }
}



/* Calculate the right hand side of the pressure equation */

/* Red/Black SOR to solve the poisson equation */
int poissonSolver(float **p, float **rhs, char **flag, int imax, int jmax,
    float delx, float dely, float eps, int itermax, float omega,
    float *res, int ifull)
{
    int i, j, iter;
    float beta_2, beta_mod;

    //int rb; /* Red-black value. */

    float rdx2 = 1.0/(delx*delx);
    float rdy2 = 1.0/(dely*dely);
    beta_2 = -omega/(2.0*(rdx2+rdy2));

    /* Red/Black SOR-iteration */

    for (iter = 0; iter < itermax; iter++) {
            #pragma omp parallel for
            for (i = 1; i <= imax; i+=2) {
              #pragma omp simd
                for (j = 1; j <= jmax; j+=2) {
                    //if ((i+j) % 2 != rb) { continue; }
                    if (flag[i][j] == (C_F | B_NSEW)) {
                        p[i][j] = (1.-omega)*p[i][j] -
                              beta_2*(
                                    (p[i+1][j]+p[i-1][j])*rdx2
                                  + (p[i][j+1]+p[i][j-1])*rdy2
                                  -  rhs[i][j]
                              );
                    } else if (flag[i][j] & C_F) {
                        /* modified star near boundary */
                        beta_mod = -omega/((eps_E+eps_W)*rdx2+(eps_N+eps_S)*rdy2);
                        p[i][j] = (1.-omega)*p[i][j] -
                            beta_mod*(
                                  (eps_E*p[i+1][j]+eps_W*p[i-1][j])*rdx2
                                + (eps_N*p[i][j+1]+eps_S*p[i][j-1])*rdy2
                                - rhs[i][j]
                            );
                    }
                } /* end of j */
            } /* end of i */

            #pragma omp parallel for
            for (i = 2; i <= imax; i+=2) {
              #pragma omp simd
                for (j = 2; j <= jmax; j+=2) {
                    //if ((i+j) % 2 != rb) { continue; }
                    if (flag[i][j] == (C_F | B_NSEW)) {
                        p[i][j] = (1.-omega)*p[i][j] -
                              beta_2*(
                                    (p[i+1][j]+p[i-1][j])*rdx2
                                  + (p[i][j+1]+p[i][j-1])*rdy2
                                  -  rhs[i][j]
                              );
                    } else if (flag[i][j] & C_F) {
                        /* modified star near boundary */
                        beta_mod = -omega/((eps_E+eps_W)*rdx2+(eps_N+eps_S)*rdy2);
                        p[i][j] = (1.-omega)*p[i][j] -
                            beta_mod*(
                                  (eps_E*p[i+1][j]+eps_W*p[i-1][j])*rdx2
                                + (eps_N*p[i][j+1]+eps_S*p[i][j-1])*rdy2
                                - rhs[i][j]
                            );
                    }
                } /* end of j */
            } /* end of i */

            #pragma omp parallel for
            for (i = 1; i <= imax; i+=2) {
              #pragma omp simd
                for (j = 2; j <= jmax; j+=2) {
                    if (flag[i][j] == (C_F | B_NSEW)) {
                        p[i][j] = (1.-omega)*p[i][j] -
                              beta_2*(
                                    (p[i+1][j]+p[i-1][j])*rdx2
                                  + (p[i][j+1]+p[i][j-1])*rdy2
                                  -  rhs[i][j]
                              );
                    } else if (flag[i][j] & C_F) {
                        /* modified star near boundary */
                        beta_mod = -omega/((eps_E+eps_W)*rdx2+(eps_N+eps_S)*rdy2);
                        p[i][j] = (1.-omega)*p[i][j] -
                            beta_mod*(
                                  (eps_E*p[i+1][j]+eps_W*p[i-1][j])*rdx2
                                + (eps_N*p[i][j+1]+eps_S*p[i][j-1])*rdy2
                                - rhs[i][j]
                            );
                    }
                } /* end of j */
            } /* end of i */

            #pragma omp parallel for
            for (i = 2; i <= imax; i+=2) {
              #pragma omp simd
                for (j = 1; j <= jmax; j+=2) {
                    //if ((i+j) % 2 != rb) { continue; }
                    if (flag[i][j] == (C_F | B_NSEW)) {
                        p[i][j] = (1.-omega)*p[i][j] -
                              beta_2*(
                                    (p[i+1][j]+p[i-1][j])*rdx2
                                  + (p[i][j+1]+p[i][j-1])*rdy2
                                  -  rhs[i][j]
                              );
                    } else if (flag[i][j] & C_F) {
                        /* modified star near boundary */
                        beta_mod = -omega/((eps_E+eps_W)*rdx2+(eps_N+eps_S)*rdy2);
                        p[i][j] = (1.-omega)*p[i][j] -
                            beta_mod*(
                                  (eps_E*p[i+1][j]+eps_W*p[i-1][j])*rdx2
                                + (eps_N*p[i][j+1]+eps_S*p[i][j-1])*rdy2
                                - rhs[i][j]
                            );
                    }
                } /* end of j */
            } /* end of i */

    } /* end of iter */
    //
    return iter;
}


/* Update the velocity values based on the tentative
 * velocity values and the new pressure matrix
 */
//Talk about loop fusion and loop peeling
void updateVelocity(float **u, float **v, float **f, float **g, float **p,
    char **flag, int imax, int jmax, float del_t, float delx, float dely)
{
    int i, j;

    #pragma omp parallel for
    for (i=1; i<=imax-1; i++) {
        for (j=1; j<=jmax-1; j++) {
            /* only if both adjacent cells are fluid cells */
            if ((flag[i][j] & C_F) && (flag[i+1][j] & C_F)) {
                u[i][j] = f[i][j]-(p[i+1][j]-p[i][j])*del_t/delx;
            }

            if ((flag[i][j] & C_F) && (flag[i][j+1] & C_F)) {
                v[i][j] = g[i][j]-(p[i][j+1]-p[i][j])*del_t/dely;
            }
        }
    }

    j=jmax;
    for (i=1; i<=imax-1; i++) {
    u[i][j] = f[i][j]-(p[i+1][j]-p[i][j])*del_t/delx;
    }

    i=imax;
    for (j=1; j<=jmax-1; j++) {
    v[i][j] = g[i][j]-(p[i][j+1]-p[i][j])*del_t/dely;
    }
}


/* Set the timestep size so that we satisfy the Courant-Friedrichs-Lewy
 * conditions (ie no particle moves more than one cell width in one
 * timestep). Otherwise the simulation becomes unstable.
 */

//fusion and peeling
 void setTimestepInterval(float *del_t, int imax, int jmax, float delx,
     float dely, float **u, float **v, float Re, float tau)
 {
     int i, j;
     float umax, vmax, deltu, deltv, deltRe;

     /* del_t satisfying CFL conditions */
     if (tau >= 1.0e-10) { /* else no time stepsize control */
         umax = 1.0e-10;
         vmax = 1.0e-10;

         i=0;
         for (j=1; j<=jmax+1; j++) {
           umax = max(fabs(u[i][j]), umax);
         }

         j=0;
         for (i=1; i<=imax+1; i++) {
           vmax = max(fabs(v[i][j]), vmax);
         }

         #pragma omp simd
         for (i=1; i<=imax+1; i++) {
           //#pragma omp ordered simd
             for (j=1; j<=jmax+1; j++) {
                 umax = max(fabs(u[i][j]), umax);
                 vmax = max(fabs(v[i][j]), vmax);
             }
         }

         deltu = delx/umax;
         deltv = dely/vmax;
         deltRe = 1/(1/(delx*delx)+1/(dely*dely))*Re/2.0;

         if (deltu<deltv) {
             *del_t = min(deltu, deltRe);
         } else {
             *del_t = min(deltv, deltRe);
         }
         *del_t = tau * (*del_t); /* multiply by safety factor */
     }
 }

void applyBoundaryConditions(float **u, float **v, char **flag,
    int imax, int jmax, float ui, float vi)
{
    int i, j;

    for (j=0; j<=jmax+1; j++) {
        /* Fluid freely flows in from the west */
        u[0][j] = u[1][j];
        v[0][j] = v[1][j];

        /* Fluid freely flows out to the east */
        u[imax][j] = u[imax-1][j];
        v[imax+1][j] = v[imax][j];
    }

    for (i=0; i<=imax+1; i++) {
        /* The vertical velocity approaches 0 at the north and south
         * boundaries, but fluid flows freely in the horizontal direction */
        v[i][jmax] = 0.0;
        u[i][jmax+1] = u[i][jmax];

        v[i][0] = 0.0;
        u[i][0] = u[i][1];
    }

    /* Apply no-slip boundary conditions to cells that are adjacent to
     * internal obstacle cells. This forces the u and v velocity to
     * tend towards zero in these cells.
     */
     #pragma omp parallel for
     for (i=1; i<=imax; i++) {
        for (j=1; j<=jmax; j++) {
            if (flag[i][j] & B_NSEW) {
                switch (flag[i][j]) {
                    case B_N:
                        v[i][j]   = 0.0;
                        u[i][j]   = -u[i][j+1];
                        u[i-1][j] = -u[i-1][j+1];
                        break;
                    case B_E:
                        u[i][j]   = 0.0;
                        v[i][j]   = -v[i+1][j];
                        v[i][j-1] = -v[i+1][j-1];
                        break;
                    case B_S:
                        v[i][j-1] = 0.0;
                        u[i][j]   = -u[i][j-1];
                        u[i-1][j] = -u[i-1][j-1];
                        break;
                    case B_W:
                        u[i-1][j] = 0.0;
                        v[i][j]   = -v[i-1][j];
                        v[i][j-1] = -v[i-1][j-1];
                        break;
                    case B_NE:
                        v[i][j]   = 0.0;
                        u[i][j]   = 0.0;
                        v[i][j-1] = -v[i+1][j-1];
                        u[i-1][j] = -u[i-1][j+1];
                        break;

                    case B_SE:
                        v[i][j-1] = 0.0;
                        u[i][j]   = 0.0;
                        v[i][j]   = -v[i+1][j];
                        u[i-1][j] = -u[i-1][j-1];
                        break;
                    case B_SW:
                        v[i][j-1] = 0.0;
                        u[i-1][j] = 0.0;
                        v[i][j]   = -v[i-1][j];
                        u[i][j]   = -u[i][j-1];
                        break;
                    case B_NW:
                        v[i][j]   = 0.0;
                        u[i-1][j] = 0.0;
                        v[i][j-1] = -v[i-1][j-1];
                        u[i][j]   = -u[i][j+1];
                        break;
                }
            }
        }
    }

    /* Finally, fix the horizontal velocity at the  western edge to have
     * a continual flow of fluid into the simulation.
     */
    v[0][0] = 2*vi-v[1][0];
    for (j=1;j<=jmax;j++) {
        u[0][j] = ui;
        v[0][j] = 2*vi-v[1][j];
    }
}
