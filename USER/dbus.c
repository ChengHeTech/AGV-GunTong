
#include "dbus.h"

u16 Dbus_Data[100];//寄存器

u16 g_modebus_sendlength2;
u16 responseTime=3000;//响应时间3秒
u16 repeatNum=3; //重发次数3次
u16 DbusLocalAddress = 4;//本机地址
u16 response=0;//响应标志

char Dbus_TX_BUF[100];

//延时函数，最大65535ms
void dbus_delay_ms(u16 time)
{
//	OS_ERR err;OSTimeDly(10,OS_OPT_TIME_HMSM_STRICT,&err);
//	u16 i=0;
//	for(i=0;i<time;i++)
//	{
//		//OSTimeDlyHMSM(0,0,0,1,OS_OPT_TIME_HMSM_STRICT,&err);
//			OSTimeDly(10,OS_OPT_TIME_HMSM_STRICT,&err);
//	}
}

void Heart()//心跳函数
{
	u16 crctemp=0;
	
	Dbus_TX_BUF[0] = DbusLocalAddress>>8;//本机地址高
	Dbus_TX_BUF[1] = DbusLocalAddress;//本机地址低
	Dbus_TX_BUF[2] = 0;//帧类型
	Dbus_TX_BUF[3] = 0;//目标地址高
	Dbus_TX_BUF[4] = 1;//目标地址低
	
	crctemp=dbus_CalcCrc(Dbus_TX_BUF,5);
	
	Dbus_TX_BUF[5] = crctemp>>8;//CRC高
	Dbus_TX_BUF[6] = crctemp;//CRC低
	
	USART_OUT(USART6,Dbus_TX_BUF,7);
}

/// <summary>
/// 写单个字到目标地址
/// </summary>
/// <param name="DstAdress">目标地址</param>
/// <param name="RegisterAdress">寄存器地址</param>
/// <param name="Data">待写入数据</param>
u8 Write_Word(u16 DstAdress,u16 RegisterAdress,u16 data)//写单个寄存器
{
	u16 crctemp=0;
	u16 j,timeout,num=0;
	OS_ERR err;
	for(j=0;j<repeatNum; j++)
	{
		Dbus_TX_BUF[0] = DbusLocalAddress>>8;//本机地址高
		Dbus_TX_BUF[1] = DbusLocalAddress;//本机地址低
		Dbus_TX_BUF[2] = 1;//帧类型
		Dbus_TX_BUF[3] = DstAdress>>8;//目标地址高
		Dbus_TX_BUF[4] = DstAdress;//目标地址低
		Dbus_TX_BUF[5] = 2;//功能码
		Dbus_TX_BUF[6] = RegisterAdress>>8;//寄存器地址高
		Dbus_TX_BUF[7] = RegisterAdress;//寄存器地址低
		Dbus_TX_BUF[8] = data>>8;//数据高
		Dbus_TX_BUF[9] = data;//数据低
	 
		crctemp=dbus_CalcCrc(Dbus_TX_BUF,10);
		
		Dbus_TX_BUF[10] = crctemp>>8;//CRC高
		Dbus_TX_BUF[11] = crctemp;//CRC低
		
		USART_OUT(USART6,Dbus_TX_BUF,12);
		
		//等待响应
		timeout=0;
		while(response==0&&(timeout<responseTime/10))
		{
			timeout++;
			OSTimeDly(10,OS_OPT_TIME_HMSM_STRICT,&err);
		}
		if(timeout<(responseTime/10))//正常响应，结束
		{
			j = repeatNum;
			response = 0;		
			return 1;
		}
		
		//已重发次数加一
		num++;
		
		if(timeout<(responseTime/10))//超时
		{
			if(num==repeatNum)//重发次数到达上限，发送失败，返回0
			{
				return 0;
			}
		}
	}
	return 1;
}


/// <summary>
/// 写多个字到目标地址
/// </summary>
/// <param name="DstAdress">目标地址</param>
/// <param name="RegisterAdress">寄存器起始地址</param>
/// <param name="Num">寄存器数量</param>
/// <param name="Data">待写入数据指针</param>
u8 Write_MultipleWord(u16 DstAdress,u16 RegisterAdress,u8 Num,u16* Data)//写单个寄存器
{
	u16 crctemp=0;
	u16 i = 0,j,timeout,num=0;
	OS_ERR err;
	for(j=0;j<repeatNum; j++)
	{
		Dbus_TX_BUF[0] = DbusLocalAddress>>8;//本机地址高
		Dbus_TX_BUF[1] = DbusLocalAddress;//本机地址低
		Dbus_TX_BUF[2] = 1;//帧类型
		Dbus_TX_BUF[3] = DstAdress>>8;//目标地址高
		Dbus_TX_BUF[4] = DstAdress;//目标地址低
		Dbus_TX_BUF[5] = 4;//功能码
		Dbus_TX_BUF[6] = RegisterAdress>>8;//寄存器地址高
		Dbus_TX_BUF[7] = RegisterAdress;//寄存器地址低
		Dbus_TX_BUF[8] = Num;//寄存器数量

		//循环写入数据到发送缓冲区
		for(i=0;i<Num;i++)
		{
			Dbus_TX_BUF[9+2*i] = Data[i]>>8;//数据高
			Dbus_TX_BUF[10+2*i] = Data[i];//数据低
		}
	 
		crctemp=dbus_CalcCrc(Dbus_TX_BUF,9+2*Num);
		
		Dbus_TX_BUF[8+2*Num+1] = crctemp>>8;//CRC高
		Dbus_TX_BUF[8+2*Num+2] = crctemp;//CRC低

		USART_OUT(USART6,Dbus_TX_BUF,9+2*Num+2);

		//等待响应
		timeout=0;
		while(response==0&&(timeout<responseTime/10))//
		{
			timeout++;
			OSTimeDly(10,OS_OPT_TIME_HMSM_STRICT,&err);
			//dbus_delay_ms(10);
		}
		if(timeout<(responseTime/10))//正常响应，结束
		{
			j = repeatNum;
			response = 0;		
			return 1;
		}
		
		//已重发次数加一
		num++;
		
		if(timeout<(responseTime/10))//超时
		{
			if(num==repeatNum)//重发次数到达上限，发送失败，返回0
			{
				return 0;
			}
		}
	}
	return 1;
}

void AnalyzeDbus(void) //分析并生成响应报文
{
	u16 crc;
	u8 c1,c2;
	
	//判断目标地址
	if(((g_ethernet_RXbuff[3]<<8)|g_ethernet_RXbuff[4])==DbusLocalAddress)
	{
		//CRC校验
		crc=dbus_CalcCrc(g_ethernet_RXbuff,(g_dma_RXData_number2-2));  
		c1=crc>>8; //CRC高字节
		c2=crc;    //CRC低字节
		if(c1==g_ethernet_RXbuff[g_dma_RXData_number2-2]&&c2==g_ethernet_RXbuff[g_dma_RXData_number2-1])//校验正确
		{		
			//解析
			if(g_ethernet_RXbuff[2]==1)//操作帧
			{
				switch(g_ethernet_RXbuff[5])//功能码
				{
					case 0x01:RecFuc1();break;         //读单个寄存器
					case 0x02:RecFuc2();break;         //写单个寄存器				      
					case 0x03:RecFuc3();break;	       //读多个寄存器
					case 0x04:RecFuc4();break;         //写多个寄存器
					default:					break;  	
				}	
			}  
			else 
			if(g_ethernet_RXbuff[2]==2)//响应帧
			{
				response = 1;
			}
			else
			if(g_ethernet_RXbuff[2]==0)//心跳帧
			{
				memset(g_ethernet_RXbuff,0,g_dma_RXData_number2);
			}
		} 
		
	}
}


/*从机响应函数*/

void send2_comm( void )
{
	
}

/*分析01功能码*/ 
void RecFuc1()
 {

 } 
	  

/*分析02写单个寄存器*/ 
void RecFuc2()
 {
	 u16 startadd;//待写入寄存器地址
	 u16 data;//待写入数据
	 
	 startadd = (g_ethernet_RXbuff[6]<<8)|g_ethernet_RXbuff[7];
	 data = (g_ethernet_RXbuff[8]<<8)|g_ethernet_RXbuff[9];
	 //更新数据
	 Dbus_Data[startadd] = data;
	 	 
	 //回复响应帧
	 responsedata((g_ethernet_RXbuff[0]<<8)|g_ethernet_RXbuff[1],g_ethernet_RXbuff[5],1);
 } 
	


/*分析03功能码报文*/ 
void RecFuc3()
 {

 }
 
/*分析04写多个寄存器*/ 
void RecFuc4()
 {
	 u16 startadd;//待写入寄存器地址
	 u16 len;//待写入数据长度
	 u16 i;
	 startadd = (g_ethernet_RXbuff[6]<<8)|g_ethernet_RXbuff[7];
	 len = g_ethernet_RXbuff[8];
	 for(i=0;i<len;i++)
	 {
		  Dbus_Data[startadd+i] = (g_ethernet_RXbuff[9+2*i]<<8)|g_ethernet_RXbuff[10+2*i];
	 }
	 
	 //回复响应帧
	 responsedata((g_ethernet_RXbuff[0]<<8)|g_ethernet_RXbuff[1],g_ethernet_RXbuff[5],1);
 }  


/// <summary>
/// 响应帧
/// </summary>
/// <param name="DstAdress">目标地址</param>
/// <param name="func">功能码</param>
/// <param name="Data">结果</param>
void responsedata(u16 DstAdress,u8 func,u8 resault)
{
	u16 crctemp=0;
	Dbus_TX_BUF[0] = DbusLocalAddress>>8;//本机地址高
	Dbus_TX_BUF[1] = DbusLocalAddress;//本机地址低
	Dbus_TX_BUF[2] = 2;//帧类型
	Dbus_TX_BUF[3] = DstAdress>>8;//目标地址高
	Dbus_TX_BUF[4] = DstAdress;//目标地址低
	Dbus_TX_BUF[5] = func;//功能码
	Dbus_TX_BUF[6] = resault;//结果
	 
	crctemp=dbus_CalcCrc(Dbus_TX_BUF,7);
	
	Dbus_TX_BUF[7] = crctemp>>8;//CRC高
	Dbus_TX_BUF[8] = crctemp;//CRC低
	
	USART_OUT(USART6,Dbus_TX_BUF,9);
}

/*错误返回*/
void errorsend2(uint8_t func,uint8_t type)
{

}

 
/*************************************************
crc16校验码计算函数,计算算法：
1、设置crc校验寄存器的初值为0xffff;
2、将报文的第一个字节与crc寄存器的低字节异或，结果存入crc寄存器
3、判断crc的第一位是否为1，如果是1,crc右移1位后和0xa001异或，如果为0，则再移1位；
4、重复步骤3，直到完成8个位；
5、重复步骤2、3、4直到完成所有字节
6、返回计算结果
***********************************************/
uint16_t dbus_CalcCrc(char *chData,unsigned short uNo)
{
	uint16_t crc=0xffff;
	uint16_t i,j;
	for(i=0;i<uNo;i++)
	{
	  crc^=chData[i];
	  for(j=0;j<8;j++)
	  {
		if(crc&1)
		{
		 crc>>=1;
		 crc^=0xA001;
		}
		else
		 crc>>=1;
	  }
	}
	return (crc);
}


void clear_rxBuf2(void)
{
	u16 i=0;
	for(i=0;i<g_ethernet_RXbuff_len;i++)
			g_ethernet_RXbuff[i]=	0;
}
//src:指令
//dst:返回信息
//timeout:超时时间
int check(char* dst,u16 timeout,char* src,...)
{
	u16 num=0;
	char *resault;
	
	u16 i;
	OS_ERR err;			
	va_list ap;
	va_start(ap,src);
	vsprintf((char*)Dbus_TX_BUF,src,ap);
	va_end(ap);
	i=strlen((const char*)Dbus_TX_BUF);//此次发送数据的长度

  USART_OUT(USART6,Dbus_TX_BUF,i);
	while(!resault&&num<(timeout/10.0))
	{
		resault=strstr(g_ethernet_RXbuff,dst);
		OSTimeDlyHMSM(0,0,0,10,OS_OPT_TIME_HMSM_STRICT,&err);
		num++;
	}
	clear_rxBuf2();
	if(resault)
	{
		return 1;
	}
	else
		return 0;
}

//比较字符串和目标地址
u16 ComperStr(u16 RegisterAdress,char* str)
{
	u16 len,i;
	len = strlen(str);//数据长度
	for(i=0;i<len;i++)
	{
		if(Dbus_Data[RegisterAdress+i/2]>>8 != str[i])
		{
			return 0;
		}
		if(i<(len-1))
		{
			if((Dbus_Data[RegisterAdress+i/2]&0xff) != str[i+1])
			{
				return 0;
			}
		}
		i++;
	}
	return 1;
}

//写字符串到目标地址
u16 WriteStr(u16 DstAdress,u16 RegisterAdress,char* str)
{
	u16 len,i,len2=0;
	u16 data[100];
	len = strlen(str);//数据长度
	
	for(i=0;i<len;i++)
	{
		data[len2] = str[i]<<8;
		if(i<(len-1))
		{
			data[len2] |= str[i+1];
		}
		i++;
		len2++;
	}
	return Write_MultipleWord(DstAdress,RegisterAdress,len2,data);
}

