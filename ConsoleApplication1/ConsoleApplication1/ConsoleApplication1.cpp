/*
 * GRAPHING CALCULATOR - OOP PROJECT
 * Console-based graphing calculator with expression evaluation and plotting
 * Demonstrates: Classes, Inheritance, Polymorphism, Abstraction, File Handling, Linked List
 */

#include <iostream>
#include <cmath>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <vector>

using namespace std;

// ==================== ABSTRACTION: Abstract Base Class ====================
class Expression {
protected:
    string expressionStr;
public:
    Expression(string expr = "") : expressionStr(expr) {}
    virtual ~Expression() {}

    virtual double evaluate(double x) = 0;
    virtual void display() const = 0;
    virtual string getType() const = 0;

    string getExpression() const { return expressionStr; }
    void setExpression(string expr) { expressionStr = expr; }
};

// ==================== INHERITANCE: Derived Classes ====================
class LinearFunction : public Expression {
private:
    double m, c;
public:
    LinearFunction(double slope = 1, double intercept = 0)
        : Expression(), m(slope), c(intercept) {
        stringstream ss;
        ss << "y = " << m << "x";
        if (c >= 0) ss << " + " << c;
        else ss << " - " << (-c);
        expressionStr = ss.str();
    }

    double evaluate(double x) override { return m * x + c; }
    void display() const override { cout << "Linear Function: " << expressionStr << endl; }
    string getType() const override { return "Linear"; }
};

class QuadraticFunction : public Expression {
private:
    double a, b, c;
public:
    QuadraticFunction(double coefA = 1, double coefB = 0, double coefC = 0)
        : Expression(), a(coefA), b(coefB), c(coefC) {
        stringstream ss;
        ss << "y = " << a << "x^2";
        if (b >= 0) ss << " + " << b << "x";
        else ss << " - " << (-b) << "x";
        if (c >= 0) ss << " + " << c;
        else ss << " - " << (-c);
        expressionStr = ss.str();
    }

    double evaluate(double x) override { return a * x * x + b * x + c; }
    void display() const override { cout << "Quadratic Function: " << expressionStr << endl; }
    string getType() const override { return "Quadratic"; }
};

class ExponentialFunction : public Expression {
private:
    double coefficient, exponent;
public:
    ExponentialFunction(double A = 1, double B = 1)
        : Expression(), coefficient(A), exponent(B) {
        stringstream ss;
        ss << "y = " << A << "*e^(" << B << "x)";
        expressionStr = ss.str();
    }

    double evaluate(double x) override { return coefficient * exp(exponent * x); }
    void display() const override { cout << "Exponential Function: " << expressionStr << endl; }
    string getType() const override { return "Exponential"; }
};

// ==================== LINKED LIST IMPLEMENTATION ====================
struct HistoryNode {
    Expression* function;
    HistoryNode* next;
    HistoryNode(Expression* func) : function(func), next(nullptr) {}
};

class FunctionHistory {
private:
    HistoryNode* head;
    int count;

public:
    FunctionHistory() : head(nullptr), count(0) {}
    ~FunctionHistory() { clear(); }

    void addFunction(Expression* func) {
        HistoryNode* newNode = new HistoryNode(func);
        newNode->next = head;
        head = newNode;
        count++;
    }

    void displayHistory() const {
        if (!head) { cout << "No history available.\n"; return; }
        cout << "\n========== FUNCTION HISTORY ==========\n";
        HistoryNode* current = head;
        int index = 1;
        while (current) {
            cout << index++ << ". ";
            current->function->display();
            current = current->next;
        }
        cout << "======================================\n";
    }

    void clear() {
        HistoryNode* current = head;
        while (current) {
            HistoryNode* temp = current;
            current = current->next;
            delete temp->function;
            delete temp;
        }
        head = nullptr;
        count = 0;
    }

    int getCount() const { return count; }
};

// ==================== GRAPHING ENGINE ====================
class GraphPlotter {
private:
    int width, height;
    double xMin, xMax, yMin, yMax;
public:
    GraphPlotter(int w = 61, int h = 21) : width(w), height(h), xMin(-10), xMax(10), yMin(-10), yMax(10) {}
    void setRange(double xmin, double xmax, double ymin, double ymax) { xMin = xmin; xMax = xmax; yMin = ymin; yMax = ymax; }
    void setRange(double xmin, double xmax) { xMin = xmin; xMax = xmax; }
    int xToScreen(double x) { return (int)((x - xMin) / (xMax - xMin) * (width - 1)); }
    int yToScreen(double y) { return (int)((yMax - y) / (yMax - yMin) * (height - 1)); }
    double screenToX(int col) { return xMin + col * (xMax - xMin) / (width - 1); }
    double screenToY(int row) { return yMax - row * (yMax - yMin) / (height - 1); }

    void plot(Expression* func) {
        vector<vector<char>> canvas(height, vector<char>(width, ' '));
        int xAxisRow = yToScreen(0);
        int yAxisCol = xToScreen(0);

        if (xAxisRow >= 0 && xAxisRow < height) for (int j = 0; j < width; j++) canvas[xAxisRow][j] = '-';
        if (yAxisCol >= 0 && yAxisCol < width) for (int i = 0; i < height; i++) canvas[i][yAxisCol] = '|';
        if (xAxisRow >= 0 && xAxisRow < height && yAxisCol >= 0 && yAxisCol < width) canvas[xAxisRow][yAxisCol] = '+';

        const double EPS = 0.01;
        for (int col = 0; col < width; col++) {
            double x = screenToX(col);
            double y = func->evaluate(x);
            if (y < yMin || y > yMax) continue;
            int row = yToScreen(y);
            if (fabs(y) < EPS) row = xAxisRow;
            else if (row == xAxisRow) row += (y > 0 ? -1 : +1);
            if (row >= 0 && row < height && col >= 0 && col < width) {
                if (canvas[row][col] == '-' || canvas[row][col] == '|') canvas[row][col] = '+';
                else canvas[row][col] = '*';
            }
        }

        cout << "\n";
        func->display();
        cout << "Range: X[" << xMin << " to " << xMax << "], Y[" << yMin << " to " << yMax << "]\n\n";

        for (int i = 0; i < height; i++) {
            double yValue = screenToY(i);
            cout << setw(6) << fixed << setprecision(1) << yValue << " |";
            for (int j = 0; j < width; j++) cout << canvas[i][j];
            cout << "\n";
        }

        cout << "       +";
        for (int j = 0; j < width - 1; j++) cout << "-";
        cout << "\n        ";
        int numXLabels = 11;
        for (int i = 0; i < numXLabels; i++) {
            double xValue = xMin + i * (xMax - xMin) / (numXLabels - 1);
            if (i == 0) cout << setw(1) << fixed << setprecision(0) << xValue;
            else {
                int spacing = (width - 1) / (numXLabels - 1);
                cout << setw(spacing) << fixed << setprecision(0) << xValue;
            }
        }
        cout << "\n        " << string(width / 2 - 5, ' ') << "X-axis ?\n\n";
    }
};

// ==================== FILE HANDLING ====================
class FileManager {
public:
    static void saveFunction(Expression* func, string filename = "functions.txt") {
        ofstream outFile(filename, ios::app);
        if (outFile.is_open()) {
            outFile << func->getType() << "|" << func->getExpression() << "\n";
            outFile.close();
            cout << "Function saved to " << filename << endl;
        }
        else cout << "Error: Could not open file for writing.\n";
    }

    static void saveGraphData(Expression* func, double xMin, double xMax, int points, string filename = "graph_data.txt") {
        ofstream outFile(filename);
        if (outFile.is_open()) {
            outFile << "Function: " << func->getExpression() << "\nX\t\tY\n=============================\n";
            double step = (xMax - xMin) / points;
            for (int i = 0; i <= points; i++) {
                double x = xMin + i * step;
                double y = func->evaluate(x);
                outFile << fixed << setprecision(4) << x << "\t\t" << y << "\n";
            }
            outFile.close();
            cout << "Graph data saved to " << filename << endl;
        }
        else cout << "Error: Could not open file for writing.\n";
    }

    static void loadFunctions(string filename = "functions.txt") {
        ifstream inFile(filename);
        if (inFile.is_open()) {
            cout << "\n========== SAVED FUNCTIONS ==========\n";
            string line;
            int count = 1;
            while (getline(inFile, line)) cout << count++ << ". " << line << endl;
            inFile.close();
            cout << "====================================\n";
        }
        else cout << "No saved functions found.\n";
    }
};

// ==================== MAIN CALCULATOR CLASS ====================
class GraphingCalculator {
private:
    GraphPlotter plotter;
    FunctionHistory history;

public:
    void displayMenu() {
        cout << "\ngraphing calculator\n";
        cout << "1. Plot Linear Function (y = mx + c)\n";
        cout << "2. Plot Quadratic Function (y = ax^2 + bx + c)\n";
        cout << "3. Plot Exponential Function (y = A*e^(Bx))\n";
        cout << "4. View Function History\n";
        cout << "5. Load Saved Functions\n";
        cout << "6. Clear History\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
    }

    void plotLinear() {
        double m, c;
        cout << "\n--- Linear Function: y = mx + c ---\n";
        cout << "Enter slope (m): "; cin >> m;
        cout << "Enter y-intercept (c): "; cin >> c;
        LinearFunction* func = new LinearFunction(m, c);
        double range = max(10.0, abs(c) * 2);
        plotter.setRange(-10, 10, -range, range);
        plotter.plot(func);
        history.addFunction(func);
        char save; cout << "Save this function? (y/n): "; cin >> save;
        if (save == 'y' || save == 'Y') { FileManager::saveFunction(func); FileManager::saveGraphData(func, -10, 10, 100); }
    }

    void plotQuadratic() {
        double a, b, c;
        cout << "\n--- Quadratic Function: y = ax^2 + bx + c ---\n";
        cout << "Enter coefficient a: "; cin >> a;
        cout << "Enter coefficient b: "; cin >> b;
        cout << "Enter coefficient c: "; cin >> c;
        QuadraticFunction* func = new QuadraticFunction(a, b, c);
        plotter.setRange(-10, 10, -20, 50);
        plotter.plot(func);
        history.addFunction(func);
        char save; cout << "Save this function? (y/n): "; cin >> save;
        if (save == 'y' || save == 'Y') { FileManager::saveFunction(func); FileManager::saveGraphData(func, -10, 10, 100); }
    }

    void plotExponential() {
        double A, B;
        cout << "\n--- Exponential Function: y = A*e^(Bx) ---\n";
        cout << "Enter coefficient (A): "; cin >> A;
        cout << "Enter exponent coefficient (B): "; cin >> B;
        ExponentialFunction* func = new ExponentialFunction(A, B);
        plotter.setRange(-3, 3, -5, 30);
        plotter.plot(func);
        history.addFunction(func);
        char save; cout << "Save this function? (y/n): "; cin >> save;
        if (save == 'y' || save == 'Y') { FileManager::saveFunction(func); FileManager::saveGraphData(func, -3, 3, 100); }
    }

    void run() {
        int choice;
        cout << "Welcome\n";
        do {
            displayMenu();
            cin >> choice;
            if (cin.fail()) { cin.clear(); cin.ignore(10000, '\n'); cout << "Invalid input! Please enter a number.\n"; continue; }
            switch (choice) {
            case 1: plotLinear(); break;
            case 2: plotQuadratic(); break;
            case 3: plotExponential(); break;
            case 4: history.displayHistory(); break;
            case 5: FileManager::loadFunctions(); break;
            case 6: history.clear(); cout << "History cleared successfully.\n"; break;
            case 0: cout << "thankyou\n"; break;
            default: cout << "Invalid choice! Please try again.\n";
            }
            if (choice != 0) { cout << "\nPress Enter to continue..."; cin.ignore(); cin.get(); }
        } while (choice != 0);
    }
};

// ==================== MAIN FUNCTION ====================
int main() {
    GraphingCalculator calculator;
    calculator.run();
    return 0;
}