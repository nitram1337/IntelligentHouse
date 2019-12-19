/*
 * keypad.h
 *
 * Created: 13-12-2019 12:28:02
 *  Author: Martin Sonne
 */ 

	//	Columns:	1(PK3)	2(PK2)	3(PK1)	4(PK0)
	//	Row1 (PK4)	   1	   2	   3	  A
	//	Row2 (PK5)	   4	   5	   6	  B
	//	Row3 (PK6)	   7	   8	   9	  C
	//	Row4 (PK7)	   *	   0	   #	  D


#ifndef KEYPAD_H_
#define KEYPAD_H_

#pragma region ColumnPorts
	#define Col1 PK3
	#define Col2 PK2
	#define Col3 PK1
	#define Col4 PK0
#pragma endregion ColumnPorts

#pragma region RowPorts
	#define Row1 PK4
	#define Row2 PK5
	#define Row3 PK6
	#define Row4 PK7
#pragma endregion RowPorts

#pragma region FunctionPrototyping
	void INIT_KEYPAD();
	int ColumnScan();
	int ReadRows();
	void DecodeKeyboard(int row, int column);
#pragma endregion FunctionPrototyping

#endif /* KEYPAD_H_ */