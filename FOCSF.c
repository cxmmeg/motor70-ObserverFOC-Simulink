#define S_FUNCTION_NAME  FOCSF
#define S_FUNCTION_LEVEL 2

#include "simstruc.h"
#include "FOC.h"

PI_str D_PI  = {0};
PI_str Q_PI  = {0};
PI_str Spd_PI  = {0};
ControlCommand_str CtrlCom = {0};
MotorParameter_str MotorParameter = {0};
MotorObserver_str MotorObserver = {0};
MotorRealTimeInformation_str MRT_Inf = {0};

/* 模块初始化函数 */
static void mdlInitializeSizes(SimStruct *S)
{
    memset(&D_PI, 0, sizeof(D_PI));
    memset(&Q_PI, 0, sizeof(Q_PI));
    memset(&Spd_PI, 0, sizeof(Spd_PI));
    memset(&CtrlCom, 0, sizeof(CtrlCom));
    memset(&MotorParameter, 0, sizeof(MotorParameter));
    memset(&MotorObserver, 0, sizeof(MotorObserver));
    memset(&MRT_Inf, 0, sizeof(MRT_Inf));
    /* 设置参数数量 */
    ssSetNumSFcnParams(S, 17);

    ssSetSFcnParamTunable(S,  0, 1);
    ssSetSFcnParamTunable(S,  1, 1);
    ssSetSFcnParamTunable(S,  2, 1);
    ssSetSFcnParamTunable(S,  3, 1);
    ssSetSFcnParamTunable(S,  4, 1);
    ssSetSFcnParamTunable(S,  5, 1);
    ssSetSFcnParamTunable(S,  6, 1);
    ssSetSFcnParamTunable(S,  7, 1);
    ssSetSFcnParamTunable(S,  8, 1);
    ssSetSFcnParamTunable(S,  9, 1);
    ssSetSFcnParamTunable(S, 10, 1);
    ssSetSFcnParamTunable(S, 11, 1);
    ssSetSFcnParamTunable(S, 12, 1);
    ssSetSFcnParamTunable(S, 13, 1);
    ssSetSFcnParamTunable(S, 14, 1);
    ssSetSFcnParamTunable(S, 15, 1);
    ssSetSFcnParamTunable(S, 16, 1);

    /* 设置输入端口数量 */
    if (!ssSetNumInputPorts(S, 6)) return;

    /* 配置输入端口 */
    ssSetInputPortDataType(S, 0, SS_DOUBLE);   
    ssSetInputPortWidth(S, 0, 1);
    ssSetInputPortDirectFeedThrough(S, 0, TRUE);
    ssSetInputPortRequiredContiguous(S, 0, TRUE);

    ssSetInputPortDataType(S, 1, SS_DOUBLE);   
    ssSetInputPortWidth(S, 1, 2);
    ssSetInputPortDirectFeedThrough(S, 1, TRUE);
    ssSetInputPortRequiredContiguous(S, 1, TRUE);	

    ssSetInputPortDataType(S, 2, SS_DOUBLE);   
    ssSetInputPortWidth(S, 2, 3);
    ssSetInputPortDirectFeedThrough(S, 2, TRUE);
    ssSetInputPortRequiredContiguous(S, 2, TRUE);	

    ssSetInputPortDataType(S, 3, SS_DOUBLE);   
    ssSetInputPortWidth(S, 3, 2);
    ssSetInputPortDirectFeedThrough(S, 3, TRUE);
    ssSetInputPortRequiredContiguous(S, 3, TRUE);	

    ssSetInputPortDataType(S, 4, SS_DOUBLE);   
    ssSetInputPortWidth(S, 4, 1);
    ssSetInputPortDirectFeedThrough(S, 4, TRUE);
    ssSetInputPortRequiredContiguous(S, 4, TRUE);

    ssSetInputPortDataType(S, 5, SS_DOUBLE);   
    ssSetInputPortWidth(S, 5, 1);
    ssSetInputPortDirectFeedThrough(S, 5, TRUE);
    ssSetInputPortRequiredContiguous(S, 5, TRUE);	

    /* 设置输出端口数量 */
    if (!ssSetNumOutputPorts(S, 10)) return;

    /* 配置输出端口 */
    ssSetOutputPortDataType(S, 0, SS_DOUBLE);
    ssSetOutputPortWidth(S, 0, 2);
    
    ssSetOutputPortDataType(S, 1, SS_DOUBLE);
    ssSetOutputPortWidth(S, 1, 2);

    ssSetOutputPortDataType(S, 2, SS_DOUBLE);
    ssSetOutputPortWidth(S, 2, 3);
    
    ssSetOutputPortDataType(S, 3, SS_DOUBLE);
    ssSetOutputPortWidth(S, 3, 1);

    ssSetOutputPortDataType(S, 4, SS_DOUBLE);
    ssSetOutputPortWidth(S, 4, 3);

    ssSetOutputPortDataType(S, 5, SS_DOUBLE);
    ssSetOutputPortWidth(S, 5, 2);

    ssSetOutputPortDataType(S, 6, SS_DOUBLE);
    ssSetOutputPortWidth(S, 6, 2);

    ssSetOutputPortDataType(S, 7, SS_DOUBLE);
    ssSetOutputPortWidth(S, 7, 11);

    ssSetOutputPortDataType(S, 8, SS_DOUBLE);
    ssSetOutputPortWidth(S, 8, 1);

    ssSetOutputPortDataType(S, 9, SS_DOUBLE);
    ssSetOutputPortWidth(S, 9, 4);
}

/* 模块采样时间初始化函敿 */
static void mdlInitializeSampleTimes(SimStruct *S)
{
	/* 设置采样时间为从连接的端口继承采样时间 */
    ssSetSampleTime(S, 0, INHERITED_SAMPLE_TIME);
	/* 设置采样偏移时间 */
    ssSetOffsetTime(S, 0, FIXED_IN_MINOR_STEP_OFFSET);
}

/* 模块输出函数 */
static void mdlOutputs(SimStruct *S, int_T tid){
    /* 获取参数/输入/输出的数据 */
    real_T* iTheta = (real_T*) ssGetInputPortSignal(S, 0);
    real_T* iIdq   = (real_T*) ssGetInputPortSignal(S, 1);
    real_T* iIabc  = (real_T*) ssGetInputPortSignal(S, 2);
    real_T* iUdq   = (real_T*) ssGetInputPortSignal(S, 3);
    real_T* iMode  = (real_T*) ssGetInputPortSignal(S, 4);
    real_T* iSpd   = (real_T*) ssGetInputPortSignal(S, 5);
    
    real_T* Np     = (real_T*) ssGetRunTimeParamInfo(S,  0)->data;
    real_T* CurKp  = (real_T*) ssGetRunTimeParamInfo(S,  1)->data;
    real_T* CurKi  = (real_T*) ssGetRunTimeParamInfo(S,  2)->data;
    real_T* VolMax = (real_T*) ssGetRunTimeParamInfo(S,  3)->data;
    real_T* SpdKp  = (real_T*) ssGetRunTimeParamInfo(S,  4)->data;
    real_T* SpdKi  = (real_T*) ssGetRunTimeParamInfo(S,  5)->data;
    real_T* CurMax = (real_T*) ssGetRunTimeParamInfo(S,  6)->data;
    real_T* Udc    = (real_T*) ssGetRunTimeParamInfo(S,  7)->data;
    real_T* CurTs  = (real_T*) ssGetRunTimeParamInfo(S,  8)->data;
    real_T* SpdTs  = (real_T*) ssGetRunTimeParamInfo(S,  9)->data;
    real_T* Ls     = (real_T*) ssGetRunTimeParamInfo(S, 10)->data;
    real_T* Rs     = (real_T*) ssGetRunTimeParamInfo(S, 11)->data;
    real_T* Kt     = (real_T*) ssGetRunTimeParamInfo(S, 12)->data;
    real_T* J      = (real_T*) ssGetRunTimeParamInfo(S, 13)->data;
    real_T* ObsSpdKp  = (real_T*) ssGetRunTimeParamInfo(S,  14)->data;
    real_T* ObsSpdKi  = (real_T*) ssGetRunTimeParamInfo(S,  15)->data;
    real_T* ObsTMax = (real_T*) ssGetRunTimeParamInfo(S,  16)->data;

    real_T* oSinCosTheta = (real_T*) ssGetOutputPortSignal(S, 0);
    real_T* oUxy = (real_T*) ssGetOutputPortSignal(S, 1);
    real_T* oU123 = (real_T*) ssGetOutputPortSignal(S, 2);
    real_T* oSector = (real_T*) ssGetOutputPortSignal(S, 3);
    real_T* oCCRabc = (real_T*) ssGetOutputPortSignal(S, 4);
    real_T* oIxy = (real_T*) ssGetOutputPortSignal(S, 5);
    real_T* oIdq = (real_T*) ssGetOutputPortSignal(S, 6);
    real_T* oUdq = (real_T*) ssGetOutputPortSignal(S, 7);
    real_T* oSpd = (real_T*) ssGetOutputPortSignal(S, 8);
    real_T* oObs = (real_T*) ssGetOutputPortSignal(S, 9);

    D_PI.Kp    = *CurKp;
    D_PI.Ki    = *CurKi;
    D_PI.Max   = *VolMax;
    
    Q_PI.Kp    = *CurKp;
    Q_PI.Ki    = *CurKi;
    Q_PI.Max   = *VolMax;
    
    Spd_PI.Kp  = *SpdKp;
    Spd_PI.Ki  = *SpdKi;
    Spd_PI.Max = *CurMax;

    MotorObserver.Spd_PI.Kp = *ObsSpdKp;
    MotorObserver.Spd_PI.Ki = *ObsSpdKi;
    MotorObserver.Spd_PI.Max = *ObsTMax;

    MotorParameter.Np = (uint8_t)(*Np);
    MotorParameter.Ls = *Ls;
    MotorParameter.Rs = *Rs;
    MotorParameter.Kt = *Kt;
    MotorParameter.J  = *J;
    MotorParameter.Flux = MotorParameter.Kt / 1.5 / MotorParameter.Np;

    CtrlCom.Mode  = (uint8_t)(*iMode);
    CtrlCom.CurTs = *CurTs;
    CtrlCom.SpdTs = *SpdTs;

    MRT_Inf.Udc   = *Udc;
    MRT_Inf.Theta = GetTheta((int32_t)iTheta[0]);
    MRT_Inf.Ia = GetCur((int32_t)iIabc[0]);
    MRT_Inf.Ic = GetCur((int32_t)iIabc[2]);

    // CtrlCom.Ud = iUdq[0];
    // CtrlCom.Uq = iUdq[1];
    // CtrlCom.Id = iIdq[0];
    // CtrlCom.Iq = iIdq[1];
    CtrlCom.Spd = iSpd[0];
    
    /* 调用函数接口 */
    FOC(&D_PI, &Q_PI, &Spd_PI, &CtrlCom, &MotorParameter, &MotorObserver, &MRT_Inf);

    oSinCosTheta[0] = MRT_Inf.SinTheta;
    oSinCosTheta[1] = MRT_Inf.CosTheta;

    oUxy[0] = MRT_Inf.Ux;
    oUxy[1] = MRT_Inf.Uy;

    oU123[0] = MRT_Inf.U1;
    oU123[1] = MRT_Inf.U2;
    oU123[2] = MRT_Inf.U3;

    oSector[0] = MRT_Inf.Sector;

    oCCRabc[0] = MRT_Inf.CCRa;
    oCCRabc[1] = MRT_Inf.CCRb;
    oCCRabc[2] = MRT_Inf.CCRc;

    oIxy[0] = MRT_Inf.Ix;
    oIxy[1] = MRT_Inf.Iy;

    oIdq[0] = MRT_Inf.Id;
    oIdq[1] = MRT_Inf.Iq;

    oUdq[0]  = MRT_Inf.Ud;
    oUdq[1]  = MRT_Inf.Uq;
    oUdq[2]  = MRT_Inf.Ud_qCoupling;
    oUdq[3]  = MRT_Inf.Uq_dCoupling;
    oUdq[4]  = MRT_Inf.Ud_Electrical;
    oUdq[5]  = MRT_Inf.Uq_Electrical;
    oUdq[6]  = MRT_Inf.EMF;
    oUdq[7]  = MRT_Inf.Ud_ElectricalMaxUp;
    oUdq[8]  = MRT_Inf.Ud_ElectricalMaxDown;
    oUdq[9]  = MRT_Inf.Uq_ElectricalMaxUp;
    oUdq[10] = MRT_Inf.Uq_ElectricalMaxDown;

    oSpd[0] = MRT_Inf.Spd;

    oObs[0] = MotorObserver.Spd_Bef;
    oObs[1] = MotorObserver.Spd;
    oObs[2] = MotorObserver.TL;
    oObs[3] = MotorObserver.Spd_PI.Error;
}

/* 用于存储全局变量和运行时参数，在确定端口的宽度和采样时间后调用 */
#define MDL_SET_WORK_WIDTHS
#if defined(MDL_SET_WORK_WIDTHS) && defined(MATLAB_MEX_FILE)
static void mdlSetWorkWidths(SimStruct *S)
{
     /* 设置运行时参数的数量 */
    if (!ssSetNumRunTimeParams(S, 17)) return;

    /* 注册参数 */
    ssRegDlgParamAsRunTimeParam(S,  0,  0,  "Np",     ssGetDataTypeId(S, "double"));
    ssRegDlgParamAsRunTimeParam(S,  1,  1,  "CurKp",  ssGetDataTypeId(S, "double"));
    ssRegDlgParamAsRunTimeParam(S,  2,  2,  "CurKi",  ssGetDataTypeId(S, "double"));
    ssRegDlgParamAsRunTimeParam(S,  3,  3,  "VolMax", ssGetDataTypeId(S, "double"));
    ssRegDlgParamAsRunTimeParam(S,  4,  4,  "SpdKp",  ssGetDataTypeId(S, "double"));
    ssRegDlgParamAsRunTimeParam(S,  5,  5,  "SpdKi",  ssGetDataTypeId(S, "double"));
    ssRegDlgParamAsRunTimeParam(S,  6,  6,  "CurMax", ssGetDataTypeId(S, "double"));
    ssRegDlgParamAsRunTimeParam(S,  7,  7,  "Udc",    ssGetDataTypeId(S, "double"));
    ssRegDlgParamAsRunTimeParam(S,  8,  8,  "CurTs",  ssGetDataTypeId(S, "double"));
    ssRegDlgParamAsRunTimeParam(S,  9,  9,  "SpdTs",  ssGetDataTypeId(S, "double"));
    ssRegDlgParamAsRunTimeParam(S, 10, 10,  "Ls",     ssGetDataTypeId(S, "double"));
    ssRegDlgParamAsRunTimeParam(S, 11, 11,  "Rs",     ssGetDataTypeId(S, "double"));
    ssRegDlgParamAsRunTimeParam(S, 12, 12,  "Kt",     ssGetDataTypeId(S, "double"));
    ssRegDlgParamAsRunTimeParam(S, 13, 13,  "J",      ssGetDataTypeId(S, "double"));
    ssRegDlgParamAsRunTimeParam(S, 14, 14,  "ObsSpdKp",  ssGetDataTypeId(S, "double"));
    ssRegDlgParamAsRunTimeParam(S, 15, 15,  "ObsSpdKi",  ssGetDataTypeId(S, "double"));
    ssRegDlgParamAsRunTimeParam(S, 16, 16,  "ObsTMax", ssGetDataTypeId(S, "double"));
}
#endif

/* 模块结束函数 */
static void mdlTerminate(SimStruct *S)
{
    
}

#ifdef MATLAB_MEX_FILE
/* 被Mex-file编译 */
#include "simulink.c"
#else
/* 不被Mex-file编译 */
#include "cg_sfun.h"
#endif
