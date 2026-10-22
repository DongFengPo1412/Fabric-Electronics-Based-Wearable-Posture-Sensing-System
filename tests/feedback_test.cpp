#include "posture_logic.h"
extern "C" int run_tests() {
    if(!Posture::IsAbnormal(31,0)||!Posture::IsAbnormal(0,36))return 1;
    if(Posture::IsAbnormal(30,35))return 2;
    Posture::Feedback pwm;
    for(unsigned t=0;t<1000;t+=25)if(pwm.Update(t,true,false)!=1023)return 3;
    for(unsigned t=1000;t<100000;t+=25){int value=pwm.Update(t,false,true);if(value<0||value>1023)return 4;}
    pwm.Update(100000,true,true);
    int previous=1023,edges=0;
    for(unsigned t=100050;t<=101000;t+=50){int value=pwm.Update(t,true,true);if(value!=previous)++edges;previous=value;}
    if(edges!=20)return 5;
    if(pwm.Update(101001,false,true)!=1023||pwm.Update(101026,false,true)!=1018)return 6;
    Posture::Feedback wrap;wrap.Update(0xfffffff0U,false,true);
    if(wrap.Update(0x00000009U,false,true)!=1018)return 7;
    return 0;
}
