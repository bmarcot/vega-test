CSRC = $(wildcard vega-test/$(TEST)/*.c)
SSRC = $(wildcard vega-test/$(TEST)/*.S)

TEST_OBJ = $(wildcard vega-test/$(TEST)/*.o)
LIB_OBJ = $(wildcard vega-test/lib/*.o)

CFLAGS = -Ivega-test/lib

ifeq ($(SEMIHOSTING),1)
CFLAGS += -DCONFIG_SEMIHOSTING
SSRC += vega-test/lib/semih.S
endif

include Makefile

clean::
	rm -f $(TEST_OBJ) $(LIB_OBJ)
