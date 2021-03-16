
CC=g++
CFLAGS=-std=c++11 -lpthread
CUR_DIR=$(PWD)
COMMON_DIR=$(CUR_DIR)/common 

INCLUDE=-I${COMMON_DIR}

SRC=${wildcard ${COMMON_DIR}/*.c} \
    ${wildcard ${CUR_DIR}/*.c}

obj=${patsubst %.c,%.o,${SRC}}

target=a.out

all:$(target)

$(target):$(obj)
	$(CC) $(obj) $(INCLUDE)  $(CFLAGS) 



.PHONY: clean
clean:
	@echo "hello world"
	@echo ${COMMON_DIR}
	@#@rm *.o
