#include <iostream> 
#include <fstream> 
#include <iostream> 

using namespace std;
// user trace
double user_trace[30] ={3.39000295,  4.19023632,  3.36040886,  6.4743884 ,  1.07649651, 3.18809352,  4.33270505,  5.05307169,  3.26893777, 14.86689934, 10.84986202,  1.52934174,  5.0338094 ,  5.10926823,  6.51793169, 7.99837617,  2.37411482,  4.93306196,  0.25718793,  3.91265111, 5.36237206, 10.75861334,  9.83579555,  6.23926547,  0.106591  , 8.49352796,  3.99947568,  6.78984908,  5.37966168, 10.62492493};
double BW[250]; // network trace
double Buf[30]; // thoi gian buffer cua tung video
int a; // next video dang duoc buffer
int Bseg ; // gioi han so segment dc buffer
int K; // so luong next video dc buffer
double bitrate = 2000; // 1000 kb/s
double time_step=0.001;
int Tong_video =30;
double s=0; // tong thoi gian user xem
int p=0; // So TH
void Read_BW(double BW[250])// doc file network trace
{
    ifstream Network ("trace3.txt");
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
    for (Bseg=1; Bseg<=15; Bseg++)
    {
        for (K=1; K<=30; K++)
        {
            double t=0; // thoi gian thuc
            double Tb=0; // thoi gian rebuffer
            double TB=0; // tong thoi gian rebuffer
            double W=0; // waste
            double Waste=0; // tong waste
            double Q=0; // thoi gian chay video hien tai
            double tx=0; // thoi gian xem video hien tai
            double seg=0; // segment = 1 se buffer 1s cho video
            int video =0;
            int v=0; // vi tri video dc buffer sau khi chuyen video

            for (int i=0; i<30;i++)
            {
                // s+=user_trace[i];
                Buf[i]=0;
            }
            while(t<=500)
            {
                if(tx>=user_trace[video]) // chuyen video
                {
                    Waste+= (Buf[video]-Q);
                    W= (Buf[video]-Q); 
                    if ((Buf[video]-Q) < Bseg && Buf[video] < 15)
                    {
                        W+= seg;
                        Waste+=seg;
                        seg=0;
                    }
                    // cout <<"video: "<< video<< " " <<"W: "<< W << " "<<"t: "<< t << endl; 
                    Q=0;
                    tx=0;
                    video++; 
                    // cout << "chuyen video t: "<<t<<endl;
                }
                // cout << t<<endl;
                a= 1;
                if(video == Tong_video) // user ko xem nua
                {
                    // cout << t;
                    break;
                }else if (video < Tong_video-1)  // chon video it buffer nhat de buffer
                {
                    for(int i=video+2; (i <= video+K)&&(i < Tong_video); i++)
                    {
                        if (Buf[video+a] > Buf[i])
                        {
                            a= i-video;
                        }
                    }  
                }
                if((Buf[video]-Q)<=0) // rebuffer
                {
                    while ((Buf[video]-Q)<=0)
                    {
                        seg+=time_step*(BW[(int)t]/bitrate);
                        if( seg >=1)
                        {
                            Buf[video]+=seg;
                            seg=0;
                        }
                        tx+=time_step;
                        Tb+=time_step;
                        TB+=time_step;
                        t+=time_step;
                        if(tx>=user_trace[video])
                        {
                            break;
                        }
                        // cout << "rebuffer t: "<< t << endl;
                    }
                    // cout <<"video: "<<video << " " <<"Tb: "<< Tb <<" "<<"t: "<< t << endl;
                    Tb=0;
                }else
                {
                    if(Buf[video] < 15 && video < Tong_video && (Buf[video]-Q) < Bseg) //buffer video hien tai
                    {
                        while(1)
                        {
                            // BW=5000 => sau 0.01s n?? t???i ??c 50kb => buffer cho video ??c 50/1000 = 0.05s
                            seg+=time_step*(BW[(int)t]/bitrate);
                            tx+=time_step;
                            t+=time_step;
                            if((Buf[video]-Q)>0)
                            {
                                Q+=time_step;
                            }else if ((Buf[video]-Q)<=0)
                            {
                                break;
                            }
                            if(tx>=user_trace[video])
                            {
                                break;
                            }
                            if( seg >=1)
                            {
                                Buf[video]+=seg;
                                seg=0;
                                break;
                            }   
                            // cout << "buffer video hien tai t: "<< t << endl;
                        }
                    }else if(Buf[video+a] < Bseg && video < Tong_video - 1 ) // buffer video tiep theo
                    {
                        v=0;
                        while(1)
                        {
                            // BW=5000 => sau 0.01s n?? t???i ??c 50kb => buffer cho video ??c 50/1000 = 0.05s
                            seg+=time_step*(BW[(int)t]/bitrate);
                            tx+=time_step;
                            t+=time_step;
                            if((Buf[video]-Q)>0)
                            {
                                Q+=time_step;
                            }else if ((Buf[video]-Q)<=0) // rebuffer trong luc buffer next K video
                            {
                                Tb+=time_step;
                                TB+=time_step;
                                // cout << " rebuffer trong luc buffer next video t: "<< t << endl;
                            }
                            if(tx>=user_trace[video]) // chuyen video trong luc buffer next K video
                            {
                                v++;
                                if (v>a)
                                { 
                                    break;
                                }
                                Waste+= (Buf[video]-Q);
                                W= (Buf[video]-Q); 
                                // cout <<"video: "<< video<< " " <<"W: "<< W << " "<<"t: "<< t << endl; 
                                Q=0;
                                tx=0;
                                video++; 
                                v++;
                                if(video == Tong_video) // user ko xem nua
                                {
                                    break;
                                }
                                // cout << " chuyen video trong luc buffer next K video t: "<< t << endl;
                            }
                            if( seg >=1)
                            {
                                Buf[video+a-v]+=seg;
                                seg=0;
                            
                                break;
                            }   
                            // cout << " buffer video tiep theo t: "<< t << endl;
                        }                      
                    }else // da buffer het B segment video hien tai va next K video
                    {
                        tx+=time_step;
                        t+=time_step;
                        Q+=time_step;
                        // cout << "khong buffer t: "<<t<<endl;
                    }
                }  
                // cout << "t: "<< t << endl;
            }  
                // cout << "t: "<< t << endl;
            p++;
            cout << Bseg << ","  << K << "," << Waste << "," << TB << endl;
        }
    }
}
