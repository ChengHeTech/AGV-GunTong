#ifndef __PID_H
#define __PID_H
#include "my_include.h"
/***********************************************/
//PID结构体定义
/***********************************************/
struct Agv_Pid
{
	int SetTarget;
	int SumError;
	int Error;							//Error[n]
	int LastError;					//Error[n-1]
	int PrevError;					//Error[n-2]
	float Kp;
	float Ki;
	float Kd;
};


extern struct  Agv_Pid PID;
extern struct  Agv_Pid PID2;

//struct Agv_SD_Pid
//{
//	double SetTarget;
//	double SumError;
//	double Error;							//Error[n]
//	double LastError;					//Error[n-1]
//	double PrevError;					//Error[n-2]
//	float Kp;
//	float Ki;
//	float Kd;
//};




void PID_Init(void);
int PosPIDCalc(int NextTarget);
int PosPIDCalc2(int NextTarget);
	
int IncPIDCalc(int NextTemper);
int IncPIDCalc2(int NextTarget);

//double IncPIDCalc_SD(double NextTarget);

#endif
