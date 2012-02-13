// BusinessTransferEngine.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

const int MAX_OPERATOR = 10;
const int MAX_RETRY_STEP = 10;
const int MAX_FAIL_STEP = 3;
const int MAX_OPERATOR_STEP = 30;
const int MAX_TEST_COUNT = 100;

enum OperatorAnswer{SUCCESS,FAIL,UNKNOW};
const char g_answer_string[3][10]={"Success","Fail","Unknow"};

enum BussinessOperatorType {LocalCheck,TradeFreeze,AvailCheck,BankOut,TradeOut,LocalOut,TradeUnfreeze,TradeUnout,BankUnout1,BankUnout2};

typedef int (* BussinessFunc)(int BussinessNo);

struct stBussinessOperator
{
	char* Note;
	BussinessFunc Execute;
	struct stBussinessOperator *SuccessOperator;
	struct stBussinessOperator *FailOperator;
	struct stBussinessOperator *UnknowOperator;
};
typedef struct stBussinessOperator BussinessOperator;

typedef struct
{
	int UnknowTime;
	int Answer;
}OpertorTestAnswerStrategy;

BussinessOperator g_bolist[MAX_OPERATOR];
BussinessOperator* g_bohead;

//Transfer g_translist[MAX_TEST_COUNT];
OpertorTestAnswerStrategy g_testlist[MAX_TEST_COUNT][MAX_OPERATOR] = {0};
int g_testcount = 0;

int MockOperator(int tid,int oid)
{
	int rt;
	OpertorTestAnswerStrategy *p = &g_testlist[tid][oid];
	if(p->UnknowTime > 0)
	{
		p->UnknowTime--;
		rt = UNKNOW;
	}
	else
	{
		rt = p->Answer;
	}
	printf("Transfer(%d).%s:%s\n",tid,g_bolist[oid].Note,g_answer_string[rt]);
	return rt;
}

int MockLocalCheck(int id)
{
	return MockOperator(id,LocalCheck);
}

int MockTradeFreeze(int id)
{
	return MockOperator(id,TradeFreeze);
}

int MockAvailCheck(int id)
{
	return MockOperator(id,AvailCheck);
}

int MockBankOut(int id)
{
	return MockOperator(id,BankOut);
}

int MockTradeOut(int id)
{
	return MockOperator(id,TradeOut);
}

int MockLocalOut(int id)
{
	return MockOperator(id,LocalOut);
}

int MockTradeUnfreeze(int id)
{
	return MockOperator(id,TradeUnfreeze);
}

int MockTradeUnout(int id)
{
	return MockOperator(id,TradeUnout);
}

int MockBankUnout1(int id)
{
	return MockOperator(id,BankUnout1);
}

int MockBankUnout2(int id)
{
	return MockOperator(id,BankUnout2);
}

void InitOneBussinessOperator(int id,char* note,BussinessFunc func,int success,int fail,int unknow)
{
	g_bolist[id].Note = note;
	g_bolist[id].Execute = func;
	g_bolist[id].SuccessOperator = (success >= 0 ? &g_bolist[success]:0);
	g_bolist[id].FailOperator = (fail >= 0 ? &g_bolist[fail]:0);
	g_bolist[id].UnknowOperator = (unknow >= 0 ? &g_bolist[unknow]:0);
}

bool InitBussinessOperatorList()
{
	InitOneBussinessOperator(LocalCheck,"LocalCheck",MockLocalCheck,TradeFreeze,-1,LocalCheck);
	InitOneBussinessOperator(TradeFreeze,"TradeFreeze",MockTradeFreeze,AvailCheck,-1,TradeFreeze);
	InitOneBussinessOperator(AvailCheck,"AvailCheck",MockAvailCheck,BankOut,TradeUnfreeze,AvailCheck);
	InitOneBussinessOperator(BankOut,"BankOut",MockBankOut,TradeOut,TradeUnfreeze,BankOut);
	InitOneBussinessOperator(TradeOut,"TradeOut",MockTradeOut,LocalOut,BankUnout1,TradeOut);
	InitOneBussinessOperator(LocalOut,"LocalOut",MockLocalOut,-1,BankUnout2,LocalOut);
	InitOneBussinessOperator(TradeUnfreeze,"TradeUnfreeze",MockTradeUnfreeze,-1,TradeUnfreeze,TradeUnfreeze);
	InitOneBussinessOperator(TradeUnout,"TradeUnout",MockTradeUnout,-1,TradeUnout,TradeUnout);
	InitOneBussinessOperator(BankUnout1,"BankUnout1",MockBankUnout1,TradeUnfreeze,BankUnout1,BankUnout1);
	InitOneBussinessOperator(BankUnout2,"BankUnout2",MockBankUnout2,TradeUnout,BankUnout2,BankUnout2);

	g_bohead = &g_bolist[LocalCheck];
	return true;
}

int InitTestList()
{
	//LocalCheck,TradeFreeze,AvailCheck,BankOut,TradeOut,LocalOut,TradeUnfreeze,TradeUnout,BankUnout1,BankUnout2
	g_testcount++;

	g_testlist[g_testcount][LocalCheck].Answer = FAIL;
	g_testcount++;

	g_testlist[g_testcount][TradeFreeze].Answer = FAIL;
	g_testcount++;

	g_testlist[g_testcount][AvailCheck].Answer = FAIL;
	g_testcount++;

	g_testlist[g_testcount][BankOut].Answer = FAIL;
	g_testcount++;

	g_testlist[g_testcount][TradeOut].Answer = FAIL;
	g_testcount++;

	g_testlist[g_testcount][LocalOut].Answer = FAIL;
	g_testcount++;

	g_testlist[g_testcount][LocalCheck].UnknowTime = 1;
	g_testcount++;

	g_testlist[g_testcount][LocalCheck].UnknowTime = 2;
	g_testcount++;

	g_testlist[g_testcount][LocalCheck].UnknowTime = MAX_RETRY_STEP + 1;
	g_testcount++;

	g_testlist[g_testcount][LocalCheck].UnknowTime = 1;
	g_testlist[g_testcount][LocalCheck].Answer = FAIL;
	g_testcount++;

	g_testlist[g_testcount][LocalCheck].UnknowTime = 3;
	g_testlist[g_testcount][LocalCheck].Answer = FAIL;
	g_testcount++;

	g_testlist[g_testcount][BankOut].UnknowTime = 2;
	g_testcount++;

	g_testlist[g_testcount][LocalCheck].UnknowTime = 1;
	g_testlist[g_testcount][BankOut].UnknowTime = 3;
	g_testcount++;

	g_testlist[g_testcount][LocalCheck].UnknowTime = 7;
	g_testlist[g_testcount][BankOut].UnknowTime = 8;
	g_testcount++;

	g_testlist[g_testcount][BankOut].UnknowTime = 2;
	g_testlist[g_testcount][BankOut].Answer = FAIL;
	g_testcount++;

	g_testlist[g_testcount][AvailCheck].Answer = FAIL;
	g_testlist[g_testcount][TradeUnfreeze].Answer = FAIL;
	g_testcount++;

	g_testlist[g_testcount][BankOut].Answer = FAIL;
	g_testlist[g_testcount][TradeUnfreeze].Answer = FAIL;
	g_testcount++;

	return g_testcount;
}

void ProcessTransfer(int ID)
{
	printf("Transfer(%d).begin\n",ID);
	BussinessOperator* p = g_bohead;
	int count = 0;
	int unknowtime = 0;
	int failtime = 0;
	int ret;
	while(p)
	{
		if(unknowtime > MAX_RETRY_STEP)
		{
			printf("Transfer(%d):too many unknows,break\n",ID);
			break;
		}

		if(failtime > MAX_FAIL_STEP)
		{
			printf("Transfer(%d):too many fails,break\n",ID);
			break;
		}

		if(count > MAX_OPERATOR_STEP)
		{
			printf("Transfer(%d):too many steps,break\n",ID);
			break;
		}

		ret = p->Execute(ID);
		switch(ret)
		{
			case SUCCESS:
				unknowtime = 0;
				failtime = 0;
				p = p->SuccessOperator;
				break;
			case FAIL:
				unknowtime = 0;
				failtime++;
				p = p->FailOperator;
				break;
			default:
				unknowtime++;
				failtime = 0;
				p = p->UnknowOperator;
		}
		count++;
	}

	if(!p)
	{
		printf("Transfer(%d).finished\n",ID);
	}
	printf("-------------------------------------\n");
}

void test()
{
	for(int i = 0; i < g_testcount; i++)
	{
		ProcessTransfer(i);
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	InitBussinessOperatorList();
	InitTestList();
	test();
	getchar();
	return 0;
}

