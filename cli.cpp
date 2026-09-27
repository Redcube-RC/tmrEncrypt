#include "cli.h"
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

using namespace std;

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

// 对应 MainWindow::lgg()
static int lgg(string a) {
    for (int i = 0; i <= 35; i++) {
        if (lggen[i] == a) {
            if (i >= 16 && i <= 31) return i - 16;
            else return i;
        }
    }
    return -1;
}

// 对应 MainWindow::gggg()
static int gggg(string a) {
    for (int i = 0; i <= 15; i++) {
        if (ggggen[i] == a) {
            return i;
        }
    }
    return -1;
}

static const long long pow62[10] = { 1, 62, 3844, 238328, 14776336, 916132832,
                                     56800235584, 3521614606208, 218340105584896,
                                     13537086546263552 };
static const long pow16[8] = { 1, 16, 256, 4096, 65536, 1048576, 16777216, 268435456 };

static const int S[16][16] = { 0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
                               0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
                               0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
                               0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
                               0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
                               0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
                               0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
                               0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
                               0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
                               0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
                               0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
                               0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
                               0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
                               0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
                               0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
                               0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16 };

static const long long Rcon[10] = {
    0x01000000, 0x02000000,
    0x04000000, 0x08000000,
    0x10000000, 0x20000000,
    0x40000000, 0x80000000,
    0x1b000000, 0x36000000
};

// 对应 mainwindow.cpp 的 ch_to_int / str_long / int_to_chs / keyToNumber / awa / rd
static int ch_to_int(char& ch) {
    int ans = 0;
    if (ch >= 48 && ch <= 57) ans = ch - '0';
    else if (ch >= 'a' && ch <= 'f') ans = ch - 'a' + 10;
    return ans;
}

static long long str_long(string str) {
    long long ans = 0;
    for (char ch : str) ans = ans * 16 + ch_to_int(ch);
    return ans;
}

static string int_to_chs(long long num) {
    string ans = "";
    while (num) {
        int x = num & 0xf;
        if (x <= 9) ans += char(x + '0');
        else ans += char(x - 10 + 'a');
        num >>= 4;
    }
    int left = 0, right = int(ans.length()) - 1;
    while (left < right) {
        char ch = ans[left];
        ans[left] = ans[right];
        ans[right] = ch;
        left++;
        right--;
    }
    return ans;
}

static vector<string> group_key(string& key) {
    vector<string> groups(4);
    int index = 0;
    for (string& g : groups) {
        g = key.substr(index, 8);
        index += 8;
    }
    return groups;
}

static string loop_wordbyte(string& wi_1) {
    return wi_1.substr(2) + wi_1.substr(0, 2);
}

static string wordbyte_sub(string& wi_1) {
    int len = int(wi_1.length());
    string ans = "";
    for (int i = 0; i < len; i += 2) {
        int x = ch_to_int(wi_1[i]), y = ch_to_int(wi_1[i + 1]);
        int num = S[x][y];
        string s = int_to_chs(num);
        while (s.length() < 2) s = "0" + s;
        ans += s;
    }
    return ans;
}

static string xor_with_const(string& wi_1, int rounds) {
    long long num = 0;
    for (int i = 0; i < 8; ++i) {
        char ch = wi_1[i];
        num = num * 16 + ch_to_int(ch);
    }
    num ^= Rcon[rounds];
    string res = int_to_chs(num);
    while (res.length() < 8) res = "0" + res;
    return res;
}

static string string_xor(string s1, string s2) {
    long long num = str_long(s1) ^ str_long(s2);
    string ans = int_to_chs(num);
    while (ans.length() < 8) ans = "0" + ans;
    return ans;
}

static string T(string& wi_1, int round) {
    string ans = loop_wordbyte(wi_1);
    ans = wordbyte_sub(ans);
    ans = xor_with_const(ans, round);
    return ans;
}

static vector<string> extend_key(string& key) {
    vector<string> w_key = group_key(key);
    for (int i = 0; i < 40; ++i) {
        int index = 4 + i;
        string temp = w_key[index - 1];
        if (index % 4 == 0) temp = T(temp, index / 4 - 1);
        w_key.push_back(string_xor(temp, w_key[index - 4]));
    }
    return w_key;
}

static vector<string> move_row(vector<string>& s) {
    vector<string> ans = s;
    for (int i = 0; i < 4; ++i) {
        int k = i * 2;
        for (int j = 0; j < 4; ++j) {
            ans[j][k] = s[(j + i) % 4][k];
            ans[j][k + 1] = s[(j + i) % 4][k + 1];
        }
    }
    return ans;
}

static vector<string> split_s(string& s) {
    vector<string> ans;
    for (int i = 0; i < int(s.length()); i += 2) ans.emplace_back(s.substr(i, 2));
    return ans;
}

static string int_ch2(int num) {
    string ans = int_to_chs(num);
    while (ans.length() < 2) ans = "0" + ans;
    return ans;
}

static int power(int num) {
    int ans = (num << 1) % 256;
    if (num & 0x80) ans ^= 0x1b;
    return ans;
}

static vector<string> col_confuse(vector<string>& s) {
    vector<string> ans = s;
    for (int i = 0; i < 4; ++i) {
        auto temp = split_s(s[i]);
        int s0 = int(str_long(temp[0])), s1 = int(str_long(temp[1])),
            s2 = int(str_long(temp[2])), s3 = int(str_long(temp[3]));
        int t0 = power(s0) ^ power(s1) ^ s1 ^ s2 ^ s3;
        int t1 = s0 ^ power(s1) ^ power(s2) ^ s2 ^ s3;
        int t2 = s0 ^ s1 ^ power(s2) ^ s3 ^ power(s3);
        int t3 = s0 ^ power(s0) ^ s1 ^ s2 ^ power(s3);
        ans[i] = int_ch2(t0) + int_ch2(t1) + int_ch2(t2) + int_ch2(t3);
    }
    return ans;
}

static string aes(string& plain_text, string& key) {
    vector<string> keys = extend_key(key);
    int index = 0;
    vector<string> texts = group_key(plain_text);
    for (int i = 0; i < 4; ++i) {
        texts[i] = string_xor(texts[i], keys[i]);
    }
    index += 4;
    for (int k = 0; k < 10; ++k) {
        for (int j = 0; j < 4; ++j) texts[j] = wordbyte_sub(texts[j]);
        texts = move_row(texts);
        if (k < 9) texts = col_confuse(texts);
        for (int i = 0; i < 4; ++i) texts[i] = string_xor(texts[i], keys[i + index]);
        index += 4;
    }
    string result = "";
    for (int i = 0; i < 4; ++i) result += texts[i];
    return result;
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


// 对应 MainWindow::lggen_4()
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

// 对应 MainWindow::de_4()
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

// 对应 MainWindow::ggggen_4()
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

// 对应 MainWindow::lggen_3()
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

// 对应 MainWindow::de_3()
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

// 对应 MainWindow::ggggen_3()
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

// 对应 MainWindow::ggggen_2()
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

// 对应 MainWindow::de_2()
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

// 对应 MainWindow::lggen_2()
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
    parser.setApplicationDescription("高松灯文本加密器4 命令行模式");

    QCommandLineOption encOpt(QStringList() << "e" << "encrypt", "加密（默认）");
    QCommandLineOption decOpt(QStringList() << "d" << "decrypt", "解密");
    QCommandLineOption modeOpt(QStringList() << "m" << "mode", "编码模式：lgg 或 gggg", "mode");
    QCommandLineOption keyOpt(QStringList() << "k" << "key", "密钥", "key");
    QCommandLineOption algoOpt(QStringList() << "a" << "algo", "算法版本：2、3 或 4（默认 4）", "version");

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
                    "高松灯文本加密器4 命令行模式\n"
                    "\n"
                    "选项:\n"
                    "  -e, --encrypt         加密（默认）\n"
                    "  -d, --decrypt         解密\n"
                    "  -m, --mode <mode>     编码模式：lgg 或 gggg\n"
                    "  -k, --key <key>       密钥\n"
                    "  -a, --algo <version>  算法版本：2、3 或 4（默认 4）\n"
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

    if (algo.isEmpty()) algo = "4";  // 默认tmr4
    else if (algo != "2" && algo != "3" && algo != "4") {
        cerr << "错误：-a 必须是 2、3 或 4" << endl;
        return 1;
    }

    QString text = texts.join(" ");
    string result;

	string input = text.toStdString();
    u32string u32input = text.toStdU32String();
    string skey = key.toStdString();
    string key3 = key_translater_3(skey);
    string key2 = key.toStdString();

    if (algo == "4") {
        if (encrypt && mode == "gggg") result = cli_ggggen_4(u32input, key3);
        else if (encrypt && mode == "lgg") result = cli_lggen_4(u32input, key3);
		else result = cli_de_4(input, key3);
    }
    if (algo == "3") {
        if (encrypt && mode == "gggg") result = cli_ggggen_3(u32input, key3);
        else if (encrypt && mode == "lgg") result = cli_lggen_3(u32input, key3);
        else result = cli_de_3(input, key3);
    }
    if (algo == "2") {
        if (encrypt && mode == "gggg") result = cli_ggggen_2(u32input, key2);
        else if (encrypt && mode == "lgg") result = cli_lggen_2(u32input, key2);
        else result = cli_de_2(input, key2);
    }

    cout << result << endl;
    return 0;
}
