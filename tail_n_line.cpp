#include <iostream>
#include <fstream>

using namespace std;

void tail(const string& filename, int n) {
    ifstream file(filename, ios::in | ios::ate); // Open file and seek to end
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return;
    }
    
    streampos file_size = file.tellg();
    file.seekg(0, ios::end);
    
    int count = 0;
    string line;
    streampos pos = file_size - 1;
    
    while (pos > 0 && count < n) {
        file.seekg(--pos);
        if (file.peek() == '\n') {
            count++;
        }
    }
    
    if (pos > 0) file.seekg(pos + 1); // Move forward to the start of the last n lines
    
    while (getline(file, line)) {
        cout << line << endl;
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <filename>" << endl;
        return 1;
    }
    
    tail(argv[1], 100);
    return 0;
}
