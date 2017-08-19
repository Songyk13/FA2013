#pragma once
#include <vector>
#include "uniclass.h"

class AChannel
{
public:
	AChannel(void);
	~AChannel(void);
//	AChannel(const AChannel&);
//	AChannel& operator=(const AChannel&);

	AChannel operator+(const AChannel& AChannel2) const;
	AChannel operator+(const double& Num) const;
	AChannel operator-(const AChannel& AChannel2) const;
	AChannel operator-(const double& Num) const;
	AChannel operator*(const AChannel& AChannel2) const;
	AChannel operator*(const double& Num) const;
	AChannel operator/(const AChannel& AChannel2) const;
	AChannel operator/(const double& Num) const;

	friend AChannel operator+(const double& Num,const AChannel& AChannel2);
	friend AChannel operator-(const double& Num,const AChannel& AChannel2);
	friend AChannel operator*(const double& Num,const AChannel& AChannel2);
//	AChannel operator/(const AChannel& AChannel2) const;

public:
	int An;         // index number
	CString name;  // 记录名称
	CString ph;                  // phase: A,B,C or none
	CString ccbm;   // 测量量名称
	CString uu;       // 单位
	double multiplier;        //乘子    
	double offset;            //偏置
	double skew;              //延时
	int min;               //最小值
	int max;               //最大值
	double primary;        //初级比率
	double secondary;      //次级比率
	CString PS;               //所得值为初级还是次级
	std::vector<double> AData;
	//多线程
	std::vector<double> AmpData;
	std::vector<double> PhData;
	std::vector<double> Harmonics;  //暂存当前点谐波
	double Ph_base;                 //基波FFT后的相位值(弧度)
	phasor APhasor;              	// 暂存当前点相量
	bool isReal;

	drawParaChan m_drawPara;
	double maxAData;
	int ThreadFlag;//判断该通道是否计算完成


public:	
	AChannel(int _An,CString _name, CString _ph, CString _ccbm, CString _uu, double _multiplier, double _offset, double _skew, int _min,int _max,
		double _primary, double _secondary, CString _PS, bool Real);
	int InstFFT(int SamplePoint,std::vector<SampleRate>& m_SampleRate);

	int MyFFT(int start, int end, double SampRate);

	int GetCurrentPhasor(int SamplePoint,std::vector<SampleRate>& m_SampleRate);
	int drawChannel(CDC* p, CPoint pPos,int startSample, int endSample, drawParaCom drawParam);
};

class DChannel
{
public:
	DChannel(void);
	~DChannel(void);
public:
	int Dn;         // index number
	CString name;  // 记录名称
	CString ph;                  // phase: A,B,C or none
	CString ccbm;   // 测量量名称
    int y;                    // 正常状态 0 or 1 
	std::vector<BOOL> DData;
public:
	DChannel(int _Dn,CString _name, CString _ph, CString _ccbm, int _y);
	int drawChannel(CDC* p, CPoint pPos,int startSample, int endSample, drawParaCom drawParam);

};