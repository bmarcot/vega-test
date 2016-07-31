CSRC = $(wildcard test/$(TEST)/*.c)
SSRC = $(wildcard test/$(TEST)/*.S)

TEST_OBJ = $(wildcard test/$(TEST)/*.o)
LIB_OBJ = $(wildcard test/lib/*.o)

CFLAGS = -Itest/lib

ifeq ($(SEMIHOSTING),1)
CFLAGS += -DCONFIG_SEMIHOSTING
SSRC += test/lib/semih.S
endif

include Makefile

clean::
	rm -f $(TEST_OBJ) $(LIB_OBJ)
