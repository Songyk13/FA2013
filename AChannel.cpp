#include "StdAfx.h"
#include "AChannel.h"
#include "fftpf.h"
#include "FA2013.h"
#include "splab_vector.h"

using splab::PI;
using std::complex;
using std::polar;

AChannel::~AChannel(void)
{
}

//AChannel(const AChannel&)
//{
//}

//AChannel& operator=(const AChannel&)
//{
//}
AChannel AChannel::operator+(const AChannel& AChannel2) const
{
	AChannel* temp = new AChannel(0,_T(""),_T(""),_T(""),_T(""),1,0,0,0,0,1,1,_T(""),0);
	int len = (int)AData.size();
	temp->AData.resize(len);
	temp->AmpData.resize(len);
	temp->PhData.resize(len);
	complex<double> C1,C2,Re;
	for (int i=0; i<len; ++i)
	{
		//temp->AData[i]=(AData[i]*multiplier+offset)+(AChannel2.AData[i]*AChannel2.multiplier+AChannel2.offset);
		temp->AData[i]=AData[i]+AChannel2.AData[i];
		if(PhData[i]>180||AChannel2.PhData[i]>180)
		{
			temp->PhData[i]=360;
			temp->AmpData[i]=0;
			continue;
		}
		C1=polar(AmpData[i],PhData[i]*PI/180);
		C2=polar(AChannel2.AmpData[i],AChannel2.PhData[i]*PI/180);
		Re=C1+C2;
		temp->AmpData[i]=sqrt(pow(std::real(Re),2)+pow(std::imag(Re),2));
		temp->PhData[i]=atan2(std::imag(Re),std::real(Re))*180/PI;
	}
	return *temp;
}
AChannel AChannel::operator+(const double& Num) const
{
	AChannel* temp = new AChannel(0,_T(""),_T(""),_T(""),_T(""),1,0,0,0,0,1,1,_T(""),0);
	int len = (int)AData.size();
	temp->AData.resize(len);
	for (int i=0; i<len; ++i)
	{
		//temp->AData[i]=(AData[i]*multiplier+offset)+Num;
		temp->AData[i]=AData[i]+Num;
	}
	return *temp;
}
AChannel AChannel::operator-(const AChannel& AChannel2) const
{
	AChannel* temp = new AChannel(0,_T(""),_T(""),_T(""),_T(""),1,0,0,0,0,1,1,_T(""),0);
	int len = (int)AData.size();
	temp->AData.resize(len);
	temp->AmpData.resize(len);
	temp->PhData.resize(len);
	complex<double> C1,C2,Re;
	for (int i=0; i<len; ++i)
	{
		temp->AData[i]=AData[i]-AChannel2.AData[i];
		if(PhData[i]>180||AChannel2.PhData[i]>180)
		{
			temp->PhData[i]=360;
			temp->AmpData[i]=0;
			continue;
		}
		C1=polar(AmpData[i],PhData[i]*PI/180);
		C2=polar(AChannel2.AmpData[i],AChannel2.PhData[i]*PI/180);
		Re=C1-C2;
		temp->AmpData[i]=sqrt(pow(std::real(Re),2)+pow(std::imag(Re),2));
		temp->PhData[i]=atan2(std::imag(Re),std::real(Re))*180/PI;
	}
	return *temp;
}
AChannel AChannel::operator-(const double& Num) const
{
	AChannel* temp = new AChannel(0,_T(""),_T(""),_T(""),_T(""),1,0,0,0,0,1,1,_T(""),0);
	int len = (int)AData.size();
	temp->AData.resize(len);
	for (int i=0; i<len; ++i)
	{
		temp->AData[i]=AData[i]-Num;
	}
	return *temp;
}
AChannel AChannel::operator*(const AChannel& AChannel2) const
{
	AChannel* temp = new AChannel(0,_T(""),_T(""),_T(""),_T(""),1,0,0,0,0,1,1,_T(""),0);
	int len = (int)AData.size();
	temp->AData.resize(len);
	temp->AmpData.resize(len);
	temp->PhData.resize(len);
	complex<double> C1,C2,Re;
	for (int i=0; i<len; ++i)
	{
		temp->AData[i]=AData[i]*AChannel2.AData[i];
		if(PhData[i]>180||AChannel2.PhData[i]>180)
		{
			temp->PhData[i]=360;
			temp->AmpData[i]=0;
			continue;
		}
		C1=polar(AmpData[i],PhData[i]*PI/180);
		C2=polar(AChannel2.AmpData[i],AChannel2.PhData[i]*PI/180);
		Re=C1*C2;
		temp->AmpData[i]=sqrt(pow(std::real(Re),2)+pow(std::imag(Re),2));
		temp->PhData[i]=atan2(std::imag(Re),std::real(Re))*180/PI;
	}
	return *temp;
}

AChannel AChannel::operator*(const double& Num) const
{
	AChannel* temp = new AChannel(0,_T(""),_T(""),_T(""),_T(""),1,0,0,0,0,1,1,_T(""),0);
	int len = (int)AData.size();
	temp->AData.resize(len);
	for (int i=0; i<len; ++i)
	{
		temp->AData[i]=AData[i]*Num;
	}
	return *temp;
}

AChannel AChannel::operator/(const AChannel& AChannel2) const
{
	AChannel* temp = new AChannel(0,_T(""),_T(""),_T(""),_T(""),1,0,0,0,0,1,1,_T(""),0);
	int len = (int)AData.size();
	temp->AData.resize(len);
	temp->AmpData.resize(len);
	temp->PhData.resize(len);
	complex<double> C1,C2,Re;
	for (int i=0; i<len; ++i)
	{
		temp->AData[i]=AData[i]/AChannel2.AData[i];
		if(PhData[i]>180||AChannel2.PhData[i]>180)
		{
			temp->PhData[i]=360;
			temp->AmpData[i]=0;
			continue;
		}
		C1=polar(AmpData[i],PhData[i]*PI/180);
		C2=polar(AChannel2.AmpData[i],AChannel2.PhData[i]*PI/180);
		Re=C1/C2;
		temp->AmpData[i]=sqrt(pow(std::real(Re),2)+pow(std::imag(Re),2));
		temp->PhData[i]=atan2(std::imag(Re),std::real(Re))*180/PI;
	}
	return *temp;
}

AChannel AChannel::operator/(const double& Num) const
{
	AChannel* temp = new AChannel(0,_T(""),_T(""),_T(""),_T(""),1,0,0,0,0,1,1,_T(""),0);
	int len = (int)AData.size();
	temp->AData.resize(len);
	for (int i=0; i<len; ++i)
	{
		temp->AData[i]=AData[i]/Num;
	}
	return *temp;
}
AChannel operator+(const double& Num,const AChannel& AChannel2)
{
	AChannel* temp = new AChannel(0,_T(""),_T(""),_T(""),_T(""),1,0,0,0,0,1,1,_T(""),0);
	int len = (int)AChannel2.AData.size();
	temp->AData.resize(len);
	for (int i=0; i<len; ++i)
	{
		temp->AData[i]=Num+AChannel2.AData[i];
	}
	return *temp;
}
AChannel operator-(const double& Num,const AChannel& AChannel2)
{
	AChannel* temp = new AChannel(0,_T(""),_T(""),_T(""),_T(""),1,0,0,0,0,1,1,_T(""),0);
	int len = (int)AChannel2.AData.size();
	temp->AData.resize(len);
	for (int i=0; i<len; ++i)
	{
		temp->AData[i]=Num-AChannel2.AData[i];
	}
	return *temp;
}
AChannel operator*(const double& Num,const AChannel& AChannel2)
{
	AChannel* temp = new AChannel(0,_T(""),_T(""),_T(""),_T(""),1,0,0,0,0,1,1,_T(""),0);
	int len = (int)AChannel2.AData.size();
	temp->AData.resize(len);
	for (int i=0; i<len; ++i)
	{
		temp->AData[i]=Num*AChannel2.AData[i];
	}
	return *temp;
}

//AChannel AChannel::operator/(const AChannel& AChannel2) const
//{
//}

AChannel::AChannel(void)
{
	this->An=0;
	this->name=_T("");
	this->ph=_T("");
	this->ccbm=_T("");
	this->uu=_T("");
	this->multiplier=0;
	this->offset=0;
	this->skew=0;
	this->min=0;
	this->max=0;
	this->primary=0;
	this->secondary=0;
	this->PS=_T("");
	this->AData.resize(0);

	this->maxAData=0;
	this->m_drawPara.UPScale=0;
	this->ThreadFlag = 0;
}
AChannel::AChannel(int _An,CString _name, CString _ph, CString _ccbm, CString _uu, double _multiplier, double _offset, double _skew, int _min,int _max,
		double _primary, double _secondary, CString _PS, bool Real)
{
	An=_An;
	name=_name;
	ph=_ph;
	ccbm=_ccbm;
	uu=_uu;
	multiplier=_multiplier;
	offset=_offset;
	skew=_skew;
	min=_min;
	max=_max;
	primary=_primary;
	secondary=_secondary;
	PS=_PS;
	AData.resize(1);
	isReal=Real;
	ThreadFlag =0;
	maxAData=0;
}

DChannel::DChannel(void)
{
	this->Dn=0;
	this->name=_T("");
	this->ph=_T("");
	this->ccbm=_T("");
	this->y=0;
	DData.resize(1);
}

DChannel::~DChannel(void)
{
}

DChannel::DChannel(int _Dn,CString _name, CString _ph, CString _ccbm, int _y)
{
	Dn=_Dn;
    name=_name;
	ph=_ph;
	ccbm=_ccbm;
	y=_y;
	this->DData.resize(1);
}


int AChannel::InstFFT(int SamplePoint,std::vector<SampleRate>& m_SampleRate)
{
	double SampRate = 0;
	if (SamplePoint >= m_SampleRate[0].samp*0.02 && SamplePoint <= m_SampleRate[0].endsamp)
		SampRate = m_SampleRate[0].samp;
	else if (SamplePoint < m_SampleRate[0].endsamp)
		return 1;
	else
	{
		for (int i = 1; i < (int)m_SampleRate.size(); ++i)   //size类型
		{
			if (SamplePoint <= m_SampleRate[i].endsamp && SamplePoint >= m_SampleRate[i-1].endsamp + m_SampleRate[i].samp*0.02)
			{
				SampRate = m_SampleRate[i].samp;
				break;
			}
		}
		if (SampRate == 0)
			return 1;
	}
	if ( SampRate <= 2*FREQ)
		return 1;
	MyFFT((int)(SamplePoint - SampRate*0.02 +1), SamplePoint, SampRate);
	return 0;
}


int AChannel::MyFFT(int start, int end, double SampRate)
{
	this->Harmonics.resize((int)(SampRate/50));
	splab::Vector<std::complex<double>> Sk;
	splab::Vector<double> sn;
	splab::FFTPF<double> FFTTrans;
	int L=end-start+1;
	sn.resize(L);
	Sk.resize(L);
	int j=0;
	for(int i=start;i<=end;i++)
		sn[j++]=this->AData[i];
	FFTTrans.fft(sn,Sk);
	for(int i=0;i!=L;i++)
		this->Harmonics[i]=abs(Sk[i])*2/L;
	this->Ph_base=arg(Sk[1]);
	return 0;
}


int AChannel::GetCurrentPhasor(int SamplePoint,std::vector<SampleRate>& m_SampleRate)
{
	using splab::PI;
	int Flag=1;
	if (this->ThreadFlag == 1)
	{
		if (this->PhData[SamplePoint-1]>180)
			return 1;
		else
		{
			this->APhasor.Amplitude = this->AmpData[SamplePoint-1];
			this->APhasor.phase = this->PhData[SamplePoint-1];
		}
	}
	else
	{
		Flag=InstFFT(SamplePoint,m_SampleRate);
		if(Flag==1)
		{
			this->APhasor.Amplitude =0.000;   //130528 原为-1
			this->APhasor.phase = 360.0;
			return 1;
		}
		double Amp = this->Harmonics[1];
		this->APhasor.Amplitude = Amp;
		double Ph = this->Ph_base;
		int L = Harmonics.size();
		int SampRate = L*FREQ;
		double Current_Base = Amp*cos(FREQ*2*PI*(L-1)/SampRate + Ph );
		double Last_Base = Amp*cos(FREQ*2*PI*(L-2)/SampRate + Ph );
		if (Last_Base < Current_Base)
			this->APhasor.phase = 180/PI*asin(Current_Base/Amp);
		else if(Last_Base > Current_Base)
			this->APhasor.phase = 180 - 180/PI*asin(Current_Base/Amp);
		if(this->APhasor.phase > 180)
			this->APhasor.phase = this->APhasor.phase - 360;
	}
	return 0;
}

int AChannel::drawChannel(CDC* pDC, CPoint pPos,int startSample, int endSample, drawParaCom drawParam)
{
	int x,y;
	double SPScale = drawParam.SPScale;
	double UPScale;
	if(this->m_drawPara.UPScale==0)
	{
		switch(this->uu[0])
		{
		case _T('V'):
			{
				UPScale = drawParam.defaultVPScale;
				break;
			}
		case _T('A'):
			{
				UPScale = drawParam.defaultAPScale;
				break;
			}
		case _T('H'):
			{
				UPScale = drawParam.defaultHPScale;
				break;
			}
		}
	}
	else
	{
		UPScale=m_drawPara.UPScale;
	}
	CPen* pOld;
	CBrush* pOldb;
	extern CFA2013App theApp;

	int VSPACE=m_drawPara.VSpace;
	int Length=(int)((endSample-startSample)*SPScale);
	//刻度线
	pOld = pDC->SelectObject(&theApp.PenGridGray);
	pDC->MoveTo(pPos.x,pPos.y-VSPACE/2);
	pDC->LineTo(Length+pPos.x,pPos.y-VSPACE/2);
	pDC->MoveTo(pPos.x,pPos.y-VSPACE/4);
	pDC->LineTo(Length+pPos.x,pPos.y-VSPACE/4);
	pDC->MoveTo(pPos.x,pPos.y);
	pDC->LineTo(Length+pPos.x,pPos.y);
	pDC->MoveTo(pPos.x,pPos.y+VSPACE/4);
	pDC->LineTo(Length+pPos.x,pPos.y+VSPACE/4);
	pDC->MoveTo(pPos.x,pPos.y+VSPACE/2);
	pDC->LineTo(Length+pPos.x,pPos.y+VSPACE/2);
	for(int i=0;i<=40;i++)
	{
		x= pPos.x+i*100;
		if(x<(Length+pPos.x))
		{
			pDC->MoveTo(x,pPos.y-VSPACE/2);
			pDC->LineTo(x,pPos.y+VSPACE/2);
		}
	}
	//曲线
	//if (!isReal)  pOld = pDC->SelectObject(&theApp.PenV);
	int incre = 1;
	if(SPScale < 1)
	{
		incre = (int)(1/SPScale);
	}//改变绘图精度
	if (ph==_T("A"))  pOld = pDC->SelectObject(&theApp.PenA);
    else if (ph==_T("B"))  pOld = pDC->SelectObject(&theApp.PenB);
	else if (ph==_T("C"))  pOld = pDC->SelectObject(&theApp.PenC);
	else if (ph==_T("N"))  pOld = pDC->SelectObject(&theApp.PenN);
	else if ((ph==_T(" "))||(ph==_T("")))   pOld = pDC->SelectObject(&theApp.PenF);
	else if (ph==_T("V"))  pOld = pDC->SelectObject(&theApp.PenV);
	pDC->MoveTo(pPos.x,pPos.y+(int)((-AData[startSample])/UPScale));
	int asize = AData.size();
	for(int i=startSample;i<=endSample&&i<asize;i+=incre)
	{
		x = (int)(SPScale*i-SPScale*startSample)+pPos.x;
		y = (int)(-AData[i]/UPScale+pPos.y);
		pDC->LineTo(x,y);
		pDC->MoveTo(x,y);
	}
	pDC->MoveTo(pPos.x,pPos.y);
	pDC->LineTo(pPos.x-20,pPos.y);
	pDC->LineTo(pPos.x-20,pPos.y-7);
	pDC->SelectObject(pOld);
	//文字
	CFont* def_font = pDC->SelectObject(&theApp.font);
	if (ph==_T("A")) 	pDC->SetTextColor(Yellow);
    else if (ph==_T("B"))  	pDC->SetTextColor(Green);
	else if (ph==_T("C"))  	pDC->SetTextColor(Red);
	else if (ph==_T("N"))  	pDC->SetTextColor(White);
	else if ((ph==_T(" "))||(ph==_T("")))   pDC->SetTextColor(Orange);
	else if (ph==_T("V"))  pDC->SetTextColor(Virtual);
	pDC->SetBkMode(TRANSPARENT);
	pDC->TextOutW(5+pPos.x,pPos.y-VSPACE/2-drawParam.VGap,this->name);
	pDC->SelectObject(def_font);
	//箭头
	if (ph==_T("A"))  pOldb=pDC->SelectObject(&theApp.BrushA);
    else if (ph==_T("B"))  pOldb=pDC->SelectObject(&theApp.BrushB);
	else if (ph==_T("C"))  pOldb=pDC->SelectObject(&theApp.BrushC);
	else if (ph==_T("N"))  pOldb=pDC->SelectObject(&theApp.BrushN);
	else if ((ph==_T(" "))||(ph==_T("")))   pOldb=pDC->SelectObject(&theApp.BrushF);
	else if (ph==_T("V"))  pOldb = pDC->SelectObject(&theApp.BrushV);
	pDC->BeginPath();
	pDC->MoveTo(pPos.x,pPos.y);
	pDC->LineTo(pPos.x-10,pPos.y+7);
	pDC->LineTo(pPos.x-10,pPos.y-7);
	pDC->LineTo(pPos.x,pPos.y);
	pDC->EndPath();
	pDC->FillPath();
	pDC->SelectObject(pOldb);
	return 0;
}
int DChannel::drawChannel(CDC* pDC, CPoint pPos,int startSample, int endSample, drawParaCom drawParam)
{
	int x,y;
	double SPScale = drawParam.SPScale;
	double UPScale = drawParam.defaultDPScale;
	int Length=(int)((endSample-startSample)*SPScale);
	CPen* pOld;
	extern CFA2013App theApp;

	pDC->MoveTo(pPos.x,(int)(pPos.y+1/UPScale/2));
	pOld=pDC->SelectObject(&theApp.PenGridGray);
	pDC->LineTo(Length+pPos.x,(int)(pPos.y+1/UPScale/2));

	pDC->MoveTo(pPos.x,(int)(pPos.y-1/UPScale/2));
	pDC->SelectObject(&theApp.PenGridGray);
	pDC->LineTo(Length+pPos.x,(int)(pPos.y-1/UPScale/2));

	pDC->MoveTo(pPos.x,(int)(pPos.y+1/UPScale/2+(-DData[startSample])/UPScale));
	int dsize = DData.size();
	for(int i=startSample;i<=endSample&&i<dsize;i++)
	{
		x = (int)(SPScale*i-SPScale*startSample+pPos.x);
		y = (int)(-DData[i]/UPScale+pPos.y+1/UPScale/2);
		if (DData[i]==0)
			pDC->SelectObject(&theApp.Pen0);
		else
			pDC->SelectObject(&theApp.Pen1);
		pDC->LineTo(x,y);
		pDC->MoveTo(x,y);
	}
	pDC->SelectObject(pOld);

	CFont* def_font = pDC->SelectObject(&theApp.font);
	pDC->SetTextColor(RGB(255,255,255));
	pDC->SetBkMode(TRANSPARENT);
	pDC->TextOutW(5+pPos.x,pPos.y-drawParam.defaultDVSpace/2-drawParam.DGap,this->name);
	pDC->SelectObject(def_font);
	return 0;
}