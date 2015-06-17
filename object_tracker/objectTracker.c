/*
 * File: objectTracker.c
 *
 * MATLAB Coder version            : 2.7
 * C/C++ source code generated on  : 17-Jun-2015 12:18:42
 */

/* Include Files */
#include "rt_nonfinite.h"
#include "objectTracker.h"
#include "objectTracker_emxutil.h"

/* Function Definitions */

/*
 * % find distance between lane model and object measurements to get the measurment of s (s_measure)
 * Arguments    : short init
 *                const emxArray_real_T *laneModel
 *                double lanePieceLength
 *                double r[3]
 *                double Pk[9]
 *                double Q
 *                double R
 *                const emxArray_real_T *x_measure
 *                const emxArray_real_T *y_measure
 *                double delta_x
 * Return Type  : void
 */
void objectTracker(short init, const emxArray_real_T *laneModel, double
                   lanePieceLength, double r[3], double Pk[9], double Q, double
                   R, const emxArray_real_T *x_measure, const emxArray_real_T
                   *y_measure, double delta_x)
{
  emxArray_real_T *P;
  int laneModel_idx_0;
  int i0;
  double phi;
  emxArray_real_T *b_P;
  int loop_ub;
  int b_loop_ub;
  int c_loop_ub;
  emxArray_real_T *c_P;
  emxArray_real_T *d_P;
  emxArray_real_T *e_P;
  double s_measure;
  double f_P[2];
  double M_idx_1;
  double g_P[2];
  double v[2];
  double S;
  emxArray_real_T *x;
  double a[3];
  static const double b_a[9] = { 1.0, 0.0, 0.0, 0.01, 1.0, 0.0, 5.0E-5, 0.01,
    1.0 };

  static const signed char c_a[3] = { -1, 0, 0 };

  double d_a[9];
  double e_a[9];
  static const double b[9] = { 1.0, 0.01, 5.0E-5, 0.0, 1.0, 0.01, 0.0, 0.0, 1.0
  };

  double d[9];
  static const signed char f_a[3] = { 1, 0, 0 };

  static const signed char b_b[3] = { 1, 0, 0 };

  double K[3];
  emxInit_real_T(&P, 2);

  /*  init: [scalar] set to "1" once when object is first detected to initialize state vector, then set to "0" */
  /*  laneModel: [px1] state vector of the current lane model */
  /*  lanePieceLength: [scalar] length of linear pieces in the lane model */
  /*  r: [3x1] state vector of the object being tracked */
  /*      [ s (arc length from vehicle to object in the diection of the lane model);  */
  /*        v (velocity of object);  */
  /*        a (acceleration of object)] */
  /*  Pk:[3x3] covariance of the current state  */
  /* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% old: Q: [3x3] covariance of the state transition */
  /*  Q: [scalar] standard deviation of the state transition in x-direction (how reliable is delta_x?)  */
  /*  R: [scalar] variance of the measurement (e.g. the measured distance) */
  /*  x_measure: [nx1] measured x values of the object being tracked */
  /*  y_measure: [nx1] measured y values of the object being tracked */
  /*  delta_x: [scalar] forward movement of the vehicle relative to the lane model */
  laneModel_idx_0 = laneModel->size[0];
  i0 = P->size[0] * P->size[1];
  P->size[0] = laneModel_idx_0;
  P->size[1] = 3;
  emxEnsureCapacity((emxArray__common *)P, i0, (int)sizeof(double));
  laneModel_idx_0 = laneModel->size[0] * 3;
  for (i0 = 0; i0 < laneModel_idx_0; i0++) {
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
  for (laneModel_idx_0 = 2; laneModel_idx_0 - 2 <= laneModel->size[0] - 3;
       laneModel_idx_0++) {
    /*  "-" wegen VZ-Definition der Kruemmung */
    phi = (phi + 2.0 * acos(-lanePieceLength * laneModel->data[laneModel_idx_0] /
            2.0)) - 3.1415926535897931;
    P->data[laneModel_idx_0] = P->data[laneModel_idx_0 - 1] + lanePieceLength *
      cos(phi);
    P->data[laneModel_idx_0 + P->size[0]] = P->data[(laneModel_idx_0 + P->size[0])
      - 1] + lanePieceLength * sin(phi);
    P->data[laneModel_idx_0 + (P->size[0] << 1)] = phi;
  }

  b_emxInit_real_T(&b_P, 1);
  laneModel_idx_0 = P->size[0];
  loop_ub = P->size[0];
  b_loop_ub = P->size[0];
  c_loop_ub = P->size[0];
  i0 = b_P->size[0];
  b_P->size[0] = c_loop_ub;
  emxEnsureCapacity((emxArray__common *)b_P, i0, (int)sizeof(double));
  for (i0 = 0; i0 < c_loop_ub; i0++) {
    b_P->data[i0] = P->data[i0];
  }

  b_emxInit_real_T(&c_P, 1);
  c_loop_ub = P->size[0];
  i0 = c_P->size[0];
  c_P->size[0] = c_loop_ub;
  emxEnsureCapacity((emxArray__common *)c_P, i0, (int)sizeof(double));
  for (i0 = 0; i0 < c_loop_ub; i0++) {
    c_P->data[i0] = P->data[i0 + P->size[0]];
  }

  b_emxInit_real_T(&d_P, 1);
  c_loop_ub = P->size[0];
  i0 = d_P->size[0];
  d_P->size[0] = c_loop_ub;
  emxEnsureCapacity((emxArray__common *)d_P, i0, (int)sizeof(double));
  for (i0 = 0; i0 < c_loop_ub; i0++) {
    d_P->data[i0] = P->data[i0 + (P->size[0] << 1)];
  }

  emxInit_real_T(&e_P, 2);
  i0 = e_P->size[0] * e_P->size[1];
  e_P->size[0] = laneModel_idx_0;
  e_P->size[1] = 3;
  emxEnsureCapacity((emxArray__common *)e_P, i0, (int)sizeof(double));
  for (i0 = 0; i0 < laneModel_idx_0; i0++) {
    e_P->data[i0] = b_P->data[i0];
  }

  emxFree_real_T(&b_P);
  for (i0 = 0; i0 < loop_ub; i0++) {
    e_P->data[i0 + e_P->size[0]] = c_P->data[i0];
  }

  emxFree_real_T(&c_P);
  for (i0 = 0; i0 < b_loop_ub; i0++) {
    e_P->data[i0 + (e_P->size[0] << 1)] = d_P->data[i0];
  }

  emxFree_real_T(&d_P);
  laneModel_idx_0 = x_measure->size[0];
  i0 = P->size[0] * P->size[1];
  P->size[0] = laneModel_idx_0;
  P->size[1] = 3;
  emxEnsureCapacity((emxArray__common *)P, i0, (int)sizeof(double));
  laneModel_idx_0 = x_measure->size[0] * 3;
  for (i0 = 0; i0 < laneModel_idx_0; i0++) {
    P->data[i0] = 10000.0;
  }

  for (laneModel_idx_0 = 0; laneModel_idx_0 < laneModel->size[0];
       laneModel_idx_0++) {
    for (loop_ub = 0; loop_ub < x_measure->size[0]; loop_ub++) {
      s_measure = e_P->data[laneModel_idx_0] - x_measure->data[loop_ub];
      phi = e_P->data[laneModel_idx_0 + e_P->size[0]] - y_measure->data[loop_ub];
      phi = sqrt(s_measure * s_measure + phi * phi);
      if (phi < P->data[loop_ub + (P->size[0] << 1)]) {
        P->data[loop_ub] = 1.0 + (double)laneModel_idx_0;
        P->data[loop_ub + P->size[0]] = 0.0;
        P->data[loop_ub + (P->size[0] << 1)] = phi;
      }

      if (1 + laneModel_idx_0 > 1) {
        f_P[0] = e_P->data[laneModel_idx_0 - 1];
        f_P[1] = e_P->data[(laneModel_idx_0 + e_P->size[0]) - 1];
        phi = x_measure->data[loop_ub];
        M_idx_1 = y_measure->data[loop_ub];

        /* Abstand d zwischen der Gerade g (von P in Richtung Q) und dem Punkt M */
        /* S ist der Punkt auf g mit kleinstem Abstand zu M */
        g_P[0] = e_P->data[laneModel_idx_0];
        g_P[1] = e_P->data[laneModel_idx_0 + e_P->size[0]];
        for (i0 = 0; i0 < 2; i0++) {
          v[i0] = g_P[i0] - f_P[i0];
        }

        S = -(v[0] * (f_P[0] - phi) + v[1] * (f_P[1] - M_idx_1)) / (v[0] * v[0]
          + v[1] * v[1]);
        for (i0 = 0; i0 < 2; i0++) {
          f_P[i0] += S * v[i0];
        }

        s_measure = phi - f_P[0];
        phi = M_idx_1 - f_P[1];
        phi = sqrt(s_measure * s_measure + phi * phi);
        if ((phi < P->data[loop_ub + (P->size[0] << 1)]) && (S > 0.0) && (S <
             1.0)) {
          P->data[loop_ub] = (1.0 + (double)laneModel_idx_0) - 1.0;
          P->data[loop_ub + P->size[0]] = S;
          P->data[loop_ub + (P->size[0] << 1)] = phi;
        }
      }
    }
  }

  emxFree_real_T(&e_P);
  b_emxInit_real_T(&x, 1);
  laneModel_idx_0 = P->size[0];
  i0 = x->size[0];
  x->size[0] = laneModel_idx_0;
  emxEnsureCapacity((emxArray__common *)x, i0, (int)sizeof(double));
  for (i0 = 0; i0 < laneModel_idx_0; i0++) {
    x->data[i0] = P->data[i0] + P->data[i0 + P->size[0]];
  }

  emxFree_real_T(&P);
  if (x->size[0] == 0) {
    phi = 0.0;
  } else {
    phi = x->data[0];
    for (laneModel_idx_0 = 2; laneModel_idx_0 <= x->size[0]; laneModel_idx_0++)
    {
      phi += x->data[laneModel_idx_0 - 1];
    }
  }

  s_measure = (phi / (double)x->size[0] - 1.0) * lanePieceLength;

  /*  initialize the state vector on first object detection */
  emxFree_real_T(&x);
  if (init == 1) {
    r[0] = s_measure;
    r[1] = 0.0;
    r[2] = 0.0;
  } else {
    /* % state space model matrices */
    /*  covariance of the state transition */
    phi = Q * Q;

    /* % Kalman filter  */
    /*  prediction */
    for (i0 = 0; i0 < 3; i0++) {
      M_idx_1 = 0.0;
      for (laneModel_idx_0 = 0; laneModel_idx_0 < 3; laneModel_idx_0++) {
        M_idx_1 += b_a[i0 + 3 * laneModel_idx_0] * r[laneModel_idx_0];
      }

      a[i0] = M_idx_1 + (double)c_a[i0] * delta_x;
    }

    for (i0 = 0; i0 < 3; i0++) {
      r[i0] = a[i0];
      for (laneModel_idx_0 = 0; laneModel_idx_0 < 3; laneModel_idx_0++) {
        d_a[i0 + 3 * laneModel_idx_0] = 0.0;
        for (loop_ub = 0; loop_ub < 3; loop_ub++) {
          d_a[i0 + 3 * laneModel_idx_0] += b_a[i0 + 3 * loop_ub] * Pk[loop_ub +
            3 * laneModel_idx_0];
        }
      }

      for (laneModel_idx_0 = 0; laneModel_idx_0 < 3; laneModel_idx_0++) {
        e_a[i0 + 3 * laneModel_idx_0] = 0.0;
        for (loop_ub = 0; loop_ub < 3; loop_ub++) {
          e_a[i0 + 3 * laneModel_idx_0] += d_a[i0 + 3 * loop_ub] * b[loop_ub + 3
            * laneModel_idx_0];
        }
      }
    }

    d[0] = phi;
    d[3] = phi * 0.01;
    d[6] = phi * 0.0001;
    d[1] = phi * 0.01;
    d[4] = phi * 0.0001;
    d[7] = phi * 1.0000000000000002E-6;
    d[2] = phi * 0.0001;
    d[5] = phi * 1.0000000000000002E-6;
    d[8] = phi * 1.0E-8;

    /*  kalman gain */
    M_idx_1 = 0.0;
    for (i0 = 0; i0 < 3; i0++) {
      for (laneModel_idx_0 = 0; laneModel_idx_0 < 3; laneModel_idx_0++) {
        Pk[laneModel_idx_0 + 3 * i0] = e_a[laneModel_idx_0 + 3 * i0] +
          d[laneModel_idx_0 + 3 * i0];
      }

      a[i0] = 0.0;
      for (laneModel_idx_0 = 0; laneModel_idx_0 < 3; laneModel_idx_0++) {
        a[i0] += (double)f_a[laneModel_idx_0] * Pk[laneModel_idx_0 + 3 * i0];
      }

      M_idx_1 += a[i0] * (double)b_b[i0];
    }

    S = M_idx_1 + R;
    for (i0 = 0; i0 < 3; i0++) {
      M_idx_1 = 0.0;
      for (laneModel_idx_0 = 0; laneModel_idx_0 < 3; laneModel_idx_0++) {
        M_idx_1 += Pk[i0 + 3 * laneModel_idx_0] * (double)b_b[laneModel_idx_0];
      }

      K[i0] = M_idx_1 / S;
    }

    /*  update */
    M_idx_1 = 0.0;
    for (i0 = 0; i0 < 3; i0++) {
      M_idx_1 += (double)f_a[i0] * r[i0];
    }

    phi = s_measure - M_idx_1;
    for (i0 = 0; i0 < 3; i0++) {
      r[i0] += K[i0] * phi;
    }

    for (i0 = 0; i0 < 3; i0++) {
      for (laneModel_idx_0 = 0; laneModel_idx_0 < 3; laneModel_idx_0++) {
        Pk[i0 + 3 * laneModel_idx_0] -= K[i0] * S * K[laneModel_idx_0];
      }
    }
  }
}

/*
 * File trailer for objectTracker.c
 *
 * [EOF]
 */
