CSRC = $(wildcard vega-test-master/$(TEST)/*.c)
SSRC = $(wildcard vega-test-master/$(TEST)/*.S)

TEST_OBJ = $(wildcard vega-test-master/$(TEST)/*.o)
LIB_OBJ = $(wildcard vega-test-master/lib/*.o)

CFLAGS = -Ivega-test-master/lib

ifeq ($(SEMIHOSTING),1)
CFLAGS += -DCONFIG_SEMIHOSTING
SSRC += vega-test-master/lib/semih.S
endif

include Makefile

clean::
	rm -f $(TEST_OBJ) $(LIB_OBJ)
