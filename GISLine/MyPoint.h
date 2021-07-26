#pragma once

// MyPoint 命令目标

class MyPoint : public CObject
{
public:
	MyPoint();
	virtual ~MyPoint();
public:
	double m_dLat;
	double m_dLon;
	int m_nID;
};


