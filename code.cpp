#include <iostream> 
#include <fstream> 
#include <iostream> 

using namespace std;
// user trace
double user_trace[30] ={15.57795671, 19.10070052, 20.87108367, 27.81344504, 20.48538818, 12.23868246,  9.60973767,  7.27891733, 36.44802634, 29.99245136, 26.62980921,  0.82352023, 16.19487166, 16.27068487, 30.95755438, 24.70715178, 20.26623842, 27.74044977, 18.17260481, 23.2370618 , 30.50835249,  7.55502456, 16.89573937,  5.15584425, 18.04638475, 12.15315521, 29.22638936, 28.8735984 , 24.58742808, 41.41185648};
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
double Buf[30]; // thoi gian buffer cua tung video
int a;
int v=0;
int Bseg =3; // gioi han so segment dc buffer
int K=4; // so luong next video dc buffer
double seg=0; // segment = 1 se buffer 1s cho video
double bitrate = 2000; // 1000 kb/s
double time_step=0.001;
int video =0;
int Tong_video =30;
double s=0; // tong thoi gian user xem
// doc file network trace
void Read_BW(double BW[250])
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
    // system("pause");
}
int main()
{
    Read_BW(BW);
    // for(int i =0; i < 200; i++)
    // {
    //     cout<<BW[i]<<endl;
    // }

    for (int i=0; i<30;i++)
    {
        s+=user_trace[i];
        Buf[i]=0;
    }
    // cout << s<<endl;
    int x=3; // chon thuat toan
    // 1 : Next One
    // 2 : Water Fall
    // 3 : Proposed
    switch (x)
    {
        case 3: // Proposed
        {
            while(t<=500)
            {
                if(t>10)
                {
                    for(int i=(int)t-10; i<t-1 ;i++)
                    {
                        s+=BW[i];
                    }
                    s=s/10;
                    if(s>2.5*bitrate)
                    {
                        Bseg=2;                  
                        K=12;
                    }else if(s>2*bitrate)
                    {
                        Bseg=3;
                        K=7;
                    }else if(s>1.5*bitrate)
                    {
                        Bseg=3;   
                        K=4;
                    }else
                    {
                        Bseg=4;   
                        K=7;
                    }
                    
                }
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
                     cout <<"video: "<< video<< " " <<"W: "<< W << " "<<"t: "<< t << endl; 
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
                     cout <<"video: "<<video << " " <<"Tb: "<< Tb <<" "<<"t: "<< t << endl;
                    Tb=0;
                }else
                {
                    if(Buf[video] < 15 && video < Tong_video && (Buf[video]-Q) < Bseg) //buffer video hien tai
                    {
                        while(1)
                        {
                            // BW=5000 => sau 0.01s nó tải đc 50kb => buffer cho video đc 50/1000 = 0.05s
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
                            // BW=5000 => sau 0.01s nó tải đc 50kb => buffer cho video đc 50/1000 = 0.05s
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
                                 cout <<"video: "<< video<< " " <<"W: "<< W << " "<<"t: "<< t << endl; 
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
                        Tb=0;                     
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
            break;
        }
        case 1: // Next One
        {
            while(t<=500)
            {
                if(tx>=user_trace[video]) // chuyen video
                {
                    Waste+= (B0-Q);
                    W= (B0-Q); 
                    if (B0 < 15)
                    {
                        W+= seg;
                        Waste+= seg;
                        seg =0;
                    }
                    cout <<"video: "<< video<< " " <<"W: "<< W << " "<<"t: "<< t << endl; 
                    Q=0;
                    tx=0;
                    B0=B1;
                    B1=0;
                    video++;     
                }
                // cout << t<<endl;
                if(video == Tong_video) // user ko xem nua
                {
                    // cout << t;
                    break;
                }
                if((B0-Q)<=0) // rebuffer
                {
                    while ((B0-Q)<=0)
                    {
                        seg+=time_step*(BW[(int)t]/bitrate);
                        if( seg >=1)
                        {
                            B0+=seg;
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
                    }
                    cout <<"video: "<<video << " " <<"Tb: "<< Tb <<" "<<"t: "<< t << endl;
                    Tb=0;
                }else
                {
                    if(B0 < 15 && video < Tong_video) //buffer video hien tai
                    {
                        // BW=5000 => sau 0.01s nó tải đc 50kb => buffer cho video đc 50/1000 = 0.05s
                        seg+=time_step*(BW[(int)t]/bitrate);
                        if( seg >=1)
                        {
                            B0+=seg;
                            seg=0;
                        }
                        Q+=time_step;
                        tx+=time_step;
                    }else if(B0 >=15 && B1 < 15 && video < Tong_video -1) // buffer next video 
                    {
                        seg+=time_step*(BW[(int)t]/bitrate);
                        if( seg >=1)
                        {
                            B1+=seg;
                            seg=0;
                        }
                        Q+=time_step;
                        tx+=time_step;
                    }else if ((B0 >=15 && B1 >= 15 && video < Tong_video-1) || (B0 >=15 && video == Tong_video-1)  ) // da buffer toan bo video hien tai va next video
                    {
                        Q+=time_step;
                        tx+=time_step;
                    }
                    t+=time_step;
                }
                
            }
            break;
        }
        case 2: // Water Fall
        {
            while(t<=500)
            {
                if(tx>=user_trace[video]) // chuyen video
                {
                    Waste+= (B0-Q);
                    W= (B0-Q);
                    if (B0 < 15)
                    {
                        W+= seg;
                        Waste+= seg;
                        seg =0;
                    }
                    cout <<"video: "<< video<< " " <<"W: "<< W << " "<<"t: "<< t << endl; 
                    Q=0;
                    tx=0;
                    B0=B1;
                    B1=B2;
                    B2=0;
                    video++;   
                    // cout << W << endl;        
                }
                if(video == Tong_video) // user ko xem nua
                {
                    // cout << t;
                    break;
                }
                if((B0-Q)<=0) // rebuffer
                {
                    while ((B0-Q)<=0)
                    {
                        seg+=time_step*(BW[(int)t]/bitrate);
                        if( seg >=1)
                        {
                            B0+=seg;
                            seg=0;
                        }
                        tx+=time_step;
                        Tb+=time_step;
                        TB+=time_step;
                        t+=time_step;
                        if(tx==user_trace[video]||tx>user_trace[video])
                        {
                            break;
                        }
                    }
                    cout <<"video: "<<video << " " <<"Tb: "<< Tb <<" "<<"t: "<< t << endl;
                    Tb=0;
                }else
                {
                    if(B0 < 15) //buffer video hien tai
                    {
                        // BW=5000 => sau 0.01s nó tải đc 50kb => buffer cho video đc 50/1000 = 0.05s
                        seg+=time_step*(BW[(int)t]/bitrate);
                        if( seg >=1)
                        {
                            B0+=seg;
                            seg=0;
                        }
                        Q+=time_step;
                        tx+=time_step;
                    }else if(B0 >=15 && B1 < 15 && video <Tong_video -1) // buffer next video 
                    {
                        seg+=time_step*(BW[(int)t]/bitrate);
                        if( seg >=1)
                        {
                            B1+=seg;
                            seg=0;
                        }
                        Q+=time_step;
                        tx+=time_step;
                    }else if(B0 >=15 && B1 >= 15 && video <Tong_video - 2 && B2 <15) // buffer video thu 3
                    {
                        seg+=time_step*(BW[(int)t]/bitrate);
                        if( seg >=1)
                        {
                            B2+=seg;
                            seg=0;
                        }
                        Q+=time_step;
                        tx+=time_step;
                    }
                    else if ((B0 >=15 && B1 >= 15 && video < Tong_video-1) || (B0 >=15 && video == Tong_video-1) || (B0 >=15 && B1 >= 15 && B2 >=15 && video < Tong_video-2))// da buffer toan bo video hien tai, next video va video thu 3
                    {
                        Q+=time_step;
                        tx+=time_step;
                    }
                    t+=time_step;
                }
                
                
            }
            break;
        }
        default:
            break;
    }
    
    cout << "Waste: " << Waste << endl<< "Time rebuffer: "<< TB;
    return 0;
}
