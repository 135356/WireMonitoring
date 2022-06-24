//
// Created by 邦邦 on 2022/4/22.
//

#ifndef BB_BASE64_H
#define BB_BASE64_H
#include <string>
#include <cstring>

namespace bb{
    class Base64{
    public:
        static std::string en(const char *src_str){
            //定义base64编码表
            char base64_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
            //经过base64编码后的字符串长度
            size_t src_str_size = strlen(src_str), res_str_size{};
            if(src_str_size % 3 == 0){
                res_str_size = src_str_size / 3 * 4;
            }else{
                res_str_size = (src_str_size / 3 + 1) * 4;
            }
            int i, j;
            std::string res_str{};
            //char *res_str = new char[sizeof(unsigned char) * res_str_size + 1]; //内部申请的栈空间要在外部释放容易忘记
            for(i = 0, j = 0; i < res_str_size - 2; j += 3, i += 4){ //以3个8位字符为一组进行编码
                res_str += base64_table[src_str[j] >> 2]; //取出第一个字符的前6位并找出对应的结果字符
                res_str += base64_table[(src_str[j] & 0x3) << 4 | (src_str[j + 1] >> 4)]; //将第一个字符的后位与第二个字符的前4位进行组合并找到对应的结果字符
                res_str += base64_table[(src_str[j + 1] & 0xf) << 2 | (src_str[j + 2] >> 6)]; //将第二个字符的后4位与第三个字符的前2位组合并找出对应的结果字符
                res_str += base64_table[src_str[j + 2] & 0x3f]; //取出第三个字符的后6位并找出结果字符
            }
            //处理最后一组数据
            switch(src_str_size % 3){
                case 1:
                    res_str[i - 2] = '=';
                    res_str[i - 1] = '=';
                    break;
                case 2:
                    res_str[i - 1] = '=';
                    break;
            }
            return res_str;
        }

        static std::string de(const char *code){
            //根据base64表，以字符找到对应的十进制数据
            char decode64_table[] = {
                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                    62, // '+'
                    0, 0, 0,
                    63, // '/'
                    52, 53, 54, 55, 56, 57, 58, 59, 60, 61, // '0'-'9'
                    0, 0, 0, 0, 0, 0, 0,
                    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
                    13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, // 'A'-'Z'
                    0, 0, 0, 0, 0, 0,
                    26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38,
                    39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, // 'a'-'z'
            };
            std::string res_str{};
            size_t len = strlen(code);
            for(int i = 0; i < len - 2; i += 4){ //以4个字符为一位进行解码
                res_str += (decode64_table[code[i]]) << 2 | ((decode64_table[code[i + 1]]) >> 4); //取出第一个字符对应base64表的十进制数的前6位与第二个字符对应base64表的十进制数的后2位进行组合
                if(code[i + 1] == '=' || code[i + 2] == '='){break;}
                res_str += ((decode64_table[code[i + 1]]) << 4) | ((decode64_table[code[i + 2]]) >> 2); //取出第二个字符对应base64表的十进制数的后4位与第三个字符对应bas464表的十进制数的后4位进行组合
                if(code[i + 2] == '=' || code[i + 3] == '='){break;}
                res_str += ((decode64_table[code[i + 2]]) << 6) | (decode64_table[code[i + 3]]); //取出第三个字符对应base64表的十进制数的后2位与第4个字符进行组合
            }
            return res_str;
        }
    };
}
#endif //BB_BASE64_H
