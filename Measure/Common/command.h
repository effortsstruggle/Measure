#ifndef COMMAND_H
#define COMMAND_H
#include <QtGlobal>

;
#pragma pack(push)
#pragma pack(1)
struct PACKET_HEAD
{
    qint8 work_id_hi;//事务ID 高8位
    qint8 work_id_lo;//事务ID 低8位
    qint16 procotol_id ; //协议ID
    qint8 length_hi ; //数据长度
    qint8 length_lo ; //数据长度
    qint8 device_addr ; //设备ID
};

/************写入多个寄存器**********/
struct PACKET_BODY_ManyReg
{
    qint8 functioncode; //功能码
    qint8 start_addr_hi;//起始地址 高8位
    qint8 start_addr_lo;//起始地址 低8位
    qint8 reg_num_hi;//寄存器数量 高8位
    qint8 reg_num_lo;//寄存器数量 低8位
    qint8 data_byte;//数据占用的字节
    float value_1; //往寄存器1写入的值
    float value_2; //往寄存器2写入的值
};
#pragma pack(pop)

//struct PACKET_BODY_DATA
//{
//    float yxzlsjlfssll; //右X 1#凝水节流阀实时流量
//    float yxflsjlfssll;  //右X 1#辅凝水节流阀实时流量
//    float yxcyqyltjfssll; //右X 1#除氧器压力调节阀实时流量
//    float yxlsbssll_1; //右X 1#凝水泵实时流量
//    float yxlsbssll_2; //右X 2#凝水泵实时流量
//    float yxlsbqlqpqfssll_1; //右X 1#凝水泵汽轮机配气阀实时流量
//    float yxlsbqlqpqfssll_2; //右X 2#凝水泵汽轮机配气阀实时流量
//    float yxlsgzlxs; //右X 凝水管 阻力系数
//    float zxzlsjlfssll; // 左X 2#凝水节流阀实时流量
//    float zxflsjlfssll; //左X 2#辅凝水节流阀实时流量
//    float zxcyqyltjfssll; // 左X 2#除氧器压力调节阀实时流量
//    float zxlsbssll_1; // 左X 3#凝水泵实时流量
//    float zxlsbssll_2; // 左X 4#凝水泵实时流量
//    float zxlsbqlqpqfssll_1; //左X 3#凝水泵汽轮机配气阀实时流量
//    float zxlsbqlqpqfssll_2; //左X 4#凝水泵汽轮机配气阀实时流量
//    float zxlsgzlxs; //左X 凝水管 阻力系数
//    float backup_1; //备用1
//    float backup_2; //备用2
//    float backup_3; //备用3
//    float backup_4; //备用4
//    float backup_5; //备用5
//};

//struct PACKET_BODY
//{
//    PACKET_BODY_HEAD body_head;
//    PACKET_BODY_DATA body_data;
//};

//typedef struct
//{
//   PACKET_HEAD head;
//   PACKET_BODY body;

//}RESPONSE_PACKET;





#endif // COMMAND_H
