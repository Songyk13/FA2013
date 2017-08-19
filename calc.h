#pragma once

#include "stdafx.h"
#include "Comtrade.h"
#include "AChannel.h"

#include <map>
#include <stack>
#include <locale>


//std::stack<CString> Operator;
//std::stack<AChannel*> Operand;
//CString Expression(_T("(3+abs((2-6/2)*5))"));
//bool quit;
//std::locale loc ( "English" );
//std::map<wchar_t,int> opPri;
//std::map<CString,double> Constants;

int exam(CString* result, CString& inputExp,CComtrade* inputFile);
AChannel EVA(const CString& inputExp, CComtrade* inputFile);
//void pushChannel(AChannel Channel);
void pushChannel(double num, int len);
//AChannel Chan_abs(AChannel* Channel);
AChannel Chan_Fx(AChannel* Channel, const CString& Fx);
void Multi_Fx(CComtrade* inputFile);