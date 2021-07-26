#pragma once

// MyLine 命令目标
#include<List>

class MyLine : public CObject
{
public:
	MyLine();
	virtual ~MyLine();
public:
	std::list<int> m_lIdOrder;
	double m_dPoint[3000][2];
};


