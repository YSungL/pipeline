#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <string.h>
#include <bitset>
#include <sstream>
#include <math.h>
#include <fstream>
#include <cctype>
#include <algorithm>
using namespace std;
vector<long long> reg;
vector<string >d_mem;
#define mult_sign 9223372036854775807
#define mult_unsign 18446744073709551615
int lock_num = 0;
int lock = 0;
int safe_H = 0;//use for mult
int safe_L = 0;
int cycle = 0;
ofstream fout1 , fout2;
long long convert_dex(string str_bin)
{
    int str_size = str_bin.size();
    long long dec = 0;
    int bin_pow = 0;
    for(int i = (str_size-1) ; i>=0 ; i--)
    {
        int num = 0;
        //cost ~0.017sec
        stringstream temp_num;
        temp_num << str_bin[i];
        temp_num >> num;
        long long temp = num * pow(2,bin_pow);
        dec += temp;
        bin_pow++;
        // cost ~0.024sec
        /*
        if(str_bin[i] == '1') num = 1;
        int temp = num * pow(2,bin_pow);
        dec += temp;
        bin_pow++;*/
    }
    return dec;
}
string convert_bin(long long num)
{
    if(num < 0)//avoid data is signed
    {
        num = num & 4294967295;
    }
    string temp = "";
    while(true)
    {
        if(num % 2 == 0)
        {
            temp += "0";
            num = num/2;
        }
        else if(num % 2 == 1)
        {
            temp += "1";
            num = num/2;
        }
        if(num == 0)
            break;
    }
    string output = "";
    int s_num = temp.size();
    for(int i = 31; i >= 0 ; i--)
    {
        if(i >= s_num)
            output += "0";
        else
            output += temp[i];
    }
    return output;
}
string convert_hex(long long num)
{
    if(num < 0)//avoid data is signed
    {
        num = num & 4294967295;
    }
    string temp = "";
    while(true)
    {
        if(num % 16 == 0)
        {
            temp += "0";
            num = num/16;
        }
        else if(num % 16 == 1)
        {
            temp += "1";
            num = num/16;
        }
        else if(num % 16 == 2)
        {
            temp += "2";
            num = num/16;
        }
        else if(num % 16 == 3)
        {
            temp += "3";
            num = num/16;
        }
        else if(num % 16 == 4)
        {
            temp += "4";
            num = num/16;
        }
        else if(num % 16 == 5)
        {
            temp += "5";
            num = num/16;
        }
        else if(num % 16 == 6)
        {
            temp += "6";
            num = num/16;
        }
        else if(num % 16 == 7)
        {
            temp += "7";
            num = num/16;
        }
        else if(num % 16 == 8)
        {
            temp += "8";
            num = num/16;
        }
        else if(num % 16 == 9)
        {
            temp += "9";
            num = num/16;
        }
        else if(num % 16 == 10)
        {
            temp += "A";
            num = num/16;
        }
        else if(num % 16 == 11)
        {
            temp += "B";
            num = num/16;
        }
        else if(num % 16 == 12)
        {
            temp += "C";
            num = num/16;
        }
        else if(num % 16 == 13)
        {
            temp += "D";
            num = num/16;
        }
        else if(num % 16 == 14)
        {
            temp += "E";
            num = num/16;
        }
        else
        {
            temp += "F";
            num = num/16;
        }
        if(num == 0)
            break;
    }
    string output = "0x";
    int s_num = temp.size();
    for(int i = 7; i >= 0 ; i--)
    {
        if(i >= s_num)
            output += "0";
        else
            output += temp[i];
    }
    return output;

}
string to_str(int &i){
  string s;
  stringstream ss(s);
  ss << i;
  return ss.str();
}
string upper(string &word)
{

    string result = "";
    result = word;
    //cout << "result : " << result << endl;
    transform(result.begin(), result.end(), result.begin(), (int(*)(int))toupper);
    return result;
}
void pipline_change(string (&arr)[5])
{
    for(int i = 5 ; i > 1 ; i--)
    {
        arr[i-1] = "";
        arr[i-1] = arr[i-2];
        if(i-2 == 0)
           arr[0] = "";
    }
}
//pipline_stage : wb -> stall detection -> stage change
string int_to_str(int &i) {
  string s;
  stringstream ss(s);
  ss << i;
  return ss.str();
}

int ins_add(string rs , string rt , string rd , int wb_inf , long long &rt_buf , int &error_1, int &error_2 , int &write_reg) // sign +-
{
    if(wb_inf == 0)
    {
        int booling = 0;
        int t1 = convert_dex(rs);
        int t2 = convert_dex(rt);
        int t3 = convert_dex(rd);
        long long temp_1 = 0;
        long long temp_2 = 0;
        long long temp = reg[t3];
        if(reg[t1] > 2147483647) //from overflow error
            temp_1 = int32_t(reg[t1]);
        else if(reg[t1] < int(-2147483648))
            temp_1 = (reg[t1]) & 2147483647;
        else
            temp_1 = reg[t1];
        if(reg[t2] > 2147483647)
            temp_2 = int32_t(reg[t2]);
        else if(reg[t2] < int(-2147483648))
            temp_2 = (reg[t2]) & 2147483647;
        else
            temp_2 = reg[t2];
        reg[t3] = temp_1 + temp_2;
        //fout1 << "cycle " << cycle << endl;
        if(t3 == 0)
        {
            error_1 = 1;
            //fout2 << "In cycle " << cycle << ": " << "Write $0 Error" << endl;
            booling = 1;
        }
        int no_double = 0;
        if((reg[t3]) > 2147483647 || (reg[t3]) < int(-2147483648) )
        {
            error_2 = 1;
            //fout2 << "In cycle " << cycle << ": " << "Number Overflow" << endl;
            no_double = 1;
        }
        if((reg[t1] < 0 && reg[t2] < 0 && reg[t3] > 0) ||(reg[t1] > 0 && reg[t2] > 0 && reg[t3] < 0))
        {
            if(no_double == 0)
                error_2 = 1;
                //fout2 << "In cycle " << cycle << ": " << "Number Overflow" << endl;
        }
        if(booling == 1)
            reg[t3] = 0;
        rt_buf = reg[t3];
        if(convert_bin(reg[t3]) != convert_bin(temp))
            write_reg = 1;
        reg[34] = reg[34] + 4;
        fout1 << "PC: " << convert_hex(reg[34]) << endl;
    }

    else
    {
        int t3 = convert_dex(rd);
        if(error_1 == 1)
            fout2 << "In cycle " << cycle << ": " << "Write $0 Error" << endl;
        if(error_2 == 1)
            fout2 << "In cycle " << cycle << ": " << "Number Overflow" << endl;
        if(write_reg == 1)
        {
            if(t3 < 10)
                fout1 << "$0" <<t3 << ": " << convert_hex(rt_buf) << endl;
            else
                fout1 << "$" << t3 << ": " << convert_hex(rt_buf) << endl;
        }
    }

    return 0;

}
int ins_addu(string rs , string rt , string rd, int wb_inf , long long &rt_buf , int &error_1, int &error_2 , int &write_reg)
{
    if(wb_inf == 0)
    {
        int t1 = convert_dex(rs);
        int t2 = convert_dex(rt);
        int t3 = convert_dex(rd);
        unsigned long long temp = reg[t3];
        unsigned int temp_num_1 = 0;
        unsigned int temp_num_2 = 0;
        if(reg[t1] < 0 && reg[t2] >0)
        {
            temp_num_1 = reg[t1]& 4294967295;
            reg[t3] = temp_num_1 + reg[t2];
        }
        else if (reg[t1] > 0 && reg[t2] < 0)
        {
            temp_num_2 = reg[t2]& 4294967295;
            reg[t3] = temp_num_2 + reg[t1];
        }
        else if(reg[t1] < 0 && reg[t2] < 0)
        {
            temp_num_1 = reg[t1]& 4294967295;
            temp_num_2 = reg[t2]& 4294967295;
            reg[t3] = temp_num_1 + temp_num_2;
        }
        else
            reg[t3] = reg[t1] + reg[t2];
        //fout1 << "cycle " << cycle << endl;
        if(t3 == 0)
        {
            error_1 = 1;
            //fout2 << "In cycle " << cycle << ": " << "Write $0 Error" << endl;
            reg[t3] = 0;
        }
        if(reg[t3] > 4294967295)
        {
            reg[t3] = reg[t3] & 4294967295;
        }
        rt_buf = reg[t3];
        if(convert_bin(reg[t3]) != convert_bin(temp))
            write_reg = 1;
        reg[34] = reg[34] + 4;
        fout1 << "PC: " << convert_hex(reg[34]) << endl;
    }
    else
    {
        int t3 = convert_dex(rd);
        if(error_1 == 1)
            fout2 << "In cycle " << cycle << ": " << "Write $0 Error" << endl;
        if(error_2 == 1)
            fout2 << "In cycle " << cycle << ": " << "Number Overflow" << endl;
        if(write_reg == 1)
        {
            if(t3 < 10)
                fout1 << "$0" <<t3 << ": " << convert_hex(rt_buf) << endl;
            else
                fout1 << "$" <<t3 << ": " << convert_hex(rt_buf) << endl;
        }
    }

    return 0;
}
int ins_sub(string rs , string rt , string rd, int wb_inf , long long &rt_buf , int &error_1, int &error_2 , int &write_reg)// sign +-
{
    if(wb_inf == 0)
    {
        int booling = 0;
        int t1 = convert_dex(rs);
        int t2 = convert_dex(rt);
        int t3 = convert_dex(rd);
        //cout << reg[t1] << endl;
        //cout << reg[t2] << endl;
        long long temp = reg[t3];
        long long temp_1 = 0;
        long long temp_2 = 0;
        if(reg[t1] > 2147483647)
            temp_1 = int32_t(reg[t1]);
        else if(reg[t1] < int(-2147483648))
            temp_1 = (reg[t1]) & 2147483647;
        else
            temp_1 = reg[t1];
        if(reg[t2] > 2147483647)
            temp_2 = int32_t(reg[t2]);
        else if(reg[t2] < int(-2147483648))
            temp_2 = (reg[t2]) & 2147483647;
        else
            temp_2 = reg[t2];
        reg[t3] = (temp_1 - temp_2);
        //fout1 << "cycle " << cycle << endl;
        if(t3 == 0)
        {
            error_1 = 1;
            //fout2 << "In cycle " << cycle << ": " << "Write $0 Error" << endl;
            booling = 1;
        }
        int no_double = 0;
        if((reg[t3]) > 2147483647 || (reg[t3]) < int(-2147483648))
        {
            error_2 = 1;
            //fout2 << "In cycle " << cycle << ": " << "Number Overflow" << endl;
            no_double = 1;
        }
        if((reg[t1] < 0 && reg[t2] < 0 && reg[t3] > 0) ||(reg[t1] > 0 && reg[t2] > 0 && reg[t3] < 0))
        {
            if(no_double == 0)
                error_2 = 1;
                //fout2 << "In cycle " << cycle << ": " << "Number Overflow" << endl;
        }
        if(reg[t1] == int(-2147483648) && reg[t2]== int32_t(-2147483648))
        {
            if(no_double == 0)
                error_2 = 1;
                //fout2 << "In cycle " << cycle << ": " << "Number Overflow" << endl;
        }
        //cout << reg[t3] << endl;
        if(booling == 1)
            reg[t3] = 0;
        rt_buf = reg[t3];
        if(convert_bin(reg[t3]) != convert_bin(temp))
                write_reg = 1;
        reg[34] = reg[34] + 4;
        fout1 << "PC: " << convert_hex(reg[34]) << endl;
    }
    else
    {
        int t3 = convert_dex(rd);
        if(error_1 == 1)
            fout2 << "In cycle " << cycle << ": " << "Write $0 Error" << endl;
        if(error_2 == 1)
            fout2 << "In cycle " << cycle << ": " << "Number Overflow" << endl;
        if(write_reg == 1)
        {
            if(t3 < 10)
                fout1 << "$0" <<t3 << ": " << convert_hex(rt_buf) << endl;
            else
                fout1 << "$"<<t3 << ": " << convert_hex(rt_buf) << endl;
        }

    }


    return 0;
}
int ins_and(string rs , string rt , string rd, int wb_inf , long long &rt_buf, int &error_1, int &error_2 , int &write_reg)// sign +-
{
    if(wb_inf == 0)
    {
        int t1 = convert_dex(rs);
        int t2 = convert_dex(rt);
        int t3 = convert_dex(rd);
        long long temp = reg[t3];
        reg[t3] = (reg[t1]) & (reg[t2]);
        //fout1 << "cycle " << cycle << endl;
        if(t3 == 0)
        {
            error_1 = 1;
            //fout2 << "In cycle " << cycle << ": " << "Write $0 Error" << endl;
            reg[t3] = 0;
        }
        rt_buf = reg[t3];
        if(convert_bin(reg[t3]) != convert_bin(temp))
        {
            write_reg = 1;
        }
        reg[34] = reg[34] + 4;
        fout1 << "PC: " << convert_hex(reg[34]) << endl;
    }
    else
    {
        int t3 = convert_dex(rd);
        if(error_1 == 1)
            fout2 << "In cycle " << cycle << ": " << "Write $0 Error" << endl;
        if(error_2 == 1)
            fout2 << "In cycle " << cycle << ": " << "Number Overflow" << endl;
        if(write_reg == 1)
        {
            if(t3 < 10)
                fout1 << "$0" <<t3 << ": " << convert_hex(rt_buf) << endl;
            else
                fout1 << "$"<<t3 << ": " << convert_hex(rt_buf) << endl;
        }

    }
    return 0;
}
int ins_or(string rs , string rt , string rd, int wb_inf , long long  &rt_buf, int &error_1, int &error_2 , int &write_reg)// sign +-
{
    if(wb_inf == 0)
    {
        int t1 = convert_dex(rs);
        int t2 = convert_dex(rt);
        int t3 = convert_dex(rd);
        long long temp = reg[t3];
        reg[t3] = (reg[t1]) | (reg[t2]);
        //fout1 << "cycle " << cycle << endl;
        if(t3 == 0)
        {
            error_1 = 1;
            //fout2 << "In cycle " << cycle << ": " << "Write $0 Error" << endl;
            reg[t3] = 0;
        }
        rt_buf = reg[t3];
        if(convert_bin(reg[t3]) != convert_bin(temp))
        {
            write_reg = 1;
        }
        reg[34] = reg[34] + 4;
        fout1 << "PC: " << convert_hex(reg[34]) << endl;
    }
    else
    {
        int t3 = convert_dex(rd);
        if(error_1 == 1)
            fout2 << "In cycle " << cycle << ": " << "Write $0 Error" << endl;
        if(error_2 == 1)
            fout2 << "In cycle " << cycle << ": " << "Number Overflow" << endl;
        if(write_reg == 1)
        {
            if(t3 < 10)
                fout1 << "$0" <<t3 << ": " << convert_hex(rt_buf) << endl;
            else
                fout1 << "$"<<t3 << ": " << convert_hex(rt_buf) << endl;
        }
    }

    return 0;
}
int ins_xor(string rs , string rt , string rd, int wb_inf, long long &rt_buf, int &error_1, int &error_2 , int &write_reg)// sign +-
{
    if(wb_inf == 0)
    {
        int t1 = convert_dex(rs);
        int t2 = convert_dex(rt);
        int t3 = convert_dex(rd);
        long long temp = reg[t3];
        reg[t3] = (reg[t1]) ^ (reg[t2]);
        //cout << "xor-t3 = " << t3 << endl;
        //fout1 << "cycle " << cycle << endl;
        if(t3 == 0)
        {
            error_1 = 1;
            //fout2 << "In cycle " << cycle << ": " << "Write $0 Error" << endl;
            reg[t3] = 0;
        }
        rt_buf = reg[t3];
        if(convert_bin(reg[t3]) != convert_bin(temp))
        {
            write_reg = 1;
        }
        reg[34] = reg[34] + 4;
        fout1 << "PC: " << convert_hex(reg[34]) << endl;
    }
    else
    {
        int t3 = convert_dex(rd);
        if(error_1 == 1)
            fout2 << "In cycle " << cycle << ": " << "Write $0 Error" << endl;
        if(error_2 == 1)
            fout2 << "In cycle " << cycle << ": " << "Number Overflow" << endl;
        if(write_reg == 1)
        {
            if(t3 < 10)
                fout1 << "$0" <<t3 << ": " << convert_hex(rt_buf) << endl;
            else
                fout1 << "$"<<t3 << ": " << convert_hex(rt_buf) << endl;
        }

    }

    return 0;
}
int ins_nor(string rs , string rt , string rd, int wb_inf, long long &rt_buf, int &error_1, int &error_2 , int &write_reg)// sign +-
{
    if(wb_inf == 0)
    {
        int t1 = convert_dex(rs);
        int t2 = convert_dex(rt);
        int t3 = convert_dex(rd);
        long long temp = reg[t3];
        reg[t3] = ~((reg[t1]) | (reg[t2]));
        //fout1 << "cycle " << cycle << endl;
        //cout << "nor-t3 = " << t3 << endl;
        //cout << reg[t3] << endl;
        if(t3 == 0)
        {
            error_1 = 1;
            //fout2 << "In cycle " << cycle << ": " << "Write $0 Error" << endl;
            reg[t3] = 0;
        }
        rt_buf = reg[t3];
        if(convert_bin(reg[t3]) != convert_bin(temp))
        {
            write_reg = 1;
        }
        reg[34] = reg[34] + 4;
        fout1 << "PC: " << convert_hex(reg[34]) << endl;
    }
    else
    {
        int t3 = convert_dex(rd);
        if(error_1 == 1)
            fout2 << "In cycle " << cycle << ": " << "Write $0 Error" << endl;
        if(error_2 == 1)
            fout2 << "In cycle " << cycle << ": " << "Number Overflow" << endl;
        if(write_reg == 1)
        {
            if(t3 < 10)
                fout1 << "$0" <<t3 << ": " << convert_hex(rt_buf ) << endl;
            else
                fout1 << "$"<<t3 << ": " << convert_hex(rt_buf ) << endl;
        }

    }
    return 0;
}
int ins_nand(string rs , string rt , string rd, int wb_inf, long long &rt_buf, int &error_1, int &error_2 , int &write_reg)// sign +-
{
    if(wb_inf == 0)
    {
        int t1 = convert_dex(rs);
        int t2 = convert_dex(rt);
        int t3 = convert_dex(rd);
        long long temp = reg[t3];
        reg[t3] = ~((reg[t1]) & (reg[t2]));
        //cout << "nand-t3 = " << t3 << endl;
        //fout1 << "cycle " << cycle << endl;
        if(t3 == 0)
        {
            error_1 = 1;
            //fout2 << "In cycle " << cycle << ": " << "Write $0 Error" << endl;
            reg[t3] = 0;
        }
        rt_buf = reg[t3];
        if(convert_bin(reg[t3]) != convert_bin(temp))
        {
            write_reg = 1;
        }
        reg[34] = reg[34] + 4;
        fout1 << "PC: " << convert_hex(reg[34]) << endl;
    }
    else
    {
        int t3 = convert_dex(rd);
        if(error_1 == 1)
            fout2 << "In cycle " << cycle << ": " << "Write $0 Error" << endl;
        if(error_2 == 1)
            fout2 << "In cycle " << cycle << ": " << "Number Overflow" << endl;
        if(write_reg == 1)
        {
            if(t3 < 10)
                fout1 << "$0" <<t3 << ": " << convert_hex(rt_buf) << endl;
            else
                fout1 << "$"<<t3 << ": " << convert_hex(rt_buf) << endl;
        }

    }
    return 0;
}
int ins_slt(string rs , string rt , string rd, int wb_inf, long long &rt_buf, int &error_1, int &error_2 , int &write_reg)
{
    if(wb_inf == 0)
    {
        int t1 = convert_dex(rs);
        int t2 = convert_dex(rt);
        int t3 = convert_dex(rd);
        long long temp = reg[t3];
        long long comp_1 = 0;
        long long comp_2 = 0;
        //fout1 << "cycle " << cycle << endl;
        //cout << "xor-t1 = " << t1 << endl;
        //cout << "xor-t2 = " << t2 << endl;
        if(reg[t1] > 2147483647 )
            comp_1 = int32_t(reg[t1]);
        else if(reg[t1] < int(-2147483648))
            comp_1 = (reg[t1]) & 2147483647;
        else
            comp_1 = reg[t1];
        if(reg[t2] > 2147483647)
            comp_2 = int32_t(reg[t2]);
        else if(reg[t2] < int(-2147483648))
            comp_2 = (reg[t2]) & 2147483647;
        else
            comp_2 = reg[t2];
        //cout << t1 <<endl;
        //cout << reg[t2] <<endl;
        if(int(comp_1) < int(comp_2))
            reg[t3] = 1;
        else
            reg[t3] = 0;
        //cout << reg[t3] <<endl;
        if(t3 == 0)
        {
            error_1 = 1;
            //fout2 << "In cycle " << cycle << ": " << "Write $0 Error" << endl;
            reg[t3] = 0;
        }
        rt_buf = reg[t3];
        if(convert_bin(reg[t3]) != convert_bin(temp))
        {
            write_reg = 1 ;
        }
        reg[34] = reg[34] + 4;
        fout1 << "PC: " << convert_hex(reg[34]) << endl;
    }
    else
    {
        int t3 = convert_dex(rd);
        if(error_1 == 1)
            fout2 << "In cycle " << cycle << ": " << "Write $0 Error" << endl;
        if(error_2 == 1)
            fout2 << "In cycle " << cycle << ": " << "Number Overflow" << endl;
        if(write_reg == 1)
        {
            if(t3 < 10)
                fout1 << "$0" <<t3 << ": " << convert_hex(rt_buf) << endl;
            else
                fout1 << "$"<<t3 << ": " << convert_hex(rt_buf) << endl;
        }

    }
    return 0;
}
int ins_sll(string rt , string rd , string shamt, int wb_inf, long long &rt_buf, int &error_1, int &error_2 , int &write_reg)//+ ~ -
{
    if(wb_inf == 0)
    {
        int booling = 0;
        int t1 = convert_dex(rt);
        int t2 = convert_dex(rd);
        int t3 = convert_dex(shamt);
        long long use_1 = 0;
        if(reg[t1] > 2147483647)
            use_1 = int32_t(reg[t1]);
        else if(reg[t1] < int(-2147483648))
            use_1 = (reg[t1]) & 2147483647;
        else
            use_1 = (reg[t1]);
        long long temp = reg[t2];
        //fout1 << "cycle " << cycle << endl;
        int spec = 0;
        if(t2 == 0 && t1 == 0 && t3 == 0)
            spec = 1;
        if((t2 == 0) && (spec != 1))
        {
            error_1 = 1;
            //fout2 << "In cycle " << cycle << ": " << "Write $0 Error" << endl;
            booling = 1;
        }
        reg[t2] = (use_1) << t3;
        if(booling == 1)
            reg[t2] = 0;
        rt_buf = reg[t2];
        if(convert_bin(reg[t2]) != convert_bin(temp))
        {
            write_reg = 1;
        }
        reg[34] = reg[34] + 4;
        fout1 << "PC: " << convert_hex(reg[34]) << endl;
    }
    else
    {
        int t2 = convert_dex(rd);
        if(error_1 == 1)
            fout2 << "In cycle " << cycle << ": " << "Write $0 Error" << endl;
        if(error_2 == 1)
            fout2 << "In cycle " << cycle << ": " << "Number Overflow" << endl;
        if(write_reg == 1)
        {
            if(t2 < 10)
                fout1 << "$0" << t2 << ": " << convert_hex(rt_buf) << endl;
            else
                fout1 << "$"<<t2 << ": " << convert_hex(rt_buf) << endl;
        }

    }
    return 0;
}

int ins_srl(string rt , string rd , string shamt, int wb_inf, long long &rt_buf, int &error_1, int &error_2 , int &write_reg)//+ ~ -
{
    if(wb_inf == 0)
    {
        int t1 = convert_dex(rt);
        int t2 = convert_dex(rd);
        int t3 = convert_dex(shamt);
        long long temp = reg[t2];
        long long temp_1 = 0;
        if(reg[t1] > 2147483647)
            temp_1 = int32_t(reg[t1]);
        else if(reg[t1] < int(-2147483648))
            temp_1 = (reg[t1]) & 2147483647;
        else
            temp_1 = reg[t1];
        //cout << "reg[t1] = "<< reg[t1] << endl;
        //cout << "reg[t2] = "<< reg[t2] << endl;
        //cout << "t3 = " << t3 << endl;
        //fout1 << "cycle " << cycle << endl;
        if(temp_1 < 0)
        {
            long long temp_num = temp_1 >> t3;
            int ppow = pow(2,(32-t3)) - 1;
            reg[t2] = (temp_num & ppow);
        }
        else
            reg[t2] = temp_1 >> t3;

        //cout << "reg[t2] = "<< reg[t2] << endl;
        if(t2 == 0)
        {
            error_1 = 1;
            //fout2 << "In cycle " << cycle << ": " << "Write $0 Error" << endl;
            reg[t2] = 0;
        }
        rt_buf = reg[t2];
        if(convert_bin(reg[t2]) != convert_bin(temp))
            write_reg = 1;
        reg[34] = reg[34] + 4;
        fout1 << "PC: " <<convert_hex(reg[34]) << endl;
    }
    else
    {
        int t2 = convert_dex(rd);
        if(error_1 == 1)
            fout2 << "In cycle " << cycle << ": " << "Write $0 Error" << endl;
        if(error_2 == 1)
            fout2 << "In cycle " << cycle << ": " << "Number Overflow" << endl;
        if(write_reg == 1)
        {
            if(t2 < 10)
                fout1 << "$0" << t2 << ": " << convert_hex(rt_buf) << endl;
            else
                fout1 << "$" << t2 << ": " << convert_hex(rt_buf) << endl;
        }

    }
    return 0;
}

int ins_sra(string rt , string rd , string shamt, int wb_inf, long long &rt_buf, int &error_1, int &error_2 , int &write_reg)//+~- sign extend
{
    if(wb_inf == 0)
    {
        int t1 = convert_dex(rt);
        int t2 = convert_dex(rd);
        int t3 = convert_dex(shamt);
        long long temp = reg[t2];
        long long temp_1 = 0;
        if(reg[t1] > 2147483647)
            temp_1 = int32_t(reg[t1]);
        else if(reg[t1] < int(-2147483648))
            temp_1 = (reg[t1]) & 2147483647;
        else
            temp_1 = reg[t1];
        reg[t2] = temp_1 >> t3;
        //fout1 << "cycle " << cycle << endl;
        if(t2 == 0)
        {
            error_1 = 1;
            //fout2 << "In cycle " << cycle << ": " << "Write $0 Error" << endl;
            reg[t2] = 0;
        }
        if((reg[t2]) > 4294967295 || (reg[t2]) < int(-2147483648))
        {
            reg[t2] = 0;
        }
        rt_buf = reg[t2];
        if(convert_bin(reg[t2]) != convert_bin(temp))
        {
            write_reg = 1;
        }
        reg[34] = reg[34] + 4;
        fout1 << "PC: " <<convert_hex(reg[34]) << endl;
    }
    else
    {
        int t2 = convert_dex(rd);
        if(error_1 == 1)
            fout2 << "In cycle " << cycle << ": " << "Write $0 Error" << endl;
        if(error_2 == 1)
            fout2 << "In cycle " << cycle << ": " << "Number Overflow" << endl;
        if(write_reg == 1)
        {
            if(t2 < 10)
                fout1 << "$0" << t2 << ": " << convert_hex(rt_buf) << endl;
            else
                fout1 << "$"<<t2 << ": " << convert_hex(rt_buf) << endl;
        }

    }
    return 0;
}

int ins_jr(string rs, int wb_inf)
{
    if(wb_inf == 0)
    {
        int t1 = convert_dex(rs);
        int temp_add;
        //fout1 << "cycle " << cycle << endl;
        temp_add = reg[34];
        temp_add = (temp_add-reg[t1])/4;
        reg[34] = reg[t1];
        //fout1 << "temp_add = " << temp_add << endl;
        fout1 << "PC: " << convert_hex(reg[34]) << endl;
        //system("PAUSE");
        //cout << temp_add << endl;
        return temp_add;
    }
    else
    {
        return 0;
    }

}

int ins_mult(string rs , string rt, int wb_inf , long long &rt_buf_H ,long long &rt_buf_L , int &error_1, int &error_2 , int &write_reg)
{
    if(wb_inf == 0)
    {
        long long result_1 = 0;
        long long result_2 = 0;
        long long result = 0;
        long long temp = 0;
        int t1 = convert_dex(rs);
        int t2 = convert_dex(rt);
        long long temp_HI = reg[32];
        long long temp_LO = reg[33];
        result = int32_t(reg[t1]) * int32_t(reg[t2]);
        //fout1 << "cycle " << cycle << endl;
        if(result < 0)
        {
            result = result & mult_unsign;
        }
        result_1 = result/pow(2,32);
        reg[32] = result_1 ;
        temp = result_1 * pow(2,32);
        result_2 = result - temp;
        reg[33]  = result_2 ;
        if((reg[t1] < 0 && reg[t2] > 0) || (reg[t1] > 0 && reg[t2] < 0))
        {
            unsigned long long ind_result = int32_t(reg[t1]) * int32_t(reg[t2]);
            reg[32] = ind_result/pow(2,32);
            long long ind_temp = reg[32] * pow(2,32);
            reg[33] = ind_result - ind_temp;
        }
        if(reg[32] > 4294967295)
            reg[32] = 4294967295;
        if(reg[33] > 4294967295)
            reg[33] = 4294967295;
        if((reg[32]) > 4294967295 || (reg[33]) > 4294967295)
            error_1 = 1;
            //fout2 << "In cycle " << cycle << ": " << "Number Overflow" << endl;
        int no_double = 0;
        if(safe_H == 1 && safe_L == 1)
        {
            if(convert_bin(reg[32]) != convert_bin(temp_HI))
            {
                error_2 = 1;
                //fout2 << "In cycle " << cycle << ": " << "Overwrite HI-LO registers" << endl;
                no_double = 1;
            }
            if(convert_bin(reg[33]) != convert_bin(temp_LO) && no_double == 0)
                error_2 = 1;
                //fout2 << "In cycle " << cycle << ": " << "Overwrite HI-LO registers" << endl;
        }
        rt_buf_H = reg[32];
        rt_buf_L = reg[33];
        if(convert_bin(reg[32]) != convert_bin(temp_HI))
            write_reg = 1;
            //fout1 << "$HI: " << convert_hex(reg[32]) << endl;
        if(convert_bin(reg[33]) != convert_bin(temp_LO))
        {
            if(write_reg == 1)
                write_reg = 3;
            else
                write_reg = 2;
        }
            //fout1 << "$LO: " << convert_hex(reg[33]) << endl;
        safe_H = 1;
        safe_L = 1;
        reg[34] = reg[34] + 4;
        fout1 << "PC: " << convert_hex(reg[34]) << endl;
    }
    else if(wb_inf == 2)
    {
        if(error_1 == 1)
            fout2 << "In cycle " << cycle << ": " << "Number Overflow" << endl;
        if(error_2 == 1)
            fout2 << "In cycle " << cycle << ": " << "Overwrite HI-LO registers" << endl;
        if(write_reg == 1)
            fout1 << "$HI: " << convert_hex(rt_buf_H) << endl;
        else if(write_reg == 2)
            fout1 << "$LO: " << convert_hex(rt_buf_L) << endl;
        else if(write_reg == 3)
        {
            fout1 << "$HI: " << convert_hex(rt_buf_H) << endl;
            fout1 << "$LO: " << convert_hex(rt_buf_L) << endl;
        }

    }
    return 0;

}
int ins_multu(string rs , string rt, int wb_inf , long long &rt_buf_H ,long long &rt_buf_L , int &error_1, int &error_2 , int &write_reg)
{
    if(wb_inf == 0)
    {
        unsigned long long result = 0;
        unsigned long long temp = 0;
        int t1 = convert_dex(rs);
        int t2 = convert_dex(rt);
        long long temp_1 = 0;
        long long temp_2 = 0;
        //fout1 << "cycle " << cycle << endl;
        if(reg[t1] < 0 && reg[t2] > 0)
        {
            temp_1 = reg[t1] &4294967295;
            result = temp_1 * reg[t2];
        }
        else if(reg[t2] < 0 && reg[t1] > 0)
        {
            temp_2 = reg[t2] &4294967295;
            result = reg[t1] * temp_2;
        }
        else if(reg[t2] < 0 && reg[t1] < 0)
        {
            temp_1 = reg[t1] &4294967295;
            temp_2 = reg[t2] &4294967295;
            result = temp_1 * temp_2;
        }
        else
            result = reg[t1] * reg[t2];
        long long temp_HI = reg[32];
        long long temp_LO = reg[33];
        //cout << result << endl;
        result = (result&mult_unsign);
        //cout << "HI = " << reg[32] << endl;
        //cout << "LO = " << reg[33] << endl;
        //cout << result << endl;
        reg[32] = result/pow(2,32);
        if(reg[32] > 4294967295)
            reg[32] = reg[32] & 4294967295;
        temp = reg[32] * pow(2,32);
        reg[33] = result - temp ;
        //cout << "HI = " << reg[32] << endl;
        //cout << "LO = " << reg[33] << endl;
        //system("PAUSE");
        if((reg[32]) > 4294967295 || (reg[33]) > (4294967295))
            error_1 = 1;
            //fout2 << "In cycle " << cycle << ": " << "Number Overflow" << endl;
        int no_double = 0;
        if(safe_H == 1 && safe_L == 1)
        {
            if(convert_bin(reg[32]) != convert_bin(temp_HI))
            {
                error_2 = 1;
                //fout2 << "In cycle " << cycle << ": " << "Overwrite HI-LO registers" << endl;
                no_double = 1;
            }
            if(convert_bin(reg[33]) != convert_bin(temp_LO) && no_double == 0)
                error_2 = 1;
                //fout2 << "In cycle " << cycle << ": " << "Overwrite HI-LO registers" << endl;
        }
        rt_buf_H = reg[32];
        rt_buf_L = reg[33];
        if(convert_bin(reg[32]) != convert_bin(temp_HI))
            write_reg = 1;
           // fout1 << "$HI: " << convert_hex(reg[32]) << endl;
        if(convert_bin(reg[33]) != convert_bin(temp_LO))
        {
            if(write_reg == 1)
                write_reg = 3;
            else
                write_reg = 2;
        }
            //fout1 << "$LO: " << convert_hex(reg[33]) << endl;
        //cout << "temp_HI = " << temp_HI << endl;
        safe_H = 1;
        safe_L = 1;
        reg[34] = reg[34] + 4;
        fout1 << "PC: " << convert_hex(reg[34]) << endl;
    }
    else if (wb_inf == 2)
    {
        if(write_reg == 1)
            fout1 << "$HI: " << convert_hex(rt_buf_H) << endl;
        else if(write_reg == 2)
            fout1 << "$LO: " << convert_hex(rt_buf_L) << endl;
        else if(write_reg == 3)
        {
            fout1 << "$HI: " << convert_hex(rt_buf_H) << endl;
            fout1 << "$LO: " << convert_hex(rt_buf_L) << endl;
        }
        if(error_1 == 1)
            fout2 << "In cycle " << cycle << ": " << "Number Overflow" << endl;
        if(error_2 == 1)
            fout2 << "In cycle " << cycle << ": " << "Overwrite HI-LO registers" << endl;
    }
    return 0;
}
int ins_mfhi(string rd, int wb_inf , long long &rt_buf, int &error_1, int &error_2 , int &write_reg)
{
    if(wb_inf == 0)
    {
        int t1 = convert_dex(rd);
        long long temp = reg[t1];
        //fout1 << "cycle " << cycle << endl;
        reg[t1] = reg[32];
        //reg[32] = 0;
        safe_H = 0;
        if(t1 == 0)
        {
            error_1 = 1;
            //fout2 << "In cycle " << cycle << ": " << "Write $0 Error" << endl;
            reg[t1] = 0;
        }
        rt_buf = reg[34];
        if(convert_bin(temp) != convert_bin(reg[t1]))
        {
            write_reg = 1;
        }
        reg[34] = reg[34] + 4;
        fout1 << "PC: " << convert_hex(reg[34]) << endl;
    }
    else
    {
        int t1 = convert_dex(rd);
        if(error_1 == 1)
            fout2 << "In cycle " << cycle << ": " << "Write $0 Error" << endl;
        if(error_2 == 1)
            fout2 << "In cycle " << cycle << ": " << "Number Overflow" << endl;
        if(write_reg == 1)
        {
                if(t1 < 10)
                    fout1 << "$0" << t1 << ": " << convert_hex(rt_buf) << endl;
                else
                    fout1 << "$"<<t1 << ": " << convert_hex(rt_buf) << endl;
        }

    }
    return 0;
}
int ins_mflo(string rd, int wb_inf , long long &rt_buf, int &error_1, int &error_2 , int &write_reg)
{
    if(wb_inf == 0)
    {
        int t1 = convert_dex(rd);
        long long temp = reg[t1];
        //fout1 << "cycle " << cycle << endl;
        reg[t1] = reg[33];
        //reg[33] = 0;
        safe_L = 0;
        if(t1 == 0)
        {
            error_1 = 1;
            //fout2 << "In cycle " << cycle << ": " << "Write $0 Error" << endl;
            reg[t1] = 0;
        }
        rt_buf = reg[t1];
        if(convert_bin(temp) != convert_bin(reg[t1]))
        {
            write_reg = 1;
        }
        reg[34] = reg[34] + 4;
        fout1 << "PC: " << convert_hex(reg[34]) << endl;
    }
    else
    {
        int t1 = convert_dex(rd);
        if(error_1 == 1)
            fout2 << "In cycle " << cycle << ": " << "Write $0 Error" << endl;
        if(error_2 == 1)
            fout2 << "In cycle " << cycle << ": " << "Number Overflow" << endl;
        if(write_reg == 1)
        {
            if(t1 < 10)
                fout1 << "$0" << t1 << ": " << convert_hex(rt_buf) << endl;
            else
                fout1 << "$"<<t1 << ": " << convert_hex(rt_buf) << endl;
        }

    }
    return 0;
}
//*****************************************I Type
int ins_addi(string rs , string rt , string imm, int wb_inf , long long &rt_buf, int &error_1, int &error_2 , int &write_reg)
{
    if(wb_inf == 0)
    {
        int booling = 0;
        int t1 = convert_dex(rs);
        int t2 = convert_dex(rt);
        long long temp_1 = 0;
        if(reg[t1] > 2147483647)
            temp_1 = int32_t(reg[t1]);
        else if(reg[t1] < int(-2147483648))
            temp_1 = (reg[t1]) & 2147483647;
        else
            temp_1 = reg[t1];
        //fout1 << "cycle " << cycle << endl;
        int t3 = atoi(imm.c_str());
        int temp_sign = 0;
        long long temp = reg[t2];
        reg[t2] = (temp_1) + t3;

        if(t2 == 0)
        {
            error_1 = 1;
            //fout2 << "In cycle " << cycle << ": " << "Write $0 Error" << endl;
            booling = 1;
        }
        int no_double = 0;
        if(reg[t2] > 2147483647 || reg[t2] <int(-2147483648))
        {
            error_2 =1;
            //fout2 << "In cycle " << cycle << ": " << "Number Overflow" << endl;
            no_double = 1;
        }
        if((reg[t1] < 0 && t3 < 0 && reg[t2] > 0) ||(reg[t1] > 0 && t3 > 0 && reg[t2] < 0))
        {
            if(no_double == 0)
                error_2 = 1;
                //fout2 << "In cycle " << cycle << ": " << "Number Overflow" << endl;
        }
        if(booling == 1)
            reg[t2] = 0;
        rt_buf = reg[t2];
        if(convert_bin(reg[t2]) != convert_bin(temp))
        {
            write_reg = 1;
        }
        reg[34] = reg[34] + 4;
        fout1 << "PC: " << convert_hex(reg[34]) << endl;
    }
    else
    {
        int t2 = convert_dex(rt);
        if(error_1 == 1)
            fout2 << "In cycle " << cycle << ": " << "Write $0 Error" << endl;
        if(error_2 == 1)
            fout2 << "In cycle " << cycle << ": " << "Number Overflow" << endl;
        if(write_reg == 1)
        {
            if(t2 < 10)
                fout1 << "$0" << t2 << ": " << convert_hex(rt_buf) << endl;
            else
                fout1 << "$"<<t2 << ": " << convert_hex(rt_buf) << endl;
        }

    }

    return 0;

}
int ins_addiu(string rs , string rt , string imm, int wb_inf , long long &rt_buf, int &error_1, int &error_2 , int &write_reg)
{
    if(wb_inf == 0)
    {
        int t1 = convert_dex(rs);
        int t2 = convert_dex(rt);
        //cout << "imm = " << imm << endl;
        int t3 = atoi(imm.c_str());
        //cout << "t3 = " << t3 << endl;
        unsigned int temp_num_1 = 0;
        unsigned int temp_num_2 = 0;
        long long temp = reg[t2];
        //fout1 << "cycle " << cycle << endl;
        reg[t2] = int32_t(reg[t1]) + int32_t(t3) ;
        if(t2 == 0)
        {
            error_1 = 1;
            //fout2 << "In cycle " << cycle << ": " << "Write $0 Error" << endl;
            reg[t2] = 0;
        }
        if(reg[t2] > 4294967295)
        {
            reg[t2] = reg[t2] & 4294967295;
        }
        string ttemp = "";
        ttemp = convert_bin(reg[t2]);
        if(ttemp[16] == '1')
        {
            reg[t2] = reg[t2] | 4294901760;
        }
        if(reg[t2] > 4294967295)
        {
            reg[t2] = reg[t2] & 4294967295;
        }
        //cout << "temp = " << temp << endl;
        //cout << "reg[t2] = " << reg[t2] << endl;
        rt_buf = reg[t2];

        if(reg[t2] != temp)
        {
            write_reg = 1;
        }
        //cout << "write_reg = " << write_reg << endl;
        reg[34] = reg[34] + 4;
        fout1 << "PC: " << convert_hex(reg[34]) << endl;
    }
    else
    {
        int t2 = convert_dex(rt);
        if(error_1 == 1)
            fout2 << "In cycle " << cycle << ": " << "Write $0 Error" << endl;
        if(error_2 == 1)
            fout2 << "In cycle " << cycle << ": " << "Number Overflow" << endl;
        if(write_reg == 1)
        {
            if(t2 < 10)
                fout1 << "$0" << t2 << ": " << convert_hex(rt_buf) << endl;
            else
                fout1 << "$"<<t2 << ": " << convert_hex(rt_buf) << endl;
        }

    }

    return 0;
}
int ins_lw(string rs , string rt , string imm, int wb_inf , long long &rt_buf , int &error_1, int &error_2 , int &write_reg)
{
    if(wb_inf == 0)
    {
        int booling = 0;
        long long cover = 0;
        int close = 0;
        int t1 = convert_dex(rs);
        int t2 = convert_dex(rt);
        int t3 = atoi(imm.c_str());//if=4(remember imm is dexc)
        int temp_sign = 0;
        long long add_num = 0;
        long long temp_1 = 0;
        long long temp = reg[t2];
        //cout << reg[t2] << endl;
        //cout << reg[t1] << endl;
        if(reg[t1] > 2147483647)
            temp_1 = int32_t(reg[t1]);
        else if(reg[t1] < int(-2147483648))
            temp_1 = (reg[t1]) & 2147483647;
        else
            temp_1 = reg[t1];
        add_num = temp_1+(t3);
        cover = add_num;
        //cout << reg[t1] << endl;
        //cout << imm << endl;
        //cout << add_num << endl;
        int no_double = 0;
        if(t2 == 0){
            error_1 = 1;
            //fout2 << "In cycle " << cycle << ": " << "Write $0 Error" << endl;
            booling = 1;
        }
        if(add_num > 2147483647 || add_num < int(-2147483648))
        {
            error_2 = 1;
            //fout2 << "In cycle " << cycle << ": " << "Number Overflow" << endl;
            no_double = 1;
        }
        if((reg[t1] > 0 && t3 > 0 && add_num < 0 )|| (reg[t1] < 0 && t3 < 0 && add_num > 0))
        {
            if(no_double == 0)
                error_2 = 1;
            //fout2 << "In cycle " << cycle << ": " << "Number Overflow" << endl;
        }
        if(add_num > 1020 || add_num < 0)
        {
            fout2 << "In cycle " << cycle+1 << ": " << "Address Overflow" << endl;
            add_num = 0;
            close = 1;
        }
        int start_area = add_num/4;
        int start_word = add_num%4;
        string str_temp = d_mem[start_area+2];
        string str_real = "";
        for(int i = (start_word*8) ; i < ((start_word*8)+32) ; i++)
            str_real += str_temp[i];
        reg[t2] = convert_dex(str_real);
        //cout << imm << endl;
        if(cover % 4 != 0)
        {
            fout2 << "In cycle " << cycle+1 << ": " << "Misalignment Error" << endl;
            close = 1;
        }
        if(close == 1)
            return -1;
        if(booling == 1)
            reg[t2] = 0;
        //fout1 << "cycle " << cycle << endl;
        //cout << "t2 = " << t2 << endl;
        //cout << "reg[t2] = " << reg[t2] << endl;
        rt_buf = reg[t2];
        if(convert_bin(reg[t2]) != convert_bin(temp))
        {
            write_reg = 1;

        }
        //reg[34] = reg[34] + 4;
        //fout1 << "PC: " << convert_hex(reg[34]) << endl;
    }
    else
    {
        int t2 = convert_dex(rt);
        if(error_1 == 1)
            fout2 << "In cycle " << cycle << ": " << "Write $0 Error" << endl;
        if(error_2 == 1)
            fout2 << "In cycle " << cycle << ": " << "Number Overflow" << endl;
        if(write_reg == 1)
        {
            if(t2 < 10)
                fout1 << "$0" << t2 << ": " << convert_hex(rt_buf) << endl;
            else
                fout1 << "$"<<t2 << ": " << convert_hex(rt_buf) << endl;
        }

    }
    //system("PAUSE");
    return 0;
}
int ins_lh(string rs , string rt , string imm, int wb_inf, long long &rt_buf , int &error_1, int &error_2 , int &write_reg)
{
    if(wb_inf == 0)
    {
        int booling =0;
        long long cover =0;
        int close = 0;
        int t1 = convert_dex(rs);
        int t2 = convert_dex(rt);
        int t3 = atoi(imm.c_str());
        int temp_sign = 0;
        long long add_num = 0;
        long long temp = reg[t2];
        long long temp_1 = 0;
        if(reg[t1] > 2147483647)
            temp_1 = int32_t(reg[t1]);
        else if(reg[t1] < int(-2147483648))
            temp_1 = (reg[t1]) & 2147483647;
        else
            temp_1 = reg[t1];
        if(t2 == 0){
            error_1 = 1;
            //fout2 << "In cycle " << cycle << ": " << "Write $0 Error" << endl;
            booling = 1;
        }
        add_num = temp_1+(t3);
        cover = add_num;
        int no_double = 0;
        if(add_num > 2147483647 || add_num < int(-2147483648))
        {
            error_2 = 1;
            //fout2 << "In cycle " << cycle << ": " << "Number Overflow" << endl;
            no_double = 1;
        }
        if((reg[t1] > 0 && t3 > 0 && add_num < 0 )|| (reg[t1] < 0 && t3 < 0 && add_num > 0))
        {
            if(no_double == 0)
                error_2 = 1;
            //fout2 << "In cycle " << cycle << ": " << "Number Overflow" << endl;
        }
        if(add_num > 1022 || add_num < 0)
        {
            fout2 << "In cycle " << cycle+1 << ": " << "Address Overflow" << endl;
            add_num = 0;
            close = 1;
        }
        int start_area = add_num/4;
        int start_word = add_num%4;
        string str_temp = d_mem[start_area+2];
        string str_real = "";
        int count = 0;
        if(cover % 2 != 0)
        {
            fout2 << "In cycle " << cycle+1 << ": " << "Misalignment Error" << endl;
            close = 1;
        }
        for(int i = (start_word*8) ; i < ((start_word*8)+16) ; i++)
        {
            if(str_temp[start_word*8] == '1')
            {
                if(count < 16)
                {
                    str_real += "1";
                    i = (start_word*8)-1;
                    count ++ ;
                }
                else
                {
                    str_real += str_temp[i];
                }
            }
            else if(str_temp[start_word*8] == '0')
            {
                if(count < 16)
                {
                    str_real += "0";
                    i = (start_word*8)-1;
                    count ++ ;
                }
                else
                {
                    str_real += str_temp[i];
                }
            }
        }
        reg[t2] = convert_dex(str_real);
        if(close == 1)
            return -1;
        if(booling == 1)
            reg[t2] = 0;
        rt_buf = reg[t2];
        //fout1 << "cycle " << cycle << endl;
        if(convert_bin(reg[t2]) != convert_bin(temp))
        {
            write_reg = 1;
        }
        //reg[34] = reg[34] + 4;
        //fout1 << "PC: " << convert_hex(reg[34]) << endl;
    }
    else
    {
        int t2 = convert_dex(rt);
        if(error_1 == 1)
            fout2 << "In cycle " << cycle << ": " << "Write $0 Error" << endl;
        if(error_2 == 1)
            fout2 << "In cycle " << cycle << ": " << "Number Overflow" << endl;
        if(write_reg == 1)
        {
            if(t2 < 10)
                fout1 << "$0" << t2 << ": " << convert_hex(rt_buf) << endl;
            else
                fout1 << "$"<<t2 << ": " << convert_hex(rt_buf) << endl;
        }
    }

    return 0;
}
int ins_lb(string rs , string rt , string imm, int wb_inf, long long &rt_buf , int &error_1, int &error_2 , int &write_reg)
{
    if(wb_inf == 0)
    {
        int booling = 0;
        int close = 0;
        int t1 = convert_dex(rs);
        int t2 = convert_dex(rt);
        int t3 = atoi(imm.c_str());;
        //fout1 << "t3"<< t3 << endl;
        long long add_num =0;
        int temp_sign=0;
        long long temp = reg[t2];
        long long temp_1 = 0;
        if(reg[t1] > 2147483647)
            temp_1 = int32_t(reg[t1]);
        else if(reg[t1] < int(-2147483648))
            temp_1 = (reg[t1]) & 2147483647;
        else
            temp_1 = reg[t1];
        if(t2 == 0)
        {
            error_1 = 1;
            //fout2 << "In cycle " << cycle << ": " << "Write $0 Error" << endl;
            booling = 1;
        }
        add_num = temp_1+(t3);
        int no_double = 0;
        if(add_num > 2147483647 || add_num < int(-2147483648))
        {
            error_2 = 1;
            //fout2 << "In cycle " << cycle << ": " << "Number Overflow" << endl;
            no_double = 1;
        }
        if((reg[t1] > 0 && t3 > 0 && add_num < 0 )|| (reg[t1] < 0 && t3 < 0 && add_num > 0))
        {
            if(no_double == 0)
                error_2 = 1;
                //fout2 << "In cycle " << cycle << ": " << "Number Overflow" << endl;
        }
        if(add_num > 1023 || add_num < 0)
        {
            fout2 << "In cycle " << cycle+1 << ": " << "Address Overflow" << endl;
            add_num = 0;
            close = 1;
        }
        int start_area = add_num/4;
        int start_word = add_num%4;
        string str_temp = d_mem[start_area+2];
        string str_real = "";
        int count = 0;
        for(int i = (start_word*8) ; i < ((start_word*8)+8) ; i++)
        {
            if(str_temp[start_word*8] == '1')
            {
                if(count < 24)
                {
                    str_real += "1";
                    i = (start_word*8)-1;
                    count ++ ;
                }
                else
                {
                    str_real += str_temp[i];
                }
            }
            else if(str_temp[start_word*8] == '0')
            {
                if(count < 24)
                {
                    str_real += "0";
                    i = (start_word*8)-1;
                    count ++ ;
                }
                else
                {
                    str_real += str_temp[i];
                }
            }
        }
        reg[t2] = convert_dex(str_real);
        if(close == 1)
            return -1;
        if(booling == 1)
            reg[t2] = 0;
        rt_buf = reg[t2];
        //fout1 << "cycle " << cycle << endl;
        if(convert_bin(reg[t2]) != convert_bin(temp))
        {
            write_reg = 1;
        }
        //reg[34] = reg[34] + 4;
        //fout1 << "PC: " << convert_hex(reg[34]) << endl;
        return 0;
    }
    else
    {
        int t2 = convert_dex(rt);
        if(error_1 == 1)
            fout2 << "In cycle " << cycle << ": " << "Write $0 Error" << endl;
        if(error_2 == 1)
            fout2 << "In cycle " << cycle << ": " << "Number Overflow" << endl;
        if(write_reg == 1)
        {
            if(t2 < 10)
                fout1 << "$0" << t2 << ": " << convert_hex(rt_buf) << endl;
            else
                fout1 << "$"<<t2 << ": " << convert_hex(rt_buf) << endl;
        }

    }

}
int ins_lhu(string rs , string rt , string imm, int wb_inf, long long &rt_buf , int &error_1, int &error_2 , int &write_reg)//+16-1(imm = 16 bits)
{
    if(wb_inf == 0)
    {
        int booling =0;
        long long cover = 0;
        int close = 0;
        int t1 = convert_dex(rs);
        int t2 = convert_dex(rt);
        int t3 = atoi(imm.c_str());;
        //cout << reg[t1] << endl;
        //cout << imm << endl;
        long long temp = reg[t2];
        long long add_num = reg[t1] + t3;
        cover = add_num;
        if(t2 == 0){
            error_1 = 1;
            //fout2 << "In cycle " << cycle << ": " << "Write $0 Error" << endl;
            booling = 1;
        }
        if(add_num > 2147483647)
        {
            error_2 = 1;
            //fout2 << "In cycle " << cycle << ": " << "Number Overflow" << endl;
        }
        if(add_num > 1022 || add_num < 0)
        {
            error_2 = 1;
            //fout2 << "In cycle " << cycle << ": " << "Address Overflow" << endl;
            add_num = 0;
            close = 1;
        }
        int start_area = add_num/4;
        int start_word = add_num%4;
        string str_temp = d_mem[start_area+2];
        string str_real = "";
        int count = 0;
        if(cover % 2 != 0)
        {
            fout2 << "In cycle " << cycle+1 << ": " << "Misalignment Error" << endl;
            close = 1;
        }
        for(int i = (start_word*8) ; i < ((start_word*8)+16) ; i++)
        {
            if(count < 16)
            {
                str_real += "0";
                i = (start_word*8)-1;
                count ++ ;
            }
            else
            {
                str_real += str_temp[i];
            }
        }
        reg[t2] = convert_dex(str_real);
        if(close == 1)
            return -1;
        if(booling == 1)
            reg[t2] = 0;
        rt_buf = reg[t2];
        //fout1 << "cycle " << cycle << endl;
        if(convert_bin(reg[t2]) != convert_bin(temp))
        {
            write_reg == 1;
        }
        //reg[34] = reg[34] + 4;
        //fout1 << "PC: " << convert_hex(reg[34]) << endl;
    }
    else
    {
        int t2 = convert_dex(rt);
        if(error_1 == 1)
            fout2 << "In cycle " << cycle << ": " << "Write $0 Error" << endl;
        if(error_2 == 1)
            fout2 << "In cycle " << cycle << ": " << "Number Overflow" << endl;
        if(write_reg == 1)
        {
            if(t2 < 10)
                fout1 << "$0" << t2 << ": " << convert_hex(rt_buf) << endl;
            else
                fout1 << "$"<<t2 << ": " << convert_hex(rt_buf) << endl;
        }


    }


    return 0;
}
int ins_lbu(string rs , string rt , string imm, int wb_inf, long long &rt_buf , int &error_1, int &error_2 , int &write_reg)//+16-1(imm = 16 bits)
{
    if(wb_inf == 0)
    {
        int booling = 0;
        int close = 0;
        int t1 = convert_dex(rs);
        int t2 = convert_dex(rt);
        int t3 = atoi(imm.c_str());;
        long long temp = reg[t2];
        long long add_num = reg[t1] + t3;
        if(t2 == 0)
        {
            error_1 = 1;
            //fout2 << "In cycle " << cycle << ": " << "Write $0 Error" << endl;
            booling =1 ;
        }
        if(add_num > 2147483647)
        {
            error_2 = 1;
            //fout2 << "In cycle " << cycle << ": " << "Number Overflow" << endl;
        }
        if(add_num > 1023 || add_num < 0)
        {
            error_2 = 1;
            //fout2 << "In cycle " << cycle << ": " << "Address Overflow" << endl;
            add_num = 0;
            close = 1;
        }
        int start_area = add_num/4;
        int start_word = add_num%4;
        string str_temp = d_mem[start_area+2];
        string str_real = "";
        int count = 0;
        for(int i = (start_word*8) ; i < ((start_word*8)+8) ; i++)
        {
            if(count < 24)
            {
                str_real += "0";
                i = (start_word*8)-1;
                count ++ ;
            }
            else
            {
                str_real += str_temp[i];
            }
        }
        reg[t2] = convert_dex(str_real);
        if(close == 1)
            return -1;
        if(booling == 1)
            reg[t2] = 0;
        rt_buf = reg[t2];
        //ghfout1 << "cycle " << cycle << endl;
        if(convert_bin(reg[t2]) != convert_bin(temp))
        {
            write_reg = 1;
        }
        //reg[34] = reg[34] + 4;
        //fout1 << "PC: " << convert_hex(reg[34]) << endl;
    }
    else
    {
        int t2 = convert_dex(rt);
        if(error_1 == 1)
            fout2 << "In cycle " << cycle << ": " << "Write $0 Error" << endl;
        if(error_2 == 1)
            fout2 << "In cycle " << cycle << ": " << "Number Overflow" << endl;
        if(write_reg == 1)
        {
            if(t2 < 10)
                fout1 << "$0" << t2 << ": " << convert_hex(rt_buf) << endl;
            else
                fout1 << "$"<<t2 << ": " << convert_hex(rt_buf) << endl;
        }

    }

    return 0;
}
int ins_sw(string rs , string rt , string imm, int wb_inf, int &error_1 , int &error_2, int &write_reg)
{
    if(wb_inf == 0)
    {
        int close = 0;
        long long cover = 0;
        int t1 = convert_dex(rs);
        int t2 = convert_dex(rt);
        int t3 = atoi(imm.c_str());;
        long long add_num =0;
        int temp_sign = 0;
        long long temp = reg[t2];
        long long temp_1 = 0;
        if(reg[t1] > 2147483647)
            temp_1 = int32_t(reg[t1]);
        else if(reg[t1] < int(-2147483648))
            temp_1 = (reg[t1]) & 2147483647;
        else
            temp_1 = reg[t1];
        //cout << "reg[t1] = " << reg[t1] <<endl;
        /*
        if(reg[t1] > 2147483647)
        {
            temp_sign = int32_t(reg[t1]);
            add_num = temp_sign+(imm);
        }
        else
            add_num = reg[t1]+(imm)*/
        add_num = temp_1+(t3);
        cover = add_num;
        //cout << add_num << endl;
        int no_double = 0;
        if(add_num > 2147483647 || add_num < int(-2147483648))
        {
            error_2 = 1;
            //fout2 << "In cycle " << cycle << ": " << "Number Overflow" << endl;
            no_double = 1;
        }
        if((reg[t1] > 0 && t3 > 0 && add_num < 0 )|| (reg[t1] < 0 && t3 < 0 && add_num > 0))
        {
            if(no_double == 0)
                error_2 = 1;
            //fout2 << "In cycle " << cycle << ": " << "Number Overflow" << endl;
        }
        if(add_num > 1020 || add_num < 0)
        {
            fout2 << "In cycle " << cycle+1 << ": " << "Address Overflow" << endl;
            close = 1;
        }
        if(cover % 4 != 0)
        {
            fout2 << "In cycle " << cycle+1 << ": " << "Misalignment Error" << endl;
            add_num = 0;
            close = 1;
        }
        int start_area = add_num/4;
        d_mem[start_area+2]=convert_bin(reg[t2]);
        if(close == 1)
            return -1;
        fout1 << "cycle " << cycle << endl;
        //reg[34] = reg[34] + 4;
        //fout1 << "PC: " << convert_hex(reg[34]) << endl;
    }
    else
    {
        if(error_1 == 1)
            fout2 << "In cycle " << cycle << ": " << "Write $0 Error" << endl;
        if(error_2 == 1)
            fout2 << "In cycle " << cycle << ": " << "Number Overflow" << endl;
    }

    return 0;
}
int ins_sh(string rs , string rt , string imm, int wb_inf, int &error_1, int &error_2, int &write_reg)
{
    if(wb_inf == 0)
    {
        int close = 0;
        long long cover = 0;
        int t1 = convert_dex(rs);
        int t2 = convert_dex(rt);
        int t3 = atoi(imm.c_str());;
        long long add_num = 0;
        int temp_sign = 0;
        long long temp = reg[t2];
        long long temp_a = 0;
        if(reg[t1] > 2147483647)
            temp_a = int32_t(reg[t1]);
        else if(reg[t1] < int(-2147483648))
            temp_a = (reg[t1]) & 2147483647;
        else
            temp_a = reg[t1];
        add_num = temp_a+(t3);
        cover = add_num;
        int no_double = 0;
        if(add_num > 2147483647 || add_num < int(-2147483648))
        {
            error_2 = 1;
            //fout2 << "In cycle " << cycle << ": " << "Number Overflow" << endl;
            no_double = 1;
        }
        if((reg[t1] > 0 && t3 > 0 && add_num < 0 )|| (reg[t1] < 0 && t3 < 0 && add_num > 0))
        {
            if(no_double == 0)
                error_2 = 1;
            //fout2 << "In cycle " << cycle << ": " << "Number Overflow" << endl;
        }
        if(add_num > 1022 || add_num < 0)
        {
            fout2 << "In cycle " << cycle+1 << ": " << "Address Overflow" << endl;
            add_num = 0;
            close = 1;
        }
        if(cover % 2 != 0)
        {
            fout2 << "In cycle " << cycle+1 << ": " << "Misalignment Error" << endl;
            close = 1;
        }
        if(close == 1)
            return -1;
        //fout1 << "cycle " << cycle << endl;
        int start_area = add_num/4;
        int start_word = add_num%4;
        d_mem[start_area+2];
        unsigned int temp_1 = 0;
        unsigned int temp_2 = 0;
        if(start_word == 0)
        {
            temp_1 = reg[t2]&65535;
            temp_1 = temp_1 << 16;
            temp_2 = convert_dex(d_mem[start_area+2])&65535;
            d_mem[start_area+2] = convert_bin(temp_1+temp_2);
        }
        else
        {
            temp_1 = reg[t2]&65535;//low
            temp_2 = convert_dex(d_mem[start_area+2])&4294901760;//(2^32-1)-(2^16-1) high
            d_mem[start_area+2] = convert_bin(temp_1+temp_2);
        }
        //reg[34] = reg[34] + 4;
        //fout1 << "PC: " << convert_hex(reg[34]) << endl;
    }
    else
    {
        if(error_1 == 1)
            fout2 << "In cycle " << cycle << ": " << "Write $0 Error" << endl;
        if(error_2 == 1)
            fout2 << "In cycle " << cycle << ": " << "Number Overflow" << endl;
    }

    return 0;
}
int ins_sb(string rs , string rt , string imm, int wb_inf, int &error_1, int &error_2, int &write_reg)
{
    if(wb_inf == 0)
    {
        int close = 0;
        int t1 = convert_dex(rs);
        int t2 = convert_dex(rt);
        int t3 = atoi(imm.c_str());;
        int temp_sign = 0;
        long long add_num = 0;
        long long temp = reg[t2];
        long long temp_a = 0;
        if(reg[t1] > 2147483647)
            temp_a = int32_t(reg[t1]);
        else if(reg[t1] < int(-2147483648))
            temp_a = (reg[t1]) & 2147483647;
        else
            temp_a = reg[t1];
        add_num = temp_a+t3;
        int no_double = 0;
        if(add_num > 2147483647 || add_num < int(-2147483648))
        {
            error_2 = 1;
            //fout2 << "In cycle " << cycle << ": " << "Number Overflow" << endl;
            no_double = 1;
        }
        if((reg[t1] > 0 && t3 > 0 && add_num < 0 )|| (reg[t1] < 0 && t3 < 0 && add_num > 0))
        {
            if(no_double == 0)
                error_2 = 1;
                //fout2 << "In cycle " << cycle << ": " << "Number Overflow" << endl;
        }
        if(add_num > 1023 || add_num < 0)
        {
            fout2 << "In cycle " << cycle+1 << ": " << "Address Overflow" << endl;
            add_num = 0;
            close = 1;
        }
        if(close == 1)
            return -1;
        //fout1 << "cycle " << cycle << endl;
        int start_area = add_num/4;
        int start_word = add_num%4;
        d_mem[start_area+2];
        unsigned int temp_1 = 0;
        unsigned int temp_2 = 0;
        unsigned int temp_3 = 0;
        if(start_word == 0)
        {
            temp_1 = reg[t2]&255;//low
            temp_1 = temp_1 << 24;
            temp_2 = convert_dex(d_mem[start_area+2])&16777215;//(2^32-1)-(2^16-1) high
            d_mem[start_area+2] = convert_bin(temp_1+temp_2);
        }
        else if(start_word == 1)
        {
            temp_1 = reg[t2]&255;//low
            temp_1 = temp_1 << 16;
            temp_2 = convert_dex(d_mem[start_area+2])&4278190079;
            temp_3 = convert_dex(d_mem[start_area+2])&65535;
            d_mem[start_area+2] = convert_bin(temp_1+temp_2+temp_3);
        }
        else if(start_word == 2)
        {
            temp_1 = reg[t2]&255;//low
            temp_1 = temp_1 << 8;
            temp_2 = convert_dex(d_mem[start_area+2])&4294901760;//(2^32-1)-(2^16-1) high
            temp_3 = convert_dex(d_mem[start_area+2])&255;
            d_mem[start_area+2] = convert_bin(temp_1+temp_2+temp_3);
        }
        else
        {
            temp_1 = reg[t2]&255;//low
            temp_2 = convert_dex(d_mem[start_area+2])&4294967040;//(2^32-1)-(2^16-1) high
            d_mem[start_area+2] = convert_bin(temp_1+temp_2);
        }
        //reg[34] = reg[34] + 4;
        //fout1 << "PC: " << convert_hex(reg[34]) << endl;
    }
    else
    {
        if(error_1 == 1)
            fout2 << "In cycle " << cycle << ": " << "Write $0 Error" << endl;
        if(error_2 == 1)
            fout2 << "In cycle " << cycle << ": " << "Number Overflow" << endl;
    }

    return 0;
}
int ins_lui(string rt , string imm, int wb_inf, long long &rt_buf, int &error_1, int &error_2 , int &write_reg)
{
    if(wb_inf == 0)
    {
        int t2 = convert_dex(rt);
        int t3 = atoi(imm.c_str());
        long long temp = reg[t2];
        //fout1 << "cycle " << cycle << endl;
        reg[t2] = t3 << 16;
        if(t2 == 0)
        {
            error_1 = 1;
            //fout2 << "In cycle " << cycle << ": " << "Write $0 Error" << endl;
            reg[t2] = 0;
        }
        rt_buf = reg[t2];
        if(convert_bin(reg[t2]) != convert_bin(temp))
        {
            write_reg = 1;
        }
        reg[34] = reg[34] + 4;
        fout1 << "PC: " << convert_hex(reg[34]) << endl;
    }
    else
    {
        int t2 = convert_dex(rt);
        if(error_1 == 1)
            fout2 << "In cycle " << cycle << ": " << "Write $0 Error" << endl;
        if(error_2 == 1)
            fout2 << "In cycle " << cycle << ": " << "Number Overflow" << endl;
        if(write_reg == 1)
        {
            if(t2 < 10)
                fout1 << "$0" << t2 << ": " << convert_hex(rt_buf) << endl;
            else
                fout1 << "$"<<t2 << ": " << convert_hex(rt_buf) << endl;
        }

    }

    return 0;
}
int ins_andi(string rs , string rt , string imm, int wb_inf, long long &rt_buf, int &error_1, int &error_2 , int &write_reg)
{
    if(wb_inf == 0)
    {
        int t1 = convert_dex(rs);
        int t2 = convert_dex(rt);
        int t3 = atoi(imm.c_str());;
        long long temp = reg[t2];
        //fout1 << "cycle " << cycle << endl;
        if(t2 == 0)
        {
            error_1 = 1;
            //fout2 << "In cycle " << cycle << ": " << "Write $0 Error" << endl;
            reg[t2] = 0;
        }
        if(t3 < 0)
        {
            unsigned long long  j = (unsigned long long ) t3;
            j = j & 4294967295;
            t3 = j;
        }
        reg[t2] = reg[t1]&t3;
        rt_buf = reg[t2];
        if(convert_bin(reg[t2]) != convert_bin(temp))
        {
            write_reg = 1;
        }
        reg[34] = reg[34] + 4;
        fout1 << "PC: " << convert_hex(reg[34]) << endl;
    }
    else
    {
        int t2 = convert_dex(rt);
        if(error_1 == 1)
            fout2 << "In cycle " << cycle << ": " << "Write $0 Error" << endl;
        if(error_2 == 1)
            fout2 << "In cycle " << cycle << ": " << "Number Overflow" << endl;
        if(write_reg == 1)
        {
            if(t2 < 10)
                fout1 << "$0" << t2 << ": " << convert_hex(rt_buf) << endl;
            else
                fout1 << "$"<<t2 << ": " << convert_hex(rt_buf) << endl;
        }

    }

    return 0;
}
int ins_ori(string rs , string rt , string imm, int wb_inf , long long &rt_buf, int &error_1, int &error_2 , int &write_reg)
{
    if(wb_inf == 0)
    {
        int t1 = convert_dex(rs);
        int t2 = convert_dex(rt);
        int t3 = atoi(imm.c_str());;
        long long temp = reg[t2];
        //fout1 << "cycle " << cycle << endl;
        if(t3 < 0)
        {
            unsigned long long  j = (unsigned long long ) t3;
            j = j & 4294967295;
            t3 = j;
        }
        reg[t2] = reg[t1]|t3;
        if(t2 == 0)
        {
            error_1 = 1;
            //fout2 << "In cycle " << cycle << ": " << "Write $0 Error" << endl;
            reg[t2] = 0;
        }
        rt_buf = reg[t2];
        if(convert_bin(reg[t2]) != convert_bin(temp))
        {
            write_reg = 1;
        }
        reg[34] = reg[34] + 4;
        fout1 << "PC: " << convert_hex(reg[34]) << endl;
    }
    else
    {
        int t2 = convert_dex(rt);
        if(error_1 == 1)
            fout2 << "In cycle " << cycle << ": " << "Write $0 Error" << endl;
        if(error_2 == 1)
            fout2 << "In cycle " << cycle << ": " << "Number Overflow" << endl;
        if(write_reg == 1)
        {
            if(t2 < 10)
                fout1 << "$0" << t2 << ": " << convert_hex(rt_buf) << endl;
            else
                fout1 << "$"<<t2 << ": " << convert_hex(rt_buf) << endl;
        }

    }

    return 0;
}
int ins_nori(string rs , string rt , string imm, int wb_inf, long long &rt_buf, int &error_1, int &error_2 , int &write_reg)
{
    if(wb_inf == 0)
    {
        int t1 = convert_dex(rs);
        int t2 = convert_dex(rt);
        int t3 = atoi(imm.c_str());;
        long long temp = reg[t2];
        //fout1 << "cycle " << cycle << endl;
        if(t3 < 0)
        {
            unsigned long long  j = (unsigned long long ) t3;
            j = j & 4294967295;
            t3 = j;
        }
        reg[t2] = ~(reg[t1]|t3);
        if(t2 == 0)
        {
            error_1 = 1;
            //fout2 << "In cycle " << cycle << ": " << "Write $0 Error" << endl;
            reg[t2] = 0;
        }
        rt_buf = reg[t2];
        if(convert_bin(reg[t2]) != convert_bin(temp))
        {
            write_reg = 1;
        }
        reg[34] = reg[34] + 4;
        fout1 << "PC: " << convert_hex(reg[34]) << endl;
    }
    else
    {
        int t2 = convert_dex(rt);
        if(error_1 == 1)
            fout2 << "In cycle " << cycle << ": " << "Write $0 Error" << endl;
        if(error_2 == 1)
            fout2 << "In cycle " << cycle << ": " << "Number Overflow" << endl;
        if(write_reg == 1)
        {
            if(t2 < 10)
                fout1 << "$0" << t2 << ": " << convert_hex(rt_buf) << endl;
            else
                fout1 << "$"<<t2 << ": " << convert_hex(rt_buf) << endl;
        }

    }

    return 0;
}
int ins_slti(string rs , string rt , string imm, int wb_inf, long long &rt_buf, int &error_1, int &error_2 , int &write_reg)
{
    if(wb_inf == 0)
    {
        int t1 = convert_dex(rs);
        int t2 = convert_dex(rt);
        int t3 = atoi(imm.c_str());;
        long long temp = reg[t2];
        //fout1 << "cycle " << cycle << endl;
        long long temp_1 = 0;
        if(reg[t1] > 2147483647) //from overflow error
            temp_1 = int32_t(reg[t1]);
        else if(reg[t1] < int(-2147483648))
            temp_1 = (reg[t1]) & 2147483647;
        else
            temp_1 = reg[t1];
        if(temp_1 < t3) // <X -> 1
            reg[t2] = 1;
        else
            reg[t2] = 0;
        if(t2 == 0)
        {
            error_1 = 1;
            //fout2 << "In cycle " << cycle << ": " << "Write $0 Error" << endl;
            reg[t2] = 0;
        }
        rt_buf = reg[t2];
        if(reg[t2] != temp)
        {
            write_reg = 1 ;
        }
        reg[34] = reg[34] + 4;
        fout1 << "PC: " << convert_hex(reg[34]) << endl;
    }
    else
    {
        int t2 = convert_dex(rt);
        if(error_1 == 1)
            fout2 << "In cycle " << cycle << ": " << "Write $0 Error" << endl;
        if(error_2 == 1)
            fout2 << "In cycle " << cycle << ": " << "Number Overflow" << endl;
        if(write_reg == 1)
        {
            if(t2 < 10)
                fout1 << "$0" << t2 << ": " << convert_hex(rt_buf) << endl;
            else
                fout1 << "$"<<t2 << ": " << convert_hex(rt_buf) << endl;
        }

    }

    return 0;
}
int ins_beq(string rs , string rt , string imm, int wb_inf) // ==
{
    if(wb_inf == 0)
    {
        int t1 = convert_dex(rs);
        int t2 = convert_dex(rt);
        int t3 = atoi(imm.c_str());;
        long long temp = reg[t2];
        //fout1 << "cycle " << cycle << endl;
        long long temp_1 = 0;
        long long temp_2 = 0;
        //cout << "reg[t1] = " << reg[t1] <<endl;
        //cout << "reg[t2] = " << reg[t2] <<endl;
        if(reg[t1] > 2147483647)
            temp_1 = int32_t(reg[t1]);
        else if(reg[t1] < int(-2147483648))
            temp_1 = (reg[t1]) & 2147483647;
        else
            temp_1 = reg[t1];
        if(reg[t2] > 2147483647)
            temp_2 = int32_t(reg[t2]);
        else if(reg[t2] < int(-2147483648))
            temp_2 = (reg[t2]) & 2147483647;
        else
            temp_2 = reg[t2];
        if(temp_1 == temp_2)
        {
            lock_num = 0;
            lock = 0;
        }
        else
        {
            lock = 1;
        }
        if(lock == 0)
        {
            reg[34] = reg[34] + 4 + (4*t3);
            fout1 << "PC: " << convert_hex(reg[34]) << endl;
            return (t3);
        }
        else
        {
            reg[34] = reg[34] + 4;
            fout1 << "PC: " << convert_hex(reg[34]) << endl;
        }
        if(lock == 1 && lock_num != 0)
            lock_num = 0;
        return 0;
    }
    else
    {
        return 0 ;
    }


}
int ins_bne(string rs , string rt , string imm, int wb_inf) // !=
{
    if(wb_inf == 0)
    {
        int t1 = convert_dex(rs);
        int t2 = convert_dex(rt);
        int t3 = atoi(imm.c_str());;
        long long temp = reg[t2];
        long long temp_1 =0;
        long long temp_2 = 0;
        if(reg[t1] > 2147483647)
            temp_1 = int32_t(reg[t1]);
        else if(reg[t1] < int(-2147483648))
            temp_1 = (reg[t1]) & 2147483647;
        else
            temp_1 = reg[t1];
        if(reg[t2] > 2147483647)
            temp_2 = int32_t(reg[t2]);
        else if(reg[t2] < int(-2147483648))
            temp_2 = (reg[t2]) & 2147483647;
        else
            temp_2 = reg[t2];
        //fout1 << "cycle " << cycle << endl;
        //fout1 << reg[t1] << endl;
        //fout1 << reg[t2] << endl;
        if(temp_1 != temp_2)
        {
            lock_num = 0;
            lock = 0;
        }
        else
        {
            lock = 1;
        }
        if(lock == 0)
        {
            reg[34] = reg[34] + 4 + (4*t3);
            fout1 << "PC: " << convert_hex(reg[34]) << endl;
            return (t3);
        }
        else
        {
            reg[34] = reg[34] + 4;
            fout1 << "PC: " << convert_hex(reg[34]) << endl;
        }
        if(lock == 1 && lock_num != 0)
            lock_num = 0;
        return 0;
    }
    else
    {
        return 0;
    }

}
int ins_bgtz(string rs, string imm, int wb_inf) // !=
{
    if(wb_inf == 0)
    {
        int t1 = convert_dex(rs);
        int t3 = atoi(imm.c_str());;
        long long temp_1 = 0;
        if(reg[t1] > 2147483647)
            temp_1 = int32_t(reg[t1]);
        else if(reg[t1] < int(-2147483648))
            temp_1 = (reg[t1]) & 2147483647;
        else
            temp_1 = reg[t1];
        //fout1 << "cycle " << cycle << endl;
        //cout << "reg[t1] = "<< reg[t1] << endl;
        if(temp_1 > 0)
        {
            lock_num = 0;
            lock = 0;
        }
        else
        {
            lock = 1;
        }
        //cout << lock << endl;
        if(lock == 0)
        {
            reg[34] = reg[34] + 4 + (4*t3);
            fout1 << "PC: " << convert_hex(reg[34]) << endl;
            return t3;
        }
        else
        {
            reg[34] = reg[34] + 4;
            fout1 << "PC: " << convert_hex(reg[34]) << endl;
        }
        if(lock == 1 && lock_num != 0)
            lock_num = 0;
        return 0;
    }
    else
    {
        return 0 ;
    }

}
int ins_j(string address, int wb_inf) //come back
{
    if(wb_inf == 0)
    {
        int t3 = atoi(address.c_str());
        int next_pos = 0;
        //fout1 << "cycle " << cycle << endl;
        next_pos = (int(reg[34])-t3)/4;
        reg[34] = t3;
        fout1 << "PC: " << convert_hex(reg[34]) << endl;
        return (next_pos);
    }
    else
    {
        return 0 ;
    }


}
int ins_jal(string address, int wb_inf) // come back
{
    if(wb_inf == 0)
    {
        int t3 = atoi(address.c_str());
        //fout1 << "cycle " << cycle << endl;
        int next_pos = 0;
        unsigned int temp = reg[31];
        reg[31] = reg[34]+4;
        reg[34] = t3;
        next_pos = (reg[34]-reg[31])/4;
        if(convert_bin(temp) != convert_bin(reg[31]))
            fout1 << "$31: " << convert_hex(reg[31]) << endl;
        fout1 << "PC: " << convert_hex(reg[34]) << endl;
        return (next_pos);
    }
    else
    {
        return 0;
    }

}
void no_ins()
{
    reg[34] = reg[34] + 4;
    fout1 << "PC: " << convert_hex(reg[34]) << endl;
}
string conver_to_decode(int label , string opcode = "" , string func = "")
{
    string type_r[17] = {"add" , "addu" , "sub" , "and" , "or" ,
                       "xor" , "nor" , "nand" , "slt" , "sll" ,
                       "srl" , "sra" , "jr" , "mult" , "multu" ,
                       "mfhi" , "mflo"};
    string r_fun[17] = {"100000" , "100001" , "100010" , "100100" , "100101" ,
                     "100110" , "100111" , "101000" , "101010" , "000000" ,
                     "000010" , "000011" , "001000" , "011000" , "011001",
                     "010000" , "010010"};

    string type_i[18] = {"addi" , "addiu" , "lw" , "lh" , "lhu" ,
                       "lb" , "lbu" , "sw" , "sh" , "sb" ,
                       "lui" , "andi" , "ori" , "nori" , "slti" ,
                       "beq" , "bne" , "bgtz"};
    string i_opcode[18] = {"001000" , "001001" , "100011" , "100001" , "100101",
                         "100000" , "100100" , "101011" , "101001" , "101000",
                         "001111" , "001100" , "001101" , "001110" , "001010",
                         "000100" , "000101" , "000111"};
    string type_j[2] = {"j", "jal"};
    string j_opcode[2] = {"000010" , "000011"};
    string type_s[1] = {"halt"};
    string s_opcode[1] = {"111111"};
    if(label == 1) // divide and conquer
    {
        for(int i = 0; i < 17 ;i++)
        {
            string op = r_fun[i];
            if(op == func)
                return type_r[i];
            if(i == 16)
                return "no instrument";
        }
    }
    else if(label == 2)
    {
        for(int i = 0; i < 18 ;i++)
        {
            string op = i_opcode[i];
            if(op == opcode)
                return type_i[i];
            if(i == 17)
                return "no instrument";
        }

    }
    else if(label == 3)
    {
        if(j_opcode[0] == opcode)
            return type_j[0];
        else if(j_opcode[1] == opcode)
            return type_j[1];
        else
            return "no instrument";
    }
    else
    {
        if(s_opcode[0] == opcode)
            return type_s[0];
        else
            return "no instrument";

    }

}
//new_ins[0] : new_ins bin code ; new_ins[1] : new_ins decode ; new_ins[2] : old_ins decode
void write_stage(string (&stage)[5] , string (&new_ins)[3] , string (&reg_table)[5][4] , long long (&result_buffer)[5][2], int (&error_table)[5][2] ,
                int (&write_table)[5] , int &stall_inf , int &pipe_line)
{
    fout1 << endl;
    fout1 << endl;
    fout1 << "cycle " << cycle << endl;
    //cout << "result_buffer[4][0] = " << result_buffer[4][0] << endl;
    if(stage[4] != "NOP")
    {
        //cout << "reg_table[4][2] = " << reg_table[4][2] << endl;
        if(stage[4] == "add")//(char *rs , char *rt , char *rd , int wb_inf , long long &rt_buf , int &error_1, int &error_2 , int &write_reg)
            ins_add("","",reg_table[4][2], 1 ,result_buffer[4][0] , error_table[4][0] , error_table[4][1] , write_table[4]);
        if(stage[4] == "addu")
            ins_addu("","",reg_table[4][2], 1 ,result_buffer[4][0] , error_table[4][0] , error_table[4][1] , write_table[4]);
        if(stage[4] == "sub")
            ins_sub("","",reg_table[4][2], 1 ,result_buffer[4][0] , error_table[4][0] , error_table[4][1] , write_table[4]);
        if(stage[4] == "and")
            ins_and("","",reg_table[4][2], 1 ,result_buffer[4][0] , error_table[4][0] , error_table[4][1] , write_table[4]);
        if(stage[4] == "or")
            ins_or("","",reg_table[4][2], 1 ,result_buffer[4][0] , error_table[4][0] , error_table[4][1] , write_table[4]);
        if(stage[4] == "xor")
            ins_xor("","",reg_table[4][2], 1 ,result_buffer[4][0] , error_table[4][0] , error_table[4][1] , write_table[4]);
        if(stage[4] == "nor")
            ins_nor("","",reg_table[4][2], 1 ,result_buffer[4][0] , error_table[4][0] , error_table[4][1] , write_table[4]);
        if(stage[4] == "nand")
            ins_nand("","",reg_table[4][2], 1 ,result_buffer[4][0] , error_table[4][0] , error_table[4][1] , write_table[4]);
        if(stage[4] == "slt")
            ins_slt("","",reg_table[4][2], 1 ,result_buffer[4][0] , error_table[4][0] , error_table[4][1] , write_table[4]);
        if(stage[4] == "sll")//(char *rt , char *rd , char *shamt, int wb_inf, long long &rt_buf, int &error_1, int &error_2 , int &write_reg)
            ins_sll("",reg_table[4][2],"", 1 ,result_buffer[4][0],error_table[4][0] , error_table[4][1] , write_table[4]);
        if(stage[4] == "srl")
            ins_srl("",reg_table[4][2],"", 1 ,result_buffer[4][0],error_table[4][0] , error_table[4][1] , write_table[4]);
        if(stage[4] == "sra")
            ins_sra("",reg_table[4][2],"", 1 ,result_buffer[4][0],error_table[4][0] , error_table[4][1] , write_table[4]);
        if(stage[4] == "jr")
            ins_jr("",1);
        if(stage[4] == "mult")//char *rs , char *rt, int wb_inf , long long &rt_buf_H ,long long &rt_buf_L , int &error_1, int &error_2 , int &write_reg
            ins_mult("",reg_table[4][1], 1 ,result_buffer[4][0],result_buffer[4][1],error_table[4][0],error_table[4][1],write_table[4]);
        if(stage[4] == "multu")
            ins_multu("",reg_table[4][1], 1 ,result_buffer[4][0],result_buffer[4][1],error_table[4][0],error_table[4][1],write_table[4]);
        if(stage[4] == "mfhi")//char *rd, int wb_inf , long long &rt_buf, int &error_1, int &error_2 , int &write_reg
            ins_mfhi(reg_table[4][2] ,1 ,result_buffer[4][0],error_table[4][0],error_table[4][1],write_table[4]);
        if(stage[4] == "mflo")
            ins_mflo(reg_table[4][2] ,1 ,result_buffer[4][0],error_table[4][0],error_table[4][1],write_table[4]);
        if(stage[4] == "addi")//char *rs , char *rt , int imm, int wb_inf , long long &rt_buf, int &error_1, int &error_2 , int &write_reg
            ins_addi("",reg_table[4][1],"",1,result_buffer[4][0],error_table[4][0],error_table[4][1],write_table[4]);
        if(stage[4] == "addiu")
            ins_addiu("",reg_table[4][1],"",1,result_buffer[4][0],error_table[4][0],error_table[4][1],write_table[4]);
        if(stage[4] == "lui")//char *rt , int imm, int wb_inf, long long &rt_buf, int &error_1, int &error_2 , int &write_reg
            ins_lui(reg_table[4][1],"",1,result_buffer[4][0],error_table[4][0],error_table[4][1],write_table[4]);
        if(stage[4] == "andi")
            ins_andi("",reg_table[4][1],"",1,result_buffer[4][0],error_table[4][0],error_table[4][1],write_table[4]);
        if(stage[4] == "ori")
            ins_ori("",reg_table[4][1],"",1,result_buffer[4][0],error_table[4][0],error_table[4][1],write_table[4]);
        if(stage[4] == "nori")
            ins_nori("",reg_table[4][1],"",1,result_buffer[4][0],error_table[4][0],error_table[4][1],write_table[4]);
        if(stage[4] == "slti")
            ins_slti("",reg_table[4][1],"",1,result_buffer[4][0],error_table[4][0],error_table[4][1],write_table[4]);
        if(stage[4] == "beq")
            ins_beq("" , "" , "" , 1);
        if(stage[4] == "bne")
            ins_bne("" , "" , "" , 1);
        if(stage[4] == "bgtz")
            ins_bgtz("" , "" , 1);
        if(stage[4] == "j")
            ins_j("", 1);
        if(stage[4] == "jal")
            ins_jal("", 1);
        if(stage[4] == "lw")
            ins_lw("",reg_table[4][1],"",1,result_buffer[4][0] , error_table[4][0], error_table[4][1] , write_table[4]);
        if(stage[4] == "lh")
            ins_lh("",reg_table[4][1],"",1,result_buffer[4][0] , error_table[4][0], error_table[4][1] , write_table[4]);
        if(stage[4] == "lhu")
            ins_lhu("",reg_table[4][1],"",1,result_buffer[4][0] , error_table[4][0], error_table[4][1] , write_table[4]);
        if(stage[4] == "lb")
            ins_lb("",reg_table[4][1],"",1,result_buffer[4][0] , error_table[4][0], error_table[4][1] , write_table[4]);
        if(stage[4] == "lbu")
            ins_lbu("",reg_table[4][1],"",1,result_buffer[4][0] , error_table[4][0], error_table[4][1] , write_table[4]);
        if(stage[4] == "sw")
            ins_sw("", "", "", 1, error_table[4][0] , error_table[4][1], write_table[4]);
        if(stage[4] == "sh")
            ins_sh("", "", "", 1, error_table[4][0] , error_table[4][1], write_table[4]);
        if(stage[4] == "sb")
            ins_sb("", "", "", 1, error_table[4][0] , error_table[4][1], write_table[4]);

    }
    if(stall_inf == 0)
    {
        pipline_change(stage);
        for(int i = 4 ; i >= 0 ; i-- )
        {
            if(i == 0)
            {
                reg_table[i][0] = "";
                reg_table[i][1] = "";
                reg_table[i][2] = "";
                reg_table[i][3] = "";
            }
            else
            {
                reg_table[i][0] = reg_table[i-1][0];
                reg_table[i][1] = reg_table[i-1][1];
                reg_table[i][2] = reg_table[i-1][2];
                reg_table[i][3] = reg_table[i-1][3];
            }

        }
        for(int i = 4 ; i >= 2 ; i--)
        {
            result_buffer[i][0] = result_buffer[i-1][0];
            result_buffer[i][1] = result_buffer[i-1][1];
        }
        for(int i = 4 ; i >= 2 ; i-- )
        {
            error_table[i][0] = error_table[i-1][0];
            error_table[i][1] = error_table[i-1][1];
        }
        for(int i = 4 ; i >= 2 ; i-- )
        {
            write_table[i] = write_table[i-1];
        }
    }
    else if(stall_inf == 1)
    {
        //cout << "STOP" << endl;
        //system("PAUSE");
        string EX_buf = stage[2];
        string MEM_buf = stage[3];
        string WB_buf = stage[4];
        stage[2] = "";
        stage[3] = "";
        stage[4] = "";
        stage[2] = "NOP";
        stage[3] = EX_buf;
        stage[4] = MEM_buf;
        for(int i = 4 ; i >= 3 ; i-- )
        {
            if(i == 0)
            {
                reg_table[i][0] = "";
                reg_table[i][1] = "";
                reg_table[i][2] = "";
                reg_table[i][3] = "";
            }
            else
            {
                reg_table[i][0] = reg_table[i-1][0];
                reg_table[i][1] = reg_table[i-1][1];
                reg_table[i][2] = reg_table[i-1][2];
                reg_table[i][3] = reg_table[i-1][3];
            }

        }
        for(int i = 4 ; i >= 3 ; i--)
        {
            result_buffer[i][0] = result_buffer[i-1][0];
            result_buffer[i][1] = result_buffer[i-1][1];
        }
        for(int i = 4 ; i >= 3 ; i-- )
        {
            error_table[i][0] = error_table[i-1][0];
            error_table[i][1] = error_table[i-1][1];
        }
        for(int i = 4 ; i >= 3 ; i-- )
        {
            write_table[i] = write_table[i-1];
        }
        pipe_line = pipe_line -1;
        stall_inf = 0;

    }
    else if(stall_inf == 2)
    {
        stage[0] = "";
        reg_table[0][0] = "";
        reg_table[0][1] = "";
        reg_table[0][2] = "";
        reg_table[0][3] = "";
        pipline_change(stage);
        for(int i = 4 ; i >= 0 ; i-- )
        {
            if(i == 0)
            {
                reg_table[i][0] = "";
                reg_table[i][1] = "";
                reg_table[i][2] = "";
                reg_table[i][3] = "";
            }
            else
            {
                reg_table[i][0] = reg_table[i-1][0];
                reg_table[i][1] = reg_table[i-1][1];
                reg_table[i][2] = reg_table[i-1][2];
                reg_table[i][3] = reg_table[i-1][3];
            }

        }
        for(int i = 4 ; i >= 2 ; i--)
        {
            result_buffer[i][0] = result_buffer[i-1][0];
            result_buffer[i][1] = result_buffer[i-1][1];
        }
        for(int i = 4 ; i >= 2 ; i-- )
        {
            error_table[i][0] = error_table[i-1][0];
            error_table[i][1] = error_table[i-1][1];
        }
        for(int i = 4 ; i >= 2 ; i-- )
        {
            write_table[i] = write_table[i-1];
        }
        stall_inf = 0;

    }

}
void five_stage(string (&stage)[5] , string (&new_ins)[3] , string (&reg_table)[5][4] , long long (&result_buffer)[5][2], int &stall_inf , int (&error_table)[5][2] ,
                int (&write_table)[5] , int &pipe_line)//pipe_line:the line of program
{
    //cout << "stage[2] = " << stage[2] << endl;
    string pipeline_messeger_s = "";
    string pipeline_messeger_t = "";
    string pipeline_messeger_branch_s = "";
    string pipeline_messeger_branch_t = "";
    //stage[0] == IF ; stage[1] == ID ; stage[2] == EX ; stage[3] == MEM ; stage[4] == WB
    stage[1] = "";
    stage[1] = new_ins[2];
    stage[0] = new_ins[0];
    int reg_flag = 0;
    int reg_flag_2 = 0;
    if(stage[1] == "addi" || stage[1] == "addiu" || stage[1] == "lui" || stage[1] == "andi" || stage[1] == "ori" ||
       stage[1] == "nori" || stage[1] == "slti"  || stage[1] == "lw"  || stage[1] == "lh"   || stage[1] == "lhu" ||
       stage[1] == "lb"   || stage[1] == "lbu"   || stage[1] == "sw"  || stage[1] == "sh"   || stage[1] == "sb" ||
       stage[1] == "bgtz" )
       reg_flag = 1; //one source - one dest
    else
       reg_flag = 2; //two source - one dest or zero dest mult/multu
    if(stage[2] == "addi" || stage[2] == "addiu" || stage[2] == "lui" || stage[2] == "andi" || stage[2] == "ori" ||
       stage[2] == "nori" || stage[2] == "slti"  || stage[2] == "lw"  || stage[2] == "lh"   || stage[2] == "lhu" ||
       stage[2] == "lb"   || stage[2] == "lbu"   || stage[2] == "sw"  || stage[2] == "sh"   || stage[2] == "sb"  ||
       stage[2] == "bgtz")
       reg_flag_2 = 1; //one source - one dest
    else
       reg_flag_2 = 2; //two source - one dest
    int pipe_forward_s = 0;
    int pipe_forward_t = 0;
    int pipe_forward_branch_s = 0;
    int pipe_forward_branch_t = 0;
    if(stage[1] != "NOP" && stage[1] != "HALT" && stage[1] != "beq" && stage[1] != "bne")
    {
        if((stage[2] == "NOP" && stage[3] == "NOP" && stage[4] == "NOP")||
           (stage[2] == "HALT" && stage[3] == "HALT" && stage[4] == "HALT")) // case1 initial
           {
               pipe_forward_s = 0;
               pipe_forward_t = 0;
           }

        else
        {
            if(reg_flag == 2) // rs + rt +rd
            {
                /*
                if(stage[3] != "NOP" && stage[3] != "beq" && stage[3] != "bne" && stage[3] != "bgtz" && stage[3] != "sw"  && stage[3] != "sh"   && stage[3] != "sb" &&stage[3]!= "HALT")//no write
                {

                    //stall detection
                    if(stage[3] == "addi" || stage[3] == "addiu" || stage[3] == "lui" || stage[3] == "andi" || stage[3] == "ori" ||
                       stage[3] == "nori" || stage[3] == "slti"  || stage[3] == "lw"  || stage[3] == "lh"   || stage[3] == "lhu" ||
                       stage[3] == "lb"   || stage[3] == "lbu"  ) // rs + rt
                    {
                        cout << "A" << endl;
                        cout << "reg_table[1][0] = " << reg_table[1][0] << endl;
                        cout << "reg_table[1][1] = " << reg_table[1][1] << endl;
                        cout << "reg_table[3][1] = " << reg_table[3][1] << endl;
                        //reg_table[][0] == rs reg_table[][1] == rt reg_table[][2] == rd ; if no reg == null
                        if((reg_table[1][0] == reg_table[3][1] && reg_table[1][0] != "" && reg_table[1][0] != "00000")  ||
                           (reg_table[1][1] == reg_table[3][1] && reg_table[1][1] != "" && reg_table[1][1] != "00000"))
                        {
                            stall_inf = 1;
                        }
                    }
                    else // rs + rt + rd
                    {cout << "B" << endl;
                        if((reg_table[1][0] == reg_table[3][2] &&  reg_table[1][0] != "" && reg_table[1][0] != "00000") ||
                           (reg_table[1][1] == reg_table[3][2] &&  reg_table[1][1] != "" && reg_table[1][1] != "00000"))
                        {
                            cout << "reg_table[1][0] = " << reg_table[1][0] << endl;
                            cout << "reg_table[2][2] = " << reg_table[2][2] << endl;
                            cout << "reg_table[1][1] = " << reg_table[1][1] << endl;
                            cout << "reg_table[2][2] = " << reg_table[2][2] << endl;
                            stall_inf = 1;
                        }
                    }

                }*/
                //cout << "stall_stage[1] = " << stage[1] << endl;
                //cout << "stall_stage[2] = " << stage[2] << endl;
                //cout << "reg_table[1][0] = " << reg_table[1][0] << endl;
                //cout << "reg_table[2][1] = " << reg_table[2][1] << endl;
                //cout << "reg_table[1][1] = " << reg_table[1][1] << endl;
                if(stage[2] == "lw" || stage[2] == "lh" || stage[2] == "lhu" || stage[2] == "lb" || stage[2] == "lbu")
                {
                    //cout << "C" << endl;
                    if((reg_table[1][0] == reg_table[2][1] && reg_table[1][0] != "" && reg_table[1][0] != "00000") ||
                       (reg_table[1][1] == reg_table[2][1] && reg_table[1][1] != "" && reg_table[1][1] != "00000"))
                    {
                        stall_inf = 1;
                    }
                }
            }
            else if(reg_flag == 1) // rs + rt
            {
                /*
                if(stage[3] != "NOP" && stage[3] != "beq" && stage[3] != "bne" && stage[3] != "bgtz" && stage[3] != "sw"  && stage[3] != "sh"   && stage[3] != "sb" && stage[3] != "HALT")
                {
                    //stall detection
                    if(stage[3] == "addi" || stage[3] == "addiu" || stage[3] == "lui" || stage[3] == "andi" || stage[3] == "ori" ||
                       stage[3] == "nori" || stage[3] == "slti"  || stage[3] == "lw"  || stage[3] == "lh"   || stage[3] == "lhu" ||
                       stage[3] == "lb"   || stage[3] == "lbu" ) // rs + rt
                    {
                        cout << "stop3_reg_table[1][0]" << reg_table[1][0] << endl;
                        cout << "D" << endl;
                        //reg_table[][0] == rs reg_table[][1] == rt reg_table[][2] == rd ; if no reg == null
                        if(reg_table[1][0] == reg_table[3][1] && reg_table[1][0] != "" && reg_table[1][0] != "00000")
                        {
                            cout << stage[1] << " " << stage[2] << endl;
                            cout << reg_table[1][0] << " " << reg_table[2][1] << endl;
                            stall_inf = 1;
                        }
                    }
                    else // rs + rt + rd
                    {
                        cout << "E" << endl;
                        if(reg_table[1][0] == reg_table[3][2] && reg_table[1][0] != "" && reg_table[1][0] != "00000")
                        {
                            stall_inf = 1;
                        }
                    }

                }*/
                if(stage[2] == "lw" || stage[2] == "lh" || stage[2] == "lhu" || stage[2] == "lb" || stage[2] == "lbu")
                {
                    //cout << "F" << endl;
                    if(reg_table[1][0] == reg_table[2][1] && reg_table[1][0] != ""  && reg_table[1][0] != "00000")
                    {
                        stall_inf = 1;
                    }
                }

            }

        }
    }
    else if(stage[1] == "beq" || stage[1] == "bne")
    {
         if(stage[2] == "lw" || stage[2] == "lh" || stage[2] == "lhu" || stage[2] == "lb" || stage[2] == "lbu")
        {
            //cout << "C" << endl;
            if((reg_table[1][0] == reg_table[2][1] && reg_table[1][0] != "" && reg_table[1][0] != "00000") ||
               (reg_table[1][1] == reg_table[2][1] && reg_table[1][1] != "" && reg_table[1][1] != "00000"))
            {
                stall_inf = 1;
            }
        }
        else if(stage[2] != "NOP" && stage[2] != "beq" && stage[2] != "bne" && stage[2] != "bgtz" && stage[2] != "sw"  && stage[2] != "sh"   && stage[2] != "sb" && stage[2] != "HALT")
        {
            if(stage[2] == "addi" || stage[2] == "addiu" || stage[2] == "lui" || stage[3] == "andi" || stage[3] == "ori" ||
            stage[2] == "nori" || stage[2] == "slti"  ) // rs + rt
            {
                if((reg_table[1][0] == reg_table[2][1] && reg_table[1][0] != "" && reg_table[1][0] != "00000")  ||
                   (reg_table[1][1] == reg_table[2][1] && reg_table[1][1] != "" && reg_table[1][1] != "00000"))
                {
                    stall_inf = 1;
                }
            }
            else
            {
                if((reg_table[1][0] == reg_table[2][2] && reg_table[1][0] != "" && reg_table[1][0] != "00000")  ||
                   (reg_table[1][1] == reg_table[2][2] && reg_table[1][1] != "" && reg_table[1][1] != "00000"))
                {
                    stall_inf = 1;
                }
            }
        }
        else if(stage[3] == "lw" || stage[3] == "lh" || stage[3] == "lhu" || stage[3] == "lb" || stage[3] == "lbu")
        {
            //cout << "C" << endl;
            if((reg_table[1][0] == reg_table[3][1] && reg_table[1][0] != "" && reg_table[1][0] != "00000") ||
               (reg_table[1][1] == reg_table[3][1] && reg_table[1][1] != "" && reg_table[1][1] != "00000"))
            {
                stall_inf = 1;
            }
        }
    }
    //cout << "stall = " << stall_inf << endl;
    //system("PAUSE");
    if(stall_inf != 1)
    {
        //forward detection
        if(stage[2] != "NOP" && stage[2] != "beq" && stage[2] != "bne" && stage[2] != "bgtz")
        {
            if(reg_flag_2 == 2) // rs + rt + rd
            {
                if(stage[3] != "lw" && stage[3] != "lh" && stage[3] != "lhu" && stage[3] != "lb" && stage[3] != "lbu" &&
                   stage[3] != "sw" && stage[3] != "sh" && stage[3] != "sb"  && stage[3] != "beq"&& stage[3] != "bne" &&
                   stage[3] != "bgtz")
                {
                    //cout << "stage[2] = " << stage[2] << endl;
                    //reg_table[][0] == rs reg_table[][1] == rt reg_table[][2] == rd ;
                    if(stage[3] == "addi" || stage[3] == "addiu" || stage[3] == "lui" || stage[3] == "andi" || stage[3] == "ori" || stage[3] == "nori" || stage[3] == "slti")
                    {
                        //cout << "in" << endl;
                        if(reg_table[2][0] == reg_table[3][1] && reg_table[2][0]!= "" && reg_table[2][0] != "00000")// EX-DM
                        {
                            //cout << "A" << endl;
                            string temp_messeger = "fwd_EX-DM_rs_$";
                            int reg_num = convert_dex(reg_table[2][0]);
                            pipeline_messeger_s = temp_messeger + to_str(reg_num);
                            pipe_forward_s = 1;
                        }
                        if(reg_table[2][1] == reg_table[3][1] && reg_table[2][1]!= "" && reg_table[2][1] != "00000")
                        {

                            //cout << "B" << endl;
                            //cout << "stage[2] = "<< stage[2] << endl;
                            //cout << "reg_table[2][1] = "<< reg_table[2][1] << endl;
                            //cout << "reg_table[3][1] = "<< reg_table[3][1] << endl;
                            //cout << "reg_table[2][0] = "<< reg_table[2][0] << endl;
                            string temp_messeger = "fwd_EX-DM_rt_$";
                            int reg_num = convert_dex(reg_table[2][1]);
                            pipeline_messeger_t = temp_messeger + to_str(reg_num);
                            pipe_forward_t = 1;
                        }
                    }
                    else
                    {
                        //cout << "stage[2] = " << stage[2] << endl;
                        //cout << "stage[3] = " << stage[3] << endl;
                        //cout << "reg_table[2][0] = " << reg_table[2][0] << endl;
                        //cout << "reg_table[3][2] = " << reg_table[3][2] << endl;
                        if(reg_table[2][0] == reg_table[3][2] && reg_table[2][0] != "" && reg_table[2][0] != "00000")// EX-DM
                        {
                            //cout << "C" << endl;
                            string temp_messeger = "fwd_EX-DM_rs_$";
                            int reg_num = convert_dex(reg_table[2][0]);
                            pipeline_messeger_s = temp_messeger + to_str(reg_num);
                            pipe_forward_s = 1;
                        }
                        if(reg_table[2][1] == reg_table[3][2] && reg_table[2][1] != "" && reg_table[2][1] != "00000")
                        {
                            //cout << "D" << endl;
                            string temp_messeger = "fwd_EX-DM_rt_$";
                            int reg_num = convert_dex(reg_table[2][1]);
                            pipeline_messeger_t = temp_messeger + to_str(reg_num);
                            pipe_forward_t = 1;
                        }

                    }

                }
                if(stage[4] != "sw" && stage[4] != "sh" && stage[4] != "sb"  && stage[4] != "beq"&& stage[4] != "bne" &&
                   stage[4] != "bgtz")
                {
                    if(stage[4] == "lw" || stage[4] == "lh" || stage[4] == "lhu" || stage[4] == "lb" || stage[4] == "lbu"||
                       stage[4] == "addi" || stage[4] == "addiu" || stage[4] == "lui" || stage[4] == "andi" || stage[4] == "ori" ||
                       stage[4] == "nori" || stage[4] == "slti" )
                    {
                        if(reg_table[2][0] == reg_table[4][1] && reg_table[2][0] != "" && reg_table[2][0] != "00000" && pipe_forward_s != 1)// DM-WB
                        {
                            //cout << "E" << endl;
                            string temp_messeger = "fwd_DM-WB_rs_$";
                            int reg_num = convert_dex(reg_table[2][0]);
                            pipeline_messeger_s = temp_messeger + to_str(reg_num);
                            pipe_forward_s = 1;

                        }
                        if(reg_table[2][1] == reg_table[4][1] && reg_table[2][1] != "" && reg_table[2][1] != "00000" && pipe_forward_t != 1)
                        {
                            //cout << "F" << endl;
                            string temp_messeger = "fwd_DM-WB_rt_$";
                            int reg_num = convert_dex(reg_table[2][1]);
                            pipeline_messeger_t = temp_messeger + to_str(reg_num);
                            pipe_forward_t = 1;
                        }
                    }
                    else
                    {

                        if(reg_table[2][0] == reg_table[4][2] && reg_table[2][0] != "" && reg_table[2][0] != "00000" && pipe_forward_s != 1)// EX-DM
                        {
                            //cout << "G" << endl;
                            string temp_messeger = "fwd_DM-WB_rs_$";
                            int reg_num = convert_dex(reg_table[2][0]);
                            pipeline_messeger_s = temp_messeger + to_str(reg_num);
                            pipe_forward_s = 1;
                        }
                        if(reg_table[2][1] == reg_table[4][2] && reg_table[2][1] != "" && reg_table[2][1] != "00000" && pipe_forward_t != 1)
                        {
                            //cout << "H" << endl;
                            string temp_messeger = "fwd_DM-WB_rt_$";
                            int reg_num = convert_dex(reg_table[2][1]);
                            pipeline_messeger_t = temp_messeger + to_str(reg_num);
                            pipe_forward_t = 1;
                        }
                    }


                }
            }
            else // rs + rt
            {
                if(stage[3] != "lw" && stage[3] != "lh" && stage[3] != "lhu" && stage[3] != "lb" && stage[3] != "lbu" &&
                   stage[3] != "sw" && stage[3] != "sh" && stage[3] != "sb"  && stage[3] != "beq"&& stage[3] != "bne" &&
                   stage[3] != "bgtz")
                {
                    //reg_table[][0] == rs reg_table[][1] == rt reg_table[][2] == rd ;
                    if(stage[3] == "addi" || stage[3] == "addiu" || stage[3] == "lui" || stage[3] == "andi" || stage[3] == "ori" || stage[3] == "nori" || stage[3] == "slti")
                    {
                        if(reg_table[2][0] == reg_table[3][1] && reg_table[2][0] != "" && reg_table[2][0] != "00000")// EX-DM
                        {
                            //cout << "I" << endl;
                            string temp_messeger = "fwd_EX-DM_rs_$";
                            int reg_num = convert_dex(reg_table[2][0]);
                            pipeline_messeger_s = temp_messeger + to_str(reg_num);
                            pipe_forward_s = 1;
                        }
                    }
                    else
                    {
                        if(reg_table[2][0] == reg_table[3][2] && reg_table[2][0] != "" && reg_table[2][0] != "00000")// EX-DM
                        {
                            //cout << "J" << endl;
                            string temp_messeger = "fwd_EX-DM_rs_$";
                            int reg_num = convert_dex(reg_table[2][0]);
                            pipeline_messeger_s = temp_messeger + to_str(reg_num);
                            pipe_forward_s = 1;
                        }
                   }

                }
                if(stage[4] != "sw" && stage[4] != "sh" && stage[4] != "sb"  && stage[4] != "beq"&& stage[4] != "bne" &&
                   stage[4] != "bgtz")
                {
                    if(stage[4] == "lw" || stage[4] == "lh" || stage[4] == "lhu" || stage[4] == "lb" || stage[4] == "lbu"||
                       stage[4] == "addi" || stage[4] == "addiu" || stage[4] == "lui" || stage[4] == "andi" || stage[4] == "ori" ||
                       stage[4] == "nori" || stage[4] == "slti")
                    {
                        if(reg_table[2][0] == reg_table[4][1] && reg_table[2][0]!= "" && reg_table[2][0] != "00000" && pipe_forward_s != 1)// DM-WB
                        {
                            //cout << "K" << endl;
                            string temp_messeger = "fwd_DM-WB_rs_$";
                            int reg_num = convert_dex(reg_table[2][0]);
                            pipeline_messeger_s = temp_messeger + to_str(reg_num);
                            pipe_forward_s = 1;

                        }
                    }
                    else
                    {
                        if(reg_table[2][0] == reg_table[4][2] && reg_table[2][0]!= "" && reg_table[2][0] != "00000" && pipe_forward_s != 1)// DM-WB
                        {
                            //cout << "L" << endl;
                            string temp_messeger = "fwd_DM-WB_rs_$";
                            int reg_num = convert_dex(reg_table[2][0]);
                            pipeline_messeger_s = temp_messeger + to_str(reg_num);
                            pipe_forward_s = 1;

                        }

                    }
                }

            }

        }

    }
    if(stage[3] == "lw" || stage[3] == "lh" || stage[3] == "lhu" || stage[3] == "lb"  || stage[3] == "lbu"||
       stage[3] == "sw" || stage[3] == "sh" || stage[3] == "sb"  || stage[3] == "mult"|| stage[3] == "multu") //fout no output
    {
        if(stage[3] == "mult")
                 ins_mult(reg_table[3][0],reg_table[3][1], 2 ,result_buffer[3][0] ,result_buffer[3][1], error_table[3][0] , error_table[3][1] , write_table[3]);
        if(stage[3] == "multu")
                 ins_multu(reg_table[3][0],reg_table[3][1], 2 ,result_buffer[3][0] ,result_buffer[3][1], error_table[3][0] , error_table[3][1] , write_table[3]);
        if(stage[3] == "lw")
        {
            if(ins_lw(reg_table[3][0],reg_table[3][1],reg_table[3][3],0,result_buffer[3][0] , error_table[3][0], error_table[3][1] , write_table[3]) == -1)
            {
                pipe_line = -1;
            }
        }

        if(stage[3] == "lh")
            if(ins_lh(reg_table[3][0],reg_table[3][1],reg_table[3][3],0,result_buffer[3][0] , error_table[3][0], error_table[3][1] , write_table[3]) == -1)
            {
                pipe_line = -1;
            }
        if(stage[3] == "lhu")
            if(ins_lhu(reg_table[3][0],reg_table[3][1],reg_table[3][3],0,result_buffer[3][0] , error_table[3][0], error_table[3][1] , write_table[3]) == -1)
            {
                pipe_line = -1;
            }
        if(stage[3] == "lb")
            if(ins_lb(reg_table[3][0],reg_table[3][1],reg_table[3][3],0,result_buffer[3][0] , error_table[3][0], error_table[3][1] , write_table[3]) == -1)
            {
                pipe_line = -1;
            }
        if(stage[3] == "lbu")
            if(ins_lbu(reg_table[3][0],reg_table[3][1],reg_table[3][3],0,result_buffer[3][0] , error_table[3][0], error_table[3][1] , write_table[3]) == -1)
            {
               pipe_line = -1;
            }
        if(stage[3] == "sw")
            if(ins_sw(reg_table[3][0],reg_table[3][1],reg_table[3][3], 0 , error_table[3][0] , error_table[3][1], write_table[3]) == -1)
            {
               pipe_line = -1;
            }
        if(stage[3] == "sh")
            if(ins_sh(reg_table[3][0],reg_table[3][1],reg_table[3][3], 0 , error_table[3][0] , error_table[3][1], write_table[3]) == -1)
            {
                pipe_line = -1;
            }
        if(stage[3] == "sb")
            if(ins_sb(reg_table[3][0],reg_table[3][1],reg_table[3][3], 0 , error_table[3][0] , error_table[3][1], write_table[3]) == -1)
            {
                pipe_line = -1;
            }

    }
    //cout << "stage[2] = " << stage[2] << endl;
    if(stage[2] != "NOP")//EXE stage
    {
        if(stage[2] == "lw" || stage[2] == "lh" || stage[2] == "lhu" || stage[2] == "lb" || stage[2] == "lbu"||
           stage[2] == "sw" || stage[2] == "sh" || stage[2] == "sb"  || stage[2] == "beq" || stage[2] == "bne" ||
           stage[2] == "bgtz" || stage[2] == "HALT")
        {
            no_ins();
        }
        else
        {
            if(stage[2] == "add")
                ins_add(reg_table[2][0],reg_table[2][1],reg_table[2][2], 0 ,result_buffer[2][0] , error_table[2][0] , error_table[2][1] , write_table[2]);
            if(stage[2] == "addu")
                ins_addu(reg_table[2][0],reg_table[2][1],reg_table[2][2], 0 ,result_buffer[2][0] , error_table[2][0] , error_table[2][1] , write_table[2]);
            if(stage[2] == "sub")
                ins_sub(reg_table[2][0],reg_table[2][1],reg_table[2][2], 0 ,result_buffer[2][0] , error_table[2][0] , error_table[2][1] , write_table[2]);
            if(stage[2] == "and")
                ins_and(reg_table[2][0],reg_table[2][1],reg_table[2][2], 0 ,result_buffer[2][0] , error_table[2][0] , error_table[2][1] , write_table[2]);
            if(stage[2] == "or")
                ins_or(reg_table[2][0],reg_table[2][1],reg_table[2][2], 0 ,result_buffer[2][0] , error_table[2][0] , error_table[2][1] , write_table[2]);
            if(stage[2] == "xor")
                ins_xor(reg_table[2][0],reg_table[2][1],reg_table[2][2], 0 ,result_buffer[2][0] , error_table[2][0] , error_table[2][1] , write_table[2]);
            if(stage[2] == "nor")
                ins_nor(reg_table[2][0],reg_table[2][1],reg_table[2][2], 0 ,result_buffer[2][0] , error_table[2][0] , error_table[2][1] , write_table[2]);
            if(stage[2] == "nand")
                ins_nand(reg_table[2][0],reg_table[2][1],reg_table[2][2], 0 ,result_buffer[2][0] , error_table[2][0] , error_table[2][1] , write_table[2]);
            if(stage[2] == "slt")
                ins_slt(reg_table[2][0],reg_table[2][1],reg_table[2][2], 0 ,result_buffer[2][0] , error_table[2][0] , error_table[2][1] , write_table[2]);
            if(stage[2] == "sll")
                 ins_sll(reg_table[2][0],reg_table[2][1],reg_table[2][3], 0 ,result_buffer[2][0] , error_table[2][0] , error_table[2][1] , write_table[2]);
            if(stage[2] == "srl")
                 ins_srl(reg_table[2][0],reg_table[2][1],reg_table[2][3], 0 ,result_buffer[2][0] , error_table[2][0] , error_table[2][1] , write_table[2]);
            if(stage[2] == "sra")
                 ins_sra(reg_table[2][0],reg_table[2][1],reg_table[2][3], 0 ,result_buffer[2][0] , error_table[2][0] , error_table[2][1] , write_table[2]);
            if(stage[2] == "jr")
            {
                int return_block = ins_jr(reg_table[2][0], 0);
                pipe_line = pipe_line - return_block - 1;
            }
            if(stage[2] == "mult")
                 ins_mult(reg_table[2][0],reg_table[2][1], 0 ,result_buffer[2][0] ,result_buffer[2][1], error_table[2][0] , error_table[2][1] , write_table[2]);
            if(stage[2] == "multu")
                 ins_multu(reg_table[2][0],reg_table[2][1], 0 ,result_buffer[2][0] ,result_buffer[2][1], error_table[2][0] , error_table[2][1] , write_table[2]);
            if(stage[2] == "mfhi")
                 ins_mfhi(reg_table[2][2], 0 ,result_buffer[2][0] , error_table[2][0] , error_table[2][1] , write_table[2]);
            if(stage[2] == "mflo")
                 ins_mflo(reg_table[2][2], 0 ,result_buffer[2][0] , error_table[2][0] , error_table[2][1] , write_table[2]);
            if(stage[2] == "addi")//char *rs , char *rt , int imm, int wb_inf , long long &rt_buf, int &error_1, int &error_2 , int &write_reg
            ins_addi(reg_table[2][0], reg_table[2][1], reg_table[2][3], 0, result_buffer[2][0], error_table[2][0], error_table[2][1], write_table[2]);
            if(stage[2] == "addiu")
            ins_addiu(reg_table[2][0], reg_table[2][1], reg_table[2][3], 0, result_buffer[2][0], error_table[2][0], error_table[2][1], write_table[2]);
            if(stage[2] == "lui")
                ins_lui(reg_table[2][1] , reg_table[2][3] , 0 , result_buffer[2][0] , error_table[2][0] , error_table[2][1] , write_table[2]);
            if(stage[2] == "andi")
                ins_andi(reg_table[2][0] , reg_table[2][1] , reg_table[2][3] , 0 , result_buffer[2][0] , error_table[2][0] , error_table[2][1] , write_table[2]);
            if(stage[2] == "ori")
                ins_ori(reg_table[2][0] , reg_table[2][1] , reg_table[2][3] , 0 , result_buffer[2][0] , error_table[2][0] , error_table[2][1] , write_table[2]);
            if(stage[2] == "nori")
                ins_nori(reg_table[2][0] , reg_table[2][1] , reg_table[2][3] , 0 , result_buffer[2][0] , error_table[2][0] , error_table[2][1] , write_table[2]);
            if(stage[2] == "slti")
                ins_slti(reg_table[2][0] , reg_table[2][1] , reg_table[2][3] , 0 , result_buffer[2][0] , error_table[2][0] , error_table[2][1] , write_table[2]);
            if(stage[2] == "j")
            {
                int jal_mark = 0;
                jal_mark = ins_j(reg_table[2][3], 0);
                pipe_line = pipe_line - jal_mark-1;
            }
            if(stage[2] == "jal")
            {
                int jal_mark = 0;
                jal_mark = ins_j(reg_table[2][3], 0);
                pipe_line = pipe_line + jal_mark;
            }
        }
        if(stall_inf == 1)
            reg[34] = reg[34] - 4;
    }
    else
    {
        if(stage[1] != "beq" && stage[1] != "bne" && stage[1] != "bgtz" )
            no_ins();
        if((stage[1] == "beq" || stage[1] == "bne" || stage[1] == "bgtz") && stall_inf == 1)
            fout1 << "PC: " << convert_hex(reg[34]+4)<< endl;
    }
    //system("PAUSE");
    if((stage[1] == "beq" || stage[1] == "bne" || stage[1] == "bgtz") && stall_inf == 0)
    {
        //cout << "stage[1] = " << stage[1] << endl;
        //cout << "IN" << endl;
        if(stage[3] != "lw" && stage[3] != "lh" && stage[3] != "lhu" && stage[3] != "lb" && stage[3] != "lbu" &&
           stage[3] != "sw" && stage[3] != "sh" && stage[3] != "sb"  && stage[3] != "beq"&& stage[3] != "bne" &&
           stage[3] != "bgtz")
        {
            //reg_table[][0] == rs reg_table[][1] == rt reg_table[][2] == rd ;
            if(stage[3] == "addi" || stage[3] == "addiu" || stage[3] == "lui" || stage[3] == "andi" || stage[3] == "ori" || stage[3] == "nori" || stage[3] == "slti")
            {
                if(reg_table[1][0] == reg_table[3][1] && reg_table[1][0] != "" && reg_table[1][0] != "00000")// EX-DM
                {
                    string temp_messeger = "fwd_EX-DM_rs_$";
                    int reg_num = convert_dex(reg_table[1][0]);
                    pipeline_messeger_branch_s = temp_messeger + to_str(reg_num);
                    pipe_forward_branch_s = 1;
                }
                if(reg_table[1][1] == reg_table[3][1] && reg_table[1][1]!= "" && reg_table[1][1] != "00000")
                {
                    string temp_messeger = "fwd_EX-DM_rt_$";
                    int reg_num = convert_dex(reg_table[1][1]);
                    pipeline_messeger_branch_t = temp_messeger + to_str(reg_num);
                    pipe_forward_branch_t = 1;
                }
            }
            else
            {
                if(reg_table[1][0] == reg_table[3][2] && reg_table[1][0]!= "" && reg_table[1][0] != "00000")// EX-DM
                {

                    string temp_messeger = "fwd_EX-DM_rs_$";
                    int reg_num = convert_dex(reg_table[1][0]);
                    pipeline_messeger_branch_s = temp_messeger + to_str(reg_num);
                    pipe_forward_branch_s = 1;
                }
                if(reg_table[1][1] == reg_table[3][2] && reg_table[1][1]!= "" && reg_table[1][1] != "00000")
                {
                    string temp_messeger = "fwd_EX-DM_rt_$";
                    int reg_num = convert_dex(reg_table[1][1]);
                    pipeline_messeger_branch_t = temp_messeger + to_str(reg_num);
                    pipe_forward_branch_t = 1;
                }
            }

        }
        /*
        if(stage[4] == "lw" || stage[4] == "lh" || stage[4] == "lhu" || stage[4] == "lb" || stage[4] == "lbu")
            {
                cout << "reg_table[1][0] = " << reg_table[1][0] << endl;
                cout << "reg_table[4][1] = " << reg_table[4][1] << endl;
                if(reg_table[1][0] == reg_table[4][1] && reg_table[1][0]!= "" && reg_table[1][0] != "00000")// DM-WB
                {
                    cout << "IN2" << endl;
                    string temp_messeger = "fwd_EX-DM_rs_$";
                    int reg_num = convert_dex(reg_table[1][0]);
                    pipeline_messeger_branch_s = temp_messeger + to_str(reg_num);
                    pipe_forward_branch_s = 1;

                }
                else if(reg_table[1][1] == reg_table[4][1] && reg_table[1][1] != "" && reg_table[1][1] != "00000")// EX-DM
                {
                    string temp_messeger = "fwd_EX-DM_rt_$";
                    int reg_num = convert_dex(reg_table[1][1]);
                    pipeline_messeger_branch_t = temp_messeger + to_str(reg_num);
                    pipe_forward_branch_t = 1;

                }
            }*/
        /*
        if(stage[4] != "sw" && stage[4] != "sh" && stage[4] != "sb"  && stage[4] != "beq"&& stage[4] != "bne" && stage[4] != "bgtz")
        {


            else
            {
                if(reg_table[1][0] == reg_table[4][2] && reg_table[1][0]!= "" && reg_table[1][0] == "00000")// DM-WB
                {
                    string temp_messeger = "fwd_DM-WB_rs_$";
                    int reg_num = convert_dex(reg_table[1][0]);
                    pipeline_messeger_branch_s = temp_messeger + to_str(reg_num);
                    pipe_forward_branch_s = 1;

                }
                else if(reg_table[1][1] == reg_table[4][2] && reg_table[1][1] != "" && reg_table[1][1] == "00000")// EX-DM
                {
                    string temp_messeger = "fwd_DM-WB_rt_$";
                    int reg_num = convert_dex(reg_table[1][1]);
                    pipeline_messeger_branch_t = temp_messeger + to_str(reg_num);
                    pipe_forward_branch_t = 1;

                }

            }*/

        int new_block = 0;
        if(stage[1] == "beq")
            new_block = ins_beq(reg_table[1][0] , reg_table[1][1] , reg_table[1][3] , 0 );
        if(stage[1] == "bne")
            new_block = ins_bne(reg_table[1][0] , reg_table[1][1] , reg_table[1][3] , 0 );
        if(stage[1] == "bgtz")
            new_block = ins_bgtz(reg_table[1][0] , reg_table[1][3] , 0 );
        if(pipe_forward_branch_s == 1 && pipe_forward_branch_t == 1 && pipe_forward_s == 1 && pipe_forward_t == 1)
        {
            string temp_st = pipeline_messeger_s + " " + pipeline_messeger_t;
            string temp_branch_st = pipeline_messeger_branch_s + " " + pipeline_messeger_branch_t;
            if(new_block != 0)
            {
                pipe_line = pipe_line + new_block ;
                fout1 << "IF: " << stage[0] << " to_be_flushed" << endl;
                stall_inf = 2;
            }
            else
                fout1 << "IF: " << stage[0] << endl;
            fout1 << "ID: " << upper(stage[1]) << " " << temp_branch_st <<endl;
            fout1 << "EX: " << upper(stage[2]) << " " << temp_st <<endl;
            fout1 << "DM: " << upper(stage[3]) << endl;
            fout1 << "WB: " << upper(stage[4]) << endl;
        }
        else if(pipe_forward_branch_s == 1 && pipe_forward_branch_t == 1 && pipe_forward_s == 1 && pipe_forward_t == 0)
        {
            string temp_branch_st = pipeline_messeger_branch_s + " " + pipeline_messeger_branch_t;
            if(new_block != 0)
            {
                pipe_line = pipe_line + new_block ;
                fout1 << "IF: " << stage[0] << " to_be_flushed" << endl;
                stall_inf = 2;
            }
            else
                fout1 << "IF: " << stage[0] << endl;
            fout1 << "ID: " << upper(stage[1]) << " " << temp_branch_st << endl;
            fout1 << "EX: " << upper(stage[2]) << " " << pipeline_messeger_s << endl;
            fout1 << "DM: " << upper(stage[3]) << endl;
            fout1 << "WB: " << upper(stage[4]) << endl;
        }
        else if(pipe_forward_branch_s == 1 && pipe_forward_branch_t == 1 && pipe_forward_s == 0 && pipe_forward_t == 1)
        {
            string temp_branch_st = pipeline_messeger_branch_s + " " + pipeline_messeger_branch_t;
            if(new_block != 0)
            {
                pipe_line = pipe_line + new_block ;
                fout1 << "IF: " << stage[0] << " to_be_flushed" << endl;
                stall_inf = 2;
            }
            else
                fout1 << "IF: " << stage[0] << endl;
            fout1 << "ID: " << upper(stage[1]) << " " << temp_branch_st<< endl;
            fout1 << "EX: " << upper(stage[2]) << " " << pipeline_messeger_t << endl;
            fout1 << "DM: " << upper(stage[3]) << endl;
            fout1 << "WB: " << upper(stage[4]) << endl;

        }
        else if(pipe_forward_branch_s == 1 && pipe_forward_branch_t == 0 && pipe_forward_s == 1 && pipe_forward_t == 1)
        {
            string temp_st = pipeline_messeger_s + " " + pipeline_messeger_t;
            if(new_block != 0)
            {
                pipe_line = pipe_line + new_block ;
                fout1 << "IF: " << stage[0] << " to_be_flushed" << endl;
                stall_inf = 2;
            }
            else
                fout1 << "IF: " << stage[0] << endl;
            fout1 << "ID: " << upper(stage[1]) << " " << pipeline_messeger_branch_s << endl;
            fout1 << "EX: " << upper(stage[2]) << " " << temp_st << endl;
            fout1 << "DM: " << upper(stage[3]) << endl;
            fout1 << "WB: " << upper(stage[4]) << endl;

        }
        else if(pipe_forward_branch_s == 0 && pipe_forward_branch_t == 1 && pipe_forward_s == 1 && pipe_forward_t == 1)
        {
            string temp_st = pipeline_messeger_s + " " + pipeline_messeger_t;
             if(new_block != 0)
            {
                pipe_line = pipe_line + new_block ;
                fout1 << "IF: " << stage[0] << " to_be_flushed" << endl;
                stall_inf = 2;
            }
            else
                fout1 << "IF: " << stage[0] << endl;
            fout1 << "ID: " << upper(stage[1]) << " " << pipeline_messeger_branch_t <<endl;
            fout1 << "EX: " << upper(stage[2]) << " " << temp_st << endl;
            fout1 << "DM: " << upper(stage[3]) << endl;
            fout1 << "WB: " << upper(stage[4]) << endl;

        }
        else if(pipe_forward_branch_s == 1 && pipe_forward_branch_t == 1 && pipe_forward_s == 0 && pipe_forward_t == 0)//1100
        {
            string temp_branch_st = pipeline_messeger_branch_s + " " + pipeline_messeger_branch_t;
             if(new_block != 0)
            {
                pipe_line = pipe_line + new_block ;
                fout1 << "IF: " << stage[0] << " to_be_flushed" << endl;
                stall_inf = 2;
            }
            else
                fout1 << "IF: " << stage[0] << endl;
            fout1 << "ID: " << upper(stage[1]) << " " << temp_branch_st <<endl;
            fout1 << "EX: " << upper(stage[2]) << endl;
            fout1 << "DM: " << upper(stage[3]) << endl;
            fout1 << "WB: " << upper(stage[4]) << endl;

        }
        else if(pipe_forward_branch_s == 1 && pipe_forward_branch_t == 0 && pipe_forward_s == 0 && pipe_forward_t == 1)//1001
        {
             if(new_block != 0)
            {
                pipe_line = pipe_line + new_block ;
                fout1 << "IF: " << stage[0] << " to_be_flushed" << endl;
                stall_inf = 2;
            }
            else
                fout1 << "IF: " << stage[0] << endl;
            fout1 << "ID: " << upper(stage[1]) << " " << pipeline_messeger_branch_s <<endl;
            fout1 << "EX: " << upper(stage[2]) << " " << pipeline_messeger_t << endl;
            fout1 << "DM: " << upper(stage[3]) << endl;
            fout1 << "WB: " << upper(stage[4]) << endl;

        }
        else if(pipe_forward_branch_s == 0 && pipe_forward_branch_t == 0 && pipe_forward_s == 1 && pipe_forward_t == 1)//0011
        {
            string temp_st = pipeline_messeger_s + " " + pipeline_messeger_t;
             if(new_block != 0)
            {
                pipe_line = pipe_line + new_block ;
                fout1 << "IF: " << stage[0] << " to_be_flushed" << endl;
                stall_inf = 2;
            }
            else
                fout1 << "IF: " << stage[0] << endl;
            fout1 << "ID: " << upper(stage[1]) << endl;
            fout1 << "EX: " << upper(stage[2]) << " " << temp_st <<endl;
            fout1 << "DM: " << upper(stage[3]) << endl;
            fout1 << "WB: " << upper(stage[4]) << endl;

        }
        else if(pipe_forward_branch_s == 0 && pipe_forward_branch_t == 1 && pipe_forward_s == 1 && pipe_forward_t == 0)//0110
        {
             if(new_block != 0)
            {
                pipe_line = pipe_line + new_block ;
                fout1 << "IF: " << stage[0] << " to_be_flushed" << endl;
                stall_inf = 2;
            }
            else
                fout1 << "IF: " << stage[0] << endl;
            fout1 << "ID: " << upper(stage[1]) << " " << pipeline_messeger_branch_t <<endl;
            fout1 << "EX: " << upper(stage[2]) << " " << pipeline_messeger_s << endl;
            fout1 << "DM: " << upper(stage[3]) << endl;
            fout1 << "WB: " << upper(stage[4]) << endl;

        }
        else if(pipe_forward_branch_s == 0 && pipe_forward_branch_t == 1 && pipe_forward_s == 0 && pipe_forward_t == 1)//0101
        {
             if(new_block != 0)
            {
                pipe_line = pipe_line + new_block ;
                fout1 << "IF: " << stage[0] << " to_be_flushed" << endl;
                stall_inf = 2;
            }
            else
                fout1 << "IF: " << stage[0] << endl;
            fout1 << "ID: " << upper(stage[1]) << " " << pipeline_messeger_branch_t <<endl;
            fout1 << "EX: " << upper(stage[2]) << " " << pipeline_messeger_t << endl;
            fout1 << "DM: " << upper(stage[3]) << endl;
            fout1 << "WB: " << upper(stage[4]) << endl;

        }
        else if(pipe_forward_branch_s == 1 && pipe_forward_branch_t == 0 && pipe_forward_s == 1 && pipe_forward_t == 0)//1010
        {
             if(new_block != 0)
            {
                pipe_line = pipe_line + new_block ;
                fout1 << "IF: " << stage[0] << " to_be_flushed" << endl;
                stall_inf = 2;
            }
            else
                fout1 << "IF: " << stage[0] << endl;
            fout1 << "ID: " << upper(stage[1]) << " " << pipeline_messeger_branch_s <<endl;
            fout1 << "EX: " << upper(stage[2]) << " " << pipeline_messeger_s << endl;
            fout1 << "DM: " << upper(stage[3]) << endl;
            fout1 << "WB: " << upper(stage[4]) << endl;

        }
        else if(pipe_forward_branch_s == 0 && pipe_forward_branch_t == 0 && pipe_forward_s == 0 && pipe_forward_t == 1)//0001
        {
             if(new_block != 0)
            {
                pipe_line = pipe_line + new_block ;
                fout1 << "IF: " << stage[0] << " to_be_flushed" << endl;
                stall_inf = 2;
            }
            else
                fout1 << "IF: " << stage[0] << endl;
            fout1 << "ID: " << upper(stage[1]) << endl;
            fout1 << "EX: " << upper(stage[2]) << " " << pipeline_messeger_t <<endl;
            fout1 << "DM: " << upper(stage[3]) << endl;
            fout1 << "WB: " << upper(stage[4]) << endl;

        }
        else if(pipe_forward_branch_s == 1 && pipe_forward_branch_t == 0 && pipe_forward_s == 0 && pipe_forward_t == 0)//1000
        {
             if(new_block != 0)
            {
                pipe_line = pipe_line + new_block ;
                fout1 << "IF: " << stage[0] << " to_be_flushed" << endl;
                stall_inf = 2;
            }
            else
                fout1 << "IF: " << stage[0] << endl;
            fout1 << "ID: " << upper(stage[1]) << " " << pipeline_messeger_branch_s <<endl;
            fout1 << "EX: " << upper(stage[2]) << endl;
            fout1 << "DM: " << upper(stage[3]) << endl;
            fout1 << "WB: " << upper(stage[4]) << endl;

        }
        else if(pipe_forward_branch_s == 0 && pipe_forward_branch_t == 1 && pipe_forward_s == 0 && pipe_forward_t == 0)//0100
        {
             if(new_block != 0)
            {
                pipe_line = pipe_line + new_block ;
                fout1 << "IF: " << stage[0] << " to_be_flushed" << endl;
                stall_inf = 2;
            }
            else
                fout1 << "IF: " << stage[0] << endl;
            fout1 << "ID: " << upper(stage[1]) << " " << pipeline_messeger_branch_t <<endl;
            fout1 << "EX: " << upper(stage[2]) << endl;
            fout1 << "DM: " << upper(stage[3]) << endl;
            fout1 << "WB: " << upper(stage[4]) << endl;

        }
        else if(pipe_forward_branch_s == 0 && pipe_forward_branch_t == 0 && pipe_forward_s == 1 && pipe_forward_t == 0)//0010
        {
             if(new_block != 0)
            {
                pipe_line = pipe_line + new_block ;
                fout1 << "IF: " << stage[0] << " to_be_flushed" << endl;
                stall_inf = 2;
            }
            else
                fout1 << "IF: " << stage[0] << endl;
            fout1 << "ID: " << upper(stage[1]) << endl;
            fout1 << "EX: " << upper(stage[2]) << " " << pipeline_messeger_s <<endl;
            fout1 << "DM: " << upper(stage[3]) << endl;
            fout1 << "WB: " << upper(stage[4]) << endl;

        }
        else if(pipe_forward_branch_s == 0 && pipe_forward_branch_t == 0 && pipe_forward_s == 0 && pipe_forward_t == 0)
        {
             if(new_block != 0)
            {
                pipe_line = pipe_line + new_block ;
                fout1 << "IF: " << stage[0] << " to_be_flushed" << endl;
                stall_inf = 2;
            }
            else
                fout1 << "IF: " << stage[0] << endl;
            fout1 << "ID: " << upper(stage[1]) << endl;
            fout1 << "EX: " << upper(stage[2]) << endl;
            fout1 << "DM: " << upper(stage[3]) << endl;
            fout1 << "WB: " << upper(stage[4]) << endl;

        }
    }
    else if(stall_inf == 0 && pipe_forward_s == 1 && pipe_forward_t == 1)
    {
        string temp_st = pipeline_messeger_s + " " + pipeline_messeger_t;
        fout1 << "IF: " << stage[0] << endl;
        fout1 << "ID: " << upper(stage[1]) << endl;
        fout1 << "EX: " << upper(stage[2]) << " " << temp_st <<endl;
        fout1 << "DM: " << upper(stage[3]) << endl;
        fout1 << "WB: " << upper(stage[4]) << endl;

    }
    else if(stall_inf == 0 && pipe_forward_s == 1 && pipe_forward_t == 0)
    {
        fout1 << "IF: " << stage[0] << endl;
        fout1 << "ID: " << upper(stage[1]) << endl;
        fout1 << "EX: " << upper(stage[2]) << " " << pipeline_messeger_s <<endl;
        fout1 << "DM: " << upper(stage[3]) << endl;
        fout1 << "WB: " << upper(stage[4]) << endl;

    }
    else if(stall_inf == 0 && pipe_forward_s == 0 && pipe_forward_t == 1)
    {
        fout1 << "IF: " << stage[0] << endl;
        fout1 << "ID: " << upper(stage[1]) << endl;
        fout1 << "EX: " << upper(stage[2]) << " " << pipeline_messeger_t <<endl;
        fout1 << "DM: " << upper(stage[3]) << endl;
        fout1 << "WB: " << upper(stage[4]) << endl;

    }
    else if(stall_inf == 0 && pipe_forward_s == 0 && pipe_forward_t == 0)
    {
        fout1 << "IF: " << stage[0] << endl;
        fout1 << "ID: " << upper(stage[1]) << endl;
        fout1 << "EX: " << upper(stage[2]) << endl;
        fout1 << "DM: " << upper(stage[3]) << endl;
        fout1 << "WB: " << upper(stage[4]) << endl;
    }
    else if(stall_inf == 1 && pipe_forward_s == 1 && pipe_forward_t == 1)
    {
        string temp_st = pipeline_messeger_s + " " + pipeline_messeger_t;
        fout1 << "IF: " << stage[0] << " to_be_stalled" << endl;
        fout1 << "ID: " << upper(stage[1]) << " to_be_stalled" << endl;
        fout1 << "EX: " << upper(stage[2]) << " " << temp_st <<endl;
        fout1 << "DM: " << upper(stage[3]) << endl;
        fout1 << "WB: " << upper(stage[4]) << endl;
    }
    else if(stall_inf == 1 && pipe_forward_s == 1 && pipe_forward_t == 0)
    {
        fout1 << "IF: " << stage[0] << " to_be_stalled" << endl;
        fout1 << "ID: " << upper(stage[1]) << " to_be_stalled" << endl;
        fout1 << "EX: " << upper(stage[2]) << " " << pipeline_messeger_s <<endl;
        fout1 << "DM: " << upper(stage[3]) << endl;
        fout1 << "WB: " << upper(stage[4]) << endl;

    }
    else if(stall_inf == 1 && pipe_forward_s == 0 && pipe_forward_t == 1)
    {
        fout1 << "IF: " << stage[0] << " to_be_stalled" << endl;
        fout1 << "ID: " << upper(stage[1]) << " to_be_stalled" << endl;
        fout1 << "EX: " << upper(stage[2]) << " " << pipeline_messeger_t <<endl;
        fout1 << "DM: " << upper(stage[3]) << endl;
        fout1 << "WB: " << upper(stage[4]) << endl;

    }
    else if(stall_inf == 1 && pipe_forward_s == 0 && pipe_forward_t == 0)
    {
        fout1 << "IF: " << stage[0] << " to_be_stalled" << endl;
        fout1 << "ID: " << upper(stage[1]) << " to_be_stalled" << endl;
        fout1 << "EX: " << upper(stage[2]) << endl;
        fout1 << "DM: " << upper(stage[3]) << endl;
        fout1 << "WB: " << upper(stage[4]) << endl;

    }
}
int main()
{
    fout1.open("snapshot.rpt");
    fout2.open("error_dump.rpt");
    FILE* fp;
    fp = fopen("iimage.bin" , "rb");
    if (!fp)
        return 1 ;
    unsigned char buff;
    vector<unsigned char>inst;
    //int cnt = 0, line = 0;
    while(fread(&buff,sizeof(char),1,fp) == 1)
    {
        inst.push_back(buff);

    }
    fclose(fp);
    vector<string >binary;
    string tempbin = "";
    for(int i = 0 ; i < inst.size() ; i++)
    {
        bitset<8> bin(inst[i]); //nest block bin is new declare
        string str = bin.to_string<char,std::string::traits_type,std::string::allocator_type>();
        tempbin = tempbin + str;
        if((i+1)%4 == 0)
        {
            binary.push_back(tempbin);
            tempbin = "";
        }
    }
    inst.clear();
    //dimage open ******************************************************************
    FILE* fp_2;
    fp_2 = fopen("dimage.bin" , "rb");
    if (!fp_2)
        return 1 ;
    unsigned char buff_2;
    vector<unsigned char>d_bin;
    while(fread(&buff_2,sizeof(char),1,fp_2) == 1)
    {
        d_bin.push_back(buff_2);

    }
    fclose(fp_2);
    string tempbin_2 = "";
    for(int i = 0 ; i < d_bin.size() ; i++)
    {
        bitset<8> bin(d_bin[i]); //nest block bin is new declare
        string str = bin.to_string<char,std::string::traits_type,std::string::allocator_type>();
        tempbin_2 = tempbin_2 + str;
        if((i+1)%4 == 0)
        {
            d_mem.push_back(tempbin_2);
            tempbin_2 = "";
        }
    }
    d_bin.clear();
    for(int i = 0 ; i < 512 ; i++)
    {
        if(i >= d_mem.size())
            d_mem.push_back(convert_bin(0));
    }
    fout1 << "cycle " << cycle << endl;
    for(int i = 0 ; i < 35 ; i++)
    {
        if(i == 29)
            reg.push_back(convert_dex(d_mem[0]));
        else if (i == 34)
            reg.push_back(convert_dex(binary[0]));
        else
            reg.push_back(0x00);
    }
    for(int i = 0 ; i < 35 ; i++)
    {
        if(i < 10)
            fout1 << "$0" << i << ": " << convert_hex(reg[i]) << endl;
        else if (i == 32)
            fout1 << "$HI: " << convert_hex(reg[i]) << endl;
        else if (i == 33)
            fout1 << "$LO: " << convert_hex(reg[i]) << endl;
        else if (i == 34)
            fout1 << "PC: " << convert_hex(reg[i]) << endl;
        else
            fout1 << "$" << i << ": " << convert_hex(reg[i]) << endl;
    }
    //fout1 << endl;
    //fout1 << endl;
    string stage[5] = {"NOP" , "NOP", "NOP", "NOP", "NOP"};
    string new_ins[3] = {"NOP" , "NOP", "NOP"};
    string reg_table[5][4] = {{}};
    long long result_buffer[5][2] ={{0}};
    int error_table[5][2] = {{0}};
    int write_table[5] = {0};
    int halt_count = 0;
    int stall_inf = 0;
    int halt = 0;
    int initial = 1;
    int i = 2;
    bool non_stop = 0;
    while(non_stop != 1)
    {
        //cout << "i = " << i << endl;
        if(stall_inf == 0)
        {
            new_ins[2] = new_ins[1];
            new_ins[1] = new_ins[0];
            new_ins[0] = "";
            string rec_bin(binary[i]);
            int temp_bin = convert_dex(rec_bin);
            new_ins[0] = convert_hex(temp_bin);
        }
        else if(stall_inf == 2)
        {
            new_ins[2] = "NOP";
            new_ins[1] = "NOP";
            new_ins[0] = "";
            string rec_bin(binary[i]);
            int temp_bin = convert_dex(rec_bin);
            new_ins[0] = convert_hex(temp_bin);
        }
        if(initial != 1)
        {
            cycle++;
            write_stage(stage, new_ins, reg_table , result_buffer, error_table ,write_table , stall_inf , i);

        }
        if(new_ins[0] == "0xFFFFFFFF")
        {
            halt++ ;
        }
        else
        {
            halt = 0;
        }
        if(stall_inf != 1)
        {
            if(i < 2)
            {
                cycle++;
                new_ins[1] == "NOP";
                //no_ins();
                //fout1 << endl;
                //fout1 << endl;
            }
            else{
                int data_error = 0;
                //fout1 << "line = " <<(i-1) << endl;
                const string null = "";
                char* temp_bin = strdup(binary[i].c_str());
                char *opcode = new char[6]();
                opcode[6] = '\0';
                memcpy(opcode,temp_bin,6);
                string str_opcode = opcode;
                //delete[] opcode;
                //cout << "cycle " << cycle << endl;
                //fout1 << opcode << endl;
                //fout1 << "YES" << endl;
                if(str_opcode.compare("000000") == 0)
                {
                    //label[i] = 1;
                    char *func = new char[6]();
                    func[6] = '\0';
                    memcpy(func,temp_bin+26,6);
                    char *ch_rs = new char[5]();
                    ch_rs[5] = '\0';
                    memcpy(ch_rs,temp_bin+6,5);
                    string rs(ch_rs);
                    char *ch_rt = new char[5]();
                    ch_rt[5] = '\0';
                    memcpy(ch_rt,temp_bin+11,5);
                    string rt(ch_rt);
                    char *ch_rd = new char[5]();
                    ch_rd[5] = '\0';
                    memcpy(ch_rd,temp_bin+16,5);
                    string rd(ch_rd);
                    char *ch_shamt = new char[5]();
                    ch_shamt[5] = '\0';
                    memcpy(ch_shamt,temp_bin+21,5);
                    string shamt(ch_shamt);
                    string ins = "";
                    //fout1 << func << endl;
                    ins = conver_to_decode(1, null, func);
                    new_ins[1] = "";
                    new_ins[1] = ins;
                    reg_table[0][0] = rs;
                    reg_table[0][1] = rt;
                    reg_table[0][2] = rd;
                    reg_table[0][3] = shamt;
                    delete[] func;
                    delete[] ch_rs;
                    delete[] ch_rt;
                    delete[] ch_rd;
                    delete[] ch_shamt;
                    //fout1 << "r_type" << label[i] << endl;
                }
                else if(str_opcode.compare("000010")== 0) // jump
                {
                    char *addrt = new char[26]();
                    addrt[26] = '\0';
                    memcpy(addrt,temp_bin+6,26);
                    string temp_str = addrt;
                    delete[] addrt;
                    int addr = convert_dex(temp_str);
                    int jal_mark = 0;
                    char *temp_move_1 = new char[2]();
                    temp_move_1[2] = '\0';
                    memcpy(temp_move_1,temp_bin,2);
                    char *temp_move_2 = new char[24]();
                    temp_move_2[24] = '\0';
                    memcpy(temp_move_2,temp_bin+8,24);
                    char real_add[26] = {'0'};
                    strcpy(real_add, temp_move_2);
                    strcat(real_add, temp_move_1);
                    int ch_real_addr = convert_dex(real_add);
                    string real_addr = to_str(ch_real_addr);
                    new_ins[1] = "j";
                    reg_table[0][3] = real_addr;
                    /*
                    jal_mark = ins_j(real_addr);
                    i = i - jal_mark-1;*/
                    delete[] temp_move_1;
                    delete[] temp_move_2;
                    delete[] addrt;
                }
                else if(str_opcode.compare("000011") == 0)
                {
                    char *addrt = new char[26]();
                    addrt[26] = '\0';
                    memcpy(addrt,temp_bin+6,26);
                    string temp_str = addrt;
                    delete[] addrt;
                    int addr = convert_dex(temp_str);
                    int jal_mark = 0;
                    char *temp_move_1 = new char[2]();
                    temp_move_1[2] = '\0';
                    memcpy(temp_move_1,temp_bin,2);
                    char *temp_move_2 = new char[24]();
                    temp_move_2[24] = '\0';
                    memcpy(temp_move_2,temp_bin+8,24);
                    char real_add[26] = {'0'};
                    strcpy(real_add, temp_move_2);
                    strcat(real_add, temp_move_1);
                    int ch_real_addr = convert_dex(real_add);
                    string real_addr = to_str(ch_real_addr);
                    new_ins[1] = "jal";
                    reg_table[0][3] = real_addr;
                    /*jal_mark = ins_jal(real_addr);
                    i = i + jal_mark;*/
                    delete[] temp_move_1;
                    delete[] temp_move_2;
                    delete[] addrt;
                }
                else if(str_opcode.compare("111111") == 0)
                {
                    //cout << new_ins[0] << endl;
                    new_ins[1] = "HALT";
                }
                else
                {

                    int new_block = 0;
                    char *ch_rs = new char[5]();
                    ch_rs[5] = '\0';
                    memcpy(ch_rs,temp_bin+6,5);
                    string rs(ch_rs);
                    char *ch_rt = new char[5]();
                    ch_rt[5] = '\0';
                    memcpy(ch_rt,temp_bin+11,5);
                    string rt(ch_rt);
                    char *a1 = new char[16]();
                    a1[16] = '\0';
                    memcpy(a1,temp_bin+16,16);
                    int ch_imm_unsigned = convert_dex(a1);
                    int ch_imm_signed = 0;
                    if(ch_imm_unsigned > 32767)
                    {
                        ch_imm_signed = (int16_t) ch_imm_unsigned;
                    }
                    else
                        ch_imm_signed = ch_imm_unsigned;
                    string imm_unsigned = int_to_str(ch_imm_unsigned);
                    string imm_signed = int_to_str(ch_imm_signed);
                    string ins = "";
                    ins = conver_to_decode(2, opcode, null);
                    //cout << ins << endl;
                    //system("PAUSE");
                    new_ins[1] = ins;
                    reg_table[0][0] = rs;
                    reg_table[0][1] = rt;
                    //cout << "sign = "<< imm_signed << endl;
                    //cout << "unsign = "<< imm_unsigned << endl;
                    if(ins == "addi")
                        reg_table[0][3] = imm_signed;
                    if(ins == "addiu")
                        reg_table[0][3] = imm_signed;
                    if(ins == "lw")
                    {
                        reg_table[0][3] = imm_signed;
                    }
                    if(ins == "lh")
                        reg_table[0][3] = imm_signed;
                    if(ins == "lhu")
                        reg_table[0][3] = imm_unsigned;
                    if(ins == "lb")
                        reg_table[0][3] = imm_signed;
                    if(ins == "lbu")
                        reg_table[0][3] = imm_unsigned;
                    if(ins == "sw")
                        reg_table[0][3] = imm_signed;
                    if(ins == "sh")
                        reg_table[0][3] = imm_signed;
                    if(ins == "sb")
                        reg_table[0][3] = imm_signed;
                    if(ins == "lui")
                        reg_table[0][3] = imm_signed;
                    //ins_lui(rt,imm_signed);
                    if(ins == "andi")
                        reg_table[0][3] = imm_unsigned;
                    //ins_andi(rs,rt,imm_unsigned);
                    if(ins == "ori")
                        reg_table[0][3] = imm_unsigned;
                    //ins_ori(rs,rt,imm_unsigned);
                    if(ins == "nori")
                        reg_table[0][3] = imm_unsigned;
                    //ins_nori(rs,rt,imm_unsigned);
                    if(ins == "slti")
                        reg_table[0][3] = imm_signed;
                    //ins_slti(rs,rt,imm_signed);
                    if(ins == "beq")
                        reg_table[0][3] = imm_signed;
                    //new_block = ins_beq(rs,rt,imm_signed);
                    if(ins == "bne")
                        reg_table[0][3] = imm_signed;
                    //new_block = ins_bne(rs,rt,imm_signed);
                    if(ins == "bgtz")
                        reg_table[0][3] = imm_signed;
                    //new_block = ins_bgtz(rs,imm_signed);
                    delete[] a1;
                    delete[] ch_rs;
                    delete[] ch_rt;
                }
                delete[] opcode;
                free(temp_bin);

            }
        }
        if(initial != 1)
        {
            five_stage(stage, new_ins,reg_table, result_buffer, stall_inf ,error_table, write_table, i);

        }
        if(initial == 1)
        {
            stage[0] = new_ins[0];
            fout1 << "IF: " << stage[0] << endl;
            fout1 << "ID: " << upper(stage[1]) << endl;
            fout1 << "EX: " << upper(stage[2]) << endl;
            fout1 << "DM: " << upper(stage[3]) << endl;
            fout1 << "WB: " << upper(stage[4]) << endl;
            initial = 0;
        }
        if(halt == 5)
        {
            fout1 << endl;
            fout1 << endl;
            break;
        }

        if(i == -1)
        {
            fout1 << endl;
            fout1 << endl;
            break;
        }

        i++;
    }
    d_mem.clear();
    d_bin.clear();
    #undef mult_sign
    #undef mult_unsign
    fout1.clear();
    fout1.close();
    fout2.clear();
    fout2.close();
}
