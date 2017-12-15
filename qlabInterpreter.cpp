#include <iostream>
#include <vector>
#include <cstdint>
#include <fstream>
#include <stdexcept>
#include <regex>

// debug switch
#define _DBG_ 0

// imaginary opcodes
#define PUSH    0xf1
#define DUP     0xf2 
#define ADD     0xf3
#define MUL     0xf4
#define READ    0xf5
#define WRITE   0xf6

/**
 * @brief Finds and returns an int inside another string
 * @param _line The string we will search in
 * @return Returns the integer
 */
int64_t findInt(std::string _line) {
  std::regex number_("[0-9]+");
  std::smatch result_;
  std::string stringint_;
  if (std::regex_search(_line, result_, number_)) {
    return std::stoi(result_.str());
  }
  return -1;
}

/**
 * @brief Translates the text format to the imaginary binary format
 * @param _path The path to the text file
 * @return Returns the bytecode
 */
std::vector<uint8_t> text2hex(std::string _path) {
  std::ifstream _textfile;
  _textfile.open(_path);
  std::string line_;
  std::vector<uint8_t> ret_;

  while(std::getline(_textfile, line_)) {
    int64_t immediate_ = 0;
    if (line_.find("PUSH") != std::string::npos) {
      ret_.push_back(PUSH);
      immediate_ = findInt(line_);
      if (immediate_ > 0) {
        ret_.push_back(uint32_t(immediate_) & 0xff000000);
        ret_.push_back(uint32_t(immediate_) & 0x00ff0000);
        ret_.push_back(uint32_t(immediate_) & 0x0000ff00);
        ret_.push_back(uint32_t(immediate_) & 0x000000ff);
      } else {
        std::cout << "bad immediate to PUSH\n";
      }
    }
    else if (line_ == "DUP") {ret_.push_back(DUP);}
    else if (line_ == "ADD") {ret_.push_back(ADD);}
    else if (line_ == "MUL") {ret_.push_back(MUL);}
    else if (line_ == "READ") {ret_.push_back(READ);}
    else if (line_ == "WRITE") {ret_.push_back(WRITE);}
    else {throw std::runtime_error("unknown instruction menmonic.\n");}
  }

  return ret_;
}

/**
 * @brief The interpreter class
 */
class Interpreter {
  public:
    Interpreter(std::vector<uint8_t> _bytecode) : bytecode_(_bytecode) {}
    ~Interpreter() {}

    /**
     * @brief Start interpreting the bytecode
     */
    void run(void) {
      for (int i = 0; i < bytecode_.size(); ++i) {
        switch (bytecode_[i]) {
          case PUSH:
            dummy_reg1 = bytecode_[i + 1];
            dummy_reg1 = (dummy_reg1 << 8) | bytecode_[i + 2];
            dummy_reg1 = (dummy_reg1 << 8) | bytecode_[i + 3];
            dummy_reg1 = (dummy_reg1 << 8) | bytecode_[i + 4];
            stack_.push_back(dummy_reg1);
            i = i + 4;
            break;
          case DUP:
            this->checkStack(1);
            dummy_reg1 = stack_.back();
            stack_.push_back(dummy_reg1);
            break;
          case ADD:
            this->checkStack(2);
            dummy_reg2 = stack_.back(), stack_.pop_back();
            dummy_reg1 = stack_.back(), stack_.pop_back();
            stack_.push_back(uint32_t(dummy_reg1 + dummy_reg2));
            break;
          case MUL:
            this->checkStack(2);
            dummy_reg2 = stack_.back(), stack_.pop_back();
            dummy_reg1 = stack_.back(), stack_.pop_back();
            stack_.push_back(uint32_t(dummy_reg1 + dummy_reg2));
            break;
          case READ:
            std::cin >> dummy_reg1;
            stack_.push_back(dummy_reg1);
            break;
          case WRITE:
            this->checkStack(1);
            dummy_reg1 = stack_.back(), stack_.pop_back();
            std::cout << dummy_reg1 << "\n";
            break;
          default:
            throw std::runtime_error("unknown opcode.\n");
            break;
        }
      }
    }

    /**
     * @brief Dump the bytecode the interpreter is interpreting. meant for debugging.
     */
    void dumpBytecode(void) {
      std::cout << "start of bytecode dump:\n";
      for (auto &iter : bytecode_) {std::cout << int(iter) << "\n";}
      std::cout << "end of bytecode dump.\n";
    }

    /**
     * @brief Checks whether there are enough entries in the stack to pop
     * @param _stack_height The number to check against
     * @return returns true if there are enough elements in the stack otherwise false
     */
    bool checkStack(int _stack_height) {
      if ((stack_.size() / 4) < _stack_height) return false;
      else return true;
    }

  private:
    std::vector<uint8_t> bytecode_;
    std::vector<uint32_t> stack_;
    uint32_t dummy_reg1;
    uint32_t dummy_reg2;
};

int main(int argc, char** argv) {
  if (argc != 2) {std::cout << "did not provide an input file to ran on...";}
  else {
    Interpreter interpreter(text2hex(argv[1]));
    interpreter.run();
#if _DBG_
    interpreter.dumpBytecode();
#endif
  }
  return 0;
}
