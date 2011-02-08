#include <stdio.h>
#include <stdlib.h>
#include "CpuTime.h"

using namespace TIMING;

// Must initialize in cpp file to avoid multiple definitions
std::vector<TIMING::CpuTime*> TIMING::CpuTime::CpuTimeList;

//----------------------------------------------------------------------
CpuTime::CpuTime()
{
	name = "";
	scale = 0.;
	count = 0;
	unit = "ms";
	t = 0.0; 
	t1 = 0;
	t2 = 0;

	this->nbCalls = 0;
	this->offset = 0;
	reset();
}
//----------------------------------------------------------------------
CpuTime::CpuTime(const char* name_, int nbCalls, int offset)
{
	name = name_;

	switch (CLOCKS_PER_SEC) {
	case 1000000:
		scale = 1000. / (double) CLOCKS_PER_SEC;
		break;
	case 1000:
		scale = 1. / (double) CLOCKS_PER_SEC;
		break;
	default:
		printf("CpuTime does handle this case\n");
		printf("CLOCKS_PER_SEC= %ld\n", (long) CLOCKS_PER_SEC);
		exit(0);
	}
	count = 0;
	unit = "ms";
	t = 0.0;
	t1 = 0;
	t2 = 0;

	this->nbCalls = nbCalls;
	this->offset = offset;
	CpuTimeList.push_back(this);
	//printf("constructor: this= %d, name= %s\n", this, name.c_str());
	reset();
}
//----------------------------------------------------------------------
CpuTime::CpuTime(const CpuTime& t)
{
	name = t.name;
	scale = t.scale;
	count = t.count;
	this->t = t.t;
	this->t1 = t.t1;
	this->t2 = t.t2;
	this->nbCalls = t.nbCalls;
	this->offset = t.offset;
	this->CpuTimeList.push_back(this);
	reset();
}
//----------------------------------------------------------------------
CpuTime::~CpuTime()
{
}
//----------------------------------------------------------------------
void CpuTime::reset()
{
	t = 0.0;
	t1 = clock();
	count = 0;
}

//----------------------------------------------------------------------
int getCpuTimeofday (struct timeval *tp, void *tz)
{
struct _timeb timebuffer;
_ftime_s (&timebuffer);
tp->tv_sec = (long)timebuffer.time;
tp->tv_usec = (long)timebuffer.millitm * 1000;
return 0;
}

//----------------------------------------------------------------------
void CpuTime::begin()
{
	if (count < offset) {
		count++;
		return;
	}
	getCpuTimeofday(&t_start,NULL);
	t1 = clock();
	t2 = (long)0.0;
	count++;
}
//----------------------------------------------------------------------
void CpuTime::end()
{
	if (count <= offset) return;

	getCpuTimeofday(&t_end, NULL);
	double tt = (t_end.tv_sec - t_start.tv_sec) +
	     (t_end.tv_usec - t_start.tv_usec) * 1.e-6;
	//printf("tt= %f\n", tt);
	t += 1000*tt;

	if (count == nbCalls) {
		print();
		reset();
	}

	//t +=  (clock() - t1) * scale;
}
//----------------------------------------------------------------------
void CpuTime::print()
{
	if (count <= 0) return;
	printf("--> %s: tot (ms): %g, avg: %g, (count=%d)\n", 
		name.c_str(), t, t/count, count);
}

void CpuTime::printToFile(FILE *out)
{
	if (count <= 0) return;
	fprintf(out,"--> %s: tot (ms): %g, avg: %g, (count=%d)\n", 
		name.c_str(), t, t/count, count);
}

//----------------------------------------------------------------------
void CpuTime::printReset()
{
	//end();
	// I would rather control end() myself
	print();
	reset();
}
//----------------------------------------------------------------------
void CpuTime::printAll()
{
#if 1
	printf("********* printAll\n");

	for (int i=0; i < (int)CpuTimeList.size(); i++) {
		CpuTime& tim = *(CpuTimeList[i]);
		tim.print();
	}
#endif
}
//----------------------------------------------------------------------
void CpuTime::printAllToFile(FILE *out)
{
#if 1
	for (int i=0; i < (int)CpuTimeList.size(); i++) {
		CpuTime& tim = *(CpuTimeList[i]);
		tim.printToFile(out);
	}
#endif
}

