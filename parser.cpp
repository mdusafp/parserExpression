#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>

using namespace std;
//FNV-1a hash, 32-bit
//for string switch
inline constexpr uint32_t fnv1a(const char* str, uint32_t hash = 2166136261UL) {
    return *str ? fnv1a(str + 1, (hash ^ *str) * 16777619ULL) : hash;
}

void output(vector<int> &v) {
    for (int i = 0; i < v.size(); i++)
        cout << v[i] << ' ';
    cout << '\n';
}

string calculate(string &l, string &r, char &sign) {
    string exp = "";
    exp.push_back(sign);
    int answer;
    switch (fnv1a(exp.c_str())) {
        case fnv1a("*"):
        answer = atoi(l.c_str()) * atoi(r.c_str());
        break;
        case fnv1a("/"):
        answer = atoi(l.c_str()) / atoi(r.c_str());
        break;
        case fnv1a("+"):
        answer = atoi(l.c_str()) + atoi(r.c_str());
        break;
        case fnv1a("-"):
        answer = atoi(l.c_str()) - atoi(r.c_str());
        break;
    }
    cout << "answer = " << answer << '\n';
    exp = to_string(answer);
    return exp;
}

void move(string &exp, string &leftOp, string &rightOp, vector<int> &signI, int &begL, int &endL, int &begR, int &endR, char prevS) {
    endL = signI [ signI.size() - 2 ] - 1;
    begR = signI [ signI.size() - 2 ] + 1;
    endR = signI [ signI.size() - 1 ] - 1;

    leftOp.assign(exp, begL, endL - begL + 1);
    cout << "left op = " << leftOp << '\n';
    rightOp.assign(exp, begR, endR - begR + 1);
    cout << "right op = " << rightOp << '\n';

    exp.replace(begL, endR - begL + 1, calculate(leftOp, rightOp, prevS));
    signI.pop_back();
}


string simplification(string &exp) {
//    init
    char prevSign, Sign;
    string leftOperand, rightOperand;
    int begLeftOperand = 0, endLeftOperand, begRightOperand, endRightOperand;
    vector<int> signIndex;
    for (int i = 0; i < exp.length(); i++) {
        if (exp[i] == '+' || exp[i] == '-' || exp[i] == '/' || exp[i] == '*') {

            signIndex.push_back(i);
            if (signIndex.size() > 1) { // always <= 2

                prevSign = exp [ signIndex [ signIndex.size() - 2 ] ];
                Sign = exp [ signIndex [ signIndex.size() - 1 ] ];

                if (prevSign == '*' || prevSign == '/' || Sign == '+' || Sign == '-') {
                    move(exp, leftOperand, rightOperand, signIndex, begLeftOperand, endLeftOperand, begRightOperand, endRightOperand, prevSign);
                    i = 0;
                    signIndex.clear();
                    begLeftOperand = 0;
                } else {
                    begLeftOperand = signIndex [ signIndex.size() - 2 ] + 1;
                }
            }
        } else if (i == exp.length() - 1 && signIndex.size() == 1) {// you can copypaste less then there
            endLeftOperand = signIndex[0] - 1;
            begRightOperand = signIndex[0] + 1;
            endRightOperand = exp.length() - 1;

            leftOperand.assign(exp, begLeftOperand, endLeftOperand - begLeftOperand + 1);
            rightOperand.assign(exp, begRightOperand, endRightOperand - begRightOperand + 1);
            prevSign = exp[ signIndex[0] ];
            exp.replace(begLeftOperand, endRightOperand - begLeftOperand + 1, calculate(leftOperand, rightOperand, prevSign));

        }
        if (i == exp.length() - 1 && signIndex.size() == 2) {
            endLeftOperand = signIndex[1] - 1;
            begRightOperand = signIndex[1] + 1;
            endRightOperand = exp.length() - 1;

            leftOperand.assign(exp, begLeftOperand, endLeftOperand - begLeftOperand + 1);
            rightOperand.assign(exp, begRightOperand, endRightOperand - begRightOperand + 1);
            prevSign = exp[ signIndex[1] ];
            exp.replace(begLeftOperand, endRightOperand - begLeftOperand + 1, calculate(leftOperand, rightOperand, prevSign));
            i = 0;
            signIndex.clear();
            begLeftOperand = 0;

        }
    }
    output(signIndex);
    return exp;
}

string openBrackets(string &exp, int indexLeft, int indexRight) {
    string curr = "";
    int pos = indexLeft + 1;
    int size = indexRight - indexLeft - 1;

    curr.assign(exp, pos, size);
    exp.replace(indexLeft, size + 2, simplification(curr));

    return exp;
}

int backTrack(vector<int> &left, vector<int> &right, int &indexLeft) {
    int i;

    left.pop_back();
    right.pop_back();
    if (left.size()) {
        indexLeft = left[left.size() - 1];
        i = indexLeft + 1;
    }

    return i;
}

string parser(string &exp) {
    int indexLeft, indexRight;
    vector<int> left, right;
    for (int i = 0; i < exp.length(); i++) {
                                                                                                                                // crash
        if (left.size() < right.size()) {
            cout << "This expression is awesome ;(\n";
            exit(EXIT_FAILURE);
        }

        if (i == exp.length() - 1 && left.empty()) {
            exp = simplification(exp);
        }

        if (exp[i] == '(') {
            indexLeft = i;
            left.push_back(i);
        }

        if (exp[i] == ')') {
            indexRight = i;
            right.push_back(i);

            exp = openBrackets(exp, indexLeft, indexRight);
            i = backTrack(left, right, indexLeft);
        }
    }
    cout << "left: ";
    output(left);
    cout << "right: ";
    output(right);
    if (left.size() != right.size()) {
        cout << "This expression is awesome ;(\n";
        exit(EXIT_FAILURE);
    }
    return exp;
}

int main(int argc, char const *argv[]) {
    string expression;
    int x = 1;
    while (x) {
        cin >> expression;
        expression = parser(expression);
        cout << "expression = " <<  expression << '\n';
        cout << "continue? ";
        cin >> x;
    }
    return 0;
}
