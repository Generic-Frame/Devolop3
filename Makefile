#	$@ 目标文件，$^ 所有的依赖文件，$< 第一个依赖文件
#	-DSLAVE

X86 = gcc

MIPS = mips-linux-gnu-gcc

LIBS += -lpthread -lrt

INCLUDE += -I./include

CFLAGS += -std=gnu99 -DSLAVE

ifneq (,$(findstring -DSLAVE,$(CFLAGS)))
	CC = $(MIPS)
else
	CC = $(X86)
endif

src +=  $(wildcard ./*.c)
src +=  $(wildcard ./applicant/*.c)
src +=  $(wildcard ./business/*.c)
src +=  $(wildcard ./component/*.c)
src +=  $(wildcard ./driver/*.c)


main.out : $(src)

	$(CC) $^ -o $@   $(LIBS) $(INCLUDE) $(CFLAGS)
clean:
	rm main.out -f
