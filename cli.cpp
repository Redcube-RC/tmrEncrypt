#include <QString>
#include <QStringList>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QCoreApplication>

#include <string>
#include <vector>
#include <cmath>
#include <random>
#include <algorithm>
#include <iostream>
#include <windows.h>

#include "cli.h"
#include "aes.h"

using namespace std;

static const long long pow62[10] = {1, 62, 3844, 238328, 14776336, 916132832, 56800235584, 3521614606208, 218340105584896, 13537086546263552};
static const long pow16[8] = {1, 16, 256, 4096, 65536, 1048576, 16777216, 268435456};

static string lggen[36] = { "灵灵","灵感","灵菇","灵哩",
                          "感灵","感感","感菇","感哩",
                          "菇灵","菇感","菇菇","菇哩",
                          "哩灵","哩感","哩菇","哩哩",
                          "灵刮","灵擦","感刮","感擦",
                          "菇刮","菇擦","哩刮","哩擦",
                          "刮灵","刮感","刮菇","刮哩",
                          "擦灵","擦感","擦菇","擦哩",
                          "刮刮","刮擦","擦刮","擦擦" };
static string ggggen[16] = { "咕咕咕咕","咕咕咕嘎","咕咕嘎咕","咕咕嘎嘎",
                           "咕嘎咕咕","咕嘎咕嘎","咕嘎嘎咕","咕嘎嘎嘎",
                           "嘎咕咕咕","嘎咕咕嘎","嘎咕嘎咕","嘎咕嘎嘎",
                           "嘎嘎咕咕","嘎嘎咕嘎","嘎嘎嘎咕","嘎嘎嘎嘎" };

static int lgg(string a) {
    for (int i = 0; i <= 35; i++) {
        if (lggen[i] == a) {
            if (i >= 16 && i <= 31) return i - 16;
            else return i;
        }
    }
    return -1;
}

static int gggg(string a) {
    for (int i = 0; i <= 15; i++) {
        if (ggggen[i] == a) {
            return i;
        }
    }
    return -1;
}

static int keyToNumber(char a) {
    int num = static_cast<int>(a);
    if (num >= 48 && num <= 57) return num - 48;
    else if (num >= 65 && num <= 90) return num - 55;
    else if (num >= 97 && num <= 122) return num - 61;
    else return 0;
}

static int awa(int a) {
    if (a >= 16) return a - 16;
    else if (a < 0) return a + 16;
    else return a;
}

static int rd(int a, int b) {
    static random_device rd;
    static mt19937 gen(rd());
    uniform_int_distribution<int> dist(a, b);
    return dist(gen);
}

static string key_translater_3(string keyInputRaw) {
    long long keyNum = 0;
    string keyinput = keyInputRaw + "                       ";
    for (int i = 0; i < 10; i++) {
        keyNum += keyToNumber(keyinput[i]) * pow62[i];
    }
    string key = int_to_chs(keyNum) + "";
    key = string(15 - key.length(), '0') + key;
    char keyCarrying = key[0];
    key.erase(0, 1);
    keyNum = 0;
    for (int i = 10; i < 20; i++) {
        keyNum += keyToNumber(keyinput[i]) * pow62[i - 10];
    }
    key = int_to_chs(keyNum + ch_to_int(keyCarrying)) + key;
    key = string(29 - key.length(), '0') + key;
    keyCarrying = key[0];
    key.erase(0, 1);
    keyNum = 0;
    for (int i = 20; i < 24; i++) {
        keyNum += keyToNumber(keyinput[i]) * pow62[i - 20];
    }
    key = int_to_chs(keyNum + ch_to_int(keyCarrying)) + key;
    if (key.length() < 32) key = string(32 - key.length(), '0') + key;
    else if (key.length() > 32) key.erase(0, key.length() - 32);
    return key;
}

static string cli_lggen_5(u32string& u32input, string& key){
    vector<int> a;
    string done;
    long numCount = 0; // 计算总的数字个数

    for (long i = 0; i < u32input.length(); i++) {
        int ch = static_cast<uint32_t>(u32input[i]);
        if (ch >= 65536) {
            numCount += 2;
            a.push_back(0x2FE0 + (ch/0x10000)%10);
        }else{
            numCount++;
        }
        a.push_back(ch%0x10000);
    }

    for (int i = 0; i < (8-numCount%8)%8; i++){
        a.push_back(0);
    }

    for (int i = 0; i < static_cast<int>(a.size())/8; i++){
        string plain_text = "";
        for(int j = 0; j < 8; j++){
            string chs = int_to_chs(a[8*i+j]);
            chs = string(4 - chs.length(), '0') + chs;
            plain_text += chs;
        }
        string aesResult = aes(plain_text, key);
        for(int j = 0; j < 32; j++){
            done += lggen[ch_to_int(aesResult[j]) + rd(0, 1)*16];
        }
    }

    return done;
}

static string cli_de_5(string& input, string& key){
    string done;
    vector<int> a;
    u32string u32input = QString::fromUtf8(input).toStdU32String();
    if (input.find("灵") != string::npos || input.find("感") != string::npos || input.find("菇") != string::npos ||
        input.find("擦") != string::npos || input.find("刮") != string::npos || input.find("哩") != string::npos) {
        for (int i = 0; i < u32input.size() / 64; i++){
            string aesResult;
            for(int j = 0; j < 32; j++){
                int num = lgg(QString::fromStdU32String(u32input.substr(i*64+j*2, 2)).toStdString());
                aesResult += (num == 0) ? "0" : int_to_chs(num);
            }
            string plain_text = in_aes(aesResult, key);
            for(int j = 0; j < 8; j++){
                a.push_back(str_long(plain_text.substr(j*4, 4)));
            }
        }

        for (int i = 0; i < u32input.size() / 8; ){
            uint32_t result;
            if (a[i] <= 0x2FEF && a[i] >= 0x2FE1){
                result = (a[i] % 16) * 0x10000 + a[i+1];
                i += 2;
            }else if(a[i] == 0x2FE0){
                result = 0x100000 + a[i+1];
                i += 2;
            }else{
                result = a[i];
                i++;
            }
            char32_t ch = static_cast<char32_t>(result);
            QString qstr = QString::fromUcs4(&ch, 1);
            if (result != 0) done += qstr.toStdString();
        }
    }
    else if (input.find("咕") != string::npos || input.find("嘎") != string::npos) {
        for (int i = 0; i < u32input.size() / 128; i++){
            string aesResult;
            for(int j = 0; j < 32; j++){
                int num = gggg(QString::fromStdU32String(u32input.substr(i*128+j*4, 4)).toStdString());
                aesResult += (num == 0) ? "0" : int_to_chs(num);
            }
            string plain_text = in_aes(aesResult, key);
            for(int j = 0; j < 8; j++){
                a.push_back(str_long(plain_text.substr(j*4, 4)));
            }
        }

        for (int i = 0; i < u32input.size() / 16; ){
            uint32_t result;
            if (a[i] <= 0x2FEF && a[i] >= 0x2FE1){
                result = (a[i] % 16) * 0x10000 + a[i+1];
                i += 2;
            }else if(a[i] == 0x2FE0){
                result = 0x100000 + a[i+1];
                i += 2;
            }else{
                result = a[i];
                i++;
            }
            char32_t ch = static_cast<char32_t>(result);
            QString qstr = QString::fromUcs4(&ch, 1);
            if (result != 0) done += qstr.toStdString();
        }
    }
    return done;
}

static string cli_ggggen_5(u32string& u32input, string& key){
    vector<int> a;
    string done;
    long numCount = 0; // 计算总的数字个数

    for (long i = 0; i < u32input.length(); i++) {
        int ch = static_cast<uint32_t>(u32input[i]);
        if (ch >= 65536) {
            numCount += 2;
            a.push_back(0x2FE0 + (ch/0x10000)%10);
        }else{
            numCount++;
        }
        a.push_back(ch%0x10000);
    }

    for (int i = 0; i < (8-numCount%8)%8; i++){
        a.push_back(0);
    }

    for (int i = 0; i < static_cast<int>(a.size())/8; i++){
        string plain_text = "";
        for(int j = 0; j < 8; j++){
            string chs = int_to_chs(a[8*i+j]);
            chs = string(4 - chs.length(), '0') + chs;
            plain_text += chs;
        }
        string aesResult = aes(plain_text, key);
        for(int j = 0; j < 32; j++){
            done += ggggen[ch_to_int(aesResult[j]) + rd(0, 1)*16];
        }
    }

    return done;
}

static string cli_lggen_4(u32string& u32input, string& key) {
    // 分割明文
    vector<int> a(u32input.length());
    long numCount = 0; // 计算总的数字个数

    for (long i = 0; i < u32input.length(); i++) {
        char32_t ch = u32input[i];
        a[i] = static_cast<uint32_t>(ch);
        numCount += (a[i] >= 65536) ? 6 : 4;
    }

    vector<int> num(numCount);
    long count = 0; // 索引
    for (long j = 0; j < u32input.length(); j++) {
        if (a[j] >= 65536) {
            num[count] = (a[j] <= 1048575) ? rd(32, 33) : rd(34, 35);
            count++;
            for (int i = 4; i >= 0; i--, count++) {
                int result = a[j] / pow16[i];
                num[count] = result;
                a[j] -= pow16[i] * result;
            }
        }
        else {
            for (int i = 3; i >= 0; i--, count++) {
                int result = a[j] / pow16[i];
                num[count] = result;
                a[j] -= pow16[i] * result;
            }
        }
    }

    // 生成偏移表
    vector<int> shiftList(numCount);
    long shiftCount = 0;
    for (long i = 0; shiftCount < numCount; i++) {
        string plain_text = int_to_chs(i);
        plain_text = string(32 - plain_text.length(), '0') + plain_text;
        string aesResult = aes(plain_text, key);
        for (int j = 0; j < 32 && shiftCount < numCount; j++, shiftCount++) {
            shiftList[shiftCount] = ch_to_int(aesResult[j]);
        }
    }

    // 进行偏移
    count = 0; // 索引
    for (long i = 0; i < numCount; i++) {
        if (num[i] < 32) {
            num[i] = (num[i] + shiftList[count]) % 16;
            count++;
        }
    }

    // 生成交换表
    vector<int> exchangeList(numCount);
    long exchangeCount = 0;
    for (int i = 0; exchangeCount < numCount; i++) {
        string plain_text = int_to_chs(i);
        plain_text = string(32 - plain_text.length(), '0') + plain_text;
        string aesResult = aes(plain_text, key);
        int exchangeLength = ceil(log2(static_cast<double>(numCount)) / 4); // 交换表中每个元素的十六进制位数
        for (int j = 0; j <= 32 / exchangeLength && exchangeCount < numCount; j++, exchangeCount++) {
            exchangeList[exchangeCount] = str_long(aesResult.substr(j * exchangeLength, exchangeLength));
        }
    }

    // 进行交换
    for (long i = 0; i < numCount; i++) {
        long exchangeIndex = exchangeList[i] % numCount;
        swap(num[i], num[exchangeIndex]);
    }

    // 合并
    string result;
    for (long i = 0; i < numCount; i++) {
        if (num[i] < 32) {
            result += lggen[num[i] + rd(0, 1) * 16];
        }
        else {
            result += lggen[num[i]];
        }
    }

    return result;
}

static string cli_de_4(string& input, string& key) {
    string done;
    long numCount;
    u32string u32input = QString::fromUtf8(input).toStdU32String();
    if (input.find("灵") != string::npos || input.find("感") != string::npos || input.find("菇") != string::npos ||
        input.find("擦") != string::npos || input.find("刮") != string::npos || input.find("哩") != string::npos) {
        numCount = u32input.size() / 2;
        vector<int> num(numCount);
        vector<long> exchangeList(numCount);
        vector<int> shiftList(numCount);

        bool InputCorrect = true;
        for (long i = 0; i < numCount; i++) {
            num[i] = lgg(QString::fromStdU32String(u32input.substr(i * 2, 2)).toStdString());
            if (num[i] == -1) InputCorrect = false;
        }

        // 生成交换表
        long exchangeCount = 0;
        for (int i = 0; exchangeCount < numCount; i++) {
            string plain_text = int_to_chs(i);
            plain_text = string(32 - plain_text.length(), '0') + plain_text;
            string aesResult = aes(plain_text, key);
            int exchangeLength = ceil(log2(static_cast<double>(numCount)) / 4); // 交换表中每个元素的十六进制位数
            for (int j = 0; j <= 32 / exchangeLength && exchangeCount < numCount; j++, exchangeCount++) {
                exchangeList[exchangeCount] = str_long(aesResult.substr(j * exchangeLength, exchangeLength));
            }
        }

        // 进行交换
        for (long i = numCount - 1; i >= 0; i--) {
            long exchangeIndex = exchangeList[i] % numCount;
            swap(num[i], num[exchangeIndex]);
        }

        // 生成偏移表
        long shiftCount = 0;
        for (long i = 0; shiftCount < numCount; i++) {
            string plain_text = int_to_chs(i);
            plain_text = string(32 - plain_text.length(), '0') + plain_text;
            string aesResult = aes(plain_text, key);
            for (int j = 0; j < 32 && shiftCount < numCount; j++, shiftCount++) {
                shiftList[shiftCount] = ch_to_int(aesResult[j]);
            }
        }

        // 进行偏移
        long count = 0; // 索引
        for (long i = 0; i < numCount; i++) {
            if (num[i] < 32) {
                num[i] = awa(num[i] - shiftList[count]);
                count++;
            }
        }

        if (InputCorrect) {
            for (int i = 0; i < numCount; ) {
                if (num[i] >= 32) {
                    i++;
                    uint32_t result = ((num[i] <= 32) ? 0 : 0x100000), index = 4;
                    for (int j = i; j <= i + 4; j++) {
                        result += num[j] * pow16[index];
                        index--;
                    }
                    i += 5;
                    char32_t ch = static_cast<char32_t>(result);
                    QString qstr = QString::fromUcs4(&ch, 1);
                    done += qstr.toStdString();

                }
                else {
                    uint32_t result = 0, index = 3;
                    for (int j = i; j <= i + 3; j++) {
                        result += num[j] * pow16[index];
                        index--;
                    }
                    i += 4;
                    char32_t ch = static_cast<char32_t>(result);
                    QString qstr = QString::fromUcs4(&ch, 1);
                    done += qstr.toStdString();
                }
            }
        }
    }
    else if (input.find("咕") != string::npos || input.find("嘎") != string::npos) {
        numCount = u32input.size() / 4;
        vector<int> num(numCount);
        vector<long> exchangeList(numCount);
        vector<int> shiftList(numCount);

        bool InputCorrect = true;
        for (long i = 0; i < numCount; i++) {
            num[i] = gggg(QString::fromStdU32String(u32input.substr(i * 4, 4)).toStdString());
            if (num[i] == -1) InputCorrect = false;
        }

        // 生成交换表
        long exchangeCount = 0;
        for (int i = 0; exchangeCount < numCount; i++) {
            string plain_text = int_to_chs(i);
            plain_text = string(32 - plain_text.length(), '0') + plain_text;
            string aesResult = aes(plain_text, key);
            int exchangeLength = ceil(log2(static_cast<double>(numCount)) / 4); // 交换表中每个元素的十六进制位数
            for (int j = 0; j <= 32 / exchangeLength && exchangeCount < numCount; j++, exchangeCount++) {
                exchangeList[exchangeCount] = str_long(aesResult.substr(j * exchangeLength, exchangeLength));
            }
        }

        // 进行交换
        for (long i = numCount - 1; i >= 0; i--) {
            long exchangeIndex = exchangeList[i] % numCount;
            swap(num[i], num[exchangeIndex]);
        }

        // 生成偏移表
        long shiftCount = 0;
        for (long i = 0; shiftCount < numCount; i++) {
            string plain_text = int_to_chs(i);
            plain_text = string(32 - plain_text.length(), '0') + plain_text;
            string aesResult = aes(plain_text, key);
            for (int j = 0; j < 32 && shiftCount < numCount; j++, shiftCount++) {
                shiftList[shiftCount] = ch_to_int(aesResult[j]);
            }
        }

        // 进行偏移
        for (long i = 0; i < numCount; i++) { num[i] = awa(num[i] - shiftList[i]); }

        if (InputCorrect) {
            for (int i = 0; i < numCount; ) {
                uint32_t result = 0, index = 3;
                for (int j = i; j <= i + 3; j++) {
                    result += num[j] * pow16[index];
                    index--;
                }
                if (result >= 0x2FE0 && result <= 0x2FEF) {
                    result = ((result <= 0x2FE7) ? 0 : 0x100000), index = 4;
                    i += 4;
                    for (int j = i; j <= i + 4; j++) {
                        result += num[j] * pow16[index];
                        index--;
                    }
                    i += 5;
                }
                else {
                    i += 4;
                }
                char32_t ch = static_cast<char32_t>(result);
                QString qstr = QString::fromUcs4(&ch, 1);
                done += qstr.toStdString();
            }
        }
    }
    return done;
}

static string cli_ggggen_4(u32string& u32input, string& key) {
    vector<int> a(u32input.length());
    string done;
    long numCount = 0; // 计算总的数字个数

    for (long i = 0; i < u32input.length(); i++) {
        char32_t ch = u32input[i];
        a[i] = static_cast<uint32_t>(ch);
        numCount += (a[i] >= 65536) ? 9 : 4;
    }

    vector<int> num(numCount);
    long count = 0; // 索引
    for (int j = 0; j < u32input.length(); j++) {
        if (a[j] >= 65536) {
            num[count] = 2, num[count + 1] = 15, num[count + 2] = 14, num[count + 3] = (a[j] <= 1048575) ? rd(0, 7) : rd(8, 15);
            count += 4;
            for (int i = 4; i >= 0; i--, count++) {
                int result = a[j] / pow16[i];
                num[count] = result;
                a[j] -= pow16[i] * result;
            }
        }
        else {
            for (int i = 3; i >= 0; i--, count++) {
                int result = a[j] / pow16[i];
                num[count] = result;
                a[j] -= pow16[i] * result;
            }
        }
    }

    // 生成偏移表
    vector<int> shiftList(numCount);
    long shiftCount = 0;
    for (long i = 0; shiftCount < numCount; i++) {
        string plain_text = int_to_chs(i);
        plain_text = string(32 - plain_text.length(), '0') + plain_text;
        string aesResult = aes(plain_text, key);
        for (int j = 0; j < 32 && shiftCount < numCount; j++, shiftCount++) {
            shiftList[shiftCount] = ch_to_int(aesResult[j]);
        }
    }

    // 进行偏移
    for (long i = 0; i < numCount; i++) { num[i] = (num[i] + shiftList[i]) % 16; }

    // 生成交换表
    vector<int> exchangeList(numCount);
    long exchangeCount = 0;
    for (int i = 0; exchangeCount < numCount; i++) {
        string plain_text = int_to_chs(i);
        plain_text = string(32 - plain_text.length(), '0') + plain_text;
        string aesResult = aes(plain_text, key);
        int exchangeLength = ceil(log2(static_cast<double>(numCount)) / 4); // 交换表中每个元素的十六进制位数
        for (int j = 0; j <= 32 / exchangeLength && exchangeCount < numCount; j++, exchangeCount++) {
            exchangeList[exchangeCount] = str_long(aesResult.substr(j * exchangeLength, exchangeLength));
        }
    }

    // 进行交换
    for (long i = 0; i < numCount; i++) {
        long exchangeIndex = exchangeList[i] % numCount;
        swap(num[i], num[exchangeIndex]);
    }

    // 合并
    string result;
    for (long i = 0; i < numCount; i++) { result += ggggen[num[i]]; }

    return result;
}

static string cli_lggen_3(u32string& u32input, string& key) {
    // 分割明文
    vector<long> a(u32input.length());
    string done;

    for (int i = 0; i < u32input.length(); i++) {
        char32_t ch = u32input[i];
        a[i] = static_cast<uint32_t>(ch);
    }

    for (int j = 0; j < u32input.length(); j++) {
        if (a[j] >= 65536) {
            done += lggen[(a[j] <= 1048575) ? rd(32, 33) : rd(34, 35)];
            for (int i = 4; i >= 0; i--) {
                int result = a[j] / pow16[i];
                done += lggen[result + rd(0, 1) * 16];
                a[j] -= pow16[i] * result;
            }
        }
        else {
            for (int i = 3; i >= 0; i--) {
                int result = a[j] / pow16[i];
                done += lggen[result + rd(0, 1) * 16];
                a[j] -= pow16[i] * result;
            }
        }
    }
    vector<string> donechar(done.length() / 3);
    vector<int> exchangeList(done.length() / 3);
    for (int i = 0; i < done.length() / 3; i++) {
        // 分割密文
        donechar[i] = done.substr(i * 3, 3);
        // 生成交换表
        string plain_text = int_to_chs(i);
        plain_text = string(32 - plain_text.length(), '0') + plain_text;
        exchangeList[i] = (str_long(aes(plain_text, key)) % (done.length() / 3) + (done.length() / 3)) % (done.length() / 3);
    }
    // 交换
    for (int i = 0; i < done.length() / 3; i++) {
        string exchangeCache1 = donechar[i];
        string exchangeCache2 = donechar[exchangeList[i]];
        donechar[i] = exchangeCache2;
        donechar[exchangeList[i]] = exchangeCache1;
    }
    // 合并
    string result = "";
    for (int i = 0; i < done.length() / 3; i++) {
        result += donechar[i];
    }
    return result;
}

static string cli_de_3(string& inputCache, string& key) {
    string done;
    vector<string> inputchar(inputCache.length() / 3);
    vector<int> exchangeList(inputCache.length() / 3);
    for (int i = 0; i < inputCache.length() / 3; i++) {
        // 分割密文
        inputchar[i] = inputCache.substr(i * 3, 3);
        // 生成交换表
        string plain_text = int_to_chs(i);
        plain_text = string(32 - plain_text.length(), '0') + plain_text;
        exchangeList[i] = (str_long(aes(plain_text, key)) % (inputCache.length() / 3) + (inputCache.length() / 3)) % (inputCache.length() / 3);
    }
    // 交换
    for (int i = inputCache.length() / 3 - 1; i >= 0; i--) {
        string exchangeCache1 = inputchar[i];
        string exchangeCache2 = inputchar[exchangeList[i]];
        inputchar[i] = exchangeCache2;
        inputchar[exchangeList[i]] = exchangeCache1;
    }
    // 合并
    string input = "";
    for (int i = 0; i < inputCache.length() / 3; i++) {
        input += inputchar[i];
    }

    bool InputCorrect = true;
    if (input.find("灵") != string::npos || input.find("感") != string::npos || input.find("菇") != string::npos ||
        input.find("擦") != string::npos || input.find("刮") != string::npos || input.find("哩") != string::npos) {
        if (input.length() % 6 != 0) InputCorrect = false;
        else {
            vector<string> de(input.length() / 6);
            for (int i = 0; i < input.length() / 6; i++) {
                de[i] = input.substr(6 * i, 6);
                if (lgg(de[i]) == -1) InputCorrect = false;
            }
            for (int i = 0; i < input.length() / 6; ) {
                if (lgg(de[i]) >= 32) {
                    i++;
                    uint32_t result = ((lgg(de[i]) <= 33) ? 0 : 0x100000), index = 4;
                    for (int j = i; j <= i + 4; j++) {
                        result += lgg(de[j]) * pow16[index];
                        index--;
                    }
                    i += 5;
                    if (InputCorrect) {
                        char32_t ch = static_cast<char32_t>(result);
                        QString qstr = QString::fromUcs4(&ch, 1);
                        done += qstr.toStdString();
                    }

                }
                else {
                    uint32_t result = 0, index = 3;
                    for (int j = i; j <= i + 3; j++) {
                        result += lgg(de[j]) * pow16[index];
                        index--;
                    }
                    i += 4;
                    if (InputCorrect) {
                        char32_t ch = static_cast<char32_t>(result);
                        QString qstr = QString::fromUcs4(&ch, 1);
                        done += qstr.toStdString();
                    }
                }
            }
        }
    }
    else if (input.find("咕") != string::npos || input.find("嘎") != string::npos) {
        if (input.length() % 12 != 0) InputCorrect = false;
        else {
            vector<string> de(input.length() / 12);
            for (int i = 0; i < input.length() / 12; i++) {
                de[i] = input.substr(12 * i, 12);
                if (gggg(de[i]) == -1) InputCorrect = false;
            }
            for (int i = 0; i < input.length() / 12; ) {
                uint32_t result = 0, index = 3;
                for (int j = i; j <= i + 3; j++) {
                    result += gggg(de[j]) * pow16[index];
                    index--;
                }
                if (result >= 0x2FE0 && result <= 0x2FEF) {
                    result = ((result <= 0x2FE7) ? 0 : 0x100000), index = 4;
                    i += 4;
                    for (int j = i; j <= i + 4; j++) {
                        result += gggg(de[j]) * pow16[index];
                        index--;
                    }
                    i += 5;
                }
                else {
                    i += 4;
                }
                if (InputCorrect) {
                    char32_t ch = static_cast<char32_t>(result);
                    QString qstr = QString::fromUcs4(&ch, 1);
                    done += qstr.toStdString();
                }
            }
        }
    }
    return done;
}

static string cli_ggggen_3(u32string& u32input, string& key) {
    vector<long> a(u32input.length());
    string done;

    for (int i = 0; i < u32input.length(); i++) {
        char32_t ch = u32input[i];
        a[i] = static_cast<uint32_t>(ch);
    }
    for (int j = 0; j < u32input.length(); j++) {
        if (a[j] >= 65536) {
            done += ggggen[2] + ggggen[15] + ggggen[14] + ggggen[(a[j] <= 1048575) ? rd(0, 7) : rd(8, 15)];
            for (int i = 4; i >= 0; i--) {
                int result = a[j] / pow16[i];
                done += ggggen[result];
                a[j] -= pow16[i] * result;
            }
        }
        else {
            for (int i = 3; i >= 0; i--) {
                int result = a[j] / pow16[i];
                done += ggggen[result];
                a[j] -= pow16[i] * result;
            }
        }
    }

    vector<string> donechar(done.length() / 3);
    vector<int> exchangeList(done.length() / 3);
    for (int i = 0; i < done.length() / 3; i++) {
        // 分割密文
        donechar[i] = done.substr(i * 3, 3);
        // 生成交换表
        string plain_text = int_to_chs(i);
        plain_text = string(32 - plain_text.length(), '0') + plain_text;
        exchangeList[i] = (str_long(aes(plain_text, key)) % (done.length() / 3) + (done.length() / 3)) % (done.length() / 3);
    }
    // 交换
    for (int i = 0; i < done.length() / 3; i++) {
        string exchangeCache1 = donechar[i];
        string exchangeCache2 = donechar[exchangeList[i]];
        donechar[i] = exchangeCache2;
        donechar[exchangeList[i]] = exchangeCache1;
    }
    // 合并
    string result = "";
    for (int i = 0; i < done.length() / 3; i++) {
        result += donechar[i];
    }
    return result;
}

static string cli_ggggen_2(u32string& u32input, string& keyEditinput) {
    int keyinput = 0;

    try {
        keyinput = stoi(keyEditinput);
    }
    catch (const invalid_argument& e) {
        cerr << "错误：密钥必须是整数" << endl;
    }

    if (keyinput < 0 || keyinput > 65535) keyinput = 0;
    int key[4];
    for (int i = 3; i >= 0; i--) {
        key[i] = (int)(keyinput / pow16[i]);
        keyinput -= key[i] * pow16[i];
    }

    vector<long> a(u32input.length());
    string done;

    for (int i = 0; i < u32input.length(); i++) {
        char32_t ch = u32input[i];
        a[i] = static_cast<uint32_t>(ch);
    }


    for (int j = 0; j < u32input.length(); j++) {
        if (a[j] > 65536) {
            done += ggggen[awa(2 + key[3])] + ggggen[awa(15 + key[2])] + ggggen[awa(14 + key[1])] + ggggen[rd(0, 15)];
            for (int i = 7; i >= 0; i--) {
                int result = a[j] / pow16[i];
                if (i >= 4) result += key[i - 4];
                else result += key[i];
                if (result >= 16) result -= 16;
                done += ggggen[result];
                a[j] -= pow16[i] * ((int)(a[j] / pow16[i]));
            }
        }
        else {
            for (int i = 3; i >= 0; i--) {
                int result = a[j] / pow16[i] + key[i];
                if (result >= 16) result -= 16;
                done += ggggen[result];
                a[j] -= pow16[i] * ((int)(a[j] / pow16[i]));
            }
        }
    }
    return done;
}

static string cli_de_2(string& input, string& keyEditinput) {
    int keyinput = 0;

    try {
        keyinput = stoi(keyEditinput);
    }
    catch (const invalid_argument& e) {
        cerr << "错误：密钥必须是整数" << endl;
    }

    if (keyinput < 0 || keyinput > 65535) keyinput = 0;
    int key[4];
    for (int i = 3; i >= 0; i--) {
        key[i] = (int)(keyinput / pow16[i]);
        keyinput -= key[i] * pow16[i];
    }
    
    string done;

    bool InputCorrect = true;
    if (input.find("灵") != string::npos || input.find("感") != string::npos || input.find("菇") != string::npos ||
        input.find("擦") != string::npos || input.find("刮") != string::npos || input.find("哩") != string::npos) {
        if (input.length() % 6 != 0) InputCorrect = false;
        else {
            vector<string> de(input.length() / 6);
            for (int i = 0; i < input.length() / 6; i++) {
                de[i] = input.substr(6 * i, 6);
                if (lgg(de[i]) == -1) InputCorrect = false;
            }
            for (int i = 0; i < input.length() / 6; ) {
                if (lgg(de[i]) >= 32) {
                    i++;
                    uint32_t result = 0, index = 7;
                    for (int j = i; j <= i + 7; j++) {
                        if (index >= 4) result += awa(lgg(de[j]) - key[index - 4]) * pow16[index];
                        else result += awa(lgg(de[j]) - key[index]) * pow16[index];
                        index--;
                    }
                    i += 8;
                    if (InputCorrect) {
                        char32_t ch = static_cast<char32_t>(result);
                        QString qstr = QString::fromUcs4(&ch, 1);
                        done += qstr.toStdString();
                    }

                }
                else {
                    uint32_t result = 0, index = 3;
                    for (int j = i; j <= i + 3; j++) {
                        result += awa(lgg(de[j]) - key[index]) * pow16[index];
                        index--;
                    }
                    i += 4;
                    if (InputCorrect) {
                        char32_t ch = static_cast<char32_t>(result);
                        QString qstr = QString::fromUcs4(&ch, 1);
                        done += qstr.toStdString();
                    }
                }
            }
        }
    }
    else if (input.find("咕") != string::npos || input.find("嘎") != string::npos) {
        if (input.length() % 12 != 0) InputCorrect = false;
        else {
            vector<string> de(input.length() / 12);
            for (int i = 0; i < input.length() / 12; i++) {
                de[i] = input.substr(12 * i, 12);
                if (gggg(de[i]) == -1) InputCorrect = false;
            }
            for (int i = 0; i < input.length() / 12; ) {
                uint32_t result = 0, index = 3;
                for (int j = i; j <= i + 3; j++) {
                    result += awa(gggg(de[j]) - key[index]) * pow16[index];
                    index--;
                }
                if (result >= 12256 && result <= 12271) {
                    result = 0, index = 7;
                    i += 4;
                    for (int j = i; j <= i + 7; j++) {
                        if (index >= 4) result += awa(gggg(de[j]) - key[index - 4]) * pow16[index];
                        else result += awa(gggg(de[j]) - key[index]) * pow16[index];
                        index--;
                    }
                    i += 8;
                }
                else {
                    i += 4;
                }
                if (InputCorrect) {
                    char32_t ch = static_cast<char32_t>(result);
                    QString qstr = QString::fromUcs4(&ch, 1);
                    done += qstr.toStdString();
                }
            }
        }
    }
    return done;
}

static string cli_lggen_2(u32string& u32input, string& keyEditinput) {
    int keyinput = 0;

    try {
        keyinput = stoi(keyEditinput);
    }
    catch (const invalid_argument& e) {
        cerr << "错误：密钥必须是整数" << endl;
    }

    if (keyinput < 0 || keyinput > 65535) {
        cerr << "错误：密钥必须在[0, 65535]内" << endl;
        keyinput = 0;
    }
    int key[4];
    for (int i = 3; i >= 0; i--) {
        key[i] = (int)(keyinput / pow16[i]);
        keyinput -= key[i] * pow16[i];
    }
    
    vector<long> a(u32input.length());
    string done;

    for (int i = 0; i < u32input.length(); i++) {
        char32_t ch = u32input[i];
        a[i] = static_cast<uint32_t>(ch);
    }


    for (int j = 0; j < u32input.length(); j++) {
        if (a[j] > 65536) {
            done += lggen[rd(32, 35)];
            for (int i = 7; i >= 0; i--) {
                int result = a[j] / pow16[i];
                if (i >= 4) result += key[i - 4];
                else result += key[i];
                if (result >= 16) result -= 16;
                done += lggen[result + rd(0, 1) * 16];
                a[j] -= pow16[i] * ((int)(a[j] / pow16[i]));
            }
        }
        else {
            for (int i = 3; i >= 0; i--) {
                int result = a[j] / pow16[i] + key[i];
                if (result >= 16) result -= 16;
                done += lggen[result + rd(0, 1) * 16];
                a[j] -= pow16[i] * ((int)(a[j] / pow16[i]));
            }
        }
    }
    return done;
}

int runCli(int argc, char* argv[], QCoreApplication& app) {
    SetConsoleOutputCP(CP_UTF8);
    QCommandLineParser parser;
    parser.setApplicationDescription("高松灯文本加密器5 命令行模式");

    QCommandLineOption encOpt(QStringList() << "e" << "encrypt", "加密（默认）");
    QCommandLineOption decOpt(QStringList() << "d" << "decrypt", "解密");
    QCommandLineOption modeOpt(QStringList() << "m" << "mode", "编码模式：lgg 或 gggg", "mode");
    QCommandLineOption keyOpt(QStringList() << "k" << "key", "密钥", "key");
    QCommandLineOption algoOpt(QStringList() << "a" << "algo", "算法版本：2、3、4或 5（默认 5）", "version");

    parser.addOption(encOpt);
    parser.addOption(decOpt);
    parser.addOption(modeOpt);
    parser.addOption(keyOpt);
    parser.addOption(algoOpt);
    parser.addPositionalArgument("text", "要处理的文本");

    for (const QString& a : QCoreApplication::arguments()) {
        if (a == "-h" || a == "--help" || a == "-?" || a == "--help-all" || a == "/?") {
            cout << "用法: tmr4 [选项] <文本>\n"
                    "\n"
                    "高松灯文本加密器5 命令行模式\n"
                    "\n"
                    "选项:\n"
                    "  -e, --encrypt         加密（默认）\n"
                    "  -d, --decrypt         解密\n"
                    "  -m, --mode <mode>     编码模式：lgg 或 gggg\n"
                    "  -k, --key <key>       密钥\n"
                    "  -a, --algo <version>  算法版本：2、3、4或 5（默认 5）\n"
                    "  -h, --help            显示本帮助\n"
                    "\n"
                    "示例:\n"
                    "  tmr4 --encrypt --mode lgg --key iloveu --algo 4 你好\n"
                    "  tmr4 -d -k 123 -a 2 菇擦擦哩擦感菇刮刮灵哩刮感擦哩刮\n";
            return 0;
        }
    }

    parser.process(app);

    bool encrypt = !parser.isSet(decOpt);
    if (parser.isSet(encOpt) && parser.isSet(decOpt)) {
        cerr << "错误：-e 和 -d 不能同时指定" << endl;
        return 1;
    }
    QString mode = parser.value(modeOpt);
    QString key = parser.value(keyOpt);
    QString algo = parser.value(algoOpt);
    QStringList texts = parser.positionalArguments();

    if (encrypt && (mode != "lgg" && mode != "gggg")) {
        cerr << "错误：-m 必须是 lgg 或 gggg" << endl;
        return 1;
    }

    if (algo.isEmpty()) algo = "5";  // 默认tmr5
    else if (algo != "2" && algo != "3" && algo != "4" && algo != "5") {
        cerr << "错误：-a 必须是 2、3、4或 5" << endl;
        return 1;
    }

    QString text = texts.join(" ");
    string result;

	string input = text.toStdString();
    u32string u32input = text.toStdU32String();
    string skey = key.toStdString();
    string key3 = key_translater_3(skey);
    string key2 = key.toStdString();

    if (algo == "5") {
        if (encrypt && mode == "gggg") result = cli_ggggen_5(u32input, key3);
        else if (encrypt && mode == "lgg") result = cli_lggen_5(u32input, key3);
        else result = cli_de_5(input, key3);
    }
    else if (algo == "4") {
        if (encrypt && mode == "gggg") result = cli_ggggen_4(u32input, key3);
        else if (encrypt && mode == "lgg") result = cli_lggen_4(u32input, key3);
		else result = cli_de_4(input, key3);
    }
    else if (algo == "3") {
        if (encrypt && mode == "gggg") result = cli_ggggen_3(u32input, key3);
        else if (encrypt && mode == "lgg") result = cli_lggen_3(u32input, key3);
        else result = cli_de_3(input, key3);
    }
    else if (algo == "2") {
        if (encrypt && mode == "gggg") result = cli_ggggen_2(u32input, key2);
        else if (encrypt && mode == "lgg") result = cli_lggen_2(u32input, key2);
        else result = cli_de_2(input, key2);
    }

    cout << result << endl;
    return 0;
}
