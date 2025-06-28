#include <bits/stdc++.h>


using namespace std;

struct symbol {
    string name;
    int address;
    bool set;
    int set_value;
};

struct literal {
    int value;
    int address;
};

vector<symbol> symbol_table;
vector<literal> literal_table;

static inline string trim(string &s) {
    size_t start = s.find_first_not_of(" \t\n\r\f\v");  // Find first non-whitespace character
    if (start == string::npos) {  // If no non-whitespace character is found
        s.clear();  // Return an empty string
    } else {
        size_t end = s.find_last_not_of(" \t\n\r\f\v");  // Find last non-whitespace character
        s = s.substr(start, end - start + 1);  // Substring the string to get the trimmed version
    }
    return s;
}

bool is_number(const std::string& s)
{
    // Handle empty string case
    if (s.empty()) return false;

    std::string temp = s;

    // Handle sign at the start of the string
    if (temp.front() == '+' || temp.front() == '-') {
        temp = temp.substr(1);  // Remove the sign character
    }

    // Handle hexadecimal numbers (starting with "0x")
    if (temp.substr(0, 2) == "0x" || temp.substr(0, 2) == "0X") {
        temp = temp.substr(2);  // Remove the "0x" prefix
        if (temp.empty()) return false;
        // Check if all remaining characters are valid hexadecimal digits
        for (char c : temp) {
            if (!std::isxdigit(c)) {
                return false;
            }
        }
        return true;  // Valid hexadecimal number
    }

    // Check if the remaining string is a valid decimal number
    for (char c : temp) {
        if (!std::isdigit(c)) {
            return false;  // Contains non-digit characters
        }
    }

    return true;  // Valid number (all characters are digits)
}

int tonum(string s) {
    s = trim(s);  // Trim leading or trailing spaces

    // Handle signed integers
    bool is_negative = false;
    if (s.find("-") == 0) {
        is_negative = true;
        s = s.substr(1);  // Remove the negative sign for processing
    } else if (s.find("+") == 0) {
        s = s.substr(1);  // Remove the positive sign for processing
    }

    // Handle hexadecimal (0x or 0X prefix)
    if (s.find("0x") == 0 || s.find("0X") == 0) {
        return is_negative ? -stoul(s.substr(2), nullptr, 16) : stoul(s.substr(2), nullptr, 16);
    }

    // Handle octal (0 prefix)
    if (s.find("0") == 0 && s.length() > 1 && s[1] != 'x' && s[1] != 'X') {
        return is_negative ? -stoul(s.substr(1), nullptr, 8) : stoul(s.substr(1), nullptr, 8);
    }

    // Handle decimal (standard number)
    return is_negative ? -stoul(s, nullptr, 10) : stoul(s, nullptr, 10);
}

bool contains(string name) {
    for (const auto& ts : symbol_table) {
        if (ts.name == name) {
            return true;
        }
    }
    return false;
}

string int_to_hex(int i)
{
    const char hex_digits[] = "0123456789ABCDEF";  // Hexadecimal digit map
    string result = "";

    // Loop through each byte (4 bits per hex digit)
    for (int j = 28; j >= 0; j -= 4) {
        int hex_digit = (i >> j) & 0xF;  // Extract the current 4 bits (1 hex digit)
        result += hex_digits[hex_digit];  // Append corresponding hex character
    }

    // Remove leading zeros (if any)
    size_t first_non_zero = result.find_first_not_of('0');
    if (first_non_zero != string::npos) {
        result = result.substr(first_non_zero);  // Trim leading zeros
    }

    // Ensure the result is at least 8 characters wide (pad with leading zeros if needed)
    result = string(8 - result.length(), '0') + result;

    return result;
}


bool is_valid_label(const string& label) {
    // Check if the label is not empty and starts with a valid letter
    if (label.empty() || !isalpha(label[0])) {
        return false;
    }

    // Check if all remaining characters are either alphanumeric or an underscore
    for (char c : label.substr(1)) {
        if (!isalnum(c) && c != '_') {
            return false;
        }
    }

    return true;
}



map<string, string> mot;

void mot_init() {
    mot = {
        {"ldc", "00"},
        {"adc", "01"},
        {"ldl", "02"},
        {"stl", "03"},
        {"ldnl", "04"},
        {"stnl", "05"},
        {"add", "06"},
        {"sub", "07"},
        {"shl", "08"},
        {"shr", "09"},
        {"adj", "0a"},
        {"a2sp", "0b"},
        {"sp2a", "0c"},
        {"call", "0d"},
        {"return", "0e"},
        {"brz", "0f"},
        {"brlz", "10"},
        {"br", "11"},
        {"HALT", "12"},
        {"data", "13"},
        {"SET", "14"}
    };
}


string inst_to_table(string instr, int* loc_ptr, int line) {
    int loc = *loc_ptr;
    string errors = "";

    // Function to process labels and add to the symbol table
    auto process_label = [&] (const string& label, int loc) {
        if (contains(label)) {
            cout << "ERROR: Duplicate Label at line " << line << endl;
            errors += "ERROR: Duplicate Label at line " + to_string(line) + "\n";
        }
        if (!is_valid_label(label)) {
            cout << "WARNING: Incorrect label format at line " << line << endl;
            errors += "WARNING: Incorrect label format at line " + to_string(line) + "\n";
        }
        // Add label to the symbol table with appropriate flag and location
        symbol_table.push_back({label, loc, 0, -1});
    };

    // Function to handle instructions (including SET and literals)
    auto process_instruction = [&] (const string& instr) {
        // Extracting operand and value
        int space = instr.find(" ");
        string op = instr.substr(0, space);
        string operand = instr.substr(space + 1);

        // Handle SET instructions
        if (op == "SET") {
            literal_table.push_back({tonum(operand), -1});
        }
        else {
            symbol_table.push_back({instr, loc, 0, -1});
        }
    };

    // Identify label and process accordingly
    size_t colon_pos = instr.find(":");
    if (colon_pos != string::npos) {
        string label = instr.substr(0, colon_pos);
        string remaining_instr = instr.substr(colon_pos + 1);
        remaining_instr = trim(remaining_instr); // Remove extra spaces

        // Process label
        process_label(label, loc);

        // If instruction exists after the label
        if (!remaining_instr.empty()) {
            process_instruction(remaining_instr);
        } else {
            *loc_ptr = loc - 1; // Adjust location counter if no instruction after label
        }
    }
    else {
        // No label, handle literal or constant
        int space = instr.find(" ");
        if (space != string::npos) {
            string operand = instr.substr(space + 1);
            operand = trim(operand); // Remove spaces

            // Check if it's a number, and add to lit_table
            if (is_number(operand)) {
                literal_table.push_back({tonum(operand), -1});
            }
        }
    }

    return errors;
}
void analyse(const string& file, ofstream& logfile)
{
    string line;
    int loc = 0;
    int line_count = 1;

    // Open file with error handling
    ifstream MyFile(file);
    if (!MyFile.is_open()) {
        cerr << "ERROR: Could not open file " << file << endl;
        return;
    }

    // Function to process each line
    auto process_line = [&] (const string& line, int line_num) -> string {
        string instr = line.substr(0, line.find(";", 0));  // Remove comments
        instr = trim(instr);  // Trim extra spaces
        if (instr.empty()) {
            return "";  // Return empty if line is empty
        }

        return inst_to_table(instr, &loc, line_num);  // Process instruction
    };

    // Reading file line by line
    while (getline(MyFile, line)) 
    {
        string errors = process_line(line, line_count);
        if (!errors.empty()) {
            logfile << errors;  // Write errors to the log file
        }
        loc++;  // Increment location after processing each line
        line_count++;  // Increment line counter
    }

    // Processing literals (LTORG loop)
    for (auto& lit : literal_table) {
        if (lit.address == -1) {
            lit.address = loc++;  // Assign address for undefined literals
        }
    }

    // Optional: Verification for debugging (commented out)
    // for (const auto& sym : sym_table)
    //     cout << sym.name << " " << sym.address << " " << sym.set << endl;
    // for (const auto& lit : lit_table)
    //     cout << lit.literal << " " << lit.address << endl;

    // Close the input file after EOF
    MyFile.close();
}

tuple<string, string, string> inst_to_code(string instr, int* loc_ptr, int line)
{
    // Program Counter
    int loc = *loc_ptr;

    // Hex Code of Program Counter
    string encoding = int_to_hex(loc) + " ";

    // Warnings and errors
    string encoding_ = "";
    string errors = "";
    string machine_code = "";

    // Helper function to process operands (number or variable)
    auto process_operand = [&](string sub_operand, string sub_operation) -> string {
        string hex_string = "";
        
        // If operand is a number
        if (is_number(sub_operand)) {
            hex_string = int_to_hex(tonum(sub_operand));
        }
        // If operand is a variable (symbol table lookup)
        else {
            bool defined = false;
            for (const auto& sym : symbol_table) {
                if (sym.name == sub_operand) {
                    defined = true;
                    if (sym.set) {
                        hex_string = int_to_hex(sym.set_value);
                    } else if (sub_operation == "call" || sub_operation == "brz" || sub_operation == "brlz" || sub_operation == "br") {
                        hex_string = int_to_hex(sym.address - loc - 1);
                    } else {
                        hex_string = int_to_hex(sym.address);
                    }
                    break;
                }
            }
            if (!defined) {
                errors += "ERROR: Unknown Symbol as operand at line " + to_string(line) + "\n";
                cout << "ERROR: Unknown Symbol as operand at line " << line << endl;
            }
        }
        return hex_string;
    };

    // Handling label instructions
    if (instr.find(':') != string::npos)
    {
        int colon = instr.find(":", 0);

        // Instruction could be present after the colon
        if (colon != instr.length() - 1)
        {
            string subs = instr.substr(colon + 1);
            subs = trim(subs);
            tie(encoding_, errors, machine_code) = inst_to_code(subs, &loc, line);
            
            // Ensure encoding_ has enough length before taking the substring
            if (encoding_.length() >= 18) {
                encoding += encoding_.substr(9, 9);  // Extract 9 hex characters from the encoded part
            } else {
                encoding += "         "; // Placeholder if encoding_ is too short
            }
        }
        // If no instruction after colon, PC shouldn't change
        else
        {
            encoding += "         ";
            *loc_ptr = *loc_ptr - 1;
        }

        encoding += instr + "\n";
    }
    else
    {
        // Instruction without label
        int space = instr.find(" ");
        string sub_operation = instr.substr(0, space);
        string sub_operand = (space != string::npos) ? instr.substr(space + 1) : "";

        sub_operand = trim(sub_operand);
        sub_operation = trim(sub_operation);

        // Checking for invalid mnemonics
        if (mot[sub_operation] == "")
        {
            errors += "ERROR: Mnemonic not defined at line " + to_string(line) + "\n";
            cout << "ERROR: Mnemonic not defined at line " << line << endl;
        }
        else if (sub_operation == "add" || sub_operation == "sub" || sub_operation == "shl" || sub_operation == "shr" || 
                 sub_operation == "a2sp" || sub_operation == "sp2a" || sub_operation == "return" || sub_operation == "HALT")
        {
            encoding += "000000" + mot[sub_operation] + " ";
            machine_code += "000000" + mot[sub_operation] + " ";
            
            if (sub_operation.length() != instr.length())
            {
                errors += "ERROR: Operand not expected at line " + to_string(line) + "\n";
                cout << "ERROR: Operand not expected at line " << line << endl;
            }
        }
        else
        {
            // For operands that need processing
            string hex_string = process_operand(sub_operand, sub_operation);
            
            // Ensure hex_string has enough length before taking the substring
            if (hex_string.length() >= 6) {
                encoding += hex_string.substr(hex_string.length() - 6) + mot[sub_operation] + " ";
                machine_code += hex_string.substr(hex_string.length() - 6) + mot[sub_operation] + " ";
            } else {
                encoding += "      " + mot[sub_operation] + " "; // Placeholder if hex_string is too short
                machine_code += "      " + mot[sub_operation] + " "; // Placeholder if hex_string is too short
            }
        }

        encoding += instr + "\n";
    }

    return make_tuple(encoding, errors, machine_code);
}



void process_line(const string& line, int* loc_ptr, int line_count, ofstream& outfile, ofstream& logfile, ofstream& objfile) {
    string instr = line.substr(0, line.find(";"));
    instr = trim(instr);

    if (instr.empty()) {
        return; // Skip empty lines
    }

    string encoding, errors, machine_code;
    tie(encoding, errors, machine_code) = inst_to_code(instr, loc_ptr, line_count);
    
    // Write encoding to output files
    outfile << encoding;
    logfile << errors;

    if (!machine_code.empty()) {
        uint32_t temp = stoul("0x" + trim(machine_code), nullptr, 16);
        objfile.write(reinterpret_cast<char*>(&temp), sizeof(temp)); // Write to objfile
    }
}

void synthesize(string file, ofstream& outfile, ofstream& logfile, ofstream& objfile)
{
    string line;
    int loc = 0;
    int line_count = 1;

    ifstream input_file(file);  // Open the input file for reading
    if (!input_file) {
        cerr << "Error: Unable to open file " << file << endl;
        return;
    }

    // Read and process individual lines from the file
    while (getline(input_file, line)) {
        process_line(line, &loc, line_count++, outfile, logfile, objfile);
        loc++;  // Increment Program Counter after processing each line
    }

    input_file.close();  // Ensure the file is closed after reading
}



int main(int argc, char* argv[]) 
{
    // Check for correct number of arguments
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <input_file>" << endl;
        return 1;
    }

    // Initialize Machine Operation Table
    mot_init();

    // Argument for input file
    string in_file = argv[1];

    // Argument for output file (Listing file)
    string out_file = in_file.substr(0, in_file.find(".", 0)) + ".L";

    // Argument for log file
    string log_file = in_file.substr(0, in_file.find(".", 0)) + ".log";

    // Argument for object file
    string obj_file = in_file.substr(0, in_file.find(".", 0)) + ".o";

    // Defining output listing, log file, and object file streams
    ofstream outfile(out_file);
    ofstream logfile(log_file);
    ofstream objfile(obj_file, ios::out | ios::binary);

    // Check if files are open successfully
    if (!outfile.is_open()) {
        cerr << "Error opening output file: " << out_file << endl;
        return 1;
    }
    if (!logfile.is_open()) {
        cerr << "Error opening log file: " << log_file << endl;
        return 1;
    }
    if (!objfile.is_open()) {
        cerr << "Error opening object file: " << obj_file << endl;
        return 1;
    }

    // Pass-1 of assembly (analysis phase)
    analyse(in_file, logfile);

    // Pass-2 of assembly (synthesis phase)
    synthesize(in_file, outfile, logfile, objfile);

    // Close files after processing
    outfile.close();
    logfile.close();
    objfile.close();

    cout << "Assembly complete!" << endl;

    return 0;
}