#pragma once

#ifndef _UNICLASS_H_
#define _UNICLASS_H_


#include "stdafx.h"
#define FREQ 50
#include<complex>
struct SampleRate
{
	double samp;
	int endsamp;
};

struct phasor
{
	double Amplitude;//幅值
	double phase;//相位角度
};

struct ChannelGroup
{
	CString Name;
	int A;
	int B;
	int C;
	int N;
};

struct Seq_Comp    //序分量
{
	CString name;//电压or电流
	std::complex<double> PSC;   //正序
	std::complex<double> NSC;   //负序
	std::complex<double> ZSC;   //零序
};

struct Z
{
	double R1;
	double X1;
	std::complex<double> Z1;
	double R2;
	double X2;
	std::complex<double> Z2;
	double R3;
	double X3;
	std::complex<double> Z3;
};
struct LineData
{
	std::complex<double> z1;
	std::complex<double> z0;
};

struct BusIDiff
{
	double rIA;
	double rIB;
	double rIC;
	double resIA;
	double resIB;
	double resIC;
};

struct drawParaChan               //绘图参数，用于class Channel
{
	int VSpace;                //纵向横坐标轴间距,通道高度（计算得到）
	int defultVSpace;           //纵向横坐标轴间距,通道高度（默认）
	double UPScale;            //量纲每像素(V/A/Hz)
	double defaultUPScale; 
};

struct drawParaCom              //绘图参数，用于class Comtrade
{
	int defaultVSpace;
	int defaultDVSpace;     //d channel
	int VGap;
	int DGap;
	double defaultVPScale;   //量纲每像素
	double defaultAPScale;
	double defaultHPScale;
	double defaultSPScale;          //采样点每像素(ms)，默认1.5
	double defaultDPScale;   //D通道量纲每像素

	double SPScale;
	double LastSPScale;

	int topSpace;
	int rightSpace;
	int leftSpace;

};

enum CursorType{CURSOR,VZOOM,HZOOM};

#define Yellow  RGB(255,255,0)
#define Green  RGB(0,255,0)
#define Red RGB(255,0,0)
#define White RGB(255,255,255)
#define Orange RGB(255,160,50)
#define Gray RGB(100,100,100)
#define Pink RGB(205,0,205)
#define Gray00 RGB(230,230,230)
#define Gray0 RGB(190,190,190)
#define Gray1 RGB(150,150,150)
#define Virtual RGB(127,255,212)

	

#endif 