#ifndef __MAIN_H__
#define __MAIN_H__

#include <math.h>
#include <stdint.h>

typedef struct{
    double Kp;
    double Ki;
    double Max;
    double up;
    double ui;
    double Error;
    double Out_temp;
}PI_str;

typedef struct{
    uint8_t Spd_Tick;
    double Theta_Pre;
    
    double Spd;

    double Id;
    double Iq;

    double CurTs;
    double SpdTs;
    double CurFs;
    double SpdFs;

    double Ud;
    double Uq;

    uint8_t Mode;
}ControlCommand_str;

typedef struct{
    double Ls;
    double Rs;
    double Kt;
    double J;
    double Flux;
    uint8_t Np;
}MotorParameter_str;

typedef struct{
    double Theta;
    double Spd;

    double Udc;

    double SinTheta;
    double CosTheta; 

    double Ux;       
    double Uy;

    double U1;    
    double U2;   
    double U3;       

    uint8_t Sector;

    double CCRa;
    double CCRb; 
    double CCRc;

    double Ia;
    double Ic;

    double Ix;       
    double Iy;

    double Id;
    double Iq;

    double Ud;
    double Uq;

    double EMF;

    double Ud_qCoupling;
    double Uq_dCoupling;

    double Ud_Electrical;
    double Uq_Electrical;

    double Ud_ElectricalMaxUp;
    double Ud_ElectricalMaxDown;
    double Uq_ElectricalMaxUp;
    double Uq_ElectricalMaxDown;
}MotorRealTimeInformation_str;

typedef struct{
    double Te;
    double TL;
    double Acc;
    double Spd;
    double Spd_Temp;
    double Spd_Bef;
    double Spd_Pre;
    double Theta;
    double Theta_Pre;
    PI_str Spd_PI;
}MotorObserver_str;

typedef struct{
    double Ix_Bef;
    double Iy_Bef;
    double Ex;
    double Ey;
    double Ix;
    double Iy;
    double Vx;
    double Vy;
    double h;
    double de;
    PI_str SpdE_PI;
    double SpdE;
    double ThetaE;
    double SinTheta;
    double CosTheta;
    double Flag;
}SlidingModeObserver_str;

#define PI acos(-1)
#define TRUE 1
#define FALSE 0

#endif
