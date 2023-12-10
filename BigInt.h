#pragma once
#ifndef BIGINT_H
#define BIGINT_H

#include <iostream>
#include <string>
using namespace std;

class BIG_INT
{
private:
    string bigInt;

public:
    BIG_INT() : bigInt("") {}
    BIG_INT(string str)
    {
        for (int i = 0; i < str.length(); i++)
            bigInt += str[i];
    }
    BIG_INT(const BIG_INT& copy)
    {
        this->bigInt = copy.bigInt;
    }
    bool validate() {
        if (bigInt == "") return false;
        for (int i = 0; i < this->bigInt.length(); i++) {
            if (bigInt[i] < '0' && bigInt[i] > '9') {
                return false;
            }
        }
        if (bigInt.length() > 1 && bigInt[0] == '0') return false;
        return true;
    }
    string getBIG_INT() { return this->bigInt; }
    BIG_INT operator+(const BIG_INT& other)
    {
        string str1, str2; // str1 is always greator than str2
        // setting both str1 and str2
        if (other.bigInt.length() > this->bigInt.length())
        {
            str1 = other.bigInt;
            str2 = this->bigInt;
            string zeros = string(str1.length() - str2.length(), '0'); // it makes a string of zeroes if sizes are different
            str2 = zeros + str2;
        }
        else
        {
            str1 = this->bigInt;
            str2 = other.bigInt;
            string zeros = string(str1.length() - str2.length(), '0'); // it makes a string of zeroes if sizes are different
            str2 = zeros + str2;
        }
        // calculating their sum
        int carry = 0, val1, val2, f_val; // val1 is for str1 and val2 is for val2 and f_val is final value
        string sum = "";
        for (int i = str1.length() - 1; i >= 0; i--)
        {
            val1 = int(str1[i] - '0');
            val2 = int(str2[i] - '0');
            f_val = val1 + val2 + carry;
            carry = f_val / 10;
            f_val %= 10;
            sum += to_string(f_val);
        }
        sum += to_string(carry);
        while (sum.length() > 1 && sum.back() == '0') // it removes any leading zeroes
            sum.pop_back();
        BIG_INT temp;
        for (int i = sum.length() - 1; i >= 0; i--)
            temp.bigInt += sum[i];
        return temp;
    }
    BIG_INT operator-(const BIG_INT& other)
    {
        string str1, str2; // str1 is always greator than str2
        bool setNegative = false;
        // setting both str1 and str2
        if (other.bigInt.length() > this->bigInt.length())
        {
            setNegative = true;
            str1 = other.bigInt;
            str2 = this->bigInt;
            string zeros = string(str1.length() - str2.length(), '0'); // it makes a string of zeroes if sizes are different
            str2 = zeros + str2;
        }
        else
        {
            str1 = this->bigInt;
            str2 = other.bigInt;
            string zeros = string(str1.length() - str2.length(), '0'); // it makes a string of zeroes if sizes are different
            str2 = zeros + str2;
        }
        // calculating their difference
        int borrow = 10, val1, val2, f_val; // val1 is for str1 and val2 is for val2 and f_val is final value
        string difference = "";
        for (int i = str1.length() - 1; i >= 0; i--)
        {
            val1 = int(str1[i] - '0');
            val2 = int(str2[i] - '0');
            if (val1 < val2)
            {
                int j = i - 1;
                while (str1[j] == '0') // making all 0s 9s
                {
                    str1[j] = '9';
                    j--;
                }
                str1[j] = str1[j] - 1; // subtracting 1 from the value which borrow is taken from
                val1 += borrow;        // adding the borrow into the desired digit
            }
            f_val = val1 - val2;
            difference += to_string(f_val);
        }
        while (difference.length() > 1 && difference.back() == '0') // is removes any leading zeros
            difference.pop_back();
        if (setNegative)
            difference = difference + "-";
        BIG_INT temp;
        for (int i = difference.length() - 1; i >= 0; i--)
            temp.bigInt += difference[i];
        return temp;
    }
    BIG_INT operator=(const BIG_INT& other)
    {
        this->bigInt = other.bigInt;
        return *this;
    }
    BIG_INT operator%(const BIG_INT& other) {
        BIG_INT temp;
        temp = this->bigInt;
        if (temp < other) return temp;
        while (true) {
            if (temp > other) {
                temp = temp - other;
            }
            else {
                break;
            }
        }

        return temp;
    }
    BIG_INT operator*(const BIG_INT& other) {
        string thisString; thisString = this->bigInt;
        string otherString = other.bigInt;
        if (thisString == "0" || otherString == "0") return BIG_INT("0");

        int n = thisString.size();
        int m = otherString.size();
        string res(n + m, '0');

        for (int i = n - 1; i >= 0; i--) {
            for (int j = m - 1; j >= 0; j--) {
                int num = (thisString[i] - '0') * (otherString[j] - '0') + (res[i + j + 1] - '0'); // res will store carry
                res[i + j + 1] = num % 10 + '0';
                res[i + j] += num / 10; // carry

            }
        }

        int i = 0;
        while (i < res.size() && res[i] == '0') i++;
        return BIG_INT(res.substr(i));
    
    }
    bool operator<(const BIG_INT& other)
    {
        bool flag = false;
        if (this->bigInt.length() < other.bigInt.length())
            return true;
        else if (this->bigInt.length() > other.bigInt.length())
            return false;
        else
        {
            for (int i = 0; i < other.bigInt.length(); i++)
                if (this->bigInt[i] > other.bigInt[i])
                    return false;
                else if (this->bigInt[i] == other.bigInt[i])
                    flag = true;
                else if (this->bigInt[i] < other.bigInt[i])
                {
                    flag = false;
                    break;
                }
        }
        return flag ? false : true;
    }
    bool operator>(const BIG_INT& other)
    {
        bool flag = false;
        if (this->bigInt.length() > other.bigInt.length())
            return true;
        else if (this->bigInt.length() < other.bigInt.length())
            return false;
        else
        {
            for (int i = 0; i < other.bigInt.length(); i++)
                if (this->bigInt[i] < other.bigInt[i])
                    return false;
                else if (this->bigInt[i] == other.bigInt[i])
                    flag = true;
                else if (this->bigInt[i] > other.bigInt[i])
                {
                    flag = false;
                    break;
                }
        }
        return flag ? false : true;
    }
    bool operator==(const BIG_INT& other)
    {
        if (this->bigInt.length() != other.bigInt.length())
            return false;
        else
            for (int i = 0; i < other.bigInt.length(); i++)
                if (this->bigInt[i] != other.bigInt[i])
                    return false;
        return true;
    }
    bool operator<=(const BIG_INT& other)
    {
        if ((*this < other) || (*this == other))
            return true;
        return false;
    }
    bool operator>=(const BIG_INT& other)
    {
        if ((*this > other) || (*this == other))
            return true;
        return false;
    }
    friend ostream& operator<<(ostream& out, const BIG_INT& obj)
    {
        for (int i = 0; i < obj.bigInt.length(); i++)
            out << obj.bigInt[i];
        return out;
    }
};

#endif // !BIGINT_H