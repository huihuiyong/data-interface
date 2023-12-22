CC = gcc
# CC = aarch64-linux-gnu-gcc
# CC = /root/gcc-linaro-7.5.0-2019.12-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-gcc
CFLAGS = -I./include

# 源文件列表
SOURCES = $(wildcard *.c)

# 生成的可执行文件列表
TARGETS = $(SOURCES:.c=)

# 默认目标，编译所有源文件
all: $(TARGETS)

# 编译规则，每个源文件对应一个可执行文件
%: %.c
	$(CC) $(CFLAGS) -o $@ $< lib/lib.c -lpthread

# 清理规则
clean:
	rm -f $(TARGETS)
