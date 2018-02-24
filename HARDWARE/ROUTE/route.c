#include "route.h"

u16 SystemParameter[SystemParaNum];//系统参数

u16 RouteStationNum[RouteNum]; 		//每条路径包含站点数
u16 NowRouteInfor[StationNum][StaiionInfor];   //当前路径信息
u16 StationInfor[StaiionInfor]; 
u16 HmiRouteNumFlag = 0; //上一个路径号，判断路径变化用
u16 HmiRouteCopiedNumTemp=0;//被复制路径号临时变量


u16 ProcessStepNum[ProcessNum]; 		   //每个流程包含步数
u16 NowProcessInfor[StepNum][StepInfor];  //当前流程信息
//u16 NowStepInfor[StepInfor];              //单个步骤信息

u16 HmiProcessNumFlag = 0; //上一个流程号，判断流程变化用
u16 HmiProcessCopiedNumTemp=0;//被复制流程号临时变量



//---------------系统------------//
//从系统获取一个参数 num:参数位置
u16 GetOneParameterFromSystem(u16 num)
{
	u16 buf[1];
	//读取系统参数	
	W25QXX_Read_16(buf,num,1);
	return buf[0];
}

//获取所有系统参数，并存入默认数组SystemParameter
void GetAllParameterFromSystem(void)
{
	//读取系统参数	
	W25QXX_Read_16(SystemParameter,0,SystemParaNum);
}

//设置一个参数到系统 value:参数值 num:参数序号
void SetOneParameterToSystem(u16 value,u16 num)
{
	u16 buf[1];
	buf[0] = value;
	W25QXX_Write_16(buf,num,1);
}

//设置所有参数到系统 将SystemParameter数组的数据存入Flash
void SetAllParameterToSystem()
{
	W25QXX_Write_16(SystemParameter,0,SystemParaNum);
}
//---------------路径------------//

//获取路径包含站点数 num:路径号
u16 GetRouteStationNum(u16 num)
{
	//读取
	W25QXX_Read_16(&RouteStationNum[num-1],RouteStationNumAdress(num),1);	
	
	//超出范围置零
	if(RouteStationNum[num-1]>StationNum)
		RouteStationNum[num-1] = 0;
	return RouteStationNum[num-1];
}

//保存所有路径包含的站点数
void SetAllStationNum()
{
	W25QXX_Write_16(RouteStationNum,RouteStationNumAdress(1),RouteNum);
}
//获取路径信息,num:路径号
void GetRouteData(u16 num)
{
	u16 i;
	//读取站点数
	HmiStationNum = GetRouteStationNum(num);		//更新到触摸屏上
	
	//读取站点信息
	for(i=0;i<RouteStationNum[num-1];i++)
	{
		W25QXX_Read_16(&NowRouteInfor[i][0],NowStationInforAdress(num,i),StaiionInfor);
	}
}


//保存路径信息,num:路径号
void SetRouteData(u16 num)
{
	u16 i;
	//保存路径包含站点数
	RouteStationNum[HmiRouteNum-1] = HmiStationNum;
	//保存
	W25QXX_Write_16(&RouteStationNum[HmiRouteNum-1],RouteStationNumAdress(HmiRouteNum),1);	

	
	//保存路径站点包含信息
	for(i=0;i<RouteStationNum[num-1];i++)
	{
		W25QXX_Write_16(&NowRouteInfor[i][0],NowStationInforAdress(num,i),StaiionInfor);	
//		osdelay_ms(10);
	}
}


//更新路径站点数据到界面
void UpdataStationToHmi()
{
		HmiRoadData0  = NowRouteInfor[HmiStationSerialNum-1][0];
		HmiRoadData1  = NowRouteInfor[HmiStationSerialNum-1][1];
		HmiRoadData2  = NowRouteInfor[HmiStationSerialNum-1][2];
		HmiRoadData3  = NowRouteInfor[HmiStationSerialNum-1][3];
		HmiRoadData4  = NowRouteInfor[HmiStationSerialNum-1][4];
		HmiRoadData5  = NowRouteInfor[HmiStationSerialNum-1][5];
		HmiRoadData6  = NowRouteInfor[HmiStationSerialNum-1][6];
//		HmiRoadData7  = NowRouteInfor[HmiStationSerialNum-1][7];
//		HmiRoadData8  = NowRouteInfor[HmiStationSerialNum-1][8];
//		HmiRoadData9  = NowRouteInfor[HmiStationSerialNum-1][9];
//		HmiRoadData10 = NowRouteInfor[HmiStationSerialNum-1][10];
//		HmiRoadData11 = NowRouteInfor[HmiStationSerialNum-1][11];
//		HmiRoadData12 = NowRouteInfor[HmiStationSerialNum-1][12];
//		HmiRoadData13 = NowRouteInfor[HmiStationSerialNum-1][13];
//		HmiRoadData14 = NowRouteInfor[HmiStationSerialNum-1][14];
//		HmiRoadData15 = NowRouteInfor[HmiStationSerialNum-1][15];
//		HmiRoadData16 = NowRouteInfor[HmiStationSerialNum-1][16];
//		HmiRoadData17 = NowRouteInfor[HmiStationSerialNum-1][17];
//		HmiRoadData18 = NowRouteInfor[HmiStationSerialNum-1][18];
//		HmiRoadData19 = NowRouteInfor[HmiStationSerialNum-1][19];
}

//更新路径站点数据到当前
void UpdataStationToNow()
{
		NowRouteInfor[HmiStationSerialNum-1][0]  = HmiRoadData0 ;
		NowRouteInfor[HmiStationSerialNum-1][1]  = HmiRoadData1 ;
		NowRouteInfor[HmiStationSerialNum-1][2]  = HmiRoadData2 ;
		NowRouteInfor[HmiStationSerialNum-1][3]  = HmiRoadData3 ;
		NowRouteInfor[HmiStationSerialNum-1][4]  = HmiRoadData4 ;
		NowRouteInfor[HmiStationSerialNum-1][5]  = HmiRoadData5 ;
		NowRouteInfor[HmiStationSerialNum-1][6]  = HmiRoadData6 ;
//		NowRouteInfor[HmiStationSerialNum-1][7]  = HmiRoadData7 ;
//		NowRouteInfor[HmiStationSerialNum-1][8]  = HmiRoadData8 ;
//   	NowRouteInfor[HmiStationSerialNum-1][9]  = HmiRoadData9 ;
//		NowRouteInfor[HmiStationSerialNum-1][10] = HmiRoadData10;
//		NowRouteInfor[HmiStationSerialNum-1][11] = HmiRoadData11;
//		NowRouteInfor[HmiStationSerialNum-1][12] = HmiRoadData12;
//   	NowRouteInfor[HmiStationSerialNum-1][13] = HmiRoadData13;
//		NowRouteInfor[HmiStationSerialNum-1][14] = HmiRoadData14;
//		NowRouteInfor[HmiStationSerialNum-1][15] = HmiRoadData15;
//		NowRouteInfor[HmiStationSerialNum-1][16] = HmiRoadData16;
//		NowRouteInfor[HmiStationSerialNum-1][17] = HmiRoadData17;
//		NowRouteInfor[HmiStationSerialNum-1][18] = HmiRoadData18;
//		NowRouteInfor[HmiStationSerialNum-1][19] = HmiRoadData19;
}




//更新路径站点数据到界面
void ClearStationToHmi()
{
	//默认站点信息
	HmiRoadData0  = 0;	//远程红外 默认为0 0：保持1：开2：关
	HmiRoadData1  = 0;	//到站延时时间
	HmiRoadData2  = 0;	//岔路口左分叉 0：保持1：左分叉 2：右分叉	
	HmiRoadData3  = 0;	//叉臂下 0：保持 1：叉臂上 2：叉臂下	
	HmiRoadData4  = 0;	//档位默认选择慢速 0：保持 1：慢速 2：中速 3：快速   	       
	HmiRoadData5  = 666;	//地标默认
	HmiRoadData6  = 0;	//动作	0：停止	1：前进	2:后左	3：后右  4：通过
//	HmiRoadData7  = 0;	//工作区
//	HmiRoadData8  = 5;	//快旋时间
//	HmiRoadData9  = 1;	//旋转次数
//	HmiRoadData10 = 0;	//电梯的使用与否 0:不使用1:去一楼2:去三楼 //使用电梯:叉车从一楼叉货上三楼或叉车从三楼叉货上一楼,其余为不使用电梯
//	HmiRoadData11 = 0;  //位置
//	HmiRoadData12 = 0;  //左磁导航模拟地标不触发
//	HmiRoadData13 = 0;  //右磁导航模拟地标不触发
//	HmiRoadData14 = 0;  //前磁导航模拟地标不触发
//	HmiRoadData15 = 1;  //左磁导航模拟地标触发1次
//	HmiRoadData16 = 1;  //右磁导航模拟地标触发1次
//	HmiRoadData17 = 1;  //前磁导航模拟地标触发1次
//	HmiRoadData18 = 0;  //设备给车发送的启动信号
//	HmiRoadData19 = 0;  //叉臂避障

}


//---------------流程------------//

//保存及更新流程数据（当前操作的流程信息）
void	SetProcessData()
{
	u16 i = 0;
	//流程步数
	
	ProcessStepNum[HmiProcessNum-1] = HmiStepNum;
	//保存
	W25QXX_Write_16(&ProcessStepNum[HmiProcessNum-1],ProcessStepNumAdress(HmiProcessNum),1);	

	
	//流程信息
	//更新当前单步的起始路径号
	NowProcessInfor[HmiProcessSerialNum-1][0] = HmiProcessStartNum;
	//更新当前单步的动作
	NowProcessInfor[HmiProcessSerialNum-1][1] = HmiProcessAction;
	//更新当前单步的结束路径号或重复执行次数
	NowProcessInfor[HmiProcessSerialNum-1][2] = HmiProcessStopNum;
	
	//保存
	for(i=0;i<HmiStepNum;i++)
		W25QXX_Write_16(&NowProcessInfor[i][0],NowStepInforAdress(HmiProcessNum,i),StepInfor);	
}


//获取流程包含步数
u16	GetProcessStepNum(u16 num)
{
	//读取
	W25QXX_Read_16(&ProcessStepNum[num-1],ProcessStepNumAdress(num),1);	
	
	return ProcessStepNum[num-1];
}

//保存所有流程包含步数
void SetAllStepNum()
{
	W25QXX_Write_16(ProcessStepNum,ProcessStepNumAdress(1),ProcessNum);
}

//获取及更新流程数据（当前操作的流程信息）
void	GetProcessData()
{
	u16 i = 0;
	//流程步数
	
	//读取
	W25QXX_Read_16(&ProcessStepNum[HmiProcessNum-1],ProcessStepNumAdress(HmiProcessNum),1);	
	//更新
	HmiStepNum = ProcessStepNum[HmiProcessNum-1];
	
	HmiProcessSerialNum = 1;
	
	//流程步数超过设定最大值，直接置0
	if(HmiStepNum>StepNum)
	{
		//流程数设置为0
		HmiStepNum = 0;
		//序号设置为0
		HmiProcessSerialNum = 0;
	}
	
	//流程信息
	//读取
	for(i=0;i<HmiStepNum;i++)
		W25QXX_Read_16(&NowProcessInfor[i][0],NowStepInforAdress(HmiProcessNum,i),StepInfor);	
	
	//更新当前单步的起始路径号
	HmiProcessStartNum = 	NowProcessInfor[HmiProcessSerialNum-1][0];
	//更新当前单步的动作
	HmiProcessAction = 	NowProcessInfor[HmiProcessSerialNum-1][1];
	//更新当前单步的结束路径号或重复执行次数
	HmiProcessStopNum = 	NowProcessInfor[HmiProcessSerialNum-1][2];
}


//获取及更新指定流程数据（指定的流程信息）
void	GetProcessDataFrom(u16 num)
{
	u16 i = 0;
	//流程步数
	
	//读取
	W25QXX_Read_16(&ProcessStepNum[num-1],ProcessStepNumAdress(num),1);	
	//更新
	HmiStepNum = ProcessStepNum[num-1];
	
	HmiProcessSerialNum = 1;
	
	//流程步数超过设定最大值，直接置0
	if(HmiStepNum>StepNum)
	{
		//流程数设置为0
		HmiStepNum = 0;
		//序号设置为0
		HmiProcessSerialNum = 0;
	}
	
	//流程信息
	//读取
	for(i=0;i<HmiStepNum;i++)
		W25QXX_Read_16(&NowProcessInfor[i][0],NowStepInforAdress(num,i),StepInfor);	
	
	//更新当前单步的起始路径号
	HmiProcessStartNum = 	NowProcessInfor[HmiProcessSerialNum-1][0];
	//更新当前单步的动作
	HmiProcessAction = 	NowProcessInfor[HmiProcessSerialNum-1][1];
	//更新当前单步的结束路径号或重复执行次数
	HmiProcessStopNum = 	NowProcessInfor[HmiProcessSerialNum-1][2];
}



//更新流程数据到界面
void UpdataProcessToHmi()
{
	//更新当前单步的起始路径号
	HmiProcessStartNum = 	NowProcessInfor[HmiProcessSerialNum-1][0];
	//更新当前单步的动作
	HmiProcessAction = 	NowProcessInfor[HmiProcessSerialNum-1][1];
	//更新当前单步的结束路径号或重复执行次数
	HmiProcessStopNum = 	NowProcessInfor[HmiProcessSerialNum-1][2];
}

//更新流程数据到界面
void ClearStepToHmi()
{
	//更新当前单步的起始路径号
	HmiProcessStartNum = 1;
	//更新当前单步的动作
	HmiProcessAction = 0;
	//更新当前单步的结束路径号或重复执行次数
	HmiProcessStopNum = 0;
}

//插入及更新流程数据（当前操作的流程信息）
void	InsertProcessData()
{
	u16 i,j;
	//流程步数
	
	ProcessStepNum[HmiProcessNum-1] = HmiStepNum;
	//保存
	W25QXX_Write_16(&ProcessStepNum[HmiProcessNum-1],ProcessStepNumAdress(HmiProcessNum),1);	

	
	//流程信息
	//将当前及后面所数据向后移动一位
	for(i=HmiStepNum;i>=HmiProcessSerialNum;i--)
	{
		for(j=0;j<StepInfor;j++)
		{
			NowProcessInfor[i-1][j] = NowProcessInfor[i-1-1][j];
		}
	}
	
	//更新新插入数据
	
	//更新当前单步的起始路径号
	NowProcessInfor[HmiProcessSerialNum-1][0] = HmiProcessStartNum;
	//更新当前单步的动作
	NowProcessInfor[HmiProcessSerialNum-1][1] = HmiProcessAction;
	//更新当前单步的结束路径号或重复执行次数
	NowProcessInfor[HmiProcessSerialNum-1][2] = HmiProcessStopNum;

		
	//保存
	for(i=0;i<HmiStepNum;i++)
		W25QXX_Write_16(&NowProcessInfor[i][0],NowStepInforAdress(HmiProcessNum,i),StepInfor);	
}


//删除及更新流程数据（当前操作的流程信息）
void	DeleteProcessData()
{
	u16 i,j;
	//流程步数
	if(HmiStepNum>0)
	{
		//将后面所数据向前移动一位
		if((HmiStepNum>0)&&(HmiStepNum!=HmiProcessSerialNum))
		{
			for(i=HmiProcessSerialNum;i<HmiStepNum;i++)
			{
				for(j=0;j<StepInfor;j++)
				{
					NowProcessInfor[i-1][j] = NowProcessInfor[i][j];
				}
			}
		}
		
		HmiStepNum--;
		if(HmiStepNum==0)
			HmiProcessSerialNum = 0;
		else
			if((HmiStepNum+1)==HmiProcessSerialNum)
				HmiProcessSerialNum--;
		
		//保存
		for(i=0;i<HmiStepNum;i++)
			W25QXX_Write_16(&NowProcessInfor[i][0],NowStepInforAdress(HmiProcessNum,i),StepInfor);	
		
		ProcessStepNum[HmiProcessNum-1] = HmiStepNum;
		//保存
		W25QXX_Write_16(&ProcessStepNum[HmiProcessNum-1],ProcessStepNumAdress(HmiProcessNum),1);	

		//更新界面
		UpdataProcessToHmi();	
	}
}


//自动界面显示信息重载
void HmiAutoReload()
{
	//如果正在执行流程，则重载路径号
	if(HmiTask == 2)
	{
		//获取流程信息
		GetProcessData();
		HmiRouteNum = NowProcessInfor[0][0];
	}
	//重载站点数
	//获取路径包含站点数
	HmiStationNum = GetRouteStationNum(HmiRouteNum);
	//当前地标清零
	HmiRfidNum = 0;
	
	//获取当前路径信息
	GetRouteData(HmiRouteNum);	
	//重载下一地标
	HmiNextRfidNum = NowRouteInfor[0][5];
}


void UserConfigInit(void)
{
    //获取所有系统参数
    GetAllParameterFromSystem();

    if(SystemParameter[0] != 5) //首次flash中没数据时或者意外导致数据丢失先刷入初始量
    {
#if 1						//为1	//车的前100个内存中写入初始量
        SystemParameter[0] = 5;		//判断是否第一次写入的标志
        SystemParameter[1] = 0;	
        SystemParameter[2] = 0;
        SystemParameter[3] = 0;		//手动速度
        SystemParameter[4] = 0; 
        SystemParameter[5] = 0; 
        SystemParameter[6] = 0;	//
        SystemParameter[7] = 0;
		
        SystemParameter[8] = 25;  	//方向Kp
        SystemParameter[9] = 40; 	//方向Ki
        SystemParameter[10] = 5; 	//方向Kd
		
        SystemParameter[11] = 5;	//前进方向的PID控制周期--ms
        SystemParameter[12] = 1050; 	//前轮中值 -- mv //要除1000
        SystemParameter[13] = 1050; 	//后轮中值
        SystemParameter[14] = 0; 	//前轮角度
        SystemParameter[15] = 0; 	//后轮
        SystemParameter[16] = 0;
        SystemParameter[17] = 0;
        SystemParameter[18] = 200; //低速
        SystemParameter[19] = 400; //中速
        SystemParameter[20] = 600; //高速
        SystemParameter[21] = 0;
        SystemParameter[22] = 0;
        SystemParameter[23] = 0;
        SystemParameter[24] = 0; 
        SystemParameter[25] = 0; //
        SystemParameter[26] = 0; //
        SystemParameter[27] = 0; //
        SystemParameter[28] = 0; //
        SystemParameter[29] = 0; //
        SystemParameter[30] = 0;
        SystemParameter[31] = 0; //
        SystemParameter[32] = 0; //
        SystemParameter[33] = 0; //
        SystemParameter[34] = 0; //
        SystemParameter[35] = 0; //
        SystemParameter[36] = 0; //
        SystemParameter[37] = 0; //
        SystemParameter[38] = 0; //
        SystemParameter[39] = 0;
        SystemParameter[40] = 0;
        SystemParameter[41] = 0;
        SystemParameter[42] = 0;
        SystemParameter[43] = 0;
        SystemParameter[44] = 0;
        SystemParameter[45] = 0;
        SystemParameter[46] = 0;
        SystemParameter[47] = 0;
        SystemParameter[48] = 0;
        SystemParameter[49] = 0;
        SystemParameter[50] = 0;
        SystemParameter[51] = 0;
        SystemParameter[52] = 0;	//
        SystemParameter[53] = 0;
        SystemParameter[54] = 0;
        SystemParameter[55] = 0;
        SystemParameter[56] = 0;
        SystemParameter[57] = 0;
        SystemParameter[58] = 0;
        SystemParameter[59] = 1;  //

        SystemParameter[60] = 0; //
        SystemParameter[61] = 0; //
        SystemParameter[62] = 0; //
        SystemParameter[63] = 0; //
        SystemParameter[64] = 0; //
        SystemParameter[65] = 0; //
        SystemParameter[66] = 0; //
        SystemParameter[67] = 0; //
        SystemParameter[68] = 0; //
        SystemParameter[69] = 0; //
        SystemParameter[70] = 0; //
        SystemParameter[71] = 0; //
        SystemParameter[72] = 0; //
        SystemParameter[73] = 0; //
        SystemParameter[74] = 0; //
        SystemParameter[75] = 0; //
		SystemParameter[76] = 0; //
        SystemParameter[77] = 0;
        SystemParameter[78] = 0;
        SystemParameter[79] = 0;
        SystemParameter[80] = 0;
        SystemParameter[81] = 0;
        SystemParameter[82] = 0;
        SystemParameter[83] = 0;
        SystemParameter[84] = 0;
        SystemParameter[85] = 0;
        SystemParameter[86] = 0;
        SystemParameter[87] = 0;
        SystemParameter[88] = 0;
        SystemParameter[89] = 0;
        SystemParameter[98] = 4; //系统音量
        SystemParameter[99] = 200; //工位对接速度
#endif
        //存储所有系统参数
        SetAllParameterToSystem();

#if 1
        //清除路径包含站点数
        memset(RouteStationNum, 0, RouteNum);
        //保存所有路径包含的站点数
        SetAllStationNum();

        //清除流程包含步数
        memset(ProcessStepNum, 0, ProcessNum);
        //保存所有流程包含的步数
        SetAllStepNum();
#endif
    }


	
//	
//系统参数更新到触摸屏上
//	
	
	
	
//    PLC_Data[52] = SystemParameter[52]; //叉车号





//    PID.Kp = PLC_Data[8] = SystemParameter[8];		//初始化屏幕的PID参数kp
//    PID.Ki = PLC_Data[9] = SystemParameter[9];     //初始化屏幕的PID参数ki
//    PID.Kd = PLC_Data[10] = SystemParameter[10];   //初始化屏幕的PID参数kd

//    PLC_Data[11] = SystemParameter[11];			   //测试前进的PID控制周期

//	PLC_Data[12] = SystemParameter[12]; //前轮中值
//	PLC_Data[13] = SystemParameter[13]; //后轮中值
//	
//    HmiDiSu    = SystemParameter[18];		//一档速度
//    HmiZhongSu = SystemParameter[19];   //二档速度
//    HmiGaoSu   = SystemParameter[20];   //三档速度


//
//加载自动界面信息
//
    //HmiAutoReload();


    //获取路径包含站点数
    GetRouteStationNum(HmiRouteNum);

    //获取当前路径信息
    GetRouteData(HmiRouteNum);

    //获取及更新流程数据
    GetProcessData();
}







AGV_station agv_routr2station[StationNum];		//路径到站点的二维数组

























