/*
 * File: objectTracker.c
 *
 * MATLAB Coder version            : 2.7
 * C/C++ source code generated on  : 23-Jun-2015 12:44:02
 */

/* Include Files */
#include "rt_nonfinite.h"
#include "objectTracker.h"
#include "objectTracker_emxutil.h"
#include "mean.h"

/* Function Definitions */

/*
 * % find distance between lane model and object measurements to get the measurment of s (s_measure)
 * Arguments    : short init
 *                const emxArray_real_T *laneModel
 *                double lanePieceLength
 *                double r[4]
 *                double Pk[16]
 *                double Q
 *                double Rx
 *                double Ry
 *                const emxArray_real_T *x_measure
 *                const emxArray_real_T *y_measure
 *                double delta_x
 * Return Type  : void
 */
void objectTracker(short init, const emxArray_real_T *laneModel, double
                   lanePieceLength, double r[4], double Pk[16], double Q, double
                   Rx, double Ry, const emxArray_real_T *x_measure, const
                   emxArray_real_T *y_measure, double delta_x)
{
  emxArray_real_T *P;
  int r1;
  int i0;
  double phi;
  int r2;
  emxArray_real_T *b_P;
  int loop_ub;
  int b_loop_ub;
  emxArray_real_T *c_P;
  emxArray_real_T *d_P;
  emxArray_real_T *e_P;
  emxArray_real_T *D;
  double a22;
  double a[2];
  double b_x_measure[2];
  double f_P[2];
  double v[2];
  double c_x_measure;
  double b_a[3];
  double b[3];
  double b_y_measure;
  emxArray_real_T *b_D;
  emxArray_real_T *c_D;
  emxArray_real_T *d_D;
  double b_phi;
  double c_a[4];
  static const double d_a[16] = { 1.0, 0.0, 0.0, 0.0, 0.01, 1.0, 0.0, 0.0, 0.0,
    0.0, 1.0, 0.0, 0.0, 0.0, 0.01, 1.0 };

  static const signed char e_a[4] = { -1, 0, 0, 0 };

  double f_a[16];
  double g_a[16];
  static const double b_b[16] = { 1.0, 0.01, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0,
    0.0, 1.0, 0.01, 0.0, 0.0, 0.0, 1.0 };

  double d[16];
  double h_a[8];
  static const signed char i_a[8] = { 1, 0, 0, 0, 0, 1, 0, 0 };

  static const signed char c_b[8] = { 1, 0, 0, 0, 0, 0, 1, 0 };

  double b_Rx[4];
  double S[4];
  double y[8];
  double K[8];
  double d_x_measure[2];
  emxInit_real_T(&P, 2);

  /*  init: [scalar] set to "1" once when object is first detected to initialize state vector, then set to "0" */
  /*  laneModel: [px1] state vector of the current lane model */
  /*  lanePieceLength: [scalar] length of linear pieces in the lane model */
  /*  r: [3x1] state vector of the object being tracked */
  /*      [ s_x (arc length from vehicle to object in the diection of the lane model);  */
  /*        v_x (velocity of object in direction of lane);  */
  /*        s_y (lateral position of object relative to middle lane); */
  /*        v_y (lateral velocity of object)] */
  /*  Pk:[3x3] covariance of the current state  */
  /*  Q: [scalar] standard deviation of the state transition (how reliable is delta_x?)  */
  /*  R: [scalar] variance of the measurement (e.g. the measured distance) */
  /*  x_measure: [nx1] measured x values of the object being tracked */
  /*  y_measure: [nx1] measured y values of the object being tracked */
  /*  delta_x: [scalar] forward movement of the vehicle relative to the lane model */
  r1 = laneModel->size[0];
  i0 = P->size[0] * P->size[1];
  P->size[0] = r1;
  P->size[1] = 3;
  emxEnsureCapacity((emxArray__common *)P, i0, (int)sizeof(double));
  r1 = laneModel->size[0] * 3;
  for (i0 = 0; i0 < r1; i0++) {
    P->data[i0] = 0.0;
  }

  /* x, y, phi */
  phi = laneModel->data[1];

  /* erster Punkt  */
  P->data[0] = 0.0;
  P->data[P->size[0]] = laneModel->data[0];
  P->data[P->size[0] << 1] = laneModel->data[1];

  /* zweiter Punkt */
  P->data[1] = P->data[0] + lanePieceLength * cos(laneModel->data[1]);
  P->data[1 + P->size[0]] = P->data[P->size[0]] + lanePieceLength * sin
    (laneModel->data[1]);
  P->data[1 + (P->size[0] << 1)] = laneModel->data[1];
  for (r2 = 2; r2 - 2 <= laneModel->size[0] - 3; r2++) {
    /*  "-" wegen VZ-Definition der Kruemmung */
    phi = (phi + 2.0 * acos(-lanePieceLength * laneModel->data[r2] / 2.0)) -
      3.1415926535897931;
    P->data[r2] = P->data[r2 - 1] + lanePieceLength * cos(phi);
    P->data[r2 + P->size[0]] = P->data[(r2 + P->size[0]) - 1] + lanePieceLength *
      sin(phi);
    P->data[r2 + (P->size[0] << 1)] = phi;
  }

  b_emxInit_real_T(&b_P, 1);
  r1 = P->size[0];
  r2 = P->size[0];
  loop_ub = P->size[0];
  b_loop_ub = P->size[0];
  i0 = b_P->size[0];
  b_P->size[0] = b_loop_ub;
  emxEnsureCapacity((emxArray__common *)b_P, i0, (int)sizeof(double));
  for (i0 = 0; i0 < b_loop_ub; i0++) {
    b_P->data[i0] = P->data[i0];
  }

  b_emxInit_real_T(&c_P, 1);
  b_loop_ub = P->size[0];
  i0 = c_P->size[0];
  c_P->size[0] = b_loop_ub;
  emxEnsureCapacity((emxArray__common *)c_P, i0, (int)sizeof(double));
  for (i0 = 0; i0 < b_loop_ub; i0++) {
    c_P->data[i0] = P->data[i0 + P->size[0]];
  }

  b_emxInit_real_T(&d_P, 1);
  b_loop_ub = P->size[0];
  i0 = d_P->size[0];
  d_P->size[0] = b_loop_ub;
  emxEnsureCapacity((emxArray__common *)d_P, i0, (int)sizeof(double));
  for (i0 = 0; i0 < b_loop_ub; i0++) {
    d_P->data[i0] = P->data[i0 + (P->size[0] << 1)];
  }

  emxFree_real_T(&P);
  emxInit_real_T(&e_P, 2);
  i0 = e_P->size[0] * e_P->size[1];
  e_P->size[0] = r1;
  e_P->size[1] = 3;
  emxEnsureCapacity((emxArray__common *)e_P, i0, (int)sizeof(double));
  for (i0 = 0; i0 < r1; i0++) {
    e_P->data[i0] = b_P->data[i0];
  }

  emxFree_real_T(&b_P);
  for (i0 = 0; i0 < r2; i0++) {
    e_P->data[i0 + e_P->size[0]] = c_P->data[i0];
  }

  emxFree_real_T(&c_P);
  for (i0 = 0; i0 < loop_ub; i0++) {
    e_P->data[i0 + (e_P->size[0] << 1)] = d_P->data[i0];
  }

  emxFree_real_T(&d_P);
  emxInit_real_T(&D, 2);
  r1 = x_measure->size[0];
  i0 = D->size[0] * D->size[1];
  D->size[0] = r1;
  D->size[1] = 4;
  emxEnsureCapacity((emxArray__common *)D, i0, (int)sizeof(double));
  r1 = x_measure->size[0] << 2;
  for (i0 = 0; i0 < r1; i0++) {
    D->data[i0] = 10000.0;
  }

  for (r2 = 0; r2 < laneModel->size[0]; r2++) {
    for (loop_ub = 0; loop_ub < x_measure->size[0]; loop_ub++) {
      phi = e_P->data[r2] - x_measure->data[loop_ub];
      a22 = e_P->data[r2 + e_P->size[0]] - y_measure->data[loop_ub];
      phi = sqrt(phi * phi + a22 * a22);
      if (phi < D->data[loop_ub + (D->size[0] << 1)]) {
        D->data[loop_ub] = 1.0 + (double)r2;
        D->data[loop_ub + D->size[0]] = 0.0;
        D->data[loop_ub + (D->size[0] << 1)] = phi;
        a[0] = (unsigned int)laneModel->size[0];
        a[1] = r2 + 2U;
        r1 = (int)a[0] - 1;
        if ((unsigned int)a[1] < (unsigned int)(int)a[0]) {
          r1 = (int)a[1] - 1;
        }

        if (laneModel->data[r1] < 0.0) {
          phi = -1.0;
        } else if (laneModel->data[r1] > 0.0) {
          phi = 1.0;
        } else if (laneModel->data[r1] == 0.0) {
          phi = 0.0;
        } else {
          phi = laneModel->data[r1];
        }

        D->data[loop_ub + D->size[0] * 3] = -phi;

        /*  positive Krümmung -> vorzeichen von d negativ (und anders rum)      */
      }

      if (1 + r2 > 1) {
        a[0] = e_P->data[r2 - 1];
        a[1] = e_P->data[(r2 + e_P->size[0]) - 1];
        b_x_measure[0] = x_measure->data[loop_ub];
        b_x_measure[1] = y_measure->data[loop_ub];

        /* Abstand d zwischen der Gerade g (von P in Richtung Q) und dem Punkt M */
        /* S ist der Punkt auf g mit kleinstem Abstand zu M */
        f_P[0] = e_P->data[r2];
        f_P[1] = e_P->data[r2 + e_P->size[0]];
        for (i0 = 0; i0 < 2; i0++) {
          v[i0] = f_P[i0] - a[i0];
        }

        c_x_measure = -(v[0] * (a[0] - b_x_measure[0]) + v[1] * (a[1] -
          b_x_measure[1])) / (v[0] * v[0] + v[1] * v[1]);
        for (i0 = 0; i0 < 2; i0++) {
          phi = a[i0] + c_x_measure * v[i0];

          /* Kreuzprodukt für Vorzeichen von d */
          b_a[i0] = phi - a[i0];
          b[i0] = b_x_measure[i0] - phi;
          v[i0] = phi;
        }

        b_y_measure = b_a[0] * b[1] - b_a[1] * b[0];
        phi = b_x_measure[0] - v[0];
        a22 = b_x_measure[1] - v[1];
        phi = sqrt(phi * phi + a22 * a22);
        if ((phi < D->data[loop_ub + (D->size[0] << 1)]) && (c_x_measure > 0.0) &&
            (c_x_measure < 1.0)) {
          D->data[loop_ub] = (1.0 + (double)r2) - 1.0;
          D->data[loop_ub + D->size[0]] = c_x_measure;
          D->data[loop_ub + (D->size[0] << 1)] = phi;
          if (b_y_measure < 0.0) {
            D->data[loop_ub + D->size[0] * 3] = -1.0;
          } else if (b_y_measure > 0.0) {
            D->data[loop_ub + D->size[0] * 3] = 1.0;
          } else if (b_y_measure == 0.0) {
            D->data[loop_ub + D->size[0] * 3] = 0.0;
          } else {
            D->data[loop_ub + D->size[0] * 3] = b_y_measure;
          }

          /*  vorzeichen von d */
        }
      }
    }
  }

  emxFree_real_T(&e_P);
  b_emxInit_real_T(&b_D, 1);
  r1 = D->size[0];
  i0 = b_D->size[0];
  b_D->size[0] = r1;
  emxEnsureCapacity((emxArray__common *)b_D, i0, (int)sizeof(double));
  for (i0 = 0; i0 < r1; i0++) {
    b_D->data[i0] = D->data[i0 + D->size[0] * 3];
  }

  phi = mean(b_D);
  emxFree_real_T(&b_D);
  b_emxInit_real_T(&c_D, 1);
  r1 = D->size[0];
  i0 = c_D->size[0];
  c_D->size[0] = r1;
  emxEnsureCapacity((emxArray__common *)c_D, i0, (int)sizeof(double));
  for (i0 = 0; i0 < r1; i0++) {
    c_D->data[i0] = D->data[i0 + (D->size[0] << 1)];
  }

  b_emxInit_real_T(&d_D, 1);
  if (phi < 0.0) {
    b_phi = -1.0;
  } else if (phi > 0.0) {
    b_phi = 1.0;
  } else if (phi == 0.0) {
    b_phi = 0.0;
  } else {
    b_phi = phi;
  }

  b_y_measure = b_phi * mean(c_D);
  r1 = D->size[0];
  i0 = d_D->size[0];
  d_D->size[0] = r1;
  emxEnsureCapacity((emxArray__common *)d_D, i0, (int)sizeof(double));
  emxFree_real_T(&c_D);
  for (i0 = 0; i0 < r1; i0++) {
    d_D->data[i0] = D->data[i0] + D->data[i0 + D->size[0]];
  }

  emxFree_real_T(&D);
  c_x_measure = (mean(d_D) - 1.0) * lanePieceLength;

  /*  initialize the state vector on first object detection */
  emxFree_real_T(&d_D);
  if (init == 1) {
    r[0] = c_x_measure;
    r[1] = 0.0;
    r[2] = b_y_measure;
    r[3] = 0.0;
  } else {
    /* % state space model matrices */
    /*  covariance of the state transition */
    phi = Q * Q;

    /* % Kalman filter  */
    /*  prediction */
    for (i0 = 0; i0 < 4; i0++) {
      a22 = 0.0;
      for (r1 = 0; r1 < 4; r1++) {
        a22 += d_a[i0 + (r1 << 2)] * r[r1];
      }

      c_a[i0] = a22 + (double)e_a[i0] * delta_x;
    }

    for (i0 = 0; i0 < 4; i0++) {
      r[i0] = c_a[i0];
      for (r1 = 0; r1 < 4; r1++) {
        f_a[i0 + (r1 << 2)] = 0.0;
        for (r2 = 0; r2 < 4; r2++) {
          f_a[i0 + (r1 << 2)] += d_a[i0 + (r2 << 2)] * Pk[r2 + (r1 << 2)];
        }
      }

      for (r1 = 0; r1 < 4; r1++) {
        g_a[i0 + (r1 << 2)] = 0.0;
        for (r2 = 0; r2 < 4; r2++) {
          g_a[i0 + (r1 << 2)] += f_a[i0 + (r2 << 2)] * b_b[r2 + (r1 << 2)];
        }
      }
    }

    d[0] = phi;
    d[4] = phi * 0.01;
    d[8] = 0.0;
    d[12] = 0.0;
    d[1] = phi * 0.01;
    d[5] = phi * 0.0001;
    d[9] = 0.0;
    d[13] = 0.0;
    d[2] = 0.0;
    d[6] = 0.0;
    d[10] = phi;
    d[14] = phi * 0.01;
    d[3] = 0.0;
    d[7] = 0.0;
    d[11] = phi * 0.01;
    d[15] = phi * 0.0001;
    for (i0 = 0; i0 < 4; i0++) {
      for (r1 = 0; r1 < 4; r1++) {
        Pk[r1 + (i0 << 2)] = g_a[r1 + (i0 << 2)] + d[r1 + (i0 << 2)];
      }
    }

    /*  kalman gain */
    for (i0 = 0; i0 < 2; i0++) {
      for (r1 = 0; r1 < 4; r1++) {
        h_a[i0 + (r1 << 1)] = 0.0;
        for (r2 = 0; r2 < 4; r2++) {
          h_a[i0 + (r1 << 1)] += (double)i_a[i0 + (r2 << 1)] * Pk[r2 + (r1 << 2)];
        }
      }

      for (r1 = 0; r1 < 2; r1++) {
        c_a[i0 + (r1 << 1)] = 0.0;
        for (r2 = 0; r2 < 4; r2++) {
          c_a[i0 + (r1 << 1)] += h_a[i0 + (r2 << 1)] * (double)c_b[r2 + (r1 << 2)];
        }
      }
    }

    b_Rx[0] = Rx * Rx;
    b_Rx[2] = 0.0;
    b_Rx[1] = 0.0;
    b_Rx[3] = Ry * Ry;
    for (i0 = 0; i0 < 2; i0++) {
      for (r1 = 0; r1 < 2; r1++) {
        S[r1 + (i0 << 1)] = c_a[r1 + (i0 << 1)] + b_Rx[r1 + (i0 << 1)];
      }
    }

    for (i0 = 0; i0 < 4; i0++) {
      for (r1 = 0; r1 < 2; r1++) {
        y[i0 + (r1 << 2)] = 0.0;
        for (r2 = 0; r2 < 4; r2++) {
          y[i0 + (r1 << 2)] += Pk[i0 + (r2 << 2)] * (double)c_b[r2 + (r1 << 2)];
        }
      }
    }

    if (fabs(S[1]) > fabs(S[0])) {
      r1 = 1;
      r2 = 0;
    } else {
      r1 = 0;
      r2 = 1;
    }

    phi = S[r2] / S[r1];
    a22 = S[2 + r2] - phi * S[2 + r1];
    for (loop_ub = 0; loop_ub < 4; loop_ub++) {
      K[loop_ub + (r1 << 2)] = y[loop_ub] / S[r1];
      K[loop_ub + (r2 << 2)] = (y[4 + loop_ub] - K[loop_ub + (r1 << 2)] * S[2 +
        r1]) / a22;
      K[loop_ub + (r1 << 2)] -= K[loop_ub + (r2 << 2)] * phi;
    }

    /*  update */
    d_x_measure[0] = c_x_measure;
    d_x_measure[1] = b_y_measure;
    for (i0 = 0; i0 < 2; i0++) {
      a[i0] = 0.0;
      for (r1 = 0; r1 < 4; r1++) {
        a[i0] += (double)i_a[i0 + (r1 << 1)] * r[r1];
      }

      b_x_measure[i0] = d_x_measure[i0] - a[i0];
    }

    for (i0 = 0; i0 < 4; i0++) {
      a22 = 0.0;
      for (r1 = 0; r1 < 2; r1++) {
        a22 += K[i0 + (r1 << 2)] * b_x_measure[r1];
      }

      r[i0] += a22;
    }

    for (i0 = 0; i0 < 4; i0++) {
      for (r1 = 0; r1 < 2; r1++) {
        h_a[i0 + (r1 << 2)] = 0.0;
        for (r2 = 0; r2 < 2; r2++) {
          h_a[i0 + (r1 << 2)] += K[i0 + (r2 << 2)] * S[r2 + (r1 << 1)];
        }
      }
    }

    for (i0 = 0; i0 < 4; i0++) {
      for (r1 = 0; r1 < 4; r1++) {
        a22 = 0.0;
        for (r2 = 0; r2 < 2; r2++) {
          a22 += h_a[i0 + (r2 << 2)] * K[r1 + (r2 << 2)];
        }

        Pk[i0 + (r1 << 2)] -= a22;
      }
    }
  }
}

/*
 * File trailer for objectTracker.c
 *
 * [EOF]
 */
