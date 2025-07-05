CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -g

# 目标文件
TARGET = toyc_lexer
OBJS = main.o lexer.o

# 默认目标
all: $(TARGET)

# 链接目标
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# 编译main.cpp
main.o: main.cpp lexer.h
	$(CXX) $(CXXFLAGS) -c main.cpp

# 编译lexer.cpp
lexer.o: lexer.cpp lexer.h
	$(CXX) $(CXXFLAGS) -c lexer.cpp

# 清理
clean:
	rm -f $(OBJS) $(TARGET)

# 测试
test: $(TARGET)
	./$(TARGET)

# 创建示例ToyC文件并测试
test-file: $(TARGET) example.tc
	./$(TARGET) example.tc

example.tc:
	@echo 'int fibonacci(int n) {' > example.tc
	@echo '    if (n <= 1) {' >> example.tc
	@echo '        return n;' >> example.tc
	@echo '    }' >> example.tc
	@echo '    return fibonacci(n - 1) + fibonacci(n - 2);' >> example.tc
	@echo '}' >> example.tc
	@echo '' >> example.tc
	@echo 'int main() {' >> example.tc
	@echo '    int result = fibonacci(10);' >> example.tc
	@echo '    while (result > 0) {' >> example.tc
	@echo '        result = result - 1;' >> example.tc
	@echo '        if (result == 55) {' >> example.tc
	@echo '            break;' >> example.tc
	@echo '        }' >> example.tc
	@echo '    }' >> example.tc
	@echo '    return result;' >> example.tc
	@echo '}' >> example.tc

.PHONY: all clean test test-file 