#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <fstream>

using namespace std;
//FNV-1a hash, 32-bit
//for analysis of string switch
inline constexpr uint32_t fnv1a(const char* str, uint32_t hash = 2166136261UL) {
    return *str ? fnv1a(str + 1, (hash ^ *str) * 16777619ULL) : hash;
}

//calculation
string calculate(string &l, string &r, char &sign) {
    string exp = "";
    exp.push_back(sign);
    int answer;
    switch (fnv1a(exp.c_str())) {
        case fnv1a("*"):
        answer = atoi(l.c_str()) * atoi(r.c_str());
        break;
        case fnv1a("/"):
        if (atoi(r.c_str())) { // if second operand zero throw an error
            answer = atoi(l.c_str()) / atoi(r.c_str());
        } else {
            cout << "You try to divide by zero ;(\n";
            exit(EXIT_FAILURE);
        }
        break;
        case fnv1a("+"):
        answer = atoi(l.c_str()) + atoi(r.c_str());
        break;
        case fnv1a("-"):
        answer = atoi(l.c_str()) - atoi(r.c_str());
        break;
    }
    exp = to_string(answer);
    return exp;
}

void wrapperOfCalculation(string &exp, string &leftOp, string &rightOp, vector<int> &signI, int &begL, int &endL, int &begR, int &endR, char prevS) {
    // init left and right operand
    leftOp.assign(exp, begL, endL - begL + 1);
    rightOp.assign(exp, begR, endR - begR + 1);
    // calculation
    exp.replace(begL, endR - begL + 1, calculate(leftOp, rightOp, prevS));
}

void searchOperand(string &exp, int &endLeftOp, int &begRightOp, int &endRightOp, char &prevS, int indexLeftS, int indexRightS) {
    endLeftOp = indexLeftS - 1;
    begRightOp = indexLeftS + 1;
    endRightOp = indexRightS - 1;
    prevS = exp[ indexLeftS ];
}
// simplification of expression
string simplification(string &exp) {
    // compare prev sign and next sign and compare their priority
    char prevSign, Sign;
    string leftOperand, rightOperand;
    int begLeftOperand = 0, endLeftOperand, begRightOperand, endRightOperand;
    vector<int> signIndex;

    for (int i = 0; i < exp.length(); i++) {

        if (exp[i] == '+' || exp[i] == '-' || exp[i] == '/' || exp[i] == '*') {
            signIndex.push_back(i);

            if (signIndex.size() > 1) { // when we find two sign (always find <= 2)
                prevSign = exp [ signIndex [ signIndex.size() - 2 ] ];
                Sign = exp [ signIndex [ signIndex.size() - 1 ] ];

                if (prevSign == '*' || prevSign == '/' || Sign == '+' || Sign == '-') {
                    // search index the start and end indices of left and right operands
                    searchOperand(exp, endLeftOperand, begRightOperand, endRightOperand, prevSign, signIndex [ signIndex.size() - 2 ], signIndex [ signIndex.size() - 1 ]);
                    // wrapper of calculation
                    wrapperOfCalculation(exp, leftOperand, rightOperand, signIndex, begLeftOperand, endLeftOperand, begRightOperand, endRightOperand, prevSign);
                    i = 0;
                    signIndex.clear();
                    begLeftOperand = 0;
                } else {
                    begLeftOperand = signIndex [ signIndex.size() - 2 ] + 1;
                }
            }
        } else if (i == exp.length() - 1 && signIndex.size() == 1) {// you can copypaste less then there
            searchOperand(exp, endLeftOperand, begRightOperand, endRightOperand, prevSign, signIndex[0], exp.length());
            wrapperOfCalculation(exp,leftOperand, rightOperand, signIndex, begLeftOperand, endLeftOperand, begRightOperand, endRightOperand, prevSign);
        }
        if (i == exp.length() - 1 && signIndex.size() == 2) {
            searchOperand(exp, endLeftOperand, begRightOperand, endRightOperand, prevSign, signIndex[1], exp.length());

            wrapperOfCalculation(exp,leftOperand, rightOperand, signIndex, begLeftOperand, endLeftOperand, begRightOperand, endRightOperand, prevSign);
            i = 0;
            signIndex.clear();
            begLeftOperand = 0;

        } else if (i == exp.length() - 1 && !signIndex.size()) {

        }
    }
    return exp;
}
// replace the expression between brackets (include brackets) to accounted expression between brackets
string openBrackets(string &exp, int indexLeft, int indexRight) {
    string curr = "";
    int pos = indexLeft + 1;
    int size = indexRight - indexLeft - 1;

    curr.assign(exp, pos, size);
    exp.replace(indexLeft, size + 2, simplification(curr));

    return exp;
}
// backtrack to last not opened left bracket
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
    vector<int> leftBrackets, rightBrackets; // vector of index

    for (int i = 0; i < exp.length(); i++) {
        // crash
        if (leftBrackets.size() < rightBrackets.size()) { // if we catch right bracket before left throw an error
            cout << "It's too much right brackets before left brackets ;(\n";
            exit(EXIT_FAILURE);
        }
        // parsing
        if (i == exp.length() - 1 && leftBrackets.empty()) {
            exp = simplification(exp);
        }

        if (exp[i] == '(') {
            indexLeft = i;
            leftBrackets.push_back(i);
        }

        if (exp[i] == ')') {
            indexRight = i;
            rightBrackets.push_back(i);
            // replace the expression between brackets (include brackets) to accounted expression between brackets
            exp = openBrackets(exp, indexLeft, indexRight);
            // go back to last brackets
            i = backTrack(leftBrackets, rightBrackets, indexLeft);
        }
    }
    // if we have expression without brackets
    exp = simplification(exp);

    // crash
    if (leftBrackets.size() != rightBrackets.size()) { // if left brackets more then right brackets when we come to last symbol throw an error
        cout << "You forgot a pinch of right brackets ;(\n";
        exit(EXIT_FAILURE);
    }

    return exp;
}

void consoleProcessing(string &exp) {
    int x = 1;
    while (x) {
        cout << "Enter the expression: ";
        cin >> exp;
        cout << exp << " = "; // before parse
        exp = parser(exp);
        cout <<  exp << '\n'; // after parse (answer)
        cout << "Do you want to continue?\n";
        cin >> x;
    }
}

void fileProcessing(string &exp) {
    ifstream in("/home/mfp/QtProjects/Parser/Parser/file.txt");
    if (!in.is_open()) {
        cout << "FIle doesn't open\n";
    } else {
        while (in) {
            getline(in, exp);
            if (exp.empty()) return;
            cout << exp; // before parse
            exp = parser(exp);
            cout << " = " <<  exp << '\n'; // after parse (answer)
        }
    }
}

void menu () {
    int ch;
    string expression;
    do {
        cout << "1. console processing\n";
        cout << "2. file processing\n";
        cout << "0. exit\n";
        cin >> ch;
        switch (ch) {
            case 1:
            consoleProcessing(expression);
            break;
            case 2:
            fileProcessing(expression);
            break;
            case 0: exit(EXIT_SUCCESS);
        }
    } while (ch);
}

int main(int argc, char const *argv[]) {
    menu();
    return 0;
}
