void matrixtest()
{
TByteMatrix *tb1=new TByteMatrix(3,3);
TByteMatrix tb2(3,3);
TBitMatrix tbit(3,3);
TByteMatrix tbyte1(3,3);
TByteMatrix tbyte2(3,3);

//tb1.Init(3,3);
tb1->SetByte(1,1,0,4);
tb1->SetByte(1,2,1,5);
tbit.SetBit(0,0,0);
tb1->MoveMatrix(1,-1);
tb2=tb1;
cout<<"tb1"<<endl;
tb1->Print(0);
tb1->Print(1);
cout<<endl;
cout<<"tb2"<<endl;
tb2.Print(0);
tb2.Print(1);
cout<<endl;
cout<<"tbit"<<endl;
tbit.Print(0);
tbit.Print(1);
cout<<endl;
tbyte1+=*tb1;
tbyte1+=tb2;
//tbyte2=tbyte1+tb1+tb2;
cout<<"tbyte1"<<endl;
tbyte1.Print(0);
tbyte1.Print(1);
tbyte1+=tbit;
tbyte1+=tbit;
cout<<"tbyte1"<<endl;
tbyte1.Print(0);
tbyte1.Print(1);
//cout<<endl<<"tbyte2"<<endl;
//tbyte2.Print(0);
//tbyte2.Print(1);
}
