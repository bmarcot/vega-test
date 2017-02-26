CSRC = $(wildcard test/$(TEST)/*.c)
SSRC = $(wildcard test/$(TEST)/*.S)

TEST_OBJ = $(wildcard test/$(TEST)/*.o)
LIB_OBJ = $(wildcard test/lib/*.o)

CFLAGS = -Itest/lib

# Build a romFS drive if there is a /home directory at the root
# of the testcase directory.
ifneq ("$(wildcard test/$(TEST)/home)","")
OBJS = test/$(TEST)/sda1.o
endif

include Makefile

# resources:
#   http://stackoverflow.com/questions/17265950/linking-arbitrary-data-using-gcc-arm-toolchain
#   http://www.atmel.com/webdoc/AVRLibcReferenceManual/FAQ_1faq_binarydata.html

%sda1:
	$(VECHO) "GENROMFS $@"
	$(Q)genromfs -f test/$(TEST)/sda1 -d test/$(TEST)/home -V sda1

%sda1.o: %sda1
	$(VECHO) "BUILDFS\t$@"
	$(Q)$(OBJCOPY) -I binary -O elf32-littlearm -B arm		\
	--rename-section .data=.rodata					\
        --redefine-sym _binary_$(subst /,_,$<)_start=_binary_sda1_start	\
        --redefine-sym _binary_$(subst /,_,$<)_end=_binary_sda1_end	\
        --redefine-sym _binary_$(subst /,_,$<)_size=_binary_sda1_size	\
	test/$(TEST)/sda1 test/$(TEST)/sda1.o

clean::
	rm -f $(TEST_OBJ) $(LIB_OBJ) test/$(TEST)/sda1 test/$(TEST)/sda1.o
