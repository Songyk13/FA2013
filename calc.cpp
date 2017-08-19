#include "stdafx.h"
#include "calc.h"
#include "math.h"
#include <algorithm>
#include <numeric>
#include "splab_vector.h"

using splab::PI;
using std::complex;
using std::polar;

std::stack<CString> Operator;
std::stack<AChannel> Operand;
CString Expression(_T("(A3+A2*5-1*A3)"));
bool quit;
int errc = 0;
std::locale loc ( "English" );
std::map<wchar_t,int> opPri;
std::map<CString,double> Constants;
std::map<CString,CString> Funcs;
std::map<CString,CString> FxDes;
std::vector<AChannel*> NumChannel;

AChannel EVA(const CString& inputExp,CComtrade* inputFile)
{
		opPri[_T('+')]=5;
		opPri[_T('-')]=5;
		opPri[_T('*')]=6;
		opPri[_T('/')]=6;
		opPri[_T(',')]=0;
		//Constants[_T("PI")]=3.14159265;
		//Constants[_T("EXP")]=2.71828183;
		quit = 0;
		CString tempOd;
		CString tempOr;
		CString topOr;
		int i=0;
		AChannel tempRe;
		AChannel temp1;
		AChannel temp2;
		AChannel Result;
		CString texp(inputExp);
		while(!quit&&errc<2000)
		{
			++errc;
			tempOd=_T("");
			tempOr=_T("");
			while(!texp.IsEmpty())       //查找数字
			{
				wchar_t t=texp[0];
				if(std::isdigit(texp[0],loc)||texp[0]==_T('.'))
				{
					tempOd = tempOd + texp[0];
					texp.Delete(0,1);
					continue;
				}
				else
					break;
			}
			if(!tempOd.IsEmpty())
			{
				//Operand.push(_wtof(tempOd));
				pushChannel(_wtof(tempOd),(int)(inputFile->AVector[0]->AData.size()));
				tempOd=_T("");
			}
			
			while(!texp.IsEmpty())              //查找通道：(A###)格式
			{
				wchar_t t=texp[0];
				if(texp[0]==_T('A'))
				{
					tempOd = tempOd + texp[0];
					texp.Delete(0,1);
					continue;
				}
				else if(!tempOd.IsEmpty()&&std::isdigit(texp[0],loc))
				{
					tempOd = tempOd + texp[0];
					texp.Delete(0,1);
					continue;
				}
				else
					break;
			}
			if(!tempOd.IsEmpty())
			{
				//Operand.push(_wtof(tempOd));
				tempOd.Delete(0,1);              //删除前导A
				//pushChannel(inputFile->AVector[_wtoi(tempOd)-1]);
				Operand.push(*(inputFile->AVector[_wtoi(tempOd)-1]));
				tempOd=_T("");
			}

			while(!texp.IsEmpty())       //查找常量
			{
				wchar_t t=texp[0];
				if(std::isupper(texp[0],loc))
				{
					tempOd = tempOd + texp[0];
					texp.Delete(0,1);
					continue;
				}
				else if(!tempOd.IsEmpty()&&std::isdigit(texp[0],loc)) //支持常量中出现数字
				{
					tempOd = tempOd + texp[0];
					texp.Delete(0,1);
					continue;
				}
				else
					break;
			}
			if(!tempOd.IsEmpty())
			{
				if (Constants.count(tempOd)==1)         //查找是否存在此常量
				{
					//Operand.push(_wtof(tempOd));
					pushChannel(Constants[tempOd],(int)(inputFile->AVector[0]->AData.size()));
					tempOd=_T("");
				}
				else
				{
					pushChannel(0,(int)(inputFile->AVector[0]->AData.size()));
					tempOd=_T("");
				}
			}

			while(!texp.IsEmpty())        //查找函数或左括号
			{
				if(std::isalpha(texp[0],loc)||texp[0]==_T('_'))
				{
					tempOr = tempOr + texp[0];
					texp.Delete(0,1);
					continue;
				}
				else if(texp[i]==_T('('))
				{
					tempOr = tempOr + texp[0];
					texp.Delete(0,1);
					break;
				}
				else
					break;
			}
			if(!tempOr.IsEmpty())
			{
				Operator.push(tempOr);
				tempOr = _T("");
			}
			if(opPri.count(texp[0])||texp[0]==_T(')')||texp[0]==_T(','))   //查找运算符或右括号
			{
				tempOr = tempOr + texp[0];
				texp.Delete(0,1);
				if(Operator.empty())
				{
					Operator.push(tempOr);
					tempOr = _T("");
				}
				else
				{
					topOr=Operator.top();
					if(topOr.Right(1)==_T('(')&&tempOr[0]!=_T(')'))
					{
						Operator.push(tempOr);
						tempOr = _T("");
					}
					else if(topOr.Right(1)==_T(',')&&tempOr[0]==_T(')'))     //5.9:检测多参数函数
					{
						Multi_Fx(inputFile);
					}
					else if(tempOr[0]==_T(')'))
					{
						topOr=Operator.top();
						Operator.pop();
						while(topOr.Right(1)!=_T('(')&&topOr.Right(1)!=_T(','))  //5.9: 检测逗号
						{
							temp2=Operand.top();
							Operand.pop();
							temp1=Operand.top();
							Operand.pop();
							switch(topOr[0])
							{
								case _T('+'):
								{
									tempRe = temp1+temp2;
									Operand.push(tempRe);
									tempOr=_T("");
									break;
								}
								case _T('-'):
								{
									tempRe = temp1-temp2;
									Operand.push(tempRe);
									tempOr=_T("");
									break;
								}
								case _T('*'):
								{
									tempRe = temp1*temp2;
									Operand.push(tempRe);
									tempOr=_T("");
									break;
								}
							 case _T('/'):
								{
									tempRe = temp1/temp2;
									Operand.push(tempRe);
									tempOr=_T("");
									break;
								}
							case _T('，'):        //5.9:大于一个逗号的处理
								{
									Operator.push(topOr);
									Operand.push(temp1);
									Operand.push(temp2);
									break;
								}
							}
							topOr=Operator.top();
							Operator.pop();
						}
						if(topOr.Right(1)==_T(','))        //5.9:多参数函数
						{
							Operator.push(topOr);
							Multi_Fx(inputFile);
						}
						else if(topOr.Right(1)==_T('('))     //5.9:单参数函数或者没有函数
						{
							topOr.Remove(_T('('));
							//if(topOr=="abs")
							//{
							//	temp1=Operand.top();
							//	Operand.pop();
							//	Operand.push(Chan_abs(&temp1));
							//}
							if(Funcs.count(topOr)||topOr.GetLength()>0)
							{
								temp1=Operand.top();
								Operand.pop();
								Operand.push(Chan_Fx(&temp1,topOr));
							}
							else if(Operator.empty()&&Operand.size()==1)
							{
								Result=Operand.top();
								Operand.pop();
								break;
							}
							else
							{
								continue;
							}
							//Operator.pop();
						}
					}
					else if(opPri[topOr[0]]>=opPri[tempOr[0]])
					{
						Operator.pop();
						temp2=Operand.top();
						Operand.pop();
						temp1=Operand.top();
						Operand.pop();
						switch(topOr[0])
						{
						case _T('+'):
							{
								tempRe = temp1+temp2;
								Operand.push(tempRe);
								break;
							}
						case _T('-'):
							{
								tempRe = temp1-temp2;
								Operand.push(tempRe);
								break;
							}
						case _T('*'):
							{
								tempRe = temp1*temp2;
								Operand.push(tempRe);
								break;
							}
						case _T('/'):
							{
								tempRe = temp1/temp2;
								Operand.push(tempRe);
								break;
							}
						case _T(','):        //5.9:大于一个逗号的处理
							{
								Operator.push(topOr);
								Operand.push(temp1);
								Operand.push(temp2);
								break;
							}
						}
						Operator.push(tempOr);
						tempOr=_T("");
					}
					
					else
					{
						Operator.push(tempOr);
						tempOr = _T("");
					}
				}
			}

		}
		if(errc >= 2000)
		{
			Result.uu = _T("ERR");
		}
		return Result;
}

//void pushChannel(AChannel Channel)
//{
//	Operand.push(Channel);
//}
void pushChannel(double num, int len)
{
	AChannel* NumChan = new AChannel(0,_T(""),_T(""),_T(""),_T(""),1,0,0,0,0,1,1,_T(""),0);
	NumChan->AData.resize(len);
	NumChan->AmpData.resize(len);
	NumChan->PhData.resize(len);
	for (int i=0; i<len; ++i)
	{
		NumChan->AData[i]=num;
		NumChan->AmpData[i]=num;
		NumChan->PhData[i]=0;
	}
	NumChan->ThreadFlag=1;
	Operand.push(*NumChan);
}
//AChannel Chan_abs(AChannel* Channel)
//{
//	AChannel* AbsChan = new AChannel(0,_T(""),_T(""),_T(""),_T(""),1,0,0,0,0,1,1,_T(""),0);
//	int len = (int)(Channel->AData.size());
//	AbsChan->AData.resize(len);
//	for (int i=0; i<len; ++i)
//	{
//		AbsChan->AData[i]=fabs(Channel->AData[i]);
//	}
//	return *AbsChan;
//}

AChannel Chan_Fx(AChannel* Channel, const CString& Fx)
{
	AChannel* Chan = new AChannel(0,_T(""),_T(""),_T(""),_T(""),1,0,0,0,0,1,1,_T(""),0);
	int len = (int)(Channel->AData.size());
	Chan->AData.resize(len);
	if (Fx==_T("abs"))
	{
		for (int i=0; i<len; ++i)
		{
			Chan->AData[i]=fabs(Channel->AData[i]);
		}
	}
	else if(Fx==_T("sin"))
	{
		for (int i=0; i<len; ++i)
		{
			Chan->AData[i]=sin(Channel->AData[i]);
		}
	}
	else
	{
		for (int i=0; i<len; ++i)
		{
			Chan->AData[i]=Channel->AData[i];
		}
		errc = 2001;
	}
	return *Chan;
}

void Multi_Fx(CComtrade* inputFile)
{
	std::stack<CString> mOr;
	std::stack<AChannel> mOd;
	CString tmOr;
	CString Fx;
	AChannel tChan;
	AChannel Re;
	mOd.push(Operand.top());
	Operand.pop();
	tmOr=Operator.top();
	while(tmOr.Right(1)!=_T('('))
	{
		mOd.push(Operand.top());
		Operand.pop();
		Operator.pop();
		mOr.push(tmOr);
		tmOr=Operator.top();
	}
	Operator.pop();
	Fx=tmOr;
	Fx.Remove(_T('('));
	int nParam = mOd.size();  //检测参数数量
	int nPre = _wtoi(Funcs[Fx].Right(1));
	if(nParam != nPre&&nPre != 0)
	{
		//处理
		Re = mOd.top();
		Operand.push(Re);
		errc = 2001;
		return;
	}
	////////////////////计算执行部分
	if(Fx==_T("harm_amp"))
	{
		tChan=mOd.top();
		mOd.pop();
		int h = (int)(mOd.top().AData[0]);
		int size = tChan.AData.size();
		Re.AData.resize(size);
		if(h<0)
		{
			for (int i=0;i<size;++i)
			{
				tChan.GetCurrentPhasor(i+1,inputFile->m_SampleRate);
				Re.AData[i]=tChan.APhasor.Amplitude*sin(tChan.APhasor.phase/180*PI);
			}
		}
		else
		{
			for (int i=0;i<size;++i)
			{
				int f = tChan.InstFFT(i+1,inputFile->m_SampleRate);
				if(f==0)
				{
					Re.AData[i]=tChan.Harmonics[h];
				}
				else if(f==1)
				{
					Re.AData[i]=0;
				}
			}
		}
	}
	else if(Fx==_T("basewave"))
	{
		tChan=mOd.top();
		mOd.pop();
		int h = (int)(mOd.top().AData[0]);
		int size = tChan.AData.size();
		Re.AData.resize(size);
		if(h==1)
		{
			for (int i=0;i<size;++i)
			{
				tChan.GetCurrentPhasor(i+1,inputFile->m_SampleRate);
				Re.AData[i]=tChan.APhasor.Amplitude*cos(tChan.APhasor.phase/180*PI);
			}
		}
		else if(h==2)
		{
			for (int i=0;i<size;++i)
			{
				tChan.GetCurrentPhasor(i+1,inputFile->m_SampleRate);
				Re.AData[i]=tChan.APhasor.Amplitude;
			}

		}
		else if(h==3)
		{
			for (int i=0;i<size;++i)
			{
				tChan.GetCurrentPhasor(i+1,inputFile->m_SampleRate);
				Re.AData[i]=tChan.APhasor.phase;
			}

		}
	}
	else if(Fx==_T("power_p"))
	{
		int size = mOd.top().AData.size();
		Re.AData.resize(size);
		for (int i=0;i<size;++i)
		{
			Re.AData[i]=0;
		}

		while(!mOd.empty())
		{
			AChannel U=mOd.top();
			mOd.pop();
			AChannel I=mOd.top();
			mOd.pop();
		
			double ang = 0;
			for (int i=0;i<size;++i)
			{
				U.GetCurrentPhasor(i+1,inputFile->m_SampleRate);
				I.GetCurrentPhasor(i+1,inputFile->m_SampleRate);
				ang=(U.APhasor.phase-I.APhasor.phase)/180*PI;
				Re.AData[i]+=U.APhasor.Amplitude*I.APhasor.Amplitude*cos(ang);
			}
		}
	}
	else if(Fx==_T("power_q"))
	{
		int size = mOd.top().AData.size();
		Re.AData.resize(size);
		for (int i=0;i<size;++i)
		{
			Re.AData[i]=0;
		}

		while(!mOd.empty())
		{
			AChannel U=mOd.top();
			mOd.pop();
			AChannel I=mOd.top();
			mOd.pop();
		
			double ang = 0;
			for (int i=0;i<size;++i)
			{
				U.GetCurrentPhasor(i+1,inputFile->m_SampleRate);
				I.GetCurrentPhasor(i+1,inputFile->m_SampleRate);
				ang=(U.APhasor.phase-I.APhasor.phase)/180*PI;
				Re.AData[i]+=U.APhasor.Amplitude*I.APhasor.Amplitude*sin(ang);
			}
		}
	}
	else if(Fx==_T("power_s"))
	{
		int size = mOd.top().AData.size();
		Re.AData.resize(size);
		for (int i=0;i<size;++i)
		{
			Re.AData[i]=0;
		}

		while(!mOd.empty())
		{
			AChannel U=mOd.top();
			mOd.pop();
			AChannel I=mOd.top();
			mOd.pop();
		
			//double ang = 0;
			for (int i=0;i<size;++i)
			{
				U.GetCurrentPhasor(i+1,inputFile->m_SampleRate);
				I.GetCurrentPhasor(i+1,inputFile->m_SampleRate);
				//ang=(U.APhasor.phase-I.APhasor.phase)/180*PI;
				Re.AData[i]+=U.APhasor.Amplitude*I.APhasor.Amplitude;
			}
		}
	}
	else if(Fx==_T("imp"))
	{
		int Flag;
		AChannel U=mOd.top();
		mOd.pop();
		AChannel I=mOd.top();
		mOd.pop();
		int h = (int)(mOd.top().AData[0]);
		int size = U.AData.size();
		Re.AData.resize(size);
		for (int i=0;i<size;++i)
		{
			Flag=U.GetCurrentPhasor(i+1,inputFile->m_SampleRate);
			I.GetCurrentPhasor(i+1,inputFile->m_SampleRate);
			complex<double> UA = (polar(U.APhasor.Amplitude,U.APhasor.phase*PI/180));
			complex<double> IA = (polar(I.APhasor.Amplitude,I.APhasor.phase*PI/180));
			switch(h)
			{
			case 1:
				{
					Re.AData[i]=std::real(UA/IA);
					break;
				}
			case 2:
				{
					Re.AData[i]=std::imag(UA/IA);
					break;
				}
			case 3:
				{
					Re.AData[i]=sqrt(pow(std::real(UA/IA),2)+pow(std::imag(UA/IA),2));
					break;
				}
			case 4:
				{
					Re.AData[i]=atan2(std::imag(UA/IA),std::real(UA/IA))*180/PI;
					break;
				}
			}
		}
	}
	else if(Fx==_T("make_sine"))
	{
		double rate;
		double A=mOd.top().AData[0];
		mOd.pop();
		double P=mOd.top().AData[0];
		mOd.pop();
		double F = mOd.top().AData[0];
		int size = mOd.top().AData.size();
		Re.AData.resize(size);
		int n=0;
		double t = 2*PI*F;
		rate = inputFile->m_SampleRate[n].samp;
		double r = t/rate;
		for(int i=0;i<size;++i)
		{
			if(i>inputFile->m_SampleRate[n].endsamp)
			{
				++n;
				rate = inputFile->m_SampleRate[n].samp;
				r = t/rate;
			}
			P += r;
			if(P>2*PI)
				P -= 2*PI;
			Re.AData[i]=A*sin(P);
		}
	}
	else if(Fx==_T("make_complex"))
	{
		double rl=mOd.top().AData[0];
		mOd.pop();
		double img = (int)(mOd.top().AData[0]);
		mOd.pop();
		int t = (int)(mOd.top().AData[0]);
		int size = mOd.top().AData.size();
		Re.AData.resize(size);
		Re.AmpData.resize(size);
		Re.PhData.resize(size);
		if(t==1)
		{
			for(int i=0;i<size;++i)
			{
				Re.AmpData[i]=sqrt(pow(rl,2)+pow(img,2));
				Re.PhData[i]=atan2(img,rl)*180/PI;
				Re.AData[i]=Re.AmpData[i];
			}
		}
		else
		{
			for(int i=0;i<size;++i)
			{
				Re.AmpData[i]=rl;
				Re.PhData[i]=img;
				Re.AData[i]=rl;
			}
		}
		Re.ThreadFlag=1;
	}
	else if(Fx==_T("complex"))
	{
		AChannel temp=mOd.top();
		mOd.pop();
		int t = (int)(mOd.top().AData[0]);
		int size = temp.AData.size();
		Re.AData.resize(size);
		Re.AmpData.resize(size);
		Re.PhData.resize(size);
		complex<double> C;
		for(int i=0;i<size;++i)
		{
			if(temp.PhData[i]>180)
			{
				Re.AData[i]=0;
				continue;
			}
			C=polar(temp.AmpData[i],temp.PhData[i]*PI/180);
			Re.AmpData[i]=temp.AmpData[i];
			Re.PhData[i]=temp.PhData[i];
			Re.ThreadFlag=1;
			switch(t)
			{
			case 1:
				{
					Re.AData[i]=std::real(C);
					break;
				}
			case 2:
				{
					Re.AData[i]=std::imag(C);
					break;
				}
			case 3:
				{
					Re.AData[i]=sqrt(pow(std::real(C),2)+pow(std::imag(C),2));
					break;
				}
			case 4:
				{
					Re.AData[i]=atan2(std::imag(C),std::real(C))*180/PI;
					break;
				}
			case 5:
				{
					Re.AData[i]=std::real(C);
					Re.PhData[i]=-temp.PhData[i];
					break;
				}
			}
		}

	}
	else
	{
		Re = mOd.top();
	}
	Operand.push(Re);
	return;
}

int exam(CString* result, CString& exp,CComtrade* inputFile)
{
	int r = 0;
	int ilchar = 0;
	int ilbr = 0;
	int ilod = 0;
	int ilfunc = 0;
	int ilor = 0;
	int iloth = 0;
	CString error0 = _T("表达式非法。");
	CString error1 = _T("\r\n发现以下非法字符：");
	CString error2 = _T("\r\n发现不匹配括号。");
	CString error3 = _T("\r\n发现以下非法运算数：");
	CString error4 = _T("\r\n发现以下非法函数调用：");
	CString error5 = _T("\r\n发现缺少运算数或缺少参数。");
	CString error6 = _T("\r\n发现其他无法识别的部分。");
	int len = exp.GetLength();
	wchar_t tchar;
	quit = 0;
	opPri[_T('+')]=5;
	opPri[_T('-')]=5;
	opPri[_T('*')]=6;
	opPri[_T('/')]=6;
	opPri[_T(',')]=0;
	int orflag = 0;
	int brflag = 0;

	for(int i=0;i<len;++i)
	{
		tchar = exp[i];
		if(std::isdigit(tchar,loc)||std::isalpha(tchar,loc)||opPri.count(tchar)||tchar==_T('(')||tchar==_T(')')||tchar==_T('_')||tchar==_T('.'))
		{
			if(opPri.count(tchar))
			{
				if(orflag == 1)
				{
					ilor = 1;
				}
				orflag = 1;
			}
			else
			{
				orflag = 0;
			}
			if(tchar==_T('(')||tchar==_T(','))
			{
				brflag = 1;
			}
			else if(tchar==_T(')')||tchar==_T(','))
			{
				if(brflag == 1)
				{
					ilor = 1;
				}
				brflag = 0;
			}
			else
			{
				brflag = 0;
			}
			continue;
		}
		else
		{
			++ilchar;
			error1 = error1 + _T(' ') + tchar;
		}
	}
	if(ilchar)
		error0 += error1;
	if(ilor)
		error0 += error5;
	//检测非法字符
	std::stack<wchar_t> br;
	wchar_t topchar;
	for(int i=0;i<len;++i)
	{
		tchar = exp[i];
		if(tchar==_T('('))
		{
			br.push(tchar);
		}
		else if(tchar==_T(')'))
		{
			if(br.empty())
			{
				ilbr=1;
				break;
			}
			else
			{
				topchar = br.top();
				br.pop();
				if(topchar!=_T('('))
				{
					ilbr=1;
					break;
				}
			}
		}
	}
	if(!br.empty())
		ilbr=1;
	if(ilbr)
		error0 += error2;
	//检测非法括号
	if(ilchar||ilbr)
	{
		*result = error0;
		return 1;
	}

	CString texp(exp);
	while(!(quit||texp.IsEmpty()))
	{
		CString tempOd=_T("");
		CString tempOr=_T("");
		while(!texp.IsEmpty())       //查找数字
		{
			wchar_t t=texp[0];
			if(std::isdigit(texp[0],loc)||texp[0]==_T('.'))
			{
				tempOd = tempOd + texp[0];
				texp.Delete(0,1);
				continue;
			}
			else
				break;
		}
		if(!tempOd.IsEmpty())
		{
			_wtof(tempOd);
			tempOd=_T("");
		}
			
		while(!texp.IsEmpty())              //查找通道：(A###)格式
		{
			wchar_t t=texp[0];
			if(texp[0]==_T('A'))
			{
				tempOd = tempOd + texp[0];
				texp.Delete(0,1);
				continue;
			}
			else if(!tempOd.IsEmpty()&&std::isdigit(texp[0],loc))
			{
				tempOd = tempOd + texp[0];
				texp.Delete(0,1);
				continue;
			}
			else
				break;
		}
		if(!tempOd.IsEmpty())
		{
			//Operand.push(_wtof(tempOd));
			tempOd.Delete(0,1);              //删除前导A
			//pushChannel(inputFile->AVector[_wtoi(tempOd)-1]);
			if(_wtoi(tempOd)>inputFile->m_ANumber)
			{
				++ilod;
				error3 = error3 + _T(' ') + _T('A') + tempOd;
			}
			tempOd=_T("");
		}

		while(!texp.IsEmpty())       //查找常量
		{
			wchar_t t=texp[0];
			if(std::isupper(texp[0],loc))
			{
				tempOd = tempOd + texp[0];
				texp.Delete(0,1);
				continue;
			}
			else if(!tempOd.IsEmpty()&&std::isdigit(texp[0],loc)) //支持常量中出现数字
			{
				tempOd = tempOd + texp[0];
				texp.Delete(0,1);
				continue;
			}
			else
				break;
		}
		if(!tempOd.IsEmpty())
		{
			if (Constants.count(tempOd)==0)         //查找是否存在此常量
			{
				++ilod;
				error3 = error3 + _T(' ') + tempOd;
				tempOd=_T("");
			}
		}
		while(!texp.IsEmpty())        //查找函数或左括号
		{
			if(std::isalpha(texp[0],loc)||texp[0]==_T('_'))
			{
				tempOr = tempOr + texp[0];
				texp.Delete(0,1);
				continue;
			}
			else if(texp[0]==_T('('))
			{
				//tempOr = tempOr + texp[0];
				texp.Delete(0,1);
				break;
			}
			else
				break;
		}
		if(!tempOr.IsEmpty())
		{
			//Operator.push(tempOr);
				
			if (Funcs.count(tempOr)==0)         //查找是否存在此函数
			{
				++ilfunc;
				error4 = error4 + _T(' ') + tempOr;
				tempOr = _T("");
			}
		}
			
		if(opPri.count(texp[0])||texp[0]==_T(')'))   //查找运算符或右括号
		{
			tempOr = tempOr + texp[0];
			texp.Delete(0,1);
		}
	}
	if (ilod)
	{
		error0 += error3;
	}
	if (ilfunc)
	{
		error0 += error4;
	}
	//检测非法运算数
	//检测非法函数
	*result = error0;
	r = ilchar||ilbr||ilod||ilfunc||ilor||iloth;
	return r;
}