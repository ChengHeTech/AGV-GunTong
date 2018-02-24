#include "pid.h"

struct Agv_Pid PID;
struct Agv_Pid PID2;

//struct Agv_SD_Pid PID_SD;



/***********************************************/
//PID变量初始化
/***********************************************/
void PID_Init(void)
{
	PID.SetTarget = 0;
	PID.SumError = 0;
	PID.Error = 0;
	PID.LastError = 0;
	PID.PrevError = 0;
	PID.Kp = 1.2;
	PID.Ki = 1;
	PID.Kd = 0;
}
/******************************************************/
//位置式PID计算
//公式:U(k) = Kp*[E(n)] + Ki*SumError + Kd*[E(n)-E(n-1)]
//参数说明:Error = E(n) 
/*******************************************************/
int PosPIDCalc(int NextTarget)
{
	int Error1;
	int Out;
		
	PID.Error = PID.SetTarget - NextTarget;			//E(n)当前偏差E(n)	
	//PID.Error = NextTarget;
	
	PID.SumError +=PID.Error;							//SumError比例
	
	Error1 = PID.Error - PID.LastError;					//[E(n)-E(n-1)]微分

	PID.LastError = PID.Error;						//更新偏差值

	Out = PID.Kp*PID.Error + PID.Ki*PID.SumError + PID.Kd*Error1;	
	return(Out);
}

int PosPIDCalc2(int NextTarget)
{
	int Error1;
	int Out;
		
	PID2.Error = PID2.SetTarget - NextTarget;			//E(n)当前偏差E(n)	
	//PID.Error = NextTarget;
	
	PID2.SumError +=PID2.Error;							//SumError比例
	
	Error1 = PID2.Error - PID2.LastError;					//[E(n)-E(n-1)]微分

	PID2.LastError = PID2.Error;						//更新偏差值

	Out = PID2.Kp*PID2.Error + PID2.Ki*PID2.SumError + PID2.Kd*Error1;	
	return(Out);
}




/***********************************************/
//增量式PID计算
//公式:U(k+1)- u(K)=Kp*(E(n)-E(n-1)) + Ki*E(n) + Kd*(E(n)-2E(n-1)+E(n-2))
//参数说明:Error = E(n) 设置与实际的偏差量
/***********************************************/
int IncPIDCalc(int NextTarget)
{
	int Uk1;
	PID.Error = PID.SetTarget - NextTarget;		//E(n)
	Uk1 = PID.Kp*(PID.Error - PID.LastError) + PID.Ki*PID.Error + PID.Kd*(PID.Error - 2*PID.LastError+PID.PrevError);
	PID.PrevError = PID.LastError;	//上上一次的		//更新误差值
	PID.LastError = PID.Error;		//上一次的误差
	return(Uk1);
}


int IncPIDCalc2(int NextTarget)
{
	int Uk2;
	PID2.Error = PID2.SetTarget - NextTarget;		//E(n)
	Uk2 = PID2.Kp*(PID2.Error - PID2.LastError) + PID2.Ki*PID2.Error + PID2.Kd*(PID2.Error - 2*PID2.LastError+PID2.PrevError);
	PID2.PrevError = PID2.LastError;			//更新误差值
	PID2.LastError = PID2.Error;	
	return(Uk2);
}




