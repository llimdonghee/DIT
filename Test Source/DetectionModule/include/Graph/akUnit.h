#pragma once


// enum khUnitType
// {
// 	Distance,
// 	Velocity,
// 	Temp,
// 	Weight
// };

enum akUnit
{
	km,m,kyd,ft
};

class CakUnit
{
public:
	CakUnit(void);
	virtual ~CakUnit(void);


public:
	//void SetUnitType(khUnitType type);
	void SetDefaultUnit(int index, akUnit unit);
	bool SetChangeUnit(int index, akUnit unit);

public:
	char* GetCurrentUnitString(int index);
	char* GetUnitString(int index, akUnit unit);

protected:
	akUnit m_DefaultUnit[10];//기준 단위
	akUnit m_ChangeUnit[10];//변경되는 단위

};
