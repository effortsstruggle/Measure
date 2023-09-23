#ifndef COMMON_H
#define COMMON_H
#include <QtGlobal>

#define HUAFENGDEVICEADDR "127.0.0.1"
#define GUODIANDEVICEADDR "127.0.0.2"


/***********设备ID**********/
enum PARMODELWIDGETFLAG
{
    ZLSJLF = 0x00 ,
    FLSJLF ,
    CYQYLTJF,
    LSB_1,
    LSB_2,
    LSBQLJPQF_1,
    LSBQLJPQF_2,
    LSG ,
    ERROR
};

/************国电寄存器地址************/
enum GUODIANREGISTERADDR
{
    TXSMXH_1 = 0,
    TXSMXH_2 ,

    YXZLSJLFKD_1 = 2,             //    右X  #1主凝水节流阀开度
    YXZLSJLFKD_2 ,

    YXZLSJLFFQYL_1= 4,           //    右X  #1主凝水节流阀前压力
    YXZLSJLFFQYL_2 ,

    YXZLSJLFFHYL_1 = 6 ,          //    右X  #1主凝水节流阀阀后压力
    YXZLSJLFFHYL_2,

    ZXZLSJLFKD_1 = 8,             //    左X #2主凝水节流阀开度
    ZXZLSJLFKD_2,

    ZXZLSJLFFQYL_1 = 10,           //    左X #2主凝水节流阀前压力
    ZXZLSJLFFQYL_2,

    ZXZLSJLFFHYL_1 = 12 ,          //    左X #2主凝水节流阀阀后压力
    ZXZLSJLFFHYL_2 ,

    YXFLSJLFKD_1 = 14,             //    右X #1 辅凝水节流阀开度
    YXFLSJLFKD_2,

    YXFLSJLFFQYL_1 = 16,           //    右X #1辅凝水节流阀前压力
    YXFLSJLFFQYL_2,

    YXFLSJLFFHYL_1 = 18,           //    右X #1辅凝水节流阀阀后压力
    YXFLSJLFFHYL_2 ,

    ZXFLSJLFKD_1 = 20 ,             //    左X #2辅凝水节流阀开度
    ZXFLSJLFKD_2 ,

    ZXFLSJLFFQYL_1 = 22,           //    左X #2辅凝水节流阀前压力
    ZXFLSJLFFQYL_2 ,

    ZXFLSJLFFHYL_1 = 24 ,          //    左X #2辅凝水节流阀阀后压力
    ZXFLSJLFFHYL_2,

    YXCYQYLTJF_1 = 26,             //    右X #1除氧器压力调节阀
    YXCYQYLTJF_2,

    YXCYQYLTJFFQYL_1 = 28,         //    右X #1除氧器压力调节阀阀阀前压力
    YXCYQYLTJFFQYL_2,

    YXCYQYLTJFFHYL_1 = 30 ,        //    右X #1除氧器压力调节阀阀阀后压力
    YXCYQYLTJFFHYL_2,

    ZXCYQYLTJF_1 = 32,             //    左X #2除氧器压力调节阀
    ZXCYQYLTJF_2,

    ZXCYQYLTJFFQYL_1 = 34,         //    左X #2除氧器压力调节阀阀阀前压力
    ZXCYQYLTJFFQYL_2 ,

    ZXCYQYLTJFFHYL_1 = 36 ,        //    左X #2除氧器压力调节阀阀阀后压力
    ZXCYQYLTJFFHYL_2 ,

    YXLSBZS_1_1  = 38 ,            //    右X 1#凝水泵转速
    YXLSBZS_1_2 ,

    YXLSBRKYL_1_1 = 40 ,           //    右X 1#凝水泵吸入口前压力
    YXLSBRKYL_1_2,

    YXLSBCKYL_1_1 = 42 ,           //    右X 1#凝水泵出口压力
    YXLSBCKYL_1_2 ,

    YXLSBZS_2_1  = 44 ,            //    右X 2#凝水泵转速
    YXLSBZS_2_2,

    YXLSBRKYL_2_1 = 46 ,           //    右X 2#凝水泵吸入口前压力
    YXLSBRKYL_2_2,

    YXLSBCKYL_2_1 = 42 ,           //    右X 2#凝水泵出口压力 (共用40042地址)
    YXLSBCKYL_2_2,

    ZXLSBZS_3_1  = 48 ,            //    左X 3#凝水泵转速
    ZXLSBZS_3_2 ,

    ZXLSBRKYL_3_1 = 50 ,           //    左X 3#凝水泵吸入口前压力
    ZXLSBRKYL_3_2,

    ZXLSBCKYL_3_1 = 52 ,           //    左X 3#凝水泵出口压力
    ZXLSBCKYL_3_2,

    ZXLSBZS_4_1  = 54,             //    左X 4#凝水泵转速
    ZXLSBZS_4_2,

    ZXLSBRKYL_4_1 = 56 ,           //    左X 4#凝水泵吸入口前压力
    ZXLSBRKYL_4_2,

    ZXLSBCKYL_4_1 = 52 ,           //    左X 4#凝水泵出口压力 (共用40052地址)
    ZXLSBCKYL_4_2 ,

    YXLSBQLJPQFKD_1_1 = 58,        //    右X 1#凝水泵汽轮机配汽阀开度
    YXLSBQLJPQFKD_1_2,

    YXLSBQLJPQFFQYL_1_1 = 60,      //    右X 1#凝水泵汽轮机配汽阀前压力
    YXLSBQLJPQFFQYL_1_2,

    YXLSBQLJPQFFHYL_1_1 = 62 ,      //    右X 1#凝水泵汽轮机配汽阀后压力
    YXLSBQLJPQFFHYL_1_2 ,

    YXLSBQLJPQFKD_2_1 = 64,        //    右X 2# 凝水泵汽轮机配汽阀开度
    YXLSBQLJPQFKD_2_2 ,

    YXLSBQLJPQFFQYL_2_1 = 60,      //    右X 2# 凝水泵汽轮机配汽阀前压力 (共用40060地址)
    YXLSBQLJPQFFQYL_2_2,

    YXLSBQLJPQFFHYL_2_1 = 66,      //    右X 2# 凝水泵汽轮机配汽阀后压力
    YXLSBQLJPQFFHYL_2_2,

    ZXLSBQLJPQFKD_3_1 = 68,        //    左X 3# 凝水泵汽轮机配汽阀开度
    ZXLSBQLJPQFKD_3_2,

    ZXLSBQLJPQFFQYL_3_1 = 70,      //    左X 3# 凝水泵汽轮机配汽阀前压力
    ZXLSBQLJPQFFQYL_3_2,

    ZXLSBQLJPQFFHYL_3_1 = 72,      //    左X 3# 凝水泵汽轮机配汽阀后压力
    ZXLSBQLJPQFFHYL_3_2 ,

    ZXLSBQLJPQFKD_4_1 = 74,        //    左X 4# 凝水泵汽轮机配汽阀开度
    ZXLSBQLJPQFKD_4_2,

    ZXLSBQLJPQFFQYL_4_1 = 70,      //    左X 4# 凝水泵汽轮机配汽阀前压力
    ZXLSBQLJPQFFQYL_4_2,

    ZXLSBQLJPQFFHYL_4_1 = 76,      //    左X 4# 凝水泵汽轮机配汽阀后压力
    ZXLSBQLJPQFFHYL_4_2,

    YXLSGJKYL_1 = 42 ,             //    右X 凝水管道 进口端压力 (共用 40042 地址)
    YXLSGJKYL_2,

    YXLSGCKYL_1 = 6 ,             //    右X 凝水管 出口端压力 （共用 40006地址）
    YXLSGCKYL_2,

    YXLSGSSLL_1 = 78 ,             //    右X 凝水管 实时流量
    YXLSGSSLL_2,

    ZXLSGJKYL_1 = 52 ,             //    左X 凝水管道 进口端压力 （共用 40052地址）
    ZXLSGJKYL_2,

    ZXLSGCKYL_1 = 12 ,             //    左X 凝水管 出口端压力  （共用40012地址）
    ZXLSGCKYL_2 ,

    ZXLSGSSLL_1 = 80 ,               //    左X 凝水管 实时流量
    ZXLSGSSLL_2
};


/************华风寄存器地址*************/
enum HUAFENGREGISTERADDR
{
    YXZLSJLFREGISTER_1 = 500,    //右X 1#凝水节流阀实时流量
    YXZLSJLFREGISTER_2 ,
    YXFLSJLFREGISTER_1 = 502 ,   // 右X 1#辅凝水节流阀实时流量
    YXFLSJLFREGISTER_2 ,
    YXCYQYLTJFREGISTER_1 = 504,  //右X 1#除氧器压力调节阀实时流量
    YXCYQYLTJFREGISTER_2 ,
    YXLSB_1REGISTER_1 = 506,     //右X 1#凝水泵实时流量
    YXLSB_1REGISTER_2 ,
    YXLSB_2REGISTER_1 = 508,     //右X 2#凝水泵实时流量
    YXLSB_2REGISTER_2 ,
    YXLSBQLJPQF_1REGISTER_1 = 510, //右X 1#凝水泵汽轮机配气阀实时流量
    YXLSBQLJPQF_1REGISTER_2 ,
    YXLSBQLJPQF_2REGISTER_1 = 512, //右X 2#凝水泵汽轮机配气阀实时流量
    YXLSBQLJPQF_2REGISTER_2 ,
    YXLSGREGISTER_1=514 ,          //右X 凝水管 阻力系数
    YXLSGREGISTER_2,

    ZXZLSJLFREGISTER_1 = 516,        //左X 2#凝水节流阀实时流量
    ZXZLSJLFREGISTER_2 ,
    ZXFLSJLFREGISTER_1 = 518 ,       //左X 2#辅凝水节流阀实时流量
    ZXFLSJLFREGISTER_2 ,
    ZXCYQYLTJFREGISTER_1 = 520,      //左X 2#除氧器压力调节阀实时流量
    ZXCYQYLTJFREGISTER_2 ,
    ZXLSB_1REGISTER_1 = 522,         //左X 3#凝水泵实时流量
    ZXLSB_1REGISTER_2 ,
    ZXLSB_2REGISTER_1 = 524,         //左X 4#凝水泵实时流量
    ZXLSB_2REGISTER_2 ,
    ZXLSBQLJPQF_1REGISTER_1 = 526,   //左X 3#凝水泵汽轮机配气阀实时流量
    ZXLSBQLJPQF_1REGISTER_2 ,
    ZXLSBQLJPQF_2REGISTER_1 = 528,   //左X 4#凝水泵汽轮机配气阀实时流量
    ZXLSBQLJPQF_2REGISTER_2 ,
    ZXLSGREGISTER_1 = 530 ,            //左X 凝水管 阻力系数
    ZXLSGREGISTER_2 ,

    BACKUP1_1 = 532 ,
    BACKUP1_2,
    BACKUP2_1 = 534,
    BACKUP2_2 ,
    BACKUP3_1 = 536,
    BACKUP3_2,
    BACKUP4_1 = 538,
    BACKUP4_2,
    BACKUP5_1 = 540,
    BACKUP5_2,
};




#endif // COMMON_H
