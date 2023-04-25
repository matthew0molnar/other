// *********************************************************
// File         rscpue.cpp
// Author       Matthew Molnar
// Date         4/25/2023
// *********************************************************

#include <iostream>
#include <string>
#include <fstream>
#include <cassert>
using namespace std;

#define Z 1
#define C 2
#define V 4
#define N 8

unsigned char Flag, DR, IR, TR;
unsigned short int AC, R, PC, AR;
unsigned char mem[65536];

void printAll()
{
    cout<<"Flag = " <<(unsigned int)Flag <<" DR = " <<(unsigned int)DR <<" IR = " <<(unsigned int)IR <<" TR = " <<(unsigned int)TR <<" AC = " <<(AC&255) <<" ACE = " <<AC <<" R = " <<(R&255) <<" RE = " <<R  <<" PC = " <<PC <<" AR = " <<AR <<endl;
}

void fetch()
{
    AR = PC;
    cout<<"fetch 1: AR = " <<AR <<" and PC = " <<PC <<endl;
    DR = mem[AR];
    PC++;
    cout<<"fetch 2: DR = " <<(unsigned int)DR <<" and PC = " <<PC <<endl;
    IR = DR;
    AR = PC;
    cout<<"fetch 3: IR = " <<(unsigned int)IR <<" and AR = " <<AR <<endl;
}

void noop()
{
    cout<<"Instruction execution complete: AC = " <<(AC&255) <<" ACE = "<<AC <<" R = " <<(R&255) <<" RE = "<<R <<" Flag = " <<(unsigned int)Flag <<" AR = " <<AR <<" PC = " <<PC <<" DR = " <<(unsigned int)DR<<endl;
}

void ldac()
{
    unsigned short int highOrder = AC&65280;
    AC &=255;

    DR = mem[AR];
    PC++;
    AR++;
    cout<<"ldac1: DR = " <<(unsigned int)DR <<" PC = " <<PC <<" AR = " <<AR <<endl;

    TR = DR;
    DR = mem[AR];
    PC++;
    cout<<"ldac2: TR = " <<(unsigned int)TR <<" DR = " <<(unsigned int)DR <<" AR = " <<AR <<" PC = " <<PC <<endl;

    AR = TR;
    AR = AR << 4;
    AR |= DR;
    cout<<"ldac3: AR = " <<AR <<endl;

    DR = mem[AR];
    cout<<"ldac4: DR = " <<(unsigned int)DR <<endl;

    AC = DR;
    cout<<"ldac5: AC = " <<(AC&255) <<endl;

    AC |= highOrder;

    cout<<"ldac6: ACE = " <<(signed short int)AC <<endl;

    cout<<"Instruction execution complete: AC = "<<(AC&255)<<" ACE = "<<(signed short int)AC <<" R = "<<(R&255)<<" RE = "<<R<<" Flag = "<<(unsigned int)Flag<<endl;
    cout<<" AR = "<<AR<<" PC = "<<PC<<" DR = "<<(unsigned int)DR<<endl;
}


void stac()
{
    DR = mem[AR];
    PC++;
    AR++;
    cout<<"stac1: DR = " <<(unsigned int)DR <<" PC = " <<PC <<" AR = " <<AR <<endl;

    TR = DR;
    DR = mem[AR];
    PC++;
    cout<<"stac2: TR = " <<(unsigned int)TR <<" DR = " <<(unsigned int)DR <<" PC = " <<PC <<endl;

    AR = TR;
    AR = AR << 4;
    AR |= DR;
    cout<<"stac3: AR = " <<AR <<endl;

    DR = (AC&255);
    cout<<"stac4: DR = " <<(unsigned int)DR <<endl;

    cout<<"stac5 before store location: AR  " <<AR <<" = " <<(unsigned int)mem[AR] <<endl;
    mem[AR] = DR;
    cout<<"stac5 after store location : AR " <<AR <<" = " <<(unsigned int)mem[AR] <<endl;

    cout<<"Instruction execution complete: AC = " <<(AC&255) <<" ACE = "<<(signed short int)AC <<" R = " <<(R&255) <<" RE = "<<R <<" Flag = " <<(unsigned int)Flag<<endl;
    cout<<" AR = "<<AR<<" PC = "<<PC<<" DR = "<<(unsigned int)DR<<endl;
}

void mvac()
{
    unsigned short int highOrder = R&65280;
    R = AC&255;
    R = R | highOrder;
    cout<<"mvac1 R = " <<(R&255) <<endl;
    cout<<"mvac1 RE = " <<R <<endl;
    cout<<"Instruction execution complete: AC = " <<(AC&255) <<" ACE = "<<(signed short int)AC <<" R = " <<(R&255) <<" RE = "<<R <<" Flag = " <<(unsigned int)Flag<<endl;
    cout<<" AR = "<<AR<<" PC = "<<PC<<" DR = "<<(unsigned int)DR<<endl;
}

void movr()
{
    unsigned short int highOrder = AC&65280;
    AC = R&255;
    AC = AC | highOrder;
    cout<<"movr1 AC = " <<(AC&255) <<endl;
    cout<<"mvac1 ACE = " <<(signed short int)AC <<endl;
    cout<<"Instruction execution complete: AC = " <<(AC&255) <<" ACE = "<<(signed short int)AC <<" R = " <<(R&255) <<" RE = "<<R <<" Flag = " <<(unsigned int)Flag<<endl;
    cout<<" AR = "<<AR<<" PC = "<<PC<<" DR = "<<(unsigned int)DR<<endl;
}

void jump()
{
	DR = mem[AR];
	AR++;
	cout<<"jump1 DR = "<<(unsigned int)DR<<" AR = "<<AR<<endl;

	TR = DR;
	DR = mem[AR];
	cout<<"jump2 TR = "<<(unsigned int) TR<<" DR = "<<(unsigned int) DR<<endl;
	PC = TR*16 + DR;
	cout<<"jump3 PC = "<<PC<<endl;

        cout<<"Instruction execution complete: AC = " <<(AC&255) <<" ACE = "<<(signed short int)AC <<" R = " <<(R&255) <<" RE = "<<R <<" Flag = " <<(unsigned int)Flag<<endl;
        cout<<" AR = "<<AR<<" PC = "<<PC<<" DR = "<<(unsigned int)DR<<endl;
}

void jmpz()
{
	if((Flag & Z) == Z)
	{
                DR = mem[AR];
                AR++;
                cout<<"jmpzy1 DR = "<<(unsigned int) DR<<" AR = "<<AR<<endl;


                TR = DR;
                DR = mem[AR];
                cout<<"jmpzy2 TR = "<<(unsigned int) TR<<" DR = "<<(unsigned int) DR<<endl;
                PC = TR*16 + DR;
                cout<<"jmpzy2 PC = "<<PC<<endl;
        }
        else
        {
                PC++;
                cout<<"jmpzn1 PC = "<<PC<<endl;
                PC++;
                cout<<"jmpzn2 PC = "<<PC<<endl;
        }
        cout<<"Instruction execution complete: AC = " <<(AC&255) <<" ACE = "<<(signed short int)AC <<" R = " <<(R&255) <<" RE = "<<R <<" Flag = " <<(unsigned int)Flag<<endl;
        cout<<" AR = "<<AR<<" PC = "<<PC<<" DR = "<<(unsigned int)DR<<endl;
}

void jpnz()
{
	if((Flag & Z) != Z)
	{
		DR = mem[AR];
		AR++;
		cout<<"jpnzy1 DR = "<<(unsigned int) DR<<" AR = "<<AR<<endl;


		TR = DR;
		DR = mem[AR];
		cout<<"jpnzy2 TR = "<<(unsigned int) TR<<" DR = "<<(unsigned int) DR<<endl;
		PC = TR*16 + DR;
		cout<<"jpnzy2 PC = "<<PC<<endl;
	}
	else
	{
		PC++;
		cout<<"jpnzn1 PC = "<<PC<<endl;
		PC++;
		cout<<"jpnzn2 PC = "<<PC<<endl;
	}
        cout<<"Instruction execution complete: AC = " <<(AC&255) <<" ACE = "<<(signed short int)AC <<" R = " <<(R&255) <<" RE = "<<R <<" Flag = " <<(unsigned int)Flag<<endl;
        cout<<" AR = "<<AR<<" PC = "<<PC<<" DR = "<<(unsigned int)DR<<endl;
}



void add()
{
	unsigned short int highOrder = AC&65280;
	unsigned short int oldAC = AC&255;
	AC = AC&255;

	AC = AC + R&255;

	Flag &= 0;

        if(AC == 0)
        {
                Flag |= Z;
        }

        if((oldAC&255) > AC)
        {
                Flag |= C;
        }

        if(AC > 127)
        {
                Flag |= N;
        }

	if((AC >= 128 && (oldAC&255) < 128) || (AC < 128 && (oldAC&255) >= 128) && (AC != 0))
	{
		Flag |= V;
	}

	AC = AC | highOrder;

	cout<<"add1 AC = " <<(AC&255) <<" Flag = " <<(unsigned int)Flag <<endl;
	cout<<"add1 ACE = " <<(signed short int)AC <<endl;
        cout<<"Instruction execution complete: AC = " <<(AC&255) <<" ACE = "<<(signed short int)AC <<" R = " <<(R&255) <<" RE = "<<R <<" Flag = " <<(unsigned int)Flag<<endl;
        cout<<" AR = "<<AR<<" PC = "<<PC<<" DR = "<<(unsigned int)DR<<endl;
}

void sub()
{
	unsigned short int oldAC = AC&255;
	unsigned short int highOrder = AC&65280;

	AC = AC & 255;

	AC = AC - R&255;

	Flag &= 0;

        if(AC == 0)
        {
                Flag |= Z;
        }

        if((oldAC&255) > AC)
        {
                Flag |= C;
        }

        if(AC > 127)
        {
                Flag |= N;
        }

	if(AC < 128 && (oldAC&255) >= 128 && (R&255) < 128)
	{
		Flag |= V;
	}

	AC = AC | highOrder;

	cout<<"sub1 AC = " <<(AC&255) <<" Flag = " <<(unsigned int)Flag <<endl;
	cout<<"sub1 ACE = " <<(signed short int)AC <<endl;
        cout<<"Instruction execution complete: AC = " <<(AC&255) <<" ACE = "<<(signed short int)AC <<" R = " <<(R&255) <<" RE = "<<R <<" Flag = " <<(unsigned int)Flag<<endl;
        cout<<" AR = "<<AR<<" PC = "<<PC<<" DR = "<<(unsigned int)DR<<endl;
}

void inac()
{
    unsigned short int highOrder = AC&65280;
    unsigned short int oldAC = AC&255;

    AC = AC&255;

    AC++;

    Flag &= 0;

    if(AC == 0)
    {
        Flag |= Z;
        Flag |= C;
    }

    if(AC == 128)
    {
	Flag |= V;
    }

    if(AC > 127)
    {
	Flag |= N;
    }

    AC = AC | highOrder;

    cout<<"inac1 AC = " <<(AC&255) <<" Flag = " <<(unsigned int)Flag <<endl;
    cout<<"inac1 ACE = "<<(signed short int)AC<<endl;
    cout<<"Instruction execution complete: AC = " <<(AC&255) <<" ACE = "<<(signed short int)AC <<" R = " <<(R&255) <<" RE = "<<R <<" Flag = " <<(unsigned int)Flag<<endl;
    cout<<" AR = "<<AR<<" PC = "<<PC<<" DR = "<<(unsigned int)DR<<endl;
}

void clac()
{
    unsigned short int highOrder = AC&65280;
    AC = 0;
    Flag = 1;

    AC |= highOrder;

    cout<<"clac1 AC = " <<(AC&255) <<" Flag = " <<(unsigned int)Flag <<endl;
    cout<<"clac1 ACE = " <<(signed short int)AC <<endl;
    cout<<"Instruction execution complete: AC = " <<(AC&255) <<" ACE = "<<(signed short int)AC <<" R = " <<(R&255) <<" RE = "<<R <<" Flag = " <<(unsigned int)Flag<<endl;
    cout<<" AR = "<<AR<<" PC = "<<PC<<" DR = "<<(unsigned int)DR<<endl;
}

void anD()
{
	unsigned short int oldAC = AC&65280;

	AC = AC&255;

	AC = AC & (R&255);

	Flag = Flag & 6;

	if(AC == 0)
    	{
        	Flag |= Z;
    	}

	if(AC > 127)
	{
		Flag |= N;
	}

	AC = AC | oldAC;

	cout<<"and1 AC = " <<(AC&255) <<" Flag = " <<(unsigned int)Flag <<endl;
	cout<<"and1 ACE = "<<(signed short int)AC<<endl;
        cout<<"Instruction execution complete: AC = " <<(AC&255) <<" ACE = "<<(signed short int)AC <<" R = " <<(R&255) <<" RE = "<<R <<" Flag = " <<(unsigned int)Flag<<endl;
        cout<<" AR = "<<AR<<" PC = "<<PC<<" DR = "<<(unsigned int)DR<<endl;
}

void oR()
{
	unsigned short int oldAC = AC&65280;

	AC = AC&255;

        AC = AC | (R&255);

	Flag = Flag & 6;

        if(AC == 0)
        {
                Flag |= Z;
        }

        if(AC > 127)
        {
                Flag |= N;
        }

	AC = AC | oldAC;

	cout<<"or1 AC = " <<(AC&255) <<" Flag = " <<(unsigned int)Flag <<endl;
	cout<<"or1 ACE = " <<(signed short int)AC<<endl;
        cout<<"Instruction execution complete: AC = " <<(AC&255) <<" ACE = "<<(signed short int)AC <<" R = " <<(R&255) <<" RE = "<<R <<" Flag = " <<(unsigned int)Flag<<endl;
        cout<<" AR = "<<AR<<" PC = "<<PC<<" DR = "<<(unsigned int)DR<<endl;
}

void xoR()
{
	unsigned short int oldAC = AC&65280;

	AC = AC&255;

        AC = AC ^ (R&255);

	Flag = Flag & 6;

        if(AC == 0)
        {
                Flag |= Z;
        }

        if(AC > 127)
        {
                Flag |= N;
        }

	AC = AC | oldAC;

	cout<<"xor1 AC = " <<(AC&255) <<" Flag = " <<(unsigned int)Flag <<endl;
	cout<<"xor1 ACE = "<<(signed short int)AC<<endl;
        cout<<"Instruction execution complete: AC = " <<(AC&255) <<" ACE = "<<(signed short int)AC <<" R = " <<(R&255) <<" RE = "<<R <<" Flag = " <<(unsigned int)Flag<<endl;
        cout<<" AR = "<<AR<<" PC = "<<PC<<" DR = "<<(unsigned int)DR<<endl;
}

void noT()
{
	unsigned short int oldAC = AC&65280;

	AC = ~AC;
	AC = AC&255;

	Flag = Flag & 6;

        if(AC == 0)
        {
                Flag |= Z;
        }

        if(AC > 127)
        {
                Flag |= N;
        }

	AC = AC | oldAC;

	cout<<"not1 AC = " <<(AC&255) <<" Flag = " <<(unsigned int)Flag <<endl;
	cout<<"not1 ACE = "<<(signed short int)AC<<endl;
        cout<<"Instruction execution complete: AC = " <<(AC&255) <<" ACE = "<<(signed short int)AC <<" R = " <<(R&255) <<" RE = "<<R <<" Flag = " <<(unsigned int)Flag<<endl;
        cout<<" AR = "<<AR<<" PC = "<<PC<<" DR = "<<(unsigned int)DR<<endl;
}

void jmpc()
{
        if((Flag & C) == C)
        {
                DR = mem[AR];
                AR++;
                cout<<"jmpcy1 DR = "<<(unsigned int) DR<<" AR = "<<AR<<endl;


                TR = DR;
                DR = mem[AR];
                cout<<"jmpcy2 TR = "<<(unsigned int) TR<<" DR = "<<(unsigned int) DR<<endl;
                PC = TR*16 + DR;
                cout<<"jmpcy2 PC = "<<PC<<endl;
        }
        else
        {
                PC++;
                cout<<"jmpcn1 PC = "<<PC<<endl;
                PC++;
                cout<<"jmpcn2 PC = "<<PC<<endl;
        }
        cout<<"Instruction execution complete: AC = " <<(AC&255) <<" ACE = "<<(signed short int)AC <<" R = " <<(R&255) <<" RE = "<<R <<" Flag = " <<(unsigned int)Flag<<endl;
        cout<<" AR = "<<AR<<" PC = "<<PC<<" DR = "<<(unsigned int)DR<<endl;
}

void jv()
{
        if((Flag & V) == V)
        {
                DR = mem[AR];
                AR++;
                cout<<"jvy1 DR = "<<(unsigned int) DR<<" AR = "<<AR<<endl;


                TR = DR;
                DR = mem[AR];
                cout<<"jvy2 TR = "<<(unsigned int) TR<<" DR = "<<(unsigned int) DR<<endl;
                PC = TR*16 + DR;
                cout<<"jvy2 PC = "<<PC<<endl;
        }
        else
        {
                PC++;
                cout<<"jvn1 PC = "<<PC<<endl;
                PC++;
                cout<<"jvn2 PC = "<<PC<<endl;
        }
        cout<<"Instruction execution complete: AC = " <<(AC&255) <<" ACE = "<<(signed short int)AC <<" R = " <<(R&255) <<" RE = "<<R <<" Flag = " <<(unsigned int)Flag<<endl;
        cout<<" AR = "<<AR<<" PC = "<<PC<<" DR = "<<(unsigned int)DR<<endl;
}

void rl()
{
	unsigned short int oldAC = AC&255;
	unsigned short int highOrder = AC&65280;

	AC = AC&255;

	int leftMost = AC & 128;
	leftMost = leftMost >> 7;

	AC = AC << 1;
	AC = AC | leftMost;
	AC = AC&255;

	Flag &= 0;

	if(AC == 0)
	{
		Flag |= Z;
	}

	if(oldAC > 127)
	{
		Flag |= C;
	}

	if(AC > 127)
	{
		Flag |= N;

		if(oldAC <= 127)
		{
			Flag |= V;
		}
	}
	else if(oldAC > 127)
	{
		Flag |= V;
	}

	AC = AC | highOrder;

	cout<<"rl1 AC = " <<(AC&255) <<" Flag = " <<(unsigned int)Flag <<endl;
	cout<<"rl1 ACE = "<<(signed short int)AC<<endl;
        cout<<"Instruction execution complete: AC = " <<(AC&255) <<" ACE = "<<(signed short int)AC <<" R = " <<(R&255) <<" RE = "<<R <<" Flag = " <<(unsigned int)Flag<<endl;
        cout<<" AR = "<<AR<<" PC = "<<PC<<" DR = "<<(unsigned int)DR<<endl;
}

void rr()
{
	unsigned short int oldAC = AC&255;
	unsigned short int highOrder = AC&65280;

	AC = AC&255;

	int rightMost = AC & 1;
	rightMost = rightMost << 7;


	AC = AC >> 1;
	AC = AC | rightMost;
	AC = AC&255;

	Flag &= 0;

        if(AC == 0)
        {
                Flag |= Z;
        }

        if(oldAC % 2 == 1)
        {
                Flag |= C;
        }

        if(AC > 127)
        {
                Flag |= N;

                if(oldAC <= 127)
                {
                        Flag |= V;
                }
        }
        else if(oldAC > 127)
        {
                Flag |= V;
        }

	AC = AC | highOrder;

	cout<<"rr1 AC = " <<(AC&255) <<" Flag = " <<(unsigned int)Flag <<endl;
	cout<<"rr1 ACE = "<<(signed short int)AC<<endl;
        cout<<"Instruction execution complete: AC = " <<(AC&255) <<" ACE = "<<(signed short int)AC <<" R = " <<(R&255) <<" RE = "<<R <<" Flag = " <<(unsigned int)Flag<<endl;
        cout<<" AR = "<<AR<<" PC = "<<PC<<" DR = "<<(unsigned int)DR<<endl;
}

void lsl()
{
	unsigned short int oldAC = AC&255;
	unsigned short int highOrder = AC&65280;

	AC = AC & 255;

	AC = AC << 1;
	AC = AC&255;

	Flag &= 0;

        if(AC == 0)
        {
                Flag |= Z;
        }

        if(oldAC > 127)
        {
                Flag |= C;
        }

        if(AC > 127)
        {
                Flag |= N;

                if(oldAC <= 127)
                {
                        Flag |= V;
                }
        }
        else if(oldAC > 127)
        {
                Flag |= V;
        }

	AC = AC | highOrder;

	cout<<"asl1 AC = " <<(AC&255) <<" Flag = " <<(unsigned int)Flag <<endl;
	cout<<"asl1 ACE = "<<(signed short int)AC<<endl;
        cout<<"Instruction execution complete: AC = " <<(AC&255) <<" ACE = "<<(signed short int)AC <<" R = " <<(R&255) <<" RE = "<<R <<" Flag = " <<(unsigned int)Flag<<endl;
        cout<<" AR = "<<AR<<" PC = "<<PC<<" DR = "<<(unsigned int)DR<<endl;
}

void lsr()
{
	unsigned short int oldAC = AC&255;
	unsigned short int highOrder = AC&65280;

	AC = AC & 255;

	AC = AC >> 1;
	AC = AC&255;

	Flag &= 0;

        if(AC == 0)
        {
                Flag |= Z;
        }

        if(oldAC % 2 == 1)
        {
                Flag |= C;
        }

        if(AC > 127)
        {
                Flag |= N;

                if(oldAC <= 127)
                {
                        Flag |= V;
                }
        }
        else if(oldAC > 127)
        {
                Flag |= V;
        }

	AC = AC | highOrder;

	cout<<"asr1 AC = " <<(AC&255) <<" Flag = " <<(unsigned int)Flag <<endl;
	cout<<"asr1 ACE = " <<(signed short int)AC <<endl;
        cout<<"Instruction execution complete: AC = " <<(AC&255) <<" ACE = "<<(signed short int)AC <<" R = " <<(R&255) <<" RE = "<<R <<" Flag = " <<(unsigned int)Flag<<endl;
        cout<<" AR = "<<AR<<" PC = "<<PC<<" DR = "<<(unsigned int)DR<<endl;
}

void jn()
{
        if((Flag & N) == N)
        {
                DR = mem[AR];
                AR++;
                cout<<"jny1 DR = "<<(unsigned int) DR<<" AR = "<<AR<<endl;


                TR = DR;
                DR = mem[AR];
                cout<<"jny2 TR = "<<(unsigned int) TR<<" DR = "<<(unsigned int) DR<<endl;
                PC = TR*16 + DR;
                cout<<"jny2 PC = "<<PC<<endl;
        }
        else
        {
                PC++;
                cout<<"jnn1 PC = "<<PC<<endl;
                PC++;
                cout<<"jnn2 PC = "<<PC<<endl;
        }
        cout<<"Instruction execution complete: AC = " <<(AC&255) <<" ACE = "<<(signed short int)AC <<" R = " <<(R&255) <<" RE = "<<R <<" Flag = " <<(unsigned int)Flag<<endl;
        cout<<" AR = "<<AR<<" PC = "<<PC<<" DR = "<<(unsigned int)DR<<endl;
}

void mvi()
{
    unsigned short int highOrder = AC&65280;
    AC = AC&255;

    DR = mem[AR];
    PC++;
    AR++;
    cout<<"mvi1 DR = " <<(unsigned int)DR <<" PC = " <<PC <<" AR = " <<(unsigned int)AR <<endl;
    AC = DR;

    AC |= highOrder;

    cout<<"mvi2 AC = " <<(AC&255) <<endl;
    cout<<"mvi2 ACE = "<<(signed short int)AC<<endl;
    cout<<"Instruction execution complete: AC = " <<(AC&255) <<" ACE = "<<(signed short int)AC <<" R = " <<(R&255) <<" RE = "<<R <<" Flag = " <<(unsigned int)Flag<<endl;
    cout<<" AR = "<<AR<<" PC = "<<PC<<" DR = "<<(unsigned int)DR<<endl;
}

void halt()
{
    cout<<"Instruction execution complete: AC = " <<(AC&255) <<" ACE = "<<(signed short int)AC <<" R = " <<(R&255) <<" RE = "<<R <<" Flag = " <<(unsigned int)Flag<<endl;
    cout<<" AR = "<<AR<<" PC = "<<PC<<" DR = "<<(unsigned int)DR<<endl;
    exit(0);
}

void Eldac()
{
	DR = mem[AR];
	PC++;
	AR++;
	cout<<"ldac1e: DR = "<<(unsigned int)DR<<" PC = "<<PC<<" AR = "<<AR<<endl;

	TR = DR;
	DR = mem[AR];
	PC++;
	cout<<"ldac2e: TR = "<<(unsigned int)TR<<" DR = "<<(unsigned int)DR<<" AR= "<<AR<<" PC = "<<PC<<endl;

	AR = TR;
	AR = AR << 4;
	AR |= DR;
	cout<<"ldac3e: AR = "<<AR<<endl;

	DR = mem[AR];
	cout<<"ldac4e: DR = "<<(unsigned int)DR<<endl;

	AC = DR*256;
	cout<<"ldac5e: High Order ACE = "<<(AC&65280)<<endl;

	AR++;
	DR = mem[AR];
	cout<<"ldac6e: DR = "<<(unsigned int)DR<<" AR = "<<AR<<endl;

	AC |= DR;
	cout<<"ldac7e: ACE = "<<(signed short int)AC<<endl;
	cout<<"ldac7e: AC = "<<(AC&255)<<endl;
        cout<<"Instruction execution complete: AC = " <<(AC&255) <<" ACE = "<<(signed short int)AC <<" R = " <<(R&255) <<" RE = "<<R <<" Flag = " <<(unsigned int)Flag<<endl;
        cout<<" AR = "<<AR<<" PC = "<<PC<<" DR = "<<(unsigned int)DR<<endl;
}

void Estac()
{
	DR = mem[AR];
	PC++;
	AR++;
	cout<<"stac1e: DR = "<<(unsigned int)DR <<" PC = "<<PC<<" AR = "<<AR<<endl;

	TR = DR;
	DR = mem[AR];
	PC++;
	cout<<"stac2e: TR = "<<(unsigned int)TR<<" DR = "<<(unsigned int)DR<<" PC = "<<PC<<endl;

	AR = TR;
	AR = AR << 4;
	AR |= DR;
	cout<<"stac3e: AR = "<<AR<<endl;

	DR = AC * 256;
	cout<<"stac4e: DR = "<<(unsigned int)DR<<endl;

	cout<<"stac5e before store location: AR "<<AR<<" = "<<(unsigned int)mem[AR]<<endl;
	mem[AR] = DR;
	cout<<"stac5e after store location: AR "<<AR<<" = "<<(unsigned int)mem[AR]<<endl;

	AR++;
	cout<<"stac6e AR = "<<AR<<endl;

	DR = AC & 255;
	cout<<"stac7e before store location: AR "<<AR<<" = "<<(unsigned int)mem[AR]<<" and DR = "<<(unsigned int)DR<<endl;
	mem[AR] = DR;
	cout<<"stac7e after store location: AR "<<AR<<" = "<<(unsigned int)mem[AR]<<endl;

        cout<<"Instruction execution complete: AC = " <<(AC&255) <<" ACE = "<<(signed short int)AC <<" R = " <<(R&255) <<" RE = "<<R <<" Flag = " <<(unsigned int)Flag<<endl;
        cout<<" AR = "<<AR<<" PC = "<<PC<<" DR = "<<(unsigned int)DR<<endl;
}
void Emvac()
{
	R = AC;
	cout<<"mvac1e R  ="<<(R&255)<<endl;
	cout<<"mvac1e RE = "<<R<<endl;
	cout<<"Instruction execution complete: AC = " <<(AC&255) <<" ACE = "<<(signed short int)AC <<" R = " <<(R&255) <<" RE = "<<R <<" Flag = " <<(unsigned int)Flag<<endl;
        cout<<" AR = "<<AR<<" PC = "<<PC<<" DR = "<<(unsigned int)DR<<endl;
}
void Emovr()
{
	AC = R;
	cout<<"movr1e ACE = "<<(signed short int)AC<<endl;
	cout<<"movr1e AC = "<<(AC&255)<<endl;
        cout<<"Instruction execution complete: AC = " <<(AC&255) <<" ACE = "<<(signed short int)AC <<" R = " <<(R&255) <<" RE = "<<R <<" Flag = " <<(unsigned int)Flag<<endl;
        cout<<" AR = "<<AR<<" PC = "<<PC<<" DR = "<<(unsigned int)DR<<endl;
}

void Eadd()
{
	unsigned short int oldAC = AC;

	AC = AC + R;
	Flag &= 0;

	if(AC == 0)
	{
		Flag |= Z;
	}

	if(oldAC > AC)
	{
		Flag |= C;
	}

	if(AC > 32767)
	{
		Flag |= N;
	}

	if((AC >= 32768 && oldAC < 32768) || (AC < 32768 && oldAC >= 32768) && (AC != 0))
	{
		Flag |= V;
	}

	cout<<"add1e AC = "<<(AC&255)<<" Flag = "<<(unsigned int)Flag <<endl;
	cout<<"add1e ACE = "<<(signed short int)AC<<endl;
        cout<<"Instruction execution complete: AC = " <<(AC&255) <<" ACE = "<<(signed short int)AC <<" R = " <<(R&255) <<" RE = "<<R <<" Flag = " <<(unsigned int)Flag<<endl;
        cout<<" AR = "<<AR<<" PC = "<<PC<<" DR = "<<(unsigned int)DR<<endl;
}

void Esub()
{
	unsigned short int oldAC = AC;

	AC = AC - R;

	Flag &= 0;

	if(AC == 0)
	{
		Flag |= Z;
	}

	if(oldAC > AC)
	{
		Flag |= C;
	}

	if(AC > 32767)
	{
		Flag |= N;
	}

	if(AC < 32768 && oldAC >= 32768 && R < 32768)
	{
		Flag |= V;
	}

	cout<<"sub1e AC = "<<(AC&255)<<" Flag = "<<(unsigned int)Flag<<endl;
	cout<<"sub1e ACE = "<<(signed short int)AC<<endl;
        cout<<"Instruction execution complete: AC = " <<(AC&255) <<" ACE = "<<(signed short int)AC <<" R = " <<(R&255) <<" RE = "<<R <<" Flag = " <<(unsigned int)Flag<<endl;
        cout<<" AR = "<<AR<<" PC = "<<PC<<" DR = "<<(unsigned int)DR<<endl;
}
void Einac()
{
	unsigned short int oldAC = AC;
	AC++;

	Flag &= 0;

	if(AC == 0)
	{
		Flag |= Z;
		Flag |= C;
	}

	if(AC == 32768)
	{
		Flag |= V;
	}

	if(AC > 32767)
	{
		Flag |= N;
	}

	cout<<"inac1e AC = "<<(AC&255)<<" Flag = "<<(unsigned int)Flag<<endl;
	cout<<"inac1e ACE = "<<(signed short int)AC<<endl;
        cout<<"Instruction execution complete: AC = " <<(AC&255) <<" ACE = "<<(signed short int)AC <<" R = " <<(R&255) <<" RE = "<<R <<" Flag = " <<(unsigned int)Flag<<endl;
        cout<<" AR = "<<AR<<" PC = "<<PC<<" DR = "<<(unsigned int)DR<<endl;
}

void Eclac()
{
	AC = 0;
	Flag = 1;
	cout<<"clac1e AC = "<<(AC&255)<<" Flag = "<<(unsigned int)Flag<<endl;
	cout<<"clac1e ACE = "<<(signed short int)AC<<endl;
        cout<<"Instruction execution complete: AC = " <<(AC&255) <<" ACE = "<<(signed short int)AC <<" R = " <<(R&255) <<" RE = "<<R <<" Flag = " <<(unsigned int)Flag<<endl;
        cout<<" AR = "<<AR<<" PC = "<<PC<<" DR = "<<(unsigned int)DR<<endl;
}

void Eand()
{
	AC = AC & R;
	Flag = Flag & 6;

	if(AC == 0)
	{
		Flag |= Z;
	}

	if(AC > 32767)
	{
		Flag |= N;
	}

	cout<<"and1e AC = "<<(AC&255)<<" Flag = "<<(unsigned int)Flag<<endl;
	cout<<"and1e ACE = "<<(signed short int)AC<<endl;
        cout<<"Instruction execution complete: AC = " <<(AC&255) <<" ACE = "<<(signed short int)AC <<" R = " <<(R&255) <<" RE = "<<R <<" Flag = " <<(unsigned int)Flag<<endl;
        cout<<" AR = "<<AR<<" PC = "<<PC<<" DR = "<<(unsigned int)DR<<endl;
}

void Eor()
{
	AC = AC | R;
	Flag = Flag & 6;

	if(AC == 0)
	{
		Flag |= Z;
	}

	if(AC > 32767)
	{
		Flag |= N;
	}

	cout<<"or1e AC = "<<(AC&255)<<" Flag = "<<(unsigned int)Flag <<endl;
	cout<<"or1e ACE = "<<(signed short int)AC<<endl;
        cout<<"Instruction execution complete: AC = " <<(AC&255) <<" ACE = "<<(signed short int)AC <<" R = " <<(R&255) <<" RE = "<<R <<" Flag = " <<(unsigned int)Flag<<endl;
        cout<<" AR = "<<AR<<" PC = "<<PC<<" DR = "<<(unsigned int)DR<<endl;
}

void Exor()
{
	unsigned short int oldAC = AC;

	AC = AC ^ R;

	Flag = Flag & 6;

	if(AC == 0)
	{
		Flag |= Z;
	}

	if(AC > 32767)
	{
		Flag |= N;
	}
	cout<<"xor1e AC = " <<(AC&255) <<" Flag = " <<(unsigned int)Flag <<endl;
	cout<<"xor1e ACE = "<<(signed short int)AC<<endl;
        cout<<"Instruction execution complete: AC = " <<(AC&255) <<" ACE = "<<(signed short int)AC <<" R = " <<(R&255) <<" RE = "<<R <<" Flag = " <<(unsigned int)Flag<<endl;
        cout<<" AR = "<<AR<<" PC = "<<PC<<" DR = "<<(unsigned int)DR<<endl;
}

void Enot()
{
	AC = ~AC;
	Flag = Flag & 6;

	if(AC == 0)
	{
		Flag |= Z;
	}

	if(AC > 32767)
	{
		Flag |= N;
	}

	cout<<"not1e AC = " <<(AC&255) <<" Flag = " <<(unsigned int)Flag <<endl;
        cout<<"not1e ACE = "<<(signed short int)AC<<endl;
        cout<<"Instruction execution complete: AC = " <<(AC&255) <<" ACE = "<<(signed short int)AC <<" R = " <<(R&255) <<" RE = "<<R <<" Flag = " <<(unsigned int)Flag<<endl;
        cout<<" AR = "<<AR<<" PC = "<<PC<<" DR = "<<(unsigned int)DR<<endl;
}
void Erl()
{
	unsigned short int oldAC = AC;

	int leftMost = AC & 32768;
	leftMost = leftMost >> 15;

	AC = AC << 1;
	AC = AC | leftMost;

	Flag &= 0;

	if(AC == 0)
	{
		Flag |= Z;
	}

	if(oldAC > 32767)
	{
		Flag |= C;
	}

	if(AC > 32767)
	{
		Flag |= N;
		if(oldAC <= 32767)
		{
			Flag |= V;
		}
	}
	else if(oldAC > 32767)
	{
		Flag |= V;
	}

	cout<<"rl1e AC = "<<(AC&255)<<" Flag = "<<(unsigned int)Flag<<endl;
	cout<<"rl1e ACE = "<<(signed short int)AC<<endl;
        cout<<"Instruction execution complete: AC = " <<(AC&255) <<" ACE = "<<(signed short int)AC <<" R = " <<(R&255) <<" RE = "<<R <<" Flag = " <<(unsigned int)Flag<<endl;
        cout<<" AR = "<<AR<<" PC = "<<PC<<" DR = "<<(unsigned int)DR<<endl;
}

void Err()
{
	unsigned short int oldAC = AC;

	int rightMost = AC & 1;
	rightMost = rightMost << 15;

	AC = AC >> 1;
	AC = AC | rightMost;

	Flag &= 0;

	if(AC == 0)
	{
		Flag |= Z;
	}

	if(oldAC % 2 == 1)
	{
		Flag |= C;
	}

	if(AC > 32767)
	{
		Flag |= N;
		if(oldAC <= 32767)
		{
			Flag |= V;
		}
	}
	else if(oldAC > 32767)
	{
		Flag |= V;
	}

        cout<<"rr1e AC = " <<(AC&255) <<" Flag = " <<(unsigned int)Flag <<endl;
        cout<<"rr1e ACE = "<<(signed short int)AC<<endl;
        cout<<"Instruction execution complete: AC = " <<(AC&255) <<" ACE = "<<(signed short int)AC <<" R = " <<(R&255) <<" RE = "<<R <<" Flag = " <<(unsigned int)Flag<<endl;
        cout<<" AR = "<<AR<<" PC = "<<PC<<" DR = "<<(unsigned int)DR<<endl;
}

void Elsl()
{
	unsigned short int oldAC = AC;
	AC = AC << 1;

	Flag &= 0;

	if(AC == 0)
	{
		Flag |= Z;
	}

	if(oldAC > 32767)
	{
		Flag |= C;
	}

	if(AC > 32767)
	{
		Flag |= N;
		if(oldAC <= 32767)
		{
			Flag |= V;
		}
	}
	else if(oldAC > 32767)
	{
		Flag |= V;
	}

	cout<<"asl1e AC = "<<(AC&255)<<" Flag = "<<(unsigned int)Flag <<endl;
	cout<<"asl1e ACE = "<<(signed short int)AC<<endl;
	cout<<"Instruction execution complete: AC = " <<(AC&255) <<" ACE = "<<(signed short int)AC <<" R = " <<(R&255) <<" RE = "<<R <<" Flag = " <<(unsigned int)Flag<<endl;
        cout<<" AR = "<<AR<<" PC = "<<PC<<" DR = "<<(unsigned int)DR<<endl;
}
void Elsr()
{
	unsigned short int oldAC = AC;
        AC = AC >> 1;

        Flag &= 0;

        if(AC == 0)
        {
                Flag |= Z;
        }

        if(oldAC % 2 == 1)
        {
                Flag |= C;
        }

        if(AC > 32767)
        {
                Flag |= N;
                if(oldAC <= 32767)
                {
                        Flag |= V;
                }
        }
        else if(oldAC > 32767)
        {
                Flag |= V;
        }

        cout<<"asr1e AC = "<<(AC&255)<<" Flag = "<<(unsigned int)Flag <<endl;
        cout<<"asr1e ACE = "<<(signed short int)AC<<endl;
        cout<<"Instruction execution complete: AC = " <<(AC&255) <<" ACE = "<<(signed short int)AC <<" R = " <<(R&255) <<" RE = "<<R <<" Flag = " <<(unsigned int)Flag<<endl;
        cout<<" AR = "<<AR<<" PC = "<<PC<<" DR = "<<(unsigned int)DR<<endl;
}

void Emvi()
{
	DR = mem[AR];
	PC++;
	AR++;
	cout<<"mvi1e DR = "<<(unsigned int)DR<<" PC = "<<PC<<" AR = "<<(unsigned int)AR<<endl;

	AC = DR*256;
	cout<<"mvi2e AC = "<<(AC&255)<<endl;
	cout<<"mvi2e ACE = "<<(signed short int)AC<<endl;


	DR = mem[AR];
	PC++;
	AR++;
	cout<<"mvi3e DR = "<<(unsigned int)DR<<" PC = "<<PC<<" AR = "<<AR<<endl;

	AC |= DR;
	cout<<"mvi4e ACE = "<<(signed short int)AC<<endl;
	cout<<"mvi4e AC = "<<(AC&255)<<endl;

	cout<<"Instruction execution complete: AC = " <<(AC&255) <<" ACE = "<<(signed short int)AC <<" R = " <<(R&255) <<" RE = "<<R <<" Flag = " <<(unsigned int)Flag<<endl;
	cout<<" AR = "<<AR<<" PC = "<<PC<<" DR = "<<(unsigned int)DR<<endl;
}

int main()
{
    ifstream fin;
    string fileName;

    cout<<"rscpu written by Matthew Molnar\n" <<endl;
    printAll();
    cout<<"Enter the name of the file containing the program" <<endl;
    cin>>fileName;
    cout<<"File " <<fileName <<" inputed" <<endl;

    // Checking if able to open file
    fin.open(fileName.c_str());
    assert(fin.is_open());
    cout<<"File opened" <<endl;

    // Reading instructions into mem array, converting from hex string to int
    int memIndex = 0;
    string instructString;
    int instructInt;
    while(!fin.eof())
    {
        fin>>instructString;
        instructInt = stoul(instructString, NULL, 16);
        mem[memIndex] = (unsigned char)instructInt;
        memIndex++;
    }

    cout<<"End of file reached" <<endl;
    PC = 0;

    // Calling function of instruction
    while(true)
    {
        fetch();
        switch((unsigned int)IR)
        {
            case 0:
                cout<<"nop instruction" <<endl;
                noop();
                break;

	    case 1:
		cout<<"ldac instruction" <<endl;
		ldac();
		break;

	    case 2:
		cout<<"stac instruction" <<endl;
		stac();
		break;

	    case 3:
		cout<<"mvac instruction" <<endl;
		mvac();
		break;

	    case 4:
		cout<<"movr instruction" <<endl;
		movr();
		break;

	    case 5:
		cout<<"jump instruction" <<endl;
		jump();
		break;

	    case 6:
		cout<<"jmpz instruction" <<endl;
		jmpz();
		break;

	    case 7:
		cout<<"jpnz instruction" <<endl;
		jpnz();
		break;

	    case 8:
		cout<<"add instruction" <<endl;
		add();
		break;

	    case 9:
		cout<<"sub instruction" <<endl;
		sub();
		break;

	    case 10:
		cout<<"inac instruction" <<endl;
		inac();
		break;

	    case 11:
		cout<<"clac instruction" <<endl;
		clac();
		break;

	    case 12:
		cout<<"and instruction" <<endl;
		anD();
		break;

	    case 13:
		cout<<"or instruction" <<endl;
		oR();
		break;

	    case 14:
		cout<<"xor instruction" <<endl;
		xoR();
		break;

	    case 15:
		cout<<"not instruction" <<endl;
		noT();
		break;

	    case 16:
		cout<<"jmpc instruction" <<endl;
		jmpc();
		break;

	    case 17:
		cout<<"jv instruction" <<endl;
		jv();
		break;

	    case 18:
		cout<<"rl instruction" <<endl;
		rl();
		break;

	    case 19:
		cout<<"rr instruction" <<endl;
		rr();
		break;

	    case 20:
		cout<<"lsl instruction" <<endl;
		lsl();
		break;

	    case 21:
		cout<<"lsr instruction" <<endl;
		lsr();
		break;

	    case 22:
		cout<<"mvi instruction" <<endl;
		mvi();
		break;

	    case 23:
		cout<<"jn instruction" <<endl;
		jn();
		break;

	    case 129:
		cout<<"ldace instruction" <<endl;
		Eldac();
		break;

	    case 130:
		cout<<"stace instruction" <<endl;
		Estac();
		break;

	    case 131:
		cout<<"mvace instruction" <<endl;
		Emvac();
		break;

	    case 132:
		cout<<"movre instruction" <<endl;
		Emovr();
		break;

	    case 136:
		cout<<"adde instruction" <<endl;
		Eadd();
		break;

	    case 137:
		cout<<"sube instruction" <<endl;
		Esub();
		break;

	    case 138:
		cout<<"inace instruction" <<endl;
		Einac();
		break;

	    case 139:
		cout<<"clace instruction" <<endl;
		Eclac();
		break;

	    case 140:
		cout<<"ande instruction" <<endl;
		Eand();
		break;

	    case 141:
		cout<<"ore instruction" <<endl;
		Eor();
		break;

	    case 142:
		cout<<"xore instruction" <<endl;
		Exor();
		break;

	    case 143:
		cout<<"note instruction" <<endl;
		Enot();
		break;

	    case 146:
		cout<<"rle instruction" <<endl;
		Erl();
		break;

	    case 147:
		cout<<"rre instruction" <<endl;
		Err();
		break;

	    case 148:
		cout<<"lsle instruction" <<endl;
		Elsl();
		break;

	    case 149:
		cout<<"lsre instruction" <<endl;
		Elsr();
		break;

	    case 150:
		cout<<"mvie instruction" <<endl;
		Emvi();
		break;

            case 255:
                cout<<"halt instruction" <<endl;
                halt();
                break;

            default:
                cout<<"invalid instruction" <<endl;
                break;
        }
    }
}
