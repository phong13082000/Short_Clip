#include <iostream> 
#include <fstream> 
#include <iostream> 

using namespace std;
// user trace
double user_trace[30] ={3.72687272, 10.97615345,  3.73311376,  7.93133749,  5.46250672, 5.03175729,  4.2033281 ,  0.23471722,  3.13193818,  4.74024695, 3.63160728,  4.61824191,  3.49232399,  6.1516166 ,  5.94345566, 10.91569073,  1.71582105,  5.78847004,  6.85737497,  5.37536954, 4.60972807,  8.24144345,  6.87478599, 10.73155018,  9.92433724, 4.80577454,  8.29860233,  6.84510747,  5.85787661,  3.03570681};
double BW[250]; // network trace
double t=0; // thoi gian thuc
double Tb=0; // thoi gian rebuffer
double TB=0; // tong thoi gian rebuffer
double W=0; // waste
double Waste=0; // tong waste
double Q=0; // thoi gian chay video hien tai
double tx=0; // thoi gian xem video hien tai
double B0=0; // thoi gian video da dc buffer video hien tai 
double B1=0; // thoi gian video da dc buffer video tiep theo
double B2=0; // thoi gian video da dc buffer video thu 3
double seg=0; // segment = 1 se buffer 1s cho video
double bitrate = 1000; 
double time_step=0.001;
int video =0;
int Tong_video =30;
double Buf[20];
double s=0; // tong thoi gian user xem
// doc file network trace
void Read_BW(double BW[200])
{
    ifstream Network ("trace1.txt");
    if(! Network.is_open())
    {
        cout <<"Khong the mo file.\n";
    
    }
    else 
    {
        for(int i = 0; i < 250; i++)
        {
            Network>>BW[i];
        }
    }
    Network.close();
    system("pause");
}
int main()
{
    Read_BW(BW);
    for(int i =0; i < 21; i++)
    {
        cout<<Buf[i]<<endl;
    }
}